enum timerMode {
  INTERVAL = 1,   // переодический вызов
  TIMEOUT = 2,    // вызовит функцию 1 раз, через определённое время
};

class MTask {
  public:
    MTask(char *name) { // создаёт объект, принемает имя процесса
      strcpy(taskName, name);
      state = false;
    }
    void start() {      // позволяет запустить процесс, после остановки
      state = true;
      timer = millis();
    }
    void startTimeOut(uint16_t time = 0) {  // вызвать обработчик через time мс
      mode = TIMEOUT;
      period = (time < minPeriod) ? minPeriod : time;
      MTask::start();
    }
    void startInterval(uint16_t time = 0) { // вызывать обработчик каждые time мс
      mode = INTERVAL;
      period = (time < minPeriod) ? minPeriod : time;
      MTask::start();
    }
    void stop() {   // остановить 
      state = false;
    }
    void attach(void (*handler)()) {  // подключить функцию, которую нужно будет вызывать (будет вызывать функцию)
      callback = *handler;
    }
    bool tick(char *thisNameOut = nullptr) {  // обработчик самого таймера
      if (state && millis() - timer >= period) {
        if (thisNameOut != nullptr) strcpy(thisNameOut, taskName);
        exec();
        if (mode == TIMEOUT) state = false;
        if (thisNameOut != nullptr) *thisNameOut = "";
        timer = millis();
        return true;
      }
      return false;
    }
    bool state;   // состояние задачи
  private:
    char taskName[10] = "";
    timerMode mode;
    uint16_t minPeriod = 1;
    uint16_t period;
    uint32_t timer;

    void (*callback)();

    void exec() {
      if (*callback) callback();
    }
};
