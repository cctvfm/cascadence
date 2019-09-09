#include <tinySPI.h>

//Euclidean Sequencer
//For Cascadence
//Developed with Modular Seattle for Velocity 2019

//euclid function modified from Tom Whitwell's amazing euclidean sequencer
//https://github.com/TomWhitwell/Euclidean-sequencer
    
#define MAXSTEPLENGTH 31  //+1 because 0 is a step = 32 is the step length
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

unsigned int values[2][4];
const int A = 0;
const int B = 1;
long int euclids[2];
unsigned char stepnumber=0;
unsigned char offset_stepnumber[2];
unsigned char seq_length[2];
unsigned char seq_offset[2];
unsigned char seq_density[2];
unsigned char seq_randomness[2];
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
while(1)
{
  
  if(digitalRead(CLK_IN) == LOW)  //we've received a clock pulse!
    {
      //calculate the actual step number for each sequencer
      offset_stepnumber[A]=stepnumber+seq_offset[A];
      if(offset_stepnumber[A]>=seq_length[A])
        offset_stepnumber[A]=offset_stepnumber[A] % seq_length[A];
        
      offset_stepnumber[B]=stepnumber+seq_offset[B];
      if(offset_stepnumber[B]>=seq_length[B])
        offset_stepnumber[B]=offset_stepnumber[B] % seq_length[B];

      if(bitRead(euclids[A],offset_stepnumber[A]) == 1) //if there's a pulse
        pulse=true;

      if (random(MAXSTEPLENGTH) <= seq_randomness[A] )  //or if there's a randomly generated pulse
        pulse = !pulse;
      if(pulse==1)  
        SendPulse(A); //send one

      if(bitRead(euclids[B],offset_stepnumber[B]) == 1) //if there's a pulse
        pulse = true;
      if (random(MAXSTEPLENGTH) <= seq_randomness[B] )
        pulse = !pulse;
      if(pulse == 1)
        SendPulse(B); //send one
      
      stepnumber++;
      if(stepnumber>MAXSTEPLENGTH)
      {
        stepnumber = 0;
      }
      while(digitalRead(CLK_IN) == LOW);
      
    }
    

    updatevalues(!digitalRead(SW));
}
}

// Euclid calculation function 
//Borrowed lovingly from TomWhitwell
//https://github.com/TomWhitwell/Euclidean-sequencer

uint64_t euclid(int n, int k){ // inputs: n=total, k=beats, o = offset
  int pauses = n-k;
  int pulses = k;
  int per_pulse = pauses/k;
  int remainder = pauses%pulses;  
  long int workbeat[n];
  long int outbeat;
  long int working;
  int workbeat_count=n;
  int a; 
  int b; 
  int trim_count;
  for (a=0;a<n;a++){ // Populate workbeat with unsorted pulses and pauses 
    if (a<pulses){
      workbeat[a] = 1;
    }
    else {
      workbeat [a] = 0;
    }
  }

  if (per_pulse>0 && remainder <2){ // Handle easy cases where there is no or only one remainer  
    for (a=0;a<pulses;a++){
      for (b=workbeat_count-1; b>workbeat_count-per_pulse-1;b--){
        workbeat[a]  = ConcatBin (workbeat[a], workbeat[b]);
      }
      workbeat_count = workbeat_count-per_pulse;
    }

    outbeat = 0; // Concatenate workbeat into outbeat - according to workbeat_count 
    for (a=0;a < workbeat_count;a++){
      outbeat = ConcatBin(outbeat,workbeat[a]);
    }
    return outbeat;
  }

  else { 


    int groupa = pulses;
    int groupb = pauses; 
    int iteration=0;
    if (groupb<=1){
    }
    while(groupb>1){ //main recursive loop


      if (groupa>groupb){ // more Group A than Group B
        int a_remainder = groupa-groupb; // what will be left of groupa once groupB is interleaved 
        trim_count = 0;
        for (a=0; a<groupa-a_remainder;a++){ //count through the matching sets of A, ignoring remaindered
          workbeat[a]  = ConcatBin (workbeat[a], workbeat[workbeat_count-1-a]);
          trim_count++;
        }
        workbeat_count = workbeat_count-trim_count;

        groupa=groupb;
        groupb=a_remainder;
      }


      else if (groupb>groupa){ // More Group B than Group A
        int b_remainder = groupb-groupa; // what will be left of group once group A is interleaved 
        trim_count=0;
        for (a = workbeat_count-1;a>=groupa+b_remainder;a--){ //count from right back through the Bs
          workbeat[workbeat_count-a-1] = ConcatBin (workbeat[workbeat_count-a-1], workbeat[a]);

          trim_count++;
        }
        workbeat_count = workbeat_count-trim_count;
        groupb=b_remainder;
      }




      else if (groupa == groupb){ // groupa = groupb 
        trim_count=0;
        for (a=0;a<groupa;a++){
          workbeat[a] = ConcatBin (workbeat[a],workbeat[workbeat_count-1-a]);
          trim_count++;
        }
        workbeat_count = workbeat_count-trim_count;
        groupb=0;
      }

      else {
        //        Serial.println("ERROR");
      }
      iteration++;
    }


    outbeat = 0; // Concatenate workbeat into outbeat - according to workbeat_count 
    for (a=0;a < workbeat_count;a++){
      outbeat = ConcatBin(outbeat,workbeat[a]);
    }




    return outbeat;

  }
}
void updatevalues(boolean chan)
{
  unsigned char x;
   for(x=0;x<4;x++)
  {
    values[chan][x]=analogRead(POTS[x]);  //read the 4 pots so we don't have junk for the first sequence
  }

  seq_length[chan]=map(values[chan][0],0,1023,1,MAXSTEPLENGTH+1);
  seq_density[chan]=map(values[chan][1],0,1023,1,seq_length[chan]);
  seq_offset[chan]=map(values[chan][2],0,1023,0,seq_length[chan]);
  seq_randomness[chan]=map(values[chan][3],0,1023,0,MAXSTEPLENGTH);

  if(seq_density[chan]>seq_length[chan])
    seq_density[chan]=seq_length[chan];
  euclids[chan]=euclid(seq_length[chan],seq_density[chan]);
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

// Function to concatenate two binary numbers bitwise 
long int ConcatBin(uint64_t bina, uint64_t binb){
  int binb_len=findlength(binb);
  long int sum=(bina<<binb_len);
  sum = sum | binb;
  return sum;
}
// Function to find the binary length of a number by counting bitwise 
int findlength(long int bnry){
  boolean lengthfound = false;
  int length=1; // no number can have a length of zero - single 0 has a length of one, but no 1s for the sytem to count
  for (int q=32;q>=0;q--){
    int r=bitRead(bnry,q);
    if(r==1 && lengthfound == false){
      length=q+1;
      lengthfound = true;
    }
  }
  return length;
}
