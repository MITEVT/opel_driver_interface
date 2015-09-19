#include "chip.h"

// -------------------------------------------------------------
// Macro Definitions

#define CCAN_BAUD_RATE 500000 					// Desired CAN Baud Rate
#define UART_BAUD_RATE 9600 					// Desired UART Baud Rate

#define LED_PORT 0 								// Debug LED Port
#define LED_PIN 7								// Debug LED Pin
#define LED LED_PORT, LED_PIN					// Debug LED

#define BUFFER_SIZE 8

// -------------------------------------------------------------
// Static Variable Declaration

const uint32_t OscRateIn = 0; 					// No external oscillator

static volatile uint32_t msTicks; 				// Running count of milliseconds since start

static CCAN_MSG_OBJ_T msg_obj; 					// Message Object data structure for manipulating CAN messages
static RINGBUFF_T CAN_rx_buffer;				// Ring Buffer for storing received CAN messages
static CCAN_MSG_OBJ_T _rx_buffer[BUFFER_SIZE]; 	// Underlying array used in ring buffer

static char str[100];							// Used for composing UART messages
static uint8_t UART_rx_buffer[BUFFER_SIZE]; 	// UART received message buffer

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

/**
 * Fill the can_api_timing_cfg struct with the appropriate values for a given baud rate
 * 
 * @param baud_rate desired baudrate in hz
 * @param can_api_timing_cfg struct ot fill with data for CAN Driver
 */
void baudrateCalculate(uint32_t baud_rate, uint32_t *can_api_timing_cfg)
{
	uint32_t pClk, div, quanta, segs, seg1, seg2, clk_per_bit, can_sjw;
	Chip_Clock_EnablePeriphClock(SYSCTL_CLOCK_CAN);
	pClk = Chip_Clock_GetMainClockRate();

	clk_per_bit = pClk / baud_rate;

	for (div = 0; div <= 15; div++) {
		for (quanta = 1; quanta <= 32; quanta++) {
			for (segs = 3; segs <= 17; segs++) {
				if (clk_per_bit == (segs * quanta * (div + 1))) {
					segs -= 3;
					seg1 = segs / 2;
					seg2 = segs - seg1;
					can_sjw = seg1 > 3 ? 3 : seg1;
					can_api_timing_cfg[0] = div;
					can_api_timing_cfg[1] =
						((quanta - 1) & 0x3F) | (can_sjw & 0x03) << 6 | (seg1 & 0x0F) << 8 | (seg2 & 0x07) << 12;
					return;
				}
			}
		}
	}
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
	if (msg_obj_num == 1) {
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
 * an error has occured on the CAN bus
 * 
 * @param error_info Number describing CAN error
 */
void CAN_error(uint32_t error_info) {}

// -------------------------------------------------------------
// Interrupt Service Routines

/**
 * SysTick Timer Interrupt Handler. Counts milliseconds since start
 */
void SysTick_Handler(void) {
	msTicks++;
}

/**
 * CCAN Interrupt Handler. Calls the isr() API located in the CCAN ROM
 */
void CAN_IRQHandler(void) {
	LPC_CCAN_API->isr();
}

// -------------------------------------------------------------
// Main Program Loop

int main(void) {

	// Update the value of SystemCoreClock to the clock speed in hz
	SystemCoreClockUpdate();

	// Initialize SysTick Timer to fire interrupt at 1kHz
	if (SysTick_Config (SystemCoreClock / 1000)) {
		// Unrecoverable Error. Hang.
		while(1);
	}

	//---------------
	// Initialize GPIO and LED as output
	Chip_GPIO_Init(LPC_GPIO);
	Chip_GPIO_WriteDirBit(LPC_GPIO, LED, true);

	//---------------
	// Initialize UART Communication
	Chip_IOCON_PinMuxSet(LPC_IOCON, IOCON_PIO1_6, (IOCON_FUNC1 | IOCON_MODE_INACT));	// Rx pin
	Chip_IOCON_PinMuxSet(LPC_IOCON, IOCON_PIO1_7, (IOCON_FUNC1 | IOCON_MODE_INACT));	// Tx Pin

	Chip_UART_Init(LPC_USART);
	Chip_UART_SetBaud(LPC_USART, UART_BAUD_RATE);
	Chip_UART_ConfigData(LPC_USART, (UART_LCR_WLEN8 | UART_LCR_SBS_1BIT | UART_LCR_PARITY_DIS));
	Chip_UART_SetupFIFOS(LPC_USART, (UART_FCR_FIFO_EN | UART_FCR_TRG_LEV2));
	Chip_UART_TXEnable(LPC_USART);

	Chip_UART_SendBlocking(LPC_USART, "Started up\n\r", 12);

	//---------------
	// CAN Ring Buffer

	RingBuffer_Init(&CAN_rx_buffer, _rx_buffer, sizeof(CCAN_MSG_OBJ_T), BUFFER_SIZE);
	RingBuffer_Flush(&CAN_rx_buffer);

	//---------------

	uint32_t CanApiClkInitTable[2];
	/* Publish CAN Callback Functions */
	CCAN_CALLBACKS_T callbacks = {
		CAN_rx,
		CAN_tx,
		CAN_error,
		NULL,
		NULL,
		NULL,
		NULL,
		NULL,
	};
	baudrateCalculate(CCAN_BAUD_RATE, CanApiClkInitTable);

	LPC_CCAN_API->init_can(&CanApiClkInitTable[0], TRUE);
	/* Configure the CAN callback functions */
	LPC_CCAN_API->config_calb(&callbacks);
	/* Enable the CAN Interrupt */
	NVIC_EnableIRQ(CAN_IRQn);

	// typedef struct CCAN_MSG_OBJ {
	// 	uint32_t  mode_id;
	// 	uint32_t  mask;
	// 	uint8_t   data[8];
	// 	uint8_t   dlc;
	// 	uint8_t   msgobj;
	// } CCAN_MSG_OBJ_T;

	/* Configure message object 1 to receive all 11-bit messages */

	/* How do filters work?

		Incoming ID & Mask == Mode_ID for msgobj to accept message

		Incoming ID : 0xabc
		Mask: 		  0xF0F &
		            -----------
		              0xa0c

		mode_id == 0xa0c for msgobj to accept message

	*/
	msg_obj.msgobj = 1;
	msg_obj.mode_id = 0x000;
	msg_obj.mask = 0x000;
	LPC_CCAN_API->config_rxmsgobj(&msg_obj);

	// Turn LED On
	Chip_GPIO_SetPinState(LPC_GPIO, LED, true);

	while (1) {
		uint8_t count;
		if ((count = Chip_UART_Read(LPC_USART, UART_rx_buffer, BUFFER_SIZE)) != 0) {
			Chip_UART_SendBlocking(LPC_USART, UART_rx_buffer, count); // Echo user input
			switch (UART_rx_buffer[0]) {
				case 't': // Send a hello world
					Chip_UART_SendBlocking(LPC_USART, "\r\nHello World\r\n", 15); 
					break;
				case 's': // Transmit a message
					msg_obj.msgobj = 2;
					msg_obj.mode_id = 0x600;
					msg_obj.dlc = 1;
					msg_obj.data[0] = 0xFF;

					LPC_CCAN_API->can_transmit(&msg_obj);
					break;
			}
		}

		if (!RingBuffer_IsEmpty(&CAN_rx_buffer)) {
			CCAN_MSG_OBJ_T temp_msg;
			RingBuffer_Pop(&CAN_rx_buffer, &temp_msg);
			Chip_UART_SendBlocking(LPC_USART, "Received Message\r\n", 18);
		}	
	}
}
