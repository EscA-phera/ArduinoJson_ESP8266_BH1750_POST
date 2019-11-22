# include <ESP8266.h>
# include <ESP8266Client.h>
# include <ArduinoJson.h>
# include <SoftwareSerial.h>

# include <Wire.h>
# include <BH1750.h>

# define SSID "yourwifiname"
# define PWD "yourwifipassword"

# define connServer "yourwebsitename"
# define PORT "80"

String PATH = "/api";

SoftwareSerial esp = SoftwareSerial(11, 12);
ESP8266 wifi = ESP8266(esp);

BH1750 lightMeter;
int LED = 3;
int sensor = A0;

StaticJsonBuffer<200> jsonBuffer;
JsonObject& root = jsonBuffer.createObject();

void setup()
{ 
  Serial.begin(115200);
  wifi.begin();
  wifi.joinAP(SSID, PWD);
    
  lightMeter.begin();
  pinMode(LED, OUTPUT);
  
  Serial.println(getStatus(wifi.setMode(ESP8266_WIFI_STATION)));
  Serial.println(getStatus(wifi.joinAP(SSID, PWD)));
  Serial.println(getStatus(wifi.getAP(SSID)));
    
  Serial.println(wifi.read());
}

void loop()
{
  String data;
  root["name"] = "ArduinoStatus";
  root["data"] = "On";
  root.printTo(data);

  String postReq = "POST " + PATH + "HTTP/1.1\r\n"
                   + "HOST " + connServer + "\r\n"
                   + "ACCEPT: *" + "/" + "*\r\n"
                   + "Content-Length: " + data.length() + "\r\n"
                   + "Content-Type: application/json\r\n"
                   + "\r\n" + data;
  
  uint16_t lux = lightMeter.readLightLevel();
  
  if ((lux <= 200) && (wifi.available()) && (data.length() > 0))
  {
    digitalWrite(LED, HIGH);
    Serial.println(getStatus(wifi.send(postReq)));
  }

  else
  {
    digitalWrite(LED, LOW);
  }
}

String getStatus(bool status)
{
  if (status == true);
    return "OK";
    
  return "failed";
}


