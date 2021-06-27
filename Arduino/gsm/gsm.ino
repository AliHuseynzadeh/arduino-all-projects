#define RELE1 13
#define RELE2 8
#include <SoftwareSerial.h>
 String gelen="";
 char c;
SoftwareSerial gsm(2,3);
void setup(){
gsm.begin(9600); //gsm baslat
delay(2000);
Serial.begin(9600);
pinMode(RELE1,OUTPUT);
pinMode(RELE2,OUTPUT);
delay(2000);

 ///////////////////////MESAJ////////////
 
 gsm.println("AT"); //"OK" GELDISE GSM DUZGUN ISLEYIR
 Serialcom();
 gsm.println("AT+CMGF=1"); //TEXTI CONFUIGURE
Serialcom();
// gsm.println("AT+ CNMI=2,2,0,0,0");     //THE WAY   TO HANDLE THE SMS RECEIVED , CHECK THE MODULE MANUAL, TO SUM UP: store the text or transfer it to serial or ther number
// Serialcom();  


/////////////////zeng////////////////////
 gsm.println("ATS0=1");        //bir calmada auto acma
Serialcom();
 gsm.println("AT+DDET=1");      //DTMF MODU                 // "DDET" FOR SIM MODULE
Serialcom();
 gsm.println("AT+CMGD=1,4");   //SILMEK UCUN
Serialcom();
  }
  void loop(){
        if (gsm.available()){
      while(gsm.available()){
        delay(13);
         c=gsm.read();
        gelen+=c;
        }
      Serial.print("gelen data: ");    Serial.println(gelen);
     if(gelen.indexOf("1")){     
           Serial.println("led yandi "); 
           digitalWrite(RELE1,HIGH);
           gelen="";
        }
  else   if (gelen.indexOf("2")){
         Serial.println("led sondu: "); 
           digitalWrite(RELE1,LOW);
            gelen="";
        }
    else  if(gelen.indexOf("3")){     
           Serial.println("led2 yandi "); 
           digitalWrite(RELE2,HIGH);
           gelen="";
        }
   else  if (gelen.indexOf("4")){
         Serial.println("led2 sondu: "); 
           digitalWrite(RELE2,LOW);
            gelen="";
        }
       
                     }    
      
      }
      void Serialcom(){
        delay(500);
        while (Serial.available()){ 
          gsm.write(Serial.read()); //forward what serial received to software serial port
          }
          while (gsm.available()){
            Serial.write(gsm.read());  //forward what software serial received to serial port
            }
        }
