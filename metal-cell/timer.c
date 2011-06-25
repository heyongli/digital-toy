
#include <timer.h>



volatile unsigned  long jiffers=0;  //250us per jiffers



#ifdef HAVE_METAL_DELAY
/*0-64 seconds*/
void delay(unsigned long ticks)
{
  
   unsigned long lt = jiffers;

   while(!timeafter(jiffers,(lt+ ticks)));
   
   return;

}

#endif
