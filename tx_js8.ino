#include <AD9850SPI.h>
#include <SPI.h>

const int W_CLK_PIN = 13;
const int FQ_UD_PIN = 8;
const int RESET_PIN = 9;

double freq = 3578000;
//double freq = 7044000;
//double freq = 28078000;
double offset = 1000;
double trimFreq = 125000000;
double fout;
int rxbyte;

int phase = 0;

void setup(){
  Serial.begin(19200);
  DDS.begin(W_CLK_PIN, FQ_UD_PIN, RESET_PIN);
  DDS.calibrate(trimFreq);
  DDS.setfreq(freq, phase);
  DDS.down();
}

//  DDS.up();
//  DDS.setfreq(freq, phase);
  
void loop(){

if(Serial.available() > 0){
   // Bytes do JS8 0..7, entoa uso 8 para ligar o DDS e PTT e 9 Para desligar
   rxbyte = Serial.read() - 48;
   if(rxbyte == 8){
    DDS.up();
   }else if(rxbyte ==9){
    DDS.down();
   }else {
      fout = freq + offset + (6.25 * rxbyte);
      DDS.setfreq(fout, phase);
      Serial.print("freq: ");
      Serial.println(fout);
   }
  }
}

  
  
