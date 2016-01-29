#include "di_util.h"
#include "unity.h"
#include "unity_fixture.h"

HEARTBEAT_DATA hb_data;
WV_STATUS wv1_status;
WV_STATUS wv2_status;
BMS_PACK_STATUS bms_pack_status;
BMS_PRECHARGE_STATUS bms_precharge_status;
THROTTLE_STATUS throttle_status;
PDM_STATUS pdm_status;
RECIEVED_HEARTBEATS started_heartbeats;

TEST_GROUP(Util_Test);

TEST_SETUP(Util_Test) {
    hb_data.started_heartbeats = &started_heartbeats;
    hb_data.wv1_status = &wv1_status;
    hb_data.wv2_status = &wv2_status;
    hb_data.bms_pack_status = &bms_pack_status;
    hb_data.bms_precharge_status = &bms_precharge_status;
    hb_data.throttle_status = &throttle_status;
    hb_data.pdm_status = &pdm_status;
}

TEST_TEAR_DOWN(Util_Test) {

}

TEST(Util_Test, test_initialize_heartbeat_data) {

	initialize_heartbeat_data(&hb_data);

    TEST_ASSERT_FALSE(hb_data.started_heartbeats->bms_heartbeat1);
    TEST_ASSERT_FALSE(hb_data.started_heartbeats->bms_heartbeat2);
    TEST_ASSERT_FALSE(hb_data.started_heartbeats->bms_heartbeat3);
    TEST_ASSERT_FALSE(hb_data.started_heartbeats->throttle_heartbeat);
    TEST_ASSERT_FALSE(hb_data.started_heartbeats->wv1_heartbeat);
    TEST_ASSERT_FALSE(hb_data.started_heartbeats->wv2_heartbeat);
    TEST_ASSERT_FALSE(hb_data.started_heartbeats->pdm_heartbeat);
    TEST_ASSERT_FALSE(hb_data.started_heartbeats->ui_heartbeat);
    TEST_ASSERT_FALSE(hb_data.started_heartbeats->mi_heartbeat);

	TEST_ASSERT_EQUAL_INT(0, hb_data.last_rcvd_bms_heartbeat1);
	TEST_ASSERT_EQUAL_INT(0, hb_data.last_rcvd_bms_heartbeat2);
	TEST_ASSERT_EQUAL_INT(0, hb_data.last_rcvd_bms_heartbeat3);
	TEST_ASSERT_EQUAL_INT(0, hb_data.last_rcvd_wv1_heartbeat);
	TEST_ASSERT_EQUAL_INT(0, hb_data.last_rcvd_wv2_heartbeat);
	TEST_ASSERT_EQUAL_INT(0, hb_data.last_rcvd_throttle_heartbeat);
	TEST_ASSERT_EQUAL_INT(0, hb_data.last_rcvd_ui_heartbeat);
	TEST_ASSERT_EQUAL_INT(0, hb_data.last_rcvd_mi_heartbeat);
	TEST_ASSERT_EQUAL_INT(0, hb_data.last_rcvd_pdm_heartbeat);

    // TODO: Add asserts for module status structs contain False/0 (e.g. BMS_PRECHARGE_STATUS, WV_STATUS, etc.) in hb_data
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

TEST(Util_Test, test_process_input_heartbeat_data) {
	TEST_ASSERT_TRUE(true);
    // TODO: Write test to make sure the structs containing heartbeat data from INPUT_MESSAGES are properly used to update STATE
    //  - Make sure to test cases when no input heartbeat present, heartbeat present but no int/bool status data updated, and heartbeat present and int/bool status data updated
}

TEST(Util_Test, test_turn_all_acc_off) {
	ACCESSORIES_OUTPUT_REQUEST out_req;

	turn_all_acc_off(&out_req);

	TEST_ASSERT_FALSE(out_req.brake_lights_on);
	TEST_ASSERT_FALSE(out_req.wipers_on);
	
	TEST_ASSERT_EQUAL_INT(HEADLIGHT_OFF, out_req.headlight_state);
	TEST_ASSERT_EQUAL_INT(BLINKER_OFF, out_req.turn_blinker);
}


TEST_GROUP_RUNNER(Util_Test) {
	RUN_TEST_CASE(Util_Test, test_initialize_heartbeat_data);
	RUN_TEST_CASE(Util_Test, test_convert_acc);
	RUN_TEST_CASE(Util_Test, test_process_input_heartbeat_data);
	RUN_TEST_CASE(Util_Test, test_turn_all_acc_off);
}
