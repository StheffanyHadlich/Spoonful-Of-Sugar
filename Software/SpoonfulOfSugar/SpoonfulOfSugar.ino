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

// -------------------------------------------- OLED RELATED STUFF ----------------------

#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define OLED_RESET LED_BUILTIN
Adafruit_SSD1306 display(OLED_RESET);

#define NUMFLAKES 10
#define XPOS 0
#define YPOS 1
#define DELTAY 2

#define LOGO16_GLCD_HEIGHT 16 
#define LOGO16_GLCD_WIDTH  16 
static const unsigned char PROGMEM logo16_glcd_bmp[] =
{ B00000000, B11000000,
  B00000001, B11000000,
  B00000001, B11000000,
  B00000011, B11100000,
  B11110011, B11100000,
  B11111110, B11111000,
  B01111110, B11111111,
  B00110011, B10011111,
  B00011111, B11111100,
  B00001101, B01110000,
  B00011011, B10100000,
  B00111111, B11100000,
  B00111111, B11110000,
  B01111100, B11110000,
  B01110000, B01110000,
  B00000000, B00110000 };

#if (SSD1306_LCDHEIGHT != 64)
#error("Height incorrect, please fix Adafruit_SSD1306.h!");
#endif

//------------------------------------------- ADRESSABLE LED STUFF -----------------------------------------------------------

#include <Adafruit_NeoPixel.h>
#define PIN            2
#define NUMPIXELS      8 //number of lights
#define BRIGHTNESS 10
Adafruit_NeoPixel pixels = Adafruit_NeoPixel(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);



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


void ScreenTime(){
  display.setTextSize(2);
  display.setTextColor(WHITE);
  display.setCursor(20,30);
  display.print(hour());
  display.print(":");
  display.print(minute());
  display.print(":");
  display.print(second());
  display.display();
  delay(500);
  display.clearDisplay();  
}

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
   dateTime = NTPch.getNTPtime(21.0, 1); // NTP pool time zone, DST
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
int countMedicine = 0;
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
   void AddAlarms(LinkedList<AlarmSchedule> schedule);
   void GetAllInfo();
   void InfoDisplay();
   void InfoSerial();
   int GetGap();
   int GetAlarmsHour(int j);
   int GetAlarmsMinute(int j);
   int GetSpot();
};

Medicine *medicine[BoxSpots]; //List of medicines

void MedicineInitialize(){ //Remember to call this at the setUp
  int i;
  for(i=0;i<BoxSpots;i++)
  {
    medicine[i] = new Medicine();
  }
}

//------------------------------------------------ INSERT MEDICINE 

 int Medicine::GetGap(){
  return _gap;
}

 int Medicine::GetSpot(){
  return _spot;
}

int Medicine::GetAlarmsHour(int j){
  return (alarms->get(j)).alarmHour;
}


int Medicine::GetAlarmsMinute(int j){
  return (alarms->get(j)).alarmMinute;
}

void Medicine::InfoDisplay(){
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(0,20);
  display.println("TOMAR REMEDIO");
  display.println(_nameMedicine);
  display.println("QUANTIDADE: ");
  display.print(_amount);
  display.display();
  BlinkSpot(_spot);
  delay(5000);
  display.clearDisplay();  
}

void Medicine::InfoSerial(){
  Serial.println("Informacoes sobre Remedio");
  Serial.println(_spot);
  Serial.println(_nameMedicine);
  Serial.println(_amount);
  Serial.println(_gap);
  Serial.println("Hora dos alarmes: ");
  int i;
  for(i=0;i<_gap;i++){
    Serial.print((alarms->get(i)).alarmHour);
    printDigits((alarms->get(i)).alarmMinute);
    Serial.println(); 
  }  
}

void Medicine::GetAllInfo(){
   InfoDisplay();
   InfoSerial(); 
}


bool Medicine::InsertMedicine(String nameMedicine,String amount,int gap, LinkedList<AlarmSchedule> schedule){
    int place  = FindPlace();
    if (place != ERRORFULLBOX){
      _nameMedicine = nameMedicine;
      _amount = amount;
      _gap=gap;
      _spot = place; //needs functions to light up the led spot  
      int i;
      
      for(i=0;i<3;i++){   
         alarms->add(schedule.get(i));   
      }   
      countMedicine++;      
      return true; //Do I really need this?
    }
    else{
      Serial.println("ERROR: Full box");  
      return false;
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
      Box[i]=true;
      return i;
    } 
}

//-------------------------------------------------------------- DELETE MEDICINE
//-------------------------------------------------------------- EDIT MEDICINE


//------------------------------------------------------------------- ALARM RELATED FUNCTIONS ----------------------------------------------------------------


void IsThereAlarm(){ 
  int i, j;

  for(i=0;i<countMedicine;i++){
    for(j=0;j<medicine[i]->GetGap();j++){

      if(medicine[i]->GetAlarmsHour(j) == hour()){
        if(medicine[i]->GetAlarmsMinute(j) == minute()){
          Serial.println("*************"); 
          BlinkAlarm();         
          Serial.println(medicine[i]->GetSpot());
          digitalClockDisplay();          
          medicine[i]->GetAllInfo();
        }
        else{
          LedsOff();
        }
      }             
    } 
  }
}


// ----------------------------------- LEDS and BEEP FUNCTIONS

void LedsOff(){
  pixels.setPixelColor(0, pixels.Color(0,0,0));
          pixels.setPixelColor(1, pixels.Color(0,0,0));
          pixels.setPixelColor(2, pixels.Color(0,0,0));
          pixels.setPixelColor(3, pixels.Color(0,0,0));
          pixels.setPixelColor(4, pixels.Color(0,0,0));
          pixels.setPixelColor(5, pixels.Color(0,0,0));
          pixels.setPixelColor(6, pixels.Color(0,0,0));
          pixels.setPixelColor(7, pixels.Color(0,0,0));
          pixels.show();  
}

void BlinkSpot(int spot){
  
  pixels.setPixelColor(spot, pixels.Color(255,0,255));
  pixels.show();
}

void BlinkAlarm(){
  digitalWrite(D6, HIGH);
  delay(500);
  digitalWrite(D6, LOW);
}


// -------------------------------------------- CLIENT RELATED FUNCTIONS ??????-----------------------------------------------------


// ----------------------------------------- I GUESS I CAN CALL IT MAIN RELATED FUNCTIONS -----------------------------------------


void Test(){


      int i;

       AlarmSchedule *comprimido[2];
       LinkedList<AlarmSchedule> *schedule = new LinkedList<AlarmSchedule>();

        comprimido[0]=new AlarmSchedule(); 
        comprimido[1]=new AlarmSchedule(); 
        // REMEDIO A

        comprimido[0]->alarmHour =19;
        comprimido[0]->alarmMinute = 44;
        comprimido[1]->alarmHour =19;
        comprimido[1]->alarmMinute = 50;
 

      for(i=0;i<2;i++){               
         schedule->add(*comprimido[i]);
      }    

      AlarmSchedule *comprimido2[2];
       LinkedList<AlarmSchedule> *schedule2 = new LinkedList<AlarmSchedule>();

        comprimido2[0]=new AlarmSchedule(); 
        comprimido2[1]=new AlarmSchedule(); 
        //REMEDIO B
 
        comprimido2[0]->alarmHour =19;
        comprimido2[0]->alarmMinute = 46;
        comprimido2[1]->alarmHour =19;
        comprimido2[1]->alarmMinute = 51;

      
      for(i=0;i<2;i++){               
         schedule2->add(*comprimido2[i]);
      }    

      AlarmSchedule *comprimido3[2];
       LinkedList<AlarmSchedule> *schedule3 = new LinkedList<AlarmSchedule>();

        comprimido3[0]=new AlarmSchedule(); 
        comprimido3[1]=new AlarmSchedule();
        //REMEDIO C
        
        comprimido3[0]->alarmHour =19;
        comprimido3[0]->alarmMinute = 48;
        comprimido3[1]->alarmHour = 19;
        comprimido3[1]->alarmMinute = 53; 
 

      for(i=0;i<2;i++){               
         schedule3->add(*comprimido3[i]);
      }    
     
      
  
      medicine[0]->InsertMedicine("Remedio A","1 comprimido",schedule->size(),*schedule);
      medicine[1]->InsertMedicine("Remedio B","2 comprimido",schedule2->size(),*schedule2);
      medicine[2]->InsertMedicine("Remedio C","3 comprimido",schedule3->size(),*schedule3);
     
  
}

void setup() {
  Serial.begin(9600);
  pinMode(D0, OUTPUT);
  pinMode(D1, OUTPUT);
  pinMode(D2, OUTPUT);
  pinMode(D3, OUTPUT);
  pinMode(D4, OUTPUT);
  pinMode(D5, OUTPUT);
  pinMode(D6, OUTPUT);
  WifiConnection();
  startServer();
  MedicineInitialize();

  pixels.setBrightness(BRIGHTNESS);
  pixels.begin();
  LedsOff();
  pixels.show();

  display.begin(SSD1306_SWITCHCAPVCC, 0x3C); 
  display.display();
  delay(500);
  display.clearDisplay();

  
}


int count = 0;

void loop() {
  setCurrentTime();
  //digitalClockDisplay();

   if(count==0){
    Test();
    count++;
   }
 
  IsThereAlarm();
  digitalClockDisplay();  
  ScreenTime();
  delay(500);
}
