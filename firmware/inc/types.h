#ifndef _TYPES_H_
#define _TYPES_H_

/*
 * Enum of Error types
 */

typedef enum{
	ERROR_NONE = 0, ///<No Error
	ERROR_CAN_BUS = 1, ///<Can bus error
	ERROR_HEADLIGHTS = 2, ///<Headlights error
	ERROR_BRAKELIGHTS = 3, ///<Brakelights error
	ERROR_INIT = 4, ///Initialization Failure
	ERROR_INVALID_TOGGLE_COMBINATION = 5, ///<Toggle Combination Error
	ERROR_SHUTDOWN_FAILURE = 6 ///<Shutdown Failure
} ERROR_T;

/*
 * Enum of Key Modes
 */
typedef enum{
	OFF = 0, ///<Off Mode
	ACCESSORIES = 1, ///<Standby Mode 
	CHARGE = 2, ///Charge Mode
	DRIVE = 3 ///<Drive Mode
} KEYMODES_T;

/*
 * Enum of DSM Modes
 */
typedef enum{
	OFF = 0, ///<Off Mode
	ACCESSORIES = 1, ///<Standby Mode
	CHARGE = 2, ///<Charge Mode
	DRIVE = 3, ///<Drive Mode
	INIT = 4, ///<Initialization Mode
	SHUTDOWN = 5, ///<Normal Shutdown
	FAIL = 6 ///<Error Mode
} MODES_T;

/*
 * Enum of DSM Mode requests
 */

typedef enum{
	REQ_OFF = 0, ///<Request to OFF
	REQ_ACCESSORIES = 1, ///<Request to Standby
	REQ_CHARGE = 2, ///,Request to Charge
	REQ_DRIVE = 3, ///<Request to Drive
	REQ_INIT = 4, ///<Request to Initialize
	REQ_SHUTDOWN = 5, ///<Request to Shutdown
	REQ_NONE = 6 ///<No request
} MODE_REQUEST_T;


/*
 *struct containing the current state of the toggles
 */

typedef struct{
	bool wipers_on; ///<State of the wipers
	bool headlights_on; ///<State of the headlights
	bool highbeam_lights_on; ///<highbeam or lowbeam lights
	uint16_t window_position; ///<Placement of the window
	uint16_t vehicle_volume; ///<Volume of speakers
	bool left_blinker_on; ///<State of left blinker
	bool right_blinker_on; ///<State of right blinker
	bool brake_lights_on; ///<State of brake lights
	bool horn_on; ///<State of the horn
} TOGGLE_STATE_T;


/*
 * @details Step the Driver State Machine. Call as often as possible to
 * keep the Driver State Machine up to date
 *
 */

ERROR_T DSM_Step(MODE_REQUEST_T, TOGGLE_STATE_T, MODES_T, KEYMODES_T);

/*
 * @details Obtain the current Driver State Machine Mode
 * @return the current DSM Mode
 */

DRAIN_MODE_T DSM_GetMode(void);

#endif
