// the setup routine runs once when you press reset:
volatile int countedPulses = 0;
int S1Value;
int S2Value;
int S3Value;
unsigned long timestamp;
void setup() 
{
  // initialize serial communication at 115200 bits per second:
  Serial.begin(115200);
  // Set the analog read resolution to 12-bit.
  analogReadResolution(12); // 12-bit resolution for the ADC

  //Attach the interrupt to the digital pin in order to count the sensor pulses.
  attachInterrupt(digitalPinToInterrupt(52), countPulses, RISING);
}

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
    Serial.print(string);

    // Necessary for the live plot to update without crashing.
    delay(3);
}

void countPulses() 
{
  countedPulses++;
}
