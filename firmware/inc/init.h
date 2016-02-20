/**
 * @file init.h
 * @author Skanda Koppula and Bryson Galapon
 * @date 28 January 2016
 * @brief Defines driver interface init state/procedure
 */

#ifndef _INIT_H_
#define _INIT_H_

#include "chip.h"
#include "types.h"
#include "di_util.h"
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
void Init_Config(Init_Config_T *init_config);


/**
 * @details tears down any state that the initialization sub-machine used
 * (e.g. start wait times for tests)
 */
void Init_Cleanup(STATE *state);

/**
 *  @details checks the content and existence of PDM heartbeat. If no error,
 *           changes mode to requested mode. Also closes CS relay on first
 *           pdm_test attempt
 * 
 *  @param mode_request the requested mode (usually based on toggle/key inputs)
 *  @param input of state machine
 *  @param state of state machine
 *  @param output of state machine
 *  @param msTicks current time as per chip.h
 *  @return any errors trying to change mode
 */

DI_ERROR check_pdm(INPUT *input, STATE *state, OUTPUT *output, MODE_REQUEST mode_request, uint32_t msTicks);

/**
 *  @details checks BMS status on precharge and closing the contactors. 
 *           if no errors with BMS status, checks PDM data through check_pdm func.
 * 
 *  @param mode_request the requested mode (usually based on toggle/key inputs)
 *  @param input of state machine
 *  @param state of state machine
 *  @param output of state machine
 *  @param msTicks current time as per chip.h
 *  @return any errors on precharge/pdm errors
 */

DI_ERROR check_precharge_and_pdm(INPUT *input, STATE *state, OUTPUT *output, MODE_REQUEST mode_request, uint32_t msTicks);


/**
 * @details executes the magic of the initialization procedure
 * see the PDF in the opel_driver_interface repository /docs folder
 * for pseudocode outline
 */

DI_ERROR Init_Step(INPUT *input, STATE *state, OUTPUT *output, MODE_REQUEST mode_request, uint32_t msTicks);



#endif
