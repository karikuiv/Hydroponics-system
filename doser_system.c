#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

#include "doser_system.h"

int8_t load_node(struct doser_t *doser, char *name, uint8_t a, uint8_t b, uint8_t c, uint8_t d) {
    struct node_t *node;
    struct node_t **tmp_ptr;
    
    if (doser == NULL) {
        printf("error: doser is missing\n");
        return -1;
    } else if (name == NULL) {
        printf("error: name is missing!\n");
        return -1;
    } else if (strlen(name) < 1) {
        printf("error: name is empty!\n");
        return -1;
    }
    
    /* TODO: maybe add ip addr validation here? although if it's from the wild it should be a legit ip */
    
    node = malloc(sizeof(struct node_t));
    if (node == NULL) {
        printf("error: malloc node\n");
        return -1;
    }
    
    node->name = malloc(sizeof(char) * (strlen(name) + 1));
    if (node->name == NULL) {
        printf("error: malloc node->name\n");
        return -1;
    }
    strcpy(node->name, name);
    node->name[strlen(name)] = '\0';
    
    node->ip_addr.a = a; node->ip_addr.b = b; node->ip_addr.c = c; node->ip_addr.d = d;
    node->id = doser->num_nodes;
    
    printf("node->name: %s\n", node->name);
    
    node->devices = NULL;
    node->sensors = NULL;
    
    tmp_ptr = realloc(doser->nodes, sizeof(struct node_t *) * (doser->num_nodes + 1));
    if (tmp_ptr == NULL) {
        printf("error: realloc tmp_ptr\n");
        return -1;
    }
    doser->nodes = tmp_ptr;
    doser->nodes[doser->num_nodes] = node;
    doser->num_nodes++;
    
    return doser->num_nodes;
}

int8_t add_reading_to_data_source(struct readings_t *readings, uint8_t type, void *result) {
    if (readings == NULL) {
        printf("error: readings missing\n");
        return -1;
    }

    if (readings->latest < readings->buffer_size) {
        readings->latest++;
    } else {
        readings->latest = 0;
    }

    if (type == DATA_TYPE_FLOAT) {
        memcpy(readings->data + (readings->latest * sizeof(float)), result, sizeof(float));
    } else if (type == DATA_TYPE_UINT8) {
        memcpy(readings->data + (readings->latest * sizeof(uint8_t)), result, sizeof(uint8_t));
    } else {
        printf("error: invalid data type: %02u\n", type);
        return -1;
    }
    
    if (readings->num_readings < readings->buffer_size) {
        readings->num_readings++;
    }
    
    return readings->latest;
}

int8_t load_readings(struct doser_t *doser) {
    FILE *fp = fopen("sample_data_60_points_all_sources.txt", "r");
    const char s[1] = ",";
    char *token;
    int16_t len = 0;
    char *buffer;
    size_t bufsize = 500;
    size_t characters;

    buffer = (char *)malloc(bufsize * sizeof(char));    
    
    float data_f[60];
    uint8_t data_u[60];
    uint8_t data_type = 0;
    uint32_t latest = 0;
    memset(buffer, '\0', 500);
    if (fp != NULL) {
        for (int8_t d = 0; d < doser->num_data_sources; d++) {
            len = getline(&buffer, &bufsize, fp);
            printf("Retrieved line of length %u:\n", len);

            token = strtok(buffer, s);
            printf("%s\n", token);
            token = strtok(NULL,s);
            for(int8_t i = 0; i < 60; i++) {
                data_type = doser->data_sources[d]->data_type;
                if (data_type == DATA_TYPE_FLOAT) {
                    data_f[i] = (float) atof(token);
                    latest = add_reading_to_data_source(doser->data_sources[d]->readings, data_type, &data_f[i]);
                    printf("%02u: read: %4.2f readings: %4.2f (%02u)\n", i, data_f[i],
                           *( (float *)doser->data_sources[d]->readings->data + latest), latest);
                } else if (data_type == DATA_TYPE_UINT8) {
                    data_u[i] = (int8_t) atoi(token);
                    latest = add_reading_to_data_source(doser->data_sources[d]->readings, data_type, &data_u[i]);
                    printf("%02u: read: %02u readings: %02u (%02u)\n", i, data_u[i],
                           *( (uint8_t *)doser->data_sources[d]->readings->data + latest), latest);
                }
                    token = strtok(NULL,s);
                }
                
                /* because the data exists already in buffers, reset latest & readings to 0 to start over */
                printf("%s num readings %02u latest %02u\n",
                       doser->data_sources[d]->parent->name,
                       doser->data_sources[d]->readings->num_readings,
                       doser->data_sources[d]->readings->latest);
                       
                doser->data_sources[d]->readings->latest = 0;
                doser->data_sources[d]->readings->num_readings = 0;
        }
        printf("File closed\n");
        fclose(fp);
    } else {
        printf("error: file\n");
        return -1;
    }
    
    return 1;
}

int8_t data_acquisition(struct doser_t *doser) {
    if (doser == NULL) {
        printf("error: doser is empty\n");
        return -1;
    }
    /* temporary test implementation simulating data acquisition */
    /* for now just increase each data source's latest & num_readings by 1 */
    for (int8_t d = 0; d < doser->num_data_sources; d++) {  
        doser->data_sources[d]->readings->latest++;
        doser->data_sources[d]->readings->num_readings++;   
        /*printf("%02u latest: %02u num readings: %02u\n",
               d,
               doser->data_sources[d]->readings->latest,
               doser->data_sources[d]->readings->num_readings);*/
    }
    
    return 1;
    
}
         
int8_t update_data_providers(struct doser_t *doser) {
/* calculate running averages from last 5 seconds for now */
/* TODO: store processed data point in provider->data_points */
/* TODO: fix bug in minute average readings*/
    struct data_provider_t *provider;
    float sum_f = 0;
    uint32_t sum_u = 0;
    uint32_t latest = 0;
    
    for (uint8_t i = 0; i < doser->num_data_providers; i++) {
        sum_f = 0;
        sum_u = 0;
        
        provider = doser->data_providers[i];
        provider->data_last_updated++;
        
        /*printf("provider %02u update rate %02u last updated %02u\n",
               i, provider->data_update_rate, provider->data_last_updated);*/
        
        if (provider->data_last_updated == provider->data_update_rate) {
            if (provider->data_sources[0]->readings->num_readings >= provider->data_input_time_range) {
                if (provider->data_sources[0]->data_type == DATA_TYPE_FLOAT) {
                
                    latest = provider->data_sources[0]->readings->latest;
                    for(uint8_t d = 0; d < provider->data_input_time_range; d++) {
                        sum_f += *( (float *)provider->data_sources[0]->readings->data + latest - d);                   
                    }
                    sum_f = sum_f / provider->data_input_time_range;
                    printf("%15s avg of %02u samples: %5.2f   \tHistory: ", 
                           provider->data_sources[0]->parent->name,
                           provider->data_input_time_range,
                           sum_f);
                    latest = provider->data_sources[0]->readings->latest;
                    for(uint8_t d = 0; d < provider->data_input_time_range; d++) {
                        printf("%5.2f", *( (float *)provider->data_sources[0]->readings->data + latest - d));                   
                        if (d < (provider->data_input_time_range - 1)) {
                            printf(", ");
                        }
                    }
                    printf("\n");
                } else if (provider->data_sources[0]->data_type == DATA_TYPE_UINT8) {
                    latest = provider->data_sources[0]->readings->latest;
                    for (uint8_t d = 0; d < provider->data_input_time_range; d++) {
                        sum_u += *( (uint8_t *)provider->data_sources[0]->readings->data + latest - d);
                    }
                    sum_f = (float) sum_u / (float) provider->data_input_time_range;
                    printf("%15s avg of %02u samples: %.2f   \tHistory: ",
                           provider->data_sources[0]->parent->name,
                           provider->data_input_time_range,
                           sum_f);
                    for (uint8_t d = 0; d < provider->data_input_time_range; d++) {
                        printf("   %u", *( (uint8_t *)provider->data_sources[0]->readings->data + latest - d));
                        if (d < (provider->data_input_time_range - 1)) {
                            printf(", ");
                        }
                    }
                    printf("\n");
                }
            } else {
                printf("Not enough samples. Have: %02u / %02u\n",
                       provider->data_sources[0]->readings->num_readings,
                       provider->data_input_time_range);
            }
            provider->data_last_updated = 0;
        }
    }
    
    printf("\n");
    
    return 1;
}
         
/* check if task is enabled, then check if task is active */
/* finally check task status if it needs working, turning on, etc... */
uint8_t process_scheduled_tasks(struct doser_t *doser) {
    struct schedule_item_t *task;

    for(int t = 0; t < doser->num_tasks; t++) {
        task = doser->tasks[t];
        if (task_is_enabled(doser, task) == 1) {
            if (task_is_active(doser, task) == 1) {
                task->schedule->run_time->ss++;
                
                switch (task->type) {
                    case TASK_PH_SETPOINT: {
                        /* ph_controller(task->action_ptr, doser); */
                        if (task->schedule->run_time->ss >= 0) {
                            ( (struct task_setpoint_t *)task->action_ptr)->status = TASK_STATUS_ON;
                        }
                        
                        if (task->schedule->run_time->ss > 10) {
                            ( (struct task_setpoint_t *)task->action_ptr)->status = TASK_STATUS_DOSING;
                        }                   
                        break;
                    }
                    case TASK_EC_SETPOINT: {
                        /* ec_controller(task->action_ptr, doser); */
                        break;
                    }
                    case TASK_DEVICE: {
                        /* handle device control & status checking etc... */
                        /* TODO: calculate on/off periods etc */
                        ( (struct task_device_t *)task->action_ptr)->status = TASK_STATUS_ON;
                        break;
                    }
                }
            } else {
                switch (task->type) {
                    case TASK_PH_SETPOINT: {
                        /* ph_controller(task->action_ptr, doser); */
                        ( (struct task_setpoint_t *)task->action_ptr)->status = TASK_STATUS_INACTIVE;
                        break;
                    }
                    case TASK_EC_SETPOINT: {
                        /* ec_controller(task->action_ptr, doser); */
                        break;
                    }
                    case TASK_DEVICE: {
                        /* set status to something else if runtime > 5 seconds */
                        ( (struct task_device_t *)task->action_ptr)->status = TASK_STATUS_INACTIVE;
                        break;
                    }
                }
            }
        } else {
                switch (task->type) {
                    case TASK_PH_SETPOINT: {
                        /* ph_controller(task->action_ptr, doser); */
                        ( (struct task_setpoint_t *)task->action_ptr)->status = TASK_STATUS_DISABLED;
                        break;
                    }
                    case TASK_EC_SETPOINT: {
                        /* ec_controller(task->action_ptr, doser); */
                        break;
                    }
                    case TASK_DEVICE: {
                        /* handle device control & status checking etc... */
                        ( (struct task_device_t *)task->action_ptr)->status = TASK_STATUS_DISABLED;
                        break;
                    }
                }
        }
    }
}

 int8_t device_control (struct device_t *device, int8_t command) {
    if (device == NULL) {
        printf("error: device is missing\n");
        return -1;
    }
    
    if (device->provider_id == 0) {
        /*operate_internal_device(device);*/
    } else {
    /*  if (msg_node(device->provider_id, command, device->id) < 0) { *
            printf("error! failed to send msg to node\n");
        }       */
    }

    /*switch_device(device, command);*/ /* set status on/off and any other applicable parameters */
    /* or wait until node gives reply? */
    
    printf("Switched %s\n", device->name);
    return 1;
}

/* uint8_t msg_node (struct node_t *provider, uint8_t cmd, uint8_t data) {
        char str[20];
        sprintf(str, ",,,%02u,%02u\n", (unsigned int) cmd, (unsigned int) data);
} */

int8_t update_system_time(struct doser_t *doser) {
    time_t t;
    struct tm tt;
    
    if (doser == NULL) {
        printf("error: doser is missing\n");
        return -1;
    }
    
    t = time(NULL); 
    tt = *localtime(&t);
    
    doser->timers->system_time->hh = tt.tm_hour;
    doser->timers->system_time->mm = tt.tm_min;
    doser->timers->system_time->ss = tt.tm_sec;

    doser->timers->system_datetime->tm_hour = tt.tm_hour;
    doser->timers->system_datetime->tm_min = tt.tm_min;
    doser->timers->system_datetime->tm_sec = tt.tm_sec;
    doser->timers->system_datetime->tm_year = tt.tm_year;
    doser->timers->system_datetime->tm_mday = tt.tm_mday;
    doser->timers->system_datetime->tm_mon = tt.tm_mon;

    return 1;
}
