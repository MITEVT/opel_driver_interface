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
} Util_Config_T;

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
 * @param msTicks time as per on board oscillator on chip
 * @param check_pdm whether or not to check the PDM's critical systems data (we shouldn't check the PDM critical systems data if the critical systems aren't turned on yet)
 * @return any errors detected in the hearbeats
 */
DI_ERROR no_heartbeat_error(STATE *state, uint32_t msTicks, bool check_pdm_cs);


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
 * @details Converts the (requested) state of input accessories 
 * to a request for the corresponding hardware accessories state
 *
 * @param acc_in struct of user-input accessory state requests
 * @param brakes_on bool representing whether brakes should be on (decided not by user, but Throttle Module CAN Message)
 * @param out_req struct with hardware state requests to be populated by function
 */
void convert_acc(ACCESSORIES_INPUT_STATE *acc_in, bool brakes_on, ACCESSORIES_OUTPUT_REQUEST *out_req);

/**
 * @details Creates a hardware accessories request that turns off all accessories
 *
 * @param out_req struct with output hardware requests to be populated by function
 */
void turn_all_acc_off(ACCESSORIES_OUTPUT_REQUEST *out_req);
#endif
