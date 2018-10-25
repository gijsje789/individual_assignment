#include <SimpleTimer.h>
#include "PID.h"
#define DELAYTIME 10 // ms
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
#define P1 0
#define P2 1
#define P3 2
#define P4 3
#define BIT12ADC 4095
#define BIT10ADC 1024
#define MAXANALOGVOLTAGE 5.0 // The voltage divider is based on the conversion of 5VDC to 3.3VDC.
#define AN1PIN A0
#define AN2PIN A1
#define AN3PIN A2
#define AN4PIN A3
#define AN5PIN A4
#define D1PIN 33
#define D2PIN 35
#define D3PIN 37
#define D4PIN 39
#define D5PIN 41
#define P1_PWM 13
#define P1_INH 51
#define P2_PWM 12
#define P2_INH 49
#define P3_PWM 11
#define P3_INH 47
#define P4_PWM 10
#define P4_INH 45
#define NRPULSES 4

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

volatile int D1Value = 0;
volatile unsigned long timeStamp_D1 = 0;
volatile int D1Counter = 0;
volatile int D2Value = 0;
volatile unsigned long timeStamp_D2 = 0;
volatile int D2Counter = 0;
volatile int D3Value = 0;
volatile unsigned long timeStamp_D3 = 0;
volatile int D3Counter = 0;
volatile int D4Value = 0;
volatile unsigned long timeStamp_D4 = 0;
volatile int D4Counter = 0;
volatile int D5Value = 0;
volatile unsigned long timeStamp_D5 = 0;
volatile int D5Counter = 0;
volatile float PperMin[10] = {};

int A1Value;
int A2Value;
int A3Value;
int A4Value;
int A5Value;

String inputString = "";
bool stringComplete = false;
bool initComplete = false;

float sensorParams[3][NRSENSORS] = {-1};
float pumpParams[2][NRPUMPS] = {-1};


SimpleTimer DS_Timer;
int D1_TimerID;
int D2_TimerID;
int D3_TimerID;
int D4_TimerID;
int D5_TimerID;

PID Controller[4];
bool SensorCalibration = false;

// the setup routine runs once when you press reset:
void setup() 
{
  // initialize serial communication at 115200 bits per second:
  Serial.begin(115200);
  // Set the analog read resolution to 12-bit.
  analogReadResolution(12); // 12-bit resolution for the ADC
  analogWriteResolution(12); // 12-bit PWM resolution.
  
  // reserve 200 bytes for the inputString.
  inputString.reserve(200);

  pinMode(P1_PWM, OUTPUT);
  pinMode(P1_INH, OUTPUT);
  pinMode(P2_PWM, OUTPUT);
  pinMode(P2_INH, OUTPUT);
  pinMode(P3_PWM, OUTPUT);
  pinMode(P3_INH, OUTPUT);
  pinMode(P4_PWM, OUTPUT);
  pinMode(P4_INH, OUTPUT);
  
  // Attach the interrupt to the digital pin in order to count the sensor pulses.
  attachInterrupt(digitalPinToInterrupt(D1PIN), D1Read, RISING);
  attachInterrupt(digitalPinToInterrupt(D2PIN), D2Read, RISING);
  /******* Uncomment and enter correct pin ******/
  // attachInterrupt(digitalPinToInterrupt(D3PIN), D3Read, RISING);
  // attachInterrupt(digitalPinToInterrupt(D4PIN), D4Read, RISING);
  // attachInterrupt(digitalPinToInterrupt(D5PIN), D5Read, RISING);
} // setup

// the loop routine runs over and over again forever:
void loop() 
{ 
  if(initComplete)
  {
      DS_Timer.run();
      
      float sensorOutput[5] = {0};
      int iSensorOutput[10] = {0};
      
      if(sensorParams[siA][AN1] != -1)
      {
        A1Value = analogRead(AN1PIN);
        sensorOutput[AN1] = ( ( (float)(A1Value) ) * MAXANALOGVOLTAGE ) / BIT12ADC;
        iSensorOutput[AN1] = SCALING * (sensorOutput[AN1] - sensorParams[siB][AN1]);
        iSensorOutput[AN1] = iSensorOutput[AN1] / sensorParams[siA][AN1];
      }
      if (SensorCalibration)
        Serial.print(sensorOutput[AN1]*SCALING);
      else
        Serial.print(iSensorOutput[AN1]);
      Serial.print(' ');

      if(sensorParams[siA][AN2] != -1)
      {
        A2Value = analogRead(AN2PIN);
        sensorOutput[AN2] = ( ( (float)(A2Value) ) * MAXANALOGVOLTAGE ) / BIT12ADC;
        iSensorOutput[AN2] = SCALING * (sensorOutput[AN2] - sensorParams[siB][AN2]); 
        iSensorOutput[AN2] = iSensorOutput[AN2] / sensorParams[siA][AN2];
      }
      if (SensorCalibration)
        Serial.print(sensorOutput[AN2]*SCALING);
      else
        Serial.print(iSensorOutput[AN2]);
      Serial.print(' ');

      if(sensorParams[siA][AN3] != -1)
      {
        A3Value = analogRead(AN3PIN);
        sensorOutput[AN3] = ( ( (float)(A3Value) ) * MAXANALOGVOLTAGE ) / BIT12ADC;
        iSensorOutput[AN3] = SCALING * (sensorOutput[AN3] - sensorParams[siB][AN3]); 
        iSensorOutput[AN3] = iSensorOutput[AN3] / sensorParams[siA][AN3];
      }
      if (SensorCalibration)
        Serial.print(sensorOutput[AN3]*SCALING);
      else
        Serial.print(iSensorOutput[AN3]);
      Serial.print(' ');

      if(sensorParams[siA][AN4] != -1)
      {
        A4Value = analogRead(AN4PIN);
        sensorOutput[AN4] = ( ( (float)(A4Value) ) * MAXANALOGVOLTAGE ) / BIT12ADC;
        iSensorOutput[AN4] = SCALING * (sensorOutput[AN4] - sensorParams[siB][AN4]); 
        iSensorOutput[AN4] = iSensorOutput[AN4] / sensorParams[siA][AN4];
      }
      if (SensorCalibration)
        Serial.print(sensorOutput[AN4]*SCALING);
      else
        Serial.print(iSensorOutput[AN4]);
      Serial.print(' ');

      if(sensorParams[siA][AN5] != -1)
      {
        A5Value = analogRead(AN5PIN);
        sensorOutput[AN5] = ( ( (float)(A5Value) ) * MAXANALOGVOLTAGE ) / BIT12ADC;
        iSensorOutput[AN5] = SCALING * (sensorOutput[AN5] - sensorParams[siB][AN5]); 
        iSensorOutput[AN5] = iSensorOutput[AN5] / sensorParams[siA][AN5];
      }
       if (SensorCalibration)
        Serial.print(sensorOutput[AN5]*SCALING);
      else
        Serial.print(iSensorOutput[AN5]);
      Serial.print(' ');

      iSensorOutput[D1] = D1Value;
      if (SensorCalibration)
        Serial.print(PperMin[D1]*SCALING);
      else
        Serial.print(iSensorOutput[D1]);
      Serial.print(' ');

      iSensorOutput[D2] = D2Value;
      if (SensorCalibration)
        Serial.print(PperMin[D2]*SCALING);
      else
        Serial.print(iSensorOutput[D2]);
      Serial.print(' ');

      iSensorOutput[D3] = D3Value;
      if (SensorCalibration)
        Serial.print(PperMin[D3]*SCALING);
      else
        Serial.print(iSensorOutput[D3]);
      Serial.print(' ');

      iSensorOutput[D4] = D4Value;
      if (SensorCalibration)
        Serial.print(PperMin[D4]*SCALING);
      else
        Serial.print(iSensorOutput[D4]);
      Serial.print(' ');

      iSensorOutput[D5] = D5Value;
      if (SensorCalibration)
        Serial.print(PperMin[D5]*SCALING);
      else
        Serial.print(iSensorOutput[D5]);
      Serial.print(' ');
      // No space here for debuging purpose of controllers.

      if(pumpParams[piFLOWRATE][P1] != -1)
      {
        // Flowrate in the pumpParams is the nonscaled version in L/min, calculated sensor value is scaled by SCALING
        digitalWrite(P1_INH, HIGH);
        int PWM_val = Controller[P1].getControlSignal(SCALING*pumpParams[piFLOWRATE][P1], iSensorOutput[(int)pumpParams[piFEEDBACK][P1]]);
        analogWrite(P1_PWM, PWM_val);
      }
      else
      {
        digitalWrite(P1_INH, LOW); // Put H-bridge to sleep.
        analogWrite(P1_PWM, 0); // Do not send a PWM signal.
      }

      if(pumpParams[piFLOWRATE][P2] != -1)
      {
        // Flowrate in the pumpParams is the nonscaled version in L/min, calculated sensor value is scaled by SCALING
        digitalWrite(P2_INH, HIGH);
        int PWM_val = Controller[P2].getControlSignal(SCALING*pumpParams[piFLOWRATE][P2], iSensorOutput[(int)pumpParams[piFEEDBACK][P2]]);
        analogWrite(P2_PWM, PWM_val);
      }
      else
      {
        digitalWrite(P2_INH, LOW); // Put H-bridge to sleep.
        analogWrite(P2_PWM, 0); // Do not send a PWM signal.
      }

      if(pumpParams[piFLOWRATE][P3] != -1)
      {
        // Flowrate in the pumpParams is the nonscaled version in L/min, calculated sensor value is scaled by SCALING
        digitalWrite(P3_INH, HIGH);
        int PWM_val = Controller[P3].getControlSignal(SCALING*pumpParams[piFLOWRATE][P3], iSensorOutput[(int)pumpParams[piFEEDBACK][P3]]);
        analogWrite(P3_PWM, PWM_val);
      }
      else
      {
        digitalWrite(P3_INH, LOW); // Put H-bridge to sleep.
        analogWrite(P3_PWM, 0); // Do not send a PWM signal.
      }

      if(pumpParams[piFLOWRATE][P4] != -1)
      {
        // Flowrate in the pumpParams is the nonscaled version in L/min, calculated sensor value is scaled by SCALING
        digitalWrite(P4_INH, HIGH);
        int PWM_val = Controller[P4].getControlSignal(SCALING*pumpParams[piFLOWRATE][P4], iSensorOutput[(int)pumpParams[piFEEDBACK][P4]]);
        analogWrite(P4_PWM, PWM_val);
      }
      else
      {
        digitalWrite(P4_INH, LOW); // Put H-bridge to sleep.
        analogWrite(P4_PWM, 0); // Do not send a PWM signal.
      }
      
      Serial.print("\r\n"); // Placed at the end for debugging purposes.
    } // if Initcomplete
    // Sampling and communication frequency. 100Hz as set by meeting with Marcel and Marije Kamphuis.
  delay(DELAYTIME);
}

void D1Read() 
{
  unsigned long diff;
  DS_Timer.restartTimer(D1_TimerID);
  if (timeStamp_D1 == 0)
  {
    timeStamp_D1 = micros();
    D1Counter++;
  }
  else
  {
    D1Counter++;
    if(D1Counter >= NRPULSES)
    {
      unsigned long current = micros();
      diff = current - timeStamp_D1;
      diff = diff / D1Counter;
      PperMin[D1] = (float)(60000000.0 / (float) (diff));
      D1Value = (int) ((float)SCALING * (PperMin[D1] / (float)(sensorParams[siOUTPUT][D1])));
      timeStamp_D1 = 0;
      D1Counter = 0;
    }
  }
}

void D1_ResetValue()
{
  D1Value = 0;
}

void D2Read() 
{
  unsigned long diff;
  DS_Timer.restartTimer(D2_TimerID);
  if (timeStamp_D2 == 0)
  {
    timeStamp_D2 = micros();
    D2Counter++;
  }
  else
  {
    D2Counter++;
    if(D2Counter >= NRPULSES)
    {
      unsigned long current = micros();
      diff = current - timeStamp_D2;
      diff = diff / D2Counter;
      PperMin[D2] = (float)(60000000.0 / (float) (diff));
      D2Value = (int) ((float)SCALING * (PperMin[D2] / (float)(sensorParams[siOUTPUT][D2])));
      timeStamp_D2 = 0;
      D2Counter = 0;
    }
  }
}

void D2_ResetValue()
{
  D2Value = 0;
}

void D3Read() 
{
  unsigned long diff;
  DS_Timer.restartTimer(D3_TimerID);
  if (timeStamp_D3 == 0)
  {
    timeStamp_D3 = micros();
    D3Counter++;
  }
  else
  {
    D3Counter++;
    if(D3Counter >= NRPULSES)
    {
      unsigned long current = micros();
      diff = current - timeStamp_D3;
      diff = diff / D3Counter;
      PperMin[D3] = (float)(60000000.0 / (float) (diff));
      D3Value = (int) ((float)SCALING * (PperMin[D3] / (float)(sensorParams[siOUTPUT][D3])));
      timeStamp_D3 = 0;
      D3Counter = 0;
    }
  }
}

void D3_ResetValue()
{
  D3Value = 0;
}


void D4Read() 
{
  unsigned long diff;
  DS_Timer.restartTimer(D4_TimerID);
  if (timeStamp_D4 == 0)
  {
    timeStamp_D4 = micros();
    D4Counter++;
  }
  else
  {
    D4Counter++;
    if(D4Counter >= NRPULSES)
    {
      unsigned long current = micros();
      diff = current - timeStamp_D4;
      diff = diff / D4Counter;
      PperMin[D4] = (float)(60000000.0 / (float) (diff));
      D4Value = (int) ((float)SCALING * (PperMin[D4] / (float)(sensorParams[siOUTPUT][D4])));
      timeStamp_D4 = 0;
      D4Counter = 0;
    }
  }
}

void D4_ResetValue()
{
  D4Value = 0;
}


void D5Read() 
{
  unsigned long diff;
  DS_Timer.restartTimer(D5_TimerID);
  if (timeStamp_D5 == 0)
  {
    timeStamp_D5 = micros();
    D5Counter++;
  }
  else
  {
    D5Counter++;
    if(D5Counter >= NRPULSES)
    {
      unsigned long current = micros();
      diff = current - timeStamp_D5;
      diff = diff / D5Counter;
      PperMin[D5] = (float)(60000000.0 / (float) (diff));
      D5Value = (int) ((float)SCALING * (PperMin[D5] / (float)(sensorParams[siOUTPUT][D5])));
      timeStamp_D5 = 0;
      D5Counter = 0;
    }
  }
}

void D5_ResetValue()
{
  D5Value = 0;
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
      // Serial.println(inputString);
      char enabled = inputString[0];
      // Serial.println(enabled);
      inputString.remove(0, inputString.indexOf(' ')+1);
      // Serial.println(inputString);

      // The first character indicates if information about an analogue or digital sensor is received.
      // The second character will indicate which sensor number.
      // The third character must be a space for correct behaviour.
      // Enabled must be '1' for an enabled sensor, any other value will result in the sensor being disabled.
      if(sensor[0] == 'A')
      {
        if(enabled == '1')
        {
          // If an analog sensor is enabled, 3 values are send.
          sensorParams[siA][(int)((sensor[1]-'0')-1)] = (inputString.substring(0, inputString.indexOf(' '))).toFloat();
          inputString.remove(0, inputString.indexOf(' ')+1);
          //Serial.println(inputString);
          sensorParams[siB][(int)((sensor[1]-'0')-1)] = inputString.toFloat();
          //Serial.println(sensorParams[siB][(int)((sensor[1]-'0')-1)]);
        }
        else
        {
          // If the sensor is disabled, it doesn't matter what values are sent; they are ignored. 
          sensorParams[siA][(int)((sensor[1]-'0')-1)] = -1;
          sensorParams[siB][(int)((sensor[1]-'0')-1)] = -1;
        }
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
        /*Serial.println("Sensor: " + sensor + ", is " + String(enabled) + ", val: " + String(sensorParams[siOUTPUT][(int)((sensor[1]-'0')-1+5)]) 
                + ", aVal: " + String(sensorParams[siA][(int)((sensor[1]-'0')-1+5)]) + ", bVal: " + String(sensorParams[siB][(int)((sensor[1]-'0')-1+5)]));*/
        // Serial.println("testing: " +  String(sensorParams[siOUTPUT][D1]) + " " + String(sensorParams[siOUTPUT][D2]) + " " + String(sensorParams[siOUTPUT][D3]) + " " + String(sensorParams[siOUTPUT][D4]) + " " + String(sensorParams[siOUTPUT][D5]));
      }
      else if (sensor[0] == 'P')
      {
        if(enabled == '1')
        {
          pumpParams[piFLOWRATE][(int)((sensor[1]-'0')-1)] = (inputString.substring(0, inputString.indexOf(' '))).toFloat();
          inputString.remove(0, inputString.indexOf(' ')+1);

          if(inputString[0] == 'A')
          {
            pumpParams[piFEEDBACK][(int)((sensor[1]-'0')-1)] = (float)((inputString[1]-'0') -1);
          }
          else if(inputString[0] == 'D')
          {
            pumpParams[piFEEDBACK][(int)((sensor[1]-'0')-1)] = (float)((inputString[1]-'0') -1 + 5);
          }
        }
        else
        {
          pumpParams[piFLOWRATE][(int)((sensor[1]-'0')-1)] = -1;
          pumpParams[piFEEDBACK][(int)((sensor[1]-'0')-1)] = -1;
        }
        // Serial.println("Pump: " + sensor + " is " + String(enabled) + ", val: " + String(pumpParams[piFLOWRATE][(int)((sensor[1]-'0')-1)]) + ", with feedback: " + String(pumpParams[piFEEDBACK][(int)((sensor[1]-'0')-1)]));
      }
      else if (sensor[0] == 'C')
      {
        if(enabled == '1')
        {
          float Kp = (inputString.substring(0, inputString.indexOf(' '))).toFloat();
          inputString.remove(0, inputString.indexOf(' ')+1);
          float Ki = (inputString.substring(0, inputString.indexOf(' '))).toFloat();
          inputString.remove(0, inputString.indexOf(' ')+1);
          float Kd = (inputString.toFloat());
          Controller[(int)((sensor[1]-'0')-1)].setParameters(10, Kp, Ki, Kd, 4095, 0, 4095, SCALING);
          // Serial.println("Controller: " + String((sensor[1]-'0')-1) + String(Kp) + " " + String(Ki) + " " + String(Kd) + " is set.");
        }
        else
        {
          // Don't set any parameters.
          // Serial.println("Controller: " + String((sensor[1]-'0')-1) + " is not set.");
        }
      }
      else if (sensor[0] == 'Q')
      {
        //Serial.println("Yup, im done");
        inputString = "";
        D1_TimerID = DS_Timer.setInterval(1000, D1_ResetValue);
        D2_TimerID = DS_Timer.setInterval(1000, D2_ResetValue);
        D3_TimerID = DS_Timer.setInterval(1000, D3_ResetValue);
        D4_TimerID = DS_Timer.setInterval(1000, D4_ResetValue);
        D5_TimerID = DS_Timer.setInterval(1000, D5_ResetValue);
        DS_Timer.enable(D1_TimerID);
        DS_Timer.enable(D2_TimerID);
        DS_Timer.enable(D3_TimerID);
        DS_Timer.enable(D4_TimerID);
        DS_Timer.enable(D5_TimerID);
        DS_Timer.restartTimer(D1_TimerID);
        DS_Timer.restartTimer(D2_TimerID);
        DS_Timer.restartTimer(D3_TimerID);
        DS_Timer.restartTimer(D4_TimerID);
        DS_Timer.restartTimer(D5_TimerID);
        initComplete = true;
      }
      else if (sensor[0] == 'R')
      {
        // Perform watchdog reset.
        // Serial.println("RESET MEEEE");
        inputString = "";
        DS_Timer.deleteTimer(D1_TimerID);
        DS_Timer.deleteTimer(D2_TimerID);
        DS_Timer.deleteTimer(D3_TimerID);
        DS_Timer.deleteTimer(D4_TimerID);
        DS_Timer.deleteTimer(D5_TimerID);
        delay(1000);
        rstc_start_software_reset(RSTC);
      }
      else if (sensor[0] == 'K')
      {
        if (enabled == '1')
          SensorCalibration = true;
        else
          SensorCalibration = false;
      }
      else
      {
        // If the data starts weird, information is ignored.
        //Serial.println("Error: something went wrong." + sensor + " " + enabled);
      }
      inputString = "";
      stringComplete = false;
  } // if
} // serialEvent()

void serialEventRun(void) {
  if (Serial.available()) serialEvent();
} 
