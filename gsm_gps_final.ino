#include <TinyGPS++.h>
#include <SoftwareSerial.h>

//gps vars
static const uint32_t GPSBaud = 9600;
int buttonPin=7;
int ledPin=13;
//gsm vars
int _timeout;
String _buffer;
String number = "+21626603061"; //-> change with your number
String newLocation="";

// The TinyGPS++ object
TinyGPSPlus gps;

// SOFTWARE SERIAL DECLARATIONS
SoftwareSerial gps_Serial(4, 3);
SoftwareSerial sim(10, 11);

void setup()
{
  //GPS SETUP
  Serial.begin(115200);
  gps_Serial.begin(GPSBaud);
  pinMode(buttonPin,INPUT);
  pinMode(ledPin,OUTPUT);
  digitalWrite(ledPin,LOW);
  Serial.println(F("DeviceExample.ino"));
  Serial.println(F("A simple demonstration of TinyGPS++ with an attached GPS module"));
  Serial.print(F("Testing TinyGPS++ library v. ")); Serial.println(TinyGPSPlus::libraryVersion());
  Serial.println(F("by Mikal Hart"));
  Serial.println();
  digitalWrite(buttonPin,HIGH);
  //GSM SETUP
    //delay(7000); //delay for 7 seconds to make sure the modules get the signal
  _buffer.reserve(50);
  Serial.println("System Started...");
  sim.begin(9600);
  delay(1000);
  
}


String double2string(double n, int ndec) {//utility function: convert double 2 string
    String r = "";

    int v = n;
    r += v;     // whole number part
    r += '.';   // decimal point
    int i;
    for (i=0;i<ndec;i++) {
        // iterate through each decimal digit for 0..ndec 
        n -= v;
        n *= 10; 
        v = n;
        r += v;
    }

    return r;
}
String gpsInfo()
{
  double longitude,latitude;
  Serial.print(F("Location: ")); 
  if (gps.location.isValid())
  {
    double latitude =gps.location.lat();
    double longitude =gps.location.lng();
    Serial.println("finished reading gps info");//debugging thingy
  }
  else
  {
    Serial.print(F("INVALID"));
  }
String location="https://www.google.com/maps/search/?api=1&query="+double2string(latitude,3)+","+double2string(longitude,3);
return location;

}
//-------------------------------------------------------------------------gsm


void SendMessage(String location)
{
  //Serial.println ("Sending Message");
  sim.println("AT+CMGF=1");    //Sets the GSM Module in Text Mode
  delay(200);
  //Serial.println ("Set SMS Number");
  sim.println("AT+CMGS=\"" + number + "\"\r"); //Mobile phone number to send message
  delay(200);
  String SMS = "HELP ME PLEASE IM AT THIS LOCATION: "+location;
  sim.println(SMS);
  delay(100);
  sim.println((char)26);// ASCII code of CTRL+Z
  delay(200);
  //_buffer = _readSerial();
}

/*String _readSerial() {
  _timeout = 0;
  while  (!sim.available() && _timeout < 12000  )
  {
    delay(13);
    _timeout++;
  }
  if (sim.available()) {
    return sim.readString();
  }
}*/
boolean buttonIsClicked(int buttonPin){
    if(digitalRead(buttonPin)==LOW){
      Serial.println("click");
      digitalWrite(ledPin,HIGH);
    return true;  }
    else {
    digitalWrite(buttonPin,HIGH);
    digitalWrite(ledPin,LOW);
    return false;
  }
}
void loop()
{
  // This sketch displays information every time a new sentence is correctly encoded.
 // while (gps_Serial.available() > 0){
    //if (gps.encode(gps_Serial.read()))
      //newLocation=gpsInfo();
      //Serial.println(newLocation);
    if(buttonIsClicked(buttonPin)){
      
      SendMessage(newLocation);  }

   /* if (millis() > 5000 && gps.charsProcessed() < 10)
    {
      Serial.println(F("No GPS detected: check wiring."));
      while(true);
    }*/
  //}
}
