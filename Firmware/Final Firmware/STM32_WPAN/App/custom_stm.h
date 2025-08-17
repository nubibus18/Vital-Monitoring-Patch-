/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file    App/custom_stm.h
  * @author  MCD Application Team
  * @brief   Header for custom_stm.c module.
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

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef CUSTOM_STM_H
#define CUSTOM_STM_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Exported types ------------------------------------------------------------*/
typedef enum
{
  /* BalodiHeart */
  CUSTOM_STM_BHW,
  CUSTOM_STM_BHR,
  CUSTOM_STM_BHRV,
} Custom_STM_Char_Opcode_t;

typedef enum
{
  /* BalodiHeartWave */
  CUSTOM_STM_BHW_READ_EVT,
  CUSTOM_STM_BHW_WRITE_NO_RESP_EVT,
  CUSTOM_STM_BHW_WRITE_EVT,
  CUSTOM_STM_BHW_NOTIFY_ENABLED_EVT,
  CUSTOM_STM_BHW_NOTIFY_DISABLED_EVT,
  CUSTOM_STM_BHW_INDICATE_ENABLED_EVT,
  CUSTOM_STM_BHW_INDICATE_DISABLED_EVT,
  /* BalodiHeartRate */
  CUSTOM_STM_BHR_READ_EVT,
  CUSTOM_STM_BHR_WRITE_NO_RESP_EVT,
  CUSTOM_STM_BHR_WRITE_EVT,
  CUSTOM_STM_BHR_NOTIFY_ENABLED_EVT,
  CUSTOM_STM_BHR_NOTIFY_DISABLED_EVT,
  CUSTOM_STM_BHR_INDICATE_ENABLED_EVT,
  CUSTOM_STM_BHR_INDICATE_DISABLED_EVT,
  /* BalodiHRV */
  CUSTOM_STM_BHRV_READ_EVT,
  CUSTOM_STM_BHRV_WRITE_NO_RESP_EVT,
  CUSTOM_STM_BHRV_WRITE_EVT,
  CUSTOM_STM_BHRV_NOTIFY_ENABLED_EVT,
  CUSTOM_STM_BHRV_NOTIFY_DISABLED_EVT,
  CUSTOM_STM_BHRV_INDICATE_ENABLED_EVT,
  CUSTOM_STM_BHRV_INDICATE_DISABLED_EVT,
  CUSTOM_STM_NOTIFICATION_COMPLETE_EVT,

  CUSTOM_STM_BOOT_REQUEST_EVT
} Custom_STM_Opcode_evt_t;

typedef struct
{
  uint8_t * pPayload;
  uint8_t   Length;
} Custom_STM_Data_t;

typedef struct
{
  Custom_STM_Opcode_evt_t       Custom_Evt_Opcode;
  Custom_STM_Data_t             DataTransfered;
  uint16_t                      ConnectionHandle;
  uint8_t                       ServiceInstance;
  uint16_t                      AttrHandle;
} Custom_STM_App_Notification_evt_t;

/* USER CODE BEGIN ET */

/* USER CODE END ET */

/* Exported constants --------------------------------------------------------*/
extern uint16_t SizeBhw;
extern uint16_t SizeBhr;
extern uint16_t SizeBhrv;

/* USER CODE BEGIN EC */

/* USER CODE END EC */

/* External variables --------------------------------------------------------*/
/* USER CODE BEGIN EV */

/* USER CODE END EV */

/* Exported macros -----------------------------------------------------------*/
/* USER CODE BEGIN EM */

/* USER CODE END EM */

/* Exported functions ------------------------------------------------------- */
void SVCCTL_InitCustomSvc(void);
void Custom_STM_App_Notification(Custom_STM_App_Notification_evt_t *pNotification);
tBleStatus Custom_STM_App_Update_Char(Custom_STM_Char_Opcode_t CharOpcode,  uint8_t *pPayload);
tBleStatus Custom_STM_App_Update_Char_Variable_Length(Custom_STM_Char_Opcode_t CharOpcode, uint8_t *pPayload, uint8_t size);
tBleStatus Custom_STM_App_Update_Char_Ext(uint16_t Connection_Handle, Custom_STM_Char_Opcode_t CharOpcode, uint8_t *pPayload);
/* USER CODE BEGIN EF */

/* USER CODE END EF */

#ifdef __cplusplus
}
#endif

#endif /*CUSTOM_STM_H */
