// AMPDUINO

/* GLOBALS */
int mode = 0;
// input = A0
// output = 11

/* ADC */
// "volatile" must be used for variables inside interrupt handlers
volatile boolean sample;
volatile byte adc0;

/* AVR Commands */
#define cbi(sfr, bit) (_SFR_BYTE(sfr) &= ~_BV(bit)) // clear bit
#define sbi(sfr, bit) (_SFR_BYTE(sfr) |= _BV(bit))  // set bit

/* Effects */
#define distortionThreshold 25
#define fuzzThreshold 13

/* Control */
#define c0  2
#define c1  3
#define c2  4
int i = 0;

void setup() {
  Serial.begin(57600);

  /* ADC Setup */
  // We want to turn on ADC and read from A0 (ADC0)
  
  // ADCSRA Bits: ADEN,ADSC,ADFR,ADIF,ADIE,ADPS2,ADPS1,ADPS0
  // ADEN = ADC enable
  // ADSC = ADC start conversion
  // ADFR = ADC free running select (1: free running mode, 0: single conversion mode)
  // ADIF = ADC interrupt flag
  // ADIE = ADC interrupt enable
  // ADPS = prescaler bits, clock rate/ADPS = ADC clock freq
  sbi(ADCSRA, ADEN); // enable ADC
  //sbi(ADCSRA, ADSC); // start
  //sbi(ADCSRA, ADFR); // free running mode
  // set prescaler to 64 for clock/64 = 19kHz sampling freq
  cbi(ADCSRA, ADPS2);
  sbi(ADCSRA, ADPS1);
  sbi(ADCSRA, ADPS0);

  // ADMUX Bits: REFS1,REFS0,ADLAR,x,MUX3,MUX2,MUX1,MUX0
  // REFS = selector for internal reference voltage
  // ADLAR = ADC left adjust result
  // MUX = analog channel selection bits, ADC0 = 0000
  sbi(ADMUX, ADLAR); // left adjust, 8-bit ADC will be available for use in ADCH register
  // use internal VCC reference
  sbi(ADMUX, REFS0);
  cbi(ADMUX, REFS1);
  // use ADC0 or pin A0 to read from
  cbi(ADMUX, MUX0);
  cbi(ADMUX, MUX1);
  cbi(ADMUX, MUX2);
  cbi(ADMUX, MUX3);

  /* Timer Setup - Using Timer 2 */
  // initialize timer2
  noInterrupts(); // disable interrupts while configuring
  TCCR2A = 0;
  TCCR2B = 0;
  // TCCR2A Bits: COM2A1,COM2A0,COM2B1,COM2B0,x,x,WGM21,WGM20
  // TCCR2B Bits: ICNC2,ICES2,x,WGM23,WGM22,CS22,CS21,CS20
  /* Prescaling (CS2x bits)
   *  000 - no clock source (timer stopped)
   *  001 - clk/1 (no prescaling)
   *  010 - clk/8
   *  011 - clk/64
   *  100 - clk/256
   *  101 - clk/1024
   *  110,111 - external clock source (falling edge, rising edge)
   * Timer Modes (WGM2x bits)
   *  0000 - normal
   *  0001 - PWM, phase correct, 8-bit
   *  0010 - PWM, phase correct, 9-bit
   *  0011 - PWM, phase correct, 10-bit
   *  0100 - CTC
   *  0101 - Fast PWM, 8-bit
   *  0110 - Fast PWM, 9-bit
   *  0111 - Fast PWM, 10-bit
   *  1000 - PWM, phase and frequency correct
   *  1001 - PWM, phase and frequency correct
   *  1010 - PWM, phase correct
   *  1011 - PWM, phase correct
   *  1100 - CTC
   *  1110 - Fast PWM
   *  1111 - Fast PWM
   */
  cbi(TCCR2A, COM2A0);
  sbi(TCCR2A, COM2A1);
  // set to PWM, phase correct, 9-bit
  sbi(TCCR2A, WGM20);
  sbi(TCCR2A, WGM21);
  cbi(TCCR2B, WGM22);
  //cbi(TCCR2B, WGM23);
  // set timer2 prescaler to 1
  sbi(TCCR2B, CS20);
  cbi(TCCR2B, CS21);
  cbi(TCCR2B, CS22);
  // enable PWM port
  sbi(DDRB, 3);
  interrupts(); // re-enable interrupts
  cbi(TIMSK0, TOIE0); // turned off timer0 --- DELAY() is off now
  sbi(TIMSK2, TOIE2); // enable timer2 interrupt
}

void loop() {
  while(!sample){ } // wait until sample is received
  
  // Output audio
  switch(mode){
    case 0: // Clean sound
      OCR2A = adc0; // output audio to PWM port A11
      break;
    case 1: // Distortion effect
      // slight gain
      adc0 = (float((adc0 - 127) * 3)) + 127;
      if(adc0 > (127 + distortionThreshold)) OCR2A = 127 + distortionThreshold;
      else if(adc0 < (127 - distortionThreshold)) OCR2A = 127 - distortionThreshold;
      else OCR2A = adc0;
      break;
    case 2: // Fuzz effect
      // slight gain
      adc0 = (float((adc0 - 127) * 2)) + 127;
      if(adc0 > (127 + fuzzThreshold)) OCR2A = 255;
      else if(adc0 < (127 - fuzzThreshold)) OCR2A = 0;
      else OCR2A = adc0;
      break;
    case 3: // Bit-Crusher
      OCR2A = adc0 << 2;
      break;
    case 4: // Wind-Tunnel
      if(adc0 % 2 == 0) OCR2A = 255;
      else OCR2A = 0;
      break;
    case 5: //Inverse
      if(adc0 < 127){
        OCR2A = adc0 + 127;
      }else{
        OCR2A = adc0 - 127;
      }
    break;
  }
  
  sample = false; // reset sample

  //mode = (digitalRead(c2) << 2) + (digitalRead(c1) << 1) + (digitalRead(c0));
}

ISR(TIMER2_OVF_vect){
  adc0 = ADCH;
  sample = true; // alert the loop that there is a sample available
  sbi(ADCSRA, ADSC); // start conversion again
}
