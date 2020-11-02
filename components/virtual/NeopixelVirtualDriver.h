#ifndef VARIABLES_H
#define VARIABLES_H
#pragma once

#include "esp_heap_caps.h"
#include "soc/soc.h"
#include "soc/gpio_sig_map.h"
#include "soc/i2s_reg.h"
#include "soc/i2s_struct.h"
#include "soc/io_mux_reg.h"
#include "driver/gpio.h"
#include "driver/periph_ctrl.h"
#include "esp32/rom/lldesc.h"
#include "esp_intr_alloc.h"
#include "esp_log.h"
#include <soc/rtc.h>
#include <cstring>
#include "sdkconfig.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/semphr.h"
//#include "colorarrangement.h"
#include "esp_attr.h"
#define NUM_VIRT_PINS 7
#ifndef NBIS2SERIALPINS
#define NBIS2SERIALPINS 1
#endif
#ifndef NUM_LEDS_PER_STRIP
#define NUM_LEDS_PER_STRIP 256
#endif
#define OFFSET NUM_VIRT_PINS + 1
#define I2S_OFF (NUM_VIRT_PINS + 1 )* NUM_LEDS_PER_STRIP
#define I2S_OFF2 I2S_OFF * NBIS2SERIALPINS - NUM_LEDS_PER_STRIP
#define AA (0x00AA00AAL)
#define CC (0x0000CCCCL)
#define FF (0xF0F0F0F0L)
#define FF2 (0x0F0F0F0FL)


#ifndef I2S_DEVICE
#define I2S_DEVICE 0
#endif

#ifndef COLOR_SIZE
#define COLOR_SIZE 3
#endif

extern intr_handle_t gI2S_intr_handle;// = NULL;
static xSemaphoreHandle gTX_sem = NULL;

// -- A pointer to the memory-mapped structure: I2S0 or I2S1


// -- I2S goes to these pins until we remap them using the GPIO matrix
static int i2s_base_pin_index;

// --- I2S DMA buffers
struct DMABuffer {
    lldesc_t descriptor;
    uint8_t * buffer;
};

#define NUM_DMA_BUFFERS 4
 extern DMABuffer * dmaBuffers[800];
extern int testvar2;
static volatile  int ledToDisplay;
typedef union {
    uint8_t bytes[16];
    uint32_t shorts[8];
    uint32_t raw[2];
} Lines;

static  int brightness_b;
static  int brightness_r;
static  int brightness_g;
static uint8_t green_map[256];
static uint8_t blue_map[256];
static uint8_t red_map[256];
static i2s_dev_t * i2s;
static volatile uint32_t qsd;
static  int dmaBufferCount=2;

class NeopixelVirtualDriver {
    
    const int deviceBaseIndex[2] = {I2S0O_DATA_OUT0_IDX, I2S1O_DATA_OUT0_IDX};
    const int deviceClockIndex[2] = {I2S0O_BCK_OUT_IDX, I2S1O_BCK_OUT_IDX};
    const int deviceWordSelectIndex[2] = {I2S0O_WS_OUT_IDX, I2S1O_WS_OUT_IDX};
    const periph_module_t deviceModule[2] = {PERIPH_I2S0_MODULE, PERIPH_I2S1_MODULE};
    public:
   
    NeopixelVirtualDriver(){
    };
    
     void setPins(int LATCH_PIN,int CLOCK_PIN, int * Pins)
    {
        printf("nb of serial pin:%d\n",NBIS2SERIALPINS);
     for (int i = 0; i < NBIS2SERIALPINS; i++)
            if (Pins[i] > -1)
            {
                PIN_FUNC_SELECT(GPIO_PIN_MUX_REG[Pins[i]], PIN_FUNC_GPIO);
                gpio_set_direction((gpio_num_t)Pins[i], (gpio_mode_t)GPIO_MODE_DEF_OUTPUT);
                //pinMode(Pins[i],OUTPUT);
                gpio_matrix_out(Pins[i], deviceBaseIndex[I2S_DEVICE] + i+8, false, false);
            }
        
        //latch pin
        PIN_FUNC_SELECT(GPIO_PIN_MUX_REG[LATCH_PIN], PIN_FUNC_GPIO);
        gpio_set_direction((gpio_num_t)LATCH_PIN, (gpio_mode_t)GPIO_MODE_DEF_OUTPUT);
        //pinMode(LATCH_PIN,OUTPUT);
        gpio_matrix_out(LATCH_PIN, deviceBaseIndex[I2S_DEVICE] + 23, false, false);
        //if (baseClock > -1)
        //clock pin
        gpio_matrix_out(CLOCK_PIN, deviceClockIndex[I2S_DEVICE], false, false);
        printf("start is2init\n");
        i2sInit();
        printf("ofert init\n");
    }

    void showPixel()
    {
        // pu((uint16_t*)dmaBuffers[0]->buffer); //latch
        //         pu((uint16_t*)dmaBuffers[1]->buffer);
        //         pu((uint16_t*)dmaBuffers[2]->buffer);
        //         //pu((uint32_t*)this->dmaBuffers[3]->buffer);
        //         pu2((uint16_t*)dmaBuffers[0]->buffer); //first pulse
        //         pu2((uint16_t*)dmaBuffers[1]->buffer);


        //dmaBuffers[1]->descriptor.qe.stqe_next = &(dmaBuffers[0]->descriptor);
          //Ò dmaBuffers[2]->descriptor.qe.stqe_next = &(dmaBuffers[0]->descriptor);
          // dmaBuffers[0]->descriptor.qe.stqe_next = &(dmaBuffers[1]->descriptor);
           //runningPixel=true;
           //startTX();
             total=0;
     end=0;
     eof=0;
           i2sStart();
    }
    
   uint32_t get()
    {
        return testint;
    }
    uint32_t testint;
    uint32_t total=0;
    uint32_t end=0;
    uint32_t eof=0;
private:
    static void IRAM_ATTR  interruptHandler(void *arg);
    static void transpose16x1_noinline2(unsigned char *A, uint8_t *B) ;
    
   
   
    uint8_t * int_leds;
    DMABuffer * allocateDMABuffer(int bytes);
//     DMABuffer * allocateDMABuffer(int bytes)
//    {
//        DMABuffer * b = (DMABuffer *)heap_caps_malloc(sizeof(DMABuffer), MALLOC_CAP_DMA);
//
//        b->buffer = (uint8_t *)heap_caps_malloc(bytes, MALLOC_CAP_DMA);
//        memset(b->buffer, 0, bytes);
//
//        b->descriptor.length = bytes;
//        b->descriptor.size = bytes;
//        b->descriptor.owner = 1;
//        b->descriptor.sosf = 1;
//        b->descriptor.buf = b->buffer;
//        b->descriptor.offset = 0;
//        b->descriptor.empty = 0;
//        b->descriptor.eof = 1;
//        b->descriptor.qe.stqe_next = 0;
//
//        return b;
//    }
    
     void i2sInit()
    {

        int interruptSource;
        if (I2S_DEVICE == 0) {
            i2s = &I2S0;
            periph_module_enable(PERIPH_I2S0_MODULE);
            interruptSource = ETS_I2S0_INTR_SOURCE;
            i2s_base_pin_index = I2S0O_DATA_OUT0_IDX;
        } else {
            i2s = &I2S1;
            periph_module_enable(PERIPH_I2S1_MODULE);
            interruptSource = ETS_I2S1_INTR_SOURCE;
            i2s_base_pin_index = I2S1O_DATA_OUT0_IDX;
        }
        
        printf("here1\n");
        i2sReset();
        printf("here2\n");
        i2sReset_DMA();
        printf("here3\n");
        i2sReset_FIFO();
        printf("here4\n");
        i2s->conf.tx_right_first = 0;
        
        // -- Set parallel mode
        i2s->conf2.val = 0;
        i2s->conf2.lcd_en = 1;
        i2s->conf2.lcd_tx_wrx2_en = 1; // 0 for 16 or 32 parallel output
        i2s->conf2.lcd_tx_sdx2_en = 0; // HN
        
        // -- Set up the clock rate and sampling
        i2s->sample_rate_conf.val = 0;
        i2s->sample_rate_conf.tx_bits_mod = 16; // Number of parallel bits/pins
        i2s->sample_rate_conf.tx_bck_div_num = 1;
        i2s->clkm_conf.val = 0;

        i2s->clkm_conf.clka_en = 0;
        
        
        //rtc_clk_apll_enable(true, 10, 133,7, 1); //19.2Mhz 7 pins +1 latchrtc_clk_apll_enable(true, 31, 133,7, 1); //19.2Mhz 7 pins +1 latch
       // printf("here2\n");
        i2s->clkm_conf.clkm_div_a =1;// CLOCK_DIVIDER_A;
        i2s->clkm_conf.clkm_div_b = 32;//CLOCK_DIVIDER_B;
        i2s->clkm_conf.clkm_div_num = 63;//CLOCK_DIVIDER_N;
        //printf("here3\n");
        i2s->fifo_conf.val = 0;
        i2s->fifo_conf.tx_fifo_mod_force_en = 1;
        i2s->fifo_conf.tx_fifo_mod = 1;  // 16-bit single channel data
        i2s->fifo_conf.tx_data_num = 32;//32; // fifo length
        i2s->fifo_conf.dscr_en = 1;      // fifo will use dma
        
        i2s->conf1.val = 0;
        i2s->conf1.tx_stop_en = 0;
        i2s->conf1.tx_pcm_bypass = 1;
        
        i2s->conf_chan.val = 0;
        i2s->conf_chan.tx_chan_mod = 1; // Mono mode, with tx_msb_right = 1, everything goes to right-channel
        
        i2s->timing.val = 0;
        
        
        //rintf("here4\n");
                         for(int i=0;i<4;i++)
                {
                    dmaBuffers[i] = allocateDMABuffer(3*2*8*3);
                    //if(i>0)
                   // dmaBuffers[i-1]->descriptor.qe.stqe_next = &(dmaBuffers[i]->descriptor);
                }
        // -- Allocate two DMA buffers
        // dmaBuffers[0] = allocateDMABuffer((NUM_VIRT_PINS+1)*3*8*3*2);
        // dmaBuffers[1] = allocateDMABuffer((NUM_VIRT_PINS+1)*3*8*3*2);
        // dmaBuffers[2] = allocateDMABuffer((NUM_VIRT_PINS+1)*3*8*3*2);
        
        // -- Arrange them as a circularly linked list
        dmaBuffers[0]->descriptor.qe.stqe_next = &(dmaBuffers[1]->descriptor);
       dmaBuffers[1]->descriptor.qe.stqe_next = &(dmaBuffers[2]->descriptor);
//setting up first and last buffer to one Os if you follow using a oscillo

                dmaBuffers[0]->buffer[2]=0xff;
        dmaBuffers[0]->buffer[3]=0xff;
        
             dmaBuffers[0]->buffer[24*2*3-3]=0xff;
        dmaBuffers[0]->buffer[24*2*3-4]=0xff; 


                dmaBuffers[1]->buffer[2]=0xff;
        dmaBuffers[1]->buffer[3]=0xff;
        
             dmaBuffers[1]->buffer[24*2*3-3]=0xff;
        dmaBuffers[1]->buffer[24*2*3-4]=0xff;


                dmaBuffers[2]->buffer[2]=0xff;
        dmaBuffers[2]->buffer[3]=0xff;
        
             dmaBuffers[2]->buffer[24*2*3-3]=0xff;
        dmaBuffers[2]->buffer[24*2*3-4]=0xff;
       // dmaBuffers[1]->descriptor.qe.stqe_next = &(dmaBuffers[2]->descriptor);
        // pu((uint16_t*)dmaBuffers[0]->buffer); //latch
        // pu((uint16_t*)dmaBuffers[1]->buffer);
        // pu((uint16_t*)dmaBuffers[2]->buffer);
        // pu2((uint16_t*)dmaBuffers[0]->buffer); //first pulse
        // pu2((uint16_t*)dmaBuffers[1]->buffer);
       
        i2s->int_ena.val = 0;
        // -- Allocate i2s interrupt
        SET_PERI_REG_BITS(I2S_INT_ENA_REG(I2S_DEVICE), I2S_OUT_EOF_INT_ENA_V,1, I2S_OUT_EOF_INT_ENA_S);
        SET_PERI_REG_BITS(I2S_INT_ENA_REG(I2S_DEVICE), I2S_OUT_TOTAL_EOF_INT_ENA_V, 1, I2S_OUT_TOTAL_EOF_INT_ENA_S);
        esp_err_t e = esp_intr_alloc(interruptSource, ESP_INTR_FLAG_INTRDISABLED | ESP_INTR_FLAG_LEVEL3 | ESP_INTR_FLAG_IRAM , // ESP_INTR_FLAG_INTRDISABLED | ESP_INTR_FLAG_LEVEL3,
                                     &interruptHandler, this, &gI2S_intr_handle);
        
        // -- Create a semaphore to block execution until all the controllers are done
        /*
         if (gTX_sem == NULL) {
         gTX_sem = xSemaphoreCreateBinary();
         xSemaphoreGive(gTX_sem);
         }
         */
        // Serial.println("Init I2S");
        //gInitialized = true;
    }
    
    
    
    static void pu(uint16_t* buff);

    
    static void pu3(uint16_t* buff);
    static  void pu2(uint16_t* buff);
        
    
   
   static void i2sStart();
//    static void i2sStart()
//   {
//    // esp_intr_disable(gI2S_intr_handle);
//    printf("I2S start\n");
//    i2sReset();
//    printf("I2S start\n");
//    //Serial.println(dmaBuffers[0]->sampleCount());
//    (&I2S0)->lc_conf.val=I2S_OUT_DATA_BURST_EN | I2S_OUTDSCR_BURST_EN | I2S_OUT_DATA_BURST_EN;
//    //i2s.rx_eof_num = dmaBuffers[2]->sampleCount();
//    printf("I2S start\n");
//    (&I2S0)->out_link.addr = (uint32_t) & (dmaBuffers[0]->descriptor);
//    printf("I2S start\n");
//    //i2s.in_link.addr = (uint32_t) & (dmaBuffers[2]->descriptor);
//    (&I2S0)->out_link.start = 1;
//    ////vTaskDelay(5);
//    (&I2S0)->int_clr.val = (&I2S0)->int_raw.val;
//    // //vTaskDelay(5);
//   // i2s->int_ena.out_dscr_err = 1;
//    //enable interrupt
//    ////vTaskDelay(5);
//   esp_intr_enable(gI2S_intr_handle);
//    // //vTaskDelay(5);
//    //i2s->int_ena.val = 0;
//    //    i2s->int_ena.out_eof = 1;
//    //i2s->int_ena.out_done = 1;
//    printf("I2S start\n");
//    //start transmission
//    (&I2S0)->conf.tx_start = 1;
//    printf("I2S start\n");
//   }
    

   static void i2sReset();
    

   static  void i2sReset_DMA();
    
    static void i2sReset_FIFO();
   static  void i2sStop();
    
    
  //  void NeopixelVirtualDriver::i2sReset()
  //{
  //    // Serial.println("I2S reset");
  //    const unsigned long lc_conf_reset_flags = I2S_IN_RST_M | I2S_OUT_RST_M | I2S_AHBM_RST_M | I2S_AHBM_FIFO_RST_M;
  //    i2s->lc_conf.val |= lc_conf_reset_flags;
  //    i2s->lc_conf.val &= ~lc_conf_reset_flags;
  //
  //    const uint32_t conf_reset_flags = I2S_RX_RESET_M | I2S_RX_FIFO_RESET_M | I2S_TX_RESET_M | I2S_TX_FIFO_RESET_M;
  //    i2s->conf.val |= conf_reset_flags;
  //    i2s->conf.val &= ~conf_reset_flags;
  //}




};

//include "NeopixelVirtualDriver.cpp"
#endif
