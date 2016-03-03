//nodeMCU v1.0 (black) with Arduino IDE
//stream temperature data DS18B20 with 1wire on ESP8266 ESP12-E (nodeMCU v1.0)
//shin-ajaran.blogspot.com
//nodemcu pinout https://github.com/esp8266/Arduino/issues/584
#include <ESP8266WiFi.h>


//Def
#define myPeriodic 15 //in sec | Thingspeak pub is 15sec
#define ONE_WIRE_BUS 2  // DS18B20 on arduino pin2 corresponds to D4 on physical board
//#define mySSR 0  // Solid State Relay on pin 0


float prevTemp = 0;
const char* server = "api.thingspeak.com";
String apiKey ="BR3J0H138NYRVRSR";
const char* MY_SSID = "mxedu"; 
const char* MY_PWD = "swjtumaker";
int sent = 0;


void connectWifi()
{
  Serial.print("Connecting to "+*MY_SSID);
  WiFi.begin(MY_SSID, MY_PWD);
  while (WiFi.status() != WL_CONNECTED) {
  delay(1000);
  Serial.print(".");
  }
  
  Serial.println("");
  Serial.println("Connected");
  Serial.println("");  
}//end connect


void setup() {
  Serial.begin(115200);
  connectWifi();
}




void sendTeperatureTS(float temp)
{  
   WiFiClient client;
  
   if (client.connect(server, 80)) { // use ip 184.106.153.149 or api.thingspeak.com
   Serial.println("WiFi Client connected ");
   
   String postStr = apiKey;
   postStr += "&field1=";
   postStr += String(temp);
   postStr += "\r\n\r\n";
   
   client.print("POST /update HTTP/1.1\n");
   client.print("Host: api.thingspeak.com\n");
   client.print("Connection: close\n");
   client.print("X-THINGSPEAKAPIKEY: " + apiKey + "\n");
   client.print("Content-Type: application/x-www-form-urlencoded\n");
   client.print("Content-Length: ");
   client.print(postStr.length());
   client.print("\n\n");
   client.print(postStr);
   delay(1000);
   
   }//end if
   sent++;
 client.stop();
}//end send


void loop() {
  float temp;
  temp = analogRead(A0);
  temp = temp * 0.0048828125*100;   //使用浮点数存储温度数据，温度数据由电压值换算得到
  Serial.print(String(sent)+" Temperature: ");
  Serial.println(temp);
  

  sendTeperatureTS(temp);
  int count = myPeriodic;
  while(count--)
  delay(1000);
}
