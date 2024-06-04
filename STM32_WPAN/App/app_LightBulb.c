
/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file    App/zigbee.c
  * @author  MCD Application Team
  * @brief   Zigbee Application.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2023 STMicroelectronics.
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
#include "app_common.h"
#include "app_entry.h"
#include "dbg_trace.h"
#include "app_zigbee.h"
#include "zigbee_interface.h"
#include "shci.h"
#include "stm_logging.h"
#include "app_conf.h"
#include "stm32wbxx_core_interface_def.h"
#include "zigbee_types.h"
#include "cmsis_os.h"

/* Private includes -----------------------------------------------------------*/
#include <assert.h>
#include "zcl/zcl.h"
#include "zcl/general/zcl.onoff.h"
#include "zcl/general/zcl.identify.h"

/* USER CODE BEGIN Includes */
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */
/* USER CODE END PTD */

/* Private defines -----------------------------------------------------------*/

/* USER CODE BEGIN PD */
/* USER CODE END PD */

/* Private macros ------------------------------------------------------------*/
/* USER CODE BEGIN PM */
/* USER CODE END PM */

/* External definition -------------------------------------------------------*/
extern struct zigbee_app_info zigbee_app_info;

/* USER CODE BEGIN ED */
/* USER CODE END ED */

/* Private function prototypes -----------------------------------------------*/


/* USER CODE BEGIN PFP */
/* OnOff server 1 custom callbacks */
enum ZclStatusCodeT onOff_server_1_off(struct ZbZclClusterT *cluster, struct ZbZclAddrInfoT *srcInfo, void *arg);
enum ZclStatusCodeT onOff_server_1_on(struct ZbZclClusterT *cluster, struct ZbZclAddrInfoT *srcInfo, void *arg);
enum ZclStatusCodeT onOff_server_1_toggle(struct ZbZclClusterT *cluster, struct ZbZclAddrInfoT *srcInfo, void *arg);

/* USER CODE END PFP */

/* Private variables ---------------------------------------------------------*/
struct ZbZclOnOffServerCallbacksT OnOffServerCallbacks_1 =
{
  .off = onOff_server_1_off,
  .on = onOff_server_1_on,
  .toggle = onOff_server_1_toggle,
};

/* FreeRtos stacks attributes */

/* USER CODE BEGIN PV */
/* USER CODE END PV */
/* Functions Definition ------------------------------------------------------*/

/**
 * @brief  Configure Zigbee light bulb application endpoint
 * @param  None
 * @retval None
 */
void APP_ZIGBEE_LightBulb_ConfigEndpoints(void)
{
  /* OnOff server */
  zigbee_app_info.onOff_server_1 = ZbZclOnOffServerAlloc(zigbee_app_info.zb, SW1_ENDPOINT, &OnOffServerCallbacks_1, NULL);
  assert(zigbee_app_info.onOff_server_1 != NULL);
  ZbZclClusterEndpointRegister(zigbee_app_info.onOff_server_1);

  /* Identify server */
  zigbee_app_info.identify_server_1 = ZbZclIdentifyServerAlloc(
  zigbee_app_info.zb, SW1_ENDPOINT, NULL);
  assert(zigbee_app_info.identify_server_1 != NULL);
  ZbZclClusterEndpointRegister(zigbee_app_info.identify_server_1);
}

/**
 * @brief  OnOff server off 1 command callback
 * @param  struct ZbZclClusterT *cluster, struct ZbZclAddrInfoT *srcInfo, void *arg
 * @retval  enum ZclStatusCodeT
 */
enum ZclStatusCodeT onOff_server_1_off(struct ZbZclClusterT *cluster, struct ZbZclAddrInfoT *srcInfo, void *arg)
{
  /* USER CODE BEGIN 0 OnOff server 1 off 1 */
  uint8_t endpoint;

  endpoint = ZbZclClusterGetEndpoint(cluster);
  if (endpoint == SW1_ENDPOINT)
  {
    APP_DBG("LED_RED OFF");
    BSP_LED_Off(LED_RED);
    (void)ZbZclAttrIntegerWrite(cluster, ZCL_ONOFF_ATTR_ONOFF, 0);
  }
  else
  {
    /* Unknown endpoint */
    return ZCL_STATUS_FAILURE;
  }

  return ZCL_STATUS_SUCCESS;
  /* USER CODE END 0 OnOff server 1 off 1 */
}

/**
 * @brief  OnOff server on 1 command callback
 * @param  struct ZbZclClusterT *cluster, struct ZbZclAddrInfoT *srcInfo, void *arg
 * @retval  enum ZclStatusCodeT
 */
enum ZclStatusCodeT onOff_server_1_on(struct ZbZclClusterT *cluster, struct ZbZclAddrInfoT *srcInfo, void *arg)
{
  /* USER CODE BEGIN 1 OnOff server 1 on 1 */
  uint8_t endpoint;

  endpoint = ZbZclClusterGetEndpoint(cluster);
  if (endpoint == SW1_ENDPOINT)
  {
    APP_DBG("LED_RED ON");
    BSP_LED_On(LED_RED);
    (void)ZbZclAttrIntegerWrite(cluster, ZCL_ONOFF_ATTR_ONOFF, 1);
  }
  else
  {
    /* Unknown endpoint */
    return ZCL_STATUS_FAILURE;
  }

  return ZCL_STATUS_SUCCESS;
  /* USER CODE END 1 OnOff server 1 on 1 */
}

/**
 * @brief  OnOff server toggle command callback
 * @param  struct ZbZclClusterT *cluster, struct ZbZclAddrInfoT *srcInfo, void *arg
 * @retval  enum ZclStatusCodeT
 */
enum ZclStatusCodeT onOff_server_1_toggle(struct ZbZclClusterT *cluster, struct ZbZclAddrInfoT *srcInfo, void *arg)
{
  /* USER CODE BEGIN 2 OnOff server 1 toggle 1 */
  uint8_t attrVal;

  if (ZbZclAttrRead(cluster, ZCL_ONOFF_ATTR_ONOFF, NULL,
            &attrVal, sizeof(attrVal), false) != ZCL_STATUS_SUCCESS)
  {
    return ZCL_STATUS_FAILURE;
  }

  if (attrVal != 0)
  {
    return onOff_server_1_off(cluster, srcInfo, arg);
  }
  else
  {
    return onOff_server_1_on(cluster, srcInfo, arg);
  }
  /* USER CODE END 2 OnOff server 1 toggle 1 */
}

