#include <stdio.h>


HEARTBEAT_DATA *ignore_heartbeat_data() { 
    HEARTBEAT_DATA *hb_data;
    hb_data->ignore_heartbeats = true;
    hb_data->time_since_BMS_heartbeat = 0;
    hb_data->time_since_throttle_heartbeat = 0;
    hb_data->time_since_PDM_heartbeat = 0;
    hb_data->time_since_velocity_heartbeat = 0;
    hb_data->velocity = 0;
    return hb_data;
} 

HEARTBEAT_DATA *process_input_message(INPUT_MESSAGES *input_messages, HEARTBEAT_DATA *heartbeat_data, uint32_t msTicks) {
    HEARTBEAT_DATA *hb_data;
    
    hb_data->time_since_BMS_heartbeat = (input_messages->BMS_heartbeat) ? msTicks : heartbeat_data->time_since_BMS_heartbeat;
    hb_data->time_since_PDM_heartbeat = (input_messages->PDM_heartbeat) ? msTicks : heartbeat_data->time_since_PDM_heartbeat;
    hb_data->time_since_throttle_heartbeat = (input_messages->throttle_heartbeat) ? msTicks : heartbeat_data->time_since_throttle_heartbeat;
    hb_data->time_since_velocity_heartbeat = (input_messages->velocity != NULL) ? msTicks : heartbeat_data->time_since_velocity_heartbeat;
    hb_data->velocity = (input_messages->velocity != NULL) ? input_messages->velocity : heartbeat_data->velocity;

    return hb_data;
}
