#!/bin/bash
#PATH=/mnt/c/Users/xx/contiki-ng/armgcc/gcc-arm-none-eabi-9-2019-q4-major/bin/
#PATH=/mnt/c/Users/xx/contiki-ng/armgcc/gcc-arm-none-eabi-9-2019-q4-major/bin/:$PATH
#PATH=/mnt/c/Users/xx/contiki-ng/armgcc/gcc-arm-none-eabi-5_2-2015q4/bin/:$PATH
echo $PATH
rm build/cc26x0-cc13x0/sensortag/cc2650-new/obj/bmp*
make TARGET=cc26x0-cc13x0 BOARD=sensortag/cc2650-new -j8 clean
make TARGET=cc26x0-cc13x0 BOARD=sensortag/cc2650-new V=1 -j8

#cd build/cc26x0-cc13x0/sensortag/cc2650-new
#arm-none-eabi-gcc -DCONTIKI=1 -DCONTIKI_TARGET_CC26X0_CC13X0=1 -DCONTIKI_TARGET_STRING=\"cc26x0-cc13x0\" -Wno-unused-const-variable -DPROJECT_CONF_PATH=\"project-conf.h\" -DBOARD_SENSORTAG=1 -DBACKDOOR_IOID=0x00000000 -DCPU_FAMILY_CC26X0=1 -DCPU_FAMILY_CC26XX=1 -I../../../../arch/cpu/cc26x0-cc13x0/lib/cc26xxware -I../../../../arch/cpu/cc26x0-cc13x0/lib/cc26xxware/driverlib -I../../../../arch/cpu/cc26x0-cc13x0/lib/cc26xxware/inc -mcpu=cortex-m3 -mthumb -mabi=aapcs -mlittle-endian -Wall -std=c99 -ffunction-sections -fdata-sections -fno-strict-aliasing -fshort-enums -fomit-frame-pointer -fno-builtin -Werror -O2 -DCONTIKI_BOARD_SENSORTAG_CC2650_NEW=1 -DCONTIKI_BOARD_STRING=\"sensortag/cc2650-new\" -DMAC_CONF_WITH_CSMA=1 -DNETSTACK_CONF_WITH_IPV6=1 -DROUTING_CONF_RPL_LITE=1  -imacros ../../../../os/net/app-layer/coap/module-macros.h  -I. -I../../../../arch/platform/cc26x0-cc13x0/. -I../../../../arch/platform/cc26x0-cc13x0/sensortag/cc2650-new -I../../../../arch/platform/cc26x0-cc13x0/sensortag -I../../../../arch/platform/cc26x0-cc13x0/common -I../../../../arch/cpu/cc26x0-cc13x0/. -I../../../../arch/cpu/cc26x0-cc13x0/dev -I../../../../arch/cpu/cc26x0-cc13x0/rf-core -I../../../../arch/cpu/cc26x0-cc13x0/rf-core/api -I../../../../arch/cpu/cc26x0-cc13x0/rf-core/ble-hal -I../../../../arch/cpu/cc26x0-cc13x0/lib/cc26xxware/startup_files -I../../../../arch/cpu/cc26x0-cc13x0/../arm/cortex-m/cm3 -I../../../../arch/cpu/cc26x0-cc13x0/../arm/cortex-m -I../../../../arch/cpu/cc26x0-cc13x0/../arm/cortex-m/CMSIS -I../../../../arch/cpu/cc26x0-cc13x0/../arm/. -I../../../../os/net/app-layer/coap -I../../../../os -I../../../../arch -I../../../../os/sys -I../../../../os/lib -I../../../../os/dev -I../../../../os/net -I../../../../os/net/routing -I../../../../os/net/mac -I../../../../os/net/mac/framer -I../../../../os/storage -I../../../../os/services -I../../../../arch/dev/ext-flash -I../../../../arch/cpu/cc26x0-cc13x0/lib/cc26xxware/driverlib -I../../../../os/lib/newlib -I../../../../os/lib/dbg-io -I../../../../os/net/mac/csma -I../../../../os/net/ipv6 -I../../../../os/net/routing/rpl-lite -I../../../.. -DCONTIKI_VERSION_STRING=\"Contiki-NG-release/v4.4-36-g1b65829bb-dirty\" -c very-sleepy-demo.c -o very-sleepy-demo.o


#arm-none-eabi-gcc -DCONTIKI=1 -DCONTIKI_TARGET_CC26X0_CC13X0=1 -DCONTIKI_TARGET_STRING=\"cc26x0-cc13x0\" -Wno-unused-const-variable -DPROJECT_CONF_PATH=\"project-conf.h\" -DBOARD_SENSORTAG=1 -DBACKDOOR_IOID=0x00000000 -DCPU_FAMILY_CC26X0=1 -DCPU_FAMILY_CC26XX=1 -I../../../../arch/cpu/cc26x0-cc13x0/lib/cc26xxware -I../../../../arch/cpu/cc26x0-cc13x0/lib/cc26xxware/driverlib -I../../../../arch/cpu/cc26x0-cc13x0/lib/cc26xxware/inc -mcpu=cortex-m3 -mthumb -mabi=aapcs -mlittle-endian -Wall -std=c99 -ffunction-sections -fdata-sections -fno-strict-aliasing -fshort-enums -fomit-frame-pointer -fno-builtin -Werror -O2 -DCONTIKI_BOARD_SENSORTAG_CC2650_NEW=1 -DCONTIKI_BOARD_STRING=\"sensortag/cc2650-new\" -DMAC_CONF_WITH_CSMA=1 -DNETSTACK_CONF_WITH_IPV6=1 -DROUTING_CONF_RPL_LITE=1  -imacros ../../../../os/net/app-layer/coap/module-macros.h  -I. -I../../../../arch/platform/cc26x0-cc13x0/. -I../../../../arch/platform/cc26x0-cc13x0/sensortag/cc2650-new -I../../../../arch/platform/cc26x0-cc13x0/sensortag -I../../../../arch/platform/cc26x0-cc13x0/common -I../../../../arch/cpu/cc26x0-cc13x0/. -I../../../../arch/cpu/cc26x0-cc13x0/dev -I../../../../arch/cpu/cc26x0-cc13x0/rf-core -I../../../../arch/cpu/cc26x0-cc13x0/rf-core/api -I../../../../arch/cpu/cc26x0-cc13x0/rf-core/ble-hal -I../../../../arch/cpu/cc26x0-cc13x0/lib/cc26xxware/startup_files -I../../../../arch/cpu/cc26x0-cc13x0/../arm/cortex-m/cm3 -I../../../../arch/cpu/cc26x0-cc13x0/../arm/cortex-m -I../../../../arch/cpu/cc26x0-cc13x0/../arm/cortex-m/CMSIS -I../../../../arch/cpu/cc26x0-cc13x0/../arm/. -I../../../../os/net/app-layer/coap -I../../../../os -I../../../../arch -I../../../../os/sys -I../../../../os/lib -I../../../../os/dev -I../../../../os/net -I../../../../os/net/routing -I../../../../os/net/mac -I../../../../os/net/mac/framer -I../../../../os/storage -I../../../../os/services -I../../../../arch/dev/ext-flash -I../../../../arch/cpu/cc26x0-cc13x0/lib/cc26xxware/driverlib -I../../../../os/lib/newlib -I../../../../os/lib/dbg-io -I../../../../os/net/mac/csma -I../../../../os/net/ipv6 -I../../../../os/net/routing/rpl-lite -I../../../.. -DCONTIKI_VERSION_STRING=\"Contiki-NG-release/v4.4-36-g1b65829bb-dirty\" -MMD -c ../../../../arch/cpu/cc26x0-cc13x0/./fault-handlers.c -o build/cc26x0-cc13x0/sensortag/cc2650-new/obj/fault-handlers.o

#arm-none-eabi-gcc -DCONTIKI=1 -DCONTIKI_TARGET_CC26X0_CC13X0=1 -DCONTIKI_TARGET_STRING=\"cc26x0-cc13x0\" -Wno-unused-const-variable -DPROJECT_CONF_PATH=\"project-conf.h\" -DBOARD_SENSORTAG=1 -DBACKDOOR_IOID=0x00000000 -DCPU_FAMILY_CC26X0=1 -DCPU_FAMILY_CC26XX=1 -I../../../../arch/cpu/cc26x0-cc13x0/lib/cc26xxware -I../../../../arch/cpu/cc26x0-cc13x0/lib/cc26xxware/driverlib -I../../../../arch/cpu/cc26x0-cc13x0/lib/cc26xxware/inc -mcpu=cortex-m3 -mthumb -mabi=aapcs -mlittle-endian -Wall -std=c99 -ffunction-sections -fdata-sections -fno-strict-aliasing -fshort-enums -fomit-frame-pointer -fno-builtin -Werror -O2 -DCONTIKI_BOARD_SENSORTAG_CC2650_NEW=1 -DCONTIKI_BOARD_STRING=\"sensortag/cc2650-new\" -DMAC_CONF_WITH_CSMA=1 -DNETSTACK_CONF_WITH_IPV6=1 -DROUTING_CONF_RPL_LITE=1  -imacros ../../../../os/net/app-layer/coap/module-macros.h  -I. -I../../../../arch/platform/cc26x0-cc13x0/. -I../../../../arch/platform/cc26x0-cc13x0/sensortag/cc2650-new -I../../../../arch/platform/cc26x0-cc13x0/sensortag -I../../../../arch/platform/cc26x0-cc13x0/common -I../../../../arch/cpu/cc26x0-cc13x0/. -I../../../../arch/cpu/cc26x0-cc13x0/dev -I../../../../arch/cpu/cc26x0-cc13x0/rf-core -I../../../../arch/cpu/cc26x0-cc13x0/rf-core/api -I../../../../arch/cpu/cc26x0-cc13x0/rf-core/ble-hal -I../../../../arch/cpu/cc26x0-cc13x0/lib/cc26xxware/startup_files -I../../../../arch/cpu/cc26x0-cc13x0/../arm/cortex-m/cm3 -I../../../../arch/cpu/cc26x0-cc13x0/../arm/cortex-m -I../../../../arch/cpu/cc26x0-cc13x0/../arm/cortex-m/CMSIS -I../../../../arch/cpu/cc26x0-cc13x0/../arm/. -I../../../../os/net/app-layer/coap -I../../../../os -I../../../../arch -I../../../../os/sys -I../../../../os/lib -I../../../../os/dev -I../../../../os/net -I../../../../os/net/routing -I../../../../os/net/mac -I../../../../os/net/mac/framer -I../../../../os/storage -I../../../../os/services -I../../../../arch/dev/ext-flash -I../../../../arch/cpu/cc26x0-cc13x0/lib/cc26xxware/driverlib -I../../../../os/lib/newlib -I../../../../os/lib/dbg-io -I../../../../os/net/mac/csma -I../../../../os/net/ipv6 -I../../../../os/net/routing/rpl-lite -I../../../.. -DCONTIKI_VERSION_STRING=\"Contiki-NG-release/v4.4-36-g1b65829bb-dirty\" -MMD -c ../../../../arch/cpu/cc26x0-cc13x0/lib/cc26xxware/startup_files/startup_gcc.c -o build/cc26x0-cc13x0/sensortag/cc2650-new/obj/startup_gcc.o

#arm-none-eabi-gcc -Wl,--fatal-warnings -mcpu=cortex-m3 -nostartfiles -T ../../../../arch/cpu/cc26x0-cc13x0/cc26xx.ld \
#--cref,--no-warn-mismatch -mthumb -mlittle-endian \
#-Wl,--gc-sections,--sort-section=alignment -Wl,\
#-Map=build/cc26x0-cc13x0/sensortag/cc2650-new/very-sleepy-demo.map,\
#--cref,--no-warn-mismatch -mthumb -mlittle-endian \

#-Wl,--gc-sections,--sort-section=alignment -Wl,\
#-Map=build/cc26x0-cc13x0/sensortag/cc2650-new/very-sleepy-demo.map,\

#arm-none-eabi-gcc -Wl,--fatal-warnings -mcpu=cortex-m3 -nostartfiles -T ../../../../arch/cpu/cc26x0-cc13x0/cc26xx.ld \
#arm-none-eabi-gcc -w -Werror -Wall -o muh \
#build/cc26x0-cc13x0/sensortag/cc2650-new/obj/fault-handlers.o build/cc26x0-cc13x0/sensortag/cc2650-new/obj/ccfg.o \
#build/cc26x0-cc13x0/sensortag/cc2650-new/obj/fault-handlers.o build/cc26x0-cc13x0/sensortag/cc2650-new/obj/ccfg.o \
#build/cc26x0-cc13x0/sensortag/cc2650-new/obj/startup_gcc.o build/cc26x0-cc13x0/sensortag/cc2650-new/obj/platform.o \
#build/cc26x0-cc13x0/sensortag/cc2650-new/obj/leds-arch.o build/cc26x0-cc13x0/sensortag/cc2650-new/obj/sensortag-sensors.o \
#build/cc26x0-cc13x0/sensortag/cc2650-new/obj/board-buttons.o build/cc26x0-cc13x0/sensortag/cc2650-new/obj/sensor-common.o \
#build/cc26x0-cc13x0/sensortag/cc2650-new/obj/bmp-280-sensor.o build/cc26x0-cc13x0/sensortag/cc2650-new/obj/tmp-007-sensor.o \
#build/cc26x0-cc13x0/sensortag/cc2650-new/obj/opt-3001-sensor.o build/cc26x0-cc13x0/sensortag/cc2650-new/obj/hdc-1000-sensor.o \
#build/cc26x0-cc13x0/sensortag/cc2650-new/obj/mpu-9250-sensor.o build/cc26x0-cc13x0/sensortag/cc2650-new/obj/xmem.o \
#build/cc26x0-cc13x0/sensortag/cc2650-new/obj/buzzer.o build/cc26x0-cc13x0/sensortag/cc2650-new/obj/board.o \
#build/cc26x0-cc13x0/sensortag/cc2650-new/obj/board-i2c.o build/cc26x0-cc13x0/sensortag/cc2650-new/obj/clock.o \
#build/cc26x0-cc13x0/sensortag/cc2650-new/obj/rtimer-arch.o build/cc26x0-cc13x0/sensortag/cc2650-new/obj/soc-rtc.o \
#build/cc26x0-cc13x0/sensortag/cc2650-new/obj/uart.o build/cc26x0-cc13x0/sensortag/cc2650-new/obj/contiki-watchdog.o \
#build/cc26x0-cc13x0/sensortag/cc2650-new/obj/aux-ctrl.o build/cc26x0-cc13x0/sensortag/cc2650-new/obj/dbg.o \
#build/cc26x0-cc13x0/sensortag/cc2650-new/obj/ieee-addr.o build/cc26x0-cc13x0/sensortag/cc2650-new/obj/batmon-sensor.o \
#build/cc26x0-cc13x0/sensortag/cc2650-new/obj/adc-sensor.o build/cc26x0-cc13x0/sensortag/cc2650-new/obj/slip-arch.o \
#build/cc26x0-cc13x0/sensortag/cc2650-new/obj/slip.o build/cc26x0-cc13x0/sensortag/cc2650-new/obj/cc26xx-uart.o \
#build/cc26x0-cc13x0/sensortag/cc2650-new/obj/lpm.o build/cc26x0-cc13x0/sensortag/cc2650-new/obj/gpio-interrupt.o \
#build/cc26x0-cc13x0/sensortag/cc2650-new/obj/gpio-hal-arch.o build/cc26x0-cc13x0/sensortag/cc2650-new/obj/oscillators.o \
#build/cc26x0-cc13x0/sensortag/cc2650-new/obj/rf-core.o build/cc26x0-cc13x0/sensortag/cc2650-new/obj/rf-ble.o \
#build/cc26x0-cc13x0/sensortag/cc2650-new/obj/ieee-mode.o build/cc26x0-cc13x0/sensortag/cc2650-new/obj/ble-cc2650.o \
#build/cc26x0-cc13x0/sensortag/cc2650-new/obj/ble-hal-cc26xx.o build/cc26x0-cc13x0/sensortag/cc2650-new/obj/ble-addr.o \
#build/cc26x0-cc13x0/sensortag/cc2650-new/obj/rf-ble-cmd.o build/cc26x0-cc13x0/sensortag/cc2650-new/obj/random.o \
#build/cc26x0-cc13x0/sensortag/cc2650-new/obj/soc-trng.o build/cc26x0-cc13x0/sensortag/cc2650-new/obj/int-master.o \
#build/cc26x0-cc13x0/sensortag/cc2650-new/obj/spi-arch.o build/cc26x0-cc13x0/sensortag/cc2650-new/obj/cc26xx-aes.o \
#build/cc26x0-cc13x0/sensortag/cc2650-new/obj/coap-observe-client.o \
#build/cc26x0-cc13x0/sensortag/cc2650-new/obj/coap-blocking-api.o \
#build/cc26x0-cc13x0/sensortag/cc2650-new/obj/coap-callback-api.o \
#build/cc26x0-cc13x0/sensortag/cc2650-new/obj/coap-engine.o \
#build/cc26x0-cc13x0/sensortag/cc2650-new/obj/coap-separate.o \
#build/cc26x0-cc13x0/sensortag/cc2650-new/obj/coap-observe.o \
#build/cc26x0-cc13x0/sensortag/cc2650-new/obj/coap-timer-default.o \
#build/cc26x0-cc13x0/sensortag/cc2650-new/obj/coap-transactions.o \
#build/cc26x0-cc13x0/sensortag/cc2650-new/obj/coap-res-well-known-core.o \
#build/cc26x0-cc13x0/sensortag/cc2650-new/obj/coap-keystore-simple.o \
#build/cc26x0-cc13x0/sensortag/cc2650-new/obj/coap.o build/cc26x0-cc13x0/sensortag/cc2650-new/obj/coap-uip.o \
#build/cc26x0-cc13x0/sensortag/cc2650-new/obj/coap-timer.o build/cc26x0-cc13x0/sensortag/cc2650-new/obj/coap-block1.o \
#build/cc26x0-cc13x0/sensortag/cc2650-new/obj/coap-log.o build/cc26x0-cc13x0/sensortag/cc2650-new/obj/contiki-main.o \
#build/cc26x0-cc13x0/sensortag/cc2650-new/obj/autostart.o build/cc26x0-cc13x0/sensortag/cc2650-new/obj/stimer.o \
#build/cc26x0-cc13x0/sensortag/cc2650-new/obj/energest.o build/cc26x0-cc13x0/sensortag/cc2650-new/obj/etimer.o \
#build/cc26x0-cc13x0/sensortag/cc2650-new/obj/log.o \
#build/cc26x0-cc13x0/sensortag/cc2650-new/obj/mutex.o build/cc26x0-cc13x0/sensortag/cc2650-new/obj/ctimer.o \
#build/cc26x0-cc13x0/sensortag/cc2650-new/obj/compower.o build/cc26x0-cc13x0/sensortag/cc2650-new/obj/atomic.o \
#build/cc26x0-cc13x0/sensortag/cc2650-new/obj/rtimer.o build/cc26x0-cc13x0/sensortag/cc2650-new/obj/node-id.o \
#build/cc26x0-cc13x0/sensortag/cc2650-new/obj/process.o build/cc26x0-cc13x0/sensortag/cc2650-new/obj/timer.o \
#build/cc26x0-cc13x0/sensortag/cc2650-new/obj/stack-check.o build/cc26x0-cc13x0/sensortag/cc2650-new/obj/dbl-circ-list.o \
#build/cc26x0-cc13x0/sensortag/cc2650-new/obj/crc16.o build/cc26x0-cc13x0/sensortag/cc2650-new/obj/ifft.o \
#build/cc26x0-cc13x0/sensortag/cc2650-new/obj/ccm-star.o build/cc26x0-cc13x0/sensortag/cc2650-new/obj/sensors.o \
#build/cc26x0-cc13x0/sensortag/cc2650-new/obj/dbl-list.o build/cc26x0-cc13x0/sensortag/cc2650-new/obj/ringbufindex.o \
#build/cc26x0-cc13x0/sensortag/cc2650-new/obj/list.o build/cc26x0-cc13x0/sensortag/cc2650-new/obj/circular-list.o \
#build/cc26x0-cc13x0/sensortag/cc2650-new/obj/aes-128.o build/cc26x0-cc13x0/sensortag/cc2650-new/obj/trickle-timer.o \
#build/cc26x0-cc13x0/sensortag/cc2650-new/obj/memb.o build/cc26x0-cc13x0/sensortag/cc2650-new/obj/heapmem.o \
#build/cc26x0-cc13x0/sensortag/cc2650-new/obj/assert.o build/cc26x0-cc13x0/sensortag/cc2650-new/obj/ringbuf.o \
#build/cc26x0-cc13x0/sensortag/cc2650-new/obj/button-hal.o build/cc26x0-cc13x0/sensortag/cc2650-new/obj/serial-line.o \
#build/cc26x0-cc13x0/sensortag/cc2650-new/obj/spi.o build/cc26x0-cc13x0/sensortag/cc2650-new/obj/leds.o \
#build/cc26x0-cc13x0/sensortag/cc2650-new/obj/nullradio.o build/cc26x0-cc13x0/sensortag/cc2650-new/obj/gpio-hal.o \
#build/cc26x0-cc13x0/sensortag/cc2650-new/obj/queuebuf.o build/cc26x0-cc13x0/sensortag/cc2650-new/obj/nbr-table.o \
#build/cc26x0-cc13x0/sensortag/cc2650-new/obj/packetbuf.o build/cc26x0-cc13x0/sensortag/cc2650-new/obj/netstack.o \
#build/cc26x0-cc13x0/sensortag/cc2650-new/obj/linkaddr.o build/cc26x0-cc13x0/sensortag/cc2650-new/obj/net-debug.o \
#build/cc26x0-cc13x0/sensortag/cc2650-new/obj/link-stats.o build/cc26x0-cc13x0/sensortag/cc2650-new/obj/mac-sequence.o \
#build/cc26x0-cc13x0/sensortag/cc2650-new/obj/mac.o build/cc26x0-cc13x0/sensortag/cc2650-new/obj/frame802154e-ie.o \
#build/cc26x0-cc13x0/sensortag/cc2650-new/obj/framer-802154.o build/cc26x0-cc13x0/sensortag/cc2650-new/obj/nullframer.o \
#build/cc26x0-cc13x0/sensortag/cc2650-new/obj/frame802154.o build/cc26x0-cc13x0/sensortag/cc2650-new/obj/ext-flash.o \
#build/cc26x0-cc13x0/sensortag/cc2650-new/obj/udma.o build/cc26x0-cc13x0/sensortag/cc2650-new/obj/ssi.o \
#build/cc26x0-cc13x0/sensortag/cc2650-new/obj/aux_wuc.o build/cc26x0-cc13x0/sensortag/cc2650-new/obj/setup.o \
#build/cc26x0-cc13x0/sensortag/cc2650-new/obj/pwr_ctrl.o build/cc26x0-cc13x0/sensortag/cc2650-new/obj/i2s.o \
#build/cc26x0-cc13x0/sensortag/cc2650-new/obj/trng.o build/cc26x0-cc13x0/sensortag/cc2650-new/obj/prcm.o \
#build/cc26x0-cc13x0/sensortag/cc2650-new/obj/adi.o build/cc26x0-cc13x0/sensortag/cc2650-new/obj/aux_adc.o \
#build/cc26x0-cc13x0/sensortag/cc2650-new/obj/i2c.o build/cc26x0-cc13x0/sensortag/cc2650-new/obj/aon_wuc.o \
#build/cc26x0-cc13x0/sensortag/cc2650-new/obj/watchdog.o build/cc26x0-cc13x0/sensortag/cc2650-new/obj/aux_smph.o \
#build/cc26x0-cc13x0/sensortag/cc2650-new/obj/ioc.o build/cc26x0-cc13x0/sensortag/cc2650-new/obj/crypto.o \
#build/cc26x0-cc13x0/sensortag/cc2650-new/obj/rom_crypto.o build/cc26x0-cc13x0/sensortag/cc2650-new/obj/systick.o \
#build/cc26x0-cc13x0/sensortag/cc2650-new/obj/aon_ioc.o build/cc26x0-cc13x0/sensortag/cc2650-new/obj/ddi.o \
#build/cc26x0-cc13x0/sensortag/cc2650-new/obj/rfc.o build/cc26x0-cc13x0/sensortag/cc2650-new/obj/sys_ctrl.o \
#build/cc26x0-cc13x0/sensortag/cc2650-new/obj/interrupt.o build/cc26x0-cc13x0/sensortag/cc2650-new/obj/aux_tdc.o \
#build/cc26x0-cc13x0/sensortag/cc2650-new/obj/ccfgread.o build/cc26x0-cc13x0/sensortag/cc2650-new/obj/smph.o \
#build/cc26x0-cc13x0/sensortag/cc2650-new/obj/chipinfo.o build/cc26x0-cc13x0/sensortag/cc2650-new/obj/vims.o \
#build/cc26x0-cc13x0/sensortag/cc2650-new/obj/flash.o build/cc26x0-cc13x0/sensortag/cc2650-new/obj/cpu.o \
#build/cc26x0-cc13x0/sensortag/cc2650-new/obj/setup_rom.o build/cc26x0-cc13x0/sensortag/cc2650-new/obj/event.o \
#build/cc26x0-cc13x0/sensortag/cc2650-new/obj/aon_batmon.o build/cc26x0-cc13x0/sensortag/cc2650-new/obj/aux_timer.o \
#build/cc26x0-cc13x0/sensortag/cc2650-new/obj/gpio.o build/cc26x0-cc13x0/sensortag/cc2650-new/obj/aon_event.o \
#build/cc26x0-cc13x0/sensortag/cc2650-new/obj/aon_rtc.o build/cc26x0-cc13x0/sensortag/cc2650-new/obj/driverlib_release.o \
#build/cc26x0-cc13x0/sensortag/cc2650-new/obj/osc.o build/cc26x0-cc13x0/sensortag/cc2650-new/obj/syscalls.o \
#build/cc26x0-cc13x0/sensortag/cc2650-new/obj/strformat.o build/cc26x0-cc13x0/sensortag/cc2650-new/obj/printf.o \
#build/cc26x0-cc13x0/sensortag/cc2650-new/obj/sprintf.o build/cc26x0-cc13x0/sensortag/cc2650-new/obj/snprintf.o \
#build/cc26x0-cc13x0/sensortag/cc2650-new/obj/puts.o build/cc26x0-cc13x0/sensortag/cc2650-new/obj/putchar.o \
#build/cc26x0-cc13x0/sensortag/cc2650-new/obj/csma-security.o build/cc26x0-cc13x0/sensortag/cc2650-new/obj/anti-replay.o \
#build/cc26x0-cc13x0/sensortag/cc2650-new/obj/csma.o build/cc26x0-cc13x0/sensortag/cc2650-new/obj/csma-output.o \
#build/cc26x0-cc13x0/sensortag/cc2650-new/obj/ccm-star-packetbuf.o build/cc26x0-cc13x0/sensortag/cc2650-new/obj/uipbuf.o \
#build/cc26x0-cc13x0/sensortag/cc2650-new/obj/uip-icmp6.o build/cc26x0-cc13x0/sensortag/cc2650-new/obj/uip-sr.o \
#build/cc26x0-cc13x0/sensortag/cc2650-new/obj/uip-ds6.o build/cc26x0-cc13x0/sensortag/cc2650-new/obj/uip-packetqueue.o \
#build/cc26x0-cc13x0/sensortag/cc2650-new/obj/tcp-socket.o build/cc26x0-cc13x0/sensortag/cc2650-new/obj/ip64-addr.o \
#build/cc26x0-cc13x0/sensortag/cc2650-new/obj/uip-udp-packet.o build/cc26x0-cc13x0/sensortag/cc2650-new/obj/uip-ds6-route.o \
#build/cc26x0-cc13x0/sensortag/cc2650-new/obj/psock.o build/cc26x0-cc13x0/sensortag/cc2650-new/obj/uip-ds6-nbr.o \
#build/cc26x0-cc13x0/sensortag/cc2650-new/obj/tcpip.o build/cc26x0-cc13x0/sensortag/cc2650-new/obj/uip6.o \
#build/cc26x0-cc13x0/sensortag/cc2650-new/obj/uip-nameserver.o build/cc26x0-cc13x0/sensortag/cc2650-new/obj/uip-nd6.o \
#build/cc26x0-cc13x0/sensortag/cc2650-new/obj/udp-socket.o build/cc26x0-cc13x0/sensortag/cc2650-new/obj/simple-udp.o \
#build/cc26x0-cc13x0/sensortag/cc2650-new/obj/sicslowpan.o build/cc26x0-cc13x0/sensortag/cc2650-new/obj/uiplib.o \
#build/cc26x0-cc13x0/sensortag/cc2650-new/obj/rpl-dag.o build/cc26x0-cc13x0/sensortag/cc2650-new/obj/rpl-of0.o \
#build/cc26x0-cc13x0/sensortag/cc2650-new/obj/rpl-neighbor.o build/cc26x0-cc13x0/sensortag/cc2650-new/obj/rpl-timers.o \
#build/cc26x0-cc13x0/sensortag/cc2650-new/obj/rpl-ext-header.o build/cc26x0-cc13x0/sensortag/cc2650-new/obj/rpl.o \
#build/cc26x0-cc13x0/sensortag/cc2650-new/obj/rpl-nbr-policy.o build/cc26x0-cc13x0/sensortag/cc2650-new/obj/rpl-mrhof.o \
#build/cc26x0-cc13x0/sensortag/cc2650-new/obj/rpl-icmp6.o build/cc26x0-cc13x0/sensortag/cc2650-new/obj/rpl-dag-root.o \
#very-sleepy-demo.o  \
#-o build/cc26x0-cc13x0/sensortag/cc2650-new/very-sleepy-demo.elf
#
##very-sleepy-demo.o  \
##-Wl,--start-group -lm -Wl,--end-group -o build/cc26x0-cc13x0/sensortag/cc2650-new/very-sleepy-demo.elf
#
#echo $?