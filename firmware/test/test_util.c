#include "types.h"
#include "di_util.h"
#include "chip.h"
#include "unity.h"
#include "unity_fixture.h"

STATE state;
HEARTBEAT_DATA hb_data;
WV_STATUS wv1_status;
WV_STATUS wv2_status;
BMS_PACK_STATUS bms_pack_status;
BMS_PRECHARGE_STATUS bms_precharge_status;
THROTTLE_STATUS throttle_status;
PDM_STATUS pdm_status;
RECIEVED_HEARTBEATS started_heartbeats;
UI_STATUS ui_status;

TEST_GROUP(Util_Test);

TEST_SETUP(Util_Test) {
    hb_data.started_heartbeats = &started_heartbeats;
    hb_data.wv1_status = &wv1_status;
    hb_data.wv2_status = &wv2_status;
    hb_data.ui_status = &ui_status;
    hb_data.bms_pack_status = &bms_pack_status;
    hb_data.bms_precharge_status = &bms_precharge_status;
    hb_data.throttle_status = &throttle_status;
    hb_data.pdm_status = &pdm_status;

    state.heartbeat_data = &hb_data;
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

    TEST_ASSERT_EQUAL_INT(0, wv1_status.velocity_rpm);
    TEST_ASSERT_EQUAL_INT(0, wv2_status.velocity_rpm);
    TEST_ASSERT_FALSE(ui_status.rasp_pi_on);
    TEST_ASSERT_FALSE(bms_pack_status.cells_over_voltage);
    TEST_ASSERT_FALSE(bms_pack_status.cells_under_voltage);
    TEST_ASSERT_FALSE(bms_pack_status.cells_over_temperature);
    TEST_ASSERT_FALSE(bms_pack_status.measurement_untrusted);
    TEST_ASSERT_FALSE(bms_pack_status.cmu_comm_timeout);
    TEST_ASSERT_FALSE(bms_pack_status.vehicle_comm_timeout);
    TEST_ASSERT_FALSE(bms_pack_status.cmu_can_power_off);
    TEST_ASSERT_FALSE(bms_pack_status.bmu_setup_mode);
    TEST_ASSERT_EQUAL_INT(0, throttle_status.brake_value);
    TEST_ASSERT_EQUAL_INT(0, throttle_status.throttle_value);
    TEST_ASSERT_FALSE(pdm_status.low_voltage_status);
    TEST_ASSERT_FALSE(pdm_status.low_voltage_battery);
    TEST_ASSERT_FALSE(pdm_status.low_voltage_dcdc);
    TEST_ASSERT_FALSE(pdm_status.critical_systems_status);
    TEST_ASSERT_FALSE(pdm_status.critical_systems_battery);
    TEST_ASSERT_FALSE(pdm_status.critical_systems_dcdc);
    
    TEST_ASSERT_FALSE(bms_precharge_status.contactor_error[3]);
    TEST_ASSERT_FALSE(bms_precharge_status.contactor_output[3]);
    TEST_ASSERT_EQUAL_INT(0, bms_precharge_status.precharge_status);

    // TODO: Add asserts for module status structs contain False/0 (e.g. BMS_PRECHARGE_STATUS, WV_STATUS, etc.) in hb_data
    // TODO Add asserts for UI status!
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

TEST(Util_Test, test_initialize_state){
    STATE state;
    
    state.dsm_mode = MODE_DRIVE;
    state.direction = DRIVE_FORWARD;
    state.time_started_init_tests_ms = 3234315;
    state.time_started_close_contactors_request_ms = 43947183;
    state.time_started_PDM_tests_ms = 85947583;
    state.critical_systems_relay_on = true;
    state.low_voltage_relay_on = true;

    state.heartbeat_data->last_rcvd_bms_heartbeat1 = 8238;
    state.heartbeat_data->last_rcvd_bms_heartbeat2 = 76;
    state.heartbeat_data->last_rcvd_bms_heartbeat3 = 2;
    state.heartbeat_data->last_rcvd_wv1_heartbeat = 90;
    state.heartbeat_data->last_rcvd_wv2_heartbeat = 123;
    state.heartbeat_data->last_rcvd_throttle_heartbeat = 0;
    state.heartbeat_data->last_rcvd_ui_heartbeat = -394;
    state.heartbeat_data->last_rcvd_mi_heartbeat = -28;
    state.heartbeat_data->last_rcvd_pdm_heartbeat = -9;

    RECIEVED_HEARTBEATS *rcvd_hbs = state.heartbeat_data->started_heartbeats;
    rcvd_hbs->bms_heartbeat1 = true;
    rcvd_hbs->bms_heartbeat2 = false;
    rcvd_hbs->bms_heartbeat3 = true;
    rcvd_hbs->throttle_heartbeat = true;
    rcvd_hbs->wv1_heartbeat = true;
    rcvd_hbs->wv2_heartbeat = true;
    rcvd_hbs->pdm_heartbeat = false;
    rcvd_hbs->ui_heartbeat = true;
    rcvd_hbs->mi_heartbeat = false;

    state.heartbeat_data->wv1_status->velocity_rpm = 393;
    state.heartbeat_data->wv2_status->velocity_rpm = 8494;

    state.heartbeat_data->pdm_status->low_voltage_status = true;
    state.heartbeat_data->pdm_status->low_voltage_dcdc = false;
    state.heartbeat_data->pdm_status->low_voltage_battery = true;
    state.heartbeat_data->pdm_status->critical_systems_status = true;
    state.heartbeat_data->pdm_status->critical_systems_battery = true;
    state.heartbeat_data->pdm_status->critical_systems_dcdc = true;

    state.heartbeat_data->throttle_status->brake_value= 93;
    state.heartbeat_data->throttle_status->throttle_value = 3902;

    state.heartbeat_data->bms_precharge_status->contactor_error[0] = true;
    state.heartbeat_data->bms_precharge_status->contactor_output[0] = false;
    state.heartbeat_data->bms_precharge_status->contactor_error[1] = true;
    state.heartbeat_data->bms_precharge_status->contactor_output[1] = true;
    state.heartbeat_data->bms_precharge_status->contactor_error[2] = true;
    state.heartbeat_data->bms_precharge_status->contactor_output[2] = true;
    state.heartbeat_data->bms_precharge_status->precharge_status = 0;

    state.heartbeat_data->bms_pack_status->cells_over_voltage = false;
    state.heartbeat_data->bms_pack_status->cells_under_voltage = true;
    state.heartbeat_data->bms_pack_status->cells_over_temperature = true;
    state.heartbeat_data->bms_pack_status->measurement_untrusted = false;
    state.heartbeat_data->bms_pack_status->cmu_comm_timeout = true;
    state.heartbeat_data->bms_pack_status->vehicle_comm_timeout = false;
    state.heartbeat_data->bms_pack_status->cmu_can_power_off = true;
    state.heartbeat_data->bms_pack_status->bmu_setup_mode = true;

    state.heartbeat_data->ui_status->rasp_pi_on = true;

    initialize_state(&state);

    TEST_ASSERT_EQUAL_INT(MODE_OFF, state.dsm_mode);
    TEST_ASSERT_EQUAL_INT(DRIVE_NEUTRAL, state.direction);
    TEST_ASSERT_EQUAL_INT(0, state.time_started_init_tests_ms);
    TEST_ASSERT_EQUAL_INT(0, state.time_started_close_contactors_request_ms);
    TEST_ASSERT_EQUAL_INT(0, state.time_started_PDM_tests_ms); 
    TEST_ASSERT_FALSE(state.critical_systems_relay_on);
    TEST_ASSERT_FALSE(state.low_voltage_relay_on);

	initialize_heartbeat_data(state.heartbeat_data);

    TEST_ASSERT_FALSE(state.heartbeat_data->started_heartbeats->bms_heartbeat1);
    TEST_ASSERT_FALSE(state.heartbeat_data->started_heartbeats->bms_heartbeat2);
    TEST_ASSERT_FALSE(state.heartbeat_data->started_heartbeats->bms_heartbeat3);
    TEST_ASSERT_FALSE(state.heartbeat_data->started_heartbeats->throttle_heartbeat);
    TEST_ASSERT_FALSE(state.heartbeat_data->started_heartbeats->wv1_heartbeat);
    TEST_ASSERT_FALSE(state.heartbeat_data->started_heartbeats->wv2_heartbeat);
    TEST_ASSERT_FALSE(state.heartbeat_data->started_heartbeats->pdm_heartbeat);
    TEST_ASSERT_FALSE(state.heartbeat_data->started_heartbeats->ui_heartbeat);
    TEST_ASSERT_FALSE(state.heartbeat_data->started_heartbeats->mi_heartbeat);

	TEST_ASSERT_EQUAL_INT(0, state.heartbeat_data->last_rcvd_bms_heartbeat1);
	TEST_ASSERT_EQUAL_INT(0, state.heartbeat_data->last_rcvd_bms_heartbeat2);
	TEST_ASSERT_EQUAL_INT(0, state.heartbeat_data->last_rcvd_bms_heartbeat3);
	TEST_ASSERT_EQUAL_INT(0, state.heartbeat_data->last_rcvd_wv1_heartbeat);
	TEST_ASSERT_EQUAL_INT(0, state.heartbeat_data->last_rcvd_wv2_heartbeat);
	TEST_ASSERT_EQUAL_INT(0, state.heartbeat_data->last_rcvd_throttle_heartbeat);
	TEST_ASSERT_EQUAL_INT(0, state.heartbeat_data->last_rcvd_ui_heartbeat);
	TEST_ASSERT_EQUAL_INT(0, state.heartbeat_data->last_rcvd_mi_heartbeat);
	TEST_ASSERT_EQUAL_INT(0, state.heartbeat_data->last_rcvd_pdm_heartbeat);

    TEST_ASSERT_EQUAL_INT(0, wv1_status.velocity_rpm);
    TEST_ASSERT_EQUAL_INT(0, wv2_status.velocity_rpm);
    TEST_ASSERT_FALSE(ui_status.rasp_pi_on);
    TEST_ASSERT_FALSE(bms_pack_status.cells_over_voltage);
    TEST_ASSERT_FALSE(bms_pack_status.cells_under_voltage);
    TEST_ASSERT_FALSE(bms_pack_status.cells_over_temperature);
    TEST_ASSERT_FALSE(bms_pack_status.measurement_untrusted);
    TEST_ASSERT_FALSE(bms_pack_status.cmu_comm_timeout);
    TEST_ASSERT_FALSE(bms_pack_status.vehicle_comm_timeout);
    TEST_ASSERT_FALSE(bms_pack_status.cmu_can_power_off);
    TEST_ASSERT_FALSE(bms_pack_status.bmu_setup_mode);
    TEST_ASSERT_EQUAL_INT(0, throttle_status.brake_value);
    TEST_ASSERT_EQUAL_INT(0, throttle_status.throttle_value);
    TEST_ASSERT_FALSE(pdm_status.low_voltage_status);
    TEST_ASSERT_FALSE(pdm_status.low_voltage_battery);
    TEST_ASSERT_FALSE(pdm_status.low_voltage_dcdc);
    TEST_ASSERT_FALSE(pdm_status.critical_systems_status);
    TEST_ASSERT_FALSE(pdm_status.critical_systems_battery);
    TEST_ASSERT_FALSE(pdm_status.critical_systems_dcdc);
    
    TEST_ASSERT_FALSE(bms_precharge_status.contactor_error[3]);
    TEST_ASSERT_FALSE(bms_precharge_status.contactor_output[3]);
    TEST_ASSERT_EQUAL_INT(0, bms_precharge_status.precharge_status);


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
