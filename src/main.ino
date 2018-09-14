#include <Arduino.h>
#include <serialcmds.h>
#include <narrowbandcore.h>
#include <narrowband.h>
#include "Seeed_BME280.h"


// Select serial object for modem communication
// Please look at the specs of your board and locate
// the Serial object with a pin mapping compatible to
// the shield you're using.
BME280 bme280;
HardwareSerial& modem_serial = Serial1;

// Instantiate command adapter as the connection via serial
Narrowband::ArduinoSerialCommandAdapter ca(modem_serial);

// Driver class
Narrowband::NarrowbandCore nbc(ca);

void setup() {
    // connection speed to your terminal (e.g. via USB)
    Serial.begin(115200);
    delay(2000);
    Serial.println("Hit [ENTER] or wait 10s ");

    Serial.setTimeout(10000);
    Serial.readStringUntil('\n');
    if(!bme280.init()) {
        Serial.println("Error initializing BME280 Sensor.");
      } else {
          Serial.println("BME280 is initialized.");
      }
    // Begin modem serial communication with correct speed (check shield specs!)
    // TEKMODUL BC68-DEVKIT         9600,echo
    modem_serial.begin(9600);
    delay(3000);
}

void loop() {
  // instantiate NB object. true==reboot module
  Narrowband::Narrowband nb(nbc, true);

  // Quectel: wait at least 10 seconds
  delay(10*1000);

  // begin session
  nb.begin();
  if (!nb) {
      Serial.println("Error initializing NB module.");
      return;
  } else {
      Serial.println("NB module Initialized.");
  }
  nb.getCore().setReportError(true);

  // try to attach within 15 secs
  if (nb.attach(15000)) {
      Serial.println("Attached.");

      String ip;
      if (nb.getCore().getPDPAddress(ip)) {
          Serial.println(ip);
      }
      delay(5000);

      // request something. Put in your IP address and
      // request data.
      String req("hello world"), resp;
      /*if ( nb.sendReceiveUDP("192.168.0.1", 9876,req,resp)) {
          Serial.println(resp);
      } */

      String msg("89754C84BC46918C?Wtemp=");
      // get temperature from bme280 and send it via UDP
      msg += bme280.getTemperature();
      nb.sendUDP("40.114.225.189", 9876, msg);
      delay(2000);
      String msg2("89754C84BC46918C?Whumd=");
      // get humidity from bme280 and send it via UDP
      msg2 += bme280.getHumidity();
      nb.sendUDP("40.114.225.189", 9876, msg2);
      delay(2000);
      String msg3("89754C84BC46918C?Wpres=");
      // get pressure from bme280 and send it via UDP
      msg3 += bme280.getPressure();
      nb.sendUDP("40.114.225.189", 9876, msg3);
      delay(2000);

      nb.detach();

  } else {
      Serial.println("unable to attach to NB network.");
  }

      nb.end();
      delay(1000*60*14);
      Serial.println("Session is ended.");
      //Every 15 minutes the data will be sent
}
