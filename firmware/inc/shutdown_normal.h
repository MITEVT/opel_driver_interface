/**
 * @file shutdown_normal.h
 * @author Skanda Koppula and Bryson Galapon
 * @date 28 January 2016
 * @brief Defines driver interface normal shutdown checks and sequence
 */

#ifndef _SHUTDOWN_NORMAL_H_
#define _SHUTDOWN_NORMAL_H_

#include "chip.h"

/**
 * @details defines the threshold wait time for which we should
 * hear back from modules, check off conditions, etc.
 */
typedef struct {
    uint32_t max_wait_modules_shutdown_ms;
    uint32_t max_wait_velocity_check_ms;
    uint32_t max_wait_contactors_open_ms;
} Shutdown_Normal_Config_T;

/**
 * @details sets any shutdown state configuration constants
 * (e.g. the threshold wait times)
 *
 * @param shutdown_config config struct that has the state machine 
 * config constants
 */
void Shutdown_Normal_Config(Shutdown_Normal_Config_T *shutdown_config);


/**
 * @details tears down any state that the shutdown sub-machine used
 * (e.g. start wait times for tests)
 */
void Shutdown_Normal_Cleanup(STATE *state);

/**
 * @details executes the magic of the shutdown procedure
 * see the PDF in the opel_driver_interface repository /docs folder
 * for pseudocode outline
 */
DI_ERROR Shutdown_Normal_Step(INPUT *input, STATE *state, OUTPUT *output, MODE_REQUEST mode_request, uint32_t msTicks);

#endif
