//TECHATRONIC.COM
#include <Servo.h> // servo library  
Servo s1;
Servo s2;


// ultra sonic part

int trigPin1 = 12;//4;//12;
int echoPin1 = 14;//5;//14;

int trigPin2 = 4;//12;
int echoPin2 = 5;//14;

//define sound velocity in cm/uS
#define SOUND_VELOCITY 0.034
#define CM_TO_INCH 0.393701

long duration;
float distanceCmIn;
float distanceCmOut;



// http part
#include "ESP8266HTTPClient.h"
#include "ESP8266WiFi.h"
#include <WiFiClient.h>
WiFiClient wifiClient;


const char* ssid = "CeresOS";
const char* password =  "Unknown123";

// ***** htp data here
String htp_data; //="?temperature=24.37";
String serverName = "http://192.168.137.1:8000/hndl_url/";
//String nameDoor;


void setup()
{
  Serial.begin(115200);
  initWiFi();
  s1.attach(2);  // servo attach D3 pin of arduino
  s2.attach(0);  // servo attach D3 pin of arduino

  // ultrasonic part
  pinMode(trigPin1, OUTPUT); // Sets the trigPin as an Output
  pinMode(echoPin1, INPUT); // Sets the echoPin as an Input

  pinMode(trigPin2, OUTPUT); // Sets the trigPin as an Output
  pinMode(echoPin2, INPUT); // Sets the echoPin as an Input


}




void loop()
{ 
  s1.write(0);
  s2.write(0);
  delay(1000);

  // logic here

  // ------ in door
  distanceCmIn = ultraSonicDis(trigPin1, echoPin1);
  Serial.print("ultra_in");
  Serial.println(distanceCmIn);
  if(distanceCmIn<20){
    String tmpRes=sndWfHttp("incmng_opn_dr");
    if (tmpRes=="open"){
        s1.write(180);
        delay(2000);
        Serial.println("Veh may cross now");  // pass the veh soon
        delay(5000);
      }
     else{
        s1.write(0);
        delay(2000);
      }
    }
    
  delay(5000);
  
    // ------ out door
  distanceCmOut = ultraSonicDis(trigPin2, echoPin2);
  Serial.print("ultra_out");
  Serial.println(distanceCmOut);
  if(distanceCmOut<20){
    String tmpRes=sndWfHttp("outgng_opn_dr");
    if (tmpRes=="open"){
        s2.write(180);
        delay(1000);
        Serial.println("Veh may cross now");  // pass the veh soon
        delay(5000);
      }
     else{
        s2.write(0);
        delay(1000);
      }
    }


//
//
//  rotS1();
//  //   s1.write(180);
//  //   delay(1000);
//
//  Serial.println("s1 rotn done");
//  rotS2();
//  Serial.println("s2 rotn done");
//
//  distanceCmIn = ultraSonicDis(trigPin1, echoPin1);
//  Serial.print("ultra_in");
//  Serial.println(distanceCmIn);
//
//  distanceCmOut = ultraSonicDis(trigPin2, echoPin2);
//  Serial.print("ultra_out");
//  Serial.println(distanceCmOut);

}


float ultraSonicDis(int trg, int ech) {
  float tmpDis;
  float durtn;
  // Clears the trigPin
  digitalWrite(trg, LOW);
  delayMicroseconds(2);
  // Sets the trigPin on HIGH state for 10 micro seconds
  digitalWrite(trg, HIGH);
  delayMicroseconds(10);
  digitalWrite(trg, LOW);

  // Reads the echoPin, returns the sound wave travel time in microseconds
  durtn = pulseIn(ech, HIGH);

  // Calculate the distance
  tmpDis = durtn * SOUND_VELOCITY / 2;



  // Prints the distance on the Serial Monitor
  Serial.print("Distance (cm): ");
  //  Serial.println();
  return tmpDis;
}
//
//void rotS1() {
//  s1.write(0);
//  delay(2000) ;
//  s1.write(180);
//  delay(2000) ;
//}
//
//void rotS2() {
//  s2.write(0);
//  delay(1000);
//  s2.write(90);
//  delay(1000);
//  s2.write(180);
//}



void initWiFi() {
  // ********  connecting wifi  here
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi..");
  }

  Serial.println("Connected to the WiFi network");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}


String sndWfHttp(String tmp_data) {
  String resOpCl="close";   // bfr close and open close
  
  if (WiFi.status() == WL_CONNECTED) {

    HTTPClient http;

    //   http.begin("http://jsonplaceholder.typicode.com/posts/1");
    //   http.begin("http://192.168.137.6/1");
    String apiGetData = serverName + tmp_data;
    Serial.println(apiGetData);
    http.begin(wifiClient, apiGetData);
    http.addHeader("Content-Type", "text/plain");

    int httpResponseCode = http.GET();   //("PUT sent from ESP8266");

    if (httpResponseCode > 0) {

      String response = http.getString();
      resOpCl=response;
      Serial.println(httpResponseCode);
      Serial.println(response);

    } else {

      Serial.print("Error on sending PUT Request: ");
      Serial.println(httpResponseCode);

    }

    http.end();

  } else {
    Serial.println("Error in WiFi connection");
  }

  return resOpCl;
}
