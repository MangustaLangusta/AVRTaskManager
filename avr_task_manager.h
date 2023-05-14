#ifndef AVR_TASK_MANAGER_H
#	define AVR_TASK_MANAGER_H
#endif //AVR_TASK_MANAGER_H

#ifndef F_CPU
#	error f_cpu not defined
#endif	//F_CPU

#ifndef AVR_TASK_MANAGER_TIMER_COUNTER
#	error timer counter for AVRTaskManager not defined
#endif	//AVR_TASK_MANAGER_TIMER_COUNTER

//Библиотеки для atmega128 и NLcon1AT2 (там так же используется atmega128)
#include <avr/io.h>															//Стандартная библиотека ввода/вывода
#include <avr/interrupt.h>											//библиотека прерываний

//=========================== Объявление типов и структур =================================

typedef AVRTaskID unsigned int;
typedef AVRTaskPeriod unsigned long;
typedef AVRTime unsigned long long

struct AVRTask{
	AVRTaskID ID;
	AVRTaskPeriod period;
	AVRTime next_execution_time;
	void (*handler) (void);
};

struct AVRTasksListElement{
	AVRTask task;
	AVRTasksListElement *next;
};

struct AVRTaskManager{
	AVRTasksListElement *root_element;	//указатель на первый элемент в списке задач
};

//=========================== Публичные методы =========================================

unsigned char InitTaskManager(AVRTaskManager &task_manager);

unsigned char CreateTask(AVRTaskManager &task_manager, AVRTaskID new_ID, const void* new_handler, AVRTaskPeriod new_period);

unsigned char KillTask(AVRTaskManager &task_manager, AVRTaskID task_ID);

void DoTasks(AVRTaskManager &task_manager);

//============================ Служебные методы ============================================

AVRTasksListElement* GetLastElement(const AVRTaskManager &task_manager);

//Инициализация таймера
//В данный момент реализовано только для таймера 2
unsigned char EnableTimer();

//Обнуляет счетчик времени и все счетчики задач в случае достижения счетчиком половины
//от максимально возможного значения
void ReduceTimeCounter(AVRTaskManager &task_manager);

#endif	//AVR_TASK_MANAGER_H
