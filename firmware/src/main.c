#include "board.h"
#include "stdlib.h"
#include "can.h"

// -------------------------------------------------------------
// Macro Definitions

#define CCAN_BAUD_RATE 500000 					// Desired CAN Baud Rate

#define BUFFER_SIZE 8

// -------------------------------------------------------------
// Static Variable Declaration

extern volatile uint32_t msTicks;

static CCAN_MSG_OBJ_T rx_msg; 	                // Message Object data structure for manipulating CAN messages

static char str[100];							// Used for composing UART messages
static uint8_t uart_rx_buffer[BUFFER_SIZE]; 	// UART received message buffer

static uint32_t last_message;

static uint8_t DI_CTRL;
static uint32_t last_update;

static uint8_t motor_state;
static int16_t motor_current;
static int16_t motor_speed;
static uint16_t HV_Voltage;
static int16_t motor_torque;
static bool motor_shutdown_ok;

// -----------------------------------------
// Helper Functions

/**
 * Delay the processor for a given number of milliseconds
 * @param ms Number of milliseconds to delay
 */
void _delay(uint32_t ms) {
	uint32_t curTicks = msTicks;
	while ((msTicks - curTicks) < ms);
}

inline static void displayData(void){
	Board_UART_Print("[Motor current:");
	Board_UART_PrintNum(motor_current,10,false);
	Board_UART_Println("]");
	Board_UART_Print("[Motor Speed:");
	Board_UART_PrintNum(motor_speed,10,false);
	Board_UART_Println("]");
	Board_UART_Print("[High Voltage:");
	Board_UART_PrintNum(HV_Voltage,10,false);
	Board_UART_Println("]");
	Board_UART_Print("[Motor Torque:");
	Board_UART_PrintNum(motor_torque,10,false);
	Board_UART_Println("]");
	Board_UART_Print("[Motor Shutdown OK:");
	if(motor_shutdown_ok){
		Board_UART_Println("True]");
	}
	else{
		Board_UART_Println("False]");
	}
	Board_UART_Print("[Motor State:");
	switch(motor_state){
		case (0):
			Board_UART_Println("Powerup]");
			break;
		case (1):
			Board_UART_Println("Disabled]");
			break;
		case (3):
			Board_UART_Println("Enabled]");
			break;
		case (2):
			Board_UART_Println("Standby]");
			break;
		case (4):
			Board_UART_Println("Powerdown]");
			break;
		case (5):
			Board_UART_Println("Fault]");
			break;
		case (6):
			Board_UART_Println("Critical Fault]");
			break;
		case (7):
			Board_UART_Println("Off]");
			break;
		default:
			Board_UART_Println("Unknown]");
	}
}

static void Print_Buffer(uint8_t* buff, uint8_t buff_size) {
    Chip_UART_SendBlocking(LPC_USART, "0x", 2);
    uint8_t i;
    for(i = 0; i < buff_size; i++) {
        itoa(buff[i], str, 16);
        if(buff[i] < 16) {
            Chip_UART_SendBlocking(LPC_USART, "0", 1);
        }
        Chip_UART_SendBlocking(LPC_USART, str, 2);
    }
}


// -------------------------------------------------------------
// Main Program Loop

int main(void)
{
	DI_CTRL=0;


	//---------------
	// Initialize UART Communication
	Board_UART_Init(UART_BAUD);
	Board_UART_Println("Started up");

	//---------------
	// Initialize SysTick Timer to generate millisecond count
	if (Board_SysTick_Init()) {
		Board_UART_Println("Failed to Initialize SysTick. ");
		// Unrecoverable Error. Hang.
		while(1);
	}

	//---------------
	// Initialize GPIO and LED as output
	Board_LEDs_Init();
	LED_On(2,10);

	CAN_Init(CCAN_BAUD_RATE);

	last_message = msTicks;
	last_update = msTicks;
	
    uint8_t data[2] = {0x40, 0xAA};
	uint32_t ret;

	while (1) {
		if (last_message < msTicks) {
			last_message = msTicks + 2000;
			Board_UART_Println("Sending CAN with ID: 0x505");
            // TO FILL IN PROPERLY with 
            data[0] = DI_CTRL;
            CAN_Transmit(0x505, data, 2);
		}

		if (msTicks % 2000 == 0){
            ret = CAN_Receive(&rx_msg);
            if(ret == NO_RX_CAN_MESSAGE) {
                Board_UART_Println("No CAN message received...");
            } else if(ret == NO_CAN_ERROR) {
                Board_UART_Println("Recieved data ");
                Print_Buffer(rx_msg.data, rx_msg.dlc);
                Board_UART_Println(" from ");
                itoa(rx_msg.mode_id, str, 16);
                Board_UART_Println(str);
                Board_UART_Println("\r\n");
            } else {
                Board_UART_Println("CAN Error: ");
                itoa(ret, str, 2);
                Board_UART_Println(str);
                Board_UART_Println("\r\n");
            }
        }

		uint8_t count;
		if ((count = Board_UART_Read(uart_rx_buffer, BUFFER_SIZE)) != 0) {
			Board_UART_SendBlocking(uart_rx_buffer, count); // Echo user input
			switch (uart_rx_buffer[0]) {
				case '1':
					DI_CTRL = (DI_CTRL & ~KEY_IGNITION_BITS)|KEY_IGNITION_RUN;
					break;
				case '2':
					DI_CTRL = (DI_CTRL & ~KEY_IGNITION_BITS)|KEY_IGNITION_START;
					break;
				case '0':
					DI_CTRL = (DI_CTRL & ~KEY_IGNITION_BITS)|KEY_IGNITION_OFF;
					break;
				case 'p':
					DI_CTRL = (DI_CTRL & ~DRIVE_STATUS_BITS)|DRIVE_STATUS_PARKED;
					break;
				case 'f':
					DI_CTRL = (DI_CTRL & ~DRIVE_STATUS_BITS)|DRIVE_STATUS_FORWARD;
					break;
				case 'r':
					DI_CTRL = (DI_CTRL & ~DRIVE_STATUS_BITS)|DRIVE_STATUS_REVERSE;
					break;
				case 's':
					DI_CTRL = (DI_CTRL & ~DRIVE_STATUS_BITS)|DRIVE_STATUS_SHUTDOWN_IMPENDING;
					break;
				case 'i':
					DI_CTRL = (DI_CTRL & ~DRIVE_STATUS_BITS)|DRIVE_STATUS_INIT;
					break;
				case 'c':
					DI_CTRL = (DI_CTRL & ~DRIVE_STATUS_BITS)|DRIVE_STATUS_CHARGE;
					break;
				case 'o':
					DI_CTRL = (DI_CTRL & ~DRIVE_STATUS_BITS)|DRIVE_STATUS_OFF;
					break;
				default:
					Board_UART_Println("Invalid Command");
					break;
			}
		}
	}
}
