
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
//#include "dder.h"


//#include "NeopixelVirtualDriver.h"
//#define COLOR_COMPONENT 3
//#define FULL_DMA_BUFFER 1
#define NBIS2SERIALPINS 15
#define NUM_LEDS_PER_STRIP 256
#define STATIC_COLOR_GRB 1
#include "I2SClocklessVirtualLedDriver.h"
#define LATCH_PIN 13
#define CLOCK_PIN 27 //for a reason I don't know, the CLOCK_PIN needs to be >=16


#define NUM_STRIPS NBIS2SERIALPINS * 8
#define NUM_LEDS NUM_LEDS_PER_STRIP * NUM_STRIPS
//#define ledsperstrip 256
//#define numstrips 16
//#define nulmeds 256
//static er p;

static TaskHandle_t task2rameHandle = 0;
static TaskHandle_t taskrameHandle = 0;
// static uint32_t dd=0;

uint8_t leds[NUM_LEDS*3];
int pins[15]={12,14,26,25,33,32,15,0,2,4,15,16,21,23,19};
 //NeopixelVirtualDriver pp;
I2SClocklessVirtualLedDriver driver;



//dder kk;


extern "C" {
   void app_main(void);
//   void testasm(volatile uint16_t *buffer,uint32_t mask,uint32_t dmask,uint32_t value,uint32_t value2,uint32_t value3);
 //  void testasm2( uint8_t *buffer,volatile uint16_t *values);
}
/
void addf(int *a,int *b)
{
    *a=*b;
    return;
}
__attribute__ ((always_inline)) inline static uint32_t __clock_cycles() {
    uint32_t cyc;
    __asm__ __volatile__ ("rsr %0,ccount":"=a" (cyc));
    return cyc;
}

//void testasm(volatile uint8_t *lockvar);
void app_main(void)
{
  
driver.initled((uint8_t*)leds,pins,CLOCK_PIN,LATCH_PIN);
driver.setBrightness(20);
while(1)
{
  memset(leds,0,NUM_LEDS*3);
    //fill_solid(leds, NUM_LEDS, CRGB(15,15,15));
    /*
    * this code will create a snake on each strip where the length is the strips number
    */
    for(int i=0;i<NUM_STRIPS;i++)
    {
        int offset=i*NUM_LEDS_PER_STRIP;   //this is the offest of the strip number i
        for(int k=0;k<i+1;k++)
        {
            //leds[(start+k)%NUM_LEDS_PER_STRIP+offset]=CRGB(255,0,0);
            //Serial.printf(" %d %d ",NUM_STRIPS,(int)(i*255)/NUM_STRIPS);
            driver.setPixel(k%NUM_LEDS_PER_STRIP+offset,255,0,0);
        }
       // driver.setPixel(offset+100,0,0,255);
        //driver.setPixel(offset+99,0,255,0);
  //Serial.printf("\n");
    }
    long     lastHandle = __clock_cycles();
    driver.showPixels();
    //FastLED.show();
    long   lasthandle2=__clock_cycles();
    printf("FPS fastled: %f \n", (float) 240000000L/(lasthandle2 - lastHandle));
    vTaskDelay(30+3/portTICK_PERIOD_MS);
    
}

}
