/*
 * Copyright (c) 2021, Texas Instruments Incorporated
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * *  Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 *
 * *  Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * *  Neither the name of Texas Instruments Incorporated nor the names of
 *    its contributors may be used to endorse or promote products derived
 *    from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
 * THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR
 * CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS;
 * OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
 * WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR
 * OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
 * EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#include "ti/driverlib/dl_dma.h"
#include "ti_msp_dl_config.h"
#include <sys/cdefs.h>

#define SQUARE_INT 1024
volatile uint8_t gEchoData = 0;
uint64_t N = 0; //Sample Size

/* Repetitive sine wave */

const uint16_t gOutputSignalSine64[] = {2048, 2248, 2447, 2642, 2831, 3013,
    3185, 3347, 3496, 3631, 3750, 3854, 3940, 4007, 4056, 4086, 4095, 4086,
    4056, 4007, 3940, 3854, 3750, 3631, 3496, 3347, 3185, 3013, 2831, 2642,
    2447, 2248, 2048, 1847, 1648, 1453, 1264, 1082, 910, 748, 599, 464, 345,
    241, 155, 88, 39, 9, 0, 9, 39, 88, 155, 241, 345, 464, 599, 748, 910, 1082,
    1264, 1453, 1648, 1847};

uint16_t gOutputSignalSquare64[] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
                                SQUARE_INT,SQUARE_INT,SQUARE_INT,SQUARE_INT,SQUARE_INT,SQUARE_INT,SQUARE_INT,SQUARE_INT,SQUARE_INT,SQUARE_INT,SQUARE_INT,SQUARE_INT,SQUARE_INT,SQUARE_INT,SQUARE_INT,SQUARE_INT,SQUARE_INT,SQUARE_INT,
                                SQUARE_INT,SQUARE_INT,SQUARE_INT,SQUARE_INT,SQUARE_INT,SQUARE_INT,SQUARE_INT,SQUARE_INT,SQUARE_INT,SQUARE_INT,SQUARE_INT,SQUARE_INT,SQUARE_INT,SQUARE_INT,SQUARE_INT,SQUARE_INT,SQUARE_INT,SQUARE_INT};
uint16_t gOutputSignalOff64[] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0};

uint64_t SampleSize(uint64_t f_o, uint64_t f_s){
    N = f_s/f_o;
    return N
}


int main(void)
{
    SYSCFG_DL_init();

    /* Configure DMA source, destination and size */
    DL_DMA_setSrcAddr(
        DMA, DMA_CH0_CHAN_ID, (uint32_t) &gOutputSignalSquare64[0]);
    DL_DMA_setDestAddr(DMA, DMA_CH0_CHAN_ID, (uint32_t) & (DAC0->DATA0));
    DL_DMA_setTransferSize(
        DMA, DMA_CH0_CHAN_ID, sizeof(gOutputSignalSquare64) / sizeof(uint16_t));

    DL_DMA_enableChannel(DMA, DMA_CH0_CHAN_ID);

    while (1) {
        __WFI();
    }
}

void UART_0_INST_IRQHandler(void)
{
    switch (DL_UART_Main_getPendingInterrupt(UART_0_INST)) {  
        case DL_UART_MAIN_IIDX_RX:
            //DL_GPIO_togglePins(GPIO_LEDS_PORT,GPIO_LEDS_USER_LED_1_PIN | GPIO_LEDS_USER_TEST_PIN);
            gEchoData = DL_UART_Main_receiveData(UART_0_INST);
            DL_UART_Main_transmitData(UART_0_INST, gEchoData);
            break;
        default:
            break;
    }
}
