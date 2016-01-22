#include "di_util.h"
#include "unity.h"
#include "unity_fixture.h"

TEST_GROUP(Util_Test);

TEST_SETUP(Util_Test) {
  	
}

TEST_TEAR_DOWN(Util_Test) {

}

TEST(Util_Test, test_initialize_heartbeat_data) {

	HEARTBEAT_DATA hb_data;
	initialize_heartbeat_data(&hb_data);

	// Test if all 0
	TEST_ASSERT_EQUAL_INT(0, hb_data.time_since_bms_heartbeat);
	TEST_ASSERT_EQUAL_INT(0, hb_data.time_since_wv1_heartbeat);
	TEST_ASSERT_EQUAL_INT(0, hb_data.time_since_wv2_heartbeat);
	TEST_ASSERT_EQUAL_INT(0, hb_data.time_since_throttle_heartbeat);
	TEST_ASSERT_EQUAL_INT(0, hb_data.time_since_ui_heartbeat);
	TEST_ASSERT_EQUAL_INT(0, hb_data.time_since_mi_heartbeat);
	TEST_ASSERT_EQUAL_INT(0, hb_data.time_since_pdm_heartbeat);
}

TEST(Util_Test, test_process_input_message) {
	INPUT_MESSAGES input_messages;
	input_messages.bms_heartbeat = false;
	input_messages.throttle_heartbeat = true;
	input_messages.wv1_heartbeat = false;
	input_messages.wv2_heartbeat = false;
	input_messages.pdm_heartbeat = true;
	input_messages.ui_heartbeat = false;
	input_messages.mi_heartbeat = true;

	
	HEARTBEAT_DATA hb_data;
	hb_data.time_since_bms_heartbeat = 238;
	hb_data.time_since_throttle_heartbeat = 2443;
	hb_data.time_since_wv1_heartbeat = 40;
	hb_data.time_since_wv2_heartbeat = 1337;
	hb_data.time_since_pdm_heartbeat = 2312;
	hb_data.time_since_ui_heartbeat = 55378008;
	hb_data.time_since_mi_heartbeat = 2682232;

	uint32_t msTicks = 23433;

	process_input_message(&input_messages, &hb_data, msTicks);
	TEST_ASSERT_EQUAL_INT(238, hb_data.time_since_bms_heartbeat);
	TEST_ASSERT_EQUAL_INT(23433, hb_data.time_since_throttle_heartbeat);
	TEST_ASSERT_EQUAL_INT(40, hb_data.time_since_wv1_heartbeat);
	TEST_ASSERT_EQUAL_INT(1337, hb_data.time_since_wv2_heartbeat);
	TEST_ASSERT_EQUAL_INT(23433, hb_data.time_since_pdm_heartbeat);
	TEST_ASSERT_EQUAL_INT(55378008, hb_data.time_since_ui_heartbeat);
	TEST_ASSERT_EQUAL_INT(23433, hb_data.time_since_mi_heartbeat);
}

TEST(Util_Test, test_convert_acc) {
	ACCESSORIES_INPUT_STATE acc_in;
	acc_in.wipers_on = true;
	acc_in.headlight_switches = HEADLIGHT_ON;
	acc_in.turn_blinker_switches = LEFT_BLINKER;

	bool brakes_on = true;

	ACCESSORIES_OUTPUT_REQUEST out_req;

	convert_acc(&acc_in, brakes_on, &out_req);

	TEST_ASSERT_TRUE(out_req.brake_lights_on);
	TEST_ASSERT_TRUE(out_req.wipers_on);

	TEST_ASSERT_EQUAL_INT(HEADLIGHT_ON, out_req.headlight_state);
	TEST_ASSERT_EQUAL_INT(LEFT_BLINKER, out_req.turn_blinker);
}

TEST(Util_Test, test_turn_all_off) {
	ACCESSORIES_OUTPUT_REQUEST out_req;

	turn_all_off(&out_req);

	TEST_ASSERT_FALSE(out_req.brake_lights_on);
	TEST_ASSERT_FALSE(out_req.wipers_on);
	
	TEST_ASSERT_EQUAL_INT(HEADLIGHT_OFF, out_req.headlight_state);
	TEST_ASSERT_EQUAL_INT(BLINKER_OFF, out_req.turn_blinker);
}


TEST_GROUP_RUNNER(Util_Test) {
	RUN_TEST_CASE(Util_Test, test_initialize_heartbeat_data);
	RUN_TEST_CASE(Util_Test, test_process_input_message);
	RUN_TEST_CASE(Util_Test, test_convert_acc);
	RUN_TEST_CASE(Util_Test, test_turn_all_off);
}
