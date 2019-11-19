#include <EEPROM.h>
int NUMBER_OF_CHANNELS = 8;
unsigned int dmxaddress = 4;
 int R=0;
 int G=0;                
 int B=0;
 int kanalR;
 int kanalG;
 int kanalB;
 int Rr=0;
 int Gg=0;
 int Bb=0;
int gecis;
int katman;
int sayac;
int sayac2;
int RX_PIN= 0;
int TX_PIN= 1;
volatile byte i = 0;
volatile byte dmxreceived = 0;
volatile unsigned int dmxcurrent = 0; 
volatile byte dmxvalue[8];     
volatile boolean dmxnewvalue = false; 
volatile byte zerocounter = 0;          
void newAddressWrite() {
  EEPROM.write(510, lowByte(dmxaddress));
  EEPROM.write(511, highByte(dmxaddress));
  for (byte i = 0; i < 15; i++) {
    digitalWrite(13, (i & 1));
    delay(100);
  }
}

void action() {
volatile byte color = dmxvalue[0];
volatile byte dimmervalue = dmxvalue[1];
volatile byte shutter = dmxvalue[2];

if ((color <=25) && (color >0)){
R=dimmervalue;
G=0;
B=0;
katman=0;
}
else if ((color <=50) && (color >25)){
R=0;
G=dimmervalue;
B=0;
katman=0;
}
else if ((color <=75) && (color >50)){
R=0;
G=0;
B=dimmervalue;
katman=0;
}
else if ((color <=100) && (color >75)){
R=dimmervalue;
G=dimmervalue;
B=0;
katman=0;
}
else if ((color <=125) && (color >100)){
R=dimmervalue;
G=0;
B=dimmervalue;
katman=0;
}
else if ((color <=150) && (color >125)){
R=0;
G=dimmervalue;
B=dimmervalue;
katman=0;
}    
else if ((color <=175) && (color >150)){
R=dimmervalue;
G=dimmervalue;
B=dimmervalue;
katman=0;
}              
else{
gecis =(int) (color-174);
switch (katman) {
case 0:
{
if (Rr >=255){
katman= 1;
}
else{
Rr = Rr +gecis;
Gg =0;
Bb = Bb -gecis ; 
}
break;
}
case 1:
{
if (Gg >=255){
katman= 2;
}
else{
Rr =Rr -gecis;
Gg =Gg +gecis;
Bb = 0; 
}
break;                                                          
}
case 2:
{
if (Bb >= 255){
katman= 0;
}
else{
Rr = 0;
Gg = Gg -gecis;
Bb = Bb +gecis; 
}
break;                                                        
}
default:
{
if (Rr >= 255){
katman= 1;
}
else{
Rr = Rr +gecis;
Gg =0;
Bb = Bb -gecis ; 
}
break;
}
}
R=Rr;
G=Gg;
B=Bb;
}

if (R<0){R=0;}
if (R>255) {R=255;}
if (G<0){G=0;}
if (G>255) {G=255;}
if (B<0){B=0;}
if (B>255) {B=255;}

kanalR= (int) R*dimmervalue/255;
kanalG= (int) G*dimmervalue/255;
kanalB= (int) B*dimmervalue/255;

analogWrite(5, kanalR);
analogWrite(6, kanalG);
analogWrite(9, kanalB);   

         if ((shutter<=100) && (shutter>=1)){
         sayac = sayac + random(100);
         if((sayac<(2*shutter))&&(sayac>=shutter)){  
        analogWrite(5, 0);
        analogWrite(6, 0);
        analogWrite(9, 0);
        }
          if(sayac>=(2*shutter)){
          sayac=0;
        }
        }
        else if ((shutter<=200) && (shutter>100)){
        sayac2 = sayac2 +15;
        if((sayac2<(2*(shutter-100)))&&(sayac2>=(shutter-100))){            
        analogWrite(5, 0);
        analogWrite(6, 0);
        analogWrite(9, 0);
        }
        if(sayac2>=(2*(shutter-100))){
          sayac2=0;
        }
        }
  return;
}
ISR(TIMER2_COMPA_vect) {
  if (bitRead(PIND, PIND0)) {
    zerocounter = 0;
  }
  else {
    zerocounter++;
    if (zerocounter == 20)
    {   
      bitClear(TIMSK2, OCIE2A);
      bitSet(UCSR0B, RXCIE0);
    }
  }
}
ISR(USART_RX_vect){
  dmxreceived = UDR0;
  dmxcurrent++;
  if(dmxcurrent > dmxaddress) {
    dmxvalue[i] = dmxreceived;
    i++;
    if(i == NUMBER_OF_CHANNELS) {
      bitClear(UCSR0B, RXCIE0); 
      dmxnewvalue = 1;.
    } 
  }
}
void setup() {
  pinMode(RX_PIN, INPUT);
  Serial.begin(250000);
  cli();
  bitClear(UCSR0B, RXCIE0);
  bitClear(TCCR2A, COM2A1);
  bitClear(TCCR2A, COM2A0);
  bitClear(TCCR2A, COM2B1);
  bitClear(TCCR2A, COM2B0);
  bitSet(TCCR2A, WGM21);
  bitClear(TCCR2A, WGM20);
  bitClear(TCCR2B, FOC2A);
  bitClear(TCCR2B, FOC2B);
  bitClear(TCCR2B, WGM22);
  bitClear(TCCR2B, CS22);
  bitClear(TCCR2B, CS21);
  bitSet(TCCR2B, CS20);
  OCR2A = 64;                
  bitClear(TIMSK2, OCIE2B);
  bitSet(TIMSK2, OCIE2A);
  bitClear(TIMSK2, TOIE2);         
  sei(); 
}

void loop()  {
  if (dmxnewvalue == 1) {
    action();                                                                                    
    dmxnewvalue = 0;
    dmxcurrent = 0;
    zerocounter = 0;
    i = 0;
    bitSet(TIMSK2, OCIE2A);
  }
}




