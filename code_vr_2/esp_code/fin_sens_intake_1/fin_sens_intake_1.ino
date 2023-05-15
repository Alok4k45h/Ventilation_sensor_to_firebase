#include <Arduino.h>
#if defined(ESP32)
#include <WiFi.h>
#elif defined(ESP8266)
#include <ESP8266WiFi.h>
#endif

#include "ESP8266HTTPClient.h"
#include "ESP8266WiFi.h"
#include <WiFiClient.h>

WiFiClient wifiClient;

const char *ssid = "CeresOS";
const char *password = "Unknown123";

// ***** htp data here
String htp_data; //="?temperature=24.37";
String serverName = "http://192.168.137.1:8000/hndl_url/sensor_intake?";
// String serverPath = serverName;

// dht work

#include <Adafruit_Sensor.h>
#include <DHT.h>
#include <DHT_U.h>
#define DHTPIN 32 // 2     // Digital pin connected to the DHT sensor

// Uncomment the type of sensor in use:
#define DHTTYPE DHT11 // DHT 11
// #define DHTTYPE    DHT22     // DHT 22 (AM2302)
// #define DHTTYPE    DHT21     // DHT 21 (AM2301)
DHT_Unified dht(DHTPIN, DHTTYPE);

// data format to the firebase of humidty and temp
String rtb_pth = "reads";
String sensrid = "Sen_1";
float tmprt = 0;
float humdty = 0;
void updt_dta();

void setup()
{
    Serial.begin(115200);

    // Initialize device.
    dht.begin();
    Serial.println(F("DHTxx Unified Sensor Example")); // dht sensor
    // ********  connecting wifi  here
    WiFi.begin(ssid, password);

    while (WiFi.status() != WL_CONNECTED)
    {
        delay(1000);
        Serial.println("Connecting to WiFi..");
    }

    Serial.println("Connected to the WiFi network");
    Serial.println("IP address: ");
    Serial.println(WiFi.localIP());
    //  serverName="http://"+WiFi.localIP().toString()+":8000/hndl_url/rfid/1?";
    //  String serverName=sprintf("http://",WiFi.localIP(),":8000/hndl_url/rfid/1?");
}

void loop()
{
    // dht logic to send data to the django server
    int count=0;
    String temp_pth = rtb_pth + "_" + String(count);
    String temp_snsrid = sensrid;
    float temp_tmprt = tmprt;
    float temp_hmd = humdty;

    void updt_dta();

    Serial.println();
    // htp_data = dump_byte_array(rfid.uid.uidByte, rfid.uid.size);
    htp_data = String(tmprt)+"-"+String(humdty);
    sndWfHttp(htp_data);
    delay(10000);
    count+=1;

    // // Reset the loop if no new card present on the sensor/reader. This saves the entire process when idle.
    // if (!rfid.PICC_IsNewCardPresent())
    //   return;
    // // Verify if the NUID has been readed
    // if (!rfid.PICC_ReadCardSerial())
    //   return;
    // Serial.print(F("PICC type: "));
    // MFRC522::PICC_Type piccType = rfid.PICC_GetType(rfid.uid.sak);
    // Serial.println(rfid.PICC_GetTypeName(piccType));
    // // Check is the PICC of Classic MIFARE type
    // if (piccType != MFRC522::PICC_TYPE_MIFARE_MINI &&
    //     piccType != MFRC522::PICC_TYPE_MIFARE_1K &&
    //     piccType != MFRC522::PICC_TYPE_MIFARE_4K)
    // {
    //   Serial.println(F("Your tag is not of type MIFARE Classic."));
    //   return;
    // }
    // if (rfid.uid.uidByte[0] != nuidPICC[0] ||
    //     rfid.uid.uidByte[1] != nuidPICC[1] ||
    //     rfid.uid.uidByte[2] != nuidPICC[2] ||
    //     rfid.uid.uidByte[3] != nuidPICC[3])
    // {
    //   Serial.println(F("A new card has been detected."));
    //   // Store NUID into nuidPICC array
    //   for (byte i = 0; i < 4; i++)
    //   {
    //     nuidPICC[i] = rfid.uid.uidByte[i];
    //   }
    //   Serial.println(F("The NUID tag is:"));
    //   Serial.print(F("In hex: "));
    //   htp_data = dump_byte_array(rfid.uid.uidByte, rfid.uid.size);
    //   printHex(rfid.uid.uidByte, rfid.uid.size);
    //   Serial.println();
    //   sndWfHttp(htp_data);
    //   Serial.println("http_data_Sending_done")
    //   //      Serial.print(F("In dec: "));
    //   //      printDec(rfid.uid.uidByte, rfid.uid.size);
    //   //      Serial.println();
    // }
    // else
    //   Serial.println(F("Card read previously."));
    // // Halt PICC
    // rfid.PICC_HaltA();
    // // Stop encryption on PCD
    // rfid.PCD_StopCrypto1();
}

/**
    Helper routine to dump a byte array as hex values to Serial.
*/

void printHex(byte *buffer, byte bufferSize)
{
    for (byte i = 0; i < bufferSize; i++)
    {
        Serial.print(buffer[i] < 0x10 ? " 0" : " ");
        Serial.print(buffer[i], HEX);
    }
}
/**
    Helper routine to dump a byte array as dec values to Serial.
*/
void printDec(byte *buffer, byte bufferSize)
{
    for (byte i = 0; i < bufferSize; i++)
    {
        Serial.print(buffer[i] < 0x10 ? " 0" : " ");
        Serial.print(buffer[i], DEC);
    }
}




void sndWfHttp(String tmp_data)
{
    if (WiFi.status() == WL_CONNECTED)
    {

        HTTPClient http;

        //   http.begin("http://jsonplaceholder.typicode.com/posts/1");
        //   http.begin("http://192.168.137.6/1");
        String apiGetData = serverName + tmp_data;
        Serial.println(apiGetData);
        http.begin(wifiClient, apiGetData);
        http.addHeader("Content-Type", "text/plain");

        int httpResponseCode = http.GET(); //("PUT sent from ESP8266");

        if (httpResponseCode > 0)
        {

            String response = http.getString();

            Serial.println(httpResponseCode);
            Serial.println(response);
        }
        else
        {

            Serial.print("Error on sending GET Request: ");
            Serial.println(httpResponseCode);
        }

        http.end();
    }
    else
    {
        Serial.println("Error in WiFi connection");
    }
}

// dump_byte_array
String dump_byte_array(byte *buffer, byte bufferSize)
{
    String str = "";
    for (byte i = 0; i < bufferSize; i++)
    {
        if (((buffer[i] & 0xF0) >> 4) <= 9)
        {
            str = str + (char)(((buffer[i] & 0xF0) >> 4) + '0'); // convert HEX 0-9 to ASCII
        }
        else
        {
            str = str + (char)(((buffer[i] & 0xF0) >> 4) + 'A' - 10); // convert HEX A-F to ASCII
        }
        if ((buffer[i] & 0x0F) <= 9)
        {
            str = str + (char)((buffer[i] & 0x0F) + '0'); // convert HEX 0-9 to ASCII
        }
        else
        {
            str = str + (char)((buffer[i] & 0x0F) + 'A' - 10); // convert HEX A-F to ASCII
        }
        // if (i < (bufferSize - 1)) str = str + ' '; //uncomment if you want a space between each HEX number
    }
    Serial.println(str); //<- just for debug
    return str;
}



// ***************    dht sensor data update

void updt_dta()
{
    sensors_event_t event;
    // Get temperature event and print its value.
    dht.temperature().getEvent(&event);
    if (isnan(event.temperature))
    {
        Serial.println(F("Error reading temperature!"));
    }
    else
    {
        Serial.print(F("Temperature: "));
        float tmprt = event.temperature;
        Serial.print(event.temperature);
        Serial.println(F("°C"));
    }
    // Get humidity event and print its value.
    dht.humidity().getEvent(&event);
    if (isnan(event.relative_humidity))
    {
        Serial.println(F("Error reading humidity!"));
    }
    else
    {
        Serial.print(F("Humidity: "));
        humdty = event.relative_humidity;
        Serial.print(event.relative_humidity);
        Serial.println(F("%"));
    }
}