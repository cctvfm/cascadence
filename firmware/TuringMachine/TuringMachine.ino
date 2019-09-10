#include <tinySPI.h>

//TuringMachine
//For Cascadence
//Developed with Modular Seattle for Velocity 2019
    
#define MAXSEQLENGTH 16  
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

unsigned int values[4];
const int A = 0;
const int B = 1;
unsigned int sequence = 0;
unsigned char stepnumber=0;
unsigned char offset_stepnumber[2];
unsigned char seq_length;
char seq_randomness;
unsigned int seq_scale;
unsigned int seq_shift;

unsigned int maxvalues[]={0,1,3,7,15,31,63,127,255,511,1023,2047,4095,8191,16383,32767,65535};  //max posible values for a given bit length

const int BITSPERSEMITONE=83;

//for quantizer, 83.33 mV per semitone
//On prototype, max output is 4.096V - needs adjusting for final release
//for 12 bit output, math is nice - 83 bits per semitone
//
void setup()
{


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


  updatevalues[A];
  updatevalues[B];
  
}

void loop() {
int pulse = false;
boolean lastbit;
unsigned int outputvalue;
unsigned int leftover;
while(1)
{
  
  if(digitalRead(CLK_IN) == LOW)  //we've received a clock pulse!
    {
      lastbit=bitRead(sequence,(seq_length-1));

      if(random(90)>seq_randomness)  //randomness is between -10 and 100
        lastbit = !lastbit;
        
      sequence = sequence<<1;
      sequence = sequence & 0xFFFE;
      if(lastbit == 1)
        bitSet(sequence,0); 
      
      outputvalue = sequence & maxvalues[seq_length];
      outputvalue = map(outputvalue,0,maxvalues[seq_length],0,seq_scale);
      outputvalue = outputvalue+seq_shift;

      if(digitalRead(SW)) //Quantizer is on
      {
        leftover = outputvalue % BITSPERSEMITONE;
        if(leftover>41) //note is closest to the one above it
            outputvalue = outputvalue+BITSPERSEMITONE-leftover;

        else // or we're closer to the one below it
          outputvalue = outputvalue-leftover;
      }

      if(lastbit == 1)
        SendPulse(B);
        
      setOutput(A, GAIN_2, NO_SHTDWN, outputvalue);
      
      while(digitalRead(CLK_IN) == LOW);
      
    }
    

    updatevalues();
}
}


void updatevalues(void)
{
  unsigned char x;
   for(x=0;x<4;x++)
  {
    values[x]=analogRead(POTS[x]);  
  }

  seq_length=map(values[1],0,1023,1,MAXSEQLENGTH);
  seq_randomness=map(values[0],0,1023,-10,100);
  seq_scale=map(values[2],0,1023,0,4095);
  seq_shift=map(values[3],0,1023,0,2047);

}

void SendPulse (boolean chan)
{
  setOutput(chan, GAIN_2, NO_SHTDWN, 0xFFF);
  delay(40);
  setOutput(chan, GAIN_2, NO_SHTDWN, 0);
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

