#include <SoftwareSerial.h>

const int swCount = 4;
int sw[swCount] = {9, 10, 11, 12};
int dataLed[swCount] = {A4, A1, A2, A3};
int touch[swCount] = {4, 5, 6, 7};
bool swData[swCount];
int sendDataLed = 8;

//String ssid = "niothong";
//String password = "19061906";
//String ssid = "Mon Mina";
//String password = "tamsotam";
String ssid = "MON - NIO - PUPPY 2.4GHz";
String password = "44444444";
String mainServer = "switchcontrol.xyz";
String mainURI = "/index.php";
String dataServer = "api.thingspeak.com";
String dataURI = "/apps/thinghttp/send_request?api_key=6H2N36T47FNFS3QX";
String wifiToConnect = "\""+ssid+"\",\""+password+"\"";

int delay_speed = 1000;
int delay_1x = delay_speed;
int delay_2x = 2*delay_speed;
int delay_3x = 3*delay_speed;
int delay_4x = 4*delay_speed;
int delay_5x = 5*delay_speed;
int delay_10x = 10*delay_speed;

SoftwareSerial esp8266(2,3);

void connectWifi(String ssid, String password)
{
  sendATcommand("AT+RST", delay_2x); // Reset ESP8266
  sendATcommand("AT+CWMODE=1", delay_1x); // Set ESP8266 mode with 1: Station, 2: Access Point, 3: Both
  Serial.println("Connecting to wifi with SSID = \""+ssid+"\" and PASSWORD = \""+password+"\"");
  sendATcommand("AT+CWJAP=\""+ssid+"\",\""+password+"\"", delay_10x);
  Serial.println("Wifi connected!");
}

void sendDataToWebsite(){
  bool touched = false;
  Serial.print("Touched: ");
  for(int i=0; i<swCount; i++){
    if(digitalRead(touch[i])){
      swData[i] = !swData[i];
      touched = true;
      Serial.print(String(i+1) + " ");
    }
  }
  Serial.println();

  
  if(touched){
    Serial.println("\nSending data");
    digitalWrite(sendDataLed, HIGH);
    
    String data = "";
    for(int i=0; i<swCount; i++){
      digitalWrite(sw[i], swData[i]);
      digitalWrite(dataLed[i], swData[i]);
      data += "sw" + String(i+1) + (swData[i]?"on":"off") + "&";
    }
    data = data.substring(0, data.length()-1);
    
    Serial.println("Connecting to website \""+mainServer+"\"");
    sendATcommand("AT+CIPSTART=\"TCP\",\"" + mainServer + "\",80", delay_1x);
    Serial.println("Website connected!");
  
    Serial.println("POST request with data: " + data);
    // "YWRtaW46YWRtaW4xMjM=" is base 64 encode of username:password "admin:admin123"
    String postRequest =
      "POST " + mainURI + " HTTP/1.0\r\n" +
      "Host: " + mainServer + "\r\n" +
      "Accept: *" + "/" + "*\r\n" +
      "Authorization: Basic YWRtaW46YWRtaW4xMjM=\r\n" +
      "Content-Length: " + data.length() + "\r\n" +
      "Content-Type: application/x-www-form-urlencoded\r\n" +
      "\r\n" + data;
  
    esp8266.print("AT+CIPSEND=");
    esp8266.println(String(postRequest.length()));
    delay(delay_1x/10);
    esp8266.println(postRequest);
    delay(delay_1x/2);
    sendATcommand("AT+CIPCLOSE", delay_1x/3);
  
    Serial.println("Send data complete!");
    
    digitalWrite(sendDataLed, LOW);
  }
  
}


void getDataFromWebsite(){
    Serial.println("\nGetting data from website \"" + dataServer + "\"");
    sendATcommand("AT+CIPSTART=\"TCP\",\"" + dataServer + "\",80", delay_1x);
    Serial.println("Website connected!");
  
    String getRequest = "GET " + dataURI + "\r\nHost: " + dataServer + "\r\n";
  
    esp8266.print("AT+CIPSEND=");
    getESPData();
    esp8266.println(String(getRequest.length()));
    delay(delay_1x/5);
    getESPData();
    esp8266.println(getRequest);
    getESPData();
    delay(delay_1x);
    String rs = getESPData();
    Serial.println("rs = " + rs);
    if(rs.indexOf("sw")==-1){
      Serial.println("Get data failed!\n");
      sendATcommand("AT+CIPCLOSE", delay_1x);
      return;
    }
    
    Serial.print("Data from website: ");
    for(int i=0; i<swCount; i++){
      swData[i] = (rs.indexOf("sw"+String(i+1)+"on")!=-1)?true:false;
      digitalWrite(sw[i], swData[i]);
      digitalWrite(dataLed[i], swData[i]);
      Serial.print("sw"+String(i+1)+((swData[i])?"on":"off")+" ");
    }
    Serial.println();
    sendATcommand("AT+CIPCLOSE", delay_1x);
}

void setup()
{
  Serial.begin(9600);
  esp8266.begin(9600);

  for(int i=0; i<swCount; i++){
    pinMode(sw[i], OUTPUT);
    pinMode(dataLed[i], OUTPUT);
    pinMode(touch[i], INPUT);
  }
  pinMode(sendDataLed, OUTPUT);

  connectWifi(ssid, password);
}

void loop()
{
  getDataFromWebsite();
  sendDataToWebsite();
}

String getESPData(){
  if(esp8266.available())
  {
    String rs = "";
    while(esp8266.available())
    {
      char c = esp8266.read();
//      Serial.write(c);
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
