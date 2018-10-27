// AMPDUINO

/* GLOBALS */
#define input A0
#define output 9

/* IO Variables */
int readIn;
int sendOut;
float mapped; // value mapped to Volts

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  pinMode(input,INPUT);
  pinMode(output,OUTPUT);
}

void loop() {
  // put your main code here, to run repeatedly:
  readIn = analogRead(input);
  mapped = float(float(readIn * 5) / 1023);
  Serial.print("Value at ");
  Serial.print(millis());
  Serial.print(" is ");
  Serial.print(mapped);
  Serial.print("V, Outputting ");
  Serial.println(readIn / 4);
  analogWrite(output,readIn/4);
}
