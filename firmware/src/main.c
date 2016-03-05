/*
 * @file main.c
 * @author Skanda Koppula and Bryson Galapon
 * @date 28 January 2016
 * @brief main program file
 */

#include "board.h"
#include "di_util.h"
#include "types.h"
#include "main_helper.h"
#include "dsm.h"
#include "can_spec_constants.h"

#define CCAN_BAUD_RATE 500000
#define UART_BAUD_RATE 9600

#define BUF_SIZE 8

#define DI_HEARTBEAT_MSG_OBJ 31
#define ERROR_MSG_OBJ 30

extern volatile uint32_t msTicks;

static STATE *statep;
static INPUT *inputp;
static OUTPUT *outputp;

static uint64_t last_heartbeat_time = 0;

static CCAN_MSG_OBJ_T msg_obj;
static CCAN_MSG_OBJ_T _rx_buffer[BUF_SIZE];
static RINGBUFF_T CAN_rx_buffer;

static char str[100];						// Used for composing UART messages
static uint8_t uart_rx_buffer[BUF_SIZE]; 	// UART received message buffer

static bool volatile can_error_flag;
static uint32_t volatile can_error_info;

void CAN_rx(uint8_t msg_obj_num) {
	msg_obj.msgobj = msg_obj_num;
	LPC_CCAN_API->can_receive(&msg_obj);
	RingBuffer_Insert(&CAN_rx_buffer, &msg_obj);
}

void CAN_tx(uint8_t msg_obj_num) {}

void CAN_error(uint32_t error_info) {
	can_error_flag = true;
	can_error_info = error_info;
}

void chip_startup_routine(void) {
	// Initialize SysTick Timer to generate millisecond count
	if (Board_SysTick_Init()) {
		while(1); // Unrecoverable Error. Hang.
	}

	// Initialize GPIO and LED as output
	Board_LEDs_Init();
	Board_LED_On(LED0);

	// Initialize UART Communication
	Board_UART_Init(UART_BAUD_RATE);
	Board_UART_Println("Started up driver interface!");

	// Initialize CAN  and CAN Ring Buffer
	RingBuffer_Init(&CAN_rx_buffer, _rx_buffer, sizeof(CCAN_MSG_OBJ_T), BUF_SIZE);
	RingBuffer_Flush(&CAN_rx_buffer);

	Board_CAN_Init(CCAN_BAUD_RATE, CAN_rx, CAN_tx, CAN_error);
	can_error_flag = false;

    statep = DSM_Init();
}

void check_CAN_error(void) {
    if (can_error_flag) {
        Board_UART_Print("CAN Error. Info: ");
        can_error_flag = false;

        itoa(can_error_info, str, 2);
        Board_UART_Println(str);
    }
}

int main(void) {
    chip_startup_routine();
    configure_can_reads();
    set_state_machine_configs();


	while (1) {
        process_UART_commands(statep, uart_rx_buffer, BUF_SIZE);

        read_input_requests(inputp);
        MODE_REQUEST mode_request = get_mode_request(inputp);

        DI_ERROR step_error;
        step_error = DSM_Step(inputp, statep, outputp, mode_request, msTicks);
        handle_error(step_error);

        check_CAN_error();
	}

    return 0;
}
