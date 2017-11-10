#include <ESP8266WiFi.h>
#include <ESP8266SSDP.h>
 
const char* ssid = "Mano";//type your ssid
const char* password = "kodaskodas";//type your password

int pinUpButton = 12; // GPIO12 - D6
int pinStopButton = 13; // GPIO13 - D7
int pinDownButton = 15; // GPIO15 - D8

WiFiServer server(80);

unsigned long previousMillis = 0;
const long interval = 1000;
 
void setup() {
  Serial.begin(115200);
  delay(10);
 
 
  pinMode(pinUpButton, OUTPUT);
  digitalWrite(pinUpButton, HIGH);
  pinMode(pinStopButton, OUTPUT);
  digitalWrite(pinStopButton, HIGH);
  pinMode(pinDownButton, OUTPUT);
  digitalWrite(pinDownButton, HIGH);
   
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

  Serial.printf("Starting SSDP...\n");
  SSDP.setSchemaURL("description.xml");
  SSDP.setHTTPPort(80);
  SSDP.setName("ESP Blinds Controller");
  SSDP.setSerialNumber(ESP.getChipId());
  SSDP.setURL("index.html");
  SSDP.setModelName("ESP8266 Blinds Controller");
  SSDP.setModelNumber("v1");
  SSDP.setModelURL("http://www.google.com");
  SSDP.setManufacturer("Home Made");
  SSDP.setManufacturerURL("http://www.google.com");
  SSDP.setDeviceType("urn:schemas-upnp-org:device:RollerBlinds:1");
  SSDP.begin();
    
}
 
void loop() {
  boolean buttonUp = false;
  boolean buttonStop = false;
  boolean buttonDown = false;
  
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
  if (request.indexOf("/BUTTON=UP") != -1) {
    buttonUp=true;
    Serial.println("pinUpButton");
    digitalWrite(pinUpButton, LOW);
    delay(200);
    digitalWrite(pinUpButton, HIGH);
  } 
  if (request.indexOf("/BUTTON=STOP") != -1) {
    buttonStop=true;
    Serial.println("pinStopButton");
    digitalWrite(pinUpButton, LOW);
    digitalWrite(pinDownButton, LOW);
    delay(200);
    digitalWrite(pinUpButton, HIGH);
    digitalWrite(pinDownButton, HIGH);
  }
  if (request.indexOf("/BUTTON=DOWN") != -1) {
    buttonDown=true;
    Serial.println("pinDownButton");
    digitalWrite(pinDownButton, LOW);
    delay(200);
    digitalWrite(pinDownButton, HIGH);
  }

 
// Set ledPin according to the request
//digitalWrite(ledPin, value);
   
 
  // Return the response
  client.println("HTTP/1.1 200 OK");
  client.println("Content-Type: text/html");
  client.println(""); //  do not forget this one
  client.println("<!DOCTYPE HTML>");
  client.println("<html>");
   
  client.print("Button pressed is: ");
   
  if(buttonUp) {
    client.print("UP");}
  if(buttonStop) {
    client.print("STOP");}
  if(buttonDown) {
    client.print("DOWN");}

  client.println("<br><br>");
  client.println("Click <a href=\"/BUTTON=UP\">here</a> Roller Blinds UP<br>");
  client.println("Click <a href=\"/BUTTON=STOP\">here</a> Roller Blinds STOP<br>");
  client.println("Click <a href=\"/BUTTON=DOWN\">here</a> Roller Blinds DOWN<br>");
  client.println("</html>");
 
  delay(1);
  Serial.println("Client disonnected");
  Serial.println("");
 
}
