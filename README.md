# Ampduino

# Demonstration Video
https://www.youtube.com/watch?v=M-LkqirBfyE

# Project Description
Full source code available on https://github.com/jack-shirley/Ampduino. For any further questions on Arduino realtime signal processing, contact aschwenn@tamu.edu.

Our "Ampduino" is essentially a guitar effect box implemented using an Arduino Uno to perform real-time digital signal processing. The objective in building this project is to create a guitar effects box that is relatively small and lightweight, and allows the user to select from a variety of effects as well as manipulate the effect itself. Creation of a unique and individual tone and sound is the critical concept for guitarists, and our goal is to provide users with the ability to effectively manipulate input signals.

# Signal Processing on the Arduino

Real-time digital signal processing is possible on the Arduino by using the built-in ADC and PWM pins. The ADC pin settings can modified by manipulating the ADC registers (http://www.robotplatform.com/knowledge/ADC/adc_tutorial.html). The Arduino's ADC pins can only read from 0-5 V, so it's best to apply a DC offset to your signal (which oscillates around 0 V, typically ranging -1 to +1 V for a loud guitar input) to have it oscillate around 2.5 V to avoid losing an signal data. The pulse-width modulation (PWM) pins can be used to create an analog output by modifying the width of a pulse sent out from the digital I/O pins. The wider the width of the pulse, the higher the percieved voltage is, with a 100% duty cycle corresponding to 5 V and a 0% duty cycle corresponding to 0 V. By default, the PWM outputs at a rate of 500 Hz, which is obviously too slow for our purposes, but this can be increased for improved resolution of the output signal (http://wiki.openmusiclabs.com/wiki/PWMDAC). Timer interrupts are used to force the Arduino to read from the ADCs at a certain frequency (https://www.robotshop.com/community/forum/t/arduino-101-timers-and-interrupts/13072); the interrupt service routine can read the sample directly from the ADC's registers and output it directly to the PWM's registers (http://interface.khm.de/index.php/lab/interfaces-advanced/arduino-realtime-audio-processing/).

Modifying the value before it's sent out to the PWM allows the creation of different types of effects. Most of the effects we implemented are relatively simple, which allows the Arduino to quickly process them to avoid latency problems. Here are some basic examples of the algorithms used:

Distortion

   if (input > (threshold + 127)) output = threshold + 127;

   if (input < (127 - threshold)) output = threshold - 127;

   else output = input;

Distortion works by simply clipping the input. Since we configured out ADC's to read a single byte (0-255), 127 represents our "virtual ground", or the 2.5 V that our signal oscillates around.

Fuzz

   if (input > (threshold + 127)) output = 255;

   if (input < (127 - threshold)) output = 0;

   else output = input

Fuzz is similar, with the difference being that values above the threshold are forced to the maximum value.

Bitcrusher

   output = input << 2;

This example performs a logical shift left operation, effectively reducing the resolution of the input signal.



# Circuit Design

Input Circuit
Our input circuit adds a 2.5 V DC offset as previously mentioned, sends the signal through a bandpass filter to remove noise, and amplifies it to improve resolution when read by the Arduino.

Output Circuit
Our output circuit is a 3rd order Sallen-Key circuit which serves as a lowpass filter and removes the previously added DC offset (http://sim.okawa-denshi.jp/en/Sallenkey3Lowkeisan.htm).

# Controls/Enclosure
Our enclosure is a plastic box we drilled holes into for the controls and I/O. We used 1/4" female jacks for the input and output, and 10k potentiometers for the knobs. LEDs were used to indicate power and mode.

# Development Log
Week 1 (Oct 15 - Oct 21) :

Group met in Zachary Lab in order to discuss about system concept, goals, materials needed, and work schedule for the semester and finalized our proposal. Proposal was approved by professor. We submitted forms to utilize 3 D printer at Zachary Design Center. We placed ordered for all of our parts, which are expected to arrive by October 21.


Week 2 (Oct 22 - Oct 28):

We designed the basic circuits using ampduino and wrote the skeleton pseudocode, which we plan on implmenting in C language for various guitar effects. We initialized all the required variables and implemented the basic functions and got it running successfully. We designed input and output circuit diagrams and started utilizing the Multism.


Week 3 (Oct 29 - Nov 4):

We created and outlined our wiki page and initiated this development log in order to ensure we stay on track to get our goals accomplished in timely manner. We built and tested circuit using function generator to generate test signals and oscilloscope to measure noise reduction and proper DC offset application. We also tested the Arduino’s ability to output specified signals using the PWM pins.


Week 4 (Nov 5 - Nov 11):

We began ideating algorithms for applying certain effects and test algorithms to see if  they are possible using generated signals as input. We ensured that algorithms are running with minimal latency and are achieving desired results.


Week 5 (Nov 12 - Nov 18):

We put input/output circuits and code together and testing them. We optimized for performance and interesting effects on tone/sound. We built controls for device, allowing the user to select filter type and control filter parameters.

Error creating thumbnail: /usr/bin/convert: error while loading shared libraries: libgomp.so.1: cannot open shared object file: No such file or directory

Error code: 127

Week 6 (Nov 19 - Nov 25):

Considering electronic limitations when moving to 3D-printed enclosure, we designed and built enclosure and PCB boards. We designed and soldered input and output control. 

Error creating thumbnail: /usr/bin/convert: error while loading shared libraries: libgomp.so.1: cannot open shared object file: No such file or directory

Error code: 127

Week 7 (Nov 26 - Dec 2):

Due to management issues on Zachary Lab's end, we bought the box and drilled holes ourselves by soldering and attaching all the wires for input/out and placed switch and every other features, including LEDs on the top


Week 8 (Dec 3 - Dec 4):

Finished project

# Project Reflections
From the hardware pespective, we had two arduinos, one doing signal processing and one doing the controls and on top of the signal processing arduino, we have input and output circuits. The amount of i/o pins needed was too overwhelming for one arduino to handle and having second arduino significantly enhanced the quality of signal processing. The primary function of second arduino is continously check for which mode is currently selected, communicating with first aduino through threee digital i/o pins sending binary. We developed our knowledge of real time digital signal processing in terms of waves and accomplished to create an applicable and solidly built effect box with various modes and feature to control volume, which was implemented solely through hardware. We also learned much about different sound effect algorithms by implementing them in C language with each case of switch serving each effect. We believe that our knowledge of microcomputer system and relationship between real time and code domain was exemplified in Ampduino.

# Bill of Materials
Cost of Materials

Purchased Items:

10k potentiometers - $5.63

Knobs for potentiometers -$7.01

10k potentiometer with 10 detents - $9.47

3.5mm female jack - $7.99

Speakers - $7.87

¼” male jack - $6.99

¼” female jack - $11.99

TL972IDGKR operational amplifier - $3.45

IC OPAMP GP 12MHZ RRO 8DIP - $14.28

Likewire Dual Angle Cable - $12.98

Labeler, Xpress -$21.64

Plastic Box -$24.99


Total Cost : $134.29
