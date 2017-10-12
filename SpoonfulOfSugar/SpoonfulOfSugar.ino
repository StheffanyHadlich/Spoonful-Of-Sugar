#include <LinkedList.h>
#include <ESP8266WiFi.h>
#include <NTPtimeESP.h>
#include <TimeAlarms.h>
#include <Time.h>

#define D0 16
#define D1 5
#define D2 4
#define D3 0
#define D4 2
#define D5 14
#define D6 12
#define D7 13
#define D8 15
#define D9 3
#define D10 1



// ------------------------------------------ WEB RELATED FUNCTIONS ------------------------------------------------------------
#define ssid ""
#define password ""
WiFiServer server(80);
strDateTime dateTime;
NTPtime NTPch("br.pool.ntp.org");

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


// ----------------------------------------------------------------------- TIME RELATED FUNCTIONS -------------------------------------------------------------

void printDigits(int digits)
{
  Serial.print(":");
  if(digits < 10)
    Serial.print('0');
  Serial.print(digits);
}

void digitalClockDisplay()
{
  // digital clock display of the time
  Serial.print(hour());
  printDigits(minute());
  printDigits(second());
  Serial.println(); 
}


void setCurrentTime()
{
   dateTime = NTPch.getNTPtime(21.0, 0); // NTP pool time zone, DST
    if(dateTime.valid){
      byte actualHour = dateTime.hour;
      byte actualMinute = dateTime.minute;
      byte actualsecond = dateTime.second;
      int actualyear = dateTime.year;
      byte actualMonth = dateTime.month;
      byte actualday =dateTime.day;
      setTime(actualHour,actualMinute,actualsecond,actualday,actualMonth,actualyear); // setTime(hr,min,sec,day,mnth,yr);
   }  
}

// ----------------------------------------------------------------- CLASS MEDICINE ----------------------------------------------------------------

#define ERRORFULLBOX -1
const int MaxAlarms = 12;
const int BoxSpots = 6;
bool Box[BoxSpots] = {false};

class Medicine{
  private:
    int _spot;
    String _nameMedicine = "Empty";
    String _amount = "0";
    int _gap = 0;
    int _alarmHour[MaxAlarms] = {-1};
    int _alarmMinute = -1;
    
    
  public:
   bool InsertMedicine(String nameMedicine,String amount,int gap);
   int FindPlace();
   void calculateAlarms(int gap);
   int DefinePort(int spot);
   int GetHours(int j);
   int GetMinutes();
};

Medicine *medicine[BoxSpots];

void MedicineInitialize(){ //Remember to call this at the setUp
  int i;
  for(i=0;i<BoxSpots;i++)
  {
    medicine[i] = new Medicine();
  }
}

//------------------------------------------------ INSERT MEDICINE 

int Medicine::GetHours(int j)
{
  return _alarmHour[j];
}

int Medicine::GetMinutes()
{
  return _alarmMinute;
}

void Medicine::calculateAlarms(int gap){
  int x = 24/gap;
  int i=0, hourX;
  
  // First alarm is at the moment
  _alarmHour[i] = hour();
  _alarmMinute = minute(); //it actually does not change.
  
  for(i=1;i<x;i++){
    hourX = _alarmHour[i-1]+x;

    if(hourX > 23){
      hourX = hourX - 24;
    }
    _alarmHour[i]= hourX;
  }
  
}

bool Medicine::InsertMedicine(String nameMedicine,String amount,int gap){
    int place  = FindPlace();
    if (place != ERRORFULLBOX){
      _nameMedicine = nameMedicine;
      _amount = amount;
      _gap=gap;
      calculateAlarms(_gap);
      _spot = DefinePort(place);
      return true; //Do I really need this?
    }
    else{
      Serial.println("ERROR: Full box");  
      return false;
    }
}

int Medicine::DefinePort(int spot){
  switch(spot){
    case 0: return D0;
    case 1: return D1;
    case 2: return D2;
    case 3: return D3;
    case 4: return D4;
    case 5: return D5;
  }
}

int Medicine::FindPlace(){
    int i=0;
    while(Box[i]==true){
      i++;
    }

    if(i>=BoxSpots){
      return ERRORFULLBOX;
    }
    else{
      return i;
    } 
}

//-------------------------------------------------------------- DELETE MEDICINE
//-------------------------------------------------------------- EDIT MEDICINE


//------------------------------------------------------------------- ALARM RELATED FUNCTIONS ----------------------------------------------------------------


// ----------------------------------- CLASS ALARMS / HOUR 

class AlarmsPerHour{
  public:
    Medicine med;
    int minutes;
};

LinkedList<AlarmsPerHour> *bob = new LinkedList<AlarmsPerHour>(); 

void AlarmSchedule(){ // Looks for alarms at each hour

  bob->shift();
  
  int i,j;

  for(i=0;i<BoxSpots;i++)
  {
    for(j=0;j<MaxAlarms;j++){
      if((medicine[i]->GetHours(j))==hour()){ 
         AlarmsPerHour *P;
         P->med = *medicine[i]; 
         P->minutes= medicine[i]->GetMinutes();
         bob->add(*P);
      }
    }
  }

}

void Blink()
{
  digitalWrite(D6, HIGH);
  delay(250);
  Serial.println("Blink");
  digitalWrite(D6, LOW);
}

void AlarmBob(){
  int i;
  for(i=0;i<bob->size();i++){
    if(((bob->get(i)).minutes) == minute()){
      Blink();
    }
  }
}

// -------------------------------------------- CLIENTE RELATED FUNCTIONS -----------------------------------------------------

void InternetStuff(){ // Use this as example
  
//  // Check if a client has connected
//  WiFiClient client = server.available();
//  if (!client) {
//    return;
//  }
// 
//  // Wait until the client sends some data
//  Serial.println("new client");
//  while(!client.available()){
//    delay(1);
//  }
// 
//  // Read the first line of the request
//  String request = client.readStringUntil('\r');
//  Serial.println(request);
//  client.flush();
// 
//  // Match the request
// 
//  int value = LOW;
//  if (request.indexOf("/R") != -1)  {
//    digitalWrite(ledPin, HIGH);
//    myStepper.step(stepsPerRevolution);
//    delay(200);
//    digitalWrite(ledPin, LOW);
//
//  }
//  if (request.indexOf("/L") != -1)  {
//  digitalWrite(ledPin, HIGH);
//
//    myStepper.step(-stepsPerRevolution);
//    delay(200);
//    digitalWrite(ledPin, LOW);
//
//   }
// 
//  // Return the response
//  client.println("HTTP/1.1 200 OK");
//  client.println("Content-Type: text/html");
//  client.println(""); //  do not forget this one
//  client.println("<!DOCTYPE HTML>");
//  client.println("<html>");
// 
//  client.println("<br><br>");
//  client.println("<a href=\"/R\"\"><button>LEFT </button></a>");
//  client.println("<a href=\"/L\"\"><button>RIGHT </button></a><br />");  
//  client.println("</html>");
// 
//  delay(1);
}

// ----------------------------------------- I GUESS I CAN CALL IT MAIN RELATED FUNCTIONS -----------------------------------------

void setup() {
  Serial.begin(9600);
  pinMode(D6, OUTPUT);
  WifiConnection();
  startServer();
  MedicineInitialize();
}

int RightNow = -1;

void loop() {
  setCurrentTime();
  digitalClockDisplay();

  if(RightNow != hour()){
    RightNow = hour();
    AlarmSchedule();
  }

  AlarmBob(); // this must be use as the alarm
  
  delay(1000);
}
