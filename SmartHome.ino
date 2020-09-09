#include <SoftwareSerial.h>
#define DEBUG true
SoftwareSerial wifi(2,3); // RX, TX

void setup() {
   Serial.begin(9600);
   wifi.begin(9600);

   pinMode(10,OUTPUT);
   digitalWrite(10,HIGH);
   pinMode(11,OUTPUT);
   digitalWrite(11,HIGH);
   pinMode(12,OUTPUT);
   digitalWrite(12,HIGH);
   pinMode(13,OUTPUT);
   digitalWrite(13,HIGH);

   // Reset modul
   sendCommand("AT+RST\r\n",2000,DEBUG);
   // Konfigurasikan sebagai Akses poin
   sendCommand("AT+CWMODE=3\r\n",1000,DEBUG);
   // Sesuaikan dengan SSID dan Password
   sendCommand("AT+CWJAP=\"Kel-Cemara\",\"17041994aini\"\r\n",10000,DEBUG);
   // Mendapatkan IP adress
   sendCommand("AT+CIFSR\r\n",1000,DEBUG);
   // Konfigurasikan untuk multiple connection
   sendCommand("AT+CIPMUX=1\r\n",1000,DEBUG);
   // Aktifkan server pada port 80
   sendCommand("AT+CIPSERVER=1,80\r\n",1000,DEBUG);
   Serial.println("Server sudah siap!");
    }

void loop() {
  // Cek jika ESP mengirimkan pesan
  if(wifi.available()) {

    if(wifi.find("+IPD,")) {
     delay(1000); 
     int connectionId = wifi.read()-48;     
     wifi.find("pin=");
     int pinNumber = (wifi.read()-48)*10;
     pinNumber += (wifi.read()-48);
           
     int secondNumber = (wifi.read()-48);
     if(secondNumber>=0 && secondNumber<=9)
      {
        pinNumber*=10;
        pinNumber +=secondNumber;
      }
      pinMode(pinNumber,OUTPUT);
     digitalWrite(pinNumber, !digitalRead(pinNumber));  
    
     String closeCommand = "AT+CIPCLOSE="; 
     closeCommand+=connectionId; 
     closeCommand+="\r\n";
     sendCommand(closeCommand,1000,DEBUG); 
    }
  }
}


String sendCommand(String command, const int timeout, boolean debug) 
{
    String response = "";
    wifi.print(command);
    long int time = millis();
    
    while( (time+timeout) > millis())
    {
      while(wifi.available())
      {
        char c = wifi.read();
        response+=c;
      }  
    }
    
    if(debug) {
      Serial.print(response);
    }
    return response;
}
