#include <arduino.h>

const int IN1A = 9; // Pins connectï¿½s au pont en H
const int IN2A = 10;

//Délai pour controle du moteur
unsigned long currentTime;
unsigned long previousTime;

//Délai pour println
unsigned long currentTimeInterrupt;
unsigned long previousTimeInterrupt;

//Temps pour calcul
unsigned long lastTime;
float vTime = 0;

int impulsionA = 0;
int impulsionB = 0;
bool lastImp = 0; //0 A / 1 B
bool sens = 0; //0 << / 1 >>
int lastPos = 0;

struct Moteur_Data{
  float Time; //En ms
  float Vitesse; //En rpm
  int Position; //En Degre
};

struct Moteur_Data MD;

void Init_Motor();    // Initialisation du pont en H
void Control_Motor(); // Mise en mouvement du moteur
float calculVitesse();
float calculTime();

/**********************************A NE PAS MODIFIER ! *********************************/
void Init_Motor()
{
  pinMode(IN1A, OUTPUT);
  pinMode(IN2A, OUTPUT);

  digitalWrite(IN1A, LOW);
  digitalWrite(IN2A, LOW);
}

void Control_Motor()
{
  static int i = 0;
  static unsigned char Speed = 0;
  static unsigned char pin = IN1A;
  static bool stop = false;

  if (!stop)
  {
    if (!(i % 5))
    {
      if (i < 60)
      {
        Speed += 20;
        analogWrite(pin, Speed);
      }
      else
      {
        if (Speed != 0)
          Speed -= 20;
        analogWrite(pin, Speed);
      }
    }
    if (i == 120)
    {
      digitalWrite(pin, LOW);
      if (pin == IN2A)
        stop = true;

      pin = IN2A;
      i = 0;
    }
    else
      i++;
  }
  /*Serial.print(myMotor.Speed / 1.275);
  Serial.print(" ---------- ");
  //Serial.print(myMotor.Position);
  Serial.print("\n");*/
}

/****************************************************************************************/

void AcquisitionComputing()
{
  if (lastImp == 0) 
  {
    sens = 0;
  }

  lastImp = 0;
  impulsionA++;

  if (sens)
    MD.Position--; //<
  else
    MD.Position++; //>

  MD.Time = calculTime();
  /*MD.Vitesse = calculVitesse();*/
}

void AcquisitionComputing2() {
  if (lastImp == 1) {
    sens = 1;
  }
  lastImp = 1;
  impulsionB++; 
}

float calculVitesse() {
  float vitesse;
  int dp;
  unsigned long mil;
  dp = MD.Position - lastPos;
  dp = abs(dp);
  vitesse = dp / vTime;
  lastPos = MD.Position;
  return vitesse;
}

float calculTime() {
  unsigned long mil;
  unsigned long dt;
  float time = 0;

  mil = millis();
  dt = mil - lastTime ;
  lastTime = mil;
  time = (float)dt;

  return time;
}

/****************************************************************************************/

void setup()
{
  Init_Motor();
  Serial.begin(9600);
  pinMode(2, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(2), AcquisitionComputing, CHANGE);
  attachInterrupt(digitalPinToInterrupt(3), AcquisitionComputing2, CHANGE);
  currentTime = millis();
  previousTime = millis();

  lastTime = millis();

  MD.Position = 0;
  MD.Vitesse = 0;
  MD.Time = 0;
  Serial.println("IA;IB;Temps;Vitesse;Position");
}

void loop()
{
  currentTime = millis();
  if (previousTime + 20 <= currentTime) {
    previousTime = currentTime;
    Control_Motor();
  }

  currentTimeInterrupt = millis();
  if (previousTimeInterrupt + 200 == currentTimeInterrupt) {
    previousTimeInterrupt = currentTimeInterrupt;

    vTime = calculTime();
    MD.Vitesse = calculVitesse();

    /*Serial.print("S : ");
    if (sens)
      Serial.print("<");
    else
      Serial.print(">");*/
    
    Serial.print(impulsionA);
    Serial.print(";");
    Serial.print(impulsionB);
    Serial.print(";");
    Serial.print(MD.Time);
    Serial.print(";");
    Serial.print(MD.Vitesse);
    Serial.print(";");
    Serial.println(MD.Position);
  }
}