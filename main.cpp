#include <Arduino.h>
#include <WiFi.h>
#include <WiFiAP.h>


//Network credentials
const char* ssid = "esp32";
const char* password = "password123";


//web server port
WiFiServer server(80);

// variable for http request
String header;

// car state
String car_state = "not moving";

//assign variables to motors
const int left_clock = 27;
const int left_counter = 26;
const int right_clock = 18;
const int right_counter = 19;
const int speed1 = 32;
const int speed2 = 33;


void setup()
{
  Serial.begin(115200);
  Serial.println("configuring accesss point");
  // initialize pins to output
  pinMode(speed1, OUTPUT);
  pinMode(speed2, OUTPUT);
  pinMode(left_clock, OUTPUT);
  pinMode(left_counter, OUTPUT);  
  pinMode(right_clock, OUTPUT);
  pinMode(right_counter, OUTPUT);

  //set pins to low
  digitalWrite(left_counter,LOW);
  digitalWrite(left_clock,LOW);
  digitalWrite(right_clock,LOW);
  digitalWrite(right_counter,LOW);

  //connect to wifi
  if (!WiFi.softAP(ssid,password)){
    log_e("soft AP creation failed");
    while (1);
  }

  IPAddress myIP = WiFi.softAPIP();
  Serial.print("AP IP address: ");
  Serial.println(myIP);
  server.begin();
  Serial.println("server started");


}

void loop()
{
  WiFiClient client = server.available(); // check for clients that connect to server

  if (client)
  {
    Serial.println("New Client");
    String currentLine = "";
    while (client.connected())
    {
      if (client.available())
      {
        char c = client.read();
        Serial.write(c);
        header += c;
        if (c == '\n')
        {
          if (currentLine.length()==0)
          {
            client.println("HTTP/1.1 200 OK");
            client.println("Content-type:text/html");
            client.println("Connection: close");
            client.println();
        
          if(header.indexOf("GET /forward") >=0)
          {
            Serial.println("going forward");
            car_state = "going forward";
            analogWrite(speed1, 200);
            analogWrite(speed2, 200);
            digitalWrite(left_counter,HIGH);
            digitalWrite(left_clock,LOW);
            digitalWrite(right_clock,HIGH);
            digitalWrite(right_counter,LOW);
          }

          else if(header.indexOf("GET /back") >=0)
          {
            Serial.println("going back");
            car_state = "going back";
            analogWrite(speed1, 200);
            analogWrite(speed2, 200);
            digitalWrite(left_counter,LOW);
            digitalWrite(left_clock,HIGH);
            digitalWrite(right_clock,LOW);
            digitalWrite(right_counter,HIGH);
          }

          else if(header.indexOf("GET /right") >=0)
          {
            Serial.println("going right");
            car_state = "going right";
            analogWrite(speed1, 140);
            analogWrite(speed2, 140);
            digitalWrite(left_counter,HIGH);
            digitalWrite(left_clock,LOW);
            digitalWrite(right_clock, LOW);
            digitalWrite(right_counter,HIGH);
          }
          
          else if(header.indexOf("GET /left") >=0)
          {
            Serial.println("going left");
            car_state = "going left";
            analogWrite(speed1, 140);
            analogWrite(speed2, 140);
            digitalWrite(left_counter,LOW);
            digitalWrite(left_clock,HIGH);
            digitalWrite(right_clock,HIGH);
            digitalWrite(right_counter,LOW);
          }

          else if(header.indexOf("GET /stop") >=0)
          {
            Serial.println("Stopping");
            car_state = "Not moving";
            analogWrite(speed1, 0);
            analogWrite(speed2, 0);
            digitalWrite(left_counter,LOW);
            digitalWrite(left_clock,LOW);
            digitalWrite(right_clock,LOW);
            digitalWrite(right_counter,LOW);
          }

          client.println("<!DOCTYPE html><html>");
            client.println("<head><meta name=\"viewport\" content=\"width=device-width, initial-scale=1\">");
            client.println("<link rel=\"icon\" href=\"data:,\">");
            // CSS to style the on/off buttons 
            // Feel free to change the background-color and font-size attributes to fit your preferences
            client.println("<style>html { font-family: Helvetica; display: inline-block; margin: 0px auto; text-align: center;}");
            client.println(".button { background-color: #4CAF50; border: none; color: white; padding: 16px 40px;");
            client.println("text-decoration: none; font-size: 30px; margin: 2px; cursor: pointer;}");
            client.println(".button2 {background-color: #555555;}</style></head>");
            
            // Web Page Heading
            client.println("<body><h1>ESP32 Web Server</h1>");
            
            // Display current state, and ON/OFF buttons for GPIO 26  
            client.println("<p>" + car_state + "</p>");
            // If the output26State is off, it displays the ON button       
      
            client.println("<p><a href=\"/forward\"><button class=\"button\">forward</button></a></p>");
            client.println("<p><a href=\"/back\"><button class=\"button\">back</button></a></p>");
            client.println("<p><a href=\"/right\"><button class=\"button\">right</button></a></p>");
            client.println("<p><a href=\"/left\"><button class=\"button\">left</button></a></p>");
            client.println("<p><a href=\"/stop\"><button class=\"button\">stop</button></a></p>");
            client.println("</body></html>");
            
            // The HTTP response ends with another blank line
            client.println();
            // Break out of the while loop
            break;
          }
          else
          {
            currentLine = "";
          }
        }

        else if (c != '\r')
        {
          currentLine += c;
        }
      }
    }

    header = "";

    //close the connection

    client.stop();
    Serial.println("Client disconnected.");
    Serial.println(""); 
  }
}