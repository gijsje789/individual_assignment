#define NRSENSORS 10


volatile int countedPulses = 0;
int S1Value;
int S2Value;
int S3Value;
String inputString = "";
bool stringComplete = false;
bool initComplete = false;
bool activatedSensors[10] = {false, false, false, false, false, false, false, false, false, false};
int sensorInformation[3][10] = {};

// the setup routine runs once when you press reset:
void setup() 
{
  // initialize serial communication at 115200 bits per second:
  Serial.begin(115200);
  // Set the analog read resolution to 12-bit.
  analogReadResolution(12); // 12-bit resolution for the ADC

  // reserve 200 bytes for the inputString.
  inputString.reserve(200);
  
  //Attach the interrupt to the digital pin in order to count the sensor pulses.
  attachInterrupt(digitalPinToInterrupt(52), countPulses, RISING);

  while(!initComplete)
  {
    if(stringComplete)
    {
      int sensor = 1;
      for(int i = 0; inputString.length(); i++)
      {
        char tempChar = inputString[i];
        switch(tempChar)
        {
          case '1':
            activatedSensors[sensor-1] = true;
            sensor++;
            break;
          case '0':
            activatedSensors[sensor-1] = false;
            sensor++;
            break;
          case ',':
            break;
        } // Switch
      } // for
    } // if
  } // while
} // setup

// the loop routine runs over and over again forever:
void loop() 
{ 
    String string;
    // read the input on analog pin 0:
    S1Value = analogRead(A0);
    S2Value = analogRead(A1);
    S3Value = analogRead(A2);
    
    // Send the sensor values through the serial connection.
    string = String(S1Value) + " " + String(S2Value) + " " + String(S3Value) + "\r\n";
    // Last comma is needed to seperate the /r/n from last sensor value.
    // Serial.print(string);

    // Necessary for the live plot to update without crashing.
    delay(10);
}

void countPulses() 
{
  countedPulses++;
}

void serialEvent()
{
  while(Serial.available())
  {
    // Get new byte.
    char inChar = (char)Serial.read();

    // If the incoming character is a newline, set a flag.
    // Otherwise add it to the input string.
    if (inChar == '\n')
    {
      stringComplete = true;
    }
    else
    {
      inputString += inChar;
    }
  } // while
} // serialEvent()
