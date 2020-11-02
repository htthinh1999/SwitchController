int SW_1 = 10;
int SW_2 = 11;
int SW_3 = 12;
int SW_4 = 13;

String ssid = "MON - NIO - PUPPY 2.4GHz";
String password = "44444444";
String server = "switchcontrol.000webhostapp.com";
String mainURI = "/index.php";
String dataURI = "/data.json";

int modeESP = 1; // 1 = Station 2 = Access Point  3 = Both
String wifiToConnect = "\""+ssid+"\",\""+password+"\"";

int delay_speed = 1000;
int delay_1x = delay_speed;
int delay_2x = 2*delay_speed;
int delay_3x = 3*delay_speed;
int delay_4x = 4*delay_speed;
int delay_5x = 5*delay_speed;
int delay_10x = 10*delay_speed;


void setup()
{
  delay(delay_5x);
  Serial.begin(115200);
  
  pinMode(SW_1, OUTPUT);
  pinMode(SW_2, OUTPUT);
  pinMode(SW_3, OUTPUT);
  pinMode(SW_4, OUTPUT);
  digitalWrite(SW_1, LOW);
  digitalWrite(SW_2, LOW);
  digitalWrite(SW_3, LOW);
  digitalWrite(SW_4, LOW);
  
  connectWifi(ssid, password);
  delay(10000);
}

void loop()
{
  String data = "sw1=sw1_off&sw2=sw2_off&sw3=sw3_off&sw4=sw4_off";
  sendDataToWebsite(server, mainURI, data);
  delay(60000);
}

void connectWifi(String ssid, String password)
{
  deliverMessage("AT+RST", delay_2x); // reset ESP8266
  deliverMessage("AT+CWMODE=1", delay_1x);
  deliverMessage("AT+CWJAP="+wifiToConnect, delay_10x);
}

void sendDataToWebsite(String server, String uri, String data){
  deliverMessage("AT+CIPSTART=\"TCP\",\"" + server + "\",80", delay_1x);
  
  String postRequest =
    "POST " + uri + " HTTP/1.0\r\n" +
    "Host: " + server + "\r\n" +
    "Accept: *" + "/" + "*\r\n" +
    "Content-Length: " + data.length() + "\r\n" +
    "Content-Type: application/x-www-form-urlencoded\r\n" +
    "\r\n" + data;
  Serial.print("AT+CIPSEND=");
  Serial.println(postRequest.length());
  delay(delay_1x);
  deliverMessage(postRequest, delay_3x);
  deliverMessage("AT+CIPCLOSE", delay_1x);
}

void deliverMessage(const String& msg, int dt)
{
  Serial.println(msg);
  delay(dt);
}
