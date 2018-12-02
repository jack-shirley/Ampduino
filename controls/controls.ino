#define onLED 11
#define selector 5  // analog 5 pin black wire
// 5v = white wire
// gnd = yellow wire
#define c0  11
#define c1  12
#define c2  13

const int controlPins[] = {2,3,4,5,6,7,8,9,10};
int mode = 0;
int temp = 0;

void setup() {
  Serial.begin(9600);

  // Pins
  pinMode(onLED, OUTPUT);
  for(int i=0; i<10; i++){
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
  temp = 0;
  // Controls
  mode = controlRead();
  //Serial.println(mode);
  for(int i=0; i<10; i++){
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
  //temp = mode%2;
  //Serial.print(temp);
  mode /= 2;
  digitalWrite(c1, (mode % 2));
  //temp = mode%2;
  //Serial.print(temp);
  mode /= 2;
  digitalWrite(c2, (mode % 2));
  //temp = mode%2;
  //Serial.println(temp);
}

// reads control pot to see which one of 11 settings is currently selected
int controlRead(){
  int err = 39;
  int in = 0;
  int sampleSize = 10;
  int values[] = {1023,980,858,735,621,510,396,278,156,35};

  // read selector pin
  for(int i=0; i<sampleSize; i++){
    in += analogRead(selector);
  }
  in /= sampleSize;
  //Serial.println(in);

  // determine which mode is selected
  for(int j=0; j<10; j++){
    if((in > (values[j] - err)) && (in < (values[j] + err))){
      return j;
    }
  }
  return -1;
}
