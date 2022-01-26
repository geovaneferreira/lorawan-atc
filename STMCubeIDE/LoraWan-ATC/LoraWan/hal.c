/*
 * Copyright (c) 2014-2016 IBM Corporation.
 * All rights reserved.
 *
 *  Redistribution and use in source and binary forms, with or without
 *  modification, are permitted provided that the following conditions are met:
 *  * Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 *  * Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *  * Neither the name of the <organization> nor the
 *    names of its contributors may be used to endorse or promote products
 *    derived from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL <COPYRIGHT HOLDER> BE LIABLE FOR ANY
 * DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 * ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

/*
 * This file is a futher development from the great IBM LMIC
 * made by Hjalmar SkovHolm Hansen
 * Denmark
 */

#include "main.h"
#include "spi.h"
#include "hal.h"
#include "Struct.h"
#include "cmsis_os.h"
#include "stm32f4xx_hal.h"

#define mySPI hspi1   //  <--------- change to your setup

uint8_t digitalRead(uint16_t GPIO_Pin){
	int rt = 0;
	for(int i=0;i<100;i++){
		rt += HAL_GPIO_ReadPin(getPort(GPIO_Pin), GPIO_Pin);
	//	osDelay(1);
	}
	return rt > 60 ? 1 : 0;
	//return HAL_GPIO_ReadPin(getPort(GPIO_Pin), GPIO_Pin);
}

void digitalWrite(uint16_t GPIO_Pin, GPIO_PinState PinState){
	HAL_GPIO_WritePin(getPort(GPIO_Pin), GPIO_Pin, PinState);
}

uint8_t SPItransfer(uint8_t byte)
{
	uint8_t outbuffer[1];
	uint8_t inbuffer[1];
	outbuffer[0] = byte;
	HAL_SPI_TransmitReceive(&mySPI,outbuffer,inbuffer,sizeof(outbuffer),HAL_MAX_DELAY);
	return inbuffer[0];
}

void SPIbeginTransaction(){

}

void SPIendTransaction(){

}


void delay(int t){
	vTaskDelay(t);
}

GPIO_TypeDef* getPort(uint16_t GPIO_Pin){
	GPIO_TypeDef* GPIOx;
	if(GPIO_Pin == RFM_pins.DIO0)
			GPIOx = RFM_pins.DIO0_Port;
	else if(GPIO_Pin == RFM_pins.DIO1)
			GPIOx = RFM_pins.DIO1_Port;
	else if(GPIO_Pin == RFM_pins.RST)
			GPIOx = RFM_pins.RST_Port;
	else if(GPIO_Pin == RFM_pins.CS)
			GPIOx = RFM_pins.CS_Port;
	return GPIOx;
}


int millis(){
	TickType_t t1 = xTaskGetTickCount();
	t1 = t1 * 1000;
	return (t1/configTICK_RATE_HZ);//ms
}
