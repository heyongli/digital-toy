#include <linux/kernel.h>

#include <linux/module.h>
#include <linux/types.h>
#include <linux/timer.h>

#include <linux/interrupt.h>
#include <linux/fs.h>
#include <linux/cdev.h>

#include <asm/io.h>  //don't use <asm-generic/io.h>.it's fake



static int  ppc_dump = 5;
module_param(ppc_dump, int, 0);
MODULE_PARM_DESC(ppc_dump,	"dump PPC cpu infomation " __MODULE_STRING(ppc_dump));


static struct cdev *ppc_dump_dev=NULL;
static int  ppc_dump_maj = 0;

char  dbuf[4096]; /*Collection infomation to this buf, then dump to user space*/
long  dbuf_len = 0;

#define dump(fmt, ...)  \
	do {  dbuf_len+=sprintf(dbuf+dbuf_len,fmt,##__VA_ARGS__); } while (0)



void ppc_cpu_dump(void)
{

	void *vir=0;
	int i,ddr,cs;
	dbuf_len = 0;
	//Memory Map ==============================================
	//CCSR: Memory Map Configuration, Control, and Status Registers,  ocupy 16M mem map
	// 		AT CCSR offset 0,is the CCSRBAR register, which define CCSR regs blok's base address
	//		CCSRBAR default value is : 0x0_FE00_0000

	//CCSRBAR: all memory map start from this SOURCE....
	// refer to u-boot bdinfo, immr_base, that the CCSRBAR address take effective, it's default normally
	#define CCSRBAR_PHYS		0xffe000000ull /* physical addr of CCSRBAR, remap by u-boot */
	#define CCSARBARH  0
	#define CCSARBARL  4

	#define CCSRBASE vir
	#define ccsr_read(offset)  (*((unsigned int*)((char*)CCSRBASE+offset)))
	
	vir=ioremap(CCSRBAR_PHYS, 16*1024*1024); //CCSR 16M 
	
	
	dump("############# CCSRBAR, PHYS: 0x%llX ###############\n", CCSRBAR_PHYS);
	dump("** virtaul map to: 0x%llx\n",(unsigned long long)vir);
	dump("	CCSARBAR  LOW: 0x%X\n", ccsr_read(CCSARBARL));
	dump("	CCSARBAR HIGH: 0x%X\n", ccsr_read(CCSARBARH));

	//CCSR: Boot windows
	//BOOT WINDOW:  MMU has one 4-Kbyte page defined at 0x0_FFFF_Fnnn after reset. cpu execute the 
	//		instruction from 0x0_FFFF_FFFC. (burst read start from 0x0_FFFF_FFE0)
	// pre-boot load can enable boot space translation via Regs: BSTRH, BSTRL
	// 		if translate to out of default boot window (8 Mbytes at 0x0_FF80_0000 to 0x0_FFFF_FFFF),preBoot
	//		loader must setup targetID also.
	#define BSTRH 0x20
	#define BSTRL 0x24
	#define  BSTAR 0x28
	dump("------Boot translation---------\n");
	dump("	BSTR    LOW:0x%x\n", ccsr_read(BSTRL));
	dump("	BSTR   HIGH:0x%x\n", ccsr_read(BSTRH));
	dump("	BSTAR         :0x%x\n", ccsr_read(BSTAR));
	dump("	Boot Translation is ");
	if(ccsr_read(BSTAR) | 0x80000000)
		dump("enabled, target id:0x%x\n", (ccsr_read(BSTAR)>>20)&0xFF);
	else
		dump("disabled\n");

	dump("\n");
	//CCSR: LAWs  local acess windows
	#define LAW_BASE 0x0C00
	#define LAWBARH(n)  (LAW_BASE + n*0x10)
	#define LAWBARL(n)  (LAW_BASE + n*0x10+4)
	#define LAWAR(n)     (LAW_BASE + n*0x10+8)
	#define LAW_EN  0x80000000
	dump("------Local Acess Windows BAR\n");
	dump("--Refer to: Global Source and Target IDs\n");
	for(i=0; i<32; i++){
		#define LAWSIZE  (1LL<<((ccsr_read(LAWAR(i))&0x3F)+1))
		if(ccsr_read(LAWAR(i))&LAW_EN){
			dump("----ID:%d  high:low  0x%x:0x%x\n",i,ccsr_read(LAWBARH(i)),
														ccsr_read(LAWBARL(i)));
			dump("	LAWAR :0x%x \n", ccsr_read(LAWAR(i)));
			if(LAWSIZE>=1024*1024)
			dump("	TargetID:0x%x,  SIZE:%lldM  CSD_ID:0x%x\n",(ccsr_read(LAWAR(i))>>20)&0xFF,
					LAWSIZE/1024/1024,  (ccsr_read(LAWAR(i))>>12)&0xFF);
			else 
			dump("	TargetID:0x%x,  SIZE:%lldk  CSD_ID:0x%x\n",(ccsr_read(LAWAR(i))>>20)&0xFF,
					LAWSIZE/1024,  (ccsr_read(LAWAR(i))>>12)&0xFF);
			
				
		}
	}
	dump("\n");
	//CCSR: DDR
	//DDR basic:   a DDR chip might have 4/8/16 data pin
	//			DDR address line is multiplex: active command send Row address
	//			r/w command receive the column address
	//A 128M bits might represent by: 2M x 16 x 4  (bank size + bits(data line num) + bank)
	// 			might means one bank(2M) contains: 4096RowX512colonmx16 i/o line(data pin) 
	#define DDR_BASE(n) (0x8000+n*0x1000)
	#define DDR_CSn_BNDS(ddr, cs) (DDR_BASE(ddr)+cs*0x8)
	#define DDR_CSn_CFG(ddr, cs) (DDR_BASE(ddr)+0x80+cs*0x4)
	#define CSn_EN 0x80000000
	#define DDR_CSn_CFG2(ddr, cs) (DDR_BASE(ddr)+0xC0+cs*0x4)
	#define DDR_TIMING_CFG3(ddr)  (DDR_BASE(ddr)+0x100)
	#define DDR_TIMING_CFG(ddr, n)  (DDR_BASE(ddr)+0x104+n*0x4)
	//DDR 
	for(ddr=0;ddr<2;ddr++){
		unsigned int timing0=ccsr_read(DDR_TIMING_CFG(ddr,0));
		unsigned int timing1=ccsr_read(DDR_TIMING_CFG(ddr,1));
		unsigned int timing2=ccsr_read(DDR_TIMING_CFG(ddr,2));
		unsigned int timing3=ccsr_read(DDR_TIMING_CFG3(ddr));

		unsigned int tRAS = ((timing1>>24)&0xF)+0x10*((timing3>>24)&0x1);
		unsigned int tRFC = ((timing1>>12)&0xF) + 8 + 0x10*((timing3>>16)&0x1F);
		unsigned int tCAS_read= ((timing1>>16)&0xF) + 0x8*((timing3>>12)&0x1);
		unsigned int control_adjust = timing3&0x7;
			
		//DDR timing config
		dump("\n---------------------------\n");
		dump("DDR %d  timing 0:0x%x timing 1:0x%x  timing 2:0x%x  timing 3:0x%x \n",
				ddr, timing0,timing1,timing2,timing3);
		dump("tRAS:%d  tRFC:%d, tCAS_read:%d  control_adjust:0x%x \n",tRAS,tRFC, tCAS_read, 
							control_adjust);
		
		// Chip select config
		for(cs=0; cs<4; cs++){
			unsigned int cfg= ccsr_read(DDR_CSn_CFG(ddr,cs));
			unsigned int cfg2= ccsr_read(DDR_CSn_CFG2(ddr,cs));
			unsigned long long bnd=ccsr_read(DDR_CSn_BNDS(ddr,cs));
			dump("-------\n");
			dump("Chip %d : cfg: 0x%x  cfg2:0x%x cs_bnd:0x%llx", cs, cfg, cfg2, bnd);
			dump("	[0x%llx ----- 0x%llx] \n", (bnd&0xfff0000)<<8, (bnd&0xFFF)<<24);	

			if(cfg&CSn_EN){
				//---- cfg ------------
				dump("***");
				#define INTLV_EN 0x30000000
				#define AP_n_EN  0x00800000
				
				if(cfg&INTLV_EN){
					dump(" INTLV-");  
					switch((cfg>>24)&0xF){
						case 0:
							dump("cache-line ");
							break;
						case 1:
							dump("Page ");
							break;
						default:
							dump("bank ");

					}	
				}
				if(cfg&AP_n_EN) 
						dump(" auto-precharge ");
				dump(" read-ODT_cfg:0x%x  write-ODT_cfg:0x%x ",(cfg>>20)&0x7, (cfg>>16)&0x7);
				dump(" BankBits:%d, Rowbits:%d, ColBits:%d ",((cfg>>14)&0x3)+2, ((cfg>>8)&0x7)+12, (cfg&0x7)+8);
				//-------cfg2-----------
				if((cfg2>>24)&0x7)
					dump(" Partial Array ");

				dump("\n");
			}

		}

	}
	
	iounmap(vir);
}




int ppc_dump_open(struct inode *inode, struct file *filp)
{
	struct cdev *dev; /* device information */
	dev =  inode->i_cdev;

	//printk("xmod Open....\n");
	filp->private_data = dev; /* for other methods */
	/* now trim to 0 the length of the device if open was write-only */
	if ( (filp->f_flags & O_ACCMODE) == O_WRONLY) {
		//self trim...
	}
	return 0;
}





static ssize_t ppc_dump_read(struct file *file, char __user *buf,
		    size_t nbytes, loff_t *ppos)
{

       ppc_cpu_dump();
	
	return simple_read_from_buffer(buf, nbytes, ppos, dbuf,dbuf_len);
}

static ssize_t ppc_dump_write(struct file *file, const char __user *buf,
		    size_t nbytes, loff_t *ppos)
{
	
	return 0;//simple_read_from_buffer(buf, nbytes, ppos, xx,strlen(xx));
}


	
struct file_operations ppc_dump_fops = {
	.owner = THIS_MODULE,
	.llseek = no_llseek,
	.read = ppc_dump_read,
	.write = ppc_dump_write,
	.ioctl = NULL,
	.open = ppc_dump_open,
	.release = NULL,
};

static int __init ppc_dump_init(void)
{
	int ret;
	printk("Xmod init...\n");
	
	ppc_dump_dev = cdev_alloc();
	cdev_init(ppc_dump_dev,&ppc_dump_fops);
	ppc_dump_dev->owner = THIS_MODULE;
	ppc_dump_dev->ops = &ppc_dump_fops;
	
	alloc_chrdev_region(&ppc_dump_maj, 0,1, "xmod_dev");
	printk("major:%d  minor:%d \n", MAJOR(ppc_dump_maj),MINOR(ppc_dump_maj));

	ret = cdev_add(ppc_dump_dev,ppc_dump_maj,1); 
	if(ret)
		printk("cdev init failed %d\n", ret);
	return 0;
	
}

static void __init ppc_dump_exit(void)
{

	pr_debug("Xmod 88..\n");
	cdev_del(ppc_dump_dev);
	unregister_chrdev_region(MKDEV(MAJOR(ppc_dump_maj),0),1);
}


module_init(ppc_dump_init);
module_exit(ppc_dump_exit);

MODULE_AUTHOR("Yongli He");
MODULE_DESCRIPTION("Dump PPC CPU infomation");
MODULE_LICENSE("GPL");
