#include <SoftwareSerial.h>

#define DEBUG true
#define LED 40
#define two 42
#define three 44
#define four 46

SoftwareSerial esp8266(10,11); // make RX Arduino line is pin 2, make TX Arduino line is pin 3.
                            // This means that you need to connect the TX line from the esp to the Arduino's pin 2
                            // and the RX line from the esp to the Arduino's pin 3
void setup()
{
  pinMode(LED,OUTPUT);
  pinMode(two,OUTPUT);
  pinMode(three,OUTPUT);
  pinMode(four,OUTPUT);
 Serial.begin(9600);
 esp8266.begin(9600); // your esp's baud rate might be different
 
 sendData("AT+RST\r\n",2000,DEBUG); // reset module
 sendData("AT+CWMODE=2\r\n",1000,DEBUG); // configure as access point
 sendData("AT+CIFSR\r\n",1000,DEBUG); // get ip address
 sendData("AT+CIPMUX=1\r\n",1000,DEBUG); // configure for multiple connections
 sendData("AT+CIPSERVER=1,80\r\n",1000,DEBUG); // turn on server on port 80
 
    Serial.println("server started running on socket # 192.168.4.1:80");

}

void loop()
{
     if(esp8266.available()) // check if the esp is sending a message 
     {
//        String cc="";
         while(esp8266.available()){
           // The esp has data so display its output to the serial window 
           char c = esp8266.read(); // read the next character.
           Serial.write(c);
//           cc+=c;
         }
//        Serial.print(cc);
//        delay(1000);
    
       if(esp8266.find("+IPD,"))
       {
        delay(100);
    
        int connectionId = esp8266.read()-48; // subtract 48 because the read() function returns 
                                              // the ASCII decimal value and 0 (the first decimal number) starts at 48
        Serial.print("conn id = ");
        Serial.println(connectionId);
        
        String msg="";
        while(esp8266.available()){
          char c=esp8266.read();
          msg+=c;
        }
        Serial.print("msg = ");
        Serial.println(msg);

        String vv=msg.substring(10,16);
        Serial.print("vv = ");
        Serial.println(vv);
        vv.trim();
        /////////////////////////////////////////////////////////
         if(vv.length()>0){
          if(vv=="sw1:on"){
            Serial.println("switch 1 is turned on");
            digitalWrite(LED,HIGH);

          }
          if(vv=="sw1:of"){
            Serial.println("switch 1 is turned off");
            digitalWrite(LED,LOW);

          }
          if(vv=="sw2:on"){
            Serial.println("switch 2 is turned on");
            digitalWrite(two,HIGH);

          }
          if(vv=="sw2:of"){
            Serial.println("switch 2 is turned off");
            digitalWrite(two,LOW);

          }
          if(vv=="sw3:on"){
            Serial.println("switch 3 is turned on");
            digitalWrite(three,HIGH);

          }
          if(vv=="sw3:of"){
            Serial.println("switch 3 is turned off");
            digitalWrite(three,LOW);

          }
          if(vv=="sw4:on"){
            Serial.println("switch 4 is turned on");
            digitalWrite(four,HIGH);

          }
          if(vv=="sw4:of"){
            Serial.println("switch 4 is turned off");
            digitalWrite(four,LOW);
            

          }
          if(vv=="sw_:on"){
            Serial.println("all switches are turned on");
            digitalWrite(LED,HIGH);
            digitalWrite(two,HIGH);
            digitalWrite(three,HIGH);
            digitalWrite(four,HIGH);

          }
          if(vv=="sw_:of"){
            Serial.println("all switches are turned off");
            digitalWrite(LED,LOW);
            digitalWrite(two,LOW);
            digitalWrite(three,LOW);
            digitalWrite(four,LOW);

          }
        }

        ///////////////////////////////////////////////////////////
    
        String closeCommand = "AT+CIPCLOSE="; 
        closeCommand+=connectionId; // append connection id
        closeCommand+="\r\n";
        
        sendData(closeCommand,3000,DEBUG);
       }
   }
}


String sendData(String command, const int timeout, boolean debug)
{
   String response = "";
   Serial.print("command => ");
   esp8266.print(command); // send the read character to the esp8266
   
   long int time = millis();
   
   while( (time+timeout) > millis())
   {
     while(esp8266.available())
     {
       
       // The esp has data so display its output to the serial window 
       char c = esp8266.read(); // read the next character.
       response+=c;
     }  
   }
   
   if(debug)
   {
    Serial.print("response => ");
     Serial.print(response);
   }
   
   return response;
}

