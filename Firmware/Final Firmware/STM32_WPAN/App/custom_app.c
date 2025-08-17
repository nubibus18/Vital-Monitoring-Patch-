/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file    App/custom_app.c
  * @author  MCD Application Team
  * @brief   Custom Example Application (Server)
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2025 STMicroelectronics.
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
#include "app_common.h"
#include "dbg_trace.h"
#include "ble.h"
#include "custom_app.h"
#include "custom_stm.h"
#include "stm32_seq.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
extern I2C_HandleTypeDef hi2c1;  // Add this to access the I2C handle
#include "ADPD1080.h"

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
typedef struct
{
  /* BalodiHeart */
  uint8_t               Bhw_Notification_Status;
  uint8_t               Bhw_Indication_Status;
  uint8_t               Bhr_Notification_Status;
  uint8_t               Bhr_Indication_Status;
  uint8_t               Bhrv_Notification_Status;
  uint8_t               Bhrv_Indication_Status;
  /* USER CODE BEGIN CUSTOM_APP_Context_t */
  uint8_t               TimerMeasurement_Id;
  uint8_t               FastTimerMeasurement_Id;  // Added for 0.3s timer
  /* USER CODE END CUSTOM_APP_Context_t */

  uint16_t              ConnectionHandle;
} Custom_App_Context_t;

/* USER CODE BEGIN PTD */
// Task ID for the BalodiHeart measurement
#define CFG_TASK_BHW_MEAS_REQ_ID            (0x10)
#define CFG_TASK_FAST_MEAS_REQ_ID           (0x11)  // Added for 0.3s timer
/* USER CODE END PTD */

/* Private defines ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
#define SEC_TO_TICKS(__SEC__)  ((__SEC__) * 1600U)  // 1 second = 1600 ticks
#define MS_TO_TICKS(__MS__)    ((__MS__) * 16U)    // 1 ms = 1.6 ticks
/* USER CODE END PD */

/* Private macros -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
/**
 * START of Section BLE_APP_CONTEXT
 */

static Custom_App_Context_t Custom_App_Context;

/**
 * END of Section BLE_APP_CONTEXT
 */

uint8_t UpdateCharData[512];
uint8_t NotifyCharData[512];
uint16_t Connection_Handle;
/* USER CODE BEGIN PV */
uint8_t Count = 0;

volatile uint8_t notification_allowed = 0;
tBleStatus Test;
uint8_t Notifyindex = 0;
uint8_t Fall=0;
uint16_t steps;
//volatile uint8_t g_data_ready_flag ;
//volatile uint32_t drdy_captured_count = 0;
volatile int32_t g_channel_values[6];
volatile uint32_t g_ble_error_count=0;

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
/* BalodiHeart */
static void Custom_Bhw_Update_Char(void);
static void Custom_Bhw_Send_Notification(void);
static void Custom_Bhw_Send_Indication(void);
static void Custom_Bhr_Update_Char(void);
static void Custom_Bhr_Send_Notification(void);
static void Custom_Bhr_Send_Indication(void);
static void Custom_Bhrv_Update_Char(void);
static void Custom_Bhrv_Send_Notification(void);
static void Custom_Bhrv_Send_Indication(void);

/* USER CODE BEGIN PFP */
static void BhwMeas_PeriodicTask(void);
static void FastMeas_PeriodicTask(void);  // Added for 0.3s timer
static void Custom_Fast_Send_Notification(void);  // Added for 0.3s timer
/* USER CODE END PFP */

/* Functions Definition ------------------------------------------------------*/
void Custom_STM_App_Notification(Custom_STM_App_Notification_evt_t *pNotification)
{
  /* USER CODE BEGIN CUSTOM_STM_App_Notification_1 */

  /* USER CODE END CUSTOM_STM_App_Notification_1 */
  switch (pNotification->Custom_Evt_Opcode)
  {
    /* USER CODE BEGIN CUSTOM_STM_App_Notification_Custom_Evt_Opcode */

    /* USER CODE END CUSTOM_STM_App_Notification_Custom_Evt_Opcode */

    /* BalodiHeart */
    case CUSTOM_STM_BHW_READ_EVT:
      /* USER CODE BEGIN CUSTOM_STM_BHW_READ_EVT */

      /* USER CODE END CUSTOM_STM_BHW_READ_EVT */
      break;

    case CUSTOM_STM_BHW_WRITE_NO_RESP_EVT:
      /* USER CODE BEGIN CUSTOM_STM_BHW_WRITE_NO_RESP_EVT */

      /* USER CODE END CUSTOM_STM_BHW_WRITE_NO_RESP_EVT */
      break;

    case CUSTOM_STM_BHW_WRITE_EVT:
      /* USER CODE BEGIN CUSTOM_STM_BHW_WRITE_EVT */

      /* USER CODE END CUSTOM_STM_BHW_WRITE_EVT */
      break;

    case CUSTOM_STM_BHW_NOTIFY_ENABLED_EVT:
      /* USER CODE BEGIN CUSTOM_STM_BHW_NOTIFY_ENABLED_EVT */
      Custom_App_Context.Bhw_Notification_Status = 1;
      notification_allowed = 1;
      /* USER CODE END CUSTOM_STM_BHW_NOTIFY_ENABLED_EVT */
      break;

    case CUSTOM_STM_BHW_NOTIFY_DISABLED_EVT:
      /* USER CODE BEGIN CUSTOM_STM_BHW_NOTIFY_DISABLED_EVT */
      Custom_App_Context.Bhw_Notification_Status = 0;
      notification_allowed = 0;
      /* USER CODE END CUSTOM_STM_BHW_NOTIFY_DISABLED_EVT */
      break;

    case CUSTOM_STM_BHW_INDICATE_ENABLED_EVT:
      /* USER CODE BEGIN CUSTOM_STM_BHW_INDICATE_ENABLED_EVT */

      /* USER CODE END CUSTOM_STM_BHW_INDICATE_ENABLED_EVT */
      break;

    case CUSTOM_STM_BHW_INDICATE_DISABLED_EVT:
      /* USER CODE BEGIN CUSTOM_STM_BHW_INDICATE_DISABLED_EVT */

      /* USER CODE END CUSTOM_STM_BHW_INDICATE_DISABLED_EVT */
      break;

    case CUSTOM_STM_BHR_READ_EVT:
      /* USER CODE BEGIN CUSTOM_STM_BHR_READ_EVT */

      /* USER CODE END CUSTOM_STM_BHR_READ_EVT */
      break;

    case CUSTOM_STM_BHR_WRITE_NO_RESP_EVT:
      /* USER CODE BEGIN CUSTOM_STM_BHR_WRITE_NO_RESP_EVT */

      /* USER CODE END CUSTOM_STM_BHR_WRITE_NO_RESP_EVT */
      break;

    case CUSTOM_STM_BHR_WRITE_EVT:
      /* USER CODE BEGIN CUSTOM_STM_BHR_WRITE_EVT */

      /* USER CODE END CUSTOM_STM_BHR_WRITE_EVT */
      break;

    case CUSTOM_STM_BHR_NOTIFY_ENABLED_EVT:
      /* USER CODE BEGIN CUSTOM_STM_BHR_NOTIFY_ENABLED_EVT */
      Custom_App_Context.Bhr_Notification_Status = 1;
      /* USER CODE END CUSTOM_STM_BHR_NOTIFY_ENABLED_EVT */
      break;

    case CUSTOM_STM_BHR_NOTIFY_DISABLED_EVT:
      /* USER CODE BEGIN CUSTOM_STM_BHR_NOTIFY_DISABLED_EVT */
      Custom_App_Context.Bhr_Notification_Status = 0;
      /* USER CODE END CUSTOM_STM_BHR_NOTIFY_DISABLED_EVT */
      break;

    case CUSTOM_STM_BHR_INDICATE_ENABLED_EVT:
      /* USER CODE BEGIN CUSTOM_STM_BHR_INDICATE_ENABLED_EVT */

      /* USER CODE END CUSTOM_STM_BHR_INDICATE_ENABLED_EVT */
      break;

    case CUSTOM_STM_BHR_INDICATE_DISABLED_EVT:
      /* USER CODE BEGIN CUSTOM_STM_BHR_INDICATE_DISABLED_EVT */

      /* USER CODE END CUSTOM_STM_BHR_INDICATE_DISABLED_EVT */
      break;

    case CUSTOM_STM_BHRV_READ_EVT:
      /* USER CODE BEGIN CUSTOM_STM_BHRV_READ_EVT */

      /* USER CODE END CUSTOM_STM_BHRV_READ_EVT */
      break;

    case CUSTOM_STM_BHRV_WRITE_NO_RESP_EVT:
      /* USER CODE BEGIN CUSTOM_STM_BHRV_WRITE_NO_RESP_EVT */

      /* USER CODE END CUSTOM_STM_BHRV_WRITE_NO_RESP_EVT */
      break;

    case CUSTOM_STM_BHRV_WRITE_EVT:
      /* USER CODE BEGIN CUSTOM_STM_BHRV_WRITE_EVT */

      /* USER CODE END CUSTOM_STM_BHRV_WRITE_EVT */
      break;

    case CUSTOM_STM_BHRV_NOTIFY_ENABLED_EVT:
      /* USER CODE BEGIN CUSTOM_STM_BHRV_NOTIFY_ENABLED_EVT */
      Custom_App_Context.Bhrv_Notification_Status = 1;

      /* USER CODE END CUSTOM_STM_BHRV_NOTIFY_ENABLED_EVT */
      break;

    case CUSTOM_STM_BHRV_NOTIFY_DISABLED_EVT:
      /* USER CODE BEGIN CUSTOM_STM_BHRV_NOTIFY_DISABLED_EVT */
      Custom_App_Context.Bhrv_Notification_Status = 0;
      ADPD1080_LEDoff(&hi2c1);
      /* USER CODE END CUSTOM_STM_BHRV_NOTIFY_DISABLED_EVT */
      break;

    case CUSTOM_STM_BHRV_INDICATE_ENABLED_EVT:
      /* USER CODE BEGIN CUSTOM_STM_BHRV_INDICATE_ENABLED_EVT */

      /* USER CODE END CUSTOM_STM_BHRV_INDICATE_ENABLED_EVT */
      break;

    case CUSTOM_STM_BHRV_INDICATE_DISABLED_EVT:
      /* USER CODE BEGIN CUSTOM_STM_BHRV_INDICATE_DISABLED_EVT */

      /* USER CODE END CUSTOM_STM_BHRV_INDICATE_DISABLED_EVT */
      break;

    case CUSTOM_STM_NOTIFICATION_COMPLETE_EVT:
      /* USER CODE BEGIN CUSTOM_STM_NOTIFICATION_COMPLETE_EVT */

      /* USER CODE END CUSTOM_STM_NOTIFICATION_COMPLETE_EVT */
      break;

    default:
      /* USER CODE BEGIN CUSTOM_STM_App_Notification_default */

      /* USER CODE END CUSTOM_STM_App_Notification_default */
      break;
  }
  /* USER CODE BEGIN CUSTOM_STM_App_Notification_2 */

  /* USER CODE END CUSTOM_STM_App_Notification_2 */
  return;
}

void Custom_APP_Notification(Custom_App_ConnHandle_Not_evt_t *pNotification)
{
  /* USER CODE BEGIN CUSTOM_APP_Notification_1 */

  /* USER CODE END CUSTOM_APP_Notification_1 */

  switch (pNotification->Custom_Evt_Opcode)
  {
    /* USER CODE BEGIN CUSTOM_APP_Notification_Custom_Evt_Opcode */

    /* USER CODE END P2PS_CUSTOM_Notification_Custom_Evt_Opcode */
    case CUSTOM_CONN_HANDLE_EVT :
      /* USER CODE BEGIN CUSTOM_CONN_HANDLE_EVT */
      Custom_App_Context.ConnectionHandle = pNotification->ConnectionHandle;
      /* USER CODE END CUSTOM_CONN_HANDLE_EVT */
      break;

    case CUSTOM_DISCON_HANDLE_EVT :
      /* USER CODE BEGIN CUSTOM_DISCON_HANDLE_EVT */
      Custom_App_Context.ConnectionHandle = 0;
      notification_allowed = 0;
      Custom_App_Context.Bhw_Notification_Status = 0;
      Custom_App_Context.Bhr_Notification_Status = 0;
      Custom_App_Context.Bhrv_Notification_Status = 0;
      /* USER CODE END CUSTOM_DISCON_HANDLE_EVT */
      break;

    default:
      /* USER CODE BEGIN CUSTOM_APP_Notification_default */

      /* USER CODE END CUSTOM_APP_Notification_default */
      break;
  }

  /* USER CODE BEGIN CUSTOM_APP_Notification_2 */

  /* USER CODE END CUSTOM_APP_Notification_2 */

  return;
}

void Custom_APP_Init(void)
{
  /* USER CODE BEGIN CUSTOM_APP_Init */
  // Initialize the NotifyCharData buffer
  NotifyCharData[0] = 0;
  NotifyCharData[1] = 0;

  // Register the BalodiHeart measurement task
  UTIL_SEQ_RegTask(1 << CFG_TASK_BHW_MEAS_REQ_ID, UTIL_SEQ_RFU, Custom_Bhw_Send_Notification);

  // Register the fast measurement task
  UTIL_SEQ_RegTask(1 << CFG_TASK_FAST_MEAS_REQ_ID, UTIL_SEQ_RFU, Custom_Fast_Send_Notification);

  // Create timer for BalodiHeart Measurement
  HW_TS_Create(CFG_TIM_PROC_ID_ISR, &(Custom_App_Context.TimerMeasurement_Id), hw_ts_Repeated, BhwMeas_PeriodicTask);
  // Create timer for Fast Measurement (0.3s)
  HW_TS_Create(CFG_TIM_PROC_ID_ISR, &(Custom_App_Context.FastTimerMeasurement_Id), hw_ts_Repeated, FastMeas_PeriodicTask);

  // Start the timer to trigger measurements every 0.2 seconds
  HW_TS_Start(Custom_App_Context.TimerMeasurement_Id, SEC_TO_TICKS(0.2));
  // Start the fast timer to trigger every 5 seconds
  HW_TS_Start(Custom_App_Context.FastTimerMeasurement_Id, SEC_TO_TICKS(5));

  /* USER CODE END CUSTOM_APP_Init */
  return;
}

/* USER CODE BEGIN FD */

/* USER CODE END FD */

/*************************************************************
 *
 * LOCAL FUNCTIONS
 *
 *************************************************************/

/* BalodiHeart */
__USED void Custom_Bhw_Update_Char(void) /* Property Read */
{
  uint8_t updateflag = 0;

  /* USER CODE BEGIN Bhw_UC_1*/

  /* USER CODE END Bhw_UC_1*/

  if (updateflag != 0)
  {
	Custom_STM_App_Update_Char_Ext(Connection_Handle, CUSTOM_STM_BHW, (uint8_t *)UpdateCharData);
  }

  /* USER CODE BEGIN Bhw_UC_Last*/

  /* USER CODE END Bhw_UC_Last*/
  return;
}

void Custom_Bhw_Send_Notification(void) /* Property Notification */
{
  uint8_t updateflag = 0;

  /* USER CODE BEGIN Bhw_NS_1*/
	  // Only send notification if it's allowed (client has enabled notifications)
	  if (Custom_App_Context.Bhw_Notification_Status == 0)
	  {
		  ADPD1080_LEDoff(&hi2c1);
	    return; // Exit if notifications are not enabled
	  }


	  Count++;
	  ADPD1080_Check_FIFO(&hi2c1);  // fetch new row
	  ADPD1080_LEDon(&hi2c1);
	  if (NewReading_Check==1){
	    for (int i = 0; i < 7; i++) {  // 7 values per row
	      NotifyCharData[Notifyindex++] = (adpd1080_actual_reading_row_4[i] >> 24) & 0xFF;
	      NotifyCharData[Notifyindex++] = (adpd1080_actual_reading_row_4[i] >> 16) & 0xFF;
	      NotifyCharData[Notifyindex++] = (adpd1080_actual_reading_row_4[i] >> 8) & 0xFF;
	      NotifyCharData[Notifyindex++] = adpd1080_actual_reading_row_4[i] & 0xFF;
	    }
	  }
	  NewReading_Check = 0;
	  if (Notifyindex >= 252) {
	      Notifyindex = 0;
	      updateflag = 1;
	    }

  /* USER CODE END Bhw_NS_1*/

  if (updateflag != 0)
  {
	Custom_STM_App_Update_Char_Ext(Connection_Handle, CUSTOM_STM_BHW, (uint8_t *)NotifyCharData);
  }

  /* USER CODE BEGIN Bhw_NS_Last*/

  /* USER CODE END Bhw_NS_Last*/

  return;
}

void Custom_Bhw_Send_Indication(void) /* Property Indication */
{
  uint8_t updateflag = 0;

  /* USER CODE BEGIN Bhw_IS_1*/
  

  /* USER CODE END Bhw_IS_1*/

  if (updateflag != 0)
  {
    Custom_STM_App_Update_Char_Ext(Connection_Handle, CUSTOM_STM_BHW, (uint8_t *)NotifyCharData);
  }

  /* USER CODE BEGIN Bhw_IS_Last*/

  /* USER CODE END Bhw_IS_Last*/

  return;
}

__USED void Custom_Bhr_Update_Char(void) /* Property Read */
{
  uint8_t updateflag = 0;

  /* USER CODE BEGIN Bhr_UC_1*/
  UpdateCharData[0] = Count;
  UpdateCharData[1] = Count + 1;
  updateflag = 1;
  /* USER CODE END Bhr_UC_1*/

  if (updateflag != 0)
  {
    Custom_STM_App_Update_Char(CUSTOM_STM_BHR, (uint8_t *)UpdateCharData);
  }

  /* USER CODE BEGIN Bhr_UC_Last*/

  /* USER CODE END Bhr_UC_Last*/
  return;
}

void Custom_Bhr_Send_Notification(void) /* Property Notification */
{
  uint8_t updateflag = 0;

  /* USER CODE BEGIN Bhr_NS_1*/
  // Only send notification if it's allowed (client has enabled notifications)
  if (Custom_App_Context.Bhr_Notification_Status == 0)
  {
    return; // Exit if notifications are not enabled
  }

  if (Fall == 1) {
    NotifyCharData[0] = 0xFF; // Fall detected
    updateflag = 1;
    Fall = 0;
  } else {
    NotifyCharData[0] = 0x00; // No fall detected
    Fall = 1;
    updateflag = 1;
  }
  NotifyCharData[0]=steps>>8;
  NotifyCharData[1]=steps&&0xFF;
  /* USER CODE END Bhr_NS_1*/

  if (updateflag != 0)
  {
    Custom_STM_App_Update_Char(CUSTOM_STM_BHR, (uint8_t *)NotifyCharData);
  }

  /* USER CODE BEGIN Bhr_NS_Last*/

  /* USER CODE END Bhr_NS_Last*/

  return;
}

void Custom_Bhr_Send_Indication(void) /* Property Indication */
{
  uint8_t updateflag = 0;

  /* USER CODE BEGIN Bhr_IS_1*/

  /* USER CODE END Bhr_IS_1*/

  if (updateflag != 0)
  {
	Custom_STM_App_Update_Char(CUSTOM_STM_BHR, (uint8_t *)NotifyCharData);
  }

  /* USER CODE BEGIN Bhr_IS_Last*/

  /* USER CODE END Bhr_IS_Last*/

  return;
}

__USED void Custom_Bhrv_Update_Char(void) /* Property Read */
{
  uint8_t updateflag = 0;

  /* USER CODE BEGIN Bhrv_UC_1*/

  /* USER CODE END Bhrv_UC_1*/

  if (updateflag != 0)
  {
    Custom_STM_App_Update_Char(CUSTOM_STM_BHRV, (uint8_t *)UpdateCharData);
  }

  /* USER CODE BEGIN Bhrv_UC_Last*/

  /* USER CODE END Bhrv_UC_Last*/
  return;
}

void Custom_Bhrv_Send_Notification(void) /* Property Notification */
{
  uint8_t updateflag = 0;

  /* USER CODE BEGIN Bhrv_NS_1*/
  // Only send notification if it's allowed (client has enabled notifications)
  if (Custom_App_Context.Bhrv_Notification_Status == 0)
  {
    return; // Exit if notifications are not enabled
  }

  // Use different data for BHRV to differentiate
  NotifyCharData[0] = 0xAA; // HRV identification value
  NotifyCharData[1] = Count;
  updateflag = 1;
  /* USER CODE END Bhrv_NS_1*/

  if (updateflag != 0)
  {
    Custom_STM_App_Update_Char(CUSTOM_STM_BHRV, (uint8_t *)NotifyCharData);
  }

  /* USER CODE BEGIN Bhrv_NS_Last*/

  /* USER CODE END Bhrv_NS_Last*/

  return;
}

void Custom_Bhrv_Send_Indication(void) /* Property Indication */
{
  uint8_t updateflag = 0;

  /* USER CODE BEGIN Bhrv_IS_1*/

  /* USER CODE END Bhrv_IS_1*/

  if (updateflag != 0)
  {
	Custom_STM_App_Update_Char(CUSTOM_STM_BHRV, (uint8_t *)NotifyCharData);
  }

  /* USER CODE BEGIN Bhrv_IS_Last*/

  /* USER CODE END Bhrv_IS_Last*/

  return;
}

/* USER CODE BEGIN FD_LOCAL_FUNCTIONS*/
/**
 * @brief  Periodic task triggered by timer that requests the measurement task
 * @param  None
 * @retval None
 */
static void BhwMeas_PeriodicTask(void)
{
  // Request execution of the measurement task
  UTIL_SEQ_SetTask(1 << CFG_TASK_BHW_MEAS_REQ_ID, CFG_SCH_PRIO_0);
}

/**
 * @brief  Fast periodic task triggered every 5 seconds
 * @param  None
 * @retval None
 */
static void FastMeas_PeriodicTask(void)
{
  // Request execution of the fast measurement task
  UTIL_SEQ_SetTask(1 << CFG_TASK_FAST_MEAS_REQ_ID, CFG_SCH_PRIO_0);
}

/**
 * @brief  Fast measurement task that handles both BHR and BHRV notifications
 * @param  None
 * @retval None
 */
static void Custom_Fast_Send_Notification(void)
{
  // Send notifications for both BHR and BHRV if enabled
  if (Custom_App_Context.Bhr_Notification_Status)
  {
    Custom_Bhr_Send_Notification();
  }
  
  if (Custom_App_Context.Bhrv_Notification_Status)
  {
    Custom_Bhrv_Send_Notification();
  }
}
/* USER CODE END FD_LOCAL_FUNCTIONS*/
