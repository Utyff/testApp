/**************************************************************************************************
  Filename:       zcl_testapp_data.c
  Revised:        $Date: 2014-05-12 13:14:02 -0700 (Mon, 12 May 2014) $
  Revision:       $Revision: 38502 $


  Description:    Zigbee Cluster Library - sample device application.


  Copyright 2006-2014 Texas Instruments Incorporated. All rights reserved.

  IMPORTANT: Your use of this Software is limited to those specific rights
  granted under the terms of a software license agreement between the user
  who downloaded the software, his/her employer (which must be your employer)
  and Texas Instruments Incorporated (the "License").  You may not use this
  Software unless you agree to abide by the terms of the License. The License
  limits your use, and you acknowledge, that the Software may not be modified,
  copied or distributed unless embedded on a Texas Instruments microcontroller
  or used solely and exclusively in conjunction with a Texas Instruments radio
  frequency transceiver, which is integrated into your product.  Other than for
  the foregoing purpose, you may not use, reproduce, copy, prepare derivative
  works of, modify, distribute, perform, display or sell this Software and/or
  its documentation for any purpose.

  YOU FURTHER ACKNOWLEDGE AND AGREE THAT THE SOFTWARE AND DOCUMENTATION ARE
  PROVIDED "AS IS" WITHOUT WARRANTY OF ANY KIND, EITHER EXPRESS OR IMPLIED,
  INCLUDING WITHOUT LIMITATION, ANY WARRANTY OF MERCHANTABILITY, TITLE,
  NON-INFRINGEMENT AND FITNESS FOR A PARTICULAR PURPOSE. IN NO EVENT SHALL
  TEXAS INSTRUMENTS OR ITS LICENSORS BE LIABLE OR OBLIGATED UNDER CONTRACT,
  NEGLIGENCE, STRICT LIABILITY, CONTRIBUTION, BREACH OF WARRANTY, OR OTHER
  LEGAL EQUITABLE THEORY ANY DIRECT OR INDIRECT DAMAGES OR EXPENSES
  INCLUDING BUT NOT LIMITED TO ANY INCIDENTAL, SPECIAL, INDIRECT, PUNITIVE
  OR CONSEQUENTIAL DAMAGES, LOST PROFITS OR LOST DATA, COST OF PROCUREMENT
  OF SUBSTITUTE GOODS, TECHNOLOGY, SERVICES, OR ANY CLAIMS BY THIRD PARTIES
  (INCLUDING BUT NOT LIMITED TO ANY DEFENSE THEREOF), OR OTHER SIMILAR COSTS.

  Should you have any questions regarding your right to use this Software,
  contact Texas Instruments Incorporated at www.TI.com.
**************************************************************************************************/

/*********************************************************************
 * INCLUDES
 */
#include "ZComDef.h"
#include "OSAL.h"
#include "AF.h"
#include "ZDConfig.h"

#include "zcl.h"
#include "zcl_general.h"
#include "zcl_ha.h"
#include "zcl_ms.h"

/* TESTAPP_TODO: Include any of the header files below to access specific cluster data
#include "zcl_poll_control.h"
#include "zcl_electrical_measurement.h"
#include "zcl_diagnostic.h"
#include "zcl_meter_identification.h"
#include "zcl_appliance_identification.h"
#include "zcl_appliance_events_alerts.h"
#include "zcl_power_profile.h"
#include "zcl_appliance_control.h"
#include "zcl_appliance_statistics.h"
#include "zcl_hvac.h"
*/

#include "zcl_testapp.h"

/*********************************************************************
 * CONSTANTS
 */

#define TESTAPP_DEVICE_VERSION     1
#define TESTAPP_FLAGS              0

#define TESTAPP_HWVERSION          1
#define TESTAPP_ZCLVERSION         1

/*********************************************************************
 * TYPEDEFS
 */

/*********************************************************************
 * MACROS
 */

/*********************************************************************
 * GLOBAL VARIABLES
 */

// Global attributes
const uint16 zclTestApp_clusterRevision_all = 0x0001;

// Basic Cluster
const uint8 zclTestApp_HWRevision = TESTAPP_HWVERSION;
const uint8 zclTestApp_ZCLVersion = TESTAPP_ZCLVERSION;
const uint8 zclTestApp_ManufacturerName[] = { 6, 'D','I','Y','R','u','Z' };
const uint8 zclTestApp_ModelId[] = { 9, 'D','I','Y','R','u','Z','_','R','T' };
const uint8 zclTestApp_DateCode[] = { 8, '2','0','2','1','1','2','1','9' };
const uint8 zclTestApp_PowerSource = POWER_SOURCE_MAINS_1_PHASE;

uint8 zclTestApp_LocationDescription[17] = { 16, ' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ' };
uint8 zclTestApp_PhysicalEnvironment = 0;
uint8 zclTestApp_DeviceEnable = DEVICE_ENABLED;

// Identify Cluster
uint16 zclTestApp_IdentifyTime;

// ????????? ????
extern uint8 RELAY_STATE;

// ?????? ? ???????????
#define MAX_MEASURED_VALUE  10000  // 100.00C
#define MIN_MEASURED_VALUE  -10000  // -100.00C

extern int16 zclTestApp_MeasuredValue;
const int16 zclTestApp_MinMeasuredValue = MIN_MEASURED_VALUE;
const uint16 zclTestApp_MaxMeasuredValue = MAX_MEASURED_VALUE;

/* TESTAPP_TODO: declare attribute variables here. If its value can change,
 * initialize it in zclTestApp_ResetAttributesToDefaultValues. If its
 * value will not change, initialize it here.
 */

#if ZCL_DISCOVER
CONST zclCommandRec_t zclTestApp_Cmds[] =
{
  {
    ZCL_CLUSTER_ID_GEN_BASIC,
    COMMAND_BASIC_RESET_FACT_DEFAULT,
    CMD_DIR_SERVER_RECEIVED
  },

};

CONST uint8 zclCmdsArraySize = ( sizeof(zclTestApp_Cmds) / sizeof(zclTestApp_Cmds[0]) );
#endif // ZCL_DISCOVER

/*********************************************************************
 * ATTRIBUTE DEFINITIONS - Uses REAL cluster IDs
 */
CONST zclAttrRec_t zclTestApp_Attrs[] =
{
  // *** General Basic Cluster Attributes ***
  {
    ZCL_CLUSTER_ID_GEN_BASIC,             // Cluster IDs - defined in the foundation (ie. zcl.h)
    {  // Attribute record
      ATTRID_BASIC_HW_VERSION,            // Attribute ID - Found in Cluster Library header (ie. zcl_general.h)
      ZCL_DATATYPE_UINT8,                 // Data Type - found in zcl.h
      ACCESS_CONTROL_READ,                // Variable access control - found in zcl.h
      (void *)&zclTestApp_HWRevision  // Pointer to attribute variable
    }
  },
  {
    ZCL_CLUSTER_ID_GEN_BASIC,
    { // Attribute record
      ATTRID_BASIC_ZCL_VERSION,
      ZCL_DATATYPE_UINT8,
      ACCESS_CONTROL_READ,
      (void *)&zclTestApp_ZCLVersion
    }
  },
  {
    ZCL_CLUSTER_ID_GEN_BASIC,
    { // ?????? ??????????
      ATTRID_BASIC_APPL_VERSION,
      ZCL_DATATYPE_UINT8,
      ACCESS_CONTROL_READ,
      (void *)&zclTestApp_ZCLVersion
    }
  },
  {
    ZCL_CLUSTER_ID_GEN_BASIC,
    { // ?????? ?????
      ATTRID_BASIC_STACK_VERSION,
      ZCL_DATATYPE_UINT8,
      ACCESS_CONTROL_READ,
      (void *)&zclTestApp_ZCLVersion
    }
  },
  {
    ZCL_CLUSTER_ID_GEN_BASIC,
    { // ?????? ????????
      ATTRID_BASIC_SW_BUILD_ID,
      ZCL_DATATYPE_CHAR_STR,
      ACCESS_CONTROL_READ,
      (void *)zclTestApp_DateCode
    }
  },
  {
    ZCL_CLUSTER_ID_GEN_BASIC,
    { // Attribute record
      ATTRID_BASIC_MANUFACTURER_NAME,
      ZCL_DATATYPE_CHAR_STR,
      ACCESS_CONTROL_READ,
      (void *)zclTestApp_ManufacturerName
    }
  },
  {
    ZCL_CLUSTER_ID_GEN_BASIC,
    { // Attribute record
      ATTRID_BASIC_MODEL_ID,
      ZCL_DATATYPE_CHAR_STR,
      ACCESS_CONTROL_READ,
      (void *)zclTestApp_ModelId
    }
  },
  {
    ZCL_CLUSTER_ID_GEN_BASIC,
    { // Attribute record
      ATTRID_BASIC_DATE_CODE,
      ZCL_DATATYPE_CHAR_STR,
      ACCESS_CONTROL_READ,
      (void *)zclTestApp_DateCode
    }
  },
  {
    ZCL_CLUSTER_ID_GEN_BASIC,
    { // Attribute record
      ATTRID_BASIC_POWER_SOURCE,
      ZCL_DATATYPE_ENUM8,
      ACCESS_CONTROL_READ,
      (void *)&zclTestApp_PowerSource
    }
  },
  {
    ZCL_CLUSTER_ID_GEN_BASIC,
    { // Attribute record
      ATTRID_BASIC_LOCATION_DESC,
      ZCL_DATATYPE_CHAR_STR,
      (ACCESS_CONTROL_READ | ACCESS_CONTROL_WRITE),
      (void *)zclTestApp_LocationDescription
    }
  },
  {
    ZCL_CLUSTER_ID_GEN_BASIC,
    { // Attribute record
      ATTRID_BASIC_PHYSICAL_ENV,
      ZCL_DATATYPE_ENUM8,
      (ACCESS_CONTROL_READ | ACCESS_CONTROL_WRITE),
      (void *)&zclTestApp_PhysicalEnvironment
    }
  },
  {
    ZCL_CLUSTER_ID_GEN_BASIC,
    { // Attribute record
      ATTRID_BASIC_DEVICE_ENABLED,
      ZCL_DATATYPE_BOOLEAN,
      (ACCESS_CONTROL_READ | ACCESS_CONTROL_WRITE),
      (void *)&zclTestApp_DeviceEnable
    }
  },

#ifdef ZCL_IDENTIFY
  // *** Identify Cluster Attribute ***
  {
    ZCL_CLUSTER_ID_GEN_IDENTIFY,
    { // Attribute record
      ATTRID_IDENTIFY_TIME,
      ZCL_DATATYPE_UINT16,
      (ACCESS_CONTROL_READ | ACCESS_CONTROL_WRITE),
      (void *)&zclTestApp_IdentifyTime
    }
  },
#endif
  {
    ZCL_CLUSTER_ID_GEN_BASIC,
    {  // Attribute record
      ATTRID_CLUSTER_REVISION,
      ZCL_DATATYPE_UINT16,
      ACCESS_CONTROL_READ,
      (void *)&zclTestApp_clusterRevision_all
    }
  },
  {
    ZCL_CLUSTER_ID_GEN_IDENTIFY,
    {  // Attribute record
      ATTRID_CLUSTER_REVISION,
      ZCL_DATATYPE_UINT16,
      ACCESS_CONTROL_READ,
      (void *)&zclTestApp_clusterRevision_all
    }
  },
  // *** ???????? On/Off ???????? ***
  {
    ZCL_CLUSTER_ID_GEN_ON_OFF,
    { // ?????????
      ATTRID_ON_OFF,
      ZCL_DATATYPE_BOOLEAN,
      ACCESS_CONTROL_READ,
      (void *)&RELAY_STATE
    }
  },
  {
    ZCL_CLUSTER_ID_GEN_ON_OFF,
    {  // ?????? On/Off ????????
      ATTRID_CLUSTER_REVISION,
      ZCL_DATATYPE_UINT16,
      ACCESS_CONTROL_READ | ACCESS_CLIENT,
      (void *)&zclTestApp_clusterRevision_all
    }
  },
  // *** ???????? Temperature Measurement ???????? ***
  {
    ZCL_CLUSTER_ID_MS_TEMPERATURE_MEASUREMENT,
    { // ???????? ???????????
      ATTRID_MS_TEMPERATURE_MEASURED_VALUE,
      ZCL_DATATYPE_INT16,
      ACCESS_CONTROL_READ | ACCESS_REPORTABLE,
      (void *)&zclTestApp_MeasuredValue
    }
  },
  {
    ZCL_CLUSTER_ID_MS_TEMPERATURE_MEASUREMENT,
    { // ??????????? ???????? ???????????
      ATTRID_MS_TEMPERATURE_MIN_MEASURED_VALUE,
      ZCL_DATATYPE_INT16,
      ACCESS_CONTROL_READ,
      (void *)&zclTestApp_MinMeasuredValue
    }
  },
  {
    ZCL_CLUSTER_ID_MS_TEMPERATURE_MEASUREMENT,
    { // ???????????? ???????? ???????????
      ATTRID_MS_TEMPERATURE_MAX_MEASURED_VALUE,
      ZCL_DATATYPE_INT16,
      ACCESS_CONTROL_READ,
      (void *)&zclTestApp_MaxMeasuredValue
    }
  },
  {
    ZCL_CLUSTER_ID_MS_TEMPERATURE_MEASUREMENT,
    {  // ?????? ????????
      ATTRID_CLUSTER_REVISION,
      ZCL_DATATYPE_UINT16,
      ACCESS_CONTROL_READ,
      (void *)&zclTestApp_clusterRevision_all
    }
  },
};

uint8 CONST zclTestApp_NumAttributes = ( sizeof(zclTestApp_Attrs) / sizeof(zclTestApp_Attrs[0]) );

/*********************************************************************
 * SIMPLE DESCRIPTOR
 */
// This is the Cluster ID List and should be filled with Application
// specific cluster IDs.
const cId_t zclTestApp_InClusterList[] =
{
  ZCL_CLUSTER_ID_GEN_BASIC,
  ZCL_CLUSTER_ID_GEN_IDENTIFY,
  
  // TESTAPP_TODO: Add application specific Input Clusters Here.
  //       See zcl.h for Cluster ID definitions
  
};
#define ZCLTESTAPP_MAX_INCLUSTERS   (sizeof(zclTestApp_InClusterList) / sizeof(zclTestApp_InClusterList[0]))


const cId_t zclTestApp_OutClusterList[] =
{
  ZCL_CLUSTER_ID_GEN_BASIC,
  
  // TESTAPP_TODO: Add application specific Output Clusters Here.
  //       See zcl.h for Cluster ID definitions
};
#define ZCLTESTAPP_MAX_OUTCLUSTERS  (sizeof(zclTestApp_OutClusterList) / sizeof(zclTestApp_OutClusterList[0]))


SimpleDescriptionFormat_t zclTestApp_SimpleDesc =
{
  TESTAPP_ENDPOINT,                  //  int Endpoint;
  ZCL_HA_PROFILE_ID,                     //  uint16 AppProfId;
  // TESTAPP_TODO: Replace ZCL_HA_DEVICEID_ON_OFF_LIGHT with application specific device ID
  ZCL_HA_DEVICEID_ON_OFF_LIGHT,          //  uint16 AppDeviceId; 
  TESTAPP_DEVICE_VERSION,            //  int   AppDevVer:4;
  TESTAPP_FLAGS,                     //  int   AppFlags:4;
  ZCLTESTAPP_MAX_INCLUSTERS,         //  byte  AppNumInClusters;
  (cId_t *)zclTestApp_InClusterList, //  byte *pAppInClusterList;
  ZCLTESTAPP_MAX_OUTCLUSTERS,        //  byte  AppNumInClusters;
  (cId_t *)zclTestApp_OutClusterList //  byte *pAppInClusterList;
};

// Added to include ZLL Target functionality
#if defined ( BDB_TL_INITIATOR ) || defined ( BDB_TL_TARGET )
bdbTLDeviceInfo_t tlTestApp_DeviceInfo =
{
  TESTAPP_ENDPOINT,                  //uint8 endpoint;
  ZCL_HA_PROFILE_ID,                        //uint16 profileID;
  // TESTAPP_TODO: Replace ZCL_HA_DEVICEID_ON_OFF_LIGHT with application specific device ID
  ZCL_HA_DEVICEID_ON_OFF_LIGHT,          //uint16 deviceID;
  TESTAPP_DEVICE_VERSION,                    //uint8 version;
  TESTAPP_NUM_GRPS                   //uint8 grpIdCnt;
};
#endif

/*********************************************************************
 * GLOBAL FUNCTIONS
 */

/*********************************************************************
 * LOCAL FUNCTIONS
 */
  
void zclTestApp_ResetAttributesToDefaultValues(void)
{
  int i;
  
  zclTestApp_LocationDescription[0] = 16;
  for (i = 1; i <= 16; i++)
  {
    zclTestApp_LocationDescription[i] = ' ';
  }
  
  zclTestApp_PhysicalEnvironment = PHY_UNSPECIFIED_ENV;
  zclTestApp_DeviceEnable = DEVICE_ENABLED;
  
#ifdef ZCL_IDENTIFY
  zclTestApp_IdentifyTime = 0;
#endif
  
  /* TESTAPP_TODO: initialize cluster attribute variables. */
}

/****************************************************************************
****************************************************************************/


