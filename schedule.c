#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

#include "schedule.h"
#include "doser_system.h"
#include "environment.h"
#include "device.h"
#include "controller.h"
#include "ph_controller.h"
#include "ec_controller.h"

int8_t task_assign(struct environment_t* environment, struct schedule_item_t *task) {
    struct schedule_item_t **tasks_ptr;
    
    if (environment == NULL) {
        printf("error: environment is missing\n");
        return -1;
    } else if (task == NULL) {
        printf("error: task is missing\n");
        return -1;
    }

    tasks_ptr = realloc(environment->tasks, sizeof(struct schedule_item_t *) * (environment->num_tasks + 1));
    if(tasks_ptr == NULL) {
        printf("error: realloc environment->tasks\n");
        return -1;
    }
    environment->tasks = tasks_ptr;
    
    task->parent = environment;
    
    environment->tasks[environment->num_tasks] = task;
    environment->num_tasks++;
    return environment->num_tasks;
}

int8_t task_attach(struct schedule_item_t *task) {
    struct schedule_item_t **tasks_ptr;
    uint8_t ret = 0;
    
    if (task == NULL) {
        printf("error: task is missing\n");
        return -1;
    }
    
    if (task->type == TASK_DEVICE) {
        struct device_t *dev_ptr;
        struct task_device_t *ptr_task;
        
        ptr_task = (struct task_device_t *) task->action_ptr;
        dev_ptr = ptr_task->device;
        
        tasks_ptr = realloc(dev_ptr->attached_tasks,
                            sizeof(struct schedule_item_t *) * (dev_ptr->num_attached_tasks + 1));
        if (tasks_ptr == NULL) {
            printf("error: realloc tasks_ptr\n");
            return -1;
        }
        dev_ptr->attached_tasks = tasks_ptr;
        dev_ptr->attached_tasks[dev_ptr->num_attached_tasks] = task;
        dev_ptr->num_attached_tasks++;
        ret = dev_ptr->num_attached_tasks;
        
        printf("Task: %s attached to %s. Total %02u tasks.\n",
               task->name, dev_ptr->name, dev_ptr->num_attached_tasks);             
    } else if (task->type == TASK_PH_SETPOINT) {
        struct controller_t *dev_ptr;
        struct task_setpoint_t *ptr_task;

        ptr_task = (struct task_setpoint_t *) task->action_ptr;
        dev_ptr = ptr_task->controller;
        
        tasks_ptr = realloc(dev_ptr->attached_tasks,
                            sizeof(struct schedule_item_t *) * (dev_ptr->num_attached_tasks + 1));
        if (tasks_ptr == NULL) {
            printf("error: realloc tasks_ptr\n");
            return -1;
        }
        dev_ptr->attached_tasks = tasks_ptr;
        dev_ptr->attached_tasks[dev_ptr->num_attached_tasks] = task;
        dev_ptr->num_attached_tasks++;
        ret = dev_ptr->num_attached_tasks;
        
        printf("Task: %s attached to %s. Total %02u tasks.\n",
               task->name, dev_ptr->name, dev_ptr->num_attached_tasks);     
    } else if (task->type == TASK_EC_SETPOINT) {
        struct controller_t *dev_ptr;
        struct task_setpoint_t *ptr_task;

        ptr_task = (struct task_setpoint_t *) task->action_ptr;
        dev_ptr = ptr_task->controller;

        tasks_ptr = realloc(dev_ptr->attached_tasks,
                            sizeof(struct schedule_item_t *) * (dev_ptr->num_attached_tasks + 1));
        if (tasks_ptr == NULL) {
            printf("error: realloc tasks_ptr\n");
            return -1;
        }
        dev_ptr->attached_tasks = tasks_ptr;
        dev_ptr->attached_tasks[dev_ptr->num_attached_tasks] = task;    
        dev_ptr->num_attached_tasks++;
        ret = dev_ptr->num_attached_tasks;
        
        printf("Task: %s attached to %s. Total %02u tasks.\n",
               task->name, dev_ptr->name, dev_ptr->num_attached_tasks);
    }
    
    return ret;
}

int8_t task_add_setpoint(struct schedule_item_t *task, float setpoint) {
    if (task == NULL) {
        printf("error: task is missing\n");
        return -1;
    } else if ((task->type != TASK_PH_SETPOINT) && (task->type != TASK_EC_SETPOINT)) {
        printf("error: invalid task type\n");
        return -1;
    }
    
    ((struct task_setpoint_t *) task->action_ptr)->setpoint = setpoint;
    
    printf("Added setpoint %.2f to task %s\n", setpoint, task->name);
    
    return 1;
}

int8_t task_add_controller(struct schedule_item_t *task, struct controller_t *controller) {
    if (task == NULL) {
        printf("error: task is missing\n");
        return -1;
    } else if (controller == NULL) {
        printf("error: controller is missing\n");
        return -1;
    }
    
    struct task_setpoint_t *task_ptr;
    task_ptr = (struct task_setpoint_t *) task->action_ptr;
    task_ptr->controller = controller;
    task_ptr->status = TASK_STATUS_UNKNOWN; 
    
    printf("Added controller %s to task %s\n", controller->name, task->name);
    
    return 1;   
}

int8_t task_add_device(struct schedule_item_t *task, struct device_t *device) {
    if (task == NULL) {
        return -1;
    } else if (device == NULL) {
        return -1;
    }
    
    struct task_device_t *task_ptr;
    task_ptr = (struct task_device_t *) task->action_ptr;
    task_ptr->device = device;
    task_ptr->status = TASK_STATUS_UNKNOWN;
    
    printf("Added device %s to task %s\n", device->name, task->name);
    
    return 1;
}

int8_t task_create(struct doser_t *doser, char *name, uint8_t type, void *action) {
    struct schedule_item_t **tmp_ptr;
    struct schedule_item_t *task;
    
    if (doser == NULL) {
        printf("error: environment is missing\n");
        return -1;
    } else if (name == NULL) {
        printf("error: name is missing\n");
        return -1;
    } else if (strlen(name) < 1) {
        printf("error: name is empty\n");
        return -1;
    } else if ((type < 0) || (type > NUM_TASK_TYPES)) {
        printf("error: invalid task type\n");
        return -1;
    } else if (action == NULL) {
        printf("error: action is missing\n");
        return -1;
    }

    tmp_ptr = realloc(doser->tasks, sizeof(struct schedule_item_t *) * (doser->num_tasks + 1));
    if (tmp_ptr == NULL) {
        printf("error: realloc doser->tasks\n");
        return -1;
    }
    doser->tasks = tmp_ptr;
    
    task = malloc(sizeof(struct schedule_item_t));
    if (task == NULL) {
        printf("error: malloc task\n");
        return -1;
    }
    
    task->name = malloc(sizeof(char) * (strlen(name) + 1));
    if (task->name == NULL) {
        printf("error: malloc task->name\n");
        return -1;
    }
    strcpy(task->name, name);
    task->name[strlen(task->name)] = '\0';
    
    if (type == TASK_DEVICE) {
        task->action_ptr = malloc(sizeof(struct task_device_t));
        if (task->action_ptr == NULL) {
            printf("error: malloc task->action_ptr\n");
            return -1;
        }
        task_add_device(task, action);
    } else if (type == TASK_PH_SETPOINT) {
        task->action_ptr = malloc(sizeof(struct task_setpoint_t));
        if (task->action_ptr == NULL) {
            printf("error: malloc task->action_ptr\n");
            return -1;
        }
        task_add_controller(task, action);
    } else if (type == TASK_EC_SETPOINT) {
        task->action_ptr = malloc(sizeof(struct task_setpoint_t));
        if (task->action_ptr == NULL) {
            printf("error: malloc task->action_ptr\n");
            return -1;
        }
        task_add_controller(task, action);
    }
    
    task->type = type;
    
    task_attach(task);
    
    task->schedule = malloc(sizeof(struct schedule_t));
    if (task->schedule == NULL) {
        printf("error: malloc task->schedule\n");
        return -1;
    }   
    doser->tasks[doser->num_tasks] = task;
    doser->num_tasks++;
    
    return doser->num_tasks;
}

int8_t task_set_schedule(struct schedule_item_t *task,
                         struct time_hhmmss *on_period, struct time_hhmmss *off_period,
                         struct time_hhmmss *start_time, struct time_hhmmss *stop_time,
                         struct tm *enable_datetime, struct tm *disable_datetime) {
                              
    if (task == NULL) {
        printf("error: task is missing\n");
        return -1;
    } else if (on_period == NULL) {
        printf("error: on_period is missing\n");
        return -1;
    } else if (off_period == NULL) {
        printf("error: off_period is missing\n");
        return -1;
    } else if (start_time == NULL) {
        printf("error: start_time is missing\n");
        return -1;
    } else if (stop_time == NULL) {
        printf("error: stop_time is missing\n");
        return -1;
    } else if (enable_datetime == NULL) {
        printf("error: enable_datetime is missing\n");
        return -1;
    } else if (disable_datetime == NULL) {
        printf("error: disable_datetime is missing\n");
        return -1;
    }
    
    task->schedule->on_period = on_period;
    task->schedule->off_period = off_period;
    task->schedule->start_time = start_time;
    task->schedule->stop_time = stop_time;
    task->schedule->run_time = malloc(sizeof(struct time_hhmmss));
    if (task->schedule->run_time == NULL) {
        printf("error: malloc task->schedule->run_time\n");
        return -1;
    }
    task->schedule->run_time->hh = 0;
    task->schedule->run_time->mm = 0;
    task->schedule->run_time->ss = 0;
    task->schedule->enable_datetime = enable_datetime;
    task->schedule->disable_datetime = disable_datetime;
    
    return 1;
}

int32_t time_seconds(struct time_hhmmss *time) {
    int32_t seconds = 0;

    if (time == NULL) {
        printf("error: time is missing\n");
        return -1;
    }
    
    seconds += time->ss;
    seconds += time->mm * 60;
    seconds += time->hh * 60 * 60;
    
    return seconds;
}

int8_t check_time_between(struct time_hhmmss *time_checked,
                           struct time_hhmmss *time_start,
                           struct time_hhmmss *time_stop) {
    if (time_checked == NULL) {
        printf("error: time_checked is missing\n");
        return -1;
    } else if (time_start == NULL) {
        printf("error: time_start is missing\n");
        return -1;
    } else if (time_stop == NULL) {
        printf("error: time_stop is missing\n");
        return -1;
    }
    
    if (time_seconds(time_start) < time_seconds(time_stop)) {
        if (time_seconds(time_checked) >= time_seconds(time_start)) {
            if (time_seconds(time_checked) < time_seconds(time_stop)) {
                return 1;
            }
        }
    } else if (time_seconds(time_stop) < time_seconds(time_start)) {
        if (time_seconds(time_checked) > time_seconds(time_start)) {
            return 1;
        } else if (time_seconds(time_checked) < time_seconds(time_stop)) {
            return 1;
        }
    }
    
    return 0;
}

int8_t task_is_enabled(struct doser_t *doser, struct schedule_item_t *task) {
/*  if (check_datetime_passed(doser, task->schedule->enable_datetime) == TIME_PASSED) {
        return 1;
    } else {
        return -1;
    }
*/
    return 1;
}

int8_t task_is_active(struct doser_t *doser, struct schedule_item_t *task) {
    if (doser == NULL) {
        printf("error: doser is missing\n");
        return -1;
    } else if (task == NULL) {
        printf("error: task is missing\n");
        return -1;
    }
    
    if ((time_seconds(task->schedule->start_time) == 0) && (time_seconds(task->schedule->stop_time) == 0)) {
        return 1;
    }
    if (check_time_between(doser->timers->system_time,
                           task->schedule->start_time,
                           task->schedule->stop_time) == 1) {
        return 1;
    } else {
        return -1;
    }

    return 1;
}