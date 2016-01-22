#ifndef _TYPES_H_
#define _TYPES_H_

#include "stdint.h"
#include "stdbool.h"

/*
 * Modules:
 *   - BMS
 *   - Throttle
 *   - Wheel Velocity (WV1 and WV2)
 *   - User Interface (UI)
 *   - Motor Interface (MI)
 *   - Power Distribution Monitor (PDM)
 */

/************************************************
 *         TYPES DEFINING STATE                 *
 ***********************************************/

typedef enum {
    DRIVE_FORWARD = 0,
    DRIVE_REVERSE = 1
} DRIVE_DIRECTION;

typedef enum {
    MODE_OFF = 1, 
    MODE_ACCESSORIES = 2, 
    MODE_CHARGE = 3, 
    MODE_DRIVE = 4, 
    MODE_INIT = 5, 
    MODE_SHUTDOWN = 6
} MODE;

typedef struct {
    
} BMS_STATUS;

typedef struct {
    uint16_t brake_value;
    uint16_t throttle_value;
} THROTTLE_STATUS;

typedef struct {
    bool low_voltage_status;
    bool low_voltage_battery;
    bool low_voltage_dcdc;
    bool critical_systems_status;
    bool critical_systems_battery;
    bool critical_systems_dcdc;
} PDM_STATUS;

typedef struct {
    uint32_t velocity;
} WV_STATUS;

typedef struct {
    uint32_t time_since_bms_heartbeat;
    uint32_t time_since_wv1_heartbeat;
    uint32_t time_since_wv2_heartbeat;
    uint32_t time_since_throttle_heartbeat;
    uint32_t time_since_ui_heartbeat;
    uint32_t time_since_mi_heartbeat;
    uint32_t time_since_pdm_heartbeat;
} HEARTBEAT_DATA;

typedef struct {
    MODE dsm_mode;
    DRIVE_DIRECTION direction;
    MODE dsm_mode;
    HEARTBEAT_DATA *heartbeat_data;
} STATE;


/************************************************
 *  PASSED INTO SUB-SMs BASED ON KEY STATE      *
 ***********************************************/

typedef enum {
    REQ_OFF = 0, //Request to OFF
    REQ_ACCESSORIES = 1, //Request to Accessories
    REQ_CHARGE = 2, //Request to Charge
    REQ_DRIVE = 3, //Request to Drive
    REQ_INIT = 4, //Request to Initialize
    REQ_SHUTDOWN = 5 //Request to Shutdown
} MODE_REQUEST;

/************************************************
 *              THE INPUT TYPES               *
 ***********************************************/

typedef enum {
    KEYMODE_OFF = 0, //Off Mode
    KEYMODE_ACCESSORIES = 1, //Standby Mode 
    KEYMODE_CHARGE = 2, //Charge Mode
    KEYMODE_DRIVE = 3 //Drive Mode
} KEYMODES;

typedef enum {
    DRIVE_FORWARD = 0,
    DRIVE_REVERSE = 1
} DRIVE_DIRECTION;

typedef enum {
    HEADLIGHT_OFF = 0,
    HEADLIGHT_ON = 1,
    HIGHBEAM_ON = 2
} HEADLIGHT_STATE;

typedef enum {
    BLINKER_OFF = 0,
    LEFT_BLINKER = 1,
    RIGHT_BLINKER = 2
} TURN_BLINKER;

typedef struct {
    bool wipers_on; //State of the wipers
    HEADLIGHT_STATE headlight_switches; //State of headlights
    TURN_BLINKER turn_blinker_switches; //State of turn blinkers
} ACCESSORIES_INPUT_STATE;

typedef struct {
    bool bms_heartbeat; 
    bool throttle_heartbeat;
    bool wv1_heartbeat;
    bool wv2_heartbeat;
    bool pdm_heartbeat; 
    bool ui_heartbeat; 
    bool mi_heartbeat; 
} INPUT_MESSAGES;

typedef struct {
    ACCESSORIES_INPUT_STATE *acc_input;
    KEYMODES keymodes;
    DRIVE_DIRECTION direction; //Should only matter if keymode is drive
    INPUT_MESSAGES *messages;
} INPUT;

/************************************************
 *              THE OUTPUT TYPES               *
 ***********************************************/

typedef struct {
    bool wipers_on; //State of da wipers
    HEADLIGHT_STATE headlight_state; //State of the headlights
    TURN_BLINKER turn_blinker; //State of turn blinkers
    bool brake_lights_on; //State of brake lights
} ACCESSORIES_OUTPUT_REQUEST;

typedef enum {
    ERROR_NONE = 0,

    ERROR_ILLEGAL_STATE_REQUEST = 1,

    ERROR_LOST_BMS_HEARTBEAT = 2,
    ERROR_LOST_THROTTLE_HEARTBEAT = 3,
    ERROR_LOST_PDM_HEARTBEAT = 4,
    ERROR_LOST_VELOCITY_HEARTBEAT = 5,

    ERROR_VELOCITY_NOT_ZERO = 6,

    ERROR_VELOCITIES_NOT_EQUAL = 6,

    ERROR_LVS_BATTERY_TEST_FAILED = 7
    ERROR_LVS_DC_TEST_FAILED = 7
    ERROR_CS_BATTERY_TEST_FAILED = 7
    ERROR_CS_DC_TEST_FAILED = 7
} ERROR;

typedef struct {
    bool test; // Request hardware to send test module message to run tests
    ERROR error; 
} OUTPUT_MESSAGES;

typedef struct {
    ACCESSORIES_OUTPUT_REQUEST *acc_output;
    OUTPUT_MESSAGES *messages;
    ACCESSORIES_OUTPUT_REQUEST *acc_output;
    OUTPUT_MESSAGES *messages;
    bool close_contactors;
} OUTPUT;

#endif
