/*
 * task.h
 *
 */

#ifndef TASK_H_
#define TASK_H_

typedef enum{
	TASK_0=0,
    TASK_1,
    TASK_NUM
}EnumTask;

typedef struct{
    unsigned int TaskDelay;
	void (*TaskPtr) (void);
}StructTask;

typedef struct{
	unsigned int TaskDelayTick;
}RunTimeTask;

extern void TaskStart(void);
extern void SystemTick_ISR_100mS(void);

#endif /* TASK_H_ */
