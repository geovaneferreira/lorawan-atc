/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * File Name          : freertos.c
  * Description        : Code for freertos applications
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2022 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Includes ------------------------------------------------------------------*/
#include "FreeRTOS.h"
#include "task.h"
#include "main.h"
#include "cmsis_os.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include <stdio.h>
#include <string.h>
#include "credentials.h"
#include "spi.h"
#include "usart.h"
#include "gpio.h"
#include "debug.h"
#include "Struct.h"
#include "hal.h"
#include "lorawan-arduino-rfm.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */
const char *devAddr = "00000000";                         //coloque aqui seu dev_addr
const char *nwkSKey = "00000000000000000000000000000000"; //coloque aqui sua network session key
const char *appSKey = "00000000000000000000000000000000"; //coloque aqui sua application session key

#define IS_CHANNEL_125khz(c) (c<64)
#define IS_CHANNEL_500khz(c) (c>=64 && c<72)

char myStr[50];
char outStr[255];
uint8_t recvStatus = 0;

const unsigned long interval = 15000;    // 10 s interval to send message
unsigned long previousMillis = 0;  // will store last time message sent
unsigned int counter = 0;     // message counter
/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
/* USER CODE BEGIN Variables */

/* USER CODE END Variables */
/* Definitions for defaultTask */
osThreadId_t defaultTaskHandle;
const osThreadAttr_t defaultTask_attributes = {
  .name = "defaultTask",
  .stack_size = 256 * 4,
  .priority = (osPriority_t) osPriorityRealtime,
};

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN FunctionPrototypes */

/* USER CODE END FunctionPrototypes */

void StartDefaultTask(void *argument);

void MX_FREERTOS_Init(void); /* (MISRA C 2004 rule 8.1) */

/**
  * @brief  FreeRTOS initialization
  * @param  None
  * @retval None
  */
void MX_FREERTOS_Init(void) {
  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* USER CODE BEGIN RTOS_MUTEX */
  /* add mutexes, ... */
  /* USER CODE END RTOS_MUTEX */

  /* USER CODE BEGIN RTOS_SEMAPHORES */
  /* add semaphores, ... */
  /* USER CODE END RTOS_SEMAPHORES */

  /* USER CODE BEGIN RTOS_TIMERS */
  /* start timers, add new ones, ... */
  /* USER CODE END RTOS_TIMERS */

  /* USER CODE BEGIN RTOS_QUEUES */
  /* add queues, ... */
  /* USER CODE END RTOS_QUEUES */

  /* Create the thread(s) */
  /* creation of defaultTask */
  defaultTaskHandle = osThreadNew(StartDefaultTask, NULL, &defaultTask_attributes);

  /* USER CODE BEGIN RTOS_THREADS */
  /* add threads, ... */
  /* USER CODE END RTOS_THREADS */

  /* USER CODE BEGIN RTOS_EVENTS */
  /* add events, ... */
  /* USER CODE END RTOS_EVENTS */

}

/* USER CODE BEGIN Header_StartDefaultTask */
/**
  * @brief  Function implementing the defaultTask thread.
  * @param  argument: Not used
  * @retval None
  */
/* USER CODE END Header_StartDefaultTask */
void StartDefaultTask(void *argument)
{
  /* USER CODE BEGIN StartDefaultTask */
	debug_init();
	/* USER CODE BEGIN 2 */
	if(!init()){
		debug_str("RFM95 not detected");
		for(;;){
			osDelay(5000);
		}
	}
	// Set LoRaWAN Class change CLASS_A or CLASS_C
	setDeviceClass(CLASS_A);
	// Set Data Rate
	setDataRate(SF8BW125);
	// set channel to random
	setChannel(MULTI);

	// Put ABP Key and DevAddress here
	setNwkSKey(nwkSKey);
	setAppSKey(appSKey);
	setDevAddr(devAddr);
  /* Infinite loop */
	previousMillis = millis()  - interval - 2000;
  for(;;)
  {
	if(millis() - previousMillis > interval) {
	  previousMillis = millis();
	  sprintf(myStr, "Counter %d\r\n", counter);

	  debug_str("\r\n");
	  debug_str("Sending: ");
	  debug_str(myStr);

	  sendUplink(myStr, strlen(myStr), 0, 1);
	  counter++;

	  // Check Lora RX
	  update();
	}

	recvStatus = readData(outStr);
	if(recvStatus) {
		debug_str("Downlink: ");
		debug_str(outStr);
	}


	osDelay(100);
  }
  /* USER CODE END StartDefaultTask */
}

/* Private application code --------------------------------------------------*/
/* USER CODE BEGIN Application */

/* USER CODE END Application */

