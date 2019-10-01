//Template
//For Cascadence
//Developed with Modular Seattle for Velocity 2019

//This file is to give you all the functions you need to start writing your own Cascadence firmware

#include <tinySPI.h>
//You will need to install the TinySPI library Using the 'Manage Libraries' panel
    
//DAC Definitions 
const int GAIN_1 = 0x1;
const int GAIN_2 = 0x0;
const int NO_SHTDWN = 1;
const int SHTDWN = 0;

const int A=0;  //Use A or B to write to DAC
const int B=1;  //This makes it easy to remember which output is which

//pin definitions
const int POTS[4]={0,1,2,3};  //the 4 pots from top to bottom
const int CLK_IN = 8;
const int SW = 7;
//DAC pins
const int MOSI = 6;
const int SCK = 4;
const int PIN_CS = 5;

unsigned int values[4]; //Global array to store potentiometer values


void setup()
{


  pinMode(POTS[0], INPUT);  //Set up pins as inputs
  pinMode(POTS[1], INPUT);
  pinMode(POTS[2], INPUT);
  pinMode(POTS[3], INPUT);
  pinMode(SW, INPUT);
  pinMode(CLK_IN,INPUT_PULLUP); //Pullup might not be necessary


//mosi, sck, and pin_cs used for spi dac (mcp4822)
  pinMode(MOSI,OUTPUT);
  pinMode(SCK,OUTPUT);
  
  
  digitalWrite(PIN_CS,HIGH);  //prepare the dac CS line (active low)
  pinMode(PIN_CS, OUTPUT);

  setOutput(A, GAIN_2, NO_SHTDWN, 0); //initialize both DACs to 0
  setOutput(B, GAIN_2, NO_SHTDWN, 0);
  
}

void loop() {

{
  
  if(digitalRead(CLK_IN) == LOW)  //we've received a clock pulse!
    {
      //Insert your code here to handle a clock
      //SendPulse(A); will send a trigger on channel A
      //SendPulse(B); will send a trigger on channel B

      //setOutput(B, GAIN_2, NO_SHTDWN, 2000);
      //B or A to choose channels
      //GAIN_2 or GAIN_1 to choose 1x gain or 2x gain (I always use 2x)
      //NO_SHTDWN or SHTDWN to disable the DAC_CS
      //2000 - is the value out of 4096 to send out to the DAC
      
    }
    

    updatevalues();//subroutine to grab the 4 pot values
}
}


void updatevalues(void)
{
  unsigned char x;
   for(x=0;x<4;x++)
  {
    values[x]=analogRead(POTS[x]);  
  }

  //you can use the map() function here to scale the pot values up or down
  //e.g. sequence_length = map(values[0],0,1023,0,16);

}

void SendPulse (boolean chan) //Sends a 40ms trigger. This function blocks
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
