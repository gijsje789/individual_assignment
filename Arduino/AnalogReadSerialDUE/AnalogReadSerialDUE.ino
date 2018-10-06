#define NRSENSORS 10
#define NRPUMPS 4
#define PRECISION 3
#define SCALING 10000
#define AN1 0 // Analogue sensor 1 -> A1 is analog input 1 of arduino.
#define AN2 1
#define AN3 2
#define AN4 3
#define AN5 4
#define D1 5
#define D2 6
#define D3 7
#define D4 8
#define D5 9
#define P1 = 0
#define P2 = 1
#define P3 = 2
#define P4 = 3
#define BIT12ADC 4095
#define BIT10ADC 1024

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

enum pumpInfo
{
  piFLOWRATE = 0,
  piFEEDBACK
};

volatile int countedPulses = 0;
int A1Value;
int A2Value;
int A3Value;
int A4Value;
int A5Value;
String inputString = "";
bool stringComplete = false;
bool initComplete = false;
float sensorParams[3][NRSENSORS] = {};
float pumpInformation[2][NRPUMPS] = {};

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
  attachInterrupt(digitalPinToInterrupt(22), D1Read, RISING);
} // setup

// the loop routine runs over and over again forever:
void loop() 
{ 
  if(initComplete)
  {
      float sensorOutput[10] = {0};
      int iSensorOutput[10] = {0};
      
      if(sensorParams[siOUTPUT][AN1] != -1)
      {
        A1Value = analogRead(A0);
        sensorOutput[AN1] = ( ( (float)(A1Value) ) * sensorParams[siOUTPUT][AN1] ) / BIT12ADC;
        iSensorOutput[AN1] = SCALING * (sensorOutput[AN1] - sensorParams[siB][AN1]);
        iSensorOutput[AN1] = iSensorOutput[AN1] / sensorParams[siA][AN1];
      }
      Serial.print(iSensorOutput[AN1]);
      Serial.print(' ');

      if(sensorParams[siOUTPUT][AN2] != -1)
      {
        A2Value = analogRead(A1);
        sensorOutput[AN2] = ( ( (float)(A2Value) ) * sensorParams[siOUTPUT][AN2] ) / BIT12ADC;
        iSensorOutput[AN2] = SCALING * (sensorOutput[AN2] - sensorParams[siB][AN2]); 
        iSensorOutput[AN2] = iSensorOutput[AN2] / sensorParams[siA][AN2];
      }
      Serial.print(iSensorOutput[AN2]);
      Serial.print(' ');

      if(sensorParams[siOUTPUT][AN3] != -1)
      {
        A3Value = analogRead(A2);
        sensorOutput[AN3] = ( ( (float)(A3Value) ) * sensorParams[siOUTPUT][AN3] ) / BIT12ADC;
        iSensorOutput[AN3] = SCALING * (sensorOutput[AN3] - sensorParams[siB][AN3]); 
        iSensorOutput[AN3] = iSensorOutput[AN3] / sensorParams[siA][AN3];
      }
      Serial.print(iSensorOutput[AN3]);
      Serial.print(' ');

      if(sensorParams[siOUTPUT][AN4] != -1)
      {
        A4Value = analogRead(A3);
        sensorOutput[AN4] = ( ( (float)(A4Value) ) * sensorParams[siOUTPUT][AN4] ) / BIT12ADC;
        iSensorOutput[AN4] = SCALING * (sensorOutput[AN4] - sensorParams[siB][AN4]); 
        iSensorOutput[AN4] = iSensorOutput[AN4] / sensorParams[siA][AN4];
      }
      Serial.print(iSensorOutput[AN4]);
      Serial.print(' ');

      if(sensorParams[siOUTPUT][AN5] != -1)
      {
        A5Value = analogRead(A4);
        sensorOutput[AN5] = ( ( (float)(A5Value) ) * sensorParams[siOUTPUT][AN5] ) / BIT12ADC;
        iSensorOutput[AN5] = SCALING * (sensorOutput[AN5] - sensorParams[siB][AN5]); 
        iSensorOutput[AN5] = iSensorOutput[AN5] / sensorParams[siA][AN5];
      }
      Serial.print(iSensorOutput[AN5]);
      Serial.print(" 0 0 0 0 0");
      Serial.print("\r\n");
  }
    // Necessary for the live plot to update without crashing.
  delay(10);
}

void D1Read() 
{
  countedPulses++;
}

void serialEvent()
{
    // Serial.println("Yup I received something");
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
      
      // Serial.println(inputString);
      String sensor = inputString.substring(0, inputString.indexOf(' '));
      // Serial.println(sensor);
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
          sensorParams[siOUTPUT][(int)((sensor[1]-'0')-1)] = (inputString.substring(0, inputString.indexOf(' '))).toFloat();
          inputString.remove(0, inputString.indexOf(' ')+1);
          sensorParams[siA][(int)((sensor[1]-'0')-1)] = (inputString.substring(0, inputString.indexOf(' '))).toFloat();
          inputString.remove(0, inputString.indexOf(' ')+1);
          //Serial.println(inputString);
          sensorParams[siB][(int)((sensor[1]-'0')-1)] = inputString.toFloat();
          //Serial.println(sensorParams[siB][(int)((sensor[1]-'0')-1)]);
        }
        else
        {
          // If the sensor is disabled, it doesn't matter what values are sent; they are ignored. 
          sensorParams[siOUTPUT][(int)((sensor[1]-'0')-1)] = -1;
          sensorParams[siA][(int)((sensor[1]-'0')-1)] = -1;
          sensorParams[siB][(int)((sensor[1]-'0')-1)] = -1;
        }
        inputString = "";
        stringComplete = false;
        /*Serial.println("Sensor: " + sensor + ", is " + String(enabled) + ", val: " + String(sensorParams[siOUTPUT][(int)((sensor[1]-'0')-1)]) 
                + ", aVal: " + String(sensorParams[siA][(int)((sensor[1]-'0')-1)]) + ", bVal: " + String(sensorParams[siB][(int)((sensor[1]-'0')-1)]));*/
      }
      else if (sensor[0] == 'D')
      {
        if(enabled == '1')
        {
          // If a digital sensor is enabled, one value is expected. Other values will simply be ignored.
          sensorParams[siOUTPUT][(int)((sensor[1]-'0')-1+5)] = (inputString.substring(0, inputString.indexOf(' '))).toFloat();
          sensorParams[siA][(int)((sensor[1]-'0')-1+5)] = -1;
          sensorParams[siB][(int)((sensor[1]-'0')-1+5)] = -1;
          inputString.remove(0, inputString.indexOf(' ')+1);
        }
        else
        {
          // If a digital sensor is disabled, it doesn't matter
          sensorParams[siOUTPUT][(int)((sensor[1]-'0')-1+5)] = -1;
          sensorParams[siA][(int)((sensor[1]-'0')-1+5)] = -1;
          sensorParams[siB][(int)((sensor[1]-'0')-1+5)] = -1;
        }
        inputString = "";
        stringComplete = false;
        /*Serial.println("Sensor: " + sensor + ", is " + String(enabled) + ", val: " + String(sensorParams[siOUTPUT][(int)((sensor[1]-'0')-1+5)]) 
                + ", aVal: " + String(sensorParams[siA][(int)((sensor[1]-'0')-1+5)]) + ", bVal: " + String(sensorParams[siB][(int)((sensor[1]-'0')-1+5)])); */
      }
      else if (sensor[0] == 'P')
      {
        if(enabled == '1')
        {
          pumpInformation[piFLOWRATE][(int)((sensor[1]-'0')-1)] = (inputString.substring(0, inputString.indexOf(' '))).toFloat();
          inputString.remove(0, inputString.indexOf(' ')+1);

          if(inputString[0] == 'A')
          {
            pumpInformation[piFEEDBACK][(int)((sensor[1]-'0')-1)] = (float)((inputString[1]-'0') -1);
          }
          else if(inputString[0] == 'D')
          {
            pumpInformation[piFEEDBACK][(int)((sensor[1]-'0')-1)] = (float)((inputString[1]-'0') -1 + 5);
          }
        }
        else
        {
          pumpInformation[piFLOWRATE][(int)((sensor[1]-'0')-1)] = -1;
          pumpInformation[piFEEDBACK][(int)((sensor[1]-'0')-1)] = -1;
        }
        inputString = "";
        stringComplete = false;
        // Serial.println("Pump: " + sensor + " is " + String(enabled) + ", val: " + String(pumpInformation[piFLOWRATE][(int)((sensor[1]-'0')-1)]) + ", with feedback: " + String(pumpInformation[piFEEDBACK][(int)((sensor[1]-'0')-1)]));
      }
      else if (sensor[0] == 'Q')
      {
        //Serial.println("Yup, im done");
        inputString = "";
        initComplete = true;
      }
      else if (sensor[0] == 'R')
      {
        // Perform watchdog reset.
        // Serial.println("RESET MEEEE");
        inputString = "";
        delay(1000);
        rstc_start_software_reset(RSTC);
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
