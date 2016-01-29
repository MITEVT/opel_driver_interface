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
INPUT_MESSAGES msgs;

HEARTBEAT_DATA hb_data;

ACCESSORIES_OUTPUT_REQUEST acc_out;
OUTPUT_MESSAGES out_msgs;

RECEIVED_HEARTBEATS rcvd_hbs;
WV_STATUS wv1_stat;
WV_STATUS wv2_stat;
BMS_PACK_STATUS bms_pack_stat;
BMS_PRECHARGE_STATUS bms_precharge_stat;
THROTTLE_STATUS throttle_stat;
PDM_STATUS pdm_stat;

TEST_SETUP(Util_Test) {

	acc_inp.wipers_on = true;
	acc_inp.headlight_switches = HIGHBEAM_ON;
	acc_inp.turn_blinker_switches = RIGHT_BLINKER;

	input.keymodes = KEYMODE_OFF;
	
	msgs.received_heartbeats = &rcvd_hbs;
	msgs.wv1_status = &wv1_stat;
	msgs.wv2_status = &wv2_stat;
	msgs.bms_pack_status = &bms_pack_stat;
	msgs.bms_precharge_status = &bms_precharge_stat;
	msgs.throttle_status = &throttle_stat;
	msgs.pdm_status = &pdm_stat;

	input.dcl = REVERSE;

	state.dsm_mode = MODE_INIT;
	state.direction = DRIVE_FORWARD;
	
	hb_data.
	hb_data.

	state.time_started_init_tests_ms = 937294;
	state.time_started_close_contactors_request_ms = 7653745;
	state.time_started_PDM_test_ms = 864875;
	state.critical_systems_relay_on = false;
	state.low_voltage_relay_on = false;

	//TODO: Finish creating random default values

	input.acc_input = &acc_inp;
	input.messages = &msgs;

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
	threshold_wait_time_heartbeats_ms = 100;

	InitStep(&input, &state, &output, mode_request, msTicks);

	TEST_ASSERT_TRUE(state.low_voltage_relay_on);
	TEST_ASSERT_EQUAL_INT(2344563248, state.time_started_init_tests_ms);
	
//***Test to see if InitStep doesn't change time_started_init_test_ms or low_voltage_relay_on after multiple iterations***

	//TODO: set hb_data to values to which the DI interprets the values as "No Good"

	//Code Here

	for(int i = 0; i < 50; i++){
		msTicks++;
		InitStep(&input, &state, &output, mode_request, msTicks);
	}

	TEST_ASSERT_TRUE(state.low_voltage_relay_on);
	TEST_ASSERT_EQUAL_INT(2344563248, state.time_started_init_tests_ms);

//***Test to see if InitStep stays in init mode***
	for(int i = 0; i < 49; i++){
		msTicks++;
		InitStep(&input, &state, &output, mode_request, msTicks);
	}
	
	TEST_ASSERT_EQUAL_INT(MODE_INIT, state.dsm_mode);	

//***Test to see if InitStep times out on the init tests***
	msTicks++;
	//TODO: set first arg in following assert to the corresponding TIMEOUT ERROR for the init tests
	TEST_ASSERT_EQUAL_INT(ERROR foo, InitStep(&input, &state, &output, mode_request, msTicks));

	//TODO: set hb_data to values to which the DI interprets the values as "GOOD"
	
	//TODO: set BMS signal so that contactors are OPEN
	
//***Test to see if InitStep refreshes time_started_close_contactors_request_ms;

	msTicks = 7;
	threshold_wait_time_bms_ms = 100;

	InitStep(&input, &state, &output, mode_request, msTicks);  

	TEST_ASSERT_EQUAL_INT(7, state.time_started_close_contactors_request_ms);	

//***Test to see if InitStep times out on the bms request***
}


TEST_GROUP_RUNNER(Util_Test) {
	RUN_TEST_CASE(Util_Test, test_InitStep);
}
