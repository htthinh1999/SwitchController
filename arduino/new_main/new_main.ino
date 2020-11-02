#include <SoftwareSerial.h>

int SW_1 = 10;
int SW_2 = 11;
int SW_3 = 12;
int SW_4 = 13;
int touch_1 = 4;

//String ssid = "Mon Mina";
//String password = "tamsotam";
String ssid = "MON - NIO - PUPPY 2.4GHz";
String password = "44444444";
String server = "switchcontrol.000webhostapp.com";
String mainURI = "/index.php";
String dataURI = "/data.json";
String wifiToConnect = "\""+ssid+"\",\""+password+"\"";

int delay_speed = 1000;
int delay_1x = delay_speed;
int delay_2x = 2*delay_speed;
int delay_3x = 3*delay_speed;
int delay_4x = 4*delay_speed;
int delay_5x = 5*delay_speed;
int delay_10x = 10*delay_speed;
int delay_15x = 10*delay_speed;

SoftwareSerial esp8266(2,3);

void setup()
{
  Serial.begin(9600);
  esp8266.begin(9600);

  pinMode(touch_1, INPUT);
  
  connectWifi(ssid, password);
}
 
void loop()
{
  Serial.println(digitalRead(touch_1));
//  testESPInOut();

  //Send data to website
//  String data = "sw1=sw1_off&sw2=sw2_off&sw3=sw3_off&sw4=sw4_off";
//  sendDataToWebsite(server, mainURI, data);
//  delay(60000);
  //Get data from website
//  getDataFromWebsite(server, dataURI);
//  delay(delay_5x);
}

void connectWifi(String ssid, String password)
{
  sendATcommand("AT+RST", delay_2x); // Reset ESP8266
  sendATcommand("AT+CWMODE=1", delay_1x); // Set ESP8266 mode with 1: Station, 2: Access Point, 3: Both
  Serial.println("Connecting to wifi \""+ssid+"\",\""+password+"\"");
  sendATcommand("AT+CWJAP=\""+ssid+"\",\""+password+"\"", delay_15x);
  Serial.println("Wifi connected!");
}

void sendDataToWebsite(String server, String uri, String data){
  Serial.println("Connecting to website \""+server+"\"");
  sendATcommand("AT+CIPSTART=\"TCP\",\"" + server + "\",80", delay_2x);
  Serial.println("Website connected!");

  Serial.println("POST request with data: " + data);
  String postRequest =
    "POST " + uri + " HTTP/1.0\r\n" +
    "Host: " + server + "\r\n" +
    "Accept: *" + "/" + "*\r\n" +
    "Content-Length: " + data.length() + "\r\n" +
    "Content-Type: application/x-www-form-urlencoded\r\n" +
    "\r\n" + data;

  esp8266.print("AT+CIPSEND=");
  esp8266.println(String(postRequest.length()));
  esp8266.println(postRequest);
  delay(1000);
  esp8266.println("AT+CIPCLOSE");

  Serial.println("Send data complete!\n");
}

void getDataFromWebsite(String server, String uri){
  Serial.println("Connecting to website \"api.thingspeak.com\"");
  sendATcommand("AT+CIPSTART=\"TCP\",\"api.thingspeak.com\",80", delay_2x);
  Serial.println("Website connected!");

  String getRequest = "GET /apps/thinghttp/send_request?api_key=3WHC8COD0U68VD2N\r\nHost: api.thingspeak.com\r\n";

  esp8266.print("AT+CIPSEND=");
  getESPData();
  esp8266.println(String(getRequest.length()));
  getESPData();
  esp8266.println(getRequest);
  getESPData();
  delay(1000);
  String rs = getESPData();
  String sw1 = ((rs.indexOf("checked")!=-1)?"sw1_on":"sw1_off");
  Serial.println(rs);
  esp8266.println("AT+CIPCLOSE");

  Serial.println("Get data complete!\n");
  Serial.println("Data: " + sw1);
}

String getESPData(){
  if(esp8266.available())
  {
    String rs = "";
    while(esp8266.available())
    {
      char c = esp8266.read();
      //Serial.write(c);
      rs += String(c);
    }
    return rs;
  }
  return "\n";
}

void sendATcommand(String msg, int dt){
  esp8266.println(msg);
  delay(dt);
  getESPData();
}


void testESPInOut(){
  if(esp8266.available())
  {
    while(esp8266.available())
    {
      char c = esp8266.read();
      Serial.write(c);
    }  
  }
  if(Serial.available())
  {
    delay(1000); 
    String command="";
    
    while(Serial.available())
    {
      command+=(char)Serial.read();
    }
    esp8266.println(command);
  }
}
