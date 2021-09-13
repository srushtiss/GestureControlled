#include <ESP8266WiFi.h>
#include <WiFiClient.h> 
#include <ESP8266WebServer.h>
#include <ESP8266HTTPClient.h>


// Set these to your desired credentials.
const char *ssid = "Srkd";  //ENTER YOUR WIFI SETTINGS
const char *password = "arsenal03";

//Web/Server address to read/write from
const char *host = "192.168.0.104";


/*IDHAR IP ADDR HARDCODE KIYA HAI
 * THIS IS DIFFERENT FROM NODEMCU TO DB WALA IN CAPS FOLDER
 * PEHLE WOH BHI REFER KAROOOOOO
 * BAAKI SAB STUFF IS THE SAME
 */
IPAddress ip(192, 168, 0, 106);             // IP address of the server
IPAddress gateway(192,168,0,1);           // gateway of your network
IPAddress subnet(255,255,255,0);            // subnet mask of your network


WiFiServer server(80);    
/*SERVER PORT 80 SOCKET PROGRAMMING MAI BHI USE KIYA HAI
 * WAHI CHEEZ HAI
 */

 /*YE SAB LIBRARIES AND CLASSES KA DOCUMENTATION IS WAAY TOO TECHNICAL
  * MERKO PEHLE TOH MIL HI NAI RAHA LMAO
  * PHIR JO MILA WOH SAMJHA NAI
  */

#define arduinoOlive 14
#define arduinoWhite 12
#define arduinoRed 13
#define arduinoBlack 15


void setup(){
    
      Serial.begin(115200);                   // only for debug
      WiFi.config(ip, gateway, subnet);       // forces to use the fix IP
   //Setting up WiFi
    
    
      
      WiFi.begin(ssid, password);     //Connect to your WiFi router
      Serial.println("");
    
      Serial.print("Connecting");
      // Wait for connection
      while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(".");
      }
      server.begin();

      Serial.println("Connected to wifi");
      Serial.print("Status: "); Serial.println(WiFi.status());    // Network parameters
      Serial.print("IP: ");     Serial.println(WiFi.localIP());
      Serial.print("Subnet: "); Serial.println(WiFi.subnetMask());
      Serial.print("Gateway: "); Serial.println(WiFi.gatewayIP());
      Serial.print("SSID: "); Serial.println(WiFi.SSID());
      Serial.print("Signal: "); Serial.println(WiFi.RSSI());
 
      pinMode(LED_BUILTIN,OUTPUT);
      pinMode(arduinoOlive,INPUT);
      pinMode(arduinoWhite,INPUT);
      pinMode(arduinoRed,INPUT);
      pinMode(arduinoBlack,INPUT);
      digitalWrite(LED_BUILTIN,HIGH);
      
}

void loop(){
      /* HERE THE DB KA CODE IS COMMENTED OUT CAUSE
       *  UNNECESSARY QUERIES FIRE HOTA HAI NAI TOH
       *  
       */

      
      HTTPClient http;    //Declare object of class HTTPClient
      String data;
      
      
      WiFiClient client = server.available();
      
      /*FIRST, THE CONNECTION BETWEEN CLIENT AND SERVER IS CHECKED
       * AGAR TRUE HUA TOH IF STATEMENT WILL BE EXECUTED
       * ISKE BAAD ITLL CHECK THE CONNECTION
       * PHIR ITLL CHECK THE ARDUINO KA TRIGGER
       * USKE BAAD ITLL FIRE A TRIGGER TO THE NODEMCU ON THE DEVICE
       * UDHAR, ITLL SEND "ON" WHICH WILL BE READ BY THE OTHER NODEMCU
       * BAAKI KA PART IN LETS SEE IF THIS RUNS 3
       * JO BHI LED TRIGGERS HAI
       * THEYRE JUST A WAY OF SHOWING KI COMMUNICATION IS HAPPENING BAS
       * NOT OF MUCH USE WAISE
       * 
       */
      
       if (client) {
          if (client.connected()) {
            int olive=0;
            int white=0;
            int red=0;
            int black=0;
            olive=digitalRead(arduinoOlive);
            white=digitalRead(arduinoWhite);
            red=digitalRead(arduinoRed);
            black=digitalRead(arduinoBlack);
            
            Serial.print(olive);Serial.print(white);Serial.print(red);Serial.println(black);
            if(olive==1 && white==0 && red==0 && black==0){
              Serial.println(" switched the LIGHT on");
              client.println("LIGHT_ON\r");
              //data= "status="+ON+"&name="+Bedroom1_light;
              data= "status=ON&name=Bedroom1_light";
              http.begin("http://192.168.0.104/change_status.php?status=ON&name=Bedroom1_light");
              http.addHeader("Content-Type", "application/x-www-form-urlencoded");
              int httpCode = http.POST(data);   //Send the request
              String payload = http.getString();    //Get the response payload
              Serial.println(httpCode);   //Print HTTP return code
              Serial.println(payload);    //Print request response payload
              Serial.println("data inserted");
              http.end();  //Close connection

              delay(3000);
              }
            if(olive==1 && white==1 && red==0 && black==0){
              Serial.println("nigga switched the LIGHT OFF");
              client.println("LIGHT_OFF\r");

              data= "status=OF&name=Bedroom1_light";
           
              http.begin("http://192.168.0.104/change_status.php?status=OF&name=Bedroom1_light");
              http.addHeader("Content-Type", "application/x-www-form-urlencoded");
              int httpCode = http.POST(data);   //Send the request
              String payload = http.getString();    //Get the response payload
              Serial.println(httpCode);   //Print HTTP return code
              Serial.println(payload);    //Print request response payload
              Serial.println("data inserted");
              http.end(); 
              //Close connection
             
             delay(3000);
             }
            if(olive==1 && white==1 && red==1 && black==0){
                Serial.println(" switched the FAN on");
              client.println("FAN_ON\r");
              
              data= "status=ON&name=Bedroom1_fan";
              http.begin("http://192.168.0.104/change_status.php?status=ON&name=Bedroom1_fan");
              http.addHeader("Content-Type", "application/x-www-form-urlencoded");
              int httpCode = http.POST(data);   //Send the request
              String payload = http.getString();    //Get the response payload
              Serial.println(httpCode);   //Print HTTP return code
              Serial.println(payload);    //Print request response payload
              Serial.println("data inserted");
              http.end();  //Close connection

              delay(3000);
             }
            if(olive==0 && white==1 && red==1 && black==1){
                      Serial.println(" switched the FAN FF");
              client.println("FAN_OFF\r");
              
              data= "status=ON&name=Bedroom1_fan";
              http.begin("http://192.168.0.104/change_status.php?status=OF&name=Bedroom1_fan");
              http.addHeader("Content-Type", "application/x-www-form-urlencoded");
              int httpCode = http.POST(data);   //Send the request
              String payload = http.getString();    //Get the response payload
              Serial.println(httpCode);   //Print HTTP return code
              Serial.println(payload);    //Print request response payload
              Serial.println("data inserted");
              http.end();  //Close connection

              delay(3000);
             }
            if(olive==1 && white==1 && red==0 && black==1){
                    Serial.println(" switched the WINDOW on");
              client.println("WINDOW_ON\r");
              
              data= "status=ON&name=Bedroom1_window";
              http.begin("http://192.168.0.104/change_status.php?status=ON&name=Bedroom1_window");
              http.addHeader("Content-Type", "application/x-www-form-urlencoded");
              int httpCode = http.POST(data);   //Send the request
              String payload = http.getString();    //Get the response payload
              Serial.println(httpCode);   //Print HTTP return code
              Serial.println(payload);    //Print request response payload
              Serial.println("data inserted");
              http.end();  //Close connection

              delay(3000);
             }
            if(olive==1 && white==0 && red==1 && black==1){
                        Serial.println(" switched the WINDOW ff");
              client.println("WINDOW_OFF\r");
              
              data= "status=ON&name=Bedroom1_window";
              http.begin("http://192.168.0.104/change_status.php?status=OF&name=Bedroom1_window");
              http.addHeader("Content-Type", "application/x-www-form-urlencoded");
              int httpCode = http.POST(data);   //Send the request
              String payload = http.getString();    //Get the response payload
              Serial.println(httpCode);   //Print HTTP return code
              Serial.println(payload);    //Print request response payload
              Serial.println("data inserted");
              http.end();  //Close connection

              delay(3000);
             }
             if(olive==0 && white==1 && red==0 && black==0){
              Serial.println("nigga slowed down the fan");
              client.println("FAN_SLOW\r"); 
             }
             if(olive==0 && white==0 && red==1 && black==0){
              Serial.println("nigga speed kar diya fan");
              client.println("FAN_SPEED\r"); 
             }
          }
          delay(3000);
               /* http.begin("http://192.168.0.103/server_db_communication.php");
              http.addHeader("Content-Type", "application/x-www-form-urlencoded");
              int httpCode = http.POST(data);   //Send the request
              String payload = http.getString();    //Get the response payload
              Serial.println(httpCode);   //Print HTTP return code
              Serial.println(payload);    //Print request response payload
              Serial.println("data inserted");
              http.end();  //Close connection*/
          client.stop(); 
        }

}
