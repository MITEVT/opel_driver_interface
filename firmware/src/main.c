#include "chip.h"
#include "board.h"
#include "util.h"
#include <string.h>

/*****************************************************************************
 * Private types/enumerations/variables
 ****************************************************************************/

#define TEST_CCAN_BAUD_RATE 500000

#define LED_PORT 0
#define LED_PIN 7

#define BUFFER_SIZE 8
#define UART_RX_BUFFER_SIZE 8

// const uint32_t OscRateIn = 12000000;


volatile uint32_t msTicks;


CCAN_MSG_OBJ_T msg_obj;
STATIC RINGBUFF_T rx_buffer;
CCAN_MSG_OBJ_T _rx_buffer[8];
static bool can_error_flag;
static bool can_error_info;

static char str[100];
static char uart_rx_buf[UART_RX_BUFFER_SIZE];

/*****************************************************************************
 * Private functions
 ****************************************************************************/

/*	CAN receive callback */
/*	Function is executed by the Callback handler after
    a CAN message has been received */
void CAN_rx(uint8_t msg_obj_num) {
	// LED_On();
	/* Determine which CAN message has been received */
	msg_obj.msgobj = msg_obj_num;
	/* Now load up the msg_obj structure with the CAN message */
	LPC_CCAN_API->can_receive(&msg_obj);
	if (msg_obj_num == 1) {
		RingBuffer_Insert(&rx_buffer, &msg_obj);
	}
}

/*	CAN transmit callback */
/*	Function is executed by the Callback handler after
    a CAN message has been transmitted */
void CAN_tx(uint8_t msg_obj_num) {
	msg_obj_num = msg_obj_num;
}

/*	CAN error callback */
/*	Function is executed by the Callback handler after
    an error has occurred on the CAN bus */
void CAN_error(uint32_t error_info) {
	can_error_info = error_info;
	can_error_flag = true;
}

int main(void)
{

	SystemCoreClockUpdate();

	if (SysTick_Config (SystemCoreClock / 1000)) {
		//Error
		while(1);
	}

	//---------------
	//UART
	Chip_IOCON_PinMuxSet(LPC_IOCON, IOCON_PIO1_6, (IOCON_FUNC1 | IOCON_MODE_INACT));/* RXD */
	Chip_IOCON_PinMuxSet(LPC_IOCON, IOCON_PIO1_7, (IOCON_FUNC1 | IOCON_MODE_INACT));/* TXD */

	Chip_UART_Init(LPC_USART);
	Chip_UART_SetBaud(LPC_USART, 9600);
	Chip_UART_ConfigData(LPC_USART, (UART_LCR_WLEN8 | UART_LCR_SBS_1BIT | UART_LCR_PARITY_DIS));
	Chip_UART_SetupFIFOS(LPC_USART, (UART_FCR_FIFO_EN | UART_FCR_TRG_LEV2));
	Chip_UART_TXEnable(LPC_USART);
	//---------------

	Board_UART_Println("Started up");

    Board_LEDs_Init();
    Board_LED_On(LED0);

	//---------------
	//Ring Buffer

	RingBuffer_Init(&rx_buffer, _rx_buffer, sizeof(CCAN_MSG_OBJ_T), 8);
	RingBuffer_Flush(&rx_buffer);

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
	CAN_baudrate_calculate(TEST_CCAN_BAUD_RATE, CanApiClkInitTable);

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

	/* Configure message object 1 to only ID 0x600 */
	msg_obj.msgobj = 1;
	msg_obj.mode_id = 0x600;
	msg_obj.mask = 0x7FF;
	LPC_CCAN_API->config_rxmsgobj(&msg_obj);

	
	can_error_flag = false;
	can_error_info = 0;

	while (1) {
		if (!RingBuffer_IsEmpty(&rx_buffer)) {
			CCAN_MSG_OBJ_T temp_msg;
			RingBuffer_Pop(&rx_buffer, &temp_msg);
			Board_UART_Print("Received Message ID: 0x");
			itoa(temp_msg.mode_id, str, 16);
			Board_UART_Println(str);
		}	

		if (can_error_flag) {
			can_error_flag = false;
			Board_UART_Print("CAN Error: 0b");
			itoa(can_error_info, str, 2);
			Board_UART_Println(str);
		}

		uint8_t count;
		if ((count = Chip_UART_Read(LPC_USART, uart_rx_buf, UART_RX_BUFFER_SIZE)) != 0) {
			switch (uart_rx_buf[0]) {
				case 'a':
					Board_UART_Println("Sending CAN with ID: 0x600");
					msg_obj.msgobj = 2;
					msg_obj.dlc = 1;
					msg_obj.data[0] = 0xAA;
					LPC_CCAN_API->can_transmit(&msg_obj);
					break;
				default:
					Board_UART_Println("Invalid Command");
					break;
			}
		}
	}
}
