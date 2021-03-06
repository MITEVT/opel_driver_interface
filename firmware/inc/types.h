/**
 * @file types.h
 * @author Skanda Koppula and Bryson Galapon
 * @date 28 January 2015
 * @brief Defines globally used types
 */

#ifndef _TYPES_H_
#define _TYPES_H_

#include "chip.h"

/**
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

typedef struct {
    bool bms_heartbeat1; 
    bool bms_heartbeat2; 
    bool bms_heartbeat3; 
    bool throttle_heartbeat;
    bool wv1_heartbeat;
    bool wv2_heartbeat;
    bool pdm_heartbeat; 
    bool ui_heartbeat; 
    bool mi_heartbeat; 
    bool brusa_heartbeat1; 
    bool brusa_heartbeat2; 
} RECIEVED_HEARTBEATS;

typedef enum {
    DRIVE_FORWARD = 0,
    DRIVE_REVERSE = 1,
    DRIVE_NEUTRAL = 2
} DRIVE_DIRECTION;

typedef enum {
    MODE_OFF = 1, 
    MODE_ACCESSORIES = 2, 
    MODE_CHARGE = 3, 
    MODE_DRIVE = 4, 
    MODE_INIT = 5, 
    MODE_SHUTDOWN = 6 //TODO: decided if shutdown mode is necessary
} MODE;

typedef struct {
    // Data in BMS Heartbeat 2
    bool contactor_error[3]; // True=Error Present
    bool contactor_output[3]; // True=Contactors Closed
    uint8_t precharge_status;  // 0=Error, 1=Idle, 2=Measure, 3=Precharge, 4=Run
} BMS_PRECHARGE_STATUS;

typedef struct {
    // Data in BMS Heartbeat 3
    bool cells_over_voltage; 
    bool cells_under_voltage; 
    bool cells_over_temperature; 
    bool measurement_untrusted;
    bool cmu_comm_timeout;
    bool vehicle_comm_timeout;
    bool cmu_can_power_off;
    // make sure to invert the boolean of what BMS sent
    // if true, there is an ERROR!

    bool bmu_setup_mode;
} BMS_PACK_STATUS;

typedef struct {
    // Values are from 0 to 920 
    uint16_t brake_value;
    uint16_t throttle_value;
} THROTTLE_STATUS;

typedef struct {
    // True means there is no problem!
    bool low_voltage_status;
    bool low_voltage_battery;
    bool low_voltage_dcdc;
    bool critical_systems_status;
    bool critical_systems_battery;
    bool critical_systems_dcdc;
} PDM_STATUS;

typedef struct {
    // Can be from 0 to 1700 rpm
    uint32_t velocity_rpm; 
} WV_STATUS;

typedef struct {
    bool rasp_pi_on;
} UI_STATUS;

typedef struct {
    bool shutdown_okay;
    uint32_t motor_controller_current_mA;
} MI_STATUS;

typedef struct {
    bool overall_error_reported;
    bool specific_error_reported;
    bool hardware_on; 
} BRUSA_STATUS;

typedef struct {
    RECIEVED_HEARTBEATS *started_heartbeats;

    // The main BMS heartbeat (if on or off)
    uint32_t last_rcvd_bms_heartbeat1;
    // The BMS heartbeats about Precharge Status/Contactors (0x6F7)
    uint32_t last_rcvd_bms_heartbeat2;
    // The BMS heartbeats about overall pack status/cell status (0x6FB)
    uint32_t last_rcvd_bms_heartbeat3;

    uint32_t last_rcvd_wv1_heartbeat;
    uint32_t last_rcvd_wv2_heartbeat;
    uint32_t last_rcvd_throttle_heartbeat;
    uint32_t last_rcvd_ui_heartbeat;
    uint32_t last_rcvd_mi_heartbeat;
    uint32_t last_rcvd_pdm_heartbeat;

    uint32_t last_rcvd_brusa_heartbeat1;
    uint32_t last_rcvd_brusa_heartbeat2;

    WV_STATUS *wv1_status;
    WV_STATUS *wv2_status;
    UI_STATUS *ui_status;
    MI_STATUS *mi_status;
    BMS_PACK_STATUS *bms_pack_status;
    BMS_PRECHARGE_STATUS *bms_precharge_status;
    THROTTLE_STATUS *throttle_status;
    PDM_STATUS *pdm_status;
    BRUSA_STATUS *brusa_status;
} HEARTBEAT_DATA;

typedef struct {
    MODE dsm_mode;
    DRIVE_DIRECTION direction;
    HEARTBEAT_DATA *heartbeat_data;

    uint32_t time_started_init_tests_ms;
    uint32_t time_started_close_contactors_request_ms;
    uint32_t time_started_PDM_tests_ms;

    uint32_t time_started_module_shutdown_ms;
    uint32_t time_started_velocity_zero_ms;
    uint32_t time_started_open_contactors_ms;

    bool critical_systems_relay_on;
    bool low_voltage_relay_on;
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
    REQ_SHUTDOWN = 5 //Request to Shutdown (TODO: decide if this is necessary)
} MODE_REQUEST;

/************************************************
 *              THE INPUT TYPES               *
 ***********************************************/

typedef enum {
    KEYMODE_OFF = 0, //Off Mode
    KEYMODE_CHARGE = 1, //Charge Mode
    KEYMODE_DRIVE = 2 //Drive Mode
} KEYMODES;

typedef enum {
    FORWARD = 0,
    REVERSE = 1,
    PARK = 3
} DRIVE_CONTROL_LEVER;

typedef struct {
    RECIEVED_HEARTBEATS *recieved_heartbeats;

    WV_STATUS *wv1_status;
    WV_STATUS *wv2_status;
    BMS_PACK_STATUS *bms_pack_status;
    UI_STATUS *ui_status;
    MI_STATUS *mi_status;
    BMS_PRECHARGE_STATUS *bms_precharge_status;
    THROTTLE_STATUS *throttle_status;
    PDM_STATUS *pdm_status;
    BRUSA_STATUS *brusa_status;
} INPUT_MESSAGES;

typedef struct {
    KEYMODES keymodes;
    INPUT_MESSAGES *messages;
    DRIVE_CONTROL_LEVER dcl;
} INPUT;

/************************************************
 *              THE OUTPUT TYPES               *
 ***********************************************/

typedef enum {
    ERROR_NONE = 0,

    ERROR_ILLEGAL_STATE_REQUEST = 1,

    ERROR_LOST_BMS_HEARTBEAT = 2,
    ERROR_LOST_THROTTLE_HEARTBEAT = 3,
    ERROR_LOST_PDM_HEARTBEAT = 4,
    ERROR_LOST_UI_HEARTBEAT = 5,
    ERROR_LOST_MI_HEARTBEAT = 6,
    ERROR_LOST_VELOCITY1_HEARTBEAT = 7,
    ERROR_LOST_VELOCITY2_HEARTBEAT = 8,

    ERROR_VELOCITY_NOT_ZERO = 9,

    ERROR_VELOCITIES_NOT_EQUAL = 10,

    ERROR_LVS_BATTERY_TEST_FAILED = 11,
    ERROR_LVS_DC_TEST_FAILED = 12,
    ERROR_CS_BATTERY_TEST_FAILED = 13,
    ERROR_CS_DC_TEST_FAILED = 14,

    ERROR_INIT_UI_HEARTBEAT = 15,
    ERROR_INIT_BMS_HEARTBEAT = 16,
    ERROR_INIT_MI_HEARTBEAT = 17,
    ERROR_INIT_THROTTLE_HEARTBEAT = 18,
    ERROR_INIT_VELOCITY1_HEARTBEAT = 19,
    ERROR_INIT_VELOCITY2_HEARTBEAT = 20,
    ERROR_INIT_PDM_HEARTBEAT = 21,

    ERROR_BMS_PRECHARGE_TIMEOUT = 22,
    ERROR_PDM_TEST_TIMEOUT = 23,

    ERROR_INCONSISTENT_MODE_REQUEST = 24,

    ERROR_CONTENT_UI_HEARTBEAT = 25,
    ERROR_CONTENT_BMS_OVER_VOLTAGE = 26, // see more bms errors below
    ERROR_CONTENT_MI_HEARTBEAT = 27,
    ERROR_CONTENT_THROTTLE_HEARTBEAT = 28,
    ERROR_CONTENT_VELOCITY1_HEARTBEAT = 29,
    ERROR_CONTENT_VELOCITY2_HEARTBEAT = 30,
    ERROR_CONTENT_PDM_HEARTBEAT = 31,

    ERROR_CONTENT_BMS_UNDER_VOLTAGE = 32,
    ERROR_CONTENT_BMS_OVER_TEMP = 33,
    ERROR_CONTENT_BMS_MEASUREMENT_UNTRUSTED = 34,
    ERROR_CONTENT_BMS_CMU_COMM_TIMEOUT = 35,
    ERROR_CONTENT_BMS_VEHICLE_COMM_TIMEOUT = 36,
    ERROR_CONTENT_BMS_CAN_POWER = 37,

    ERROR_BMS_PRECHARGE = 38,

    ERROR_UI_SHUTDOWN = 39,
    ERROR_MI_SHUTDOWN = 40,

    ERROR_OPEN_CONTACTOR = 41,
    
    ERROR_VELOCITY_OUT_OF_RANGE = 42,
    ERROR_THROTTLE_OUT_OF_RANGE = 43,
    ERROR_BRAKE_OUT_OF_RANGE = 44,

    ERROR_BRUSA_NOT_ON = 45,
    ERROR_BRUSA_OVERALL_ERROR = 46,
    ERROR_BRUSA_SPECIFIC_ERROR = 47,
    ERROR_INCONSISTENT_BRAKE_AND_MOTOR_CURRENT = 48
} DI_ERROR;

typedef enum {
    DI_RUN = 0, // Turn on
    DI_START = 1, // Start contactors
    DI_OFF = 2 // Start contactors
} DI_PACKET_IGNITION;

typedef enum {
    OUT_PARKED = 0,
    OUT_FORWARD = 1,
    OUT_REVERSE = 2,
    OUT_SHUTDOWN_IMPENDING = 3,
    OUT_INIT = 4,
    OUT_CHARGE = 5,
    OUT_OFF = 6
} DI_PACKET_MODE;

typedef struct {
    DI_PACKET_IGNITION ignition;
    DI_PACKET_MODE mode;
} DI_PACKET;

typedef struct {
    DI_ERROR error; 
    DI_PACKET *di_packet;
    bool shutdown_vel_zero_req;
} OUTPUT_MESSAGES;

typedef struct {
    OUTPUT_MESSAGES *messages;
    bool low_voltage_relay_on;
    bool critical_systems_relay_on;
} OUTPUT;

#endif
