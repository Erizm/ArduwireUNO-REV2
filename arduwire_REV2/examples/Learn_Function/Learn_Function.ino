
/*
	THIS EXAMPLE IT IS COMPATIBLE WITH THE SHIELD ARDUWIRE 
	DEVELOPMENT BY ERIDEVICES MAR/07/20
	REVISE NOV/19/2020
	
*/

//IMPORT THE LIBRARY arduwire.h
#include <arduwire.h>

//CREATE THE OBJECT(MAX TEST POINTS)
arduwire ArduwireUNO(32);


void setup() {
	//INITIAL THE SERIAL PORT TO 9600 BAUD RATE
  Serial.begin(9600);
  //INITIAL THE ARDUWIRE INSTANCE
  ArduwireUNO.init();
  
  Serial.println(F("Verifying Connections............\r"));
  
  do{
	  //THIS FUNCTION VERIFY ANY CONNECTION 
	  ArduwireUNO.learn();
	  
	  
  }while(ArduwireUNO.get_learned_cable()!=true);
  
  //NOW IT IS FOUNDED THE CONNECTIONS
  Serial.print(F("Number of Connections Founded: "));
  //THIS FUNCTION RETURN THE NUMBER OF CONNECTIONS FOUNDED
  Serial.println(ArduwireUNO.get_connections_learned());
  //SMALL ALARM BEEP
  ArduwireUNO.beep_probe();
  //VARIABLE COUNTER
  uint8_t counter;
  Serial.println(F("WIRELIST LEARNED:"));
  for(uint8_t i = 0; i < ArduwireUNO.get_connections_learned(); i++){
	  //INCREMENT
	  counter++;
	  Serial.print(counter);
	  Serial.print(F(" "));
	  Serial.print(ArduwireUNO.get_pina_learned(i));
	  Serial.print(F(","));
	  Serial.println(ArduwireUNO.get_pinb_learned(i));
  }
  
 
}

void loop() {
  // put your main code here, to run repeatedly:

}
