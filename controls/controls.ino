#define onLED 8
#define selector 5  // analog 5 pin

#define c0  11
#define c1  12
#define c2  13

const int controlPins[] = {2,3,4,5,6,7,8,9,10,12,13};
int mode = 0;

void setup() {
  Serial.begin(9600);

  // Pins
  pinMode(onLED, OUTPUT);
  for(int i=0; i<11; i++){
    pinMode(controlPins[i], OUTPUT);
    digitalWrite(controlPins[i], 0);
  }
  pinMode(selector, INPUT);
  pinMode(c0, OUTPUT);
  pinMode(c1, OUTPUT);
  pinMode(c2, OUTPUT);

  // Turn on LED
  //digitalWrite(onLED, HIGH);

}

void loop() {
  // Controls
  mode = controlRead();
  Serial.println(mode);
  for(int i=0; i<11; i++){
    digitalWrite(controlPins[i], 0);
  }
  if(mode != -1){
    digitalWrite(controlPins[mode], 1);
  }

  // Outputting to other Arduino
  if (mode > 7) {
    mode = 7;
  }
  digitalWrite(c0, (mode % 2));
  mode /= 2;
  digitalWrite(c1, (mode % 2));
  mode /= 2;
  digitalWrite(c2, (mode % 2));
}

// reads control pot to see which one of 11 settings is currently selected
int controlRead(){
  int err = 30;
  int in = 0;
  int sampleSize = 10;
  int values[] = {1020,967,856,743,637,539,399,337,231,100,60};

  // read selector pin
  for(int i=0; i<sampleSize; i++){
    in += analogRead(selector);
  }
  in /= sampleSize;
  //Serial.println(in);

  // determine which mode is selected
  for(int j=0; j<11; j++){
    if((in > (values[j] - err)) && (in < (values[j] + err))){
      return j;
    }
  }
  return -1;
}
