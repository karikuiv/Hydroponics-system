/* 
 * Goal: set up essential system internals, data structures and hierarchies
 *  so the code can be reused with minimal effort in the actual system
 *
 * Compiles on windows if references to wiringPi and delay() are removed
 * Compiles on linux with -lwiringPi
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <time.h>

#include <wiringPi.h> 

#include "test_environment.h"
#include "doser_system.h"
#include "environment.h"
#include "device.h"
#include "sensor.h"
#include "schedule.h"
#include "data_provider.h"
#include "controller.h"
#include "print_hierarchy.h"

int8_t load_nodes(struct doser_t *doser) {
    /* better to parse a "a.b.c.d" string with gettok? */
    if (load_node(doser, "Internal", 127, 0, 0, 1) == -1) {
        printf("failed to load node!\n");
        return -1;
    }
    
    if (load_node(doser, "Hub_1", 192, 168, 0, 3) == -1) {
        printf("failed to load node!\n");
        return -1;
    }
    if (load_node(doser, "Hub_2", 192, 168, 0, 5 ) == -1) {
        printf("failed to load node!\n");
        return -1;
    }
}

int8_t load_devices(struct doser_t *doser) {
    /* load devices from a csv file? */
    /* file format: provider,device_name,device_name,...\nprovider,device_name,... */
    /* device id = index, used to control_device(id); */
    int8_t ret = 0;
    
    ret = load_device(doser, NODE_HUB_1, "Light_0");
    if (ret < 1) {
        printf("error: load device()");
        return -1;
    } else {
        printf("Device #%u loaded as %s from node #%02u.\n", ret,
                                           doser->devices[(doser->num_devices - 1)]->name,
                                           doser->devices[(doser->num_devices - 1)]->provider_id);
    }
    
    ret = load_device(doser, NODE_HUB_1, "Light_backup_0");
    if (ret < 1) {
        printf("error: load device()");
        return -1;
    } else {
        printf("Device #%u loaded as %s from node #%02u.\n", doser->num_devices,
                                           doser->devices[(doser->num_devices - 1)]->name,
                                           doser->devices[(doser->num_devices - 1)]->provider_id);
    }

    ret = load_device(doser, NODE_HUB_1, "Exhaust_fan_0");
    if (ret < 1) {
        printf("error: load device()");
        return -1;
    } else {
        printf("Device #%u loaded as %s from node #%02u.\n", doser->num_devices,
                                           doser->devices[(doser->num_devices - 1)]->name,
                                           doser->devices[(doser->num_devices - 1)]->provider_id);
    }
    
    ret = load_device(doser, NODE_HUB_1, "Air_fan_0");
    if (ret < 1) {
        printf("error: load device()");
        return -1;
    } else {
        printf("Device #%u loaded as %s from node #%02u.\n", doser->num_devices,
                                           doser->devices[(doser->num_devices - 1)]->name,
                                           doser->devices[(doser->num_devices - 1)]->provider_id);
    }
    
    ret = load_device(doser, NODE_HUB_1, "Air_fan_1");
    if (ret < 1) {
        printf("error: load device()");
        return -1;
    } else {
        printf("Device #%u loaded as %s from node #%02u.\n", doser->num_devices,
                                           doser->devices[(doser->num_devices - 1)]->name,
                                           doser->devices[(doser->num_devices - 1)]->provider_id);
    }   

    ret = load_device(doser, NODE_HUB_1, "Nutrient_pump_0");
    if (ret < 1) {
        printf("error: load device()");
        return -1;
    } else {
        printf("Device #%u loaded as %s from node #%02u.\n", doser->num_devices,
                                           doser->devices[(doser->num_devices - 1)]->name,
                                           doser->devices[(doser->num_devices - 1)]->provider_id);
    }

    ret = load_device(doser, NODE_HUB_1, "Mixer_pump_0");
    if (ret < 1) {
        printf("error: load device()");
        return -1;
    } else {
        printf("Device #%u loaded as %s from node #%02u.\n", doser->num_devices,
                                           doser->devices[(doser->num_devices - 1)]->name,
                                           doser->devices[(doser->num_devices - 1)]->provider_id);
    }

    ret = load_device(doser, NODE_HUB_1, "Air_pump_0");
    if (ret < 1) {
        printf("error: load device()");
        return -1;
    } else {
        printf("Device #%u loaded as %s from node #%02u.\n", doser->num_devices,
                                           doser->devices[(doser->num_devices - 1)]->name,
                                           doser->devices[(doser->num_devices - 1)]->provider_id);
    }
    
    ret = load_device(doser, NODE_HUB_1, "Nutrient_pump_1");
    if (ret < 1) {
        printf("error: load device()");
        return -1;
    } else {
        printf("Device #%u loaded as %s from node #%02u.\n", doser->num_devices,
                                           doser->devices[(doser->num_devices - 1)]->name,
                                           doser->devices[(doser->num_devices - 1)]->provider_id);
    }
    
    ret = load_device(doser, NODE_HUB_1, "Mixer_pump_1");
    if (ret < 1) {
        printf("error: load device()");
        return -1;
    } else {
        printf("Device #%u loaded as %s from node #%02u.\n", doser->num_devices,
                                           doser->devices[(doser->num_devices - 1)]->name,
                                           doser->devices[(doser->num_devices - 1)]->provider_id);
    }
    
    ret = load_device(doser, NODE_HUB_1, "Air_pump_1");
    if (ret < 1) {
        printf("error: load device()");
        return -1;
    } else {
        printf("Device #%u loaded as %s from node #%02u.\n", doser->num_devices,
                                           doser->devices[(doser->num_devices - 1)]->name,
                                           doser->devices[(doser->num_devices - 1)]->provider_id);
    }

    ret = load_device(doser, NODE_HUB_2, "Light_0");
    if (ret < 1) {
        printf("error: load device()");
        return -1;
    } else {
        printf("Device #%u loaded as %s from node #%02u.\n", doser->num_devices,
                                           doser->devices[(doser->num_devices - 1)]->name,
                                           doser->devices[(doser->num_devices - 1)]->provider_id);
    }
    
    ret = load_device(doser, NODE_HUB_2, "Light_backup_0");
    if (ret < 1) {
        printf("error: load device()");
        return -1;
    } else {
        printf("Device #%u loaded as %s from node #%02u.\n", doser->num_devices,
                                           doser->devices[(doser->num_devices - 1)]->name,
                                           doser->devices[(doser->num_devices - 1)]->provider_id);
    }   

    ret = load_device(doser, NODE_HUB_2, "Exhaust_fan_0");
    if (ret < 1) {
        printf("error: load device()");
        return -1;
    } else {
        printf("Device #%u loaded as %s from node #%02u.\n", doser->num_devices,
                                           doser->devices[(doser->num_devices - 1)]->name,
                                           doser->devices[(doser->num_devices - 1)]->provider_id);
    }

    ret = load_device(doser, NODE_HUB_2, "Air_fan_0");
    if (ret < 1) {
        printf("error: load device()");
        return -1;
    } else {
        printf("Device #%u loaded as %s from node #%02u.\n", doser->num_devices,
                                           doser->devices[(doser->num_devices - 1)]->name,
                                           doser->devices[(doser->num_devices - 1)]->provider_id);
    }

    ret = load_device(doser, NODE_HUB_2, "Air_fan_1");
    if (ret < 1) {
        printf("error: load device()");
        return -1;
    } else {
        printf("Device #%u loaded as %s from node #%02u.\n", doser->num_devices,
                                           doser->devices[(doser->num_devices - 1)]->name,
                                           doser->devices[(doser->num_devices - 1)]->provider_id);
    }

    ret = load_device(doser, NODE_HUB_2, "Mixer_pump_0");
    if (ret < 1) {
        printf("error: load device()");
        return -1;
    } else {
        printf("Device #%u loaded as %s from node #%02u.\n", doser->num_devices,
                                           doser->devices[(doser->num_devices - 1)]->name,
                                           doser->devices[(doser->num_devices - 1)]->provider_id);
    }

    ret = load_device(doser, NODE_HUB_2, "Air_pump_0");
    if (ret < 1) {
        printf("error: load device()");
        return -1;
    } else {
        printf("Device #%u loaded as %s from node #%02u.\n", doser->num_devices,
                                           doser->devices[(doser->num_devices - 1)]->name,
                                           doser->devices[(doser->num_devices - 1)]->provider_id);
    }

    ret = load_device(doser, NODE_HUB_2, "Air_pump_1");
    if (ret < 1) {
        printf("error: load device()");
        return -1;
    } else {
        printf("Device #%u loaded as %s from node #%02u.\n", doser->num_devices,
                                           doser->devices[(doser->num_devices - 1)]->name,
                                           doser->devices[(doser->num_devices - 1)]->provider_id);
    }

    return 1;
}

int8_t load_sensors(struct doser_t *doser) {
    /* load sensors from a csv file? */
    /* sensor_provider, sensor_name, num_data_sources, data_source_type, data_type, ...*/
    /* 1,DHT22_0,2,x,y,a,b,1,DHT22_1,x,y,a,b,1,PH_PROBE_0,z,b,... */
    /* TODO: ret = load_sensor(...); if (ret == bad) { error }; */
    
    load_sensor(doser, NODE_HUB_1, "DHT22_0", SENSOR_TYPE_HUMI, DATA_TYPE_UINT8,
                                              SENSOR_TYPE_TEMP, DATA_TYPE_FLOAT);
    printf("%15s (node: #%02u) loaded as Sensor #%02u\n",
           doser->sensors[doser->num_sensors - 1]->name, 
           doser->sensors[doser->num_sensors - 1]->provider_id, doser->num_sensors - 1);

    load_sensor(doser, NODE_HUB_1, "DHT22_1", SENSOR_TYPE_HUMI, DATA_TYPE_UINT8,
                                              SENSOR_TYPE_TEMP, DATA_TYPE_FLOAT);
    printf("%15s (node: #%02u) loaded as Sensor #%02u\n",
           doser->sensors[doser->num_sensors - 1]->name, 
           doser->sensors[doser->num_sensors - 1]->provider_id, doser->num_sensors - 1);

    if (load_sensor(doser, NODE_HUB_1, "PH_PROBE_0", SENSOR_TYPE_PH, DATA_TYPE_FLOAT, 0, 0) < 1) {
        printf("error: load sensor()\n");
    } else {
        printf("%15s (node: #%02u) loaded as Sensor #%02u\n",
               doser->sensors[doser->num_sensors - 1]->name, 
               doser->sensors[doser->num_sensors - 1]->provider_id, doser->num_sensors - 1);
    }

    load_sensor(doser, NODE_HUB_1, "EC_PROBE_0", SENSOR_TYPE_EC, DATA_TYPE_FLOAT, 0, 0);
    printf("%15s (node: #%02u) loaded as Sensor #%02u\n",
           doser->sensors[doser->num_sensors - 1]->name, 
           doser->sensors[doser->num_sensors - 1]->provider_id, doser->num_sensors - 1);
    
    load_sensor(doser, NODE_HUB_1, "DS18B20_0", SENSOR_TYPE_TEMP, DATA_TYPE_FLOAT, 0, 0);
    printf("%15s (node: #%02u) loaded as Sensor #%02u\n",
           doser->sensors[doser->num_sensors - 1]->name, 
           doser->sensors[doser->num_sensors - 1]->provider_id, doser->num_sensors - 1);
        
    load_sensor(doser, NODE_HUB_1, "PH_PROBE_1", SENSOR_TYPE_PH, DATA_TYPE_FLOAT, 0, 0);
    printf("%15s (node: #%02u) loaded as Sensor #%02u\n",
           doser->sensors[doser->num_sensors - 1]->name, 
           doser->sensors[doser->num_sensors - 1]->provider_id, doser->num_sensors - 1);

    load_sensor(doser, NODE_HUB_1, "EC_PROBE_1", SENSOR_TYPE_EC, DATA_TYPE_FLOAT, 0, 0);
    printf("%15s (node: #%02u) loaded as Sensor #%02u\n",
           doser->sensors[doser->num_sensors - 1]->name, 
           doser->sensors[doser->num_sensors - 1]->provider_id, doser->num_sensors - 1);

    load_sensor(doser, NODE_HUB_1, "DS18B20_1", SENSOR_TYPE_TEMP, DATA_TYPE_FLOAT, 0, 0);
    printf("%15s (node: #%02u) loaded as Sensor #%02u\n",
           doser->sensors[doser->num_sensors - 1]->name, 
           doser->sensors[doser->num_sensors - 1]->provider_id, doser->num_sensors - 1);

    load_sensor(doser, NODE_HUB_2, "DHT22_0", SENSOR_TYPE_HUMI, DATA_TYPE_UINT8,
        SENSOR_TYPE_TEMP, DATA_TYPE_FLOAT);
    printf("%15s (node: #%02u) loaded as Sensor #%02u\n",
           doser->sensors[doser->num_sensors - 1]->name, 
           doser->sensors[doser->num_sensors - 1]->provider_id, doser->num_sensors - 1);

    load_sensor(doser, NODE_HUB_2, "DHT22_1", SENSOR_TYPE_HUMI, DATA_TYPE_UINT8,
        SENSOR_TYPE_TEMP, DATA_TYPE_FLOAT);
    printf("%15s (node: #%02u) loaded as Sensor #%02u\n",
           doser->sensors[doser->num_sensors - 1]->name, 
           doser->sensors[doser->num_sensors - 1]->provider_id, doser->num_sensors - 1);
        
    load_sensor(doser, NODE_HUB_2, "PH_PROBE_0", SENSOR_TYPE_PH, DATA_TYPE_FLOAT, 0, 0);
    printf("%15s (node: #%02u) loaded as Sensor #%02u\n",
           doser->sensors[doser->num_sensors - 1]->name, 
           doser->sensors[doser->num_sensors - 1]->provider_id, doser->num_sensors - 1);

    load_sensor(doser, NODE_HUB_2, "EC_PROBE_0", SENSOR_TYPE_EC, DATA_TYPE_FLOAT, 0, 0);
    printf("%15s (node: #%02u) loaded as Sensor #%02u\n",
           doser->sensors[doser->num_sensors - 1]->name, 
           doser->sensors[doser->num_sensors - 1]->provider_id, doser->num_sensors - 1);

    load_sensor(doser, NODE_HUB_2, "DS18B20_0", SENSOR_TYPE_TEMP, DATA_TYPE_FLOAT, 0, 0);
    printf("%15s (node: #%02u) loaded as Sensor #%02u\n",
           doser->sensors[doser->num_sensors - 1]->name, 
           doser->sensors[doser->num_sensors - 1]->provider_id, doser->num_sensors - 1);

    return 1;
}

int8_t create_environments(struct doser_t *doser) {
    /*
     * create environments and reservoirs, assign reservoirs to environments
     */
     
    int8_t child_id = 0;
    int8_t parent_id = 0;    
    
    parent_id = create_environment(doser, "120x120", ENVIRONMENT_TYPE_ENVIRONMENT);
    if (parent_id < 1) {
        printf("error: create_environment()\n");
        return -1;
    }
    printf("Created environment #%02u - %s\n", parent_id - 1, doser->environments[parent_id - 1]->name);
    
    child_id = create_environment(doser, "Habanero", ENVIRONMENT_TYPE_RESERVOIR);
    if (child_id < 1) {
        printf("error: create_environment()\n");
        return -1;
    }
    printf("Created reservoir #%02u - %s\n", child_id - 1, doser->reservoirs[child_id - 1]->name);
    assign_environment((doser->environments[parent_id - 1]), (doser->reservoirs[child_id - 1]));
    
    child_id = create_environment(doser, "Scotch Bonnett", ENVIRONMENT_TYPE_RESERVOIR);
    if (child_id < 1) {
        printf("error: create_environment()\n");
        return -1;
    }
    printf("Created reservoir #%02u - %s\n", child_id - 1, doser->reservoirs[child_id - 1]->name);
    assign_environment((doser->environments[parent_id - 1]), (doser->reservoirs[child_id - 1]));    
    
    parent_id = create_environment(doser, "100x100", ENVIRONMENT_TYPE_ENVIRONMENT);
    if (parent_id < 1) {
        printf("error: create_environment()\n");
        return -1;
    }
    printf("Created environment #%02u - %s\n", parent_id - 1, doser->environments[parent_id - 1]->name);
    
    child_id = create_environment(doser, "Strawberries", ENVIRONMENT_TYPE_RESERVOIR);
    if (child_id < 1) {
        printf("error: create_environment()\n");
        return -1;
    }
    printf("Created reservoir #%02u - %s\n", child_id - 1, doser->reservoirs[child_id - 1]->name);
    assign_environment((doser->environments[parent_id - 1]), (doser->reservoirs[child_id - 1]));    
    
    return 1;
}

int8_t setup_test_environment(struct doser_t *doser) {
    /*
     * create ph & ec controllers and assign them to reservoirs
     * assign data providers to controllers later
     */

    int8_t child_id = 0;
    
    child_id = create_controller(doser, CONTROLLER_TYPE_PH, "pH controller #1");
    printf("Created controller %02u\n", child_id - 1);
    assign_controller((doser->reservoirs[0]), (doser->controllers[child_id - 1]));
    printf("Assigned %s to %s (%02u)\n", doser->controllers[child_id - 1]->name,
                                         doser->reservoirs[0]->name, doser->reservoirs[0]->id);
    
    child_id = create_controller(doser, CONTROLLER_TYPE_EC, "EC controller #1");
    printf("Created controller %02u\n", child_id - 1);
    assign_controller((doser->reservoirs[0]), (doser->controllers[child_id - 1]));
    printf("Assigned %s to %s (%02u)\n", doser->controllers[child_id - 1]->name,
                                         doser->reservoirs[0]->name, doser->reservoirs[0]->id);

    child_id = create_controller(doser, CONTROLLER_TYPE_PH, "pH controller #2");
    printf("Created controller %02u\n", child_id - 1);
    assign_controller((doser->reservoirs[1]), (doser->controllers[child_id - 1]));
    printf("Assigned %s to %s (%02u)\n", doser->controllers[child_id - 1]->name,
                                         doser->reservoirs[1]->name, doser->reservoirs[1]->id);
    
    child_id = create_controller(doser, CONTROLLER_TYPE_EC, "EC controller #2");
    printf("Created controller %02u\n", child_id - 1);
    assign_controller((doser->reservoirs[1]), (doser->controllers[child_id - 1]));
    printf("Assigned %s to %s (%02u)\n", doser->controllers[child_id - 1]->name,
                                         doser->reservoirs[1]->name, doser->reservoirs[1]->id);
    
    child_id = create_controller(doser, CONTROLLER_TYPE_PH, "pH controller #3");
    printf("Created controller %02u\n", child_id - 1);
    assign_controller((doser->reservoirs[2]), (doser->controllers[child_id - 1]));
    printf("Assigned %s to %s (%02u)\n", doser->controllers[child_id - 1]->name,
                                         doser->reservoirs[2]->name, doser->reservoirs[2]->id);
    
    child_id = create_controller(doser, CONTROLLER_TYPE_EC, "EC controller #3");
    printf("Created controller %02u\n", child_id - 1);
    assign_controller((doser->reservoirs[2]), (doser->controllers[child_id - 1]));
    printf("Assigned %s to %s (%02u)\n", doser->controllers[child_id - 1]->name,
                                         doser->reservoirs[2]->name, doser->reservoirs[2]->id);
    printf("\n");
    /*
     * assign devices to environments and reservoirs
     */
    for (uint8_t i = 0; i < 5; i++) {
        if(assign_device((doser->environments[0]), (doser->devices[i])) < 0) {
            printf("failed assigning device\n");
            return -1;
        }
        printf("Assigned %s to %s (%02u)\n", doser->devices[i]->name, doser->environments[0]->name,
                                             doser->environments[0]->id);
    }
    printf("\n");
    for (uint8_t i = 5; i < 8; i++) {
        if(assign_device((doser->reservoirs[0]), (doser->devices[i])) < 0) {
            printf("failed assigning device\n");
            return -1;
        }
        printf("Assigned %s to %s (%02u)\n", doser->devices[i]->name, doser->reservoirs[0]->name,
                                             doser->reservoirs[0]->id);
    }   
    printf("\n");
    for (uint8_t i = 8; i < 11; i++) {
        if(assign_device((doser->reservoirs[1]), (doser->devices[i])) < 0) {
            printf("failed assigning device\n");
            return -1;
        }
        printf("Assigned %s to %s (%02u)\n", doser->devices[i]->name, doser->reservoirs[1]->name,
                                             doser->reservoirs[1]->id);
    }
    printf("\n");
    for (uint8_t i = 11; i < 16; i++) {
        if(assign_device((doser->environments[1]), (doser->devices[i])) < 0) {
            printf("failed assigning device\n");
            return -1;
        }
        printf("Assigned %s to %s (%02u)\n", doser->devices[i]->name,
                                             doser->environments[1]->name, doser->environments[1]->id);
    }
    printf("\n");
    for (uint8_t i = 16; i < 19; i++) {
        if(assign_device((doser->reservoirs[2]), (doser->devices[i])) < 0) {
            printf("failed assigning device\n");
            return -1;
        }
        printf("Assigned %s to %s (%02u)\n", doser->devices[i]->name,
                                             doser->reservoirs[2]->name, doser->reservoirs[2]->id);
    }       
    printf("\n");
    /* assign sensors to environments and reservoirs */
    for (uint8_t i = 0; i < 2; i++) {
        assign_sensor(doser->environments[0], doser->sensors[i]);
        printf("Assigned %s to %s (%02u)\n", doser->sensors[i]->name,
                                             doser->environments[0]->name, doser->environments[0]->id);
    }
    printf("\n");
    for (uint8_t i = 2; i < 5; i++) {
        assign_sensor(doser->reservoirs[0], doser->sensors[i]);
        printf("Assigned %s to %s (%02u)\n", doser->sensors[i]->name,
                                             doser->reservoirs[0]->name, doser->reservoirs[0]->id);     
    }
    printf("\n");
    for (uint8_t i = 5; i < 8; i++) {
        assign_sensor(doser->reservoirs[1], doser->sensors[i]);
        printf("Assigned %s to %s (%02u)\n", doser->sensors[i]->name,
                                             doser->reservoirs[1]->name, doser->reservoirs[1]->id);     
    }
    printf("\n");
    for (uint8_t i = 8; i < 10; i++) {
        assign_sensor(doser->environments[1], doser->sensors[i]);
        printf("Assigned %s to %s (%02u)\n", doser->sensors[i]->name,
                                             doser->environments[1]->name, doser->environments[1]->id);     
    }
    printf("\n");
    for (uint8_t i = 10; i < 13; i++) {
        assign_sensor(doser->reservoirs[2], doser->sensors[i]);
        printf("Assigned %s to %s (%02u)\n", doser->sensors[i]->name,
                                             doser->reservoirs[2]->name, doser->reservoirs[2]->id);     
    }   
    printf("\n");
    /* assign provider to parent sensor's parent env, or if env type = res, then parent of res */
    uint8_t id = 0;
    struct sensor_t *tmp_ptr = NULL;
    for (uint8_t i = 0; i < doser->num_data_providers; i++) {
        tmp_ptr = doser->data_providers[i]->data_sources[0]->parent;
        
        if (tmp_ptr->parent->type == ENVIRONMENT_TYPE_RESERVOIR) {
            id = tmp_ptr->parent->parent->id;
            assign_data_provider(doser->environments[id], doser->data_providers[i]);
        } else if (tmp_ptr->parent->type == ENVIRONMENT_TYPE_ENVIRONMENT) {
            id = tmp_ptr->parent->id;
            assign_data_provider(doser->environments[id], doser->data_providers[i]);
        }
        printf("Assigned %s (%s) to %s\n",
               doser->data_providers[i]->name,
               doser->data_providers[i]->data_sources[0]->parent->name,
               doser->environments[id]->name);      
    }   
    
    return 1;
}

int8_t create_tasks(struct doser_t *doser) {
    /* create two example tasks: device and controller */
    
    /* what if create_schedule accepted structs and packed them into schedule_t */
    /* and create_task accepted schedule_t as well as other arguments */
    
    struct controller_t *ctrl_task_ptr;
    uint8_t task_id = 0;
    float setpoint = 5.80;
    
    ctrl_task_ptr = doser->reservoirs[0]->controllers[0];
    task_id = task_create(doser, "pH setpoint", TASK_PH_SETPOINT, ctrl_task_ptr);
    task_add_setpoint(doser->tasks[task_id - 1], setpoint);
    printf("Task created: %02u - %s \n", task_id, doser->tasks[task_id - 1]->name);
    
    struct time_hhmmss *on_period = malloc(sizeof(struct time_hhmmss));
    if (on_period == NULL) {
        printf("error: malloc on_period\n");
        return -1;
    }
    on_period->hh = 0;  
    on_period->mm = 0;
    on_period->ss = 1; /* on_period > 0 && off_period = 0 => on 24/7 */
    
    struct time_hhmmss *off_period = malloc(sizeof(struct time_hhmmss));
    if (off_period == NULL) {
        printf("error: malloc off_period\n");
        return -1;
    }
    off_period->hh = 0; 
    off_period->mm = 0;
    off_period->ss = 0;
    
    struct time_hhmmss *start_time = malloc(sizeof(struct time_hhmmss));
    if (start_time == NULL) {
        printf("error: malloc start_time\n");
        return -1;
    }
    start_time->hh = 0;
    start_time->mm = 0;
    start_time->ss = 0;

    struct time_hhmmss *stop_time = malloc(sizeof(struct time_hhmmss));
    if (stop_time == NULL) {
        printf("error: malloc stop_time\n");
        return -1;
    }
    /* task runs throughout the day */
    stop_time->hh = 0;
    stop_time->mm = 0;
    stop_time->ss = 0;

    struct tm *enable_datetime = malloc(sizeof(struct tm));
    if (enable_datetime == NULL) {
        printf("error: malloc enable_datetime\n");
        return -1;
    }
    enable_datetime->tm_year = 121; /* The number of years since 1900   */
    enable_datetime->tm_mon  = 10;  /* month, range 0 to 11             */
    enable_datetime->tm_mday = 1;   /* day of the month, range 1 to 31  */
    enable_datetime->tm_hour = 0;   /* hours, range 0 to 23             */
    enable_datetime->tm_min  = 0;   /* minutes, range 0 to 59           */
    enable_datetime->tm_sec  = 0;   /* seconds,  range 0 to 59          */

    struct tm *disable_datetime = malloc(sizeof(struct tm));
    if (disable_datetime == NULL) {
        printf("error: malloc disable_datetime\n");
        return -1;
    }
    disable_datetime->tm_year = 122;
    disable_datetime->tm_mon  = 1;
    disable_datetime->tm_mday = 31;
    disable_datetime->tm_hour = 0;
    disable_datetime->tm_min  = 0;
    disable_datetime->tm_sec  = 0;
    
    task_set_schedule(doser->tasks[0], on_period, off_period, start_time,
                      stop_time, enable_datetime, disable_datetime);
    printf("Task schedule created. set between %04u-%02u-%02u and %04u-%02u-%02u\n",
           doser->tasks[0]->schedule->enable_datetime->tm_year+1900,
           doser->tasks[0]->schedule->enable_datetime->tm_mon,
           doser->tasks[0]->schedule->enable_datetime->tm_mday,
           doser->tasks[0]->schedule->disable_datetime->tm_year+1900,
           doser->tasks[0]->schedule->disable_datetime->tm_mon,
           doser->tasks[0]->schedule->disable_datetime->tm_mday);
    
    task_assign(ctrl_task_ptr->parent->parent, doser->tasks[0]);
    
    printf("%s num tasks: %02u\n",
           ctrl_task_ptr->parent->parent->name, 
           ctrl_task_ptr->parent->parent->num_tasks);

    /* task 2 */
    struct device_t *dev_task_ptr = doser->reservoirs[0]->devices[0];
    task_id = task_create(doser, "Nutrient pump: continuous", TASK_DEVICE, dev_task_ptr);
    printf("Task created: %02u - %s \n", task_id, doser->tasks[task_id - 1]->name);
    
    on_period = malloc(sizeof(struct time_hhmmss));
    if (on_period == NULL) {
        printf("error: malloc on_period\n");
        return -1;
    }
    on_period->hh = 0;
    on_period->mm = 0;
    on_period->ss = 1;
    
    off_period = malloc(sizeof(struct time_hhmmss));
    if (off_period == NULL) {
        printf("error: malloc off_period\n");
        return -1;
    }
    off_period->hh = 0;
    off_period->mm = 0;
    off_period->ss = 0;
    
    start_time = malloc(sizeof(struct time_hhmmss));
    if (start_time == NULL) {
        printf("error: malloc start_time\n");
        return -1;
    }
    
    update_system_time(doser);
    start_time->hh = doser->timers->system_time->hh;
    start_time->mm = doser->timers->system_time->mm;
    start_time->ss = 00;

    stop_time = malloc(sizeof(struct time_hhmmss));
    if (stop_time == NULL) {
        printf("error: malloc stop_time\n");
        return -1;
    }
    /* task runs throughout the day */
    update_system_time(doser);
    stop_time->hh = doser->timers->system_time->hh;
    stop_time->mm = doser->timers->system_time->mm + 1;
    stop_time->ss = 00;

    enable_datetime = malloc(sizeof(struct tm));
    if (enable_datetime == NULL) {
        printf("error: malloc enable_datetime\n");
        return -1;
    }
    enable_datetime->tm_year = 121; /* The number of years since 1900   */
    enable_datetime->tm_mon  = 10;  /* month, range 0 to 11             */
    enable_datetime->tm_mday = 1;   /* day of the month, range 1 to 31  */
    enable_datetime->tm_hour = 0;   /* hours, range 0 to 23             */
    enable_datetime->tm_min  = 0;   /* minutes, range 0 to 59           */
    enable_datetime->tm_sec  = 0;   /* seconds,  range 0 to 59          */

    disable_datetime = malloc(sizeof(struct tm));
    if (disable_datetime == NULL) {
        printf("error: malloc disable_datetime\n");
        return -1;
    }
    disable_datetime->tm_year = 122;
    disable_datetime->tm_mon  = 1;
    disable_datetime->tm_mday = 31;
    disable_datetime->tm_hour = 0;
    disable_datetime->tm_min  = 0;
    disable_datetime->tm_sec  = 0;

    task_set_schedule(doser->tasks[1], on_period, off_period, start_time,
                      stop_time, enable_datetime, disable_datetime);
    printf("Task schedule created. on: %02u:%02u:%02u off: %02u:%02u:%02u. set between %04u-%02u-%02u and %04u-%02u-%02u\n",
           doser->tasks[1]->schedule->on_period->hh,
           doser->tasks[1]->schedule->on_period->mm,
           doser->tasks[1]->schedule->on_period->ss,
           doser->tasks[1]->schedule->off_period->hh,
           doser->tasks[1]->schedule->off_period->mm,
           doser->tasks[1]->schedule->off_period->ss,
           doser->tasks[1]->schedule->enable_datetime->tm_year+1900,
           doser->tasks[1]->schedule->enable_datetime->tm_mon,
           doser->tasks[1]->schedule->enable_datetime->tm_mday,
           doser->tasks[1]->schedule->disable_datetime->tm_year+1900,
           doser->tasks[1]->schedule->disable_datetime->tm_mon,
           doser->tasks[1]->schedule->disable_datetime->tm_mday);

    task_assign(dev_task_ptr->parents[0]->parent, doser->tasks[1]);
    
    /* task 3 */
    task_id = 0;
    dev_task_ptr = doser->reservoirs[0]->devices[0];
    task_id = task_create(doser, "Nutrient pump 30 / 30", TASK_DEVICE, dev_task_ptr);
    printf("Task created: %02u - %s \n", task_id, doser->tasks[task_id - 1]->name);
    
    on_period = malloc(sizeof(struct time_hhmmss));
    if (on_period == NULL) {
        printf("error: malloc on_period\n");
        return -1;
    }
    on_period->hh = 0;
    on_period->mm = 0;
    on_period->ss = 30;
    
    off_period = malloc(sizeof(struct time_hhmmss));
    if (off_period == NULL) {
        printf("error: malloc off_period\n");
        return -1;
    }
    off_period->hh = 0;
    off_period->mm = 0;
    off_period->ss = 30;
    
    start_time = malloc(sizeof(struct time_hhmmss));
    if (start_time == NULL) {
        printf("error: malloc start_time\n");
        return -1;
    }
    update_system_time(doser);
    start_time->hh = doser->timers->system_time->hh;
    start_time->mm = doser->timers->system_time->mm + 1;    
    start_time->ss = 00;

    stop_time = malloc(sizeof(struct time_hhmmss));
    if (stop_time == NULL) {
        printf("error: malloc stop_time\n");
        return -1;
    }
    /* task runs throughout the day */
    stop_time->hh = doser->timers->system_time->hh;
    stop_time->mm = doser->timers->system_time->mm + 2;
    stop_time->ss = 0;

    enable_datetime = malloc(sizeof(struct tm));
    if (enable_datetime == NULL) {
        printf("error: malloc enable_datetime\n");
        return -1;
    }
    enable_datetime->tm_year = 121; /* The number of years since 1900   */
    enable_datetime->tm_mon  = 10;  /* month, range 0 to 11             */
    enable_datetime->tm_mday = 1;   /* day of the month, range 1 to 31  */
    enable_datetime->tm_hour = 0;   /* hours, range 0 to 23             */
    enable_datetime->tm_min  = 0;   /* minutes, range 0 to 59           */
    enable_datetime->tm_sec  = 0;   /* seconds,  range 0 to 59          */

    disable_datetime = malloc(sizeof(struct tm));
    if (disable_datetime == NULL) {
        printf("error: malloc disable_datetime\n");
        return -1;
    }
    disable_datetime->tm_year = 122;
    disable_datetime->tm_mon  = 1;
    disable_datetime->tm_mday = 31;
    disable_datetime->tm_hour = 0;
    disable_datetime->tm_min  = 0;
    disable_datetime->tm_sec  = 0;

    task_set_schedule(doser->tasks[2], on_period, off_period, start_time,
                      stop_time, enable_datetime, disable_datetime);
    printf("Task schedule created. On: %02u:%02u:%02u Off: %02u:%02u:%02u. Set between %04u-%02u-%02u and %04u-%02u-%02u\n",
           doser->tasks[2]->schedule->on_period->hh,
           doser->tasks[2]->schedule->on_period->mm,
           doser->tasks[2]->schedule->on_period->ss,
           doser->tasks[2]->schedule->off_period->hh,
           doser->tasks[2]->schedule->off_period->mm,
           doser->tasks[2]->schedule->off_period->ss,
           doser->tasks[2]->schedule->enable_datetime->tm_year+1900,
           doser->tasks[2]->schedule->enable_datetime->tm_mon,
           doser->tasks[2]->schedule->enable_datetime->tm_mday,
           doser->tasks[2]->schedule->disable_datetime->tm_year+1900,
           doser->tasks[2]->schedule->disable_datetime->tm_mon,
           doser->tasks[2]->schedule->disable_datetime->tm_mday);

    task_assign(dev_task_ptr->parents[0]->parent, doser->tasks[2]); 
    
    printf("%s num tasks: %02u\n",
           dev_task_ptr->parents[0]->parent->name,
           dev_task_ptr->parents[0]->parent->num_tasks);
           
    return 1;
}

uint8_t print_system_lists(struct doser_t *doser) {
    printf("System lists:\n");
    printf("\n");
    for (uint8_t i = 0; i < doser->num_nodes; i++) {
        printf("Node: %02u\t%s\t%u.%u.%u.%u\n",
               doser->nodes[i]->id, doser->nodes[i]->name,
               doser->nodes[i]->ip_addr.a, doser->nodes[i]->ip_addr.b,
               doser->nodes[i]->ip_addr.c, doser->nodes[i]->ip_addr.d);
    }   
    printf("\n");
    for (uint8_t i = 0; i < doser->num_environments; i++) {
        printf("Environment %02u\tType: %u\t%s\n", i,
               doser->environments[i]->type, doser->environments[i]->name);
    }
    printf("\n");
    for (uint8_t i = 0; i < doser->num_reservoirs; i++) {
        printf("Reservoir %02u\tType: %u\t%s\n", i,
               doser->reservoirs[i]->type, doser->reservoirs[i]->name);
    }
    printf("\n");
    for (uint8_t i = 0; i < doser->num_devices; i++) {
        printf("Device %02u\tProvider: %02u - %s\tName: %s\n", i,
               doser->devices[i]->provider_id, 
               doser->nodes[doser->devices[i]->provider_id]->name,
               doser->devices[i]->name);
    }
    printf("\n");
    for (uint8_t i = 0; i < doser->num_sensors; i++) {
        printf("Sensor %02u\tProvider: %02u - %s\tName: %s\n", i,
               doser->sensors[i]->provider_id, 
               doser->nodes[doser->sensors[i]->provider_id]->name,
               doser->sensors[i]->name);
        
    }
    printf("\n");
    for (uint8_t i = 0; i < doser->num_controllers; i++) {
        printf("Controller %02u\tType: %02u Name: %s\tIn reservoir: %02u - %s\n", i,
               doser->controllers[i]->type,
               doser->controllers[i]->name,
               doser->controllers[i]->parent->id,
               doser->controllers[i]->parent->name);
    }
    printf("\n");
    for (uint8_t i = 0; i < doser->num_data_sources; i++) {
        printf("Data source %02u\tType: %s\tFrom sensor: %s\n", i,
               sensor_type_strings[doser->data_sources[i]->sensor_type],
               doser->data_sources[i]->parent->name);
    }
    printf("\n");
    for (uint8_t i = 0; i < doser->num_data_providers; i++) {
        printf("Data provider %02u - %s\tType: %02u\tSource: %02u (%s)\n", i,
               doser->data_providers[i]->name, doser->data_providers[i]->type,
               doser->data_providers[i]->data_sources[0]->id,
               doser->data_providers[i]->data_sources[0]->parent->name);
    }
    printf("\n");
    for (uint8_t i = 0; i < doser->num_tasks; i++) {
        printf("Task: %02u - %s (%s)\n", i,
               doser->tasks[i]->name, doser->tasks[i]->parent->name);
    }
    printf("\n");
    
    return 1;
}

int8_t main(void) {
    uint8_t print_delay = 100;
    struct doser_t doser;
    doser.num_environments = 0;
    doser.environments = NULL;
    doser.num_reservoirs = 0;
    doser.reservoirs = NULL;
    doser.num_devices = 0;
    doser.devices = NULL;
    doser.num_sensors = 0;
    doser.sensors = NULL;
    doser.num_tasks = 0;
    doser.tasks = NULL;
    doser.num_controllers = 0;
    doser.controllers = NULL;
    doser.num_data_providers = 0;
    doser.data_providers = NULL;
    doser.num_data_sources = 0;
    doser.data_sources = NULL;
    doser.num_nodes = 0;
    doser.nodes = NULL;
    
    doser.timers = malloc(sizeof(struct timers_t));
    if (doser.timers == NULL) {
        printf("error: malloc doser->timers\n");
        return 1;
    }
    doser.timers->system_datetime = malloc(sizeof(struct tm));
    if (doser.timers->system_datetime == NULL) {
        printf("error: malloc doser->timers->system_datetime\n");
        return 1;
    }
    doser.timers->system_time = malloc(sizeof(struct time_hhmmss));
    if (doser.timers->system_time == NULL) {
        printf("error: malloc doser->timers->system_time\n");
        return 1;
    }   
    
    wiringPiSetup();
    
    printf("Loading nodes...\n");
    if (load_nodes(&doser) == -1) {
        printf("failed! Exiting...\n");
        return -1;
    }
    printf("Nodes done!\n");
    printf("\n");
    delayms(print_delay);
    printf("Loading devices...\n");
    if (load_devices(&doser) == -1) {
        printf("failed! Exiting...\n");
        return -1;
    }
    printf("Devices done!\n");
    printf("\n");
    delayms(print_delay);
    printf("Loading sensors...\n");
    if (load_sensors(&doser) == -1) {
        printf("failed! Exiting...\n");
        return -1;
    }
    printf("Sensors done!\n");
    printf("\n");
    delayms(print_delay);
    printf("Creating environments...\n");
    if (create_environments(&doser) == -1) {
        printf("failed! Exiting...\n");
        return -1;
    }
    printf("Environments done!\n");
    printf("\n");
    delayms(print_delay);
    printf("Setting up the test environment...\n");
    if (setup_test_environment(&doser) == -1) {
        printf("failed! Exiting...\n");
        return -1;
    }
    printf("Test environment done!\n");
    printf("\n");
    delayms(print_delay);
    printf("Creating tasks...\n");
    create_tasks(&doser);
    printf("Tasks done!\n");
    printf("\n");
    delayms(print_delay);
    printf("Loading data...\n");
    load_readings(&doser);
    printf("Loading data done!\n");
    printf("\n");   
    delayms(print_delay);
    printf("Printing system lists...\n");
    print_system_lists(&doser);
    printf("Lists done!\n");
    printf("\n");
    delayms(print_delay);
    printf("Printing doser hierarchy...\n");    
    print_hierarchy(&doser);
    printf("Hierarchy done!\n");
    printf("\n");
    delayms(print_delay);
    printf("Main loop ticking at 1 Hz...\n");
    delayms(1000);
    printf("\033[2J");
    uint8_t running = 1;
    uint64_t round_time = 0;
    struct schedule_item_t *task;
    round_time = millis();
    uint8_t seconds = 0;
    do {
        update_system_time(&doser);
        if (millis() >= (round_time + 1000)) {
            printf("\033[1;1H");
            round_time = millis();
            data_acquisition(&doser);   
            update_data_providers(&doser);
            process_scheduled_tasks(&doser);
            
            for (uint8_t t = 0; t < doser.num_tasks; t++) {
                task = doser.tasks[t];
                print_tabs(1);
                printf("\t   Task: %02u - %s\n", t, task->name);
                print_task(task, 1);
                printf("\n");
            }
            
            printf("System time: %02u:%02u:%02u %04u-%02u-%02u\tStatus: waiting...%08u\n",
                   doser.timers->system_datetime->tm_hour,
                   doser.timers->system_datetime->tm_min,
                   doser.timers->system_datetime->tm_sec,
                   doser.timers->system_datetime->tm_year+1900,
                   doser.timers->system_datetime->tm_mon,
                   doser.timers->system_datetime->tm_mday,
                   millis());
            printf("\n");
            seconds++;
        }
        delay(1);
    } while (seconds < 59);
    
    return 0;
}