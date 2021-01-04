#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>

const int swCount = 4;
int sw[swCount] = {D6, D7, D8, D9};
int touch[swCount] = {D5, D2, D1, D0};
bool swData[swCount];
String ssid = "MON - NIO - PUPPY 2.4GHz";
String password = "44444444";
//String ssid = "Mon Mina";
//String password = "tamsotam";
String mainURL = "http://switchcontrol.xyz";
String dataURL = "http://switchcontrol.xyz/data.json";

int delay_1x = 1000;
int delay_2x = 2*delay_1x;
int delay_3x = 3*delay_1x;
int delay_4x = 4*delay_1x;
int delay_5x = 5*delay_1x;
int delay_10x = 10*delay_1x;

void setup() {
  Serial.begin(115200);
  
  for(int i=0; i<swCount; i++){
    pinMode(sw[i], OUTPUT);
    digitalWrite(sw[i], LOW);
    swData[i] = false;
    pinMode(touch[i], INPUT);
  }

  connectWifi(ssid, password);
}

void connectWifi(String ssid, String pass){
  WiFi.begin(ssid, pass); 
  while (WiFi.status() != WL_CONNECTED) 
  {
    delay(500);
    Serial.println("Connecting to wifi!");
    getTouch();
  }
  Serial.println();
  Serial.println("WiFi connected");
}

void getDataFromWebsite(){
  if (WiFi.status() == WL_CONNECTED) {
    HTTPClient http;

    http.begin(dataURL);
    int httpCode = http.GET();

    if (httpCode == 200) {
      Serial.println("Data from website:");
      String payload = http.getString();
      for(int i=0; i<swCount; i++){
        swData[i] = (payload.indexOf("sw"+String(i+1)+"on")!=-1)?true:false;
        digitalWrite(sw[i], swData[i]);
        Serial.print("sw"+String(i+1)+": "+String(swData[i]?"on":"off"));
        Serial.print((i!=swCount-1)?", ":"\n");
      }
    }else{
      Serial.println("Cannot get data from website");
    }
    http.end();
  }else{
    connectWifi(ssid, password);
  }
  delay(delay_1x);
}

bool getTouch(){
  bool touched = false;
  Serial.print("Touched: ");
  for(int i=0; i<swCount; i++){
    if(digitalRead(touch[i])){
      swData[i] = !swData[i];
      digitalWrite(sw[i], swData[i]);
      Serial.print(String(i+1) + " ");
      touched = true;
    }
  }
  Serial.print((touched)?"\n":" None\n");
  return touched;
}

void sendDataToWebsite(){
  if(getTouch()){
    if (WiFi.status() == WL_CONNECTED) {
      HTTPClient http;
  
      http.begin(mainURL);
      http.addHeader("Authorization", "Basic YWRtaW46YWRtaW4xMjM=");
      http.addHeader("Content-Type", "application/json");
  
      String data = "";
      for(int i=0; i<swCount; i++){
        data += "{\"sw"+String(i+1)+"\":\"sw"+String(i+1)+String((swData[i])?"on":"off")+"\"}";
        if(i!=swCount-1){
          data+=",";
        }
      }
      data = "[" + data + "]";
      Serial.println("Data POST Request:\n" + data);
      int httpCode = http.POST(data);
  
      Serial.println((httpCode == 200)?"Send data SUCCESS!":"Send data FAILED!");
      
      http.end();
    }else{
      connectWifi(ssid, password);
    }
    delay(delay_1x);
  }
}

void loop() {
  getDataFromWebsite();
  sendDataToWebsite();
}
