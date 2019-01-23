#include <Arduino.h>
#include <serialcmds.h>
#include <narrowbandcore.h>
#include <narrowband.h>
// Select serial object for modem communication
// Please look at the specs of your board and locate
// the Serial object with a pin mapping compatible to
// the shield you're using.

HardwareSerial& modem_serial = Serial1;

// Instantiate command adapter as the connection via serial
Narrowband::ArduinoSerialCommandAdapter ca(modem_serial);

// Driver class
Narrowband::NarrowbandCore nbc(ca);

Narrowband::FunctionConfig cfg;

// instantiate NB object.
Narrowband::Narrowband nb(nbc, cfg);

void setup() {
    // connection speed to your terminal (e.g. via USB)
    Serial.begin(115200);
    Serial.println("Hit [ENTER] or wait 10s ");

    Serial.setTimeout(10000);
    Serial.readStringUntil('\n');
    // Begin modem serial communication with correct speed (check shield specs!)
    // TEKMODUL BC68-DEVKIT 9600,echo
    modem_serial.begin(9600);
    delay(3000);
}
void loop() {
  if (modem_serial.available()) {
      int c = modem_serial.read();
      Serial.write(c);
  }

  if ( Serial.available()) {
      int c = Serial.read();

      // echo
      Serial.write(c);
      modem_serial.write(c);
  }
}
