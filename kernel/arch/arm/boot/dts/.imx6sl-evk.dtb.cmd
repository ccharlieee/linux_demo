cmd_arch/arm/boot/dts/imx6sl-evk.dtb := mkdir -p arch/arm/boot/dts/ ; arm-linux-gnueabihf-gcc -E -Wp,-MD,arch/arm/boot/dts/.imx6sl-evk.dtb.d.pre.tmp -nostdinc -I./arch/arm/boot/dts -I./arch/arm/boot/dts/include -I./drivers/of/testcase-data -undef -D__DTS__ -x assembler-with-cpp -o arch/arm/boot/dts/.imx6sl-evk.dtb.dts.tmp arch/arm/boot/dts/imx6sl-evk.dts ; ./scripts/dtc/dtc -O dtb -o arch/arm/boot/dts/imx6sl-evk.dtb -b 0 -i arch/arm/boot/dts/  -d arch/arm/boot/dts/.imx6sl-evk.dtb.d.dtc.tmp arch/arm/boot/dts/.imx6sl-evk.dtb.dts.tmp ; cat arch/arm/boot/dts/.imx6sl-evk.dtb.d.pre.tmp arch/arm/boot/dts/.imx6sl-evk.dtb.d.dtc.tmp > arch/arm/boot/dts/.imx6sl-evk.dtb.d

source_arch/arm/boot/dts/imx6sl-evk.dtb := arch/arm/boot/dts/imx6sl-evk.dts

deps_arch/arm/boot/dts/imx6sl-evk.dtb := \
  arch/arm/boot/dts/include/dt-bindings/gpio/gpio.h \
  arch/arm/boot/dts/include/dt-bindings/input/input.h \
  arch/arm/boot/dts/imx6sl.dtsi \
  arch/arm/boot/dts/include/dt-bindings/interrupt-controller/irq.h \
  arch/arm/boot/dts/include/dt-bindings/interrupt-controller/arm-gic.h \
  arch/arm/boot/dts/skeleton.dtsi \
  arch/arm/boot/dts/imx6sl-pinfunc.h \
  arch/arm/boot/dts/include/dt-bindings/clock/imx6sl-clock.h \

arch/arm/boot/dts/imx6sl-evk.dtb: $(deps_arch/arm/boot/dts/imx6sl-evk.dtb)

$(deps_arch/arm/boot/dts/imx6sl-evk.dtb):
