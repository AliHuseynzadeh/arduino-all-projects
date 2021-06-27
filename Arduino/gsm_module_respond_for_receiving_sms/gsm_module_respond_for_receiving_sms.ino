

string s = "scott>=tiger>=mushroom";
string delimiter = ">=";

size_t pos = 0;
std::string token;
while ((pos = s.find(delimiter)) != string) {
    token = s.substr(0, pos);
    cout << token << endl;
    s.erase(0, pos + delimiter.length());
}
cout << s << endl;
#define RELE1 7
#define RELE2 8
#include <SoftwareSerial.h>      
#include <Wire.h>
#include<sms.h>
#include<sim900.h>
 

char Received_SMS;              //Here we store the full received SMS (with phone sending number and date/time) as char
short RELE1_ON=-1,RELE1_OF=-1;     
short RELE2_ON=-1,RELE2_OF=-1;
String Data_SMS;               

SoftwareSerial sim800l(2,3);                    
void setup()
{
   pinMode(RELE1,OUTPUT);
    pinMode(RELE2,OUTPUT);
  sim800l.begin(9600);
  Serial.begin(9600);     
 
  Serial.println("Starting ...");
  delay(3000);       
  ReceiveMode();       //Calling the function that puts the SIM800L moduleon receiving SMS mode
  
}



void loop() {
  
  String RSMS;             //We add this new variable String type, and we put it in loop so everytime gets initialized
                           //This is where we put the Received SMS, yes above there's Recevied_SMS variable, we use a trick below
                           //To concatenate the "char Recevied_SMS" to "String RSMS" which makes the "RSMS" contains the SMS received but as a String
                           //The recevied SMS cannot be stored directly as String
  
    while(sim800l.available()>0){       //When SIM800L sends something to the Arduino... problably the SMS received... if something else it's not a problem
        
        Received_SMS=sim800l.read();  //"char Received_SMS" is now containing the full SMS received
        Serial.print(Received_SMS);   //Show it on the serial monitor (optional)     
        RSMS.concat(Received_SMS);    //concatenate "char received_SMS" to RSMS which is "empty"
        RELE1_ON=RSMS.indexOf("KON");   
        RELE1_OF=RSMS.indexOf("KOFF");   //"indexOf function looks for the substring "x" within the String (here RSMS) and gives us its index or position
        RELE2_ON=RSMS.indexOf("WON");
        RELE2_OF=RSMS.indexOf("WOFF");                                    //For example if found at the beginning it will give "0" after 1 character it will be "1"
                                          //If it's not found it will give "-1", so the variables are integers
        
    }
    
  if(RELE1_ON!=-1){                         //If "RELE1" word is found within the SMS, it means that RELE1_ON have other value than -1 so we can proceed
    Serial.println("found RELE1 ONNNN");          //Shows on the serial monitor "found RELE1" (optional)
  //  Serial.print("RELE1 Temperature = ");  Serial.print("*C      RELE111 Humidity = ");  Serial.println(" %");
                                          //Show it on the serial monitor also optional
digitalWrite (RELE1,HIGH);
  //  Data_SMS = "RELE1\nTemp = "+String('i',1)+" C"+" \nHumidity ="+String('l',1)+" %";       //Prepare the SMS to send, it contains some strings like "RELE1" "Temperature"...
                                                                                         //And then the values read
    
  // Send_Data();                      //This function set the sending SMS mode, prepare the phone number to which we gonna send, and send "Data_SMS" String
    ReceiveMode();                   //Come back to Receving SMS mode and wait for other SMS
    
     RELE1_OF=-1;
  RELE1_ON=-1;
  RELE2_OF=-1;
  RELE2_ON=-1;                      //Maybe not required... I did a lot of tests and maybe at the beginning the RSMS string kept concating and MLX word was kept there
                                    //And at this point I'm too lazy to reupload the code without it and test...
  }


  if(RELE1_OF!=-1){                   //Same thing if the "MLX" word is found, Serial.print things are optionnal
  delay(1000);
  Serial.println("found Turn OF");
  digitalWrite (RELE1,LOW);
 // Send_Data();
  ReceiveMode();
 RELE1_OF=-1;
  RELE1_ON=-1;
  RELE2_OF=-1;
  RELE2_ON=-1;
}

  if(RELE2_ON!=-1){                         
    Serial.println("found RELE1");         
  //  Serial.print("RELE1 Temperature = ");  Serial.print("*C      RELE111 Humidity = ");  Serial.println(" %");
                                          //Show it on the serial monitor also optional
digitalWrite (RELE2,HIGH);
   // Data_SMS = "RELE1\nTemp = "+String('i',1)+" C"+" \nHumidity ="+String('l',1)+" %";       //Prepare the SMS to send, it contains some strings like "RELE1" "Temperature"...
      delay(1000);
  Serial.println("found TURN ONNNNNN ");                                                                                   //And then the values read
    
   // Send_Data();                      //This function set the sending SMS mode, prepare the phone number to which we gonna send, and send "Data_SMS" String
    ReceiveMode();                   //Come back to Receving SMS mode and wait for other SMS
    RELE1_OF=-1;
  RELE1_ON=-1;
    RELE2_ON=-1;                      
    RELE2_OF=-1;                    
                             
  }


     if(RELE2_OF!=-1){                 
  delay(1000);
  Serial.println("found TURN OFFFFF ");
  digitalWrite (RELE2,LOW);
  //Send_Data();
  ReceiveMode();
  RELE1_OF=-1;
  RELE1_ON=-1;
  RELE2_OF=-1;
  RELE2_ON=-1;
}  

}


void Serialcom() //This is used with ReceiveMode function, it's okay to use for tests with Serial monitor
{
  delay(500);
  while(Serial.available())                                                                      
  {
    sim800l.write(Serial.read());//Forward what Serial received to Software Serial Port
  }
  while(sim800l.available())                                                                      
  {
    Serial.write(sim800l.read());//Forward what Software Serial received to Serial Port
  }
}

void ReceiveMode(){       //Set the SIM800L Receive mode
  
  sim800l.println("AT"); //If everything is Okay it will show "OK" on the serial monitor
  Serialcom();
  sim800l.println("AT+CMGF=1"); // Configuring TEXT mode
  Serialcom();
  sim800l.println("AT+CNMI=2,2,0,0,0"); //Configure the SIM800L on how to manage the Received SMS... Check the SIM800L AT commands manual
  Serialcom();
  sim800l.print("AT+CMGD=4");   
  Serialcom();
}

void Send_Data()
{
  Serial.println("Sending Data...");     //Displays on the serial monitor...Optional
  sim800l.print("AT+CMGF=1\r");          //Set the module to SMS mode
  delay(100);
 sim800l.print("AT+CMGS=\"+***********\"\r");  //Your phone number don't forget to include your country code example +212xxxxxxxxx"  
 delay(500);
  
  
//  sim800l.print(Data_SMS);  //This string is sent as SMS        delay(500);
  sim800l.print((char)26);//Required to tell the module that it can send the SMS
  delay(500);
  sim800l.println();
  Serial.println("Data Sent.");
  delay(500);

}
