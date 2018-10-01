#define NRSENSORS 10

enum messageStatus 
{
  msBUSY = 1,
  msDONE
};

enum sensorInfo
{
  siOUTPUT = 0,
  siA,
  siB
};

volatile int countedPulses = 0;
int S1Value;
int S2Value;
int S3Value;
String inputString = "";
bool stringComplete = false;
bool initComplete = false;
bool activatedSensors[NRSENSORS] = {false, false, false, false, false, false, false, false, false, false};
float sensorInformation[3][NRSENSORS] = {};

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
} // setup

// the loop routine runs over and over again forever:
void loop() 
{ 
  if(initComplete)
  {
      String string;
      // read the input on analog pin 0:
      S1Value = analogRead(A0);
      S2Value = analogRead(A1);
      S3Value = analogRead(A2);
      
      // Send the sensor values through the serial connection.
      string = String(S1Value) + " " + String(S2Value) + " " + String(S3Value) + "\r\n";
      // Last comma is needed to seperate the /r/n from last sensor value.
      Serial.print(string);
  }
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
    
    if(stringComplete)
    {
      // A string, ending with a new line has been read and needs decoding.
      
      //Serial.println(inputString);
      String sensor = inputString.substring(0, inputString.indexOf(' '));
      //Serial.println(sensor);
      inputString.remove(0, inputString.indexOf(' ')+1);
      //Serial.println(inputString);
      char enabled = inputString[0];
      //Serial.println(enabled);
      inputString.remove(0, inputString.indexOf(' ')+1);
      //Serial.println(inputString);

      // The first character indicates if information about an analogue or digital sensor is received.
      // The second character will indicate which sensor number.
      // The third character must be a space for correct behaviour.
      // Enabled must be '1' for an enabled sensor, any other value will result in the sensor being disabled.
      if(sensor[0] == 'A')
      {
        if(enabled == '1')
        {
          // If an analog sensor is enabled, 3 values are send.
          sensorInformation[siOUTPUT][(int)((sensor[1]-'0')-1)] = (inputString.substring(0, inputString.indexOf(' '))).toFloat();
          inputString.remove(0, inputString.indexOf(' ')+1);
          sensorInformation[siA][(int)((sensor[1]-'0')-1)] = (inputString.substring(0, inputString.indexOf(' '))).toFloat();
          inputString.remove(0, inputString.indexOf(' ')+1);
          //Serial.println(inputString);
          sensorInformation[siB][(int)((sensor[1]-'0')-1)] = inputString.toFloat();
          //Serial.println(sensorInformation[siB][(int)((sensor[1]-'0')-1)]);
          inputString = "";
          stringComplete = false;
        }
        else
        {
          // If the sensor is disabled, it doesn't matter what values are sent; they are ignored. 
          sensorInformation[siOUTPUT][(int)((sensor[1]-'0')-1)] = -1;
          sensorInformation[siA][(int)((sensor[1]-'0')-1)] = -1;
          sensorInformation[siB][(int)((sensor[1]-'0')-1)] = -1;
          inputString = "";
          stringComplete = false;
        }
        /*Serial.println("Sensor: " + sensor + ", is " + String(enabled) + ", val: " + String(sensorInformation[siOUTPUT][(int)((sensor[1]-'0')-1)]) 
                + ", aVal: " + String(sensorInformation[siA][(int)((sensor[1]-'0')-1)]) + ", bVal: " + String(sensorInformation[siB][(int)((sensor[1]-'0')-1)]));*/
      }
      else if (sensor[0] == 'D')
      {
        if(enabled == '1')
        {
          // If a digital sensor is enabled, one value is expected. Other values will simply be ignored.
          sensorInformation[siOUTPUT][(int)((sensor[1]-'0')-1+5)] = (inputString.substring(0, inputString.indexOf(' '))).toFloat();
          sensorInformation[siA][(int)((sensor[1]-'0')-1+5)] = -1;
          sensorInformation[siB][(int)((sensor[1]-'0')-1+5)] = -1;
          inputString.remove(0, inputString.indexOf(' ')+1);
          inputString = "";
          stringComplete = false;
        }
        else
        {
          // If a digital sensor is disabled, it doesn't matter
          sensorInformation[siOUTPUT][(int)((sensor[1]-'0')-1+5)] = -1;
          sensorInformation[siA][(int)((sensor[1]-'0')-1+5)] = -1;
          sensorInformation[siB][(int)((sensor[1]-'0')-1+5)] = -1;
          inputString = "";
          stringComplete = false;
        }
        /*Serial.println("Sensor: " + sensor + ", is " + String(enabled) + ", val: " + String(sensorInformation[siOUTPUT][(int)((sensor[1]-'0')-1+5)]) 
                + ", aVal: " + String(sensorInformation[siA][(int)((sensor[1]-'0')-1+5)]) + ", bVal: " + String(sensorInformation[siB][(int)((sensor[1]-'0')-1+5)])); */
      }
      else if (sensor[0] == 'Q')
      {
        //Serial.println("Yup, im done");
        initComplete = true;
      }
      else
      {
        // If the data starts weird, information is ignored.
        //Serial.println("Error: something went wrong." + sensor + " " + enabled);
        inputString = "";
        stringComplete = false;
      }
  } // if
} // serialEvent()

void serialEventRun(void) {
  if (Serial.available()) serialEvent();
} 
