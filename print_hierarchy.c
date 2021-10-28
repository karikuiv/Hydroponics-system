/* 
 * Copyright 2021 Kari Kuivalainen ( https://github.com/karikuiv )
 * Read only license: These files are uploaded for the sole purpose of showing code samples to potential employers.
 * See readme_license.txt for more information
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

#include <wiringPi.h>

#include "print_hierarchy.h"
#include "environment.h"
#include "device.h"
#include "sensor.h"
#include "schedule.h"

const char *task_status_strings[] = {"Unknown", "Off", "On", "Disabled", "Enabled",
                                     "Inactive", "Active", "Waiting", "Dosing"};

/**
 * Delay milliseconds. Uses delay() from wiringPi.h.
 * 
 */
void delayms(uint16_t delay_count) {
    for (uint16_t i = 0; i < delay_count; i++) {
        delay(1);
    }
    
    return;
}

void print_tabs(uint8_t tabs) {
    for (uint8_t t = 0; t < tabs; t++) {
        printf("\t");
    }
    return;
}

/**
 * Print task details (probably only used for test environment this way).
 */
int8_t print_task(struct schedule_item_t *task, uint8_t tabs) {
    struct task_device_t *dev;
    struct task_setpoint_t *ctrl;

    /* maybe task should have its own status and follow device & controller status */
    /* that way can use universal function... */

    /* maybe handle tasks crudely for now, just call them */
    /* once a second and check if they should be running and do stuff */
    /* controller can set its wait times through internal variables */
    
    
    /* TODO: customize status line based on task and its status
     * NOTE: on/off period and start/stop times may not divide or sync evenly and must be taken into account
     *  
     *  unknown: schedule is uninitialized?
     *      off: Off for [time_remaining] until [next_start_time] (check if inactive at or before that)
     *       on: On for [time_remaining] until [next_stop_time] (check if (inactive at or before that)
     * disabled: Disabled (add "for [time_remaining] until [time_enabled]", if task becomes enabled in the future)
     *  enabled: Enabled (maybe used internally only? Task can be enabled: inactive/active)
     * inactive: Inactive until [start_time] (check if disabled at or before that)
     *   active: Active (maybe used internally only? Task can be active: on/off)
     *  waiting: Waiting for [timeleft] until [time after timeleft]
     *   dosing: Dosing for [time_left/dose_time]
     */
    print_tabs(tabs);
    if (task->type == TASK_DEVICE) {
        dev = (struct task_device_t *) task->action_ptr;
        printf("\t Status: %s          \n",
               task_status_strings[dev->status]);
        /*, 
               task->schedule->run_time->hh,
               task->schedule->run_time->mm,
               task->schedule->run_time->ss,
               task->schedule->start_time->hh,
               task->schedule->start_time->mm,
               task->schedule->start_time->ss);*/
    } else if ((task->type == TASK_PH_SETPOINT) || (task->type == TASK_EC_SETPOINT)) {
        ctrl = (struct task_setpoint_t *) task->action_ptr;
        printf("\t Status: %s          \n",
               task_status_strings[ctrl->status]);
    }

    uint32_t on_period_seconds = 0;
    on_period_seconds += task->schedule->on_period->hh * 60 * 60;
    on_period_seconds += task->schedule->on_period->mm * 60;
    on_period_seconds += task->schedule->on_period->ss;
    
    uint32_t off_period_seconds = 0;
    off_period_seconds += task->schedule->off_period->hh * 60 * 60;
    off_period_seconds += task->schedule->off_period->mm * 60;
    off_period_seconds += task->schedule->off_period->ss;               
    delay(1);
    print_tabs(tabs);
    /* TODO: customize program printout based on settings, 24/7 or Continuous or hh:mm:ss */
    if ((on_period_seconds > 0) && (off_period_seconds == 0)) {
        printf("\tProgram: On: Continuous\n");
    } else {
        printf("\tProgram: On: %02u:%02u:%02u Off: %02u:%02u:%02u\n",
           task->schedule->on_period->hh,
           task->schedule->on_period->mm,
           task->schedule->on_period->ss,
           task->schedule->off_period->hh,
           task->schedule->off_period->mm, 
           task->schedule->off_period->ss);
    }
    delay(1);
    /* TODO: calculate time left until switch? */
    /* TODO: calculate active duration? */
    print_tabs(tabs);
    printf("\t Active: From %02u:%02u:%02u until %02u:%02u:%02u\n",
           task->schedule->start_time->hh,
           task->schedule->start_time->mm,
           task->schedule->start_time->ss,
           task->schedule->stop_time->hh,
           task->schedule->stop_time->mm,
           task->schedule->stop_time->ss);
    delay(1);
    print_tabs(tabs);
    printf("\tEnabled: From %02u:%02u:%02u %04u-%02u-%02u until %02u:%02u:%02u %04u-%02u-%02u\n",
           task->schedule->enable_datetime->tm_hour,
           task->schedule->enable_datetime->tm_min,
           task->schedule->enable_datetime->tm_sec,
           task->schedule->enable_datetime->tm_year + 1900,
           task->schedule->enable_datetime->tm_mon + 1,
           task->schedule->enable_datetime->tm_mday,
           task->schedule->disable_datetime->tm_hour,
           task->schedule->disable_datetime->tm_min,
           task->schedule->disable_datetime->tm_sec,
           task->schedule->disable_datetime->tm_year + 1900,
           task->schedule->disable_datetime->tm_mon + 1,
           task->schedule->disable_datetime->tm_mday);
    delay(1);      
    return 1;
}

/**
 * Print all devices attached to an environment.
 */
int8_t print_devices(struct environment_t *environment, uint8_t tabs) {
    if (environment == NULL) {
        printf("error: environment is missing\n");
        return -1;
    }

    struct device_t *dev;
    struct schedule_item_t *task;
    uint8_t num_tasks = 0;

    for (uint8_t i = 0; i < environment->num_devices; i++) {
        dev = environment->devices[i];
        num_tasks = dev->num_attached_tasks;
        print_tabs(tabs);
        if (num_tasks == 0) {
            printf("+ Device #%02u: %s (Unassigned)\n", i, dev->name);
            delay(1);
        } else {
            printf("- Device #%02u: %s\n", i, dev->name);
            delay(1);
            for (uint8_t t = 0; t < num_tasks; t++) {
                task = dev->attached_tasks[t];
                print_tabs(tabs);
                printf("\t   Task: %02u - %s\n", t, task->name);                
                delay(1);
                print_task(task, tabs);
                printf("\n");
                delay(1);
            }
        }
    }
    
    return 1;
}

/**
 * Print all controllers attached to a reservoir.
 */
int8_t print_controllers(struct environment_t *reservoir, uint8_t tabs) {
    if (reservoir == NULL) {
        printf("error: reservoir is missing\n");
        return -1;
    }

    struct controller_t *controller;
    struct schedule_item_t *task;
    uint8_t num_tasks = 0;

    for (uint8_t i = 0; i < reservoir->num_controllers; i++) {
        controller = reservoir->controllers[i];
        num_tasks = controller->num_attached_tasks;
        print_tabs(tabs);
        if (num_tasks == 0) {
            printf("+ Controller #%02u: %s (Unassigned)\n", i, controller->name);
            delay(1);
        } else {
            printf("- Controller #%02u: %s\n", i, controller->name);
            delay(1);
            for (uint8_t t = 0; t < num_tasks; t++) {
                task = controller->attached_tasks[t];
                print_tabs(tabs);
                printf("\t   Task: %02u - %s\n", i, task->name);                
                delay(1);
                print_task(task, tabs);
                printf("\n");
                delay(1);
            }
        }
    }
    
    return 1;
}

/**
 * Print all sensors attached to an environment.
 */
int8_t print_sensors(struct environment_t *environment, uint8_t  tabs) {
    return 1;
}

/**
 * Print a hierarchical structure of environments and reservoirs and attached sensors, devices, tasks, etc.
 */
int8_t print_hierarchy(struct doser_t *doser) {
    struct environment_t *env = NULL;
    struct environment_t *res = NULL;
    uint8_t print_delay = 250;
    
    if (doser == NULL) {
        printf("error: doser is missing\n");
        return -1;
    }
    
 /*
  * TODO: consider sprintf every line to a large buffer
  *          then print everything line by line with delays
 */
    
    printf("[System]\n");
    
    for (uint8_t i = 0; i < doser->num_environments; i++) {
        env = doser->environments[i];
        printf("- Environment: %s\n", env->name);
        delayms(print_delay);
        print_devices(env, 1);
        printf("\n");
        delayms(print_delay);
        /*print_sensors(env, 1);
        printf("\n");*/
        for (uint8_t r = 0; r < env->num_reservoirs; r++) {
            res = env->reservoirs[r];
            printf("\t- Reservoir: %s\n", res->name);
            delayms(print_delay);
            print_devices(res, 2);
            printf("\n");
            delayms(print_delay);
            print_controllers(res, 2);
            printf("\n");
            delayms(print_delay);
            /*print_sensors(res, 2);
            printf("\n");*/
        }
    }
    
    return 1;
}