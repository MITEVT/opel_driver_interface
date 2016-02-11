#include "init.h"
#include "unity.h"
#include "unity_fixture.h"

TEST_GROUP(Util_Test);

INPUT input;
STATE state;
OUTPUT output;
MODE_REQUEST mode_request;
uint32_t msTicks;

ACCESSORIES_INPUT_STATE acc_inp;
INPUT_MESSAGES in_msgs;

RECIEVED_HEARTBEATS rcvd_hbs;
WV_STATUS wv1_stat;
WV_STATUS wv2_stat;
BMS_PACK_STATUS bms_pack_stat;
BMS_PRECHARGE_STATUS bms_precharge_stat;
THROTTLE_STATUS throttle_stat;
PDM_STATUS pdm_stat;

HEARTBEAT_DATA hb_data;

ACCESSORIES_OUTPUT_REQUEST acc_out;
OUTPUT_MESSAGES out_msgs;

//Set up all of the pointers
TEST_SETUP(Util_Test) {
	in_msgs.recieved_heartbeats = &rcvd_hbs;
	in_msgs.wv1_status = &wv1_stat;
	in_msgs.wv2_status = &wv2_stat;
	in_msgs.bms_pack_status = &bms_pack_stat;
	in_msgs.bms_precharge_status = &bms_precharge_stat;
	in_msgs.throttle_status = &throttle_stat;
	in_msgs.pdm_status = &pdm_stat;

	input.acc_input = &acc_inp;
	input.messages = &in_msgs;

	state.heartbeat_data = &hb_data;

	output.acc_output = &acc_out;
	output.messages = &out_msgs;
}

TEST_TEAR_DOWN(Util_Test) {

}

TEST(Util_Test, test_InitStep) {

//***Test to see if InitStep refreshes time_started_init_tests_ms to msTicks & closes low_voltage_relay***
	state.time_started_init_tests_ms = 0;
	msTicks = 234456248;
	uint32_t threshold_wait_time_heartbeats_ms = 100;

	InitStep(&input, &state, &output, mode_request, msTicks);

	TEST_ASSERT_TRUE(state.low_voltage_relay_on);
	TEST_ASSERT_EQUAL_INT(2344563248, state.time_started_init_tests_ms);
	
//***Test to see if InitStep doesn't change time_started_init_test_ms or low_voltage_relay_on after multiple iterations***

	//TODO: set hb_data to values to which the DI interprets the values as "No Good"

	//Code Here
    int i;
	for(i = 0; i < 50; i++){
		msTicks++;
		InitStep(&input, &state, &output, mode_request, msTicks);
	}

	TEST_ASSERT_TRUE(state.low_voltage_relay_on);
	TEST_ASSERT_EQUAL_INT(2344548, state.time_started_init_tests_ms);

//***Test to see if InitStep stays in init mode***
    for(i = 0; i < 49; i++){
		msTicks++;
		InitStep(&input, &state, &output, mode_request, msTicks);
	}
	
	TEST_ASSERT_EQUAL_INT(MODE_INIT, state.dsm_mode);	

//***Test to see if InitStep times out on the init tests***
	msTicks++;
	//TODO: set first arg in following assert to the corresponding TIMEOUT ERROR for the init tests
	TEST_ASSERT_EQUAL_INT(ERROR_NONE, InitStep(&input, &state, &output, mode_request, msTicks));

	//TODO: set hb_data to values to which the DI interprets the values as "GOOD"
	
	//TODO: set BMS signal so that contactors are OPEN
	
//***Test to see if InitStep refreshes time_started_close_contactors_request_ms;

	msTicks = 7;

	InitStep(&input, &state, &output, mode_request, msTicks);  

	TEST_ASSERT_EQUAL_INT(7, state.time_started_close_contactors_request_ms);	

//***Test to see if InitStep stays in init mode***

	uint32_t threshold_wait_time_bms_ms = 100;
	for(i = 0; i < 99; i++){
		msTicks++;
		InitStep(&input, &state, &output, mode_request, msTicks);
	}

    TEST_ASSERT_EQUAL_INT(MODE_INIT, state.dsm_mode);

//***Test to see if InitStep times out on the bms request***
    msTicks++;
	//TODO: set first arg in following assert to the corresponding TIMEOUT ERROR for the init tests
	TEST_ASSERT_EQUAL_INT(ERROR_NONE, InitStep(&input, &state, &output, mode_request, msTicks));



}


TEST_GROUP_RUNNER(Util_Test) {
	RUN_TEST_CASE(Util_Test, test_InitStep);
}
