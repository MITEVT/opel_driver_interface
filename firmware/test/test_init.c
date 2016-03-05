#include "init.h"
#include "unity.h"
#include "unity_fixture.h"
#include "types.h"
#include "di_util.h"

TEST_GROUP(Init_Test);

INPUT input;
STATE state;
OUTPUT output;
MODE_REQUEST mode_request;
uint32_t msTicks;

ACCESSORIES_INPUT_STATE acc_inp;
INPUT_MESSAGES in_msgs;

HEARTBEAT_DATA hb_data;
WV_STATUS wv1_status;
WV_STATUS wv2_status;
BMS_PACK_STATUS bms_pack_status;
BMS_PRECHARGE_STATUS bms_precharge_status;
THROTTLE_STATUS throttle_status;
PDM_STATUS pdm_status;
UI_STATUS ui_status;
MI_STATUS mi_status;
BRUSA_STATUS brusa_status;

DI_PACKET di_packet;

RECIEVED_HEARTBEATS rcvd_hbs;

ACCESSORIES_OUTPUT_REQUEST acc_out;
OUTPUT_MESSAGES out_msgs;

Init_Config_T init_config;

//Set up all of the pointers
TEST_SETUP(Init_Test) {
    in_msgs.recieved_heartbeats = &rcvd_hbs;
    in_msgs.wv1_status = &wv1_status;
    in_msgs.wv2_status = &wv2_status;
    in_msgs.bms_pack_status = &bms_pack_status;
    in_msgs.bms_precharge_status = &bms_precharge_status;
    in_msgs.throttle_status = &throttle_status;
    in_msgs.pdm_status = &pdm_status;
    in_msgs.ui_status = &ui_status;
    in_msgs.mi_status = &mi_status;
    in_msgs.brusa_status = &brusa_status;
    
    input.acc_input = &acc_inp;
    input.messages = &in_msgs;

    di_packet.ignition = DI_OFF;
    di_packet.mode = OUT_OFF;

    out_msgs.di_packet = &di_packet;
    
    state.heartbeat_data = &hb_data;
    
    output.acc_output = &acc_out;
    output.messages = &out_msgs;

	init_config.threshold_wait_time_heartbeats_ms = 100;
	init_config.threshold_wait_time_bms_ms = 100;
	init_config.threshold_wait_time_pdm_ms = 100;
    Init_Config(&init_config);

}

TEST_TEAR_DOWN(Init_Test) {}

TEST(Init_Test, test_Init_Step) {
    
    // Test to see if Init_Step refreshes time_started_init_tests_ms to msTicks & closes low_voltage_relay
    mode_request = MODE_DRIVE;
	state.time_started_init_tests_ms = 0;
	msTicks = 23445;
	Init_Step(&input, &state, &output, mode_request, msTicks);


	TEST_ASSERT_TRUE(output.low_voltage_relay_on);
	TEST_ASSERT_EQUAL_INT(23445, state.time_started_init_tests_ms);
	
    // Test to see if Init_Step doesn't change time_started_init_test_ms or low_voltage_relay_on after multiple iterations

	// TODO: set hb_data to values to which the DI interprets the values as "No Good"

    int i;
	for(i = 0; i < 50; i++){
		msTicks++;
		Init_Step(&input, &state, &output, mode_request, msTicks);
	}

	TEST_ASSERT_TRUE(output.low_voltage_relay_on);
	TEST_ASSERT_EQUAL_INT(23445, state.time_started_init_tests_ms);

    // Test to see if Init_Step stays in init mode***
    DI_ERROR out_error;
    for(i = 0; i < 49; i++){
		msTicks++;
		out_error = Init_Step(&input, &state, &output, mode_request, msTicks);
	}
	
	TEST_ASSERT_EQUAL_INT(MODE_INIT, state.dsm_mode);	
	TEST_ASSERT_EQUAL_INT(ERROR_NONE, out_error);

    // Test to see if Init_Step times out on the init tests
	msTicks++;
	msTicks++;
	// TODO: set first arg in following assert to the corresponding to other TIMEOUT/CONTENT ERRORs
	TEST_ASSERT_EQUAL_INT(ERROR_INIT_BMS_HEARTBEAT, Init_Step(&input, &state, &output, mode_request, msTicks));

	// TODO: set hb_data to values to which the DI interprets the values as "GOOD"
	
	// TODO: set BMS signal so that contactors are OPEN
	
	//TODO: set first arg in following assert to the corresponding TIMEOUT ERROR for the init tests
}

TEST_GROUP_RUNNER(Init_Test) {
    RUN_TEST_CASE(Init_Test, test_Init_Step);
}
