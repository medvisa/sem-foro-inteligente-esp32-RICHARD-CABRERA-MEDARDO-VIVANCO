
const int PIN_ROJO = 25;
const int PIN_AMARILLO = 26;
const int PIN_VERDE = 27;
const int PIN_SENSOR = 14;
enum Estado { VERDE, AMARILLO, ROJO };
volatile Estado est = VERDE;
volatile int contSegundos = 0;
volatile bool vehiculo = false;
hw_timer_t *timer = NULL;

void IRAM_ATTR onTimer() {
  contSegundos++;
  vehiculo = (digitalRead(PIN_SENSOR) == LOW);
  int tiempoverde = vehiculo ? 8 : 5;
  switch (est) {

    case VERDE:
      if (contSegundos >= tiempoverde) {
        est = AMARILLO;
        contSegundos = 0;
      }
      break;

    case AMARILLO:
      if (contSegundos >= 2) {
        est = ROJO;
        contSegundos = 0;
      }
      break;

    case ROJO:
      if (contSegundos >= 5) {
        est = VERDE;
        contSegundos = 0;
      }
      break;
  }
}

void setup() {
  Serial.begin(115200);

  pinMode(PIN_VERDE, OUTPUT);
  pinMode(PIN_AMARILLO, OUTPUT);
  pinMode(PIN_ROJO, OUTPUT);
  pinMode(PIN_SENSOR, INPUT_PULLUP);
  analogWrite(PIN_VERDE, 0);
  digitalWrite(PIN_AMARILLO, LOW);
  digitalWrite(PIN_ROJO, LOW);
  timer = timerBegin(1000000);      
  timerAttachInterrupt(timer, &onTimer);
  timerAlarm(timer, 1000000, true, 0); 
}

void loop() {
  actualizarSalidas();
}

void actualizarSalidas() {

  switch (est) {

    case VERDE:
      analogWrite(PIN_VERDE, vehiculo ? 255 : 127);
      digitalWrite(PIN_AMARILLO, LOW);
      digitalWrite(PIN_ROJO, LOW);
      break;

    case AMARILLO:
      analogWrite(PIN_VERDE, 0);
      digitalWrite(PIN_AMARILLO, HIGH);
      digitalWrite(PIN_ROJO, LOW);
      break;

    case ROJO:
      analogWrite(PIN_VERDE, 0);
      digitalWrite(PIN_AMARILLO, LOW);
      digitalWrite(PIN_ROJO, HIGH);
      break;
  }
}