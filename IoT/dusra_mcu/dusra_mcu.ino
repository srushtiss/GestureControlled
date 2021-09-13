
#include <SPI.h>
#include <ESP8266WiFi.h>
#include <Servo.h>
#include <WiFiClient.h> 
#include <ESP8266WebServer.h>
#include <ESP8266HTTPClient.h>



#define LIGHT_CONTROL 5
#define FAN_CONTROL 4

#define WINDOW_CONTROL 12


byte ledPin = 2;
char ssid[] = "Srkd";           // SSID of your home WiFi
char pass[] = "arsenal03";            // password of your home WiFi

unsigned long askTimer = 0;


//Web/Server address to read/write from
const char *host = "192.168.0.104";

 Servo motor;

/*IDHAR THE ASKTIMER IS FOR CONNECTION TIMED OUT WALA SCENE
 * NEXT IS THE SERVER KE BAAD JO IP ADDR HAI
 * THAT IS APNE GLOVE KE NODEMCU KA IP
 * ISSE CONNECTION BAN GAYA
 */


IPAddress server(192,168,0,106);       // the fix IP address of the server
WiFiClient client;

void setup() {
  Serial.begin(115200);               // only for debug
  WiFi.begin(ssid, pass);             // connects to the WiFi router
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(500);
  }
  Serial.println("Connected to wifi");
  Serial.print("Status: "); Serial.println(WiFi.status());    // Network parameters
  Serial.print("IP: ");     Serial.println(WiFi.localIP());
  Serial.print("Subnet: "); Serial.println(WiFi.subnetMask());
  Serial.print("Gateway: "); Serial.println(WiFi.gatewayIP());
  Serial.print("SSID: "); Serial.println(WiFi.SSID());
  Serial.print("Signal: "); Serial.println(WiFi.RSSI());
  pinMode(ledPin, OUTPUT);
  pinMode(LIGHT_CONTROL, OUTPUT);
  pinMode(FAN_CONTROL, OUTPUT);

  pinMode(WINDOW_CONTROL, OUTPUT);
  motor.attach(12);
  motor.write(0);
}

void loop () {

        /* HERE THE DB KA CODE IS COMMENTED OUT CAUSE
       *  UNNECESSARY QUERIES FIRE HOTA HAI NAI TOH
       *  
       */

      
      /*HTTPClient http;    //Declare object of class HTTPClient
      String name_of_device,data;
      int status_of_device;
      name_of_device="A";
      status_of_device=1;
      data= "name_of_light="+name_of_device+"&status="+status_of_device;
      */


  client.connect(server, 80);   // Connection to the server


 /*SAME PORTS PE UPAR CONNECTION HUA
  * IDHAR WOH "ON\R" JO BHEJA THA FROM LETS SEE IF THIS RUNS 2 SE
  * WOH STRING IS READ NICHE AND CHECK KIYA HAI KI
  * IS IT ON ?
  * IF YES, THEN IDHAR SE ACTUAL LIGHT ON KARO
  * IDHAR SIRF ITNA HI KAAM HAI
  * ZYADA NAI
  * USKE BAAD THE CONSOLE IS FLUSHED
  * TAAKI NEW DATA AND PURANA DATA MERGE NA HO
  */
  String glove_data = client.readStringUntil('\r');   // receives the answer from the sever
  Serial.println(glove_data);


  
  if(glove_data=="LIGHT_ON"){
    Serial.println(glove_data);
    digitalWrite(ledPin, LOW);
    Serial.println("ON KIYA LIGHT");
    digitalWrite(LIGHT_CONTROL,LOW);  
    client.flush();
    delay(3000);  
    }
  if(glove_data=="LIGHT_OFF"){
    Serial.println(glove_data);
    digitalWrite(ledPin, HIGH);
    Serial.println("OFF KIYA LIGHT");
    digitalWrite(LIGHT_CONTROL,HIGH);
    client.flush();
    delay(3000);
  }
  
    if(glove_data=="FAN_ON"){
    digitalWrite(FAN_CONTROL, HIGH);
    Serial.println(glove_data);
    digitalWrite(ledPin, LOW);
    Serial.println("ON KIYA FAN");  
    client.flush();
    delay(3000);
    }
  if(glove_data=="FAN_OFF"){
    Serial.println(glove_data);
    digitalWrite(ledPin, HIGH);
    digitalWrite(FAN_CONTROL, LOW);
    Serial.println("OFF KIYA FAN");
    client.flush();
    delay(3000);
  }

    if(glove_data=="WINDOW_ON"){
    Serial.println(glove_data);
    digitalWrite(ledPin, LOW);
    motor.write(180);
    Serial.println("ON KIYA WINDOW");  
    client.flush();
    delay(3000);
    }
  if(glove_data=="WINDOW_OFF"){
    Serial.println(glove_data);
    digitalWrite(ledPin, HIGH);
    motor.write(0);
    Serial.println("OFF KIYA WINDOW");
    client.flush();
    delay(3000);
  }

 /* if(glove_data=="FAN_SLOW" && fan_flag==1){
    Serial.println(glove_data);
    digitalWrite(ledPin, HIGH);
    Serial.println("SLOW KIYA FAN");
    client.flush();
  }

  if(glove_data=="FAN_SPEED" && fan_flag==1){
    Serial.println(glove_data);
    digitalWrite(ledPin, HIGH);
    Serial.println("SPEED KIYA FAN");
    client.flush();
  }
*/



if(glove_data=="FAN_SLOW"){
                 
analogWrite(FAN_CONTROL,200);
Serial.println("SLOW KIYA FAN");
delay(3000);
}
              
 else if(glove_data=="FAN_SPEED" ){
digitalWrite(FAN_CONTROL, HIGH);
                  
Serial.println("SPEED KIYA FAN");
 delay(3000);
}


               /* http.begin("http://192.168.0.103/server_db_communication.php");
              http.addHeader("Content-Type", "application/x-www-form-urlencoded");
              int httpCode = http.POST(data);   //Send the request
              String payload = http.getString();    //Get the response payload
              Serial.println(httpCode);   //Print HTTP return code
              Serial.println(payload);    //Print request response payload
              Serial.println("data inserted");
              http.end();  //Close connection*/


}
