#include "types.h"
#include "board.h"

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
