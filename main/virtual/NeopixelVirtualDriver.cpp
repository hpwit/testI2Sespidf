//
//  NeopixelVirtaulDriver.cpp
//  
//
//  Created by Yves BAZIN on 09/09/2020.
//

#include "NeopixelVirtualDriver.h"

#include <stdio.h>
#include <rom/ets_sys.h>
DMABuffer * dmaBuffers[800];
intr_handle_t gI2S_intr_handle=NULL;
void IRAM_ATTR NeopixelVirtualDriver::interruptHandler(void *arg)
    {
        //printf("kl");
       uint8_t d= GET_PERI_REG_BITS(I2S_INT_ST_REG(I2S_DEVICE), I2S_OUT_TOTAL_EOF_INT_ST_V,  I2S_OUT_TOTAL_EOF_INT_ST_S);
        uint8_t d2= GET_PERI_REG_BITS(I2S_INT_ST_REG(I2S_DEVICE), I2S_OUT_EOF_INT_ST_V,  I2S_OUT_EOF_INT_ST_S);
        uint8_t d3= GET_PERI_REG_BITS(I2S_INT_ST_REG(I2S_DEVICE), I2S_OUT_DONE_INT_ST_V,  I2S_OUT_DONE_INT_ST_S);
        REG_WRITE(I2S_INT_CLR_REG(0), (REG_READ(I2S_INT_RAW_REG( 0 )) & 0xffffffc0) | 0x3f);
        
              if(d)
              {
                  ((NeopixelVirtualDriver *)arg)->total++;
                  //((NeopixelVirtualDriver *)arg)->i2sStop();
              }
            if(d2)
                 ((NeopixelVirtualDriver *)arg)->eof++;
        if(d3)
        {
            ((NeopixelVirtualDriver *)arg)->end++;
            if(((NeopixelVirtualDriver *)arg)->end==3)
             ((NeopixelVirtualDriver *)arg)->i2sStop();
        }
                   //return;
        //REG_WRITE(I2S_INT_CLR_REG(0, (REG_READ(I2S_INT_RAW_REG(0)) & 0xffffffc0) | 0x3f);
          //  REG_WRITE(I2S_INT_CLR_REG(0), (REG_READ(I2S_INT_RAW_REG( 0 )) & 0xffffffc0) | 0x3f);
       // ESP_LOGI("TAG","loh\n");
        //printf("jjj\n");
       // if (!i2s->int_st.out_eof)
         //	           return;

//        ((NeopixelVirtualDriver *)arg)->testint++;
       // i2s->int_clr.val = i2s->int_raw.val;
    }

void NeopixelVirtualDriver::transpose16x1_noinline2(unsigned char *A, uint8_t *B) {
        uint32_t  x, y, x1,y1,t;



        y = *(unsigned int*)(A);
        x = *(unsigned int*)(A+4);
        y1 = *(unsigned int*)(A+8);
    //x1=0;
     x1 = *(unsigned int*)(A+12);

        // pre-transform x
        t = (x ^ (x >> 7)) & AA;  x = x ^ t ^ (t << 7);
        t = (x ^ (x >>14)) & CC;  x = x ^ t ^ (t <<14);
       t = (x1 ^ (x1 >> 7)) & AA;  x1 = x1 ^ t ^ (t << 7);
       t = (x1 ^ (x1 >>14)) & CC;  x1 = x1 ^ t ^ (t <<14);
        // pre-transform y
        t = (y ^ (y >> 7)) & AA;  y = y ^ t ^ (t << 7);
        t = (y ^ (y >>14)) & CC;  y = y ^ t ^ (t <<14);
        t = (y1 ^ (y1 >> 7)) & AA;  y1 = y1 ^ t ^ (t << 7);
        t = (y1 ^ (y1 >>14)) & CC;  y1 = y1 ^ t ^ (t <<14);


        // final transform
        t = (x & FF) | ((y >> 4) & FF2);
        y = ((x << 4) & FF) | (y & FF2);
        x = t;

        t= (x1 & FF) | ((y1 >> 4) & FF2);
        y1 = ((x1 << 4) & FF) | (y1 & FF2);
        x1 = t;
    
     *((uint16_t*)(B)) = (uint16_t)(((x & 0xff000000) >>8 |((x1&0xff000000) ))>>16);
    *((uint16_t*)(B+48)) = (uint16_t)( ((x & 0xff0000) >>16|((x1&0xff0000) >>8)));
    *((uint16_t*)(B+2*48)) = (uint16_t)(((x & 0xff00) |((x1&0xff00) <<8))>>8);
    *((uint16_t*)(B+3*48)) =(uint16_t)( (x & 0xff) |((x1&0xff) <<8));
    *((uint16_t*)(B+4*48)) = (uint16_t)(((y & 0xff000000) >>8 |((y1&0xff000000) ))>>16);
   *((uint16_t*)(B+5*48)) = (uint16_t)(((y & 0xff0000) |((y1&0xff0000) <<8))>>16);
    *((uint16_t*)(B+6*48)) = (uint16_t)(((y & 0xff00) |((y1&0xff00) <<8))>>8);
    *((uint16_t*)(B+7*48)) = (uint16_t)((y & 0xff) |  (  (y1 & 0xff) << 8 ) )   ;


    }



 void NeopixelVirtualDriver::pu(uint16_t* buff)
{
    memset((uint8_t*)buff,0,(NUM_VIRT_PINS+1)*8*3*3*2);
    for (int i=0;i<24*3;i++)
    {
        buff[NUM_VIRT_PINS+i*(NUM_VIRT_PINS+1)-1-5]=0x8000;
        //buff[NUM_VIRT_PINS+i*(NUM_VIRT_PINS+1)]=0x02;
    }
}

 void NeopixelVirtualDriver::pu3(uint16_t* buff)
{
    // memset((uint8_t*)buff,0,(NUM_VIRT_PINS+1)*8*3*3*2);
    for (int i=0;i<24*3;i++)
    {
        buff[NUM_VIRT_PINS+i*(NUM_VIRT_PINS+1)-1-5]+=0x8000;
        //buff[NUM_VIRT_PINS+i*(NUM_VIRT_PINS+1)]=0x02;
    }
}

  void NeopixelVirtualDriver::pu2(uint16_t* buff)
{
    
    
    for (int j=0;j<24;j++)
    {

        buff[1+j*(3*(NUM_VIRT_PINS+1))]=0xFFFF;
        buff[0+j*(3*(NUM_VIRT_PINS+1))]=0x7FFF;
        buff[3+j*(3*(NUM_VIRT_PINS+1))]=0x7FFF;
        buff[2+j*(3*(NUM_VIRT_PINS+1))]=0x7FFF;
        buff[5+j*(3*(NUM_VIRT_PINS+1))]=0x7FFF;
        buff[4+j*(3*(NUM_VIRT_PINS+1))]=0x7FFF;
        buff[7+j*(3*(NUM_VIRT_PINS+1))]=0x7FFF;
        buff[6+j*(3*(NUM_VIRT_PINS+1))]=0x7FFF;

    }
}
    


 void  NeopixelVirtualDriver::i2sReset_DMA()
{
  //i2s->lc_conf.in_rst=1; i2s->lc_conf.in_rst=0;
  //i2s->lc_conf.out_rst=1; i2s->lc_conf.out_rst=0;
  (&I2S0)->lc_conf.out_rst=1; (&I2S0)->lc_conf.out_rst=0;
}

 void NeopixelVirtualDriver::i2sReset_FIFO()
{
  //i2s->conf.rx_fifo_reset=1; i2s->conf.rx_fifo_reset=0;
  (&I2S0)->conf.tx_fifo_reset=1; (&I2S0)->conf.tx_fifo_reset=0;
}

 void NeopixelVirtualDriver::i2sStop()
{
  // Serial.println("I2S stop");
  //ets_delay_us(80) ;
  esp_intr_disable(gI2S_intr_handle);
  i2sReset();
  //i2s->conf.rx_start = 0;
  (&I2S0)->conf.tx_start = 0;
}




 void NeopixelVirtualDriver::i2sStart()
{
 // esp_intr_disable(gI2S_intr_handle);
 printf("I2S start\n");
 i2sReset();
 printf("I2S start\n");
 //Serial.println(dmaBuffers[0]->sampleCount());
 (&I2S0)->lc_conf.val=I2S_OUT_DATA_BURST_EN | I2S_OUTDSCR_BURST_EN | I2S_OUT_DATA_BURST_EN;
 //i2s.rx_eof_num = dmaBuffers[2]->sampleCount();
 printf("I2S start\n");
 (&I2S0)->out_link.addr = (uint32_t) & (dmaBuffers[0]->descriptor);
 printf("I2S start\n");
 //i2s.in_link.addr = (uint32_t) & (dmaBuffers[2]->descriptor);
 (&I2S0)->out_link.start = 1;
 ////vTaskDelay(5);
 (&I2S0)->int_clr.val = (&I2S0)->int_raw.val;
 // //vTaskDelay(5);
// i2s->int_ena.out_dscr_err = 1;
 //enable interrupt
 ////vTaskDelay(5);
esp_intr_enable(gI2S_intr_handle);
 // //vTaskDelay(5);
 	//i2s->int_clr.val	 = i2s->int_raw.val;
 //i2s->int_ena.val = 0;
  // i2s->int_ena.out_eof = 1;
 //i2s->int_ena.out_done = 1;
 printf("I2S start\n");
 //start transmission
 (&I2S0)->conf.tx_start = 1;
 printf("I2S start\n");
}


 void NeopixelVirtualDriver::i2sReset()
{
    // Serial.println("I2S reset");
    const unsigned long lc_conf_reset_flags = I2S_IN_RST_M | I2S_OUT_RST_M | I2S_AHBM_RST_M | I2S_AHBM_FIFO_RST_M;
    (&I2S0)->lc_conf.val |= lc_conf_reset_flags;
    (&I2S0)->lc_conf.val &= ~lc_conf_reset_flags;

    const uint32_t conf_reset_flags = I2S_RX_RESET_M | I2S_RX_FIFO_RESET_M | I2S_TX_RESET_M | I2S_TX_FIFO_RESET_M;
    (&I2S0)->conf.val |= conf_reset_flags;
    (&I2S0)->conf.val &= ~conf_reset_flags;
}

DMABuffer * NeopixelVirtualDriver::allocateDMABuffer(int bytes)
{
   DMABuffer * b = (DMABuffer *)heap_caps_malloc(sizeof(DMABuffer), MALLOC_CAP_DMA);
   
   b->buffer = (uint8_t *)heap_caps_malloc(bytes, MALLOC_CAP_DMA);
   memset(b->buffer, 0, bytes);
   
   b->descriptor.length = bytes;
   b->descriptor.size = bytes;
   b->descriptor.owner = 1;
   b->descriptor.sosf = 0;
   b->descriptor.buf = b->buffer;
   b->descriptor.offset = 0;
   b->descriptor.empty = 0;
   b->descriptor.eof = 1;
   b->descriptor.qe.stqe_next = 0;
   
   return b;
}
