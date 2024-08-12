#include <Arduino.h>
#include <WiFiClient.h>
#include "wifi-driver.h"
#include "webServer.h"
#include "lm75-driver.h"
#include <Wire.h>
#include <ArduinoJson.h>
#include <ldr-driver.h>
#include <led-driver.h>
#include <voltage-driver.h>
#include <dust_sensor.h>
#include <relay-driver.h>
#include <sim800.h>


#define BLYNK_TEMPLATE_ID "TMPL2JIEb2LsZ"
#define BLYNK_TEMPLATE_NAME "Solar Street Light IoT Monitoring Device"
#define BLYNK_AUTH_TOKEN "s4pwewhOYhUYa_RdyXQmhCTNedPOYWkB"
#include <BlynkSimpleEsp32.h>

char auth[] = BLYNK_AUTH_TOKEN;

char* ssid = "YOUNGCRYPT";
char* wpassword = "badewa12345";


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
relay_t* relay_obj;
sim800_t* sim800_obj;
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
    
//     err = ldr_read(ldr_obj,&ldr_val);
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
   float temp;
   float volt;
   float dust;
   int ldr_val;

void sendData(){
  
   error_type_t ldr_err = ldr_read(ldr_obj, &ldr_val);
   Blynk.virtualWrite(V1, ldr_err);
   if (ldr_err = ERROR_OK)
   {
      Serial.println("ldr return error ok");
   }

   error_type_t volt_err = voltage_read(volt_obj, &volt);
   Blynk.virtualWrite(V2, volt_err);
    if (volt_err = ERROR_OK)
   {
      Serial.println("volt return error ok");
   }

   error_type_t lm75_err = lm75_read(lm75, &temp);
    Blynk.virtualWrite(V3, lm75_err);
    if (lm75_err = ERROR_OK)
   {
      Serial.println("lm75 return error ok");
   }

   error_type_t dust_err = dust_read(dust_obj,&dust);
    Blynk.virtualWrite(V4, dust_err);
    if (dust_err = ERROR_OK)
   {
      Serial.println("dust return error ok");
   }


}

void setup() {
   Serial.begin(9600);
   analogReadResolution(12); // Set ADC resolution to 12 bits
   analogSetAttenuation(ADC_0db);
  
   error_type_t err;
   wifi_configuration_t wifi_config = { .ssid = ssid, .password=wpassword};
   wifi_t* wifi_obj = wifi_create(&wifi_config);

  if (wifi_init(wifi_obj) != ERROR_OK) {
        Serial.println("WiFi initialization failed!");
        exit(1);
  }
  if(wifi_connect(wifi_obj) != ERROR_OK){
        Serial.println("WiFi connection failed!");
        exit(1);  
  }
   Blynk.begin(BLYNK_AUTH_TOKEN, ssid, wpassword);
  timer.setInterval(2000L, sendData);

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
    ldr_config_t ldr_driver= {.ldr_pin_number = 33};

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

    led_config_t led_driver= {.led_pin_number = 34};

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
    voltage_config_t volt_driver = {.voltage_pin_number = 34 };
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

    dust_config_t dust_driver = {.vout_pin_number = 36, .led_pin_number = 35 };
    dust_obj = dust_create(&dust_driver);
    if (!volt_obj)
    {
      Serial.println("could not create dust object");
      exit(1);

    }

    // relay object
    relay_config_t relay_driver = {.relay_pin_number = 32};
    relay_obj = relay_create(&relay_driver);
    if (!relay_obj)
    {
      Serial.println("could not create relay object");
      exit(1);

    }
    
    // Uncomment the following line if using interrupt mode
    lm75_init_interrupt_mode(lm75, lm75InterruptHandler);
    //sim800 configuration
      sim800_config_t sim_config = {.serial_number = 2, .baud_rate = 9600};
      sim800_obj = sim800_create(&sim_config);
      err = sim800_init(sim800_obj);
      if(err != ERROR_OK){
        Serial.println("Failed to initialize SIM800.");
        exit(1);
      }
      err = sim800_connect(sim800_obj);
      if(err != ERROR_OK){
        Serial.println("Failed to connect SIM800.");
        exit(1);
      }
       if(err != ERROR_OK){
        Serial.println("Failed to send sms SIM800.");
        exit(1);
      }
      String phone_number = "+2347068666080";
      //ldr fault message
      if (ldr_val < 0 || ldr_val > 1023)
      {
          String message = "LDR value is out of range!, LDR is faulty";
          err = sim800_send_sms(sim800_obj,phone_number.c_str(), message.c_str());
      }
      //Battary fault message
       if (volt < 11.0)
      {
          String message = "Battery reading  is below voltage reading!, Battery is faulty";
          err = sim800_send_sms(sim800_obj,phone_number.c_str(), message.c_str());
      }
     

}

void loop() {
   Blynk.run();
   sendData();
   timer.run();
  // put your main code here, to run repeatedly:
  ldr_read(ldr_obj, &ldr_val);
   float temperature = lm75_read(lm75, &temp);

error_type_t err;
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

 if (ldr_val > 512) { // Adjust the value 512 as per your requirement
        err = led_set(led_obj, LED_ON);
        if (err != ERROR_OK) {
            Serial.println("Failed to turn on LED");
        }
    } else {
        err = led_set(led_obj, LED_OFF);
        if (err != ERROR_OK) {
            Serial.println("Failed to turn off LED");
        }
    }

    delay(1000);

err = set_relay(relay_obj,relay_OFF);
if (err != ERROR_OK)
{
  Serial.println("cant set realay");
  exit(1);
}
delay(300);

err= set_relay(relay_obj,relay_ON);
if (err != ERROR_OK)
{
  Serial.println("cant set realay");
  exit(1);
}
delay(300);





}


