#include <Arduino.h>
#include <WiFiClient.h>
#include "wifi-driver.h"
// #include "webServer.h"
#include "lm75-driver.h"
#include <Wire.h>
// #include <ArduinoJson.h>
#include <ldr-driver.h>
#include <led-driver.h>
#include <voltage-driver.h>
#include <dust_sensor.h>
// #include <sim800.h>


#define BLYNK_TEMPLATE_ID "TMPL2JIEb2LsZ"
#define BLYNK_TEMPLATE_NAME "Solar Street Light IoT Monitoring Device"
#define BLYNK_AUTH_TOKEN "s4pwewhOYhUYa_RdyXQmhCTNedPOYWkB"
#include <BlynkSimpleEsp32.h>

char auth[] = BLYNK_AUTH_TOKEN;

char* ssid = "";
char* wpassword = "";


lm75_config_t lm75Config = {
    .i2c_addr = 0x48, // Replace with the actual I2C address of your LM75 sensor
    .mode = LM75_COMPARATOR_MODE,
    .os_pin_number = 2, // Replace with the actual pin number connected to the LM75's OS pin
    .polarity = LM75_POLARITY_ACTIVE_LOW,
    .tolerance = LM75_TOLERANCE_TWO_SAMPLES,
    .resolution = LM75_RESOLUTION_12_BITS
};

// Drivers object
lm75_t *lm75;
ldr_t* ldr_obj;
led_t* led_obj;
volt_t* volt_obj;
dust_t* dust_obj;
// sim800_t* sim800_obj;
BlynkTimer timer;
// LM75 interrupt handler
void lm75InterruptHandler()
{
    // Handle LM75 interrupt here if needed
}

// static String get_all_data_as_string(){
//     float temp;
//     float volt;
//     int ldr_val;
//     JsonDocument doc;
//     String final_data="";
//     Serial.println("inside callback");
//     error_type_t err = lm75_read(lm75,&temp); 
//     if(err == ERROR_OK){
//         Serial.println("LM75 returns error ok");
//         //final_data += (String)temp;
//         doc["temperature"] = (String)temp;
//     }
//      err =voltage_read(volt_obj, &volt);
//     if (err = ERROR_OK)
//     {
//       Serial.println("voltage return error ok");
//       //final_data += (String)volt;
//       doc["voltage"] = (String)volt;
//     }
    
    // err = ldr_read(ldr_obj,&ldr_val);
//     if(err == ERROR_OK){
//         Serial.println("ldr returns error ok");
//         //final_data += (String)temp;
//         doc["ldr"] = (String)ldr_val;
//     }
//     size_t error = serializeJson(doc, final_data);
//     if (!error){
//         Serial.printf("Json error");
//         return "";
//     }
//     return final_data;
// }

void sendData(){
   float temp;
   float volt;
   float dust;
   int ldr_val;

   error_type_t err = ldr_read(ldr_obj, &ldr_val);
   Blynk.virtualWrite(V1, err);
   if (err = ERROR_OK)
   {
      Serial.println("ldr return error ok");
   }

   err = voltage_read(volt_obj, &volt);
   Blynk.virtualWrite(V2, err);
    if (err = ERROR_OK)
   {
      Serial.println("volt return error ok");
   }

   err = lm75_read(lm75, &temp);
    Blynk.virtualWrite(V3, err);
    if (err = ERROR_OK)
   {
      Serial.println("lm75 return error ok");
   }

   err = dust_read(dust_obj,&dust);
    Blynk.virtualWrite(V4, err);
    if (err = ERROR_OK)
   {
      Serial.println("dust return error ok");
   }


}

void setup() {
   Serial.begin(9600);
   Blynk.begin(BLYNK_AUTH_TOKEN, ssid, wpassword);
   error_type_t err;
   wifi_configuration_t wifi_config = { .ssid = ssid, .password=wpassword};
   wifi_t* wifi_obj = wifi_create(&wifi_config);

  if (wifi_init(wifi_obj) != ERROR_OK) {
        Serial.println("WiFi initialization failed!");
        exit(1);
  }
  if(wifi_connect(wifi_obj) != ERROR_OK){
        Serial.println("WiFi connect failed!");
        exit(1);  
  }

  timer.setInterval(2000L, sendData);
  //webserver
  // webserver_config_t web_config = {.port_number=80,.callback = get_all_data_as_string};
  // webserver_t* webserver = webserver_create(&web_config);
  // if(!webserver){
  //   Serial.println("could not create webserver object");
  //   exit(1);
  // }
  // err = webserver_init(webserver);
  // if(err != ERROR_OK){
  //   Serial.println("could not init webserver object");
  //   exit(1);
  // }
  // err = webserver_begin(webserver);
  // if(err != ERROR_OK){
  //   Serial.println("could not begin webserver");
  //   exit(1);
  // }

    //create ldr obj
    ldr_config_t ldr_driver= {.ldr_pin_number = 32};

    ldr_obj = ldr_create(&ldr_driver);
    if(!ldr_obj){
        Serial.println("could not create ldr object");
        exit(1);
    }

    err = ldr_init(ldr_obj);

    if(err != ERROR_OK){
        Serial.println("could not init ldr object");
        exit(1);
    }

    //create led object

    led_config_t led_driver= {.led_pin_number = 2};

    led_obj = led_create(&led_driver);
    if(!led_obj){
        Serial.println("could not create led object");
        exit(1);
    }

    err = led_init(led_obj);

    if(err != ERROR_OK){
        Serial.println("could not init ldr object");
        exit(1);
    }

       // Create LM75 object
    lm75 = lm75_create(&lm75Config);

    // Initialize LM75 in comparator mode
    error_type_t initResult = lm75_init_comparator_mode(lm75);
    if (initResult != ERROR_OK)
    {
        Serial.println("Failed to initialize LM75.");
        exit(1);
    }

    // create voltage object
    voltage_config_t volt_driver = {.voltage_pin_number = 4};
    volt_obj = voltage_create(&volt_driver);
    if (!volt_obj)
    {
      Serial.println("could not create voltage object");
      exit(1);
    }

    err = voltage_init(volt_obj);
     if(err != ERROR_OK){
        Serial.println("could not init voltage object");
        exit(1);
    }

    //  create dust sensor object

    dust_config_t dust_driver = {.vout_pin_number = 15, .led_pin_number = 13 };
    dust_obj = dust_create(&dust_driver);
    if (!volt_obj)
    {
      Serial.println("could not create dust object");
      exit(1);

    }
    
    

    // Uncomment the following line if using interrupt mode
    lm75_init_interrupt_mode(lm75, lm75InterruptHandler);
    //sim800 configuration
      // sim800_config_t sim_config = {.serial_number = 2, .baud_rate = 9600};
      // sim800_obj = sim800_create(&sim_config);
      // err = sim800_init(sim800_obj);
      // if(err != ERROR_OK){
      //   Serial.println("Failed to initialize SIM800.");
      //   exit(1);
      // }
      // err = sim800_connect(sim800_obj);
      // if(err != ERROR_OK){
      //   Serial.println("Failed to connect SIM800.");
      //   exit(1);
      // }
      // String phone_number = "+2347068666080";
      // String message = "Hello world!";
      // err = sim800_send_sms(sim800_obj,phone_number.c_str(), message.c_str());
      // if(err != ERROR_OK){
      //   Serial.println("Failed to send sms SIM800.");
      //   exit(1);
      // }

}

void loop() {
   Blynk.run();
   timer.run();
  // put your main code here, to run repeatedly:
  // ldr_read(ldr_obj);
//    float temperature = lm75_read(lm75);
//     Serial.print("Temperature: ");
//     Serial.print(temperature);
//     Serial.println(" °C");

//     delay(1000);
// error_type_t err;
// err = led_set(led_obj,LED_ON);
// if(err != ERROR_OK){
//     Serial.println("cant set led");
//     exit(1);
// }
// delay(1000);
// err = led_set(led_obj,LED_OFF);
// if(err != ERROR_OK){
//     Serial.println("cant set led");
//     exit(1);
// }
// delay(1000);
}


