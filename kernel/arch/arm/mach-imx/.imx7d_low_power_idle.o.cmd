cmd_arch/arm/mach-imx/imx7d_low_power_idle.o := arm-linux-gnueabihf-gcc -Wp,-MD,arch/arm/mach-imx/.imx7d_low_power_idle.o.d  -nostdinc -isystem /usr/local/arm/gcc-linaro-4.9.4-2017.01-x86_64_arm-linux-gnueabihf/bin/../lib/gcc/arm-linux-gnueabihf/4.9.4/include -I./arch/arm/include -Iarch/arm/include/generated/uapi -Iarch/arm/include/generated  -Iinclude -I./arch/arm/include/uapi -Iarch/arm/include/generated/uapi -I./include/uapi -Iinclude/generated/uapi -include ./include/linux/kconfig.h -D__KERNEL__ -mlittle-endian   -D__ASSEMBLY__ -mabi=aapcs-linux -mno-thumb-interwork -mfpu=vfp -funwind-tables -marm -D__LINUX_ARM_ARCH__=7 -march=armv7-a  -include asm/unified.h -msoft-float -DCC_HAVE_ASM_GOTO         -Wa,-march=armv7-a   -c -o arch/arm/mach-imx/imx7d_low_power_idle.o arch/arm/mach-imx/imx7d_low_power_idle.S

source_arch/arm/mach-imx/imx7d_low_power_idle.o := arch/arm/mach-imx/imx7d_low_power_idle.S

deps_arch/arm/mach-imx/imx7d_low_power_idle.o := \
    $(wildcard include/config/smp.h) \
  arch/arm/include/asm/unified.h \
    $(wildcard include/config/arm/asm/unified.h) \
    $(wildcard include/config/cpu/v7m.h) \
    $(wildcard include/config/thumb2/kernel.h) \
  include/linux/linkage.h \
  include/linux/compiler.h \
    $(wildcard include/config/sparse/rcu/pointer.h) \
    $(wildcard include/config/trace/branch/profiling.h) \
    $(wildcard include/config/profile/all/branches.h) \
    $(wildcard include/config/enable/must/check.h) \
    $(wildcard include/config/enable/warn/deprecated.h) \
    $(wildcard include/config/kprobes.h) \
  include/linux/stringify.h \
  include/linux/export.h \
    $(wildcard include/config/have/underscore/symbol/prefix.h) \
    $(wildcard include/config/modules.h) \
    $(wildcard include/config/modversions.h) \
    $(wildcard include/config/unused/symbols.h) \
  arch/arm/include/asm/linkage.h \

arch/arm/mach-imx/imx7d_low_power_idle.o: $(deps_arch/arm/mach-imx/imx7d_low_power_idle.o)

$(deps_arch/arm/mach-imx/imx7d_low_power_idle.o):
