/*
 * task.c
 *
 */

#include "gpio.h"
#include "task.h"

static void Task1Running(void);
static void Task2Running(void);

StructTask TaskTable[TASK_NUM]={
	{10, Task1Running},             //10: task timeout after 1 second
	{20, Task2Running}              //20: task timeout after 2 secondi
};
RunTimeTask	RTTask[TASK_NUM];

void SystemTick_ISR_100mS(void)
{
	int i;

    //timout set at 100ms
	for(i=0;i<TASK_NUM;i++)
	{
        //reduce task tick, once reach 0 will task will be call
    	if(RTTask[i].TaskDelayTick)
        	RTTask[i].TaskDelayTick--;
	}
}

void Task1Running(void)
{
    GPIOLED0Toggle();
}

void Task2Running(void)
{
    GPIOLED1Toggle();
}

void TaskScheduler(void)
{
	int i;

    while(1)
    {
		for(i=0;i<TASK_NUM;i++)
		{
			if(!RTTask[i].TaskDelayTick)
			{
				//reload task delay for next call
				RTTask[i].TaskDelayTick = TaskTable[i].TaskDelay;
				//call task upon timerup
				(*TaskTable[i].TaskPtr)();
			}
		}
    }
}

void TaskInit(void)
{
	int i;

	//initalise task delay
	for(i=0;i<TASK_NUM;i++)
	{
		RTTask[i].TaskDelayTick = TaskTable[i].TaskDelay;
	}
}

void TaskStart(void)
{
    TaskInit();

    TaskScheduler();
}

