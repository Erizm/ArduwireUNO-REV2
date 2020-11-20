#include <arduwire.h>
//CREATE THE OBJECT WITH THE MAX TEST POINTS
arduwire Arduwire(32);

void setup() {
  //INITIAL THE SERIAL PORT
  Serial.begin(9600);
  //INITIAL THE ARDUWIRE UNO
  Arduwire.init();
  //THIS FUNCTION VERIFY ANY CONNECTION IN THE SHIELD
  //WAIT FOR SOME CONNECTION 
  while(Arduwire.cable_pres()!=true);

  Serial.println(("Connection FOunded!!!!!!!"));
  delay(3000);
  
  //LEARN FUNCTION RETURN BOOLEAN VALUE
  Arduwire.learn();
  
  if(Arduwire.get_learned_cable()==1){

    Serial.println(F("WIRELIT LEARNED:"));
    uint8_t counter_1 = 0;
    
    for(int i = 0; i < Arduwire.get_connections_learned(); i++){
      counter_1++;
      Serial.print(counter_1);
      Serial.print(F(" "));
      Serial.print(Arduwire.get_pina_learned(i));
      Serial.print(F(","));
      Serial.println(Arduwire.get_pinb_learned(i));
    }
    
    //ACTIVATE THE BUZZER
    Arduwire.buzzer_cbl_passed();
   
  }
  else{
    Serial.println(F("FAIL"));
  }


}

void loop() {
  //TEST ALL CONNECTION OF THE ARDUWIRE
  if(Arduwire.test()==1){
    
    //IF IT IS THE FIRST TIME TO PASS
    if(Arduwire.get_cbl_passed()==0){
      
      Arduwire.led_pass_flip();//TURN ON LED PASS & TURN OFF LED FAIL
      Arduwire.set_cbl_passed(1);//SET THE BOOLEAN TO TRUE    
      Arduwire.buzzer_cbl_passed();//ENABLE THE BUZZER
    }
        
  }
  
  else{
    
    //VERIFY SOMETHING CONNECTED    
    if(Arduwire.get_connections_tested()==0){
      //IT IS NOT CONNECT NOTHING
      Arduwire.set_cbl_passed(0);//SET VARIABLE TO TRUE
      Arduwire.all_led_off();//TURN OFF ALL LED'S
      Arduwire.buzzer_off();//TURN OF THE BUZZER
                
    }
    else{
      
      Arduwire.led_fail_flip();//TURN ON LED FAIL & TURN OFF THE LED PASS
      Arduwire.set_cbl_passed(0);//SET THE VARIABLE TO TRUE   
      Arduwire.buzzer_cbl_failed();//ENABLE ALARM "BUZZER"
    }
          
  }
}
