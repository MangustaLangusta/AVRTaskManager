#ifndef AVR_TASK_MANAGER_H
#define AVR_TASK_MANAGER_H


#ifndef F_CPU
#define F_CPU 16000000UL
#endif	//F_CPU

#ifndef AVR_TASK_MANAGER_TIMER_COUNTER
#define AVR_TASK_MANAGER_TIMER_COUNTER 2
#endif	//AVR_TASK_MANAGER_TIMER_COUNTER

#include <stddef.h>
#include <stdlib.h>

//Библиотеки для atmega128 и NLcon1AT2 (там так же используется atmega128)
#include <avr/io.h>															//Стандартная библиотека ввода/вывода
#include <avr/interrupt.h>											//библиотека прерываний

//=========================== Объявление типов и структур =================================

typedef unsigned int AVRTaskID;
typedef unsigned long AVRTaskPeriod;
typedef unsigned long long AVRTime;

struct AVRTask{
	AVRTaskID ID;
	AVRTaskPeriod period;
	AVRTime next_execution_time;
	void (*handler) (void);
};

struct AVRTasksListElement{
	struct AVRTask task;
	struct AVRTasksListElement *next;
};

struct AVRTaskManager{
	struct AVRTasksListElement *root_element;	//указатель на первый элемент в списке задач
};

//=========================== Публичные методы =========================================

unsigned char InitTaskManager(struct AVRTaskManager *task_manager);

unsigned char CreateTask(struct AVRTaskManager *task_manager, AVRTaskID new_ID, const void* new_handler, AVRTaskPeriod new_period);

unsigned char KillTask(struct AVRTaskManager *task_manager, AVRTaskID task_ID);

void DoTasks(struct AVRTaskManager *task_manager);

//============================ Служебные методы ============================================

struct AVRTasksListElement* GetLastElement(const struct AVRTaskManager *task_manager);

//Инициализация таймера
//В данный момент реализовано только для таймера 2
void EnableTimer();

//Обнуляет счетчик времени и все счетчики задач в случае достижения счетчиком половины
//от максимально возможного значения
void ReduceTimeCounter(struct AVRTaskManager *task_manager);

#endif	//AVR_TASK_MANAGER_H
