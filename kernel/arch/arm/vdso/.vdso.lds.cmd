cmd_arch/arm/vdso/vdso.lds := arm-linux-gnueabihf-gcc -E -Wp,-MD,arch/arm/vdso/.vdso.lds.d  -nostdinc -isystem /usr/local/arm/gcc-linaro-4.9.4-2017.01-x86_64_arm-linux-gnueabihf/bin/../lib/gcc/arm-linux-gnueabihf/4.9.4/include -I./arch/arm/include -Iarch/arm/include/generated/uapi -Iarch/arm/include/generated  -Iinclude -I./arch/arm/include/uapi -Iarch/arm/include/generated/uapi -I./include/uapi -Iinclude/generated/uapi -include ./include/linux/kconfig.h -D__KERNEL__ -mlittle-endian     -P -C -Uarm -P -C -Uarm -D__ASSEMBLY__ -DLINKER_SCRIPT -o arch/arm/vdso/vdso.lds arch/arm/vdso/vdso.lds.S

source_arch/arm/vdso/vdso.lds := arch/arm/vdso/vdso.lds.S

deps_arch/arm/vdso/vdso.lds := \
  include/uapi/linux/const.h \
  arch/arm/include/asm/page.h \
    $(wildcard include/config/mmu.h) \
    $(wildcard include/config/cpu/copy/v4wt.h) \
    $(wildcard include/config/cpu/copy/v4wb.h) \
    $(wildcard include/config/cpu/copy/feroceon.h) \
    $(wildcard include/config/cpu/copy/fa.h) \
    $(wildcard include/config/cpu/sa1100.h) \
    $(wildcard include/config/cpu/xscale.h) \
    $(wildcard include/config/cpu/xsc3.h) \
    $(wildcard include/config/cpu/copy/v6.h) \
    $(wildcard include/config/kuser/helpers.h) \
    $(wildcard include/config/arm/lpae.h) \
    $(wildcard include/config/have/arch/pfn/valid.h) \
  include/asm-generic/getorder.h \
  arch/arm/include/asm/vdso.h \
    $(wildcard include/config/vdso.h) \

arch/arm/vdso/vdso.lds: $(deps_arch/arm/vdso/vdso.lds)

$(deps_arch/arm/vdso/vdso.lds):
