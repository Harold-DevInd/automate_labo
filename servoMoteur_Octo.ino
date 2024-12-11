int servoGPin = 10;
int octoGPin = 3;
float temps = 1500;
float offset = 0;
bool sens = true;

void setup() {
  pinMode(servoGPin, OUTPUT);
  pinMode(octoGPin, INPUT);
  attachInterrupt(digitalPinToInterrupt(octoGPin), ajustPosition, CHANGE);
  Serial.begin(9600);
}

void loop() 
{
  digitalWrite(servoGPin, HIGH);
  delayMicroseconds(temps + offset);
  digitalWrite(servoGPin, LOW);
  delayMicroseconds(20000);

  Serial.print("Temps de base : ");
  Serial.println(temps);
  Serial.print("Offset ajoute : ");
  Serial.println(offset);
  Serial.print("Sens de rotation : ");
  if (sens)
  {
    Serial.println("Ajout jusqu'a 0.2");
  }
  else
  {
    Serial.println("Retrait jusqu'a -0.2");
  }
  Serial.println("****************************");
}

void ajustPosition()
{
  if(sens == true)
  {
    offset = offset + 10;

    if (offset == 200)
    {
      sens = false;
    }
  }
  else
  {
    offset = offset -10;

    if (offset == -200)
    {
      sens = true;
    }
  }
}
