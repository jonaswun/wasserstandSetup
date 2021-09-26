int pin1 = A1;

float val1 = 0.0;
float voltage = 0.0;
float percent = 0.0;


void setup(){
  Serial.begin(9600);
}

void loop(){
  val1 = analogRead(pin1);
  voltage = ((val1/1023.0)*10.0);
  percent = voltage * 10.0;
  char stringValue[10];
  dtostrf(percent, 4, 2, stringValue);
  Serial.println(percent);
  delay(1000);
}
