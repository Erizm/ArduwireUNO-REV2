#include <arduwire.h>

arduwire ArduwireUNO(32);


void setup() {
  ArduwireUNO.init();

}

void loop() {
  ArduwireUNO.led_pass_on();
  delay(1000);
  ArduwireUNO.led_pass_off();
  delay(1000);

}
