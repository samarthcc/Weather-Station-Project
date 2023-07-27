#include "WiFi.h"
#include "ESPAsyncWebServer.h"
#include <Adafruit_Sensor.h>
#include <DHT.h>
#include <HardwareSerial.h>
#include <TinyGPS++.h>



// Replace with your network credentials
const char* ssid = "";           // SSID
const char* password = "";       // Password

#define DHTPIN 4      // Digital pin connected to the DHT sensor
#define LED1 2


#define DHTTYPE   DHT11     


DHT dht(DHTPIN, DHTTYPE);

TinyGPSPlus gps;
HardwareSerial SerialGPS(2);


// Create AsyncWebServer object on port 80
AsyncWebServer server(80);

String readDHTTemperature() {
  
  // Read temperature as Celsius (the default)
  float t = dht.readTemperature();
  // Read temperature as Fahrenheit (isFahrenheit = true)
  //float t = dht.readTemperature(true);
  
  if (isnan(t)) {    
    Serial.println("Failed to read from DHT sensor!");
    return "--";
  }
  else {
    Serial.println(t);
    digitalWrite(LED1, 1);  //on
    delay(1000);            //delay

    digitalWrite(LED1, 0);  //off
    delay(1000);
    return String(t);
  }
}

String readDHTHumidity() {
  
  float h = dht.readHumidity();
  if (isnan(h)) {
    Serial.println("Failed to read from DHT sensor!");
    return "--";
  }
  else {
    Serial.println(h);
    return String(h);
  }
}


const char index_html[] PROGMEM = R"rawliteral(
<!DOCTYPE HTML><html>
<head>
    <meta name="viewport" content="width=device-width, initial-scale=1">
    <link rel="stylesheet" href="https://fonts.googleapis.com/css2?family=Material+Symbols+Outlined:opsz,wght,FILL,GRAD@20..48,100..700,0..1,-50..200" />
    <link rel="stylesheet" href="https://fonts.googleapis.com/css2?family=Material+Symbols+Outlined:opsz,wght,FILL,GRAD@20..48,100..700,0..1,-50..200" />
    <link rel="stylesheet" href="https://fonts.googleapis.com/css2?family=Material+Symbols+Outlined:opsz,wght,FILL,GRAD@20..48,100..700,0..1,-50..200" />
    <link rel="stylesheet" href="https://fonts.googleapis.com/css2?family=Material+Symbols+Outlined:opsz,wght,FILL,GRAD@20..48,100..700,0..1,-50..200" />
    <link rel="stylesheet" href="https://fonts.googleapis.com/css2?family=Material+Symbols+Outlined:opsz,wght,FILL,GRAD@20..48,100..700,0..1,-50..200" />
    <link rel="stylesheet" href="https://fonts.googleapis.com/css2?family=Material+Symbols+Outlined:opsz,wght,FILL,GRAD@20..48,100..700,0..1,-50..200" />
    <style>
       
        body {
         
        font-family: 'Arial', sans-serif;
        font-size: 16px;
        line-height: 1.5;
        color: #333;
        background-color: #f2f2f2;
        background-image: linear-gradient(315deg, #f2f2f2 0%, #dbdbdb 74%);
        text-align: center;
        }
        h2 {
          font-size: 3.0rem;
          color: linear-gradient(to bottom, #33ccff 0%, #ff6699 100%);
          text-transform: uppercase;
          margin-top: 30px;
          margin-bottom: 30px;
        }
        
        .styled-table {
            border-collapse: collapse;
            border-radius: 25px;
            margin: 25px 0;
            font-size: 0.9em;
            font-family: sans-serif;
            min-width: 400px;
            box-shadow: 0 0 20px rgba(8, 128, 118, 0.15);
            margin-left: auto;
            margin-right: auto;
            background-color: #fff;
            
        }
        .styled-table tbody tr.active-row {
            font-weight: bold;
            color: #206a59;
        }
        .styled-table tbody tr:hover {
          background-color: #f2f2f2;
          cursor: pointer;
        }
        .styled-table td:hover {
          box-shadow: 0 0 10px rgba(0, 0, 0, 0.1);
        }
         
        p {
          font-size: 2.0rem;
          margin-bottom: 10px;
        }
        .material-symbols-outlined {
          font-family: 'Material Symbols Outlined', sans-serif;
          font-size: 3.5rem;
          vertical-align: middle;
          margin-right: 10px;
        }
        .dht-labels {
          font-size: 1.5rem;
          vertical-align: middle;
          margin-right: 10px;
          color: #444;
        }
        .temperature {
          font-weight: bold;
          font-size: 24px;
          color: #ff6600;
        }
        .humidity {
          font-weight: normal;
          font-size: 18px;
          color: #0080ff;
          text-decoration: underline;
        }
        .latitude, .longitude {
          font-size: 16px;
          color: #666;
          background-color: #f2f2f2;
          border: 1px solid #ccc;
          border-radius: 5px;
          padding: 5px;
        }
        .speed {
          font-size: 20px;
          color: #00b300;
          text-shadow: 1px 1px #ccc;
          letter-spacing: 2px;
        }
        .date {
          font-size: 14px;
          color: #999;
          font-style: italic;
        }
        .units {
          font-size: 1.5rem;
          color: #666;
          margin-left: 5px;
        }
      </style>
  </head>
  <body>
    
    <h2>Tracking And Managaement</h2>
    <table class="styled-table" background="">
      <tr class="active-row">
        <td>
          <p>
            <span class="material-symbols-outlined" style="color:#B62C0A;">thermometer</span>
            <span class="dht-labels" style="color:#B62C0A ;">Temperature:</span> 
            <span id="temperature">%TEMPERATURE%</span>
            <sup class="units">&deg;C</sup>
          </p>
        </td>
        <td>
          <p>
            <span class="material-symbols-outlined" style="color:#1D338E; ">humidity_percentage</span>
            <span class="dht-labels" style="color:#1D338E; ">Humidity:</span>
            <span id="humidity">%HUMIDITY%</span>
            <sup class="units">&percnt;</sup>
          </p>
        </td>
      </tr>
      <tr>
        <td>
          <p>
            <span class="material-symbols-outlined" style="color: #24305f">location_on</span>
            <span class="dht-labels" style="color: #24305f">Latitude:</span>
            <span id="latitude">%LATITUDE%</span>
            <sup class="units">&deg;N</sup>
          </p>
        </td>
        <td>
          <p>
            <span class="material-symbols-outlined" style="color:#24305f">location_on</span>
            <span class="dht-labels" style="color:#24305f">Longitude :</span>
            <span id="longitude">%LONGITUDE%</span>
            <sup class="units">&deg;E</sup>
          </p>
        </td>
      </tr>
      <tr>
        <td>
          <p>
            <span class="material-symbols-outlined" style="color:#E9B41B ">pace</span>
            <span class="dht-labels" style="color:#E9B41B ">Speed :</span>
            <span id="speed">%SPEED%</span>
            <sup class="units">MPH</sup>
          </p>
        </td>
        <td>
          <p>
            <span class="material-symbols-outlined">calendar_month</span>
            <span class="dht-labels" style="color:#544310 ">Date :</span>
            <span id="date">%DATE%</span>
            <sup class="units"></sup>
          </p>
        </td>
      </tr>
    </table>
    
  </body>
  <script>
  setInterval(function ( ) {
    var xhttp = new XMLHttpRequest();
    xhttp.onreadystatechange = function() {
      if (this.readyState == 4 && this.status == 200) {
        document.getElementById("temperature").innerHTML = this.responseText;
      }
    };
    xhttp.open("GET", "/temperature", true);
    xhttp.send();
  }, 5000 ) ;
  
  setInterval(function ( ) {
    var xhttp = new XMLHttpRequest();
    xhttp.onreadystatechange = function() {
      if (this.readyState == 4 && this.status == 200) {
        document.getElementById("humidity").innerHTML = this.responseText;
      }
    };
    xhttp.open("GET", "/humidity", true);
    xhttp.send();
  }, 5000 ) ;
  setInterval(function ( ) {
    var xhttp = new XMLHttpRequest();
    xhttp.onreadystatechange = function() {
      if (this.readyState == 4 && this.status == 200) {
        document.getElementById("latitude").innerHTML = this.responseText;
      }
    };
    xhttp.open("GET", "/latitude", true);
    xhttp.send();
  }, 5000 ) ;
  setInterval(function ( ) {
    var xhttp = new XMLHttpRequest();
    xhttp.onreadystatechange = function() {
      if (this.readyState == 4 && this.status == 200) {
        document.getElementById("longitude").innerHTML = this.responseText;
      }
    };
    xhttp.open("GET", "/longitude", true);
    xhttp.send();
  }, 5000 ) ;
  setInterval(function ( ) {
    var xhttp = new XMLHttpRequest();
    xhttp.onreadystatechange = function() {
      if (this.readyState == 4 && this.status == 200) {
        document.getElementById("speed").innerHTML = this.responseText;
      }
    };
    xhttp.open("GET", "/speed", true);
    xhttp.send();
  }, 5000 ) ;
  setInterval(function ( ) {
    var xhttp = new XMLHttpRequest();
    xhttp.onreadystatechange = function() {
      if (this.readyState == 4 && this.status == 200) {
        document.getElementById("date").innerHTML = this.responseText;
      }
    };
    xhttp.open("GET", "/date", true);
    xhttp.send();
  }, 5000 ) ;
  
  
  </script>
</html>)rawliteral";

// Replaces placeholder with DHT values
String processor(const String& var) {
  if (var == "TEMPERATURE") {
    return readDHTTemperature();
  } else if (var == "HUMIDITY") {
    return readDHTHumidity();
  } else if (var == "LATITUDE") {
    float lat = gps.location.lat();
    // convert float to string with 6 decimal places
    char latStr[15];
    dtostrf(lat, 7, 6, latStr);
    return latStr;
  }
  else if(var=="LONGITUDE"){
    float lat = gps.location.lng();
    // convert float to string with 6 decimal places
    char latStr[15];
    dtostrf(lat, 7, 6, latStr);
    return latStr;
  }
  else if(var == "SPEED") {
    char speedStr[10];
    dtostrf(gps.speed.mph(), 6, 2, speedStr); // convert double to string with 2 decimal places
    return speedStr;
  }
  else if(var == "DATE") {
  char altStr[10];
  sprintf(altStr, "%02d/%02d/%02d", gps.date.month(), gps.date.day(), gps.date.year());
  return altStr;
}
  
  return String();
}

void setup(){
  // Serial port for debugging purposes
  Serial.begin(115200);
  SerialGPS.begin(9600, SERIAL_8N1, 16, 17);
  pinMode(LED1, OUTPUT);
  dht.begin();
  
  
  // Connect to Wi-Fi
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi..");
  }

  // Print ESP32 Local IP Address
  Serial.println(WiFi.localIP());

  // Route for root / web page
  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send_P(200, "text/html", index_html, processor);
  });
  server.on("/temperature", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send_P(200, "text/plain", readDHTTemperature().c_str());
  });
  server.on("/humidity", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send_P(200, "text/plain", readDHTHumidity().c_str());
  });
  



  // Start server
  server.begin();
}
 
void loop(){

  while(SerialGPS.available()>0){
    char c = SerialGPS.read();
    
    gps.encode(c);
  }

  if (gps.location.isValid()) {
    
    server.on("/latitude", HTTP_GET, [](AsyncWebServerRequest *request){
      String latitudeStr = String(gps.location.lat(), 6); // Convert float to string with 6 decimal places
      request->send_P(200, "text/plain", latitudeStr.c_str());
    });
    server.on("/longitude", HTTP_GET, [](AsyncWebServerRequest *request){
        String longitudeStr = String(gps.location.lng(), 6); // Convert float to string with 6 decimal places
        request->send_P(200, "text/plain", longitudeStr.c_str());
    });
    server.on("/speed", HTTP_GET, [](AsyncWebServerRequest *request){
    String speedStr = String(gps.speed.mph(), 1); // Convert float to string with 1 decimal place
    request->send_P(200, "text/plain", speedStr.c_str());
    });
    server.on("/date", HTTP_GET, [](AsyncWebServerRequest *request){
    String dateStr=String(gps.date.value());                  // Convert date to string
    request->send_P(200, "text/plain",dateStr.c_str());
    });
    
  }
  
  delay(5000);
  
}
