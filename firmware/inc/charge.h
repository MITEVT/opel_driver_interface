/**
 * @file charge.h
 * @author Skanda Koppula and Bryson Galapon
 * @date 20 February 2016
 * @brief Defines driver interface charge state/procedure
 */

#ifndef _CHARGE_H_
#define _CHARGE_H_

#include "chip.h"
#include "types.h"

/**
 * @details defines any constants we may need in the charge SM
 */
typedef struct {} Charge_Config_T;

/**
 * @details sets any charge state configuration constants
 *
 * @param charge_config config struct that has the state machine 
 * config constants
 */
void Charge_Config(Charge_Config_T *charge_config);


/**
 * @details tears down any state that the charge sub-machine used
 */
void Charge_Cleanup(STATE *state);

/**
 *  @details checks the content and existence of BRUSA heartbeat.
 * 
 *  @param state of state machine
 */

DI_ERROR check_brusa(STATE *state);

/**
 * @details check that velocity is nonzero and checks the BRUSA heartbeat via
 *          @see check_brusa
 *
 * @param state of the state machine
 */

DI_ERROR charge_checks(STATE *state);


/**
 * @details executes the magic of the initialization procedure
 * see the PDF in the opel_driver_interface repository /docs folder
 * for pseudocode outline
 */
DI_ERROR Charge_Step(INPUT *input, STATE *state, OUTPUT *output, MODE_REQUEST mode_request, uint32_t msTicks);

#endif
