#include "types.h"
#include "board.h"
#include "can_spec_constants.h"
#include "constants.h"

#define NUM_INP_CAN_IDs 11 // Number of CAN input message types

typedef enum {
    BMS_HB_idx = 0,
    BMS_PRECHARGE_idx = 1,
    BMS_STATUS_idx = 2,
    THROTTLE_idx = 3,
    WV1_idx = 4,
    WV2_idx = 5,
    PDM_idx = 6,
    UI_idx = 7,
    MI_idx = 8,
    BRUSA1_idx = 9,
    BRUSA2_idx = 10,
} INPUT_HEARTBEAT_INDICES;

static uint32_t INPUT_CAN_IDs[NUM_INP_CAN_IDs] = {
    BMS_HEARTBEAT__id, 
    PRECHARGE_STATUS__id,
    OVERALL_PACK_STATUS__id,
    TI_PACKET__id,
    WHEEL_VELOCITY_1__id,
    WHEEL_VELOCITY_2__id,
    PDM_PACKET__id,
    UI_PACKET__id,
    MI_PACKET__id,
    BRUSA_NLG5_STATUS__id,
    BRUSA_NLG5_ERRORS__id
};

void configure_CAN_listen(uint8_t msg_obj_num, uint32_t can_id, uint32_t mask) {
    CCAN_MSG_OBJ_T listen_msg_obj;
	listen_msg_obj.msgobj = msg_obj_num;
	listen_msg_obj.mode_id = can_id;
	listen_msg_obj.mask = mask;
	LPC_CCAN_API->config_rxmsgobj(&listen_msg_obj);
}

void configure_can_reads(void) {
    uint8_t i;
    for(i = 0; i < NUM_INP_CAN_IDs; i++) {
        configure_CAN_listen(i, INPUT_CAN_IDs[i], 0xFFFFFFFF);
    }

}

void set_state_machine_configs(void) {
    // TODO read configs from constant.h and initialize Util.h    
}

void broadcast_heartbeat_message(STATE *state) {
    // TODO
}

void handle_error(DI_ERROR error){
    // TODO When we detect a UI_HEARTBEAT error from Init step function, turn on LED
    // TODO IF WE GET AN ERROR FROM SHUTDOWN NORMAL STEP MAKE SURE TO GO TO SHUTDOWN ERROR
}

void read_input_requests(INPUT *inputp) {
    // TODO read inputs from hardware and convert that to input request struct
}


void process_UART_commands(STATE *state, uint8_t *uart_rx_buffer, uint32_t buf_size) {
    uint8_t count;
    char str[100];
    if ((count = Board_UART_Read(uart_rx_buffer, buf_size)) != 0) {
        Board_UART_SendBlocking(uart_rx_buffer, count); // Echo user input
        switch (uart_rx_buffer[0]) {
            // Get the stored current state
            case 's': {
                MODE current_dsm_state = state->dsm_mode;

                if(current_dsm_state == MODE_OFF) {
                    Board_UART_Println("Car is in OFF state.");
                } else if(current_dsm_state == MODE_ACCESSORIES) {
                    Board_UART_Println("Car is in AUX state.");
                } else if(current_dsm_state == MODE_CHARGE) {
                    Board_UART_Println("Car is in Charge state.");
                } else if(current_dsm_state == MODE_DRIVE) {
                    Board_UART_Println("Car is in Drive state.");
                } else if(current_dsm_state == MODE_INIT) {
                    Board_UART_Println("Car is in Testing state.");
                } else {
                    Board_UART_Println("Car is in Shutdown state.");
                }

                break;
            }
            case 'h': {
                Board_UART_Println("We have recieved the following heartbeats at time: ");
                HEARTBEAT_DATA *hb_data = state->heartbeat_data;
                if(hb_data->started_heartbeats->bms_heartbeat1) {
                    Board_UART_Print("\tBMS1: ");
                    itoa(hb_data->last_rcvd_bms_heartbeat1, str, 10);
                    Board_UART_Println(str);
                } else if(hb_data->started_heartbeats->bms_heartbeat2) {
                    Board_UART_Print("\tBMS2: ");
                    itoa(hb_data->last_rcvd_bms_heartbeat2, str, 10);
                    Board_UART_Println(str);
                } else if(hb_data->started_heartbeats->bms_heartbeat3) {
                    Board_UART_Print("\tBMS3: ");
                    itoa(hb_data->last_rcvd_bms_heartbeat3, str, 10);
                    Board_UART_Println(str);
                } else if(hb_data->started_heartbeats->throttle_heartbeat) {
                    Board_UART_Println("\tTHROTTLE: ");
                    itoa(hb_data->last_rcvd_throttle_heartbeat, str, 10);
                    Board_UART_Println(str);
                } else if(hb_data->started_heartbeats->wv1_heartbeat) {
                    Board_UART_Print("\tWV1: ");
                    itoa(hb_data->last_rcvd_wv1_heartbeat, str, 10);
                    Board_UART_Println(str);
                } else if(hb_data->started_heartbeats->wv2_heartbeat) {
                    Board_UART_Print("\tWV2: ");
                    itoa(hb_data->last_rcvd_wv2_heartbeat, str, 10);
                    Board_UART_Println(str);
                } else if(hb_data->started_heartbeats->mi_heartbeat) {
                    Board_UART_Print("\tMI: ");
                    itoa(hb_data->last_rcvd_mi_heartbeat, str, 10);
                    Board_UART_Println(str);
                } else if(hb_data->started_heartbeats->ui_heartbeat) {
                    Board_UART_Print("\tUI: ");
                    itoa(hb_data->last_rcvd_ui_heartbeat, str, 10);
                    Board_UART_Println(str);
                } else if(hb_data->started_heartbeats->pdm_heartbeat) {
                    Board_UART_Print("\tPDM: ");
                    itoa(hb_data->last_rcvd_pdm_heartbeat, str, 10);
                    Board_UART_Println(str);
                } else if(hb_data->started_heartbeats->brusa_heartbeat1) {
                    Board_UART_Print("\tBRUSA1: ");
                    itoa(hb_data->last_rcvd_brusa_heartbeat1, str, 10);
                    Board_UART_Println(str);
                } else if(hb_data->started_heartbeats->brusa_heartbeat2) {
                    Board_UART_Print("\tBRUSA2: ");
                    itoa(hb_data->last_rcvd_brusa_heartbeat2, str, 10);
                    Board_UART_Println(str);
                }
            }
         }
   }

}
