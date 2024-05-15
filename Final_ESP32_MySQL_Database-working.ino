//======================================== Including the libraries.
#include <WiFi.h>
#include <HTTPClient.h>
#include <Arduino_JSON.h>
#include "DHT.h"
//======================================== 

//======================================== DHT sensor settings (DHT11).
#define DHTPIN 21 //--> Digital Pin connected to the DHT11 sensor.
#define DHTTYPE DHT22 //--> Defines the type of DHT sensor used.
DHT dht(DHTPIN, DHTTYPE); //--> Initialize DHT sensor.
//========================================

// Defines the Digital Pin of the "On Board LED".
#define ON_Board_LED 2 

// Defines GPIO 18 as LED_1.
#define LED_01 18 

// Defines GPIO 19 as LED_2.
#define LED_02 19 

//======================================== SSID and Password of your WiFi router.
const char* ssid = "Wi Believe I Can Fi";
const char* password = "Fc Barcelone2001*_*";
//======================================== 

//======================================== Variables for HTTP POST request data.
String postData = ""; //--> Variables sent for HTTP POST request data.
String payload = "";  //--> Variable for receiving response from HTTP POST.
//======================================== 

//======================================== Variables for DHT11 sensor data.
float send_Temp;
int send_Humd;
String send_Status_Read_DHT11 = "";
//======================================== 

//________________________________________________________________________________ Subroutine to control LEDs after successfully fetching data from database.
void control_LEDs() {
  Serial.println();
  Serial.println("---------------control_LEDs()");
  JSONVar myObject = JSON.parse(payload);

  // JSON.typeof(jsonVar) can be used to get the type of the var
  if (JSON.typeof(myObject) == "undefined") {
    Serial.println("Parsing input failed!");
    Serial.println("---------------");
    return;
  }

  if (myObject.hasOwnProperty("LED_01")) {
    Serial.print("myObject[\"LED_01\"] = ");
    Serial.println(myObject["LED_01"]);
  }

  if (myObject.hasOwnProperty("LED_02")) {
    Serial.print("myObject[\"LED_02\"] = ");
    Serial.println(myObject["LED_02"]);
  }

  if(strcmp(myObject["LED_01"], "ON") == 0)   {digitalWrite(LED_01, HIGH);  Serial.println("LED 01 ON"); }
  if(strcmp(myObject["LED_01"], "OFF") == 0)  {digitalWrite(LED_01, LOW);   Serial.println("LED 01 OFF");}
  if(strcmp(myObject["LED_02"], "ON") == 0)   {digitalWrite(LED_02, HIGH);  Serial.println("LED 02 ON"); }
  if(strcmp(myObject["LED_02"], "OFF") == 0)  {digitalWrite(LED_02, LOW);   Serial.println("LED 02 OFF");}

  Serial.println("---------------");
}
//________________________________________________________________________________ 

// ________________________________________________________________________________ Subroutine to read and get data from the DHT11 sensor.
void get_DHT11_sensor_data() {
  Serial.println();
  Serial.println("-------------get_DHT11_sensor_data()");
  
  // Read temperature as Celsius (the default)
  send_Temp = dht.readTemperature();
  
  // Read Humidity
  send_Humd = dht.readHumidity();
  
  // Read temperature as Fahrenheit (isFahrenheit = true)
  // float ft = dht11_sensor.readTemperature(true);

  // Check if any reads failed.
  if (isnan(send_Temp) || isnan(send_Humd)) {
    Serial.println("Failed to read from DHT sensor!");
    send_Temp = 0.00;
    send_Humd = 0;
    send_Status_Read_DHT11 = "FAILED";
  } else {
    send_Status_Read_DHT11 = "SUCCEED";
  }
  
  Serial.printf("Temperature : %.2f °C\n", send_Temp);
  Serial.printf("Humidity : %d %%\n", send_Humd);
  Serial.printf("Status Read DHT11 Sensor : %s\n", send_Status_Read_DHT11);
  Serial.println("-------------");
}
// ________________________________________________________________________________

//________________________________________________________________________________ VOID SETUP()
void setup() {
  // put your setup code here, to run once:
  
  Serial.begin(115200); //--> Initialize serial communications with the PC.

  pinMode(ON_Board_LED,OUTPUT); //--> On Board LED port Direction output.
  pinMode(LED_01,OUTPUT); //--> LED_01 port Direction output.
  pinMode(LED_02,OUTPUT); //--> LED_02 port Direction output.
  
  digitalWrite(ON_Board_LED, HIGH); //--> Turn on Led On Board.
  digitalWrite(LED_01, HIGH); //--> Turn on LED_01.
  digitalWrite(LED_02, HIGH); //--> Turn on LED_02.

  delay(2000);

  digitalWrite(ON_Board_LED, LOW); //--> Turn off Led On Board.
  digitalWrite(LED_01, LOW); //--> Turn off Led LED_01.
  digitalWrite(LED_02, LOW); //--> Turn off Led LED_02.

  //---------------------------------------- Make WiFi on ESP32 in "STA/Station" mode and start connecting to WiFi Router/Hotspot.
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  //---------------------------------------- 
  
  Serial.println();
  Serial.println("-------------");
  Serial.print("Connecting");

  //---------------------------------------- The process of connecting the WiFi on the ESP32 to the WiFi Router/Hotspot.
//-->sa3at khater matconnektich mel first time ywali ye7seb time out w ya3mel restart lel carte
  int connecting_process_timed_out = 20; //--> 20 = 20 seconds.
  connecting_process_timed_out = connecting_process_timed_out * 2;
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    //........................................ Make the On Board Flashing LED on the process of connecting to the wifi router.
    digitalWrite(ON_Board_LED, HIGH);
    delay(250);
    digitalWrite(ON_Board_LED, LOW);
    delay(250);
    //........................................ 

    //........................................ Countdown "connecting_process_timed_out".
    if(connecting_process_timed_out > 0) connecting_process_timed_out--;
    if(connecting_process_timed_out == 0) {
      delay(1000);
      ESP.restart();
    }
    //........................................ 
  }
  //---------------------------------------- 
  
  digitalWrite(ON_Board_LED, LOW); //--> Turn off the On Board LED when it is connected to the wifi router.
  
  //---------------------------------------- If successfully connected to the wifi router, the IP Address that will be visited is displayed in the serial monitor
  Serial.println();
  Serial.print("Successfully connected to : ");
  Serial.println(ssid);
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
  Serial.println("-------------");
  //---------------------------------------- 

  // Setting up the DHT sensor (DHT22).
  dht.begin();

  delay(2000);
}
//________________________________________________________________________________ 

//________________________________________________________________________________ VOID LOOP()
void loop() {

  //---------------------------------------- Check WiFi connection status.
  if(WiFi.status()== WL_CONNECTED) {
    HTTPClient http;  //--> Declare object of class HTTPClient.
    int httpCode;     //--> Variables for HTTP return code.
    
    //........................................ Process to get LEDs data from database to control LEDs.
    postData = "id=esp32_01";
    payload = "";
  
    digitalWrite(ON_Board_LED, HIGH);
    Serial.println();
    Serial.println("---------------getdata.php");

    http.begin("http://192.168.1.5/final_DHT2LEDS/getdata.php");  //--> Specify request destination
    http.addHeader("Content-Type", "application/x-www-form-urlencoded");        //--> Specify content-type header
   
    httpCode = http.POST(postData); //--> Send the request
    payload = http.getString();     //--> Get the response payload
  
    Serial.print("httpCode : ");
    Serial.println(httpCode); //--> Print HTTP return code
    Serial.print("payload  : ");
    Serial.println(payload);  //--> Print request response payload
    
    http.end();  //--> Close connection
    Serial.println("---------------");
    digitalWrite(ON_Board_LED, LOW);
    //........................................ 

    // Calls the control_LEDs() subroutine.
    control_LEDs();
    
    delay(1000);

    // Calls the get_DHT11_sensor_data() subroutine.
    get_DHT11_sensor_data();
  
    //........................................ The process of sending the DHT11 sensor data and the state of the LEDs to the database.
    String LED_01_State = "";
    String LED_02_State = "";

    if (digitalRead(LED_01) == 1) LED_01_State = "ON";
    if (digitalRead(LED_01) == 0) LED_01_State = "OFF";
    if (digitalRead(LED_02) == 1) LED_02_State = "ON";
    if (digitalRead(LED_02) == 0) LED_02_State = "OFF";
    
    postData = "id=esp32_01";
    postData += "&temperature=" + String(send_Temp);
    postData += "&humidity=" + String(send_Humd);
    postData += "&status_read_sensor_dht11=" + send_Status_Read_DHT11;
    postData += "&led_01=" + LED_01_State;
    postData += "&led_02=" + LED_02_State;
    payload = "";
  
    digitalWrite(ON_Board_LED, HIGH);
    Serial.println();
    Serial.println("---------------updateDHT11data_and_recordtable.php");
    http.begin("http://192.168.1.5/final_DHT2LEDS/updateDHT11data_and_recordtable.php");  //--> Specify request destination
    http.addHeader("Content-Type", "application/x-www-form-urlencoded");  //--> Specify content-type header
   
    httpCode = http.POST(postData); //--> Send the request
    payload = http.getString();  //--> Get the response payload
  
    Serial.print("httpCode : ");
    Serial.println(httpCode); //--> Print HTTP return code
    Serial.print("payload  : ");
    Serial.println(payload);  //--> Print request response payload
    
    http.end();  //Close connection
    Serial.println("---------------");
    digitalWrite(ON_Board_LED, LOW);
    //........................................ 
    
    delay(4000);
  }
  //---------------------------------------- 
}
