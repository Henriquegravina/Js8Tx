#include <AD9850SPI.h>
#include <SPI.h>

// AD9850 config
const int W_CLK_PIN = 13;
const int FQ_UD_PIN = 8;
const int RESET_PIN = 9;
double trimFreq = 125000000;
int phase = 0;

//FS8 config
double qrg = 7044000;
double offset = 1000;
double fout;
int fs8_speed = 0;
double tx_time = 12.64; // Normal
double symbol_size = 6.25; // Normal
double qrg_correction = 188;


//Terminal Serial
String command;
char symbols[81];
int symbol;
int i;

// Hardware setup
int PTT_pin = 3;


void setup() {

  Serial.begin(19200); // Set serial speed
  Serial.println("Start");
  Serial.println(sizeof(symbols));
  
  pinMode(PTT_pin,OUTPUT);
  digitalWrite(PTT_pin,LOW); // Set PTT pin to LOW

  // Config AD9850
  DDS.begin(W_CLK_PIN, FQ_UD_PIN, RESET_PIN);
  DDS.calibrate(trimFreq);
  DDS.setfreq(qrg, phase);
  DDS.down();

}



void txSymbols(){
  
  for(i = 0; i <= (sizeof(symbols)-1); i++){
    symbol = symbols[i];
    switch (symbols[i]) {
      case 8:
        Serial.print("TXON: ");
        digitalWrite(3,HIGH);
        DDS.up();
        break;
  
      case 9:
        Serial.println(" :TXOFF"); 
        DDS.down();
        digitalWrite(3,LOW);
        break;
  
      default:
        Serial.print(symbol);
        fout = (qrg - qrg_correction ) + offset + (symbol_size * symbol);
        DDS.setfreq(fout, phase);
        Serial.print("freq: ");
        Serial.println(fout);
        delay( (tx_time)/79 * 1000 );
    }
  }
}

void parseCommand(String com)
{
     String part1;
     String part2;

     part1 = com.substring(0, com.indexOf(" "));
     part2 = com.substring(com.indexOf(" ")+1);

     // symbols 811111111112222222222333333333344444444445555555555666666666677777777771111111119
     if(part1.equalsIgnoreCase(F("SYMBOLS"))){      
      Serial.print(F("Symbols: "));
      for(i = 0; i <= (sizeof(symbols)-1); i++){
        symbols[i] = part2[i] - 48;
      }
      txSymbols();
     
            
      // SSID Setup 
     }else if(part1.equalsIgnoreCase(F("QRG"))){
      qrg = part2.toDouble();
      Serial.print(F("QRG: "));
      Serial.println(qrg);
      
     }
     else if(part1.equalsIgnoreCase(F("OFFSET"))){
      offset = part2.toDouble();
      Serial.print(F("Offset: "));
      Serial.println(offset);
      
     }else if(part1.equalsIgnoreCase(F("SPEED"))){
      fs8_speed = part2.toInt();
      Serial.print(F("Speed: "));
      Serial.println(offset);
      if(fs8_speed == 4) { tx_time = 25.28; symbol_size = 3.125; } // Slow Mode Speed
      else if(fs8_speed == 0) { tx_time = 12.64; symbol_size = 6.25; } //Normal Mode Speed
      else if(fs8_speed == 1) { tx_time = 7.9; symbol_size = 10; } // Fast Mode Speed
      else if(fs8_speed == 2) { tx_time = 3.95; symbol_size = 20; } // Turbo Mode Speed
      
     }
     
}

void loop() {     
  if(Serial.available() > 0){
    char c = Serial.read();
    if( c == 13 ){
      parseCommand(command);
      command = "";
    }else if( c == 10) { /*Do nothing*/  // To use Line feed
    }else { command += c; 
    }
  }    
}
      
