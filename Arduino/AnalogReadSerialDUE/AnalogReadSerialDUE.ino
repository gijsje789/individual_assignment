// the setup routine runs once when you press reset:
volatile int countedPulses = 0;
void setup() 
{
  // initialize serial communication at 9600 bits per second:
  Serial.begin(9600);
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
  int S1Value = analogRead(A0);
  int S2Value = analogRead(A1);
  int S3Value = analogRead(A2);
  
  // Send the sensor values through the serial connection.
  string = "S1," + String(S1Value) + ",S2," + String(S2Value) + ",S3," + String(S3Value) + ",\r\n"; 
  // Last comma is needed to seperate the /r/n from last sensor value.
  
  Serial.print(string);
  delay(1);        // delay in between reads for stability
}

void countPulses() 
{
  countedPulses++;
}
