
/* Hello World Example

   This example code is in the Public Domain (or CC0 licensed, at your option.)

   Unless required by applicable law or agreed to in writing, this
   software is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR
   CONDITIONS OF ANY KIND, either express or implied.
*/
#include <stdio.h>
#include "sdkconfig.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_system.h"
#include "esp_spi_flash.h"

#include "esp_heap_caps.h"
#include "soc/soc.h"
#include "soc/gpio_sig_map.h"
#include "soc/i2s_reg.h"
#include "soc/i2s_struct.h"
#include "soc/io_mux_reg.h"
#include "driver/gpio.h"
#include "driver/periph_ctrl.h"
#include "esp32/rom/lldesc.h"
//#include "esp_intr.h"
#include "esp_log.h"
#include <soc/rtc.h>

#ifdef CONFIG_IDF_TARGET_ESP32
#define CHIP_NAME "ESP32"
#endif

#ifdef CONFIG_IDF_TARGET_ESP32S2BETA
#define CHIP_NAME "ESP32-S2 Beta"
#endif
#include "dder.h"
#define NBIS2SERIALPINS 3

#include "NeopixelVirtualDriver.h"

//static er p;

static TaskHandle_t task2rameHandle = 0;
static TaskHandle_t taskrameHandle = 0;
static uint32_t dd=0;
 NeopixelVirtualDriver pp;

void task(void *param)
{
    for(;;){
        printf("In the second core:%d\n",dd);
        dd++;
        printf("total:%d eof:%d end:%d\n",pp.total,pp.eof,pp.end);
        //vTaskDelay(10 / portTICK_PERIOD_MS);
    }
}

void task2(void *param)
{
    for(;;){
        printf("In the first core\n");
        vTaskDelay(150 / portTICK_PERIOD_MS);
    }
}

dder kk;
#define I2S_DEVICE 0
extern "C" {
   void app_main(void);
}
int Pins[10];
void addf(int *a,int *b)
{
    *a=*b;
    return;
}
void app_main(void)
{
    
   
    printf("Hello world!\n");
   // xTaskCreatePinnedToCore(task2 ,"task2", 3000, NULL,1, &task2rameHandle, 0);
       xTaskCreatePinnedToCore(task, "task", 3000, NULL,1, &taskrameHandle, 0);
    /* Print chip information */
    esp_chip_info_t chip_info;
    esp_chip_info(&chip_info);
    printf("This is %s chip with %d CPU cores, WiFi%s%s, ",
            CHIP_NAME,
            chip_info.cores,
            (chip_info.features & CHIP_FEATURE_BT) ? "/BT" : "",
            (chip_info.features & CHIP_FEATURE_BLE) ? "/BLE" : "");

    printf("silicon revision %d, ", chip_info.revision);

    printf("%dMB %s flash\n", spi_flash_get_chip_size() / (1024 * 1024),
            (chip_info.features & CHIP_FEATURE_EMB_FLASH) ? "embedded" : "external");

    printf("size:%d\n",heap_caps_get_total_size(MALLOC_CAP_8BIT));
    printf("size:%d\n",heap_caps_get_total_size(MALLOC_CAP_SPIRAM));
    printf("size:%d\n",heap_caps_get_free_size(MALLOC_CAP_8BIT));
    printf("size:%d\n",heap_caps_get_free_size(MALLOC_CAP_SPIRAM));
    printf("trying ss");
   
    int pins[3]={14,15,16};
    int pins2[3]={14,15,16};
   pp.setPins(12,17,pins);

   pp.showPixel();
   //kk.getvar();
    addf(pins,pins2);
   // vTaskDelay(/portTICK_RATE_MS);
    //esp_restart();
}
