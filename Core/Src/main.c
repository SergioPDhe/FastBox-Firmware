/* USER CODE BEGIN Header */
/**
 ******************************************************************************
 * @file           : main.c
 * @brief          : Main program body
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
#include "main.h"
#include "usb_device.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
//#include "COMMS.h"
//#include "GC_DAC.h"
//#include "COMMS.h"
#include "SETUP.h"
#include "usbd_customhid.h"
#include "usbd_ctlreq.h"

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
UART_HandleTypeDef huart1;

/* USER CODE BEGIN PV */

volatile struct controller_inputs
{
  uint8_t reportID;
  uint8_t buttons;
  int8_t ctrl_x;
  int8_t ctrl_y;
  int8_t cstk_x;
  int8_t cstk_y;
  uint8_t analog_l;
  uint8_t analog_r;
  uint8_t dpad;
};

extern USBD_HandleTypeDef hUsbDeviceFS;

volatile struct controller_inputs ControllerInputs;

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_USART1_UART_Init(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */



//volatile uint8_t  StYXBA =    0x00;
//volatile uint8_t  LRZDpad =   0x80; // these are pulled straight from pin registers and are not necessary for the code

uint8_t modeUSB = 1;

volatile uint8_t  ControlX =  128; 	//X,Y for control stick
volatile uint8_t  ControlY =  128;

volatile uint8_t  CstickX =   128; 	// X,Y for C-Stick
volatile uint8_t  CstickY =   128;

const uint8_t  AnalogL =   0;		// Analog L and R triggers (only R is used so L can remain at zero)
volatile uint8_t  AnalogR =   0;

volatile uint8_t  DPad =      0;	// Dpad values

//int count = 0;
//int total_count = 0;			// for debugging
//int avg_count = 0;
//int count_num = 0;

bool dPad_on = false;			// if MX and MY are pressed this becomes true, allowing the use of the Dpad

//volatile uint8_t  rumble =    0;	// rumble is pulled straight from console polls into the output pin registers

/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
  /* USER CODE BEGIN 1 */
  ControllerInputs.reportID = 1;
  ControllerInputs.buttons = 0;
  ControllerInputs.ctrl_x = 0;
  ControllerInputs.ctrl_y = 0;
  ControllerInputs.cstk_x = 0;
  ControllerInputs.cstk_y = 0;
  ControllerInputs.analog_l = 0;
  ControllerInputs.analog_r = 0;
  ControllerInputs.dpad = 0;
  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_USB_DEVICE_Init();
  MX_USART1_UART_Init();
  /* USER CODE BEGIN 2 */

  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
	while (1)
	{
	  if (modeUSB == 1)
	  {
	      //Digital_Analog_Conversion();
	      UpdateInputsUSB();
	  }




	     //count++;

	     //Digital_Analog_Conversion();  // Performs the DAC algorithm for Control Stick, Cstick and analog triggers
					   // SEE DAC.c for details
					   // Runs continuously until interrupted by an external interrupt
					   // ISR below is HAL_GPIO_EXTI_Callback()


    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
		/* USER CODE BEGIN 3 */

	}
  /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Configure the main internal regulator output voltage
  */
  __HAL_RCC_PWR_CLK_ENABLE();
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);
  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLM = 25;
  RCC_OscInitStruct.PLL.PLLN = 192;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLQ = 4;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }
  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_3) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief USART1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_USART1_UART_Init(void)
{

  /* USER CODE BEGIN USART1_Init 0 */

  /* USER CODE END USART1_Init 0 */

  /* USER CODE BEGIN USART1_Init 1 */

  /* USER CODE END USART1_Init 1 */
  huart1.Instance = USART1;
  huart1.Init.BaudRate = 1000000;
  huart1.Init.WordLength = UART_WORDLENGTH_8B;
  huart1.Init.StopBits = UART_STOPBITS_1;
  huart1.Init.Parity = UART_PARITY_NONE;
  huart1.Init.Mode = UART_MODE_TX_RX;
  huart1.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart1.Init.OverSampling = UART_OVERSAMPLING_8;
  if (HAL_HalfDuplex_Init(&huart1) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN USART1_Init 2 */

  /* USER CODE END USART1_Init 2 */

}

/**
  * @brief GPIO Initialization Function
  * @param None
  * @retval None
  */
static void MX_GPIO_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_GPIOH_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(LED_GPIO_Port, LED_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(RUMBLE_GPIO_Port, RUMBLE_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin : LED_Pin */
  GPIO_InitStruct.Pin = LED_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(LED_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pins : A_Pin B_Pin X_Pin Y_Pin
                           START_Pin Z_Pin R_Pin L_Pin
                           MS_Pin LS_Pin */
  GPIO_InitStruct.Pin = A_Pin|B_Pin|X_Pin|Y_Pin
                          |START_Pin|Z_Pin|R_Pin|L_Pin
                          |MS_Pin|LS_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /*Configure GPIO pins : RIGHT_Pin LEFT_Pin C_RIGHT_Pin C_LEFT_Pin
                           C_UP_Pin C_DOWN_Pin UP_Pin DOWN_Pin
                           MX_Pin MY_Pin */
  GPIO_InitStruct.Pin = RIGHT_Pin|LEFT_Pin|C_RIGHT_Pin|C_LEFT_Pin
                          |C_UP_Pin|C_DOWN_Pin|UP_Pin|DOWN_Pin
                          |MX_Pin|MY_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

  /*Configure GPIO pin : TX_INT_Pin */
  GPIO_InitStruct.Pin = TX_INT_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_IT_FALLING;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  HAL_GPIO_Init(TX_INT_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pin : RUMBLE_Pin */
  GPIO_InitStruct.Pin = RUMBLE_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(RUMBLE_GPIO_Port, &GPIO_InitStruct);

  /* EXTI interrupt init*/
  HAL_NVIC_SetPriority(EXTI3_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(EXTI3_IRQn);

}

/* USER CODE BEGIN 4 */

void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{



  Digital_Analog_Conversion();
  ReceiveCommand(); // reads incoming message from console and replies accordingly
		    // See COMMS.h for more detail

  //count_num=count;
  //avg_count = count; // debugging
  //count = 0;

  __HAL_GPIO_EXTI_CLEAR_IT(GPIO_Pin); // clear interrupt flag
}

void PrepareInputsUSB()
{
  Digital_Analog_Conversion();

  ControllerInputs.buttons = ~BTN_PIN;

  ControllerInputs.ctrl_x = ControlX-128;
  ControllerInputs.ctrl_y = 128-ControlY;
  ControllerInputs.cstk_x = CstickX-128;
  ControllerInputs.cstk_y = CstickY-128;
  ControllerInputs.analog_l = AnalogL;
  ControllerInputs.analog_r = AnalogR;
  ControllerInputs.dpad = Dpad2HatSwitch(DPad);
}

void UpdateInputsUSB()
{
  //PrepareInputsUSB();

  USBD_CUSTOM_HID_SendInputs(&hUsbDeviceFS, &ControllerInputs, 9, &PrepareInputsUSB);


}

/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
	/* User can add his own implementation to report the HAL error return state */
	__disable_irq();
	while (1) {
	}
  /* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */

