/**
 * @file di_util.h
 * @author Skanda Koppula and Bryson Galapon
 * @date 28 January 2016
 * @brief Defines driver interface utility function
 */

#ifndef _UTIL_H_
#define _UTIL_H_

#include "chip.h"
#include "types.h"

typedef struct {
    uint32_t bms_hb1_threshold_ms; 
    uint32_t bms_hb2_threshold_ms; 
    uint32_t bms_hb3_threshold_ms; 
    uint32_t throttle_hb_threshold_ms; 
    uint32_t wv1_hb_threshold_ms; 
    uint32_t wv2_hb_threshold_ms; 
    uint32_t pdm_hb_threshold_ms; 
    uint32_t ui_hb_threshold_ms; 
    uint32_t mi_hb_threshold_ms; 
    uint32_t velocity_diff_threshold;
    uint32_t velocity_max_rpm;
    uint32_t throttle_max;
    uint32_t brake_max;
} Util_Config_T;

/**
 * @details Checks that velocities are zero
 *
 * @param state current state of the submachine
 * @return whether or not both velocities are zero
 */
bool check_velocity_zero(STATE *state);

/**
 * @details Averages readings from both velocity sensors
 *
 * @param state current state of the submachine
 * @return integer representing aggregate velocity
 */
uint32_t aggregate_velocities(STATE *state);

/**
 * @details Checks that velocities are not too different from each other
 *
 * @param state current state of the submachine
 * @return error if velocities are different
 */
DI_ERROR check_velocity_diff(STATE *state);

/**
 * @details Checks to make sure precharge status is finished, error free, and contactors are closed
 *
 * @param state the state of state machine
 * @return any errors in precharge heartbeats
 */
DI_ERROR check_bms_precharge(STATE *state);

/**
 * @details Checks the content of the heartbeat messages to ensure that the parameter values are safe
 *
 * @param state state of the state machine
 * @param check_pdm whether or not to check the PDM's critical systems data (we shouldn't check the PDM critical systems data if the critical systems aren't turned on yet)
 * @return any errors detected in the hearbeats
 */
DI_ERROR no_heartbeat_errors(STATE *state, bool check_pdm_cs);


/**
 * @details Checks whether a heartbeat has been recieved from each of the modules (values held in state) and checks to see if any heartbeat has timed out. 
 *
 * @param state state of the state machine
 * @param msTicks time as per on board oscillator on chip
 * @return any errors detected in the hearbeats
 */
DI_ERROR all_hb_exist(STATE *state, uint32_t msTicks);

/**
 * @details Converts the input toggle requests to the correct MODE_REQUEST. 
 *
 * @param input the input struct 
 * @return the mode request corresponding to the input. Valid return values: REQ_OFF, REQ_CHARGE, REQ_ACCESSORIES, REQ_DRIVE
 */
MODE_REQUEST get_mode_request(INPUT *input);

/**
 *  @details updates the state and DI packet output to reflect the mode request
 *           VALID FOR MOST STATES (EXCEPT SHUTDOWN STATE)
 *
 *  @param mode_request the requested mode (usually based on toggle/key inputs)
 *  @param state of state machine
 *  @param output of state machine
 *  @return any errors trying to change mode
 */
DI_ERROR change_mode(INPUT *input, STATE *state, OUTPUT *output, MODE_REQUEST mode_request);

/**
 * @details Returns a heartbeat state object with all recieved 
 * heartbeat status set to false; all other data set to 0 or false
 *
 * @param hb_data heartbeat data struct to be populated
 */
void initialize_heartbeat_data(HEARTBEAT_DATA *hb_data);

/**
 * @details Checks that velocities (RPM) from both sensors are within [0,1700)
 *
 * @param velocity1_rpm velocity in RPM of wheel velocity sensor 1
 * @param velocity2_rpm velocity in RPM of wheel velocity sensor 2
 */
DI_ERROR check_velocity_in_range(uint32_t velocity1_rpm, uint32_t velocity2_rpm);

//TODO: Add Spec for initialize_state
void initialize_state(STATE *state);
#endif
