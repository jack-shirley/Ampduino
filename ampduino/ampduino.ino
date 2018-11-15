// AMPDUINO

/* GLOBALS */
#define onLED 12
// input = A0
// output = 9, 10

/* ADC */
unsigned int adcLOW, adcHIGH;
int adcINPUT;

/* PWM */
#define freq 0x00FF // 31.3kHz
#define mode 0      // phase correct mode
#define qty 2       // number of PWMs in parallel (1-3)

void setup() {
  Serial.begin(9600);
  
  // Pins
  pinMode(onLED, OUTPUT);

  // Turn on LED
  digitalWrite(12, HIGH);

  /* ADC Setup */
  // We want to turn on ADC and read from A0 (ADC0)
  
  // ADCSRA Bits: ADEN,ADSC,ADFR,ADIF,ADIE,ADPS2,ADPS1,ADPS0
  // ADEN = ADC enable
  // ADSC = ADC start conversion
  // ADFR = ADC free running select (1: free running mode, 0: single conversion mode)
  // ADIF = ADC interrupt flag
  // ADIE = ADC interrupt enable
  // ADPS = prescaler bits, clock rate/ADPS = ADC clock freq
  ADCSRA = B11100101; // turn on ADC, clockrate/32

  // ADMUX Bits: REFS1,REFS0,ADLAR,x,MUX3,MUX2,MUX1,MUX0
  // REFS = selector for internal reference voltage
  // ADLAR = ADC left adjust result
  // x = not used
  // MUX = analog channel selection bits, ADC0 = 0000
  ADMUX = B1100000;

  // ADCSRB
  ADCSRB = B0000111;

  // DIDR0
  DIDR0 = B0000001; // turn off digital inputs for ADC0

  /* PWM Setup */
  // Initialize Timer1
  TCCR1A = 0;
  TCCR1B = 0;
  // TCCR1A: Timer/Counter Control Register 1A - setting Timer1
  TCCR1A = (((qty - 1) << 5) | 0x80 | (mode << 1));
  // TCCR1B: Timer/Counter Control Register 1B
  // Last 3 bits determine the timer clock setting
  TCCR1B = ((mode << 3) | 0x11); // no clock prescaling
  // more timers
  ICR1L = (freq & 0xff);
  DDRB |= ((qty << 1) | 0x02);
  sei(); // enable interrupts
}

void loop() {
  if(true){
    if(analogRead(0) > 512) analogWrite(9,255);
    else analogWrite(9,0);
  }
  else{
    analogWrite(9,analogRead(0)/4);
  }
}













/*
// Timer1 interrupt service routine
ISR(TIMER1_CAPT_vect){
  // read from ADC registers
  adcLOW = ADCL; // low 8 bits
  adcHIGH = ADCH; // high 2 bits
  adcINPUT = ((adcHIGH << 8) | adcLOW); // bit shift left on the high bits, 'OR' with low bits
  adcINPUT += 0x8000; // make it signed

  // write to pin 9
  OCR1AL = ((adcINPUT + 0x8000) >> 8); // convert to unsigned, higher byte
  // write to pin 10
  OCR1BL = adcINPUT; // lower byte
}
*/
