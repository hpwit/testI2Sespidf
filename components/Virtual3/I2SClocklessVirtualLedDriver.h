
/*
 
 */

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
#include <cstring>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/semphr.h"
#include <stdio.h>
#include <esp32/rom/ets_sys.h>
#include "esp_log.h"
#include <soc/rtc.h>





#define I2S_DEVICE 0

#define NUM_VIRT_PINS 7
#ifndef NBIS2SERIALPINS
#define NBIS2SERIALPINS 1
#endif
#ifndef NUM_LEDS_PER_STRIP
#define NUM_LEDS_PER_STRIP 256
#endif


#ifdef STATIC_COLOR_RGBW
        #define p_r 1
        #define p_g 0
        #define p_b 2
         #define nb_components 4
    #else
  #ifdef STATIC_COLOR_RGB
        #define p_r 0
        #define p_g 1
        #define p_b 2
         #define nb_components 3
        #else
  #ifdef STATIC_COLOR_RBG
        #define p_r 0
        #define p_g 2
        #define p_b 1
         #define nb_components 3
        #else
 #ifdef STATIC_COLOR_GBR
        #define p_r 2
        #define p_g 0
        #define p_b 1
         #define nb_components 3
         #else
 #ifdef STATIC_COLOR_BGR
        #define p_r 2
        #define p_g 1
        #define p_b 0
         #define nb_components 3
        #else
 #ifdef STATIC_COLOR_BRG
        #define p_r 1
        #define p_g 2
        #define p_b 0
         #define nb_components 3
        #else
 #ifdef STATIC_COLOR_GRB
        #define p_r 1
        #define p_g 0
        #define p_b 2
        #define nb_components 3
        #else

            #define p_r 2
            #define p_g 0
            #define p_b 1
           #define nb_components 3   
#endif
#endif
#endif
#endif
#endif
#endif
#endif
   
#define OFFSET (NUM_VIRT_PINS + 1)
#define I2S_OFF (((NUM_VIRT_PINS + 1 )* NUM_LEDS_PER_STRIP)*nb_components)
#define I2S_OFF2 ((I2S_OFF * NBIS2SERIALPINS - NUM_LEDS_PER_STRIP*nb_components))
#define I2S_OFF3 ((I2S_OFF * NBIS2SERIALPINS + NUM_LEDS_PER_STRIP*nb_components))
#define I2S_OFF4 ((I2S_OFF * NBIS2SERIALPINS -  3 * NUM_LEDS_PER_STRIP*nb_components))
#define AA (0x00AA00AAL)
#define CC (0x0000CCCCL)
#define FF (0xF0F0F0F0L)
#define FF2 (0x0F0F0F0FL)

typedef union {
    uint8_t bytes[16];
    uint32_t shorts[8];
    uint32_t raw[2];
} Lines;
static const char* TAG = "I2SClocklessVirtualLedDriver";

static void IRAM_ATTR _I2SClocklessVirtualLedDriverinterruptHandler(void *arg);
//static void IRAM_ATTR transpose16x1_noinline2(unsigned char *A, uint16_t *B);
static void IRAM_ATTR transpose16x1_noinline2(unsigned char *A, uint8_t *B) {
    
    uint32_t  x, y, x1,y1,t;
    
    y = *(unsigned int*)(A);
    x = *(unsigned int*)(A+4);


    y1 = *(unsigned int*)(A+8);
#if NBIS2SERIALPINS >= 12
     x1 = *(unsigned int*)(A+12);
#else
    x1=0;
#endif
    
    
    
    // pre-transform x
    t = (x ^ (x >> 7)) & AA;  x = x ^ t ^ (t << 7);
    t = (x ^ (x >>14)) & CC;  x = x ^ t ^ (t <<14);
#if NBIS2SERIALPINS >= 12
       t = (x1 ^ (x1 >> 7)) & AA;  x1 = x1 ^ t ^ (t << 7);
       t = (x1 ^ (x1 >>14)) & CC;  x1 = x1 ^ t ^ (t <<14);
#endif
    // pre-transform y
    t = (y ^ (y >> 7)) & AA;  y = y ^ t ^ (t << 7);
    t = (y ^ (y >>14)) & CC;  y = y ^ t ^ (t <<14);
    t = (y1 ^ (y1 >> 7)) & AA;  y1 = y1 ^ t ^ (t << 7);
    t = (y1 ^ (y1 >>14)) & CC;  y1 = y1 ^ t ^ (t <<14);
    
    
    // final transform
    t = (x & FF) | ((y >> 4) & FF2);
    y = ((x << 4) & FF) | (y & FF2);
    x = t;
    
#if NBIS2SERIALPINS >= 12
        t= (x1 & FF) | ((y1 >> 4) & FF2);
        y1 = ((x1 << 4) & FF) | (y1 & FF2);
        x1 = t;
#else
    x1 =((y1 >> 4) & FF2);
    y1 =  (y1 & FF2);
#endif
    
    
    *((uint16_t*)(B)) = (uint16_t)(((x & 0xff000000) >>8 |((x1&0xff000000) ))>>16);
    *((uint16_t*)(B+48)) = (uint16_t)( ((x & 0xff0000) >>16|((x1&0xff0000) >>8)));
    *((uint16_t*)(B+2*48)) = (uint16_t)(((x & 0xff00) |((x1&0xff00) <<8))>>8);
    *((uint16_t*)(B+3*48)) =(uint16_t)( (x & 0xff) |((x1&0xff) <<8));
    *((uint16_t*)(B+4*48)) = (uint16_t)(((y & 0xff000000) >>8 |((y1&0xff000000) ))>>16);
    *((uint16_t*)(B+5*48)) = (uint16_t)(((y & 0xff0000) |((y1&0xff0000) <<8))>>16);
    *((uint16_t*)(B+6*48)) = (uint16_t)(((y & 0xff00) |((y1&0xff00) <<8))>>8);
    *((uint16_t*)(B+7*48)) = (uint16_t)((y & 0xff) |  (  (y1 & 0xff) << 8 ) )   ;
    
}
static void IRAM_ATTR loadAndTranspose(uint8_t * ledt,uint16_t *buffer,int ledtodisp,uint8_t *mapg,uint8_t *mapr,uint8_t *mapb,uint8_t *mapw);


enum  colorarrangment{
    ORDER_GRBW,
    ORDER_RGB,
    ORDER_RBG,
    ORDER_GRB,
    ORDER_GBR,
    ORDER_BRG,
    ORDER_BGR,
} ;


enum displayMode {
    NO_WAIT,
    WAIT,
    LOOP,
    LOOP_INTERUPT,
};

class I2SClocklessVirtualLedDriver {
    
    
    struct I2SClocklessVirtualLedDriverDMABuffer {
        lldesc_t descriptor;
        uint8_t * buffer;
    };

    
    const int deviceBaseIndex[2] = {I2S0O_DATA_OUT0_IDX, I2S1O_DATA_OUT0_IDX};
    const int deviceClockIndex[2] = {I2S0O_BCK_OUT_IDX, I2S1O_BCK_OUT_IDX};
    const int deviceWordSelectIndex[2] = {I2S0O_WS_OUT_IDX, I2S1O_WS_OUT_IDX};
    const periph_module_t deviceModule[2] = {PERIPH_I2S0_MODULE, PERIPH_I2S1_MODULE};
public:
    i2s_dev_t * i2s;
    uint8_t __green_map[256];
    uint8_t __blue_map[256];
    uint8_t __red_map[256];
    uint8_t  __white_map[256];
     intr_handle_t _gI2SClocklessDriver_intr_handle;
    volatile xSemaphoreHandle I2SClocklessVirtualLedDriver_sem = NULL;
    volatile xSemaphoreHandle I2SClocklessVirtualLedDriver_semSync = NULL;
    volatile xSemaphoreHandle I2SClocklessVirtualLedDriver_semDisp= NULL;

    /*
     This flag is used when using the NO_WAIT modeÒÒ
     */
    volatile bool isDisplaying=false;
    volatile bool isWaiting=true;
     volatile bool framesync=false;
     volatile int counti;
    
    
    I2SClocklessVirtualLedDriver(){};
    void setPins( int * Pins,int CLOCK_PIN,int LATCH_PIN)
    {
        for (int i = 0; i < NBIS2SERIALPINS ;i++)
        {
            PIN_FUNC_SELECT(GPIO_PIN_MUX_REG[Pins[i]], PIN_FUNC_GPIO);
            gpio_set_direction((gpio_num_t)Pins[i], (gpio_mode_t)GPIO_MODE_DEF_OUTPUT);
            gpio_matrix_out(Pins[i], deviceBaseIndex[I2S_DEVICE] + i+8, false, false);
        }
         PIN_FUNC_SELECT(GPIO_PIN_MUX_REG[LATCH_PIN], PIN_FUNC_GPIO);
         gpio_set_direction((gpio_num_t)LATCH_PIN, (gpio_mode_t)GPIO_MODE_DEF_OUTPUT);
            gpio_matrix_out(LATCH_PIN, deviceBaseIndex[I2S_DEVICE] + NBIS2SERIALPINS+8, false, false);
             gpio_set_direction((gpio_num_t)CLOCK_PIN, (gpio_mode_t)GPIO_MODE_DEF_OUTPUT);
        gpio_matrix_out(CLOCK_PIN, deviceClockIndex[I2S_DEVICE], false, false);    
    }
    
    void setBrightness(int brightness)
    {
        for(int i=0;i<256;i++)
        {
            __green_map[i]=(uint8_t)((int)(i*brightness)/255);
            __blue_map[i]=(uint8_t)((int)(i*brightness)/255);
            __red_map[i]=(uint8_t)((int)(i*brightness)/255);
            __white_map[i]=(uint8_t)((int)(i*brightness)/255);
           
        }
    }

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
        
        i2sReset();
        i2sReset_DMA();
        i2sReset_FIFO();
        i2s->conf.tx_right_first = 0;
        
        // -- Set parallel mode
        i2s->conf2.val = 0;
        i2s->conf2.lcd_en = 1;
        i2s->conf2.lcd_tx_wrx2_en = 1; // 0 for 16 or 32 parallel output
        i2s->conf2.lcd_tx_sdx2_en = 0; // HN
        
        // -- Set up the clock rate and sampling
        i2s->sample_rate_conf.val = 0;
        i2s->sample_rate_conf.tx_bits_mod = 16; // Number of parallel bits/pins
        i2s->clkm_conf.val = 0;
        //i2s->sample_rate_conf.tx_bck_div_num = 1;
#ifdef DL_CLK
           // Serial.println("norml clock");
                    i2s->clkm_conf.clka_en = 0;
                    //rtc_clk_apll_enable(true, 31, 133,7, 1); //19.2Mhz 7 pins +1 latchrtc_clk_apll_enable(true, 31, 133,7, 1); //19.2Mhz 7 pins +1 latch
                    
                    // -- Data clock is computed as Base/(div_num + (div_b/div_a))
                    //    Base is 80Mhz, so 80/(3+ 7/6) = 19.2Mhz
                   
                    i2s->clkm_conf.clkm_div_a =6;// CLOCK_DIVIDER_A;
                    i2s->clkm_conf.clkm_div_b = 7;//CLOCK_DIVIDER_B;
                    i2s->clkm_conf.clkm_div_num = 3;//CLOCK_DIVIDER_N;
            
    #else
             //Serial.println("precise clock");
            i2s->clkm_conf.clka_en = 1;
            rtc_clk_apll_enable(true, 31, 133,7, 1); //19.2Mhz 7 pins +1 latchrtc_clk_apll_enable(true, 31, 133,7, 1); //19.2Mhz 7 pins +1 latch

            i2s->clkm_conf.clkm_div_a =1;// CLOCK_DIVIDER_A;
            i2s->clkm_conf.clkm_div_b = 0;//CLOCK_DIVIDER_B;
            i2s->clkm_conf.clkm_div_num = 1;//CLOCK_DIVIDER_N;
    #endif
        i2s->fifo_conf.val = 0;
        i2s->fifo_conf.tx_fifo_mod_force_en = 1;
        i2s->fifo_conf.tx_fifo_mod = 1;  // 16-bit single channel data
        i2s->fifo_conf.tx_data_num = 32;//32; // fifo length
        i2s->fifo_conf.dscr_en = 1;      // fifo will use dma
        i2s->sample_rate_conf.tx_bck_div_num = 1;
        i2s->conf1.val = 0;
        i2s->conf1.tx_stop_en = 0;
        i2s->conf1.tx_pcm_bypass = 1;
        
        i2s->conf_chan.val = 0;
        i2s->conf_chan.tx_chan_mod = 1; // Mono mode, with tx_msb_right = 1, everything goes to right-channel
        
        i2s->timing.val = 0;
        //i2s->int_ena.val = 0;
        /*
        // -- Allocate i2s interrupt
        SET_PERI_REG_BITS(I2S_INT_ENA_REG(I2S_DEVICE), I2S_OUT_EOF_INT_ENA_V,1, I2S_OUT_EOF_INT_ENA_S);
        SET_PERI_REG_BITS(I2S_INT_ENA_REG(I2S_DEVICE), I2S_OUT_TOTAL_EOF_INT_ENA_V, 1, I2S_OUT_TOTAL_EOF_INT_ENA_S);
        SET_PERI_REG_BITS(I2S_INT_ENA_REG(I2S_DEVICE), I2S_OUT_TOTAL_EOF_INT_ENA_V, 1, I2S_OUT_TOTAL_EOF_INT_ENA_S);
        */
        esp_err_t e = esp_intr_alloc(interruptSource, ESP_INTR_FLAG_INTRDISABLED | ESP_INTR_FLAG_LEVEL3 | ESP_INTR_FLAG_IRAM , &_I2SClocklessVirtualLedDriverinterruptHandler, this, &_gI2SClocklessDriver_intr_handle);
        
        // -- Create a semaphore to block execution until all the controllers are done
        
        if (I2SClocklessVirtualLedDriver_sem == NULL) {
            I2SClocklessVirtualLedDriver_sem = xSemaphoreCreateBinary();
            
        }
                
        if (I2SClocklessVirtualLedDriver_semSync == NULL) {
            I2SClocklessVirtualLedDriver_semSync = xSemaphoreCreateBinary();
            
        }
        if (I2SClocklessVirtualLedDriver_semDisp == NULL) {
            I2SClocklessVirtualLedDriver_semDisp = xSemaphoreCreateBinary();
            
        }
    }
    
    void initDMABuffers()
    {
        DMABuffersTampon[0] = allocateDMABuffer((NUM_VIRT_PINS+1)*nb_components*8*3*2); //the buffers for the
        DMABuffersTampon[1] = allocateDMABuffer((NUM_VIRT_PINS+1)*nb_components*8*3*2);
        DMABuffersTampon[2] = allocateDMABuffer((NUM_VIRT_PINS+1)*nb_components*8*3*2);
        DMABuffersTampon[3] = allocateDMABuffer((NUM_VIRT_PINS+1)*nb_components*8*3*2*4);
        
        putdefaultlatch((uint16_t*)DMABuffersTampon[2]->buffer);
        putdefaultlatch((uint16_t*)DMABuffersTampon[3]->buffer);
        putdefaultlatch((uint16_t*)DMABuffersTampon[0]->buffer );
        putdefaultlatch((uint16_t*)DMABuffersTampon[1]->buffer );
        putdefaultones((uint16_t*)DMABuffersTampon[0]->buffer );
        putdefaultones((uint16_t*)DMABuffersTampon[1]->buffer );
        
        
#ifdef FULL_DMA_BUFFER
        /*
         We do create n+2 buffers
         the first buffer is to be sure that everything is 0
         the last one is to put back the I2S at 0 the last bufffer is longer because when using the loop display mode the time between two frames needs to be longh enough.
         */
        DMABuffersTransposed=(I2SClocklessVirtualLedDriverDMABuffer **)malloc(sizeof(I2SClocklessVirtualLedDriverDMABuffer *) * (num_led_per_strip+2));
        for(int i=0;i<num_led_per_strip+2;i++)
        {
            if(i<num_led_per_strip+1)
                DMABuffersTransposed[i]=allocateDMABuffer(nb_components*8*2*3);
            else
                DMABuffersTransposed[i]=allocateDMABuffer(nb_components*8*2*3*4);
            if(i<num_led_per_strip)
                DMABuffersTransposed[i]->descriptor.eof=0;
            if (i)
            {
                DMABuffersTransposed[i - 1]->descriptor.qe.stqe_next=&(DMABuffersTransposed[i]->descriptor);
                if(i<num_led_per_strip+1)
                {
                    putdefaultones((uint16_t*)DMABuffersTransposed[i]->buffer );
                    
                }
            }
        }
#endif
        
        
    }
    
    
#ifdef FULL_DMA_BUFFER
    
    void stopDisplayLoop()
    {
        DMABuffersTransposed[num_led_per_strip+1]->descriptor.qe.stqe_next=0;
    }
    
    void showPixelsFromBuffer()
    {
        showPixelsFromBuffer(NO_WAIT);
    }
    
    
    
    void showPixelsFromBuffer(displayMode dispmode)
    {
        /*
         We cannot launch twice when in loopmode
         */
        if(__displayMode==LOOP && isDisplaying)
        {
            ESP_LOGE(TAG,"The loop mode is activated execute stopDisplayLoop() first");
            return;
        }
        /*
         We wait for the display to be stopped before launching a new one
         */
        if(__displayMode==NO_WAIT && isDisplaying==true)
            xSemaphoreTake(I2SClocklessVirtualLedDriver_semDisp,portMAX_DELAY);
        __displayMode=dispmode;
        isWaiting=false;
        if(dispmode==LOOP or dispmode==LOOP_INTERUPT)
        {
            DMABuffersTransposed[num_led_per_strip+1]->descriptor.qe.stqe_next=&(DMABuffersTransposed[0]->descriptor);
        }
        transpose=false;
        i2sStart(DMABuffersTransposed[0]);
        
        if(dispmode==WAIT)
        {
            isWaiting=true;
            xSemaphoreTake(I2SClocklessVirtualLedDriver_sem, portMAX_DELAY);
            
        }
    }
    
    
    
    
    void showPixelsFirstTranpose()
    {
        showPixelsFirstTranpose(NO_WAIT);
    }
    void showPixelsFirstTranpose(displayMode dispmode)
    {
        if(leds==NULL)
        {
            printf("no leds buffer defined");
            return;
        }
        transposeAll();
        showPixelsFromBuffer(dispmode);
    }
    
    void transposeAll()
    {
        ledToDisplay=0;
        Lines secondPixel[nb_components];
        for(int j=0;j<num_led_per_strip;j++)
        {
            uint8_t *poli=leds+ledToDisplay*nb_components;
            for(int i = 0; i < num_strips; i++) {
                
                secondPixel[p_g].bytes[i] = __green_map[*(poli+1)];
                secondPixel[p_r].bytes[i] = __red_map[*(poli+0)];
                secondPixel[p_b].bytes[i] = __blue_map[*(poli+2)];
                if (nb_components>3)
                    secondPixel[3].bytes[i] = __white_map[*(poli+3)];
                //#endif
                poli+=num_led_per_strip*nb_components;
                
            }
            ledToDisplay++;
            transpose16x1_noinline2(secondPixel[0].bytes,(uint16_t*)DMABuffersTransposed[j+1]->buffer );
            transpose16x1_noinline2(secondPixel[1].bytes,(uint16_t*)DMABuffersTransposed[j+1]->buffer+3*8);
            transpose16x1_noinline2(secondPixel[2].bytes,(uint16_t*)DMABuffersTransposed[j+1]->buffer+2*3*8);
            if (nb_components>3)
                transpose16x1_noinline2(secondPixel[3].bytes,(uint16_t*)DMABuffersTransposed[j+1]->buffer+3*3*8);
            
            
        }
    }
    
    
    void setPixelinBuffer(uint32_t pos, uint8_t red, uint8_t green, uint8_t blue,uint8_t white)
    {
        uint32_t stripNumber=pos/num_led_per_strip;
        uint32_t posOnStrip=pos%num_led_per_strip;
        
        uint16_t mask=~( 1 << stripNumber);
        
        uint8_t colors[3];
        colors[p_g]=__green_map[green];
        colors[p_r]=__red_map[red];
        colors[p_b]=__blue_map[blue];
        uint16_t *B=(uint16_t*)DMABuffersTransposed[posOnStrip+1]->buffer;
       // printf("nb c:%d\n",nb_components);
        uint8_t y=colors[0];
        *((uint16_t*)(B)) =   (*((uint16_t*)(B))& mask) | ((uint16_t)((y &   128)>>7) <<stripNumber);
        *((uint16_t*)(B+5)) =   (*((uint16_t*)(B+5))& mask) | ((uint16_t)((y & 64)>>6) <<stripNumber);
        *((uint16_t*)(B+6)) =   (*((uint16_t*)(B+6))& mask) | ((uint16_t)((y & 32)>>5) <<stripNumber);
        *((uint16_t*)(B+11)) =   (*((uint16_t*)(B+11))& mask) | ((uint16_t)((y& 16)>>4)<<stripNumber);
        *((uint16_t*)(B+12)) =   (*((uint16_t*)(B+12))& mask) | ((uint16_t)((y& 8)>>3) <<stripNumber);
        *((uint16_t*)(B+17)) =   (*((uint16_t*)(B+17))& mask) | ((uint16_t)((y& 4)>>2) <<stripNumber);
        *((uint16_t*)(B+18)) =   (*((uint16_t*)(B+18))& mask) | ((uint16_t)((y& 2)>>1) <<stripNumber);
        *((uint16_t*)(B+23)) =   (*((uint16_t*)(B+23))& mask) | ((uint16_t)(y & 1) <<stripNumber);
        
        B+=3*8;
        y=colors[1];
        *((uint16_t*)(B)) =   (*((uint16_t*)(B))& mask) | ((uint16_t)(( y&   128)>>7) <<stripNumber);
        *((uint16_t*)(B+5)) =   (*((uint16_t*)(B+5))& mask) | ((uint16_t)((y & 64)>>6) <<stripNumber);
        *((uint16_t*)(B+6)) =   (*((uint16_t*)(B+6))& mask) | ((uint16_t)((y & 32)>>5) <<stripNumber);
        *((uint16_t*)(B+11)) =   (*((uint16_t*)(B+11))& mask) | ((uint16_t)((y& 16)>>4)<<stripNumber);
        *((uint16_t*)(B+12)) =   (*((uint16_t*)(B+12))& mask) | ((uint16_t)((y& 8)>>3) <<stripNumber);
        *((uint16_t*)(B+17)) =   (*((uint16_t*)(B+17))& mask) | ((uint16_t)((y& 4)>>2) <<stripNumber);
        *((uint16_t*)(B+18)) =   (*((uint16_t*)(B+18))& mask) | ((uint16_t)((y& 2)>>1) <<stripNumber);
        *((uint16_t*)(B+23)) =   (*((uint16_t*)(B+23))& mask) | ((uint16_t)(y & 1) <<stripNumber);
        
        B+=3*8;
        y=colors[2];
        *((uint16_t*)(B)) =   (*((uint16_t*)(B))& mask) | ((uint16_t)(( y&   128)>>7) <<stripNumber);
        *((uint16_t*)(B+5)) =   (*((uint16_t*)(B+5))& mask) | ((uint16_t)((y & 64)>>6) <<stripNumber);
        *((uint16_t*)(B+6)) =   (*((uint16_t*)(B+6))& mask) | ((uint16_t)((y & 32)>>5) <<stripNumber);
        *((uint16_t*)(B+11)) =   (*((uint16_t*)(B+11))& mask) | ((uint16_t)((y& 16)>>4)<<stripNumber);
        *((uint16_t*)(B+12)) =   (*((uint16_t*)(B+12))& mask) | ((uint16_t)((y& 8)>>3) <<stripNumber);
        *((uint16_t*)(B+17)) =   (*((uint16_t*)(B+17))& mask) | ((uint16_t)((y& 4)>>2) <<stripNumber);
        *((uint16_t*)(B+18)) =   (*((uint16_t*)(B+18))& mask) | ((uint16_t)((y& 2)>>1) <<stripNumber);
        *((uint16_t*)(B+23)) =   (*((uint16_t*)(B+23))& mask) | ((uint16_t)(y & 1) <<stripNumber);
        if(nb_components>3)
        {
            B+=3*8;
            y=__white_map[white];
            *((uint16_t*)(B)) =   (*((uint16_t*)(B))& mask) | ((uint16_t)(( y&   128)>>7) <<stripNumber);
            *((uint16_t*)(B+5)) =   (*((uint16_t*)(B+5))& mask) | ((uint16_t)((y & 64)>>6) <<stripNumber);
            *((uint16_t*)(B+6)) =   (*((uint16_t*)(B+6))& mask) | ((uint16_t)((y & 32)>>5) <<stripNumber);
            *((uint16_t*)(B+11)) =   (*((uint16_t*)(B+11))& mask) | ((uint16_t)((y& 16)>>4)<<stripNumber);
            *((uint16_t*)(B+12)) =   (*((uint16_t*)(B+12))& mask) | ((uint16_t)((y& 8)>>3) <<stripNumber);
            *((uint16_t*)(B+17)) =   (*((uint16_t*)(B+17))& mask) | ((uint16_t)((y& 4)>>2) <<stripNumber);
            *((uint16_t*)(B+18)) =   (*((uint16_t*)(B+18))& mask) | ((uint16_t)((y& 2)>>1) <<stripNumber);
            *((uint16_t*)(B+23)) =   (*((uint16_t*)(B+23))& mask) | ((uint16_t)(y & 1) <<stripNumber);
        }
    }
    
    void setPixelinBuffer(uint32_t pos, uint8_t red, uint8_t green, uint8_t blue)
    {
        setPixelinBuffer(pos,red,green,blue,0);
    }
    
    void initled(int * Pinsq,int num_strips,int num_led_per_strip,colorarrangment cArr)
    {
        initled(NULL,Pinsq,num_strips,num_led_per_strip,cArr);
    }
    void waitSync(){
        xSemaphoreTake(I2SClocklessVirtualLedDriver_semSync, portMAX_DELAY);
    }
#endif
    void setPixel(uint32_t pos, uint8_t red, uint8_t green, uint8_t blue,uint8_t white)
    {
        uint8_t *offset=leds+(pos<<2); //faster than doing * 4
        *(offset)=red;
        *(++offset)=green;
        *(++offset)=blue;
        *(++offset)=white;
        
    }
    
    void setPixel(uint32_t pos, uint8_t red, uint8_t green, uint8_t blue)
    {
        uint8_t *offset=leds+(pos<<1)+pos;
        *(offset)=red;
        *(++offset)=green;
        *(++offset)=blue;
    }
    
    void showPixels()
    {
        //printf("number:%d\n",NBIS2SERIALPINS);
        if(leds==NULL)
        {
            ESP_LOGE(TAG,"no leds buffer defined");
            return;
        }
        ledToDisplay=0;
        transpose=true;
        
        DMABuffersTampon[0]->descriptor.qe.stqe_next=&(DMABuffersTampon[1]->descriptor);
        DMABuffersTampon[1]->descriptor.qe.stqe_next=&(DMABuffersTampon[0]->descriptor);
        DMABuffersTampon[2]->descriptor.qe.stqe_next=&(DMABuffersTampon[0]->descriptor);
        DMABuffersTampon[3]->descriptor.qe.stqe_next=0;
        dmaBufferActive=0;
        loadAndTranspose(leds,(uint16_t*)DMABuffersTampon[0]->buffer,ledToDisplay,__green_map,__red_map,__blue_map,__white_map);
        
        dmaBufferActive=1;
        i2sStart(DMABuffersTampon[2]);
        
        isWaiting=true;
        xSemaphoreTake(I2SClocklessVirtualLedDriver_sem, portMAX_DELAY);
 
    }
    
    
    
    
    
    void initled(uint8_t *leds,int * Pinsq,int CLOCK_PIN,int LATCH_PIN)
    {

  
/*
        switch(cArr)
        {
            case ORDER_RGB:
                nb_components=3;
                p_r=0;
                p_g=1;
                p_b=2;
                break;
            case ORDER_RBG:
                nb_components=3;
                p_r=0;
                p_g=2;
                p_b=1;
                break;
            case ORDER_GRB:
                nb_components=3;
                p_r=1;
                p_g=0;
                p_b=2;
                break;
            case ORDER_GBR:
                nb_components=3;
                p_r=2;
                p_g=0;
                p_b=1;
                break;
            case ORDER_BRG:
                nb_components=3;
                p_r=1;
                p_g=2;
                p_b=0;
                break;
            case ORDER_BGR:
                nb_components=3;
                p_r=2;
                p_g=1;
                p_b=0;
                break;
            case ORDER_GRBW:
                nb_components=4;
                p_r=1;
                p_g=0;
                p_b=2;
                break;
        }
*/

        setBrightness(255);
        dmaBufferCount=2;
        this->leds=leds;
        this->num_led_per_strip=NUM_LEDS_PER_STRIP;
        //this->num_strips=num_strips;
        this->dmaBufferCount=dmaBufferCount;
        setPins(Pinsq,CLOCK_PIN,LATCH_PIN);
        i2sInit();
        initDMABuffers();
        
        
    }
    
//private:
    volatile int dmaBufferActive=0;
    volatile bool wait;
    displayMode __displayMode;
    volatile int ledToDisplay;
   // volatile int oo=0;
    uint8_t *leds;
    int dmaBufferCount=2; //we use two buffers
    volatile bool transpose=false;
    
    volatile  int num_strips;
    volatile  int num_led_per_strip;
    //int clock_pin;
    int brigthness;

    

    int i2s_base_pin_index;

    //int nb_components;

    I2SClocklessVirtualLedDriverDMABuffer ** DMABuffersTransposed=NULL;
    //buffer array for the regular way
    I2SClocklessVirtualLedDriverDMABuffer * DMABuffersTampon[4];

    
    I2SClocklessVirtualLedDriverDMABuffer * allocateDMABuffer(int bytes)
    {
        I2SClocklessVirtualLedDriverDMABuffer * b = (I2SClocklessVirtualLedDriverDMABuffer *)heap_caps_malloc(sizeof(I2SClocklessVirtualLedDriverDMABuffer), MALLOC_CAP_DMA);
        if(!b)
        {
            ESP_LOGE(TAG,"No more memory\n");
            return NULL;
        }
        
        b->buffer = (uint8_t *)heap_caps_malloc(bytes, MALLOC_CAP_DMA);
        if(!b->buffer)
        {
            ESP_LOGE(TAG,"No more memory\n");
            return NULL;
        }
        memset(b->buffer, 0, bytes);
        
        b->descriptor.length = bytes;
        b->descriptor.size = bytes;
        b->descriptor.owner = 1;
        b->descriptor.sosf = 1;
        b->descriptor.buf = b->buffer;
        b->descriptor.offset = 0;
        b->descriptor.empty = 0;
        b->descriptor.eof = 1;
        b->descriptor.qe.stqe_next = 0;
        
        return b;
    }
    
    void  i2sReset_DMA()
    {
        
        (&I2S0)->lc_conf.out_rst=1; (&I2S0)->lc_conf.out_rst=0;
    }
    
    void i2sReset_FIFO()
    {
        
        (&I2S0)->conf.tx_fifo_reset=1; (&I2S0)->conf.tx_fifo_reset=0;
    }
    
    void i2sStop()
    {
        
        //delay(1);
        ets_delay_us(16);
       
        
        esp_intr_disable(_gI2SClocklessDriver_intr_handle);
        i2sReset();
        
        (&I2S0)->conf.tx_start = 0;
        while((&I2S0)->conf.tx_start ==1)
        {

        }
        isDisplaying=false;
        /*
         We have finished to display the strips
         */
        //ets_delay_us(1000);
        if(isWaiting)
        {
           
            xSemaphoreGive(I2SClocklessVirtualLedDriver_sem);
          
        }
        
        xSemaphoreGive(I2SClocklessVirtualLedDriver_semDisp);
        
    }
    
    void putdefaultlatch(uint16_t * buff)
    {
        //printf("dd%d\n",NBIS2SERIALPINS);
        uint16_t mask1= 1<<NBIS2SERIALPINS;
        for (int i=0;i<24*nb_components;i++)
        {
            buff[NUM_VIRT_PINS+i*(NUM_VIRT_PINS+1)-1-5]=mask1;//0x8000;
            //buff[NUM_VIRT_PINS+i*(NUM_VIRT_PINS+1)]=0x02;
        }
    }
    
    void putdefaultones(uint16_t * buff)
    {

        uint16_t mas= 0xFFFF & (~( 0xffff << (NBIS2SERIALPINS)));
        //printf("mas%d\n",mas);
        for (int j=0;j<8*nb_components;j++)
        {
  
            buff[1+j*(3*(NUM_VIRT_PINS+1))]=0xFFFF;
            buff[0+j*(3*(NUM_VIRT_PINS+1))]=mas;
            buff[3+j*(3*(NUM_VIRT_PINS+1))]=mas;
            buff[2+j*(3*(NUM_VIRT_PINS+1))]=mas;
            buff[5+j*(3*(NUM_VIRT_PINS+1))]=mas;
            buff[4+j*(3*(NUM_VIRT_PINS+1))]=mas;
            buff[7+j*(3*(NUM_VIRT_PINS+1))]=mas;
            buff[6+j*(3*(NUM_VIRT_PINS+1))]=mas;

        }
        
        
    }
    
    /*
     Transpose the pixel, as the function is static and all the variables are not static or global, we need to provide all of them.
     */
    
    

    
//    void transpose16x1_noinline2(uint8_t y,uint16_t *B,uint16_t mask,uint16_t mask2,int stripNumber) {
//
//        *((uint16_t*)(B)) =   (*((uint16_t*)(B))& mask) | ((uint16_t)((y &   128)>>7) <<stripNumber);
//        *((uint16_t*)(B+5)) =   (*((uint16_t*)(B+5))& mask) | ((uint16_t)((y & 64)>>6) <<stripNumber);
//        *((uint16_t*)(B+6)) =   (*((uint16_t*)(B+6))& mask) | ((uint16_t)((y & 32)>>5) <<stripNumber);
//        *((uint16_t*)(B+11)) =   (*((uint16_t*)(B+11))& mask) | ((uint16_t)((y& 16)>>4)<<stripNumber);
//        *((uint16_t*)(B+12)) =   (*((uint16_t*)(B+12))& mask) | ((uint16_t)((y& 8)>>3) <<stripNumber);
//        *((uint16_t*)(B+17)) =   (*((uint16_t*)(B+17))& mask) | ((uint16_t)((y& 4)>>2) <<stripNumber);
//        *((uint16_t*)(B+18)) =   (*((uint16_t*)(B+18))& mask) | ((uint16_t)((y& 2)>>1) <<stripNumber);
//        *((uint16_t*)(B+23)) =   (*((uint16_t*)(B+23))& mask) | ((uint16_t)(y & 1) <<stripNumber);
//
//    }
    
    void i2sStart(I2SClocklessVirtualLedDriverDMABuffer  *startBuffer)
    {
        
        i2sReset();
        framesync=false;
        counti=0;
        
        (&I2S0)->lc_conf.val=I2S_OUT_DATA_BURST_EN | I2S_OUTDSCR_BURST_EN | I2S_OUT_DATA_BURST_EN;
        
        (&I2S0)->out_link.addr = (uint32_t) & (startBuffer->descriptor);
        
        (&I2S0)->out_link.start = 1;
        
        (&I2S0)->int_clr.val = (&I2S0)->int_raw.val;
        
        (&I2S0)->int_clr.val = (&I2S0)->int_raw.val;
        (&I2S0)->int_ena.val = 0;
        
        /*
         If we do not use the regular showpixels, then no need to activate the interupt at the end of each pixels
         */
        //if(transpose)
            (&I2S0)->int_ena.out_eof = 1;
        
        (&I2S0)->int_ena.out_total_eof=1;
        esp_intr_enable(_gI2SClocklessDriver_intr_handle);
        
        //We start the I2S
        (&I2S0)->conf.tx_start = 1;
        
        //Set the mode to indicate that we've started
        isDisplaying=true;
        
    }
    
    
    void i2sReset()
    {
        const unsigned long lc_conf_reset_flags = I2S_IN_RST_M | I2S_OUT_RST_M | I2S_AHBM_RST_M | I2S_AHBM_FIFO_RST_M;
        (&I2S0)->lc_conf.val |= lc_conf_reset_flags;
        (&I2S0)->lc_conf.val &= ~lc_conf_reset_flags;
        const uint32_t conf_reset_flags = I2S_RX_RESET_M | I2S_RX_FIFO_RESET_M | I2S_TX_RESET_M | I2S_TX_FIFO_RESET_M;
        (&I2S0)->conf.val |= conf_reset_flags;
        (&I2S0)->conf.val &= ~conf_reset_flags;
    }
    
    
   // static void IRAM_ATTR interruptHandler(void *arg);
    
};
static  void IRAM_ATTR  _I2SClocklessVirtualLedDriverinterruptHandler(void *arg)
{

   // REG_WRITE(I2S_INT_CLR_REG(0), (REG_READ(I2S_INT_RAW_REG( 0 )) & 0xffffffc0) | 0x3f);
    //return;
    I2SClocklessVirtualLedDriver *cont=(I2SClocklessVirtualLedDriver *)arg;
  
    if(GET_PERI_REG_BITS(I2S_INT_ST_REG(I2S_DEVICE), I2S_OUT_EOF_INT_ST_V,  I2S_OUT_EOF_INT_ST_S))
    {
        cont->framesync=!cont->framesync;

        if( ((I2SClocklessVirtualLedDriver *)arg)->transpose)
        {
            cont->ledToDisplay++;
            if(cont->ledToDisplay<cont->num_led_per_strip)
            {
               loadAndTranspose(cont->leds,(uint16_t *)cont->DMABuffersTampon[cont->dmaBufferActive]->buffer,cont->ledToDisplay,cont->__green_map,cont->__red_map,cont->__blue_map,cont->__white_map);
                if(cont->ledToDisplay==(cont->num_led_per_strip-3))  //here it's not -1 because it takes time top have the change into account and it reread the buufer
                {
                    cont->DMABuffersTampon[cont->dmaBufferActive]->descriptor.qe.stqe_next=&(cont->DMABuffersTampon[3]->descriptor);
                }
                cont->dmaBufferActive = (cont->dmaBufferActive + 1)% 2;
            }
                
        }
        else
        {
            if(cont->framesync)
                {
                    portBASE_TYPE HPTaskAwoken = 0;
                    xSemaphoreGiveFromISR(cont->I2SClocklessVirtualLedDriver_semSync, &HPTaskAwoken);
                    if(HPTaskAwoken == pdTRUE) portYIELD_FROM_ISR();
                }
        }
        
        
    }
    
    if(GET_PERI_REG_BITS(I2S_INT_ST_REG(I2S_DEVICE), I2S_OUT_TOTAL_EOF_INT_ST_V,  I2S_OUT_TOTAL_EOF_INT_ST_S))
    {
        
//        portBASE_TYPE HPTaskAwoken = 0;
//            xSemaphoreGiveFromISR(((I2SClocklessVirtualLedDriver *)arg)->I2SClocklessVirtualLedDriver_semDisp, &HPTaskAwoken);
//            if(HPTaskAwoken == pdTRUE) portYIELD_FROM_ISR();
        ((I2SClocklessVirtualLedDriver *)arg)->i2sStop();

        
    }
    REG_WRITE(I2S_INT_CLR_REG(0), (REG_READ(I2S_INT_RAW_REG( 0 )) & 0xffffffc0) | 0x3f);
    
}




static  void  IRAM_ATTR loadAndTranspose(uint8_t * ledt,uint16_t *buff,int ledtodisp,uint8_t *mapg,uint8_t *mapr,uint8_t *mapb,uint8_t *mapw)
{
    Lines firstPixel[nb_components];
    
    uint8_t *poli=ledt+ledtodisp*nb_components;
     buff+=OFFSET;
//jump en deux
     poli+=NUM_LEDS_PER_STRIP*nb_components;
    for(int pin=0;pin<NBIS2SERIALPINS;pin++) {
        firstPixel[p_g].bytes[pin] = mapg[*(poli+1)];
        firstPixel[p_r].bytes[pin] = mapr[*(poli)];
        firstPixel[p_b].bytes[pin] =mapb[*(poli+2)];
        #if nb_components>3
          firstPixel[3].bytes[pin] =mapw[*(poli+3)];
        #endif 
        poli+=I2S_OFF;
    }
    
    transpose16x1_noinline2(firstPixel[0].bytes,(uint8_t*)(buff));
    transpose16x1_noinline2(firstPixel[1].bytes,(uint8_t*)(buff+192));
    transpose16x1_noinline2(firstPixel[2].bytes,(uint8_t*)(buff+384));
    #if nb_components>3
    transpose16x1_noinline2(firstPixel[3].bytes,(uint8_t*)(buff+576));
    #endif
    

    //on revient strip 1
    poli-=I2S_OFF3;
    
    buff++;

    for(int pin=0;pin<NBIS2SERIALPINS;pin++) {
        firstPixel[p_g].bytes[pin] = mapg[*(poli+1)];
        firstPixel[p_r].bytes[pin] = mapr[*(poli)];
        firstPixel[p_b].bytes[pin] =mapb[*(poli+2)];
        #if nb_components>3
          firstPixel[3].bytes[pin] =mapw[*(poli+3)];
        #endif 
        poli+=I2S_OFF;
    }
    firstPixel[0].bytes[NBIS2SERIALPINS]=255;
    firstPixel[1].bytes[NBIS2SERIALPINS]=255;
    firstPixel[2].bytes[NBIS2SERIALPINS]=255;
     #if nb_components>3
    firstPixel[3].bytes[NBIS2SERIALPINS]=255;
     #endif
    transpose16x1_noinline2(firstPixel[0].bytes,(uint8_t*)(buff));
    transpose16x1_noinline2(firstPixel[1].bytes,(uint8_t*)(buff+192));
    transpose16x1_noinline2(firstPixel[2].bytes,(uint8_t*)(buff+384));
    #if nb_components>3
    transpose16x1_noinline2(firstPixel[3].bytes,(uint8_t*)(buff+576));
    #endif

    //on va en strip 4
    poli-=I2S_OFF4;

    buff++;
    firstPixel[0].bytes[NBIS2SERIALPINS]=0;
    firstPixel[1].bytes[NBIS2SERIALPINS]=0;
    firstPixel[2].bytes[NBIS2SERIALPINS]=0;
     #if nb_components>3
        firstPixel[3].bytes[NBIS2SERIALPINS]=0 ;
     #endif


    for(int pin=0;pin<NBIS2SERIALPINS;pin++) {
        firstPixel[p_g].bytes[pin] = mapg[*(poli+1)];
        firstPixel[p_r].bytes[pin] = mapr[*(poli)];
        firstPixel[p_b].bytes[pin] =mapb[*(poli+2)];
        #if nb_components>3
          firstPixel[3].bytes[pin] =mapw[*(poli+3)];
        #endif 
        poli+=I2S_OFF;
    }
    transpose16x1_noinline2(firstPixel[0].bytes,(uint8_t*)(buff));
    transpose16x1_noinline2(firstPixel[1].bytes,(uint8_t*)(buff+192));
      transpose16x1_noinline2(firstPixel[2].bytes,(uint8_t*)(buff+384));
    #if nb_components>3
    transpose16x1_noinline2(firstPixel[3].bytes,(uint8_t*)(buff+576));
    #endif
      //on va en strip3
       poli-=I2S_OFF3;
       buff++;

        for(int pin=0;pin<NBIS2SERIALPINS;pin++) {
         firstPixel[p_g].bytes[pin] = mapg[*(poli+1)];
        firstPixel[p_r].bytes[pin] = mapr[*(poli)];
        firstPixel[p_b].bytes[pin] =mapb[*(poli+2)];
        #if nb_components>3
          firstPixel[3].bytes[pin] =mapw[*(poli+3)];
        #endif 
        poli+=I2S_OFF;
    }

    transpose16x1_noinline2(firstPixel[0].bytes,(uint8_t*)(buff));
    transpose16x1_noinline2(firstPixel[1].bytes,(uint8_t*)(buff+192));
      transpose16x1_noinline2(firstPixel[2].bytes,(uint8_t*)(buff+384));
    #if nb_components>3
    transpose16x1_noinline2(firstPixel[3].bytes,(uint8_t*)(buff+576));
    #endif
      //on va en strip6
      poli-=I2S_OFF4;
       buff++;

        for(int pin=0;pin<NBIS2SERIALPINS;pin++) {
        firstPixel[p_g].bytes[pin] = mapg[*(poli+1)];
        firstPixel[p_r].bytes[pin] = mapr[*(poli)];
        firstPixel[p_b].bytes[pin] =mapb[*(poli+2)];
        #if nb_components>3
         firstPixel[3].bytes[pin] =mapw[*(poli+3)];
        #endif 
        poli+=I2S_OFF;  
    }

      transpose16x1_noinline2(firstPixel[0].bytes,(uint8_t*)(buff));
    transpose16x1_noinline2(firstPixel[1].bytes,(uint8_t*)(buff+192));
      transpose16x1_noinline2(firstPixel[2].bytes,(uint8_t*)(buff+384));
    #if nb_components>3
    transpose16x1_noinline2(firstPixel[3].bytes,(uint8_t*)(buff+576));
    #endif
      //on va en strip5
       poli-=I2S_OFF3;
       buff++;

               for(int pin=0;pin<NBIS2SERIALPINS;pin++) {
        firstPixel[p_g].bytes[pin] = mapg[*(poli+1)];
        firstPixel[p_r].bytes[pin] = mapr[*(poli)];
        firstPixel[p_b].bytes[pin] =mapb[*(poli+2)];
        #if nb_components>3
          firstPixel[3].bytes[pin] =mapw[*(poli+3)];
        #endif 
        poli+=I2S_OFF;
        
    }
                  transpose16x1_noinline2(firstPixel[0].bytes,(uint8_t*)(buff));
    transpose16x1_noinline2(firstPixel[1].bytes,(uint8_t*)(buff+192));
      transpose16x1_noinline2(firstPixel[2].bytes,(uint8_t*)(buff+384));
    #if nb_components>3
    transpose16x1_noinline2(firstPixel[3].bytes,(uint8_t*)(buff+576));
    #endif
      //on va en strip8
      poli-=I2S_OFF4;
       buff++;

          for(int pin=0;pin<NBIS2SERIALPINS;pin++) {
        firstPixel[p_g].bytes[pin] = mapg[*(poli+1)];
        firstPixel[p_r].bytes[pin] = mapr[*(poli)];
        firstPixel[p_b].bytes[pin] =mapb[*(poli+2)];
        #if nb_components>3
          firstPixel[3].bytes[pin] =mapw[*(poli+3)];
        #endif 
        poli+=I2S_OFF;
    }

                   transpose16x1_noinline2(firstPixel[0].bytes,(uint8_t*)(buff));
    transpose16x1_noinline2(firstPixel[1].bytes,(uint8_t*)(buff+192));
      transpose16x1_noinline2(firstPixel[2].bytes,(uint8_t*)(buff+384));
    #if nb_components>3
    transpose16x1_noinline2(firstPixel[3].bytes,(uint8_t*)(buff+576));
    #endif
      //on va en strip7
       poli-=I2S_OFF3;
       buff++;
                 for(int pin=0;pin<NBIS2SERIALPINS;pin++) {
        firstPixel[p_g].bytes[pin] = mapg[*(poli+1)];
        firstPixel[p_r].bytes[pin] = mapr[*(poli)];
        firstPixel[p_b].bytes[pin] =mapb[*(poli+2)];
        #if nb_components>3
          firstPixel[3].bytes[pin] =mapw[*(poli+3)];
        #endif 
        poli+=I2S_OFF;
    }
                       transpose16x1_noinline2(firstPixel[0].bytes,(uint8_t*)(buff));
    transpose16x1_noinline2(firstPixel[1].bytes,(uint8_t*)(buff+192));
      transpose16x1_noinline2(firstPixel[2].bytes,(uint8_t*)(buff+384));
          #if nb_components>3
    transpose16x1_noinline2(firstPixel[3].bytes,(uint8_t*)(buff+576));
    #endif
}
