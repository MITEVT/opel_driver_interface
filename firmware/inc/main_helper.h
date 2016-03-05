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

void read_input_requests(INPUT *inputp);

void handle_error(DI_ERROR error);

void broadcast_heartbeat_message(STATE *state);

void initialize_state_machine_configs(void);

void set_state_machine_configs(void);

void configure_can_reads(void);

#endif
