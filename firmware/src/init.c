#include "types.h"
#include "di_util.h"
#include "init.h"
#include "chip.h"

static uint32_t threshold_wait_time_heartbeats_ms;
static uint32_t threshold_wait_time_bms_ms;
static uint32_t threshold_wait_time_pdm_ms;

void Init_Config(Init_Config_T init_config) {
    // TODO update 
}

DI_ERROR Init_Cleanup(STATE *state) {
    // TODO
}

DI_ERROR Init_Step(INPUT *input, STATE *state, OUTPUT *output, MODE_REQUEST mode_request, uint32_t msTicks) {
//    If time_started_init_tests != 0: // If we started tests at all
//        If complete (util function to make all heartbeat data is G) and no errs
//            Sending DI BMS key ignition to 'Start'
//            If(time_started_close_contactors_request_ms != 0): // We started contactors request
//                If in INput we find BMS signaled that it closed contacts0;
//                    Close the critical systems relay
//                    If time_started_PDM_tests_ms != 0: // We started PDM request
//                        If PDM data is OK:
//                            Send DI heartbeat to park/forward/reverse
//                            Change state to drive
//                            MAKE SURE TO CLEANUP
//                            return ERROR_NONE  
//                        elif msTicks - time_started_PDM_tests_ms:
//                            return ERROR
//                        else:
//                            return ERROR_NONE  
//                    else:
//                        time_started_PDM_tests_ms = msTicks
//                        return ERROR_NONE -> keep in Init_mode
//                elif msTicks - time_started_Close_Contactors_request_ms > threshold
//                    return BMS_INIT_TIMEOUT_ERROR
//                else:
//                    return ERROR_NONE // waiting
//
//            else: // We did not start contactors request
//                time_started_close_contactors_ms = msTicks;
//                return ERROR_NONE -> keep in Init_mode
//        elif all heartbeats present but theres errors:
//            If UI failure: turn on LED?
//            return SOME_HEARTBEAT_ERROR
//        elif msTicks - time_started_init_tests > threshold_wait_time
//            If UI failure: turn on LED?
//            RETURN HEARTBEAT_INIT_TIMEOUT_ERROR (main will handle error)
//        else:
//            return ERROR_NONE -> keep in Init_mode
//
//    Else: // We haven't started the tests
//        Close low voltage relay
//        Start sending DI heartbeat BMS key ignition to 'Run' | what should be mode declared | maybe have init mode in can spec??
//        time_started_init_tests = msTicks
//        return ERROR_NONE -> keep in Init_mode
}
