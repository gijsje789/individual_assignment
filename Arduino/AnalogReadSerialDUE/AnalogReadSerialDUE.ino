// the setup routine runs once when you press reset:
void setup() {
  // initialize serial communication at 9600 bits per second:
  Serial.begin(9600);
  analogReadResolution(12); // 12-bit resolution for the ADC
}

// the loop routine runs over and over again forever:
void loop() {
    String string;
  // read the input on analog pin 0:
  int S1Value = analogRead(A0);
  int S2Value = analogRead(A1);
  
  // Send the sensor values through the serial connection.
  string = "S1," + String(S1Value) + ",S2," + String(S2Value) + ","; 
  // Last comma is needed to seperate the /r/n from last sensor value.
  
  Serial.println(string);
  delay(1);        // delay in between reads for stability
}
