/*
 * 
  THIS EXAMPLE IT IS COMPATIBLE WITH THE SHIELD ARDUWIRE 
  DEVELOPMENT BY ERIDEVICES MAR/07/20
  REVISED NOV/19/2020
  THIS SKETCH RETURN THE NUMBER OF THE TEST POINT. PUT SOME NODE TO GND,
  AND THE ARDUWIRE IT IS WILL RETURN THE NUMBER
  
*/

//IMPORT THE LIBRARY arduwire.h
#include <arduwire.h>

//CREATE THE OBJECT
arduwire Arduwire(32);

//CREATE ONE VARIABLE TO STORE THE TEST POINT RETURN FOR THE ARDUWIRE
uint8_t probe_pin = 0;


void setup() {
  //INITIAL THE SERIAL PORT TO 9600 BAUD RATE
  Serial.begin(9600);
  //INITIAL THE ARDUWIRE INSTANCE
  Arduwire.init();
  

}

void loop() {

Arduwire.scanner();//READ THE SCANNER AND RETURN 1 TO 32
//VERIFY THE TEST POINT
if (probe_pin != Arduwire.get_last_pin()){
    probe_pin = Arduwire.get_last_pin();
    Serial.println(probe_pin);
}

}
