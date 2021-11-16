#include <time.h>
#include <stdio.h>
#include <string.h>

// Regelungsvariabeln
static float sollwert;
float hysterese;

int timeBetweenMovement;
float errorToDelayFaktor;
bool verbose = false;

// Pin Definition 
int zylinderAuf = 2;
int zylinderZu = 3;
int wasserstandInput = A1;

// Time Data
struct tm date;
unsigned long unixtime;
unsigned long int unixtimeStart = 1636657764;
unsigned long timeSinceBootup;
int lastMovement;


// Seriell Kommunikation

const int EnTxPin =  4;
const int En2 = 5;


void bewegeZylinderAuf(int zeit)
{
  digitalWrite(zylinderAuf, 1);
  delay(zeit);
  digitalWrite(zylinderAuf, 0);
}

void bewegeZylinderZu(int zeit)
{
  digitalWrite(zylinderZu, 1);
  delay(zeit);
  digitalWrite(zylinderZu, 0);
}


void setup() {
  pinMode(zylinderAuf, OUTPUT);
  pinMode(zylinderZu, OUTPUT);
  pinMode(wasserstandInput, INPUT);
  Serial.begin(38400);
  Serial.setTimeout(100);  
  pinMode(EnTxPin, OUTPUT);
  pinMode(En2, OUTPUT);

  sollwert = 69.0;
  hysterese = 0.5;
  timeBetweenMovement = 30;
  errorToDelayFaktor = 45;
}

float readPercent(){
  float val1 = analogRead(wasserstandInput);
  float voltage = ((val1/1023.0)*10.0);
  float percent = voltage * 10.0;
  char stringValue[10];
  dtostrf(percent, 4, 2, stringValue);
  return percent;
}

void writeData(float data)
{
  digitalWrite(EnTxPin, HIGH);
  digitalWrite(En2, HIGH);
  Serial.println(data);
  Serial.flush();
  digitalWrite(EnTxPin, LOW);
  digitalWrite(En2, LOW);
}

char readData()
{
  digitalWrite(EnTxPin, LOW); //RS485 as receiver
  digitalWrite(En2, LOW);
  char data;
  if(Serial.available())
  {
    data=Serial.read(); 
  }
  
  return data;
}

int readDataNew(int anzahlZeichen)
{
  digitalWrite(EnTxPin, LOW); //RS485 as receiver
  digitalWrite(En2, LOW);
  char buffer[3]={};
  int zahl; 
  Serial.println("ReadDataNew");
  int i=0;
  for(i; i<anzahlZeichen; i++)
  {
    buffer[i]=readData();  
  }
  
  buffer[i]="\0";
    
  int data = atoi(buffer);
  return data;
}

void calculateMovement()
{
  int timeToMove = (readPercent()-sollwert) * errorToDelayFaktor;

  if(readPercent() < (sollwert - hysterese))
  {
    bewegeZylinderZu(abs(timeToMove));
    if(verbose)
    {
      Serial.println("Zylinder zu");
      Serial.print("Sollwert:\t");
      Serial.println(sollwert);
      Serial.print("Tatsächlich:\t");
      Serial.println(readPercent());
    }
  }
  else
  {
    if(readPercent() > (sollwert + hysterese))
    {
      bewegeZylinderAuf(abs(timeToMove));
      if(verbose)
      {
        Serial.println("Zylinder auf");
        Serial.print("Sollwert:\t");
        Serial.println(sollwert);
        Serial.print("Tatsächlich:\t");
        Serial.println(readPercent());
      }
    }
    else
    {
      if(verbose)
      {
        Serial.println("Im Rahmen");
        Serial.print("Sollwert:\t");
        Serial.println(sollwert);
        Serial.print("Tatsächlich:\t");
        Serial.println(readPercent());
      }
    }
  }
}



void checkSollwert(float *sollwert)
{
  if(date.tm_hour >= 18 || date.tm_hour <= 6)
  {
    *sollwert = 55.0;
  }
  else
  {
    *sollwert = 65.0;
  }
}

void handleRequest(float *sollwert)
{
  char buffer[3];
  int request = readData();
  //Serial.println(request);
  float data;
  switch(request)
  {
    case 49:
      writeData(readPercent());
    break;
    case 50:
      //Set Sollwert
      delay(100);
      data = readDataNew(2);
      
      if(data>0&&data<100)
      {
       *sollwert = data; 
       Serial.println("Sollwert");
       Serial.println(*sollwert);
      }
    break;
    case 51:
    // Set Hysterese 
    break;
    case 52:
    // Set Zeitfaktor
    break;
    case 53:
      // Get sollwert
      writeData(*sollwert);
    break;
    case 54:
      //Serial.println("Zylinder Zu");
    break;
  }
}

void handleRequestNew()
{
  int request = readDataNew(1);
  float data;
  switch(request)
  {
    case 49:
      //writeData(readPercent());
    break;
    case 50:
      //Set Sollwert
      //Serial.println("Fall 50");
      delay(100);
      data = readDataNew(2);
      
      if(data!=0)
      {
       sollwert = data; 
      }
    break;
    case 51:
    // Set Hysterese 
    break;
    case 52:
    // Set Zeitfaktor
    break;
    case 53:
      // Get sollwert
      writeData(sollwert);
    break;
    case 54:
      //Serial.println("Zylinder Zu");
    break;
  }
}

void loop() {

  //checkSollwert(&sollwert);
  //writeData(readPercent());
  
  //Serial.println(data);
  handleRequest(&sollwert);
  if((timeSinceBootup - lastMovement) > timeBetweenMovement)
  {
    calculateMovement();
    lastMovement = timeSinceBootup;
  }
  delay(1000);
}
