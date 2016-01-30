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

/*
 * @details Returns a heartbeat state object with all recieved 
 * heartbeat status set to false; all other data set to 0 or false
 *
 * @param hb_data heartbeat data struct to be populated
 */
void initialize_heartbeat_data(HEARTBEAT_DATA *hb_data);

/*
 * @details Converts the (requested) state of input accessories 
 * to a request for the corresponding hardware accessories state
 *
 * @param acc_in struct of user-input accessory state requests
 * @param brakes_on bool representing whether brakes should be on (decided not by user, but Throttle Module CAN Message)
 * @param out_req struct with hardware state requests to be populated by function
 */
void convert_acc(ACCESSORIES_INPUT_STATE *acc_in, bool brakes_on, ACCESSORIES_OUTPUT_REQUEST *out_req);

/*
 * @details Creates a hardware accessories request that turns off all accessories
 *
 * @param out_req struct with output hardware requests to be populated by function
 */
void turn_all_acc_off(ACCESSORIES_OUTPUT_REQUEST *out_req);
#endif
