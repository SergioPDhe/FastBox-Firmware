/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : usbd_custom_hid_if.c
  * @version        : v1.0_Cube
  * @brief          : USB Device Custom HID interface file.
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
#include "usbd_custom_hid_if.h"

/* USER CODE BEGIN INCLUDE */
#include "USB_DEFS.h"

/* USER CODE END INCLUDE */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/

/* USER CODE BEGIN PV */
/* Private variables ---------------------------------------------------------*/

/* USER CODE END PV */

/** @addtogroup STM32_USB_OTG_DEVICE_LIBRARY
  * @brief Usb device.
  * @{
  */

/** @addtogroup USBD_CUSTOM_HID
  * @{
  */

/** @defgroup USBD_CUSTOM_HID_Private_TypesDefinitions USBD_CUSTOM_HID_Private_TypesDefinitions
  * @brief Private types.
  * @{
  */

/* USER CODE BEGIN PRIVATE_TYPES */

/* USER CODE END PRIVATE_TYPES */

/**
  * @}
  */

/** @defgroup USBD_CUSTOM_HID_Private_Defines USBD_CUSTOM_HID_Private_Defines
  * @brief Private defines.
  * @{
  */

/* USER CODE BEGIN PRIVATE_DEFINES */

/* USER CODE END PRIVATE_DEFINES */

/**
  * @}
  */

/** @defgroup USBD_CUSTOM_HID_Private_Macros USBD_CUSTOM_HID_Private_Macros
  * @brief Private macros.
  * @{
  */

/* USER CODE BEGIN PRIVATE_MACRO */

/* USER CODE END PRIVATE_MACRO */

/**
  * @}
  */

/** @defgroup USBD_CUSTOM_HID_Private_Variables USBD_CUSTOM_HID_Private_Variables
  * @brief Private variables.
  * @{
  */

/** Usb HID report descriptor. */
__ALIGN_BEGIN static uint8_t CUSTOM_HID_ReportDesc_FS[USBD_CUSTOM_HID_REPORT_DESC_SIZE] __ALIGN_END =
{
  /* USER CODE BEGIN 0 */
  USAGE_PAGE, GENERIC_DESKTOP, 		// go into generic desktop usage page
  USAGE_USAGE, USAGE_JOYSTICK,		// generic gamepad usage
  COLLECTION, COL_APPLICATION,		// application collection START


  //COLLECTION, COL_PHYSICAL, // 8 BYTES		// physical collection START

  REPORT_ID, 0x01,					// report id for controller (unnecesary maybe?)

  USAGE_PAGE, BUTTON, // BUTTON USAGE DATA		// go into button usage page
  USAGE_MIN, 0x01,					// buttons go from 1-8
  USAGE_MAX, 0x08,						// 8 buttons
  LOGICAL_MIN, 0x00,					// button output ranges from 0-1
  LOGICAL_MAX, 0x01,						//
  REPORT_SIZE, 0x01,					// each button has a 1-bit report
  REPORT_COUNT, 0x08, // 14 BYTES			// 8 reports in total for 8 buttons!
				// no padding nexessary since they fit nicely into 1 byte!
  /*
  UNIT_EXPONENT, 0X00,					// unit business not needed?
  UNIT, 0x00,
  */

  USB_INPUT, DATA_VAR_ABS,				// input format

  USAGE_PAGE, GENERIC_DESKTOP, // 4 BYTES		// go back to generic usage page

  //USAGE, USAGE_POINTER,

  USAGE_USAGE, USAGE_X,					// usage for control stick X-axis
  USAGE_USAGE, USAGE_Y,					// usage for control stick Y-axis
  USAGE_USAGE, USAGE_CX,				// usage for c-stick X-axis
  USAGE_USAGE, USAGE_CY,				// usage for c-stick Y-axis
  USAGE_USAGE, USAGE_L,					// usage for L button light press
  USAGE_USAGE, USAGE_R, // 12 BYTES			// usage for R button light press

  LOGICAL_MIN, 0x80,					// logical min is -128 in 2's complement
  //LOGICAL_MAX, 0xFF, 0xFF, 0x00, 0x00,
  LOGICAL_MAX, 0x7F,					// logical max is 127 in 2's complement
  REPORT_SIZE, 0x08,					// each report is 8 bits long
  REPORT_COUNT, 0x06,					// 6 reports in total
  USB_INPUT, DATA_VAR_ABS, // 10 BYTES			// input format

  USAGE_USAGE, HAT_SWITCH, // HAT SWITCH DATA		// hat switch usage
  LOGICAL_MIN, 0x00,					// hat switches output between 0 and 7 which correspond to the input direction
  LOGICAL_MAX, 0x07, // 6 BYTES					// any number larger or smaller results in a neutral input


  //PHYSICAL_MIN, 0x00,
  //PHYSICAL_MAX_2, 0x3B, 0x01, // 			// angular units if you want

  //UNIT, 0x14,

  REPORT_SIZE, 0x04,					// report is 4-bits long
  REPORT_COUNT, 0x01,					// 1 report
  USB_INPUT, DATA_VAR_ABS, // 6 BYTES			// input data

  REPORT_SIZE, 0x01, // buffer				// 4 bit buffer to round off the last input byte
  REPORT_COUNT, 0x04,					// 4 reports of 1 bit
  USB_INPUT, CONST_VAR_ABS, // 6 BYTES			// input for these is constant

  END_COLLECTION, 				// end physical application

  //67 BYTES



  // RUMBLE DESCRIPTOR CODE

  USAGE_PAGE, HAPTICS_PAGE,			// go into haptics page
  USAGE_USAGE, SIMPLE_HAPTIC,			// make a simple haptics controller
  COLLECTION, COL_APPLICATION, // 6 bytes	// application collection START

  REPORT_ID, 0x02,

  USAGE_USAGE, MANUAL_TRIGGER,				// manual trigger usage
  LOGICAL_MIN, 0x00,					// trigger goes from 0-16
  LOGICAL_MAX, 0x10,						// 0x10 = 16
  REPORT_COUNT, 0x01,					// 1 report
  REPORT_SIZE, 0x05,					// the report is 5 bits
  USB_OUTPUT, DATA_VAR_ABS, // 20 BYTES			// output format


  REPORT_SIZE, 0x03,					// add 3 constant bits to round out the byte
  USB_OUTPUT, CONST_VAR_ABS,				// constant output


  USAGE_USAGE, WAVEFORM_LIST,				// make a list of useable waveforms
  COLLECTION, COL_LOGICAL, // 8 BYTES //		// logical collection START


  USAGE_PAGE, ORDINAL_PAGE,					// go to ordinals page (these lists use ordinals)
  USAGE_MIN, 0x03,						// smallest ordinal is #3
  USAGE_MAX, 0x05,//0x10,							//largest ordinal is #16
  LOGICAL_MIN_2, 0x03, 0x10,					// the smallest ordinal is 0x1003
  LOGICAL_MAX_2, 0x05, 0x10,//0x11, 0x10,						// the largest is 0x1011

//  /* 12 bytes here
  REPORT_COUNT, 0x03, // E						// report count of 14 = 7*2????
  REPORT_SIZE, 0x0D, // D						// report size of 13
  USB_FEATURE, DATA_VAR_ABS,					// feature

  REPORT_COUNT, 0x01,					// last few bits to round out the byte
  REPORT_SIZE, 0x01, //should be 2!!
  USB_FEATURE, CONST_VAR_ABS,				// constant output
//
  END_COLLECTION,		// 19 BYTES		// logical collection END



  USAGE_PAGE, HAPTICS_PAGE,				// back to haptics page
  USAGE_USAGE, DURATION_LIST,				// make a duration list
  COLLECTION, COL_LOGICAL,	// 6 BYTES		// logical collection START


  USAGE_PAGE, ORDINAL_PAGE,					// back to ordinals page
  USAGE_MIN, 0x03,						// ordinals go from 3-16
  USAGE_MAX, 0x10,
  LOGICAL_MIN, 0x01,						// logical value for this goes between -65536 to 65535
  LOGICAL_MAX_4, 0xFF, 0xFF, 0x00, 0x00,
  REPORT_SIZE, 0x10,						// 16 bit report
  USB_FEATURE, DATA_VAR_ABS,					// feature


  END_COLLECTION,		// 18 BYTES		// logical collection END


  //REPORT_COUNT, 0x01,					// last few bits to round out the byte
  //REPORT_SIZE, 0x02, //should be 2!!
  //USB_FEATURE, CONST_VAR_ABS,				// constant output


  //END_COLLECTION,		// 7 BYTES	// application collection END

				// 78 BYTES FOR RUMBLE

  //END_COLLECTION,

  /* USER CODE END 0 */
  0xC0    /*     END_COLLECTION	             */
};

/* USER CODE BEGIN PRIVATE_VARIABLES */

/* USER CODE END PRIVATE_VARIABLES */

/**
  * @}
  */

/** @defgroup USBD_CUSTOM_HID_Exported_Variables USBD_CUSTOM_HID_Exported_Variables
  * @brief Public variables.
  * @{
  */
extern USBD_HandleTypeDef hUsbDeviceFS;

/* USER CODE BEGIN EXPORTED_VARIABLES */

/* USER CODE END EXPORTED_VARIABLES */
/**
  * @}
  */

/** @defgroup USBD_CUSTOM_HID_Private_FunctionPrototypes USBD_CUSTOM_HID_Private_FunctionPrototypes
  * @brief Private functions declaration.
  * @{
  */

static int8_t CUSTOM_HID_Init_FS(void);
static int8_t CUSTOM_HID_DeInit_FS(void);
static int8_t CUSTOM_HID_OutEvent_FS(uint8_t event_idx, uint8_t state);

/**
  * @}
  */

USBD_CUSTOM_HID_ItfTypeDef USBD_CustomHID_fops_FS =
{
  CUSTOM_HID_ReportDesc_FS,
  CUSTOM_HID_Init_FS,
  CUSTOM_HID_DeInit_FS,
  CUSTOM_HID_OutEvent_FS
};

/** @defgroup USBD_CUSTOM_HID_Private_Functions USBD_CUSTOM_HID_Private_Functions
  * @brief Private functions.
  * @{
  */

/* Private functions ---------------------------------------------------------*/

/**
  * @brief  Initializes the CUSTOM HID media low layer
  * @retval USBD_OK if all operations are OK else USBD_FAIL
  */
static int8_t CUSTOM_HID_Init_FS(void)
{
  /* USER CODE BEGIN 4 */
  return (USBD_OK);
  /* USER CODE END 4 */
}

/**
  * @brief  DeInitializes the CUSTOM HID media low layer
  * @retval USBD_OK if all operations are OK else USBD_FAIL
  */
static int8_t CUSTOM_HID_DeInit_FS(void)
{
  /* USER CODE BEGIN 5 */
  return (USBD_OK);
  /* USER CODE END 5 */
}

/**
  * @brief  Manage the CUSTOM HID class events
  * @param  event_idx: Event index
  * @param  state: Event state
  * @retval USBD_OK if all operations are OK else USBD_FAIL
  */
static int8_t CUSTOM_HID_OutEvent_FS(uint8_t event_idx, uint8_t state)
{
  /* USER CODE BEGIN 6 */
  UNUSED(event_idx);
  UNUSED(state);

  /* Start next USB packet transfer once data processing is completed */
  USBD_CUSTOM_HID_ReceivePacket(&hUsbDeviceFS);

  return (USBD_OK);
  /* USER CODE END 6 */
}

/* USER CODE BEGIN 7 */
/**
  * @brief  Send the report to the Host
  * @param  report: The report to be sent
  * @param  len: The report length
  * @retval USBD_OK if all operations are OK else USBD_FAIL
  */
/*
static int8_t USBD_CUSTOM_HID_SendReport_FS(uint8_t *report, uint16_t len)
{
  return USBD_CUSTOM_HID_SendReport(&hUsbDeviceFS, report, len);
}
*/
/* USER CODE END 7 */

/* USER CODE BEGIN PRIVATE_FUNCTIONS_IMPLEMENTATION */

/* USER CODE END PRIVATE_FUNCTIONS_IMPLEMENTATION */
/**
  * @}
  */

/**
  * @}
  */

/**
  * @}
  */

