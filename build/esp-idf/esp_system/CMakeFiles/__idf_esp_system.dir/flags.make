# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.18

# compile ASM with /Users/yvesbazin/.espressif/tools/xtensa-esp32-elf/esp-2020r3-8.4.0/xtensa-esp32-elf/bin/xtensa-esp32-elf-gcc
# compile C with /Users/yvesbazin/.espressif/tools/xtensa-esp32-elf/esp-2020r3-8.4.0/xtensa-esp32-elf/bin/xtensa-esp32-elf-gcc
ASM_DEFINES = -DMBEDTLS_CONFIG_FILE=\"mbedtls/esp_config.h\"

ASM_INCLUDES = -I/Users/yvesbazin/esp/projects/hello_world/build/config -I/Users/yvesbazin/esp/esp-idf/components/esp_system/include -I/Users/yvesbazin/esp/esp-idf/components/esp_system/port/include -I/Users/yvesbazin/esp/esp-idf/components/newlib/platform_include -I/Users/yvesbazin/esp/esp-idf/components/freertos/include -I/Users/yvesbazin/esp/esp-idf/components/freertos/xtensa/include -I/Users/yvesbazin/esp/esp-idf/components/heap/include -I/Users/yvesbazin/esp/esp-idf/components/log/include -I/Users/yvesbazin/esp/esp-idf/components/lwip/include/apps -I/Users/yvesbazin/esp/esp-idf/components/lwip/include/apps/sntp -I/Users/yvesbazin/esp/esp-idf/components/lwip/lwip/src/include -I/Users/yvesbazin/esp/esp-idf/components/lwip/port/esp32/include -I/Users/yvesbazin/esp/esp-idf/components/lwip/port/esp32/include/arch -I/Users/yvesbazin/esp/esp-idf/components/soc/include -I/Users/yvesbazin/esp/esp-idf/components/soc/src/esp32/. -I/Users/yvesbazin/esp/esp-idf/components/soc/src/esp32/include -I/Users/yvesbazin/esp/esp-idf/components/hal/esp32/include -I/Users/yvesbazin/esp/esp-idf/components/hal/include -I/Users/yvesbazin/esp/esp-idf/components/esp_rom/include -I/Users/yvesbazin/esp/esp-idf/components/esp_common/include -I/Users/yvesbazin/esp/esp-idf/components/xtensa/include -I/Users/yvesbazin/esp/esp-idf/components/xtensa/esp32/include -I/Users/yvesbazin/esp/esp-idf/components/esp32/include -I/Users/yvesbazin/esp/esp-idf/components/driver/include -I/Users/yvesbazin/esp/esp-idf/components/driver/esp32/include -I/Users/yvesbazin/esp/esp-idf/components/esp_ringbuf/include -I/Users/yvesbazin/esp/esp-idf/components/efuse/include -I/Users/yvesbazin/esp/esp-idf/components/efuse/esp32/include -I/Users/yvesbazin/esp/esp-idf/components/espcoredump/include -I/Users/yvesbazin/esp/esp-idf/components/esp_timer/include -I/Users/yvesbazin/esp/esp-idf/components/esp_ipc/include -I/Users/yvesbazin/esp/esp-idf/components/esp_pm/include -I/Users/yvesbazin/esp/esp-idf/components/soc/soc/esp32/include -I/Users/yvesbazin/esp/esp-idf/components/soc/soc/esp32/../include -I/Users/yvesbazin/esp/esp-idf/components/soc/soc/esp32/private_include -I/Users/yvesbazin/esp/esp-idf/components/vfs/include -I/Users/yvesbazin/esp/esp-idf/components/esp_wifi/include -I/Users/yvesbazin/esp/esp-idf/components/esp_wifi/esp32/include -I/Users/yvesbazin/esp/esp-idf/components/esp_event/include -I/Users/yvesbazin/esp/esp-idf/components/esp_netif/include -I/Users/yvesbazin/esp/esp-idf/components/esp_eth/include -I/Users/yvesbazin/esp/esp-idf/components/tcpip_adapter/include -I/Users/yvesbazin/esp/esp-idf/components/app_trace/include -I/Users/yvesbazin/esp/esp-idf/components/spi_flash/include -I/Users/yvesbazin/esp/esp-idf/components/app_update/include -I/Users/yvesbazin/esp/esp-idf/components/bootloader_support/include -I/Users/yvesbazin/esp/esp-idf/components/nvs_flash/include -I/Users/yvesbazin/esp/esp-idf/components/mbedtls/port/include -I/Users/yvesbazin/esp/esp-idf/components/mbedtls/mbedtls/include -I/Users/yvesbazin/esp/esp-idf/components/mbedtls/esp_crt_bundle/include -I/Users/yvesbazin/esp/esp-idf/components/pthread/include

ASM_FLAGS = -ffunction-sections -fdata-sections -fstrict-volatile-bitfields -Wall -Werror=all -Wno-error=unused-function -Wno-error=unused-but-set-variable -Wno-error=unused-variable -Wno-error=deprecated-declarations -Wextra -Wno-unused-parameter -Wno-sign-compare -ggdb -Og -D_GNU_SOURCE -DIDF_VER=\"v4.3-dev-1521-g0289d1cc8\" -DESP_PLATFORM

C_DEFINES = -DMBEDTLS_CONFIG_FILE=\"mbedtls/esp_config.h\"

C_INCLUDES = -I/Users/yvesbazin/esp/projects/hello_world/build/config -I/Users/yvesbazin/esp/esp-idf/components/esp_system/include -I/Users/yvesbazin/esp/esp-idf/components/esp_system/port/include -I/Users/yvesbazin/esp/esp-idf/components/newlib/platform_include -I/Users/yvesbazin/esp/esp-idf/components/freertos/include -I/Users/yvesbazin/esp/esp-idf/components/freertos/xtensa/include -I/Users/yvesbazin/esp/esp-idf/components/heap/include -I/Users/yvesbazin/esp/esp-idf/components/log/include -I/Users/yvesbazin/esp/esp-idf/components/lwip/include/apps -I/Users/yvesbazin/esp/esp-idf/components/lwip/include/apps/sntp -I/Users/yvesbazin/esp/esp-idf/components/lwip/lwip/src/include -I/Users/yvesbazin/esp/esp-idf/components/lwip/port/esp32/include -I/Users/yvesbazin/esp/esp-idf/components/lwip/port/esp32/include/arch -I/Users/yvesbazin/esp/esp-idf/components/soc/include -I/Users/yvesbazin/esp/esp-idf/components/soc/src/esp32/. -I/Users/yvesbazin/esp/esp-idf/components/soc/src/esp32/include -I/Users/yvesbazin/esp/esp-idf/components/hal/esp32/include -I/Users/yvesbazin/esp/esp-idf/components/hal/include -I/Users/yvesbazin/esp/esp-idf/components/esp_rom/include -I/Users/yvesbazin/esp/esp-idf/components/esp_common/include -I/Users/yvesbazin/esp/esp-idf/components/xtensa/include -I/Users/yvesbazin/esp/esp-idf/components/xtensa/esp32/include -I/Users/yvesbazin/esp/esp-idf/components/esp32/include -I/Users/yvesbazin/esp/esp-idf/components/driver/include -I/Users/yvesbazin/esp/esp-idf/components/driver/esp32/include -I/Users/yvesbazin/esp/esp-idf/components/esp_ringbuf/include -I/Users/yvesbazin/esp/esp-idf/components/efuse/include -I/Users/yvesbazin/esp/esp-idf/components/efuse/esp32/include -I/Users/yvesbazin/esp/esp-idf/components/espcoredump/include -I/Users/yvesbazin/esp/esp-idf/components/esp_timer/include -I/Users/yvesbazin/esp/esp-idf/components/esp_ipc/include -I/Users/yvesbazin/esp/esp-idf/components/esp_pm/include -I/Users/yvesbazin/esp/esp-idf/components/soc/soc/esp32/include -I/Users/yvesbazin/esp/esp-idf/components/soc/soc/esp32/../include -I/Users/yvesbazin/esp/esp-idf/components/soc/soc/esp32/private_include -I/Users/yvesbazin/esp/esp-idf/components/vfs/include -I/Users/yvesbazin/esp/esp-idf/components/esp_wifi/include -I/Users/yvesbazin/esp/esp-idf/components/esp_wifi/esp32/include -I/Users/yvesbazin/esp/esp-idf/components/esp_event/include -I/Users/yvesbazin/esp/esp-idf/components/esp_netif/include -I/Users/yvesbazin/esp/esp-idf/components/esp_eth/include -I/Users/yvesbazin/esp/esp-idf/components/tcpip_adapter/include -I/Users/yvesbazin/esp/esp-idf/components/app_trace/include -I/Users/yvesbazin/esp/esp-idf/components/spi_flash/include -I/Users/yvesbazin/esp/esp-idf/components/app_update/include -I/Users/yvesbazin/esp/esp-idf/components/bootloader_support/include -I/Users/yvesbazin/esp/esp-idf/components/nvs_flash/include -I/Users/yvesbazin/esp/esp-idf/components/mbedtls/port/include -I/Users/yvesbazin/esp/esp-idf/components/mbedtls/mbedtls/include -I/Users/yvesbazin/esp/esp-idf/components/mbedtls/esp_crt_bundle/include -I/Users/yvesbazin/esp/esp-idf/components/pthread/include

C_FLAGS = -mlongcalls -Wno-frame-address -ffunction-sections -fdata-sections -fstrict-volatile-bitfields -Wall -Werror=all -Wno-error=unused-function -Wno-error=unused-but-set-variable -Wno-error=unused-variable -Wno-error=deprecated-declarations -Wextra -Wno-unused-parameter -Wno-sign-compare -ggdb -Og -std=gnu99 -Wno-old-style-declaration -D_GNU_SOURCE -DIDF_VER=\"v4.3-dev-1521-g0289d1cc8\" -DESP_PLATFORM

# Custom flags: esp-idf/esp_system/CMakeFiles/__idf_esp_system.dir/startup.c.obj_FLAGS = -fno-stack-protector

# Custom flags: esp-idf/esp_system/CMakeFiles/__idf_esp_system.dir/sleep_modes.c.obj_FLAGS = -fno-jump-tables -fno-tree-switch-conversion

