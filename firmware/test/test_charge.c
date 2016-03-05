#include "charge.h"
#include "unity.h"
#include "unity_fixture.h"
#include "types.h"
#include "di_util.h"

TEST_GROUP(Charge_Test);

INPUT input;
STATE state;
OUTPUT output;
MODE_REQUEST mode_request;
uint32_t msTicks;

HEARTBEAT_DATA hb_data;

RECIEVED_HEARTBEATS started_heartbeats;
WV_STATUS wv1_status;
WV_STATUS wv2_status;
UI_STATUS ui_status;
MI_STATUS mi_status;
BMS_PACK_STATUS bms_pack_status;
BMS_PRECHARGE_STATUS bms_precharge_status;
THROTTLE_STATUS throttle_status;
PDM_STATUS pdm_status;
BRUSA_STATUS brusa_status; 



TEST_SETUP(Charge_Test) {
    hb_data.started_heartbeats = &started_heartbeats;
    hb_data.wv1_status = &wv1_status;
    hb_data.wv2_status = &wv2_status;
    hb_data.ui_status = &ui_status;
    hb_data.mi_status = &mi_status;
    hb_data.bms_pack_status = &bms_pack_status;
    hb_data.bms_precharge_status = &bms_precharge_status;
    hb_data.throttle_status = &throttle_status;
    hb_data.pdm_status = &pdm_status;
    hb_data.brusa_status = &brusa_status;

    state.heartbeat_data = &hb_data;    
}

TEST_TEAR_DOWN(Charge_Test) {}

TEST(Charge_Test, test_charge_checks) {

}

TEST(Charge_Test, test_check_brusa) {
    
}

TEST(Charge_Test, test_Charge_Step) {

} 

TEST_GROUP_RUNNER(Charge_Test) {

}


















