#include <ESP8266WiFi.h>
#include <Stepper.h>
 
const char* ssid = "NDakota";
const char* password = "stheffany";
const int stepsPerRevolution = 200;
Stepper myStepper(stepsPerRevolution, D1, D2, D5, D6);

int ledPin = D4; // GPIO13
WiFiServer server(80);
 
void setup() {
    myStepper.setSpeed(60);
  Serial.begin(9600);
  delay(10);
 
  pinMode(ledPin, OUTPUT);
  digitalWrite(ledPin, LOW);
 
  // Connect to WiFi network
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
 
  // Start the server
  server.begin();
  Serial.println("Server started");
 
  // Print the IP address
  Serial.print("Use this URL to connect: ");
  Serial.print("http://");
  Serial.print(WiFi.localIP());
  Serial.println("/");
 
}

void InternetStuff(){
  // Check if a client has connected
  WiFiClient client = server.available();
  if (!client) {
    return;
  }
 
  // Wait until the client sends some data
  Serial.println("new client");
  while(!client.available()){
    delay(1);
  }
 
  // Read the first line of the request
  String request = client.readStringUntil('\r');
  Serial.println(request);
  client.flush();
 
  // Match the request
 
  int value = LOW;
  if (request.indexOf("/R") != -1)  {
    digitalWrite(ledPin, HIGH);
    myStepper.step(stepsPerRevolution);
    delay(200);
    digitalWrite(ledPin, LOW);

  }
  if (request.indexOf("/L") != -1)  {
  digitalWrite(ledPin, HIGH);

    myStepper.step(-stepsPerRevolution);
    delay(200);
    digitalWrite(ledPin, LOW);

   }
 
  // Return the response
  client.println("HTTP/1.1 200 OK");
  client.println("Content-Type: text/html");
  client.println(""); //  do not forget this one
  client.println("<!DOCTYPE HTML>");
 client.println("<html>");
client.println("<head>");
  client.println("<title>Spoonfull of Sugar</title>");
  client.println("<meta charset='utf-8'>");
client.println("<h1>Spoonfull of Sugar</h1>");
client.println("</head>");
client.println("<body>");
  client.println("<form>");
    client.println("<br> Remedio:");
    client.println("<input type='text'  name='medicine'/></br>");

    client.println("<br> Quantidade:");
    client.println("<input type='text'  name='medicine'/></br>");

    client.println("<br> Adicionar Alarmes:");
    client.println("<br> Hora:");
    client.println("<input type='number' name='hour'/></br>");
    client.println("<br> Minuto:");
    client.println("<input type='number' name='hour'/></br>");


    client.println("<input type='submit'/></br>");
    client.println("<form>");
  client.println("</body>");
client.println("</html>");
 
  delay(1);
}
 
void loop() {

  InternetStuff();
 
}

