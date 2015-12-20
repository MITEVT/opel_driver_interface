#include "board.h"

// -------------------------------------------------------------
// Macro Definitions

#define CCAN_BAUD_RATE 500000 					// Desired CAN Baud Rate
#define UART_BAUD_RATE 9600 					// Desired UART Baud Rate

#define BUF_SIZE 8

#define DI_HEARTBEAT_CAN_ID 0x505
#define DI_HEARTBEAT_MSG_OBJ 31

#define ERROR_CAN_ID 0x510
#define ERROR_MSG_OBJ 30

#define DI_HEARTBEAT_DELAY 100                  // 10Hz = once every 100ms
// -------------------------------------------------------------
// Static Variable Declaration

extern volatile uint32_t msTicks;

static STATE_T *statep;
static INPUT_T *inputp;
static OUTPUT_T *outputp;

// Listen to BMS, Velocity, Throttle, and PDM modules, respectively
const static uint8_t INPUT_CAN_IDs = {0x600, 0x703, 0x700, 0x705};
const static uint8_t SIZE_INPUT_CAN_IDs = 4;

static uint64_t last_heartbeat_time = 0;

// Message Object data structure for manipulating CAN messages
static CCAN_MSG_OBJ_T msg_obj; 

// Underlying array used in ring buffer
static CCAN_MSG_OBJ_T _rx_buffer[BUF_SIZE];
static RINGBUFF_T CAN_rx_buffer;  // Ring Buffer for storing received CAN messages

static char str[100];						// Used for composing UART messages
static uint8_t uart_rx_buffer[BUF_SIZE]; 	// UART received message buffer

static bool volatile can_error_flag;
static uint32_t volatile can_error_info;

// -------------------------------------------------------------
// Helper Functions

/**
 * Delay the processor for a given number of milliseconds
 * @param ms Number of milliseconds to delay
 */
void _delay(uint32_t ms) {
	uint32_t curTicks = msTicks;
	while ((msTicks - curTicks) < ms);
}

// -------------------------------------------------------------
// CAN Driver Callback Functions

/**	
 * CAN receive callback executed by the Callback handler after a CAN message has been received 
 * @param msg_obj_num the msg_obj number that received a message
 */
void CAN_rx(uint8_t msg_obj_num) {
	/* Determine which CAN message has been received */
	msg_obj.msgobj = msg_obj_num;
	/* Now load up the msg_obj structure with the CAN message */
	LPC_CCAN_API->can_receive(&msg_obj);
	if (is_value_in_arr(msg_obj.mode_id, &INPUT_CAN_IDs, SIZE_INPUT_CAN_IDs)) {
		RingBuffer_Insert(&CAN_rx_buffer, &msg_obj);
	}
}

/**
 * CAN transmit callback executed by the Callback handler after
 * a CAN message has been transmitted 
 * @param msg_obj_num the msg_obj number that transmitted a message
 */
void CAN_tx(uint8_t msg_obj_num) {}

/**
 * CAN error callback executed by the Callback handler after
 * an error has occurred on the CAN bus
 * 
 * @param error_info Number describing CAN error
 */
void CAN_error(uint32_t error_info) {
	can_error_flag = true;
	can_error_info = error_info;
}

/*
 * Helper function to add new CAN ID to listen to (up to 32)
 */
void configure_CAN_listen(uint8_t msg_obj_num, uint32_t can_id, uint32_t mask) {
    CCAN_MSG_OBJ_T listen_msg_obj; 
	listen_msg_obj.msgobj = msg_obj_num;
	listen_msg_obj.mode_id = can_id;
	listen_msg_obj.mask = mask;
    // TODO check if we can call config_rxmsobj with multiple msg_obj values
	LPC_CCAN_API->config_rxmsgobj(&msg_obj);
}

void startup_routine(void) {
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

void process_UART_commands(void) {
    uint8_t count;
    if ((count = Board_UART_Read(uart_rx_buffer, BUF_SIZE)) != 0) {
        Board_UART_SendBlocking(uart_rx_buffer, count); // Echo user input
        switch (uart_rx_buffer[0]) {
            case 's': 
                // TODO print current state
                break;
            case 'i':
                // TODO print current input accessory requests
                break;
            default:
                Board_UART_Println("\r\nUnknown command!");
        }
    }
}

void broadcast_heartbeat_message(uint8_t data[8]) {
    msg_obj.msgobj = DI_HEARTBEAT_MSG_OBJ;
    msg_obj.mode_id = DI_HEARTBEAT_CAN_ID;
    msg_obj.mask = 0xFFFFFFFF;
    msg_obj.dlc = 8;
    msg_obj.data = data;

    LPC_CCAN_API->can_transmit(&msg_obj);
    // TODO Print more information about broadcast message
    Board_UART_Println("\r\nSent DI heartbeat.");
}

void broadcast_error_message(uint8_t data[8]) {
    msg_obj.msgobj = ERROR_MSG_OBJ;
    msg_obj.mode_id = ERROR_CAN_ID;
    msg_obj.mask = 0xFFFFFFFF;
    msg_obj.dlc = 8;
    msg_obj.data = data;

    LPC_CCAN_API->can_transmit(&msg_obj);
    // TODO Print more information about error broadcast message
    Board_UART_Println("\r\nSent error message.");
}

void process_output_requests(OUTPUT_T *output) {
    // TODO Adjust pin outputs, and send heartbeat, if necessary by timer
    //      Makes call to broadcast_heartbeat_message
    
    // if (msTicks - last_heartbeat_time > DI_HEARTBEAT_DELAY) {
    //    last_heartbeat_time = msTicks;
    //    broadcast_heartbeat_message();
    // }
}

INPUT_T *read_input_requests(void) {
    // TODO Read off of ring buffer, pins and populate an input request struct
    
    //  while(!RingBuffer_IsEmpty(&CAN_rx_buffer)) {
    //      CCAN_MSG_OBJ_T temp_msg;
    //      RingBuffer_Pop(&rx_buffer, &temp_msg);
    //      ...
    //
    //  }
    return NULL;
}

bool handle_error(ERROR_T error) {
    // TODO Process error appropriately, changing state if needed
    //      outputs True/False if should subsequently process_output_requests()
    //      should be called. Makes call to broadcast_error_message(...)
    return NULL;
}

void check_CAN_error(void) {
    if (can_error_flag) {
        Board_UART_Print("CAN Error. Info: ");
        can_error_flag = false;

        itoa(can_error_info, str, 2);
        Board_UART_Println(str);
    }
}

// -------------------------------------------------------------
// Main Program Loop

int main(void) {
    startup_routine();

    // Listen to heartbeats/regular messages from other modules
    configure_CAN_listen(0, INPUT_CAN_IDs[0], 0xFFFFFFFF); // BMS
    configure_CAN_listen(1, INPUT_CAN_IDs[1], 0xFFFFFFFF); // Velocity
    configure_CAN_listen(2, INPUT_CAN_IDs[2], 0xFFFFFFFF); // Throttle
    configure_CAN_listen(3, INPUT_CAN_IDs[3], 0xFFFFFFFF); // PDM

	while (1) {
        process_UART_commands();

        inputp = read_input_requests();

        ERROR_T step_error;
        step_error = DSM_Step(inputp, statep, outputp);
        bool process_outputs = handle_error(inputp, outputp, step_error);

        if(process_outputs) {
            process_output_requests(outputp);
        }

        check_CAN_error();
	}
}
