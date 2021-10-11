#pragma once

#include <time.h>
#include <stdint.h>

#include "doser_system.h"
#include "environment.h"
#include "device.h"

extern struct doser_t doser;

#define TASK_NOP                0x00
#define TASK_PH_SETPOINT        0x01
#define TASK_EC_SETPOINT        0x02
#define TASK_DEVICE             0x03
#define NUM_TASK_TYPES          0x04

#define TASK_STATUS_UNKNOWN     0x00
#define TASK_STATUS_OFF         0x01
#define TASK_STATUS_ON          0x02
#define TASK_STATUS_DISABLED    0x03
#define TASK_STATUS_ENABLED     0x04
#define TASK_STATUS_INACTIVE    0x05
#define TASK_STATUS_ACTIVE      0x06
#define TASK_STATUS_WAITING     0x07
#define TASK_STATUS_DOSING      0x08
#define NUM_STATUS_STRINGS      0x09

struct schedule_item_t {
    uint8_t                 id;
    char                    *name;
    uint8_t                 type;               /* TASK_X */

    struct schedule_t       *schedule;
    void                    *action_ptr;        /* pointer to the action to take */
    struct environment_t    *parent;
};

struct task_setpoint_t {
    uint8_t                 status;
    float                   setpoint;
    struct controller_t     *controller;        /* the controller the task is assigned to */
};

struct task_device_t {
    uint8_t                 status;
    uint8_t                 inactive_state;     /* state when task is inactive but enabled */
    uint8_t                 disabled_state;     /* state when task is disabled */
    struct device_t         *device;            /* the device the task is assigned to */
};

 struct time_hhmmss {
     uint8_t                hh;                 /* 0..23 */
     uint8_t                mm;                 /* 0..59 */
     uint8_t                ss;                 /* 0..59 */
 };
 
struct schedule_t {
    struct time_hhmmss      *on_period;         /* hh:mm:ss */
    struct time_hhmmss      *off_period;        /* hh:mm:ss */  
    struct time_hhmmss      *start_time;        /* hh:mm:ss */
    struct time_hhmmss      *stop_time;         /* hh:mm:ss */
    struct time_hhmmss      *run_time;          /* hh:mm:ss */
    struct tm               *enable_datetime;   /* time/date from time.h why not make own?*/
    struct tm               *disable_datetime;  /* time/date from time.h */
};

int8_t task_assign(struct environment_t* environment, struct schedule_item_t *task);
int8_t task_attach(struct schedule_item_t *task);
int8_t task_add_setpoint(struct schedule_item_t *task, float setpoint);
int8_t task_add_controller(struct schedule_item_t *task, struct controller_t *controller);
int8_t task_add_device(struct schedule_item_t *task, struct device_t *device);
int8_t task_create(struct doser_t *doser, char *name, uint8_t type, void *action);
int8_t task_set_schedule(struct schedule_item_t *task,
                         struct time_hhmmss *on_period, struct time_hhmmss *off_period,
                         struct time_hhmmss *start_time, struct time_hhmmss *stop_time,
                         struct tm *enable_datetime, struct tm *disable_datetime);
int32_t time_seconds(struct time_hhmmss *time);
int8_t check_time_between(struct time_hhmmss *time_checked,
                           struct time_hhmmss *time_start,
                           struct time_hhmmss *time_stop);
int8_t task_is_enabled(struct doser_t *doser, struct schedule_item_t *task);
int8_t task_is_active(struct doser_t *doser, struct schedule_item_t *task);