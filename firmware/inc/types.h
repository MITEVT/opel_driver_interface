#ifndef _TYPES_H_
#define _TYPES_H_

#include "stdint.h"
#include "stdbool.h"

typedef enum {
    DRIVE_FORWARD = 0,
    DRIVE_REVERSE = 1
} DRIVE_DIRECTION_T;


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
} MODE_T;

typedef enum {
	INIT_OFF = 0, //OFF Mode
	INIT_TESTING = 1, //Testing Mode
	INIT_DONE = 2, //Tests Done
	INIT_FAILED = 3
} TEST_MODES_T;

typedef struct {
    uint32_t time_since_BMS_heartbeat;
    uint32_t time_since_throttle_heartbeat;
    uint32_t time_since_PDM_heartbeat; //Power distribution module 
    uint32_t time_since_velocity_heartbeat;
    uint16_t velocity;
    bool ignore_heartbeats;
} HEARTBEAT_DATA;

typedef struct {
    DRIVE_DIRECTION_T direction;
    MODE_T dsm_mode;
    HEARTBEAT_DATA *heartbeat_data;
    TEST_MODES_T testing_mode;
    uint32_t time_since_test_start;
    int counter; //0 if we started tests, 1 if we have. Used in checkTimeout in init.c
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
    bool BMS_heartbeat; //Battery 
    bool PDM_heartbeat; //Power distribution heartbeat
    bool throttle_heartbeat;
    uint16_t velocity;
    bool init_test; //Did we pass the init tests or not?
} INPUT_MESSAGES;

typedef struct {
    ACCESSORIES_INPUT_STATE_T *acc_input;
    KEYMODES_T keymodes;
    DRIVE_DIRECTION_T direction; //Should only matter if keymode is drive
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

typedef enum {
    MESSAGE_PARKED_AUX = 0,
    MESSAGE_CHARGE = 1,
    MESSAGE_DRIVE_FORWARD = 2,
    MESSAGE_DRIVE_REVERSE = 3,
    MESSAGE_SHUTDOWN_IMPENDING = 4
} MESSAGE_DRIVE_MODE_T;

typedef enum {
    ERROR_NONE = 0,

    ERROR_ILLEGAL_STATE_REQUEST = 1,

    ERROR_LOST_BMS_HEARTBEAT = 2,
    ERROR_LOST_THROTTLE_HEARTBEAT = 3,
    ERROR_LOST_PDM_HEARTBEAT = 4,
    ERROR_LOST_VELOCITY_HEARTBEAT = 5,

    ERROR_VELOCITY_NOT_ZERO = 6,

    ERROR_TESTS_FAILED = 7
} ERROR_T;

typedef struct {
    bool test; // Request hardware to send test module message to run tests
    MESSAGE_DRIVE_MODE_T drive_mode; 
    ERROR_T error; 
    bool send_heartbeat; // Request hardware to send heartbeat with given data
} OUTPUT_MESSAGES_T;

typedef struct {
    ACCESSORIES_OUTPUT_REQUEST_T *acc_output;
    OUTPUT_MESSAGES_T *messages;
    bool close_contactors;
} OUTPUT_T;

#endif
