/**
 * @file di_util.h
 * @author Skanda Koppula and Bryson Galapon
 * @date 28 January 2016
 * @brief Defines driver interface utility function
 */

#ifndef _INIT_H_
#define _INIT_H_

#include "stdint.h"
#include "stdbool.h"

/**
 * @details defines the threshold wait time for which we should
 * hear back from all the heartbeats, BMS precharge, and PDM checks
 */
typedef struct {
    uint32_t threshold_wait_time_heartbeats_ms;
    uint32_t threshold_wait_time_bms_ms;
    uint32_t threshold_wait_time_pdm_ms;
} Init_Config_T;

/**
 * @details sets any initialization state configuration constants
 * (e.g. the threshold wait times)
 *
 * @param init_config config struct that has the state machine 
 * config constants
 */
void Init_Config(Init_Config_T init_config);

/**
 * @details tears down any state that the initialization sub-machine used
 * (e.g. start wait times for tests)
 */
ERROR INIT_Cleanup(STATE *state);

/**
 * @details executes the magic of the initialization procedure
 * see the PDF in the opel_driver_interface repository /docs folder
 * for pseudocode outline
 */
ERROR Init_Step(INPUT *input, STATE *state, OUTPUT *output, MODE_REQUEST *mode_request, uint32_t msTicks);

#endif
