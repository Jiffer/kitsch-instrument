
typedef struct buttonStruct{
  int pin;
  int state;
  int lastState;
  long lastDebounceTime;
  int bang;
};

buttonStruct readDebounce(buttonStruct button);
