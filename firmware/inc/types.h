#ifndef _TYPES_H_
#define _TYPES_H_

/*
 * Enum of Error types
 */

typedef enum {
    ERROR_NONE = 0, //No Error

    ERROR_CAN_BUS = 1, //Can bus error
    ERROR_HEADLIGHTS = 2, //Headlights error
    ERROR_BRAKELIGHTS = 3, //Brakelights error

    // From which state did we reach the error state?
    ERROR_CHARGE = 4 //Charge Failure
    ERROR_DRIVE = 5 //Drive Failure
    ERROR_ACCESSORIES = 6 //Accessories Failure
    ERROR_SHUTDOWN_FAILURE = 7 //Shutdown Failure
    ERROR_INIT = 8, //Initialization Failure
} ERROR_T;

/*
 * Enum of Key Modes
 */
typedef enum {
    OFF = 0, //Off Mode
    ACCESSORIES = 1, //Standby Mode 
    CHARGE = 2, //Charge Mode
    DRIVE = 3 //Drive Mode
} KEYMODES_T;

/*
 * Enum of DSM Modes
 */
typedef enum {
    OFF = 0, //Off Mode
    ACCESSORIES = 1, //Standby Mode
    CHARGE = 2, //Charge Mode
    DRIVE = 3, //Drive Mode
    INIT = 4, //Initialization Mode
    SHUTDOWN = 5, //Normal Shutdown
    FAIL = 6 //Error Mode
} MODES_T;

typedef struct {
    MODES_T DSM_modes;
    uint64_t time_since_BMS_heartbeat;
    uint64_t time_since_throttle_heartbeat;
    uint64_t time_since_PDM_heartbeat;
    uint64_t time_since_velocity_heartbeat;
    uint16_t velocity;
} STATE_T;

/*
 * Enum of DSM Mode requests
 */

typedef enum {
    REQ_OFF = 0, //Request to OFF
    REQ_ACCESSORIES = 1, //Request to Accessories
    REQ_CHARGE = 2, //Request to Charge
    REQ_DRIVE = 3, //Request to Drive
    REQ_INIT = 4, //Request to Initialize
    REQ_SHUTDOWN = 5, //Request to Shutdown
    REQ_NONE = 6 //No request
} MODE_REQUEST_T;

typedef enum {
    HEADLIGHT_OFF = 0;
    HEADLIGHT_ON = 1;
    HIGHBEAM_ON = 2;
} HEADLIGHT_STATE_T;

typedef enum {
    BLINKER_OFF = 0;
    LEFT_BLINKER = 1;
    RdIGHT_BLINKER = 2;
} TURN_BLINKER_T;

typedef struct {
    bool wipers_on; //State of the wipers
    HEADLIGHT_STATE_T headlight_state; //State of the headlights
    TURN_BLINKER_T turn_blinker; //State of turn blinkers
    bool brake_lights_on; //State of brake lights
} ACCESSORIES_INPUT_STATE_T;

typedef struct {
    bool wipers_on; //State of da wipers
    HEADLIGHT_STATE_T headlight_state; //State of the headlights
    TURN_BLINKER_T turn_blinker; //State of turn blinkers
    bool brake_lights_on; //State of brake lights
} ACCESSORIES_OUTPUT_REQUEST_T;

typedef struct {
    bool BMS_heartbeat;
    bool kill_switch;
    bool PDM_heartbeat;
    bool throttle_heartbeat;
    uint16_t velocity;
    bool init_test;
} INPUT_MESSAGES;

typedef struct {
    bool req_to_test;
    bool req_to_send_heartbeat;
    bool req_to_shutdown;
} OUTPUT_MESSAGE;

typedef struct {
    ACCESSORIES_INPUT_STATE_T acc_input;
    KEYMODES_T keymodes;
    INPUT_MESSAGES input_message;
} INPUT_T;

typedef struct {
    ACCESSORIES_OUTPUT_REQUEST_T acc_output;
    bool close_contactors;
    OUTPUT_MESSAGE output_message;
} OUTPUT_T;

/*
 * @details Step the Driver State Machine. Call as often as possible to
 * keep the Driver State Machine up to date
 *
 */

ERROR_T DSM_Step(INPUT_T, STATE_T, OUTPUT_T);

/*
 * @details Obtain the current Driver State Machine Mode
 * @return the current DSM Mode
 */

STATE_T DSM_GetState(void);

#endif
