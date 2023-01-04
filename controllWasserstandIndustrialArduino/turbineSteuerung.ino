// Include Industrial Shields libraries
#include <RS485.h>

float hysterese;
float errorToDelayFactor;
static float targetValue;

int open = R4;
int close = R6;

int waterlevel = 23;

// Zylinder Setup
void closeZylinder(int time);
void openZylinder(int time);
int moveZylinder(float *targetValue);
// read water Level
float readPercent();

// RS485 Communication
String readMessage();
void writeMessage(String message);
void handleRequest(float *targetValue);

void setup() {
  // Begin serial port
	Serial.begin(38400);
  Serial.println("setup");
  // Begin RS485 port
  RS485.begin(38400);

  // close and open are already configured
  pinMode(waterlevel, INPUT);

  targetValue = 69.0;
  hysterese = 1.5;
  errorToDelayFactor = 25;
}

void loop()
{
  handleRequest(&targetValue);
}



String readMessage()
{
  String message = "";
  int i=0;

  if(RS485.available())
  {
    byte rx = RS485.read();
    while(rx != 59)
    {
      rx = RS485.read();
      if(rx != 255)
      {
        message+=(char)rx;
      }
    }
  }
  if(message != 0)
  {
    Serial.println(message);
    return message;
  }
  else
  {
    return "";
  }
}

void writeMessage(String message)
{
  for(int i=0; i<(message.length()); i++)
  {
    RS485.write((int)message[i]);
  }
    RS485.write((int)"\n");
}

void handleRequest(float *targetValue)
{
  String message = readMessage();
  if(message.indexOf("wasserstand") > 0)
  {
    RS485.flush();
    int percent = readPercent();
    Serial.println(percent);
    String message = String(percent);
    message += "\n";
    writeMessage(message);
  }
  else if(message.indexOf("zylinder") > 0)
  {
    Serial.println("Received zylinder");
    Serial.println(message);
    RS485.flush();
    int timeToMove = moveZylinder(targetValue);
    Serial.println(timeToMove);
    String message = String(timeToMove);
    message += "\n";
    writeMessage(message);
  }
  else if(message.indexOf("targetValue") > 0)
  {
    Serial.println("Received sollwert");
    Serial.println(message);
    String value = message.substring(12,15);
    Serial.println(value);
    *targetValue = value.toInt();
    RS485.flush();
    message = String(*targetValue);
    message += "\n";
    writeMessage(message);
  }
  else if(message.indexOf("getsollwert") > 0)
  {
    Serial.println("Received Get Sollwert");
    Serial.println(message);
    RS485.flush();
    Serial.println(*targetValue);
    String message = String(*targetValue);
    message += "\n";
    writeMessage(message);
  }
  /*else if(message.indexOf("hysterese") > 0)
  {
    Serial.println("Received hysterese");
    Serial.println(message);
    RS485.flush();
    char tx[] = "25.1\n";
    Serial.println(tx);
    for(int i=0; i<(sizeof(tx)/sizeof(char)); i++)
    {
      Serial.print((int)tx[i]);
      writeMessage((int)tx[i]);
    }
    Serial.print("\n");
    writeMessage((int)"\n");
  }*/
}

int moveZylinder(float *targetValue)
{
  int timeToMove = (readPercent() - *targetValue) * errorToDelayFactor;

  if(readPercent() < (*targetValue - hysterese))
  {
    closeZylinder(abs(timeToMove));
    Serial.println("Close Zylinder");
  }
  else if (readPercent() > (*targetValue + hysterese))
  {
    openZylinder(abs(timeToMove));
    Serial.println("Open Zylinder");
  }
  else
  {
    Serial.println("No movement");
    timeToMove = 0;
  }
  return timeToMove;
}

float readPercent()
{
  float val = analogRead(waterlevel);
  float voltage = ((val/1023.0)*10.0);
  float percent = voltage * 10.0;
  return percent;
}

void openZylinder(int time)
{
  digitalWrite(open, HIGH);
  delay(time);
  digitalWrite(open, LOW);
}

void closeZylinder(int time)
{
  digitalWrite(close, HIGH);
  delay(time);
  digitalWrite(close, LOW);
}