cmd_arch/arm/boot/dts/imx53-voipac-bsb.dtb := mkdir -p arch/arm/boot/dts/ ; arm-linux-gnueabihf-gcc -E -Wp,-MD,arch/arm/boot/dts/.imx53-voipac-bsb.dtb.d.pre.tmp -nostdinc -I./arch/arm/boot/dts -I./arch/arm/boot/dts/include -I./drivers/of/testcase-data -undef -D__DTS__ -x assembler-with-cpp -o arch/arm/boot/dts/.imx53-voipac-bsb.dtb.dts.tmp arch/arm/boot/dts/imx53-voipac-bsb.dts ; ./scripts/dtc/dtc -O dtb -o arch/arm/boot/dts/imx53-voipac-bsb.dtb -b 0 -i arch/arm/boot/dts/  -d arch/arm/boot/dts/.imx53-voipac-bsb.dtb.d.dtc.tmp arch/arm/boot/dts/.imx53-voipac-bsb.dtb.dts.tmp ; cat arch/arm/boot/dts/.imx53-voipac-bsb.dtb.d.pre.tmp arch/arm/boot/dts/.imx53-voipac-bsb.dtb.d.dtc.tmp > arch/arm/boot/dts/.imx53-voipac-bsb.dtb.d

source_arch/arm/boot/dts/imx53-voipac-bsb.dtb := arch/arm/boot/dts/imx53-voipac-bsb.dts

deps_arch/arm/boot/dts/imx53-voipac-bsb.dtb := \
  arch/arm/boot/dts/imx53-voipac-dmm-668.dtsi \
  arch/arm/boot/dts/imx53.dtsi \
  arch/arm/boot/dts/skeleton.dtsi \
  arch/arm/boot/dts/imx53-pinfunc.h \
  arch/arm/boot/dts/include/dt-bindings/clock/imx5-clock.h \
  arch/arm/boot/dts/include/dt-bindings/gpio/gpio.h \
  arch/arm/boot/dts/include/dt-bindings/input/input.h \

arch/arm/boot/dts/imx53-voipac-bsb.dtb: $(deps_arch/arm/boot/dts/imx53-voipac-bsb.dtb)

$(deps_arch/arm/boot/dts/imx53-voipac-bsb.dtb):
