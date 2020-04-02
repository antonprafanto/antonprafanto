/* This code works with ESP8266 12E (NodeMcu v1.0) its main function is to read the data from https://www.worldometers.info/
 * Through ThingSpeak app ThingHTTP and displays on the Serial monitor
 * Refer to www.SurtrTech.com for more details and video about the project
 * This project was created during the COVID-19 Pandemic and the example here used to track the numbers in Italy  
 */

#include "WiFiClient.h"
#include "HTTPClient.h"
//------OLED----
#include <Adafruit_SSD1306.h>
#include <U8g2lib.h>
#define SDA 19
#define SCL 23
U8G2_SSD1306_128X64_NONAME_1_SW_I2C display1(U8G2_R0, SCL, SDA);
//--------------
                              
const char* ssid = "xl";              //Your router SSID and password                             
const char* password =  "wifigratiss"; 
const char* host = "api.thingspeak.com";  //We read the data from this host                                   
const int httpPortRead = 80;                                        
                                                   
                                                
const char* url1 = "/apps/thinghttp/send_request?api_key=TOTVFIQ31SANZIGJ";     //This URL is mine but I kept it so you can replace the key only
const char* url2 = "/apps/thinghttp/send_request?api_key=OBWRWU49V5I7QSR1";     //This URL is mine but I kept it so you can replace the key only



int To_remove,n;      //There are some irrelevant data on the string and here's how I keep the index
                    //of those characters 

String Cases,Deaths,Recovered,Data_Raw,Data_Raw_1,Data_Raw_2,Data_Raw_3;  //Here I keep the numbers that I got

WiFiClient client;      //Create a WiFi client and http client                                                     
HTTPClient http; 

void setup() {
  
  Serial.begin(9600);            //Start the serial communication 
  display1.begin();                                                             

  WiFi.disconnect();             //Disconnect and reconnect to the Wifi you set                                                 
  delay(1000);                                                                  
  WiFi.begin(ssid, password);                                                   

  Serial.println("Connected to the WiFi network");   //Display feedback on the serial monitor                                        
  Serial.println(WiFi.localIP()); 
  
}

//In the loop I read every URL separately and I get the data string then I remove the unecessary characters
//I keep only the values that I can display either on the serial monitor or the display
//I believe that this part is repeated 3 times and can be set on a single function that can be called
//Keep updated on www.SurtrTech.com for an optimized code  

 void loop()                                                                      
{
   if(n<=3){
      //Reading 1: Reading of cases
    if( http.begin(host,httpPortRead,url1))        //Connect to the host and the url                                      
      {
        int httpCode = http.GET();                //Check feedback if there's a response                                                  
        if (httpCode > 0)                                                              
        {
          if (httpCode == HTTP_CODE_OK || httpCode == HTTP_CODE_MOVED_PERMANENTLY) 
            {                
              
              Data_Raw = http.getString();   //Here we store the raw data string
            
              
              Data_Raw_1 = Data_Raw; 
                                                   
              To_remove = Data_Raw_1.indexOf(">");                      //I look for the position of this symbol ">"                                 
              Data_Raw_1.remove(0,To_remove+1);                        //I remove it and everything that's before
              To_remove = Data_Raw_1.indexOf("<");                     //I look for the position of this symbol ">"                                
              Data_Raw_1.remove(To_remove,Data_Raw_1.length());          //I remove it and everything that's after
                                                                     //Example: This is the raw data received <td style="font-weight: bold; text-align:right">63,927</td>
                                                                     //First we look for ">" and we remove everything before it including it
                                                                     //We stay with this 63,927</td>
                                                                     //We look for "<" symbol and we remove it + everything after
                                                                     //We keep only this 63,927 as string
              Cases=Data_Raw_1;
              Serial.print("Cases: ");  //I choosed to display it on the serial monitor to help you debug
              Serial.println(Cases); 

              Data_Raw_2=Data_Raw;

              To_remove = Data_Raw_2.indexOf("<span>");
              Data_Raw_2.remove(0,To_remove+6);
              Data_Raw_3=Data_Raw_2;
              To_remove = Data_Raw_2.indexOf("</span>");
              Data_Raw_2.remove(To_remove,Data_Raw_2.length());

              Deaths=Data_Raw_2;

              Serial.print("Deaths: ");
              Serial.println(Deaths);


              To_remove = Data_Raw_3.indexOf("<span>");
              Data_Raw_3.remove(0,To_remove+6);

              To_remove = Data_Raw_3.indexOf("<");
              Data_Raw_3.remove(To_remove,Data_Raw_3.length());

              Recovered=Data_Raw_3;
              
              Serial.print("Recovered: ");
              Serial.println(Recovered);
                                                                                        
            }
            display1.firstPage();
            do {
              display1.setFont(u8g2_font_9x15B_tf);
              display1.setCursor(20,10);
              display1.print("Indonesia");

              display1.setFont(u8g2_font_7x13B_tf);
              display1.setCursor(0,30);
              display1.print("Cases:");
              display1.setCursor(47,30);
              display1.print(Cases);

              display1.setFont(u8g2_font_7x13B_tf);
              display1.setCursor(0,45);
              display1.print("Deaths:");
              display1.setCursor(54,45);
              display1.print(Deaths);

              display1.setFont(u8g2_font_7x13B_tf);
              display1.setCursor(0,60);
              display1.print("Recovered:");
              display1.setCursor(75,60);
              display1.print(Recovered);
              
            } while ( display1.nextPage() );


        }
        else //If we can't get data
        {
          Serial.printf("[HTTP] GET... failed, error: %s\n", http.errorToString(httpCode).c_str());
        }
        http.end();                                                                 
      } 
      else //If we can't connect to the HTTP
        {
          Serial.printf("[HTTP} Unable to connect\n");
        }
       n++;
     }
     else if(n>3 && n<8){
        if( http.begin(host,httpPortRead,url2))        //Connect to the host and the url                                      
        {
          int httpCode = http.GET();                //Check feedback if there's a response                                                  
          if (httpCode > 0)                                                              
          {
            if (httpCode == HTTP_CODE_OK || httpCode == HTTP_CODE_MOVED_PERMANENTLY) 
              {                
                
                Data_Raw = http.getString();   //Here we store the raw data string
              
                
                Data_Raw_1 = Data_Raw; 
                                                     
                To_remove = Data_Raw_1.indexOf(">");                      //I look for the position of this symbol ">"                                 
                Data_Raw_1.remove(0,To_remove+1);                        //I remove it and everything that's before
                To_remove = Data_Raw_1.indexOf("<");                     //I look for the position of this symbol ">"                                
                Data_Raw_1.remove(To_remove,Data_Raw_1.length());          //I remove it and everything that's after
                                                                       //Example: This is the raw data received <td style="font-weight: bold; text-align:right">63,927</td>
                                                                       //First we look for ">" and we remove everything before it including it
                                                                       //We stay with this 63,927</td>
                                                                       //We look for "<" symbol and we remove it + everything after
                                                                       //We keep only this 63,927 as string
                Cases=Data_Raw_1;
                Serial.print("Cases: ");  //I choosed to display it on the serial monitor to help you debug
                Serial.println(Cases); 
  
                Data_Raw_2=Data_Raw;
  
                To_remove = Data_Raw_2.indexOf("<span>");
                Data_Raw_2.remove(0,To_remove+6);
                Data_Raw_3=Data_Raw_2;
                To_remove = Data_Raw_2.indexOf("</span>");
                Data_Raw_2.remove(To_remove,Data_Raw_2.length());
  
                Deaths=Data_Raw_2;
  
                Serial.print("Deaths: ");
                Serial.println(Deaths);
  
  
                To_remove = Data_Raw_3.indexOf("<span>");
                Data_Raw_3.remove(0,To_remove+6);
  
                To_remove = Data_Raw_3.indexOf("<");
                Data_Raw_3.remove(To_remove,Data_Raw_3.length());
  
                Recovered=Data_Raw_3;
                
                Serial.print("Recovered: ");
                Serial.println(Recovered);
                                                                                          
              }
              display1.firstPage();
              do {
                display1.setFont(u8g2_font_9x15B_tf);
                display1.setCursor(20,10);
                display1.print("World");
  
                display1.setFont(u8g2_font_7x13B_tf);
                display1.setCursor(0,30);
                display1.print("Cases:");
                display1.setCursor(47,30);
                display1.print(Cases);
  
                display1.setFont(u8g2_font_7x13B_tf);
                display1.setCursor(0,45);
                display1.print("Deaths:");
                display1.setCursor(54,45);
                display1.print(Deaths);
  
                display1.setFont(u8g2_font_7x13B_tf);
                display1.setCursor(0,60);
                display1.print("Recovered:");
                display1.setCursor(75,60);
                display1.print(Recovered);
                
              } while ( display1.nextPage() );

  
          }
          else //If we can't get data
          {
            Serial.printf("[HTTP] GET... failed, error: %s\n", http.errorToString(httpCode).c_str());
          }
          http.end();                                                                 
        } 
        else //If we can't connect to the HTTP
        {
          Serial.printf("[HTTP} Unable to connect\n");
        }
      n++;
     }
     else{
      if(n>=8){
        n=0;
      }
     }
    
    delay(600);

    while (WiFi.status() != WL_CONNECTED)     //In case the Wifi connexion is lost                                    
      { 
        
        WiFi.disconnect();                                                        
        delay(1000);                                                             
        
        WiFi.begin(ssid, password);                                              
        Serial.println("Reconnecting to WiFi..");       
        delay(10000);                                                             
      }

} 
  
