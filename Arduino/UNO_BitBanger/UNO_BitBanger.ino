String inputString = "";
bool stringComplete = false;
int Pmin = 100;
unsigned long waitTimeMicros = 5000;
int pin = 13;

void setup() {
  // put your setup code here, to run once:
  pinMode(pin, OUTPUT);
  digitalWrite(pin, HIGH);
  Serial.begin(9600);
}

void loop() {
  // put your main code here, to run repeatedly:
  //Serial.println("test");
  digitalWrite(pin, HIGH);
  delayMicroseconds(waitTimeMicros);
  digitalWrite(pin, LOW);
  delayMicroseconds(waitTimeMicros);
}

void serialEvent()
{
  while(Serial.available())
  {
    char inChar = (char)(Serial.read());

    if (inChar == '\n')
    {
      stringComplete = true;
    }
    else
    {
      inputString += inChar;
    }
  }
  //Serial.println(inputString);
  if (stringComplete)
  {
    Pmin = inputString.toInt();
    float hz = ((float)(Pmin))/60.0;
    waitTimeMicros = 1000000 * (1.0/(2.0*(float)(hz)));
    stringComplete = false;
    inputString = "";
    Serial.println(waitTimeMicros);
  }
}

void serialEventRun(void){
  if(Serial.available()) serialEvent();
}
