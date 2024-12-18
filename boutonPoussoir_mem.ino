#define PIN_BP 3
#define PIN_L 13

int currentState = 0;
int previousState = 0;
int lampState = 0;

void Inter();

void setup() {
  // put your setup code here, to run once:
  pinMode(PIN_BP, INPUT);
  pinMode(PIN_L, OUTPUT);
  //attachInterrupt(digitalPinToInterrupt(PIN_BP), Inter, RISING);
}

void loop() {
  currentState = digitalRead(PIN_BP);

  if (currentState == 1 && previousState == 0) {
     lampState = !lampState;
     digitalWrite(PIN_L, lampState);

  }

  previousState = currentState;
}

void Inter()
{
  lampState = !lampState;
  digitalWrite(PIN_L, lampState);
}