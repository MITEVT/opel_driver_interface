#ifndef _TYPES_H_
#define _TYPES_H_

#include "stdint.h"
#include "stdbool.h"

/************************************************
 *         TYPES DEFINING STATE                 *
 ***********************************************/

typedef enum {
    MODE_OFF = 1, //Off Mode
    MODE_ACCESSORIES = 1, //Standby Mode
    MODE_CHARGE = 2, //Charge Mode
    MODE_DRIVE = 3, //Drive Mode
    MODE_INIT = 4, //Initialization Mode
    MODE_SHUTDOWN = 5, //Normal Shutdown
    MODE_FAIL = 6 //Error Mode
} MODES_T;

typedef struct {
    uint64_t time_since_BMS_heartbeat;
    uint64_t time_since_throttle_heartbeat;
    uint64_t time_since_PDM_heartbeat; //Power distribution module 
    uint64_t time_since_velocity_heartbeat;
    uint16_t velocity;
} HEARTBEAT_DATA;

typedef struct {
    MODES_T DSM_modes;
    HEARTBEAT_DATA *heartbeat_data;
} STATE_T;


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
} MODE_REQUEST_T;

/************************************************
 *              THE INPUT TYPES               *
 ***********************************************/

typedef enum {
    KEYMODE_OFF = 0, //Off Mode
    KEYMODE_ACCESSORIES = 1, //Standby Mode 
    KEYMODE_CHARGE = 2, //Charge Mode
    KEYMODE_DRIVE = 3 //Drive Mode
} KEYMODES_T;


typedef enum {
    HEADLIGHT_OFF = 0,
    HEADLIGHT_ON = 1,
    HIGHBEAM_ON = 2
} HEADLIGHT_STATE_T;

typedef enum {
    BLINKER_OFF = 0,
    LEFT_BLINKER = 1,
    RIGHT_BLINKER = 2
} TURN_BLINKER_T;

typedef struct {
    bool wipers_on; //State of the wipers
    HEADLIGHT_STATE_T headlight_switches; //State of headlights
    TURN_BLINKER_T turn_blinker_switches; //State of turn blinkers
    bool brake_lights_on; //State of brake lights
} ACCESSORIES_INPUT_STATE_T;

typedef struct {
    bool BMS_heartbeat;
    bool PDM_heartbeat;
    bool throttle_heartbeat;
    uint16_t velocity;
    bool init_test;
} INPUT_MESSAGES;

typedef struct {
    ACCESSORIES_INPUT_STATE_T *acc_input;
    KEYMODES_T *keymodes;
    INPUT_MESSAGES *messages;
} INPUT_T;


/************************************************
 *              THE OUTPUT TYPES               *
 ***********************************************/

typedef struct {
    bool wipers_on; //State of da wipers
    HEADLIGHT_STATE_T headlight_state; //State of the headlights
    TURN_BLINKER_T turn_blinker; //State of turn blinkers
    bool brake_lights_on; //State of brake lights
} ACCESSORIES_OUTPUT_REQUEST_T;

typedef struct {
    bool test; // Request hardware to send test module message to init tests
    bool send_heartbeat; // Request hardware to send heartbeat
    bool command_shutdown; // Command a shutdown
} OUTPUT_MESSAGES;

typedef struct {
    ACCESSORIES_OUTPUT_REQUEST_T *acc_output;
    OUTPUT_MESSAGES *messages;
    bool close_contactors;
} OUTPUT_T;

typedef enum {
    ERROR_NONE = 0, //No Error

    ERROR_CAN_BUS = 1, //Can bus error
    ERROR_HEADLIGHTS = 2, //Headlights error
    ERROR_BRAKELIGHTS = 3, //Brakelights error

    // From which state did we reach the error state?
    ERROR_CHARGE = 4, //Charge Failure
    ERROR_DRIVE = 5, //Drive Failure
    ERROR_ACCESSORIES = 6, //Accessories Failure
    ERROR_SHUTDOWN_FAILURE = 7, //Shutdown Failure
    ERROR_INIT = 8, //Initialization Failure

    ERROR_ILLEGAL_STATE_REQUEST = 9
} ERROR_T;

/************************************************
 *              STEP METHODS                   *
 ***********************************************/

ERROR_T DSM_Step(INPUT_T *input, OUTPUT_T *output, STATE_T *state);

ERROR_T AccStep(INPUT_T *input, OUTPUT_T *output, STATE_T *state, MODE_REQUEST_T mode_request);

STATE_T DSM_GetState(void);

#endif
