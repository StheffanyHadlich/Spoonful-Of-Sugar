#include <ESP8266WiFi.h>

#define ssid "NDakota"
#define password "stheffanysh"
WiFiServer server(80);


void WifiConnection(void)
{
  Serial.println();
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);
 
  WiFi.begin(ssid, password);
 
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected");
}

void startServer(void)
{
  // Start the server
  server.begin();
  Serial.println("Server started");
 
  // Print the IP address
  Serial.print("Use this URL to connect: ");
  Serial.print("http://");
  Serial.print(WiFi.localIP());
  Serial.println("/"); 
}

void setup() {
  Serial.begin(9600);
  WifiConnection();
  startServer();
}

void receive(){
  WiFiClient client = server.available();

  if (!client) {
    return;
  }

  while (!client.available()) {
    delay(1);
  }

  String request = client.readString(); 
  Serial.println(request);
}

void loop() {
  
  receive();
}


