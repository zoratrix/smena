
int message = 0;//в дальнейшем будет использована для считывания положентя переключателя
// следующие переменные имеют тип long, так как время измеряется в миллисекундах
// и значение в переменных, возможно, будет превышать максимальный размер переменной типа int 
unsigned long TASK_TIME = 10000; // для проверки задаем время работы = 10 секундам 
unsigned long BREAK_TIME = 5000; // для проверки задаем время перерыва = 5 секунд
unsigned long taskDelayStart = 0; // момент времени, в который начинается отсчет (работа)
unsigned long breakDelayStart = 0; // момент времени, в который начинается отсчет (перерыв)
bool taskDelayRunning = false; // = true, если отсчет времени все еще идет (работа)
bool breakDelayRunning = false; // = true, если отсчет времени все еще идет (перерыв)
bool taskLightOn = false; // следим за состоянием светодиода

#define taskLampPin 13 //назначаем пин первому светодиоду (работа)
#define breakLampPin 12 //назначаем пин второму светодиоду (перерыв)
#define triggerPin 4 //назначаем пин переключателю

void setup() {
  pinMode(triggerPin, INPUT_PULLUP); //настраеваем на вход пин переключателя 
                                    //со встроенным подтягивающим резистором
  pinMode(taskLampPin, OUTPUT); //настраевам пин первого светодиода на выход       
  digitalWrite(taskLampPin, LOW);//при включении или перезагрузке первый светодиод выключен
  pinMode(breakLampPin, OUTPUT);//настраевам пин второго светодиода на выход         
  digitalWrite(breakLampPin, LOW);//при включении или перезагрузке второй светодиод выключен

  // подключаем монитор порта, чтобы в дальнейшем отслеживать в нем время
  Serial.begin(9600);
}

void loop() {
  message = digitalRead(triggerPin);// загружаем в переменную message состояние переключаетеля
  if (message == 0){ //если переключатель находится в правом положении (выкл), 
                    //то есть работа (задача) еще не выполнена
    if (!taskLightOn && !taskDelayRunning & !breakDelayRunning){ // если светодиоды выкл. 
                                                                // и ни один таймер еще не начал работать
      taskDelayStart = millis(); // фиксируем время начала работы
      taskDelayRunning = true;   // включаем переменную, сигнализирующую о том, что отсчет времени (работа) пошел

      digitalWrite(taskLampPin, HIGH);         //включаем светодиод 1
      taskLightOn = true;// записываем в переменную, что светодиод 1 включен
      Serial.println("New task: begin work...");//  выводим в монитор порта текст
    }

    // проверяем, вышло ли время для работы   
    if (taskLightOn && taskDelayRunning && ((millis() - taskDelayStart) >= TASK_TIME)) { 
      taskDelayStart += TASK_TIME; // защита от "проскока" нужного времени
      taskDelayRunning = false;//выключаем таймер рабочего времени
      Serial.println("time has elapsed,  take a break");//выводим в монитор порта текст
      digitalWrite(breakLampPin, HIGH);         //включаем сведодиод 2
      digitalWrite(taskLampPin, LOW);          //выключаем светодиод 1
      taskLightOn = false;//записываем в переменную, что светодиод 1 выключен

       breakDelayStart = millis(); // фиксируем время начала перерыва 
       breakDelayRunning = true;// включаем переменную, сигнализирующую о том, что отсчет времени (перерыв) пошел
     }
     
    // проверяем, закончилось ли время переыва
    if (!taskLightOn && breakDelayRunning && ((millis() - breakDelayStart) >= BREAK_TIME)) { 
      breakDelayStart += BREAK_TIME; // защита от "проскока" нужного времени
      breakDelayRunning = false;  //выключаем таймер времени перерыва
      Serial.println("break time is over, back to work"); //выводим в монитор порта текст
      taskDelayRunning = true;  //включаем таймер рабочего времени
      taskDelayStart = millis();  // снова фиксируем время начала работы
    digitalWrite(breakLampPin, LOW);         // выключаем светодиод 2
      digitalWrite(taskLampPin, HIGH);         // включаем светодиод 1
      taskLightOn = true; // записываем в перемнную, что светодиод 1 горит
    }
  }
      
    if (message == 1){ //если работа выполнена, то выключаем все и ждем действий пользователя
      breakDelayRunning = false; //записываем, что оба таймера выключены
      taskDelayRunning = false;
      if (taskLightOn){ //если светодиод 1 в этот момент горел, значит пользователь выключил 
                        //устройство до конца отсчета рабочего времени, значит задача выполнена раньше
      Serial.println("task complete"); //то выводим в монитор порта сообщение об окончании работы
      digitalWrite(taskLampPin, LOW);          //выключаем оба светодиода
      digitalWrite(breakLampPin, LOW);  
      taskLightOn = false;//выключаем переменную, сообщающую о включенном светодиоде 1
          
      }
     Serial.println(" waiting for task"); //выводим в монитор порта сообщение об ожидании
     }
}