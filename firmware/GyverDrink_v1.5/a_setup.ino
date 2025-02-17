void setup() {
  
#if (DEBUG_UART == 1)
  Serial.begin(9600);
  DEBUG("start");
#endif

  // епром
  if (EEPROM.read(1000) != 10) {
    EEPROM.write(1000, 10);
    EEPROM.put(0, thisVolume);
  }
  EEPROM.get(0, thisVolume);

  Timer2.setPeriod(DISP_PERIOD);  // Заводим Timer 2 на прерывания с нужным периодом
  Timer2.enableISR();             // Включаем прерывания Timer 2
  DEBUG("timer2 init");

  // тыкаем ленту
  strip.setBrightness(130);
  strip.clear();
  strip.show();
  DEBUG("strip init");

  // настройка пинов
  pinMode(PUMP_POWER, OUTPUT);
  pinMode(SERVO_POWER, OUTPUT);
  for (byte i = 0; i < CUM_SHOTS; i++) {
    if (SWITCH_LEVEL == 0) pinMode(SW_pins[i], INPUT_PULLUP);
  }

  // старт дисплея
  disp.clear();
  // disp.brightnessDepth(7); // DA_K
  DEBUG("disp init");

  // настройка серво
  servoON();
  servo.attach(SERVO_PIN, 600, 2400);
  if (INVERSE_SERVO) servo.setDirection(REVERSE);
   
  servo.write(0);
  delay(800);
  servo.setTargetDeg(0);
  servo.setSpeed(60);
  servo.setAccel(0.3);
  servoOFF();

  serviceMode();    // калибровка
  dispMode();       // выводим на дисплей стандартные значения
  timeoutReset();   // сброс таймаута
  TIMEOUTtimer.start();
}
