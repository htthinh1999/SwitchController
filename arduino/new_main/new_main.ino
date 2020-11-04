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
String mainServer = "switchcontrol.000webhostapp.com";
String mainURI = "/index.php";
String dataServer = "api.thingspeak.com";
String dataURI = "/apps/thinghttp/send_request?api_key=3WHC8COD0U68VD2N";
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

static bool sw1_on = false;

void connectWifi(String ssid, String password)
{
  sendATcommand("AT+RST", delay_2x); // Reset ESP8266
  sendATcommand("AT+CWMODE=1", delay_1x); // Set ESP8266 mode with 1: Station, 2: Access Point, 3: Both
  Serial.println("Connecting to wifi \""+ssid+"\",\""+password+"\"");
  sendATcommand("AT+CWJAP=\""+ssid+"\",\""+password+"\"", delay_15x);
  Serial.println("Wifi connected!");
}

void sendDataToWebsite(){
  if(digitalRead(touch_1)){
    Serial.println("\nSending data");
    digitalWrite(13, HIGH);
    sw1_on = !sw1_on;
    
    String sw1Data = (sw1_on)?"on":"off";
    String data = "sw1=sw1_"+sw1Data+"&sw2=sw2_off&sw3=sw3_off&sw4=sw4_off";
    Serial.println("Connecting to website \""+mainServer+"\"");
    sendATcommand("AT+CIPSTART=\"TCP\",\"" + mainServer + "\",80", delay_2x);
    Serial.println("Website connected!");
  
    Serial.println("POST request with data: " + data);
    String postRequest =
      "POST " + mainURI + " HTTP/1.0\r\n" +
      "Host: " + mainServer + "\r\n" +
      "Accept: *" + "/" + "*\r\n" +
      "Content-Length: " + data.length() + "\r\n" +
      "Content-Type: application/x-www-form-urlencoded\r\n" +
      "\r\n" + data;
  
    esp8266.print("AT+CIPSEND=");
    esp8266.println(String(postRequest.length()));
    esp8266.println(postRequest);
    delay(delay_1x);
    esp8266.println("AT+CIPCLOSE");
  
    Serial.println("Send data complete!");
    
    digitalWrite(13, LOW);
  }
  
}

void getDataFromWebsite(){
    Serial.println("\nGetting data from website \"" + dataServer + "\"");
    sendATcommand("AT+CIPSTART=\"TCP\",\"" + dataServer + "\",80", delay_2x);
    Serial.println("Website connected!");
  
    String getRequest = "GET " + dataURI + "\r\nHost: " + dataServer + "\r\n";
  
    esp8266.print("AT+CIPSEND=");
    getESPData();
    esp8266.println(String(getRequest.length()));
    getESPData();
    esp8266.println(getRequest);
    getESPData();
    delay(delay_1x);
    String rs = getESPData();
    if(rs.indexOf("sw")==-1){
      Serial.println("Get data failed!\n");
      return;
    }
    String sw1Data = ((rs.indexOf("checked")!=-1)?"sw1_on":"sw1_off");
    sw1_on = (sw1Data.indexOf("on")!=-1)?true:false;
    //Serial.println(rs);
    esp8266.println("AT+CIPCLOSE");
  
    Serial.println("Data from website: " + sw1Data);
}

void setup()
{
  Serial.begin(9600);
  esp8266.begin(9600);

  pinMode(touch_1, INPUT);
  pinMode(13, OUTPUT);
  
  connectWifi(ssid, password);
}

void loop()
{
  getDataFromWebsite();
  sendDataToWebsite();
  delay(100);
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
  return "";
}

void sendATcommand(String msg, int dt){
  esp8266.println(msg);
  delay(dt);
  getESPData();
}
