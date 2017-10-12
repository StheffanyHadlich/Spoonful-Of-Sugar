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
#define ssid "Hadllich"
#define password "36221595"
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

class AlarmSchedule{
  public:
    int alarmHour;
    int alarmMinute;  
};

class Medicine{
  private:
    int _spot;
    String _nameMedicine = "Empty";
    String _amount = "0";
    int _gap = 0;
    LinkedList<AlarmSchedule> *alarms = new LinkedList<AlarmSchedule>(); 
    
  public:
   bool InsertMedicine(String nameMedicine,String amount,int gap,  LinkedList<AlarmSchedule> schedule);
   int FindPlace();
   int DefinePort(int spot);
   void AddAlarms(LinkedList<AlarmSchedule> schedule);
   void GetAllInfo();
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

void Medicine::GetAllInfo(){
  Serial.println("Informacoes sobre Remedio");
  Serial.println(_spot);
  Serial.println(_nameMedicine);
  Serial.println(_amount);
  Serial.println(_gap);
  Serial.println("Hora dos alarmes: ");
  int i;
  for(i=0;i<_gap;i++){
    Serial.println((alarms->get(i)).alarmHour);
    Serial.println((alarms->get(i)).alarmMinute);
  }
}


bool Medicine::InsertMedicine(String nameMedicine,String amount,int gap, LinkedList<AlarmSchedule> schedule){
    int place  = FindPlace();
    if (place != ERRORFULLBOX){
      _nameMedicine = nameMedicine;
      _amount = amount;
      _gap=gap;
      _spot = DefinePort(place);  
      int i;
      
      for(i=0;i<3;i++){   
         alarms->add(schedule.get(i));   
      }   
            
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

void Blink()
{
  digitalWrite(D6, HIGH);
  delay(250);
  Serial.println("Blink");
  digitalWrite(D6, LOW);
}


// -------------------------------------------- CLIENTE RELATED FUNCTIONS -----------------------------------------------------


// ----------------------------------------- I GUESS I CAN CALL IT MAIN RELATED FUNCTIONS -----------------------------------------


void Test(){


      int i;

       AlarmSchedule *comprimido[3];
       LinkedList<AlarmSchedule> *schedule = new LinkedList<AlarmSchedule>();

        comprimido[0]=new AlarmSchedule(); 
        comprimido[1]=new AlarmSchedule(); 
        comprimido[2]=new AlarmSchedule(); 

      for(i=0;i<3;i++){    
           
         comprimido[i]->alarmHour =i+2;
         comprimido[i]->alarmMinute = i+3;
         schedule->add(*comprimido[i]);
      }    
     
      
  
      medicine[0]->InsertMedicine("Remedio A","1 comprimido",schedule->size(),*schedule);
      medicine[0]->GetAllInfo();
     
  
}

void setup() {
  Serial.begin(9600);
  pinMode(D6, OUTPUT);
  WifiConnection();
  startServer();
  MedicineInitialize();
}


int count = 0;

void loop() {
  setCurrentTime();
  //digitalClockDisplay();

   if(count==0){
    Test();
    count++;
   }
   
  delay(1000);
}
