cmd_/home/yhe/work/ppc-dump-mod/ppc-dump.o := /home/yhe/work/4.3-sync-5020-std/host-cross/powerpc-wrs-linux-gnu/x86-linux2/powerpc-wrs-linux-gnu-gcc -m64 -Wp,-MD,/home/yhe/work/ppc-dump-mod/.ppc-dump.o.d  -nostdinc -isystem /home/yhe/work/wrlinux4.1/layers/wrll-toolchain-4.4a-340/powerpc/toolchain/x86-linux2/bin/../lib/gcc/powerpc-wrs-linux-gnu/4.4.1/include -I/home/yhe/work/4.3-sync-5020-std/build/linux/arch/powerpc/include -Iinclude  -I/home/yhe/work/4.3-sync-5020-std/build/linux/include -include include/generated/autoconf.h   -I/home/yhe/work/ppc-dump-mod -D__KERNEL__  -I/home/yhe/work/4.3-sync-5020-std/build/linux/arch/powerpc -Iarch/powerpc -Wall -Wundef -Wstrict-prototypes -Wno-trigraphs -fno-strict-aliasing -fno-common -Werror-implicit-function-declaration -Wno-format-security -fno-delete-null-pointer-checks -O2 -msoft-float -pipe  -I/home/yhe/work/4.3-sync-5020-std/build/linux/arch/powerpc -Iarch/powerpc -mminimal-toc -mtraceback=none -mcall-aixdesc -mtune=power4 -mno-altivec -mno-spe -mspe=no -funit-at-a-time -mno-string -mno-sched-epilog -Wa,-me500 -mcpu=e5500 -Wframe-larger-than=2048 -fno-stack-protector -fno-omit-frame-pointer -fno-optimize-sibling-calls -pg -Wdeclaration-after-statement -Wno-pointer-sign -fno-strict-overflow -fno-dwarf2-cfi-asm -fconserve-stack  -DMODULE -D"KBUILD_STR(s)=\#s" -D"KBUILD_BASENAME=KBUILD_STR(ppc_dump)"  -D"KBUILD_MODNAME=KBUILD_STR(ppc_dump)"  -c -o /home/yhe/work/ppc-dump-mod/ppc-dump.o /home/yhe/work/ppc-dump-mod/ppc-dump.c

deps_/home/yhe/work/ppc-dump-mod/ppc-dump.o := \
  /home/yhe/work/ppc-dump-mod/ppc-dump.c \
  /home/yhe/work/4.3-sync-5020-std/build/linux/include/linux/kernel.h \
    $(wildcard include/config/lbdaf.h) \
    $(wildcard include/config/preempt/voluntary.h) \
    $(wildcard include/config/debug/spinlock/sleep.h) \
    $(wildcard include/config/prove/locking.h) \
    $(wildcard include/config/printk.h) \
    $(wildcard include/config/dynamic/debug.h) \
    $(wildcard include/config/ring/buffer.h) \
    $(wildcard include/config/tracing.h) \
    $(wildcard include/config/numa.h) \
    $(wildcard include/config/ftrace/mcount/record.h) \
  /home/yhe/work/wrlinux4.1/layers/wrll-toolchain-4.4a-340/powerpc/toolchain/x86-linux2/bin/../lib/gcc/powerpc-wrs-linux-gnu/4.4.1/include/stdarg.h \
  /home/yhe/work/4.3-sync-5020-std/build/linux/include/linux/linkage.h \
  /home/yhe/work/4.3-sync-5020-std/build/linux/include/linux/compiler.h \
    $(wildcard include/config/trace/branch/profiling.h) \
    $(wildcard include/config/profile/all/branches.h) \
    $(wildcard include/config/enable/must/check.h) \
    $(wildcard include/config/enable/warn/deprecated.h) \
  /home/yhe/work/4.3-sync-5020-std/build/linux/include/linux/compiler-gcc.h \
    $(wildcard include/config/arch/supports/optimized/inlining.h) \
    $(wildcard include/config/optimize/inlining.h) \
  /home/yhe/work/4.3-sync-5020-std/build/linux/include/linux/compiler-gcc4.h \
  /home/yhe/work/4.3-sync-5020-std/build/linux/arch/powerpc/include/asm/linkage.h \
  /home/yhe/work/4.3-sync-5020-std/build/linux/include/linux/stddef.h \
  /home/yhe/work/4.3-sync-5020-std/build/linux/include/linux/types.h \
    $(wildcard include/config/uid16.h) \
    $(wildcard include/config/phys/addr/t/64bit.h) \
    $(wildcard include/config/64bit.h) \
  /home/yhe/work/4.3-sync-5020-std/build/linux/arch/powerpc/include/asm/types.h \
    $(wildcard include/config/phys/64bit.h) \
  /home/yhe/work/4.3-sync-5020-std/build/linux/include/asm-generic/int-ll64.h \
  /home/yhe/work/4.3-sync-5020-std/build/linux/arch/powerpc/include/asm/bitsperlong.h \
  /home/yhe/work/4.3-sync-5020-std/build/linux/include/asm-generic/bitsperlong.h \
  /home/yhe/work/4.3-sync-5020-std/build/linux/include/linux/posix_types.h \
  /home/yhe/work/4.3-sync-5020-std/build/linux/arch/powerpc/include/asm/posix_types.h \
  /home/yhe/work/4.3-sync-5020-std/build/linux/include/linux/bitops.h \
    $(wildcard include/config/generic/find/first/bit.h) \
    $(wildcard include/config/generic/find/last/bit.h) \
    $(wildcard include/config/generic/find/next/bit.h) \
  /home/yhe/work/4.3-sync-5020-std/build/linux/arch/powerpc/include/asm/bitops.h \
  /home/yhe/work/4.3-sync-5020-std/build/linux/arch/powerpc/include/asm/asm-compat.h \
    $(wildcard include/config/power4/only.h) \
    $(wildcard include/config/ibm405/err77.h) \
  /home/yhe/work/4.3-sync-5020-std/build/linux/arch/powerpc/include/asm/ppc-opcode.h \
    $(wildcard include/config/ppc64.h) \
  /home/yhe/work/4.3-sync-5020-std/build/linux/include/linux/stringify.h \
  /home/yhe/work/4.3-sync-5020-std/build/linux/arch/powerpc/include/asm/synch.h \
    $(wildcard include/config/ppc/e500mc.h) \
    $(wildcard include/config/e500.h) \
    $(wildcard include/config/smp.h) \
  /home/yhe/work/4.3-sync-5020-std/build/linux/arch/powerpc/include/asm/feature-fixups.h \
  /home/yhe/work/4.3-sync-5020-std/build/linux/include/asm-generic/bitops/non-atomic.h \
  /home/yhe/work/4.3-sync-5020-std/build/linux/include/asm-generic/bitops/hweight.h \
  /home/yhe/work/4.3-sync-5020-std/build/linux/include/asm-generic/bitops/find.h \
  /home/yhe/work/4.3-sync-5020-std/build/linux/include/asm-generic/bitops/sched.h \
  /home/yhe/work/4.3-sync-5020-std/build/linux/include/linux/log2.h \
    $(wildcard include/config/arch/has/ilog2/u32.h) \
    $(wildcard include/config/arch/has/ilog2/u64.h) \
  /home/yhe/work/4.3-sync-5020-std/build/linux/include/linux/typecheck.h \
  /home/yhe/work/4.3-sync-5020-std/build/linux/include/linux/dynamic_debug.h \
  /home/yhe/work/4.3-sync-5020-std/build/linux/arch/powerpc/include/asm/byteorder.h \
  /home/yhe/work/4.3-sync-5020-std/build/linux/include/linux/byteorder/big_endian.h \
  /home/yhe/work/4.3-sync-5020-std/build/linux/include/linux/swab.h \
  /home/yhe/work/4.3-sync-5020-std/build/linux/arch/powerpc/include/asm/swab.h \
  /home/yhe/work/4.3-sync-5020-std/build/linux/include/linux/byteorder/generic.h \
  /home/yhe/work/4.3-sync-5020-std/build/linux/arch/powerpc/include/asm/bug.h \
    $(wildcard include/config/bug.h) \
    $(wildcard include/config/debug/bugverbose.h) \
  /home/yhe/work/4.3-sync-5020-std/build/linux/include/asm-generic/bug.h \
    $(wildcard include/config/generic/bug.h) \
    $(wildcard include/config/generic/bug/relative/pointers.h) \
  /home/yhe/work/4.3-sync-5020-std/build/linux/include/linux/module.h \
    $(wildcard include/config/symbol/prefix.h) \
    $(wildcard include/config/modules.h) \
    $(wildcard include/config/modversions.h) \
    $(wildcard include/config/unused/symbols.h) \
    $(wildcard include/config/kallsyms.h) \
    $(wildcard include/config/kgdb/modules.h) \
    $(wildcard include/config/wr/ocd/debug.h) \
    $(wildcard include/config/markers.h) \
    $(wildcard include/config/tracepoints.h) \
    $(wildcard include/config/event/tracing.h) \
    $(wildcard include/config/module/unload.h) \
    $(wildcard include/config/constructors.h) \
    $(wildcard include/config/sysfs.h) \
  /home/yhe/work/4.3-sync-5020-std/build/linux/include/linux/list.h \
    $(wildcard include/config/debug/list.h) \
  /home/yhe/work/4.3-sync-5020-std/build/linux/include/linux/poison.h \
    $(wildcard include/config/illegal/pointer/value.h) \
  /home/yhe/work/4.3-sync-5020-std/build/linux/include/linux/prefetch.h \
  /home/yhe/work/4.3-sync-5020-std/build/linux/arch/powerpc/include/asm/processor.h \
    $(wildcard include/config/vsx.h) \
    $(wildcard include/config/ppc32.h) \
    $(wildcard include/config/paravirt.h) \
    $(wildcard include/config/ppc/prep.h) \
    $(wildcard include/config/task/size.h) \
    $(wildcard include/config/kernel/start.h) \
    $(wildcard include/config/booke.h) \
    $(wildcard include/config/ppc/adv/debug/regs.h) \
    $(wildcard include/config/ppc/adv/debug/iacs.h) \
    $(wildcard include/config/ppc/adv/debug/dvcs.h) \
    $(wildcard include/config/altivec.h) \
    $(wildcard include/config/spe.h) \
  /home/yhe/work/4.3-sync-5020-std/build/linux/arch/powerpc/include/asm/reg.h \
    $(wildcard include/config/40x.h) \
    $(wildcard include/config/fsl/emb/perfmon.h) \
    $(wildcard include/config/8xx.h) \
    $(wildcard include/config/ppc/book3s/64.h) \
    $(wildcard include/config/ppc/book3s/32.h) \
    $(wildcard include/config/6xx.h) \
    $(wildcard include/config/ppc/book3e/64.h) \
    $(wildcard include/config/e200.h) \
    $(wildcard include/config/kprobes.h) \
    $(wildcard include/config/ppc/cell.h) \
  /home/yhe/work/4.3-sync-5020-std/build/linux/arch/powerpc/include/asm/cputable.h \
    $(wildcard include/config/mpc10x/bridge.h) \
    $(wildcard include/config/ppc/83xx.h) \
    $(wildcard include/config/8260.h) \
    $(wildcard include/config/ppc/mpc52xx.h) \
    $(wildcard include/config/bdi/switch.h) \
    $(wildcard include/config/4xx.h) \
    $(wildcard include/config/power3.h) \
    $(wildcard include/config/power4.h) \
    $(wildcard include/config/ppc/book3e.h) \
    $(wildcard include/config/44x.h) \
  /home/yhe/work/4.3-sync-5020-std/build/linux/arch/powerpc/include/asm/reg_booke.h \
    $(wildcard include/config/ppc85xx/vt/mode.h) \
    $(wildcard include/config/403gcx.h) \
  /home/yhe/work/4.3-sync-5020-std/build/linux/arch/powerpc/include/asm/reg_fsl_emb.h \
  /home/yhe/work/4.3-sync-5020-std/build/linux/include/linux/cache.h \
    $(wildcard include/config/arch/has/cache/line/size.h) \
  /home/yhe/work/4.3-sync-5020-std/build/linux/arch/powerpc/include/asm/cache.h \
  /home/yhe/work/4.3-sync-5020-std/build/linux/arch/powerpc/include/asm/ptrace.h \
  /home/yhe/work/4.3-sync-5020-std/build/linux/arch/powerpc/include/asm/system.h \
    $(wildcard include/config/debugger.h) \
    $(wildcard include/config/kexec.h) \
    $(wildcard include/config/booke/wdt.h) \
    $(wildcard include/config/virt/cpu/accounting.h) \
  /home/yhe/work/4.3-sync-5020-std/build/linux/include/linux/irqflags.h \
    $(wildcard include/config/trace/irqflags.h) \
    $(wildcard include/config/irqsoff/tracer.h) \
    $(wildcard include/config/preempt/tracer.h) \
    $(wildcard include/config/trace/irqflags/support.h) \
  /home/yhe/work/4.3-sync-5020-std/build/linux/arch/powerpc/include/asm/irqflags.h \
  /home/yhe/work/4.3-sync-5020-std/build/linux/arch/powerpc/include/asm/hw_irq.h \
    $(wildcard include/config/kvm/guest.h) \
  /home/yhe/work/4.3-sync-5020-std/build/linux/include/linux/errno.h \
  /home/yhe/work/4.3-sync-5020-std/build/linux/arch/powerpc/include/asm/errno.h \
  /home/yhe/work/4.3-sync-5020-std/build/linux/include/asm-generic/errno.h \
  /home/yhe/work/4.3-sync-5020-std/build/linux/include/asm-generic/errno-base.h \
  /home/yhe/work/4.3-sync-5020-std/build/linux/arch/powerpc/include/asm/paca.h \
    $(wildcard include/config/kvm/book3s/64/handler.h) \
    $(wildcard include/config/debug/preempt.h) \
    $(wildcard include/config/ppc/book3s.h) \
    $(wildcard include/config/ppc/std/mmu/64.h) \
  /home/yhe/work/4.3-sync-5020-std/build/linux/include/linux/init.h \
    $(wildcard include/config/hotplug.h) \
  /home/yhe/work/4.3-sync-5020-std/build/linux/arch/powerpc/include/asm/lppaca.h \
  /home/yhe/work/4.3-sync-5020-std/build/linux/arch/powerpc/include/asm/mmu.h \
    $(wildcard include/config/ppc/fsl/book3e.h) \
    $(wildcard include/config/ppc/std/mmu/32.h) \
    $(wildcard include/config/ppc/book3e/mmu.h) \
    $(wildcard include/config/ppc/8xx.h) \
  /home/yhe/work/4.3-sync-5020-std/build/linux/arch/powerpc/include/asm/percpu.h \
  /home/yhe/work/4.3-sync-5020-std/build/linux/include/asm-generic/percpu.h \
    $(wildcard include/config/have/setup/per/cpu/area.h) \
  /home/yhe/work/4.3-sync-5020-std/build/linux/include/linux/threads.h \
    $(wildcard include/config/nr/cpus.h) \
    $(wildcard include/config/base/small.h) \
  /home/yhe/work/4.3-sync-5020-std/build/linux/include/linux/percpu-defs.h \
    $(wildcard include/config/debug/force/weak/per/cpu.h) \
  /home/yhe/work/4.3-sync-5020-std/build/linux/arch/powerpc/include/asm/mmu-book3e.h \
    $(wildcard include/config/wrhv/asid/optimization.h) \
    $(wildcard include/config/ppc/mm/slices.h) \
    $(wildcard include/config/ppc/4k/pages.h) \
    $(wildcard include/config/ppc/64k/pages.h) \
  /home/yhe/work/4.3-sync-5020-std/build/linux/arch/powerpc/include/asm/page.h \
    $(wildcard include/config/ppc/256k/pages.h) \
    $(wildcard include/config/ppc/16k/pages.h) \
    $(wildcard include/config/hugetlb/page.h) \
    $(wildcard include/config/page/offset.h) \
    $(wildcard include/config/physical/start.h) \
    $(wildcard include/config/relocatable.h) \
    $(wildcard include/config/flatmem.h) \
    $(wildcard include/config/ppc/smlpar.h) \
  /home/yhe/work/4.3-sync-5020-std/build/linux/arch/powerpc/include/asm/kdump.h \
    $(wildcard include/config/crash/dump.h) \
  /home/yhe/work/4.3-sync-5020-std/build/linux/arch/powerpc/include/asm/page_64.h \
  /home/yhe/work/4.3-sync-5020-std/build/linux/include/asm-generic/getorder.h \
  /home/yhe/work/4.3-sync-5020-std/build/linux/include/asm-generic/memory_model.h \
    $(wildcard include/config/discontigmem.h) \
    $(wildcard include/config/sparsemem/vmemmap.h) \
    $(wildcard include/config/sparsemem.h) \
  /home/yhe/work/4.3-sync-5020-std/build/linux/arch/powerpc/include/asm/exception-64e.h \
    $(wildcard include/config/book3e/mmu/tlb/stats.h) \
  /home/yhe/work/4.3-sync-5020-std/build/linux/include/linux/stat.h \
  /home/yhe/work/4.3-sync-5020-std/build/linux/arch/powerpc/include/asm/stat.h \
  /home/yhe/work/4.3-sync-5020-std/build/linux/include/linux/time.h \
    $(wildcard include/config/arch/uses/gettimeoffset.h) \
  /home/yhe/work/4.3-sync-5020-std/build/linux/include/linux/seqlock.h \
  /home/yhe/work/4.3-sync-5020-std/build/linux/include/linux/spinlock.h \
    $(wildcard include/config/debug/spinlock.h) \
    $(wildcard include/config/generic/lockbreak.h) \
    $(wildcard include/config/preempt.h) \
    $(wildcard include/config/debug/lock/alloc.h) \
  /home/yhe/work/4.3-sync-5020-std/build/linux/include/linux/preempt.h \
    $(wildcard include/config/preempt/notifiers.h) \
  /home/yhe/work/4.3-sync-5020-std/build/linux/include/linux/thread_info.h \
    $(wildcard include/config/compat.h) \
  /home/yhe/work/4.3-sync-5020-std/build/linux/arch/powerpc/include/asm/thread_info.h \
  /home/yhe/work/4.3-sync-5020-std/build/linux/include/linux/bottom_half.h \
  /home/yhe/work/4.3-sync-5020-std/build/linux/include/linux/spinlock_types.h \
  /home/yhe/work/4.3-sync-5020-std/build/linux/arch/powerpc/include/asm/spinlock_types.h \
  /home/yhe/work/4.3-sync-5020-std/build/linux/include/linux/lockdep.h \
    $(wildcard include/config/lockdep.h) \
    $(wildcard include/config/lock/stat.h) \
    $(wildcard include/config/generic/hardirqs.h) \
    $(wildcard include/config/prove/rcu.h) \
  /home/yhe/work/4.3-sync-5020-std/build/linux/include/linux/rwlock_types.h \
  /home/yhe/work/4.3-sync-5020-std/build/linux/arch/powerpc/include/asm/spinlock.h \
    $(wildcard include/config/ppc/splpar.h) \
    $(wildcard include/config/ppc/iseries.h) \
  /home/yhe/work/4.3-sync-5020-std/build/linux/arch/powerpc/include/asm/hvcall.h \
    $(wildcard include/config/ppc/pseries.h) \
  /home/yhe/work/4.3-sync-5020-std/build/linux/arch/powerpc/include/asm/iseries/hv_call.h \
  /home/yhe/work/4.3-sync-5020-std/build/linux/arch/powerpc/include/asm/iseries/hv_call_sc.h \
  /home/yhe/work/4.3-sync-5020-std/build/linux/arch/powerpc/include/asm/iseries/hv_types.h \
  /home/yhe/work/4.3-sync-5020-std/build/linux/include/linux/rwlock.h \
  /home/yhe/work/4.3-sync-5020-std/build/linux/include/linux/spinlock_api_smp.h \
    $(wildcard include/config/inline/spin/lock.h) \
    $(wildcard include/config/inline/spin/lock/bh.h) \
    $(wildcard include/config/inline/spin/lock/irq.h) \
    $(wildcard include/config/inline/spin/lock/irqsave.h) \
    $(wildcard include/config/inline/spin/trylock.h) \
    $(wildcard include/config/inline/spin/trylock/bh.h) \
    $(wildcard include/config/inline/spin/unlock.h) \
    $(wildcard include/config/inline/spin/unlock/bh.h) \
    $(wildcard include/config/inline/spin/unlock/irq.h) \
    $(wildcard include/config/inline/spin/unlock/irqrestore.h) \
  /home/yhe/work/4.3-sync-5020-std/build/linux/include/linux/rwlock_api_smp.h \
    $(wildcard include/config/inline/read/lock.h) \
    $(wildcard include/config/inline/write/lock.h) \
    $(wildcard include/config/inline/read/lock/bh.h) \
    $(wildcard include/config/inline/write/lock/bh.h) \
    $(wildcard include/config/inline/read/lock/irq.h) \
    $(wildcard include/config/inline/write/lock/irq.h) \
    $(wildcard include/config/inline/read/lock/irqsave.h) \
    $(wildcard include/config/inline/write/lock/irqsave.h) \
    $(wildcard include/config/inline/read/trylock.h) \
    $(wildcard include/config/inline/write/trylock.h) \
    $(wildcard include/config/inline/read/unlock.h) \
    $(wildcard include/config/inline/write/unlock.h) \
    $(wildcard include/config/inline/read/unlock/bh.h) \
    $(wildcard include/config/inline/write/unlock/bh.h) \
    $(wildcard include/config/inline/read/unlock/irq.h) \
    $(wildcard include/config/inline/write/unlock/irq.h) \
    $(wildcard include/config/inline/read/unlock/irqrestore.h) \
    $(wildcard include/config/inline/write/unlock/irqrestore.h) \
  /home/yhe/work/4.3-sync-5020-std/build/linux/arch/powerpc/include/asm/atomic.h \
  /home/yhe/work/4.3-sync-5020-std/build/linux/include/asm-generic/atomic-long.h \
  /home/yhe/work/4.3-sync-5020-std/build/linux/include/linux/math64.h \
  /home/yhe/work/4.3-sync-5020-std/build/linux/arch/powerpc/include/asm/div64.h \
  /home/yhe/work/4.3-sync-5020-std/build/linux/include/asm-generic/div64.h \
  /home/yhe/work/4.3-sync-5020-std/build/linux/include/linux/kmod.h \
  /home/yhe/work/4.3-sync-5020-std/build/linux/include/linux/gfp.h \
    $(wildcard include/config/kmemcheck.h) \
    $(wildcard include/config/highmem.h) \
    $(wildcard include/config/zone/dma.h) \
    $(wildcard include/config/zone/dma32.h) \
    $(wildcard include/config/debug/vm.h) \
  /home/yhe/work/4.3-sync-5020-std/build/linux/include/linux/mmzone.h \
    $(wildcard include/config/force/max/zoneorder.h) \
    $(wildcard include/config/memory/hotplug.h) \
    $(wildcard include/config/arch/populates/node/map.h) \
    $(wildcard include/config/flat/node/mem/map.h) \
    $(wildcard include/config/cgroup/page.h) \
    $(wildcard include/config/no/bootmem.h) \
    $(wildcard include/config/have/memory/present.h) \
    $(wildcard include/config/need/node/memmap/size.h) \
    $(wildcard include/config/need/multiple/nodes.h) \
    $(wildcard include/config/have/arch/early/pfn/to/nid.h) \
    $(wildcard include/config/sparsemem/extreme.h) \
    $(wildcard include/config/nodes/span/other/nodes.h) \
    $(wildcard include/config/holes/in/zone.h) \
    $(wildcard include/config/arch/has/holes/memorymodel.h) \
  /home/yhe/work/4.3-sync-5020-std/build/linux/include/linux/wait.h \
  /home/yhe/work/4.3-sync-5020-std/build/linux/arch/powerpc/include/asm/current.h \
  /home/yhe/work/4.3-sync-5020-std/build/linux/include/linux/numa.h \
    $(wildcard include/config/nodes/shift.h) \
  /home/yhe/work/4.3-sync-5020-std/build/linux/include/linux/nodemask.h \
  /home/yhe/work/4.3-sync-5020-std/build/linux/include/linux/bitmap.h \
  /home/yhe/work/4.3-sync-5020-std/build/linux/include/linux/string.h \
    $(wildcard include/config/binary/printf.h) \
  /home/yhe/work/4.3-sync-5020-std/build/linux/arch/powerpc/include/asm/string.h \
  /home/yhe/work/4.3-sync-5020-std/build/linux/include/linux/pageblock-flags.h \
    $(wildcard include/config/hugetlb/page/size/variable.h) \
  include/generated/bounds.h \
  /home/yhe/work/4.3-sync-5020-std/build/linux/include/linux/memory_hotplug.h \
    $(wildcard include/config/have/arch/nodedata/extension.h) \
    $(wildcard include/config/memory/hotremove.h) \
  /home/yhe/work/4.3-sync-5020-std/build/linux/include/linux/notifier.h \
  /home/yhe/work/4.3-sync-5020-std/build/linux/include/linux/mutex.h \
    $(wildcard include/config/debug/mutexes.h) \
  /home/yhe/work/4.3-sync-5020-std/build/linux/include/linux/rwsem.h \
    $(wildcard include/config/rwsem/generic/spinlock.h) \
  /home/yhe/work/4.3-sync-5020-std/build/linux/arch/powerpc/include/asm/rwsem.h \
  /home/yhe/work/4.3-sync-5020-std/build/linux/include/linux/srcu.h \
  /home/yhe/work/4.3-sync-5020-std/build/linux/include/linux/topology.h \
    $(wildcard include/config/sched/smt.h) \
    $(wildcard include/config/sched/mc.h) \
  /home/yhe/work/4.3-sync-5020-std/build/linux/include/linux/cpumask.h \
    $(wildcard include/config/cpumask/offstack.h) \
    $(wildcard include/config/hotplug/cpu.h) \
    $(wildcard include/config/debug/per/cpu/maps.h) \
    $(wildcard include/config/disable/obsolete/cpumask/functions.h) \
  /home/yhe/work/4.3-sync-5020-std/build/linux/include/linux/smp.h \
    $(wildcard include/config/use/generic/smp/helpers.h) \
  /home/yhe/work/4.3-sync-5020-std/build/linux/arch/powerpc/include/asm/smp.h \
  /home/yhe/work/4.3-sync-5020-std/build/linux/arch/powerpc/include/asm/topology.h \
    $(wildcard include/config/pci.h) \
  /home/yhe/work/4.3-sync-5020-std/build/linux/include/asm-generic/topology.h \
  /home/yhe/work/4.3-sync-5020-std/build/linux/include/linux/mmdebug.h \
    $(wildcard include/config/debug/virtual.h) \
  /home/yhe/work/4.3-sync-5020-std/build/linux/include/linux/elf.h \
  /home/yhe/work/4.3-sync-5020-std/build/linux/include/linux/elf-em.h \
  /home/yhe/work/4.3-sync-5020-std/build/linux/arch/powerpc/include/asm/elf.h \
    $(wildcard include/config/spu/base.h) \
  /home/yhe/work/4.3-sync-5020-std/build/linux/include/linux/sched.h \
    $(wildcard include/config/sched/debug.h) \
    $(wildcard include/config/no/hz.h) \
    $(wildcard include/config/detect/softlockup.h) \
    $(wildcard include/config/detect/hung/task.h) \
    $(wildcard include/config/mmu.h) \
    $(wildcard include/config/core/dump/default/elf/headers.h) \
    $(wildcard include/config/bsd/process/acct.h) \
    $(wildcard include/config/taskstats.h) \
    $(wildcard include/config/audit.h) \
    $(wildcard include/config/inotify/user.h) \
    $(wildcard include/config/epoll.h) \
    $(wildcard include/config/posix/mqueue.h) \
    $(wildcard include/config/keys.h) \
    $(wildcard include/config/perf/events.h) \
    $(wildcard include/config/schedstats.h) \
    $(wildcard include/config/task/delay/acct.h) \
    $(wildcard include/config/fair/group/sched.h) \
    $(wildcard include/config/rt/group/sched.h) \
    $(wildcard include/config/blk/dev/io/trace.h) \
    $(wildcard include/config/tree/preempt/rcu.h) \
    $(wildcard include/config/cc/stackprotector.h) \
    $(wildcard include/config/sysvipc.h) \
    $(wildcard include/config/auditsyscall.h) \
    $(wildcard include/config/rt/mutexes.h) \
    $(wildcard include/config/debug/psrwlock.h) \
    $(wildcard include/config/task/xacct.h) \
    $(wildcard include/config/cpusets.h) \
    $(wildcard include/config/cgroups.h) \
    $(wildcard include/config/futex.h) \
    $(wildcard include/config/fault/injection.h) \
    $(wildcard include/config/latencytop.h) \
    $(wildcard include/config/function/graph/tracer.h) \
    $(wildcard include/config/cgroup/mem/res/ctlr.h) \
    $(wildcard include/config/have/unstable/sched/clock.h) \
    $(wildcard include/config/stack/growsup.h) \
    $(wildcard include/config/debug/stack/usage.h) \
    $(wildcard include/config/cgroup/sched.h) \
    $(wildcard include/config/mm/owner.h) \
  /home/yhe/work/4.3-sync-5020-std/build/linux/arch/powerpc/include/asm/param.h \
  /home/yhe/work/4.3-sync-5020-std/build/linux/include/asm-generic/param.h \
    $(wildcard include/config/hz.h) \
  /home/yhe/work/4.3-sync-5020-std/build/linux/include/linux/capability.h \
  /home/yhe/work/4.3-sync-5020-std/build/linux/include/linux/timex.h \
  /home/yhe/work/4.3-sync-5020-std/build/linux/include/linux/param.h \
  /home/yhe/work/4.3-sync-5020-std/build/linux/arch/powerpc/include/asm/timex.h \
  /home/yhe/work/4.3-sync-5020-std/build/linux/include/linux/jiffies.h \
  /home/yhe/work/4.3-sync-5020-std/build/linux/include/linux/rbtree.h \
  /home/yhe/work/4.3-sync-5020-std/build/linux/include/linux/mm_types.h \
    $(wildcard include/config/split/ptlock/cpus.h) \
    $(wildcard include/config/want/page/debug/flags.h) \
    $(wildcard include/config/aio.h) \
    $(wildcard include/config/proc/fs.h) \
    $(wildcard include/config/mmu/notifier.h) \
  /home/yhe/work/4.3-sync-5020-std/build/linux/include/linux/auxvec.h \
  /home/yhe/work/4.3-sync-5020-std/build/linux/arch/powerpc/include/asm/auxvec.h \
  /home/yhe/work/4.3-sync-5020-std/build/linux/include/linux/prio_tree.h \
  /home/yhe/work/4.3-sync-5020-std/build/linux/include/linux/completion.h \
  /home/yhe/work/4.3-sync-5020-std/build/linux/include/linux/page-debug-flags.h \
    $(wildcard include/config/page/poisoning.h) \
    $(wildcard include/config/page/debug/something/else.h) \
  /home/yhe/work/4.3-sync-5020-std/build/linux/arch/powerpc/include/asm/cputime.h \
  /home/yhe/work/4.3-sync-5020-std/build/linux/arch/powerpc/include/asm/time.h \
    $(wildcard include/config/power.h) \
    $(wildcard include/config/wrhv.h) \
    $(wildcard include/config/8xx/cpu6.h) \
  /home/yhe/work/4.3-sync-5020-std/build/linux/include/linux/percpu.h \
    $(wildcard include/config/need/per/cpu/embed/first/chunk.h) \
    $(wildcard include/config/need/per/cpu/page/first/chunk.h) \
  /home/yhe/work/4.3-sync-5020-std/build/linux/include/linux/pfn.h \
  /home/yhe/work/4.3-sync-5020-std/build/linux/include/linux/sem.h \
  /home/yhe/work/4.3-sync-5020-std/build/linux/include/linux/ipc.h \
  /home/yhe/work/4.3-sync-5020-std/build/linux/arch/powerpc/include/asm/ipcbuf.h \
  /home/yhe/work/4.3-sync-5020-std/build/linux/arch/powerpc/include/asm/sembuf.h \
  /home/yhe/work/4.3-sync-5020-std/build/linux/include/linux/rcupdate.h \
    $(wildcard include/config/rcu/torture/test.h) \
    $(wildcard include/config/tree/rcu.h) \
    $(wildcard include/config/tiny/rcu.h) \
    $(wildcard include/config/debug/objects/rcu/head.h) \
  /home/yhe/work/4.3-sync-5020-std/build/linux/include/linux/debugobjects.h \
    $(wildcard include/config/debug/objects.h) \
    $(wildcard include/config/debug/objects/free.h) \
  /home/yhe/work/4.3-sync-5020-std/build/linux/include/linux/rcutree.h \
  /home/yhe/work/4.3-sync-5020-std/build/linux/include/linux/signal.h \
  /home/yhe/work/4.3-sync-5020-std/build/linux/arch/powerpc/include/asm/signal.h \
  /home/yhe/work/4.3-sync-5020-std/build/linux/include/asm-generic/signal-defs.h \
  /home/yhe/work/4.3-sync-5020-std/build/linux/arch/powerpc/include/asm/siginfo.h \
  /home/yhe/work/4.3-sync-5020-std/build/linux/include/asm-generic/siginfo.h \
  /home/yhe/work/4.3-sync-5020-std/build/linux/include/linux/path.h \
  /home/yhe/work/4.3-sync-5020-std/build/linux/include/linux/pid.h \
  /home/yhe/work/4.3-sync-5020-std/build/linux/include/linux/proportions.h \
  /home/yhe/work/4.3-sync-5020-std/build/linux/include/linux/percpu_counter.h \
  /home/yhe/work/4.3-sync-5020-std/build/linux/include/linux/seccomp.h \
    $(wildcard include/config/seccomp.h) \
  /home/yhe/work/4.3-sync-5020-std/build/linux/arch/powerpc/include/asm/seccomp.h \
  /home/yhe/work/4.3-sync-5020-std/build/linux/include/linux/unistd.h \
  /home/yhe/work/4.3-sync-5020-std/build/linux/arch/powerpc/include/asm/unistd.h \
  /home/yhe/work/4.3-sync-5020-std/build/linux/include/linux/rculist.h \
  /home/yhe/work/4.3-sync-5020-std/build/linux/include/linux/rtmutex.h \
    $(wildcard include/config/debug/rt/mutexes.h) \
  /home/yhe/work/4.3-sync-5020-std/build/linux/include/linux/plist.h \
    $(wildcard include/config/debug/pi/list.h) \
  /home/yhe/work/4.3-sync-5020-std/build/linux/include/linux/resource.h \
  /home/yhe/work/4.3-sync-5020-std/build/linux/arch/powerpc/include/asm/resource.h \
  /home/yhe/work/4.3-sync-5020-std/build/linux/include/asm-generic/resource.h \
  /home/yhe/work/4.3-sync-5020-std/build/linux/include/linux/timer.h \
    $(wildcard include/config/timer/stats.h) \
    $(wildcard include/config/debug/objects/timers.h) \
  /home/yhe/work/4.3-sync-5020-std/build/linux/include/linux/ktime.h \
    $(wildcard include/config/ktime/scalar.h) \
  /home/yhe/work/4.3-sync-5020-std/build/linux/include/linux/hrtimer.h \
    $(wildcard include/config/high/res/timers.h) \
  /home/yhe/work/4.3-sync-5020-std/build/linux/include/linux/task_io_accounting.h \
    $(wildcard include/config/task/io/accounting.h) \
  /home/yhe/work/4.3-sync-5020-std/build/linux/include/linux/kobject.h \
  /home/yhe/work/4.3-sync-5020-std/build/linux/include/linux/sysfs.h \
  /home/yhe/work/4.3-sync-5020-std/build/linux/include/linux/kref.h \
  /home/yhe/work/4.3-sync-5020-std/build/linux/include/linux/latencytop.h \
  /home/yhe/work/4.3-sync-5020-std/build/linux/include/linux/cred.h \
    $(wildcard include/config/debug/credentials.h) \
    $(wildcard include/config/security.h) \
  /home/yhe/work/4.3-sync-5020-std/build/linux/include/linux/key.h \
    $(wildcard include/config/sysctl.h) \
  /home/yhe/work/4.3-sync-5020-std/build/linux/include/linux/sysctl.h \
  /home/yhe/work/4.3-sync-5020-std/build/linux/include/linux/selinux.h \
    $(wildcard include/config/security/selinux.h) \
  /home/yhe/work/4.3-sync-5020-std/build/linux/include/linux/aio.h \
  /home/yhe/work/4.3-sync-5020-std/build/linux/include/linux/workqueue.h \
    $(wildcard include/config/debug/objects/work.h) \
  /home/yhe/work/4.3-sync-5020-std/build/linux/include/linux/aio_abi.h \
  /home/yhe/work/4.3-sync-5020-std/build/linux/include/linux/uio.h \
  /home/yhe/work/4.3-sync-5020-std/build/linux/include/linux/moduleparam.h \
    $(wildcard include/config/alpha.h) \
    $(wildcard include/config/ia64.h) \
  /home/yhe/work/4.3-sync-5020-std/build/linux/include/linux/immediate.h \
  /home/yhe/work/4.3-sync-5020-std/build/linux/include/linux/marker.h \
  /home/yhe/work/4.3-sync-5020-std/build/linux/include/linux/tracepoint.h \
  /home/yhe/work/4.3-sync-5020-std/build/linux/arch/powerpc/include/asm/module.h \
    $(wildcard include/config/dynamic/ftrace.h) \
  /home/yhe/work/4.3-sync-5020-std/build/linux/include/trace/events/module.h \
  /home/yhe/work/4.3-sync-5020-std/build/linux/include/trace/define_trace.h \
  /home/yhe/work/4.3-sync-5020-std/build/linux/include/linux/interrupt.h \
    $(wildcard include/config/pm/sleep.h) \
    $(wildcard include/config/generic/irq/probe.h) \
  /home/yhe/work/4.3-sync-5020-std/build/linux/include/linux/irqreturn.h \
  /home/yhe/work/4.3-sync-5020-std/build/linux/include/linux/irqnr.h \
  /home/yhe/work/4.3-sync-5020-std/build/linux/include/linux/hardirq.h \
  /home/yhe/work/4.3-sync-5020-std/build/linux/include/linux/smp_lock.h \
    $(wildcard include/config/lock/kernel.h) \
  /home/yhe/work/4.3-sync-5020-std/build/linux/include/linux/ftrace_irq.h \
    $(wildcard include/config/ftrace/nmi/enter.h) \
  /home/yhe/work/4.3-sync-5020-std/build/linux/arch/powerpc/include/asm/hardirq.h \
  /home/yhe/work/4.3-sync-5020-std/build/linux/include/linux/irq.h \
    $(wildcard include/config/s390.h) \
    $(wildcard include/config/irq/per/cpu.h) \
    $(wildcard include/config/irq/release/method.h) \
    $(wildcard include/config/intr/remap.h) \
    $(wildcard include/config/generic/pending/irq.h) \
    $(wildcard include/config/sparse/irq.h) \
    $(wildcard include/config/numa/irq/desc.h) \
    $(wildcard include/config/generic/hardirqs/no//do/irq.h) \
    $(wildcard include/config/cpumasks/offstack.h) \
  /home/yhe/work/4.3-sync-5020-std/build/linux/arch/powerpc/include/asm/irq.h \
    $(wildcard include/config/nr/irqs.h) \
    $(wildcard include/config/irqstacks.h) \
  /home/yhe/work/4.3-sync-5020-std/build/linux/include/linux/radix-tree.h \
  /home/yhe/work/4.3-sync-5020-std/build/linux/arch/powerpc/include/asm/irq_regs.h \
  /home/yhe/work/4.3-sync-5020-std/build/linux/include/asm-generic/irq_regs.h \
  /home/yhe/work/4.3-sync-5020-std/build/linux/include/linux/fs.h \
    $(wildcard include/config/dnotify.h) \
    $(wildcard include/config/quota.h) \
    $(wildcard include/config/fsnotify.h) \
    $(wildcard include/config/inotify.h) \
    $(wildcard include/config/fs/posix/acl.h) \
    $(wildcard include/config/optimize/sd/performance.h) \
    $(wildcard include/config/debug/writecount.h) \
    $(wildcard include/config/file/locking.h) \
    $(wildcard include/config/pramfs.h) \
    $(wildcard include/config/block.h) \
    $(wildcard include/config/fs/xip.h) \
    $(wildcard include/config/migration.h) \
  /home/yhe/work/4.3-sync-5020-std/build/linux/include/linux/limits.h \
  /home/yhe/work/4.3-sync-5020-std/build/linux/include/linux/ioctl.h \
  /home/yhe/work/4.3-sync-5020-std/build/linux/arch/powerpc/include/asm/ioctl.h \
  /home/yhe/work/4.3-sync-5020-std/build/linux/include/asm-generic/ioctl.h \
  /home/yhe/work/4.3-sync-5020-std/build/linux/include/linux/kdev_t.h \
  /home/yhe/work/4.3-sync-5020-std/build/linux/include/linux/dcache.h \
  /home/yhe/work/4.3-sync-5020-std/build/linux/include/linux/semaphore.h \
  /home/yhe/work/4.3-sync-5020-std/build/linux/include/linux/fiemap.h \
  /home/yhe/work/4.3-sync-5020-std/build/linux/include/linux/quota.h \
    $(wildcard include/config/quota/netlink/interface.h) \
  /home/yhe/work/4.3-sync-5020-std/build/linux/include/linux/dqblk_xfs.h \
  /home/yhe/work/4.3-sync-5020-std/build/linux/include/linux/dqblk_v1.h \
  /home/yhe/work/4.3-sync-5020-std/build/linux/include/linux/dqblk_v2.h \
  /home/yhe/work/4.3-sync-5020-std/build/linux/include/linux/dqblk_qtree.h \
  /home/yhe/work/4.3-sync-5020-std/build/linux/include/linux/nfs_fs_i.h \
  /home/yhe/work/4.3-sync-5020-std/build/linux/include/linux/nfs.h \
  /home/yhe/work/4.3-sync-5020-std/build/linux/include/linux/sunrpc/msg_prot.h \
  /home/yhe/work/4.3-sync-5020-std/build/linux/include/linux/inet.h \
  /home/yhe/work/4.3-sync-5020-std/build/linux/include/linux/fcntl.h \
  /home/yhe/work/4.3-sync-5020-std/build/linux/arch/powerpc/include/asm/fcntl.h \
  /home/yhe/work/4.3-sync-5020-std/build/linux/include/asm-generic/fcntl.h \
  /home/yhe/work/4.3-sync-5020-std/build/linux/include/linux/err.h \
  /home/yhe/work/4.3-sync-5020-std/build/linux/include/linux/cdev.h \
  /home/yhe/work/4.3-sync-5020-std/build/linux/arch/powerpc/include/asm/io.h \
    $(wildcard include/config/ra.h) \
    $(wildcard include/config/rd.h) \
    $(wildcard include/config/ppc/indirect/io.h) \
    $(wildcard include/config/eeh.h) \
  /home/yhe/work/4.3-sync-5020-std/build/linux/include/linux/device.h \
    $(wildcard include/config/debug/devres.h) \
    $(wildcard include/config/devtmpfs.h) \
  /home/yhe/work/4.3-sync-5020-std/build/linux/include/linux/ioport.h \
  /home/yhe/work/4.3-sync-5020-std/build/linux/include/linux/klist.h \
  /home/yhe/work/4.3-sync-5020-std/build/linux/include/linux/pm.h \
    $(wildcard include/config/pm/runtime.h) \
    $(wildcard include/config/pm/ops.h) \
  /home/yhe/work/4.3-sync-5020-std/build/linux/arch/powerpc/include/asm/device.h \
    $(wildcard include/config/swiotlb.h) \
  /home/yhe/work/4.3-sync-5020-std/build/linux/include/linux/pm_wakeup.h \
    $(wildcard include/config/pm.h) \
  /home/yhe/work/4.3-sync-5020-std/build/linux/include/linux/io.h \
    $(wildcard include/config/has/ioport.h) \
  /home/yhe/work/4.3-sync-5020-std/build/linux/arch/powerpc/include/asm/delay.h \
  /home/yhe/work/4.3-sync-5020-std/build/linux/include/asm-generic/iomap.h \
  /home/yhe/work/4.3-sync-5020-std/build/linux/arch/powerpc/include/asm/io-defs.h \

/home/yhe/work/ppc-dump-mod/ppc-dump.o: $(deps_/home/yhe/work/ppc-dump-mod/ppc-dump.o)

$(deps_/home/yhe/work/ppc-dump-mod/ppc-dump.o):
