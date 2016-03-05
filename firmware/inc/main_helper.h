/**
 * @file main_helper.h
 * @author Skanda Koppula and Bryson Galapon
 * @date 28 January 2016
 * @brief Defines helper functions specific to main
 */

#ifndef _MAIN_HELPER_H_
#define _MAIN_HELPER_H_

#include "chip.h"
#include "types.h"

void process_UART_commands(STATE *state, uint8_t *uart_rx_buffer, uint32_t buf_size);

#endif
