#include "types.h"
#include "di_util.h"
#include "init.h"

static uint32_t threshold_wait_time_heartbeats_ms;
static uint32_t threshold_wait_time_bms_ms;
static uint32_t threshold_wait_time_pdm_ms;

void Init_Config(Init_Config_T init_config) {
    // TODO update 
}

ERROR Init_Cleanup(STATE *state) {
    // TODO
}

ERROR Init_Step(INPUT *input, STATE *state, OUTPUT *output, MODE_REQUEST mode_request, uint32_t msTicks) {
    // If time_started_init_tests != 0
    //      If complete (util function to make all heartbeat data is G) and no errs
    //          Sending DI BMS key ignition to 'Start'
    //		If(time_started_close_contactors_request_ms != 0){
    //              If in INput we find BMS signaled that it closed contacts0;
    //                  Close the critical systems relay
    //			If time_started_PDM_tests_ms != 0
    //                      If PDM data is OK:
    //                          Send DI heartbeat to park/forward/reverse
    //                      elif msTicks - time_started_PDM_tests_ms:
    //                          return ERROR
    //                      else:
    //                          return ERROR_NONE  
    //  		else:
    //			    time_started_PDM_tests_ms = msTicks
    //              elif msTicks - time_started_Close_Contactors_request_ms > threshold
    //                  return ERROR fuck
    //              else:
    //                  return ERROR_NONE // waiting

    //		    }else{
    //			    time_started_close_contactors_ms = msTicks;
    //
    //          MAKE SURE TO CLEANUP
    //      elif all heartbeats present but theres errors:
    //          return ERROR
    //      elif msTicks - time_started_init_tests > threshold_wait_time
    //          RETURN ERROR (main will handle error)
    //      else:
    //          return ERROR_NONE -> keep in Init_mode
    // Else:
    //     Close low voltage relay
    //     Start sending DI heartbeat BMS key ignition to 'Run' | what should be mode declared | maybe have init mode in can spec??
    //     time_started_init_tests = msTicks
    //     
    //
   //  Call get_requested_mode, check what mode was requested
   //  If Curr State == Off and Req Mode == Drive:
   //       Close Power Relay
   //
   //  Elif Curr State == Init and Req Mode == Drive:
   //       (must have been interrupted, still waiting for heartbeat)
   //       (must have been interrupted, heartbeat time elapsed)
}
