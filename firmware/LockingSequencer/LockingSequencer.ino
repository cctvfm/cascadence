#include <tinySPI.h>

//LockingSequencer 
//For Cascadence
//Developed with Modular Seattle for Velocity 2019
    

const int GAIN_1 = 0x1;
const int GAIN_2 = 0x0;
const int NO_SHTDWN = 1;
const int SHTDWN = 0;

//pin definitions
const int POTS[4]={0,1,2,3};

const int CLK_IN = 8;
const int SW = 7;

const int MOSI = 6;
const int SCK = 4;
const int PIN_CS = 5;

unsigned int outputs[2][4];
const int A = 0;
const int B = 1;
void setup()
{
  unsigned int x;

  pinMode(POTS[0], INPUT);
  pinMode(POTS[1], INPUT);
  pinMode(POTS[2], INPUT);
  pinMode(POTS[3], INPUT);
  pinMode(SW, INPUT);
  pinMode(CLK_IN,INPUT_PULLUP);


//mosi, sck, and pin_cs used for spi dac (mcp4822)
  pinMode(MOSI,OUTPUT);
  pinMode(SCK,OUTPUT);
  
  
  digitalWrite(PIN_CS,HIGH);
  pinMode(PIN_CS, OUTPUT);

  for(x=0;x<4;x++)
  {
    outputs[A][x]=analogRead(POTS[x]);  //read the 4 pots so we don't have junk for the first sequence
    outputs[B][x]=outputs[A][x];
  }
  
}

void loop() {
  int step = 0;
  unsigned int tempanalog;
  int counter = 0;
  unsigned int currentoutput[2];
  while(1)
  {
    if(digitalRead(CLK_IN) == LOW)  //we've received a clock pulse!
    {
      step++;
      if(step>3)  //keeping track of the step number
        step = 0;

      setOutput(A, GAIN_2, NO_SHTDWN,(outputs[A][step])<<2);  //output the values stored in outputs
      currentoutput[A]=outputs[A][step];  //save the current value so we can know if it changes in 'real time'
      setOutput(B, GAIN_2, NO_SHTDWN,(outputs[B][step])<<2);
      currentoutput[B]=outputs[B][step];
      while(digitalRead(CLK_IN) == LOW  );  //wait for the clock to go low again so we don't retrigger on the same clock pulse
    }

    if(digitalRead(SW) == HIGH) //Switch set to A
    {
      outputs[A][counter]=analogRead(POTS[counter]);
      if(outputs[A][step]!=currentoutput[A])  //check if we're on the current step and our pot has been changed
      {
        setOutput(A, GAIN_2, NO_SHTDWN,(outputs[A][step])<<2);  //if there's a change, update it.
        currentoutput[A]=outputs[A][step];
      }
  
    }
   
    if(digitalRead(SW) == LOW) //Switch set to B
    {

      outputs[B][counter]=analogRead(POTS[counter]);
      if(outputs[B][step]!=currentoutput[B])  //real time update
      {
        setOutput(B, GAIN_2, NO_SHTDWN,(outputs[B][step])<<2);
        currentoutput[B]=outputs[B][step];
      }
  
    }
    counter++;
    if(counter>3)
      counter = 0;  //this counter is just so we cycle through the 4 pots when we scan through the loop
  }

}

void setOutput(byte channel, byte gain, byte shutdown, unsigned int val)
{
  byte lowByte = val & 0xff;
  byte highByte = ((val >> 8) & 0xff) | channel << 7 | gain << 5 | shutdown << 4;
  //bit level stuff to make the dac work.
  //channel can be 0 or 1 (A or B)
  //gain can be 0 or 1 (x1 or x2 corresponding to 2V max or 4V max)
  //shutdown is to disable the DAC output, 0 for shutdown, 1 for no shutdown
   
  digitalWrite(PIN_CS, LOW);
  shiftOut(MOSI,SCK,MSBFIRST,highByte);
  shiftOut(MOSI,SCK,MSBFIRST,lowByte);
  digitalWrite(PIN_CS, HIGH);
}
