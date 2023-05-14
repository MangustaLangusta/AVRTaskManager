//avr_task_manager.c

static AVRTime time;

unsigned char InitTaskManager(AVRTaskManager &task_manager){
	//Инициализация полей
	task_manager.root_element = NULL;
	//Инициализация таймера-счетчика
	EnableTimer()
	//Разрешение глобальных прерываний
	sei();
	return 1;
}

unsigned char CreateTask(AVRTaskManager &task_manager, AVRTaskID new_ID, const void* new_handler, AVRTaskPeriod new_period){
	AVRTasksListElement* new_task_list_element;
	AVRTask new_task;

	new_task.ID = new_ID;
	new_task.period = new_period;
	new_task.handler = new_handler;

	new_task_list_element = malloc(AVRTasksListElement);
	if(new_task_list_element == NULL)
		return 0;
	new_task_list_element->task = new_task;
	new_task_list_element->next = NULL;

	GetLastElement(task_manager)->next = new_task_list_element;
	return 1;
}

unsigned char KillTask(AVRTaskManager &task_manager, AVRTaskID target_ID){
	AVRTasksListElement* this_element = task_manager.root_element;
	AVRTasksListElement* prev_element = NULL;
	if(task_manager.root_element->task.ID == target_ID){
		task_manager.root_element = root_element->next;
		free(this_element);
		return 1;
	}
	while(this_element != NULL){
		if(this_element->task.ID != target_ID){
			prev_element = this_element;
			this_element = this_element->next;
		}
		else{
			prev_element->next = this_element->next;
			free(this_element);
			return 1;
		}
	}
	return 0;
}

void DoTasks(AVRTaskManager &task_manager){
	AVRTasksListElement* this_element = task_manager.root_element;
	//проход по всем задачам
	while(this_element != NULL){
		//если пришло время выполнения очередной задачи,
		//вызов обработчика задачи и установка времени следующего выполнения
		if(this_element->task.next_execution_time < time){
			this_element->task.next_execution_time = time + this_element.task.period;
			this_element->task.handler();
		}
		//переход на следующую задачу
		this_element = this_element->next;
	}
	//Обнуление счетчика времени в случае, если его значение достигло половины доступного макс значения
	ReduceTimeCounter(task_manager);
}

AVRTasksListElement* GetLastElement(const AVRTaskManager &task_manager){
	AVRTasksListElement* current_element;
	current_element = task_manager.root_element;
	if(current_element == NULL)
		return NULL;
	while(current_element->next != NULL)
		current_element = current_element->next;
	return current_element;
}

//обработчик прерывания таймера
//срабатывает 1 раз/мс
ISR(TIMER2_COMP_vect){
	time++;
}

//в данный момент только для таймера 2
void EnableTimer()
{
	OCR2=(float)F_CPU/1000/64+0.5;							//Расчет значения регистра сравнения (прерывание происходит 1 раз/мс)
	TIMSK|=(1<<OCIE2);													//Разрешить прерывание по переполнению 2 таймера
	TCCR2=(1<<WGM21)|(1<<CS21)|(1<<CS20);				//Запуск таймера 2 в режиме "Сброс при совпадении". Коэффициент деления 64.
}

void ReduceTimeCounter(AVRTaskManager &task_manager){
	//Если глобальный счетчик времени все еще меньше половины доступного значения, выход из функции
	if( !(time >> (sizeof(AVRTime) >> 1)) )
		return;

	//проход по всем задачам
	AVRTasksListElement* this_element = task_manager.root_element;
	while(this_element != NULL){
		//уменьшение времени выполнения задачи на текущее значение time
		if(this_element->task.next_execution_time <= time)
			this_element->task.next_execution_time = 0;
		else
			this_element->task.next_execution_time -= time;
		//переход на следующую задачу
		this_element = this_element->next;
	}
	//Обнуление глобального счетчика времени в конце
	time = 0;
}
