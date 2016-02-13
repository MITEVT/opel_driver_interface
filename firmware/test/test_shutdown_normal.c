#include "shutdown_normal.h"
#include "unity.h"
#include "unity_fixture.h"

TEST_GROUP(Shutdown_Normal_Test);

STATE state;
HEARTBEAT_DATA heartbeat_data;

TEST_SETUP(Shutdown_Normal_Test) {
    state.heartbeat_data = &heartbeat_data;
}

TEST_TEAR_DOWN(Shutdown_Normal_Test) {

}

TEST(Shutdown_Normal_Test, test_Shutdown_Normal_Cleanup) {
    state.dsm_mode = MODE_DRIVE;
    state.direction = DRIVE_FORWARD;
    
    state.time_started_init_tests_ms = 42;
    state.time_started_close_contactors_request_ms = 1337;
    state.time_started_PDM_tests_ms = 1729;

    state.time_started_module_shutdown_ms = 43770;
    state.time_started_velocity_zero_ms = 376006;
    state.time_started_open_contactors_ms = 37047734;

    state.critical_systems_relay_on = true;
    state.low_voltage_relay_on = false;
    
     


}


TEST_GROUP_RUNNER(Shutdown_Normal_Test) {
RUN_TEST_CASE(Shutdown_Normal_Test, test_Shutdown_Normal_Cleanup);
}



