#include "Arduino.h"
#include "arduwire.h"


		arduwire::arduwire(uint8_t test_points = 32)
		: num_total_tp(test_points)
		{
			
								
			if(num_total_tp <= 32){
				enbl_pins = 2;//SET THE NUMBER OF THE ENABLE PINS 2 TO 4
			}
			else{
				enbl_pins = 4;//
			}
			
						
		}
		
		void arduwire::led_pass_on(){
			PORTD |= (1<<7);//TURN ON THE PASS LED
		}
		
		byte arduwire::calculate_crc8(const byte *data, size_t dataLength){
								
			byte crc = 0x00;
			
			while (dataLength--)
					
			{
				
				byte extract = *data++;
				
				for (byte tempI = 8; tempI; tempI--)
					
					{
						
						byte sum = (crc ^ extract) & 0x01;
						crc >>= 1;
						
						if (sum)
							
							
						{						
							crc ^= 0x8C;
						}					
						extract >>= 1;
			}
		  }
		  
		  return crc;
		  
		}
		
		uint8_t arduwire::get_crc8_pina(){
			return calculate_crc8(pina_learned, sizeof(pina_learned));
		}
		
		uint8_t arduwire::get_crc8_pinb(){
			return calculate_crc8(pinb_learned, sizeof(pinb_learned));
		}
		
		uint8_t arduwire::get_crc8_conn(){
			return calculate_crc8(get_connections_learned(), 1);
		}
		
		void arduwire::set_cbl_passed(bool state){
			cbl_passed = state;		
		}
		
		bool arduwire::get_cbl_passed(){
			return cbl_passed;
		}
		
		void arduwire::led_pass_off(){		
			PORTD &=~(1<<7);
		}
		
		void arduwire::led_fail_on(){
			PORTB |= (1<<5);//LED FAIL ON
		}
		
		void arduwire::led_fail_off(){
			PORTB &=~ (1<<5);//LED FAIL OFF
		}
		
		void arduwire::led_pass_flip(){
			PORTD |= (1<<7);
			PORTB &=~ (1<<5);
		}
		
		void arduwire::led_fail_flip(){
			PORTB |= (1<<5);
			PORTD &=~(1<<7);
		}
		
		void arduwire::all_led_off(){
			PORTD &=~(1<<7);
			PORTB &=~ (1<<5);
		}
		
		void arduwire::buzzer_on(){
			tone(6, 1000,500);
			delay(200);
			tone(6, 800,300);			
		}
		
		void arduwire::beep(){									
			tone(6,1000,500);							
		}
		
		void arduwire::buzzer_cbl_passed(){		 		 
			tone(6,1000,200);
			delay(150);
			tone(6,800,200);
			delay(300);			 
		}
		
		void arduwire::buzzer_cbl_failed(){
			 tone(6,1000,200);
			 delay(100);
			 tone(6,500,200);
			 delay(100);
		}
		
		void arduwire::beep_probe(){
			tone(6, 1000,100);
		}
		
		void arduwire::buzzer_off(){
			noTone(6);
		}
		
		uint8_t arduwire::get_total_tp(){
			return num_total_tp;
		}
		
		uint8_t arduwire::scanner(){
			
			//CLEAR THE BUFFER DRAINS
			clear_buffer_drains();
			
			for(uint8_t i = 0; i < enbl_pins; i++){			
				set_enable_4067(i);						
				for(uint8_t u=0; u < 16; u++){
					set_mux(u);//SET MUX / DEMUX
					delayMicroseconds(200);
					//////////////////////////
					if((PINB &(1<<PINB4))==0){
						delayMicroseconds(200);
						if(u+1+(i*16)!=last_pin){
							beep_probe();
							last_pin = u+1 + (i*16);
							return last_pin;
							delay(200);							
						}
						return;
					}		
				}		
			}			
			if(last_pin!=0){
				last_pin = 0; 
				return last_pin;				
				delay(200);				
			}																
		}
		
		void arduwire::set_last_pin(uint8_t num){
			last_pin = num;
		}
		
		uint8_t arduwire::get_last_pin(){
			return last_pin;
		}
		
		void arduwire::clear_buffer_drains(){
			PORTB &= ~(1<<3);	
			PORTB |= (1<<3);	
		}
		
		void arduwire::set_drains_outputs(bool status_bit, int clk_pulse1){
			  //THIS FUNCTION IT IS FOR THE DRAIN TPCI595
			if(status_bit!=0){ 
				//PB0--> SERIAL_DATA_IN_DR SET BIT TO 1
				PORTB |= (1<<0);	
			}	
			else{				
				//PB0 --> SERIAL_DATA_IN_DR SET BIT TO 0
				PORTB &= ~(1<<0);	
			}
			//PULSES FOR TWO
			int clk1 = clk_pulse1 * 2;
			  
			do{
				//CLOCK ENABLE FLIP FLOP
				//PB1 --> CLOCK_DR
				PORTB ^= (1<<1);//PULSES	
				clk1--;		
			}while(clk1);//VERIFY THE NUMBER
			  
			//RELEASE BUFFER TO THE OUTPUT PINS
			release_latch_drains();	  
		}
		
		void arduwire::release_latch_drains(){
			//PB2-> LATCH_DR HIGH STATE
			PORTB |= (1<<2);
			//RELEASE LATCH LOW STATE
			PORTB &= ~(1<<2);
		}
		
		void arduwire::high_impedance_mux(){		
			PORTC |= (1<<5);
			PORTC |= (1<<4);
			PORTC |= (1<<3);
			PORTC |= (1<<2);	
		}
		
		void arduwire::set_enable_4067(uint8_t pin){	
			switch(pin){
				case 0:
					//REV 2 BOARD
					PORTC &=~ (1<<0);
					PORTC |= (1<<1);
					PORTC |= (1<<2);
					PORTC |= (1<<3);
				
					//REV 1 BOARD
					/* PORTC &=~ (1<<5);//A5
					PORTC |= (1<<4);//A4
					PORTC |= (1<<3);//A3
					PORTC |= (1<<2);//A2 */
					break;
				case 1:
					//REV2 BOARD
					PORTC |= (1<<0);
					PORTC &=~ (1<<1);
					PORTC |= (1<<2);
					PORTC |= (1<<3);
					
					//REV 1 BOARD
					/* PORTC |=  (1<<5);//A5
					PORTC &=~ (1<<4);//A4
					PORTC |= (1<<3);//A3
					PORTC |= (1<<2);//A2 */
					break;
				case 2:
					//REV2 BOARD
					PORTC |= (1<<0);
					PORTC |= (1<<1);
					PORTC &=~(1<<2);
					PORTC |= (1<<3);
					
					//REV1 BOARD
					/* PORTC |= (1<<5);//A5
					PORTC |= (1<<4);//A4
					PORTC &=~ (1<<3);//A3
					PORTC |= (1<<2);//A2 */
					break;
				case 3:
					//REV2 BOARD
					PORTC |= (1<<0);
					PORTC |= (1<<1);
					PORTC |= (1<<2);
					PORTC &=~ (1<<3);
					
					//REV1 BOARD
					/* PORTC |= (1<<5);//A5
					PORTC |= (1<<4);//A4
					PORTC |= (1<<3);//A3
					PORTC &=~ (1<<2);//A2 */
					break;									
			}				
		}
		
		void arduwire::set_mux(uint8_t x){			
			PORTD = PORTD & 0xC3 | (x << 2);						
		}
		
		bool arduwire::learn(){
			uint8_t counter = 0;
			uint8_t net_counter = 0;
			cable_learned = false;
			num_connections_learned = 0;
			memset(pina_learned, 0, sizeof(pina_learned));//CLEAR THE VARIABLE
			memset(pinb_learned, 0, sizeof(pinb_learned));
			clear_buffer_drains();//clear the buffer
			set_drains_outputs(1,num_total_tp);//initial the drains (11111111)
			set_drains_outputs(0,1);//01111111	
			for(uint8_t i = 0; i < enbl_pins; i++){
				set_enable_4067(i);//ENABLE PINS
				for(uint8_t u = 0; u < 16; u++){
					set_mux(u);//SET MUX DEMUX
					delayMicroseconds(10);
					if((PINB &(1<<PINB4))==0){// I FOUNDED ONE CONNECTION
						counter++;//increment variable
						num_connections_learned++;			
						pina_learned[counter-1] = u+1 + (i*16);//save the number of the pina in the array						
						clear_buffer_drains();//clear the drains
						set_drains_outputs(1,1);
						set_drains_outputs(0, u+1+(i*16));
						do{
							net_counter++;
							delayMicroseconds(10);
							if((PINB &(1<<PINB4))==0){														
								pinb_learned[counter-1] = net_counter + u+1+(i*16);							
								break;								
							}
							set_drains_outputs(0,1);
						}while(net_counter + u+1+(i*16)!=num_total_tp);
						clear_buffer_drains();//CLEAR ALL DRAINS
						set_drains_outputs(1,num_total_tp);//SET ALL DRAINS OUTPUTS HIGH STATE
						set_drains_outputs(0,(u+1+(i*16))+1);
						net_counter = 0;
					}
					else{
						//SET THE DRAIN BUFFER (00111111)
						set_drains_outputs(0,1);
					}						
				}					
			}	
			if(num_connections_learned!=0){
				cable_learned = true;
				return 1;				
			}
			else{
				return 0;
			}	
		}
		
		bool arduwire::cable_pres(){
			uint8_t net_counter = 0;
			clear_buffer_drains();//clear the buffer
			set_drains_outputs(1,num_total_tp);//initial the drains
			set_drains_outputs(0,1);
			for(uint8_t i = 0; i < enbl_pins; i++){	
				set_enable_4067(i);//SET MUX DEMUX
				for(uint8_t u = 0; u < 16; u++){
					set_mux(u);
					delayMicroseconds(10);
					if((PINB &(1<<PINB4))==0){// I FOUNDED ONE CONNECTION
						return 1;
						break;//BREAK EXIT FOR THE LOOP								
					}
					else{
						net_counter++;//INCREMENT THE VARIABLE
						set_drains_outputs(0,1);
					}			
				}		
			}
			if(net_counter == num_total_tp){
				//Not connected Anything
				return 0;
			}
		}
		
		void arduwire::init(){	
			//////////////PORTD SETTINGS///////////////////
			//PD0 -> RX
			//PD1 -> TX
			//PD2 -> S0
			//PD3 -> S1
			//PD4 -> S2
			//PD5 -> S3
			//PD6 -> BUZZER(ALARM)
			//PD7 -> LED_PASS
			DDRD = 0b11111100;
			/////////////////////////////////////////////
			//////////////PORTB SETTINGS/////////////////	
			//PB0 -> DATA_IN 
			//PB1 -> CLK_DR
			//PB2 -> LATCH_DR
			//PB3 -> CLR_DR
			//PB4 -> COMM (IT IS IMPORTANT, ADD INPUT_PULLUP EXTERNAL 2.2k to 10k).
			//PB5 -> LED_FAIL
			DDRB |= (1<<0)|(1<<1)|(1<<2)|(1<<3)|(1<<5);
			PORTB |= (1<<3);
			DDRB &=~ (1<<4);//INPUT
			///////////////////////////////////////////
			///////////PORTC SETTINGS REV1 BOARD/////////////////
			//PC0 -> LEARN_BUTTON
			//PC1 -> LED_LEARN
			//PC2 -> ENABLE 4
			//PC3 -> ENABLE 3
			//PC4 -> ENABLE 2
			//PC5 -> ENABLE 1
			//REV1 BOARD
			//DDRC &=~(1<<0);//LEARN BUTTON INPUT
			//PORTC |= (1<<0);//LEARN BUTTON SET OT INPUT_PULLUP
			///////////////////////////////////
			//REV1 BOARD
			//DDRC |= (1<<1)|(1<<2)|(1<<3)|(1<<4)|(1<<5);
			
			//REV2 BOARD
			DDRC |= (1<<0)|(1<<1)|(1<<2)|(1<<3);
			
			
		}
		
		void arduwire::led_on_learn(){
			PORTC |= (1<<1);
		}
		
		void arduwire::led_off_learn(){
			PORTC &=~(1<<1);
		}
		
		uint8_t arduwire::get_pina_learned(uint8_t pin){		
			return pina_learned[pin];//return the value of the array side a	
		}
		
		uint8_t arduwire::get_pinb_learned(uint8_t pinx){
			return pinb_learned[pinx];
		}
		
		uint8_t arduwire::get_connections_learned(){
			return num_connections_learned;
		}
		
		bool arduwire::get_learned_cable(){
			return cable_learned;
		}
		
		uint8_t arduwire::get_connections_tested(){
			return num_connections_tested;
		}
		
		bool arduwire::test(){	
			uint8_t counter = 0;
			uint8_t net_counter = 0;
			num_connections_tested = 0;
			memset(pina_tested, 0, sizeof(pina_tested));//CLEAR THE VARIABLE
			memset(pinb_tested, 0, sizeof(pinb_tested));
			clear_buffer_drains();//clear the buffer
			set_drains_outputs(1,num_total_tp);//initial the drains
			set_drains_outputs(0,1);	
			for(uint8_t i = 0; i < enbl_pins; i++){	
				set_enable_4067(i);//SET MUX DEMUX	
				for(uint8_t u = 0; u < 16; u++){
					set_mux(u);
					delayMicroseconds(10);
					if((PINB &(1<<PINB4))==0){
						// I FOUNDED ONE CONNECTION
						counter++;//increment variable						
						num_connections_tested++;
						pina_tested[counter-1] = u+1 + (i*16);//save the number of the pina in the array						
						clear_buffer_drains();//clear the drains
						set_drains_outputs(1,1);
						set_drains_outputs(0, u+1+(i*16));	
						do{	
							net_counter++;
							delayMicroseconds(10);
							if((PINB &(1<<PINB4))==0){		
								pinb_tested[counter-1] = net_counter + u+1+(i*16);							
								break;					
							}	
							set_drains_outputs(0,1);		
						}while(net_counter + u+1+(i*16)!=num_total_tp);	
						clear_buffer_drains();
						set_drains_outputs(1,num_total_tp);
						set_drains_outputs(0,(u+1+(i*16))+1);
						net_counter = 0;							
					}
					else{
						set_drains_outputs(0,1);
					}						
				}					
			}
			if(memcmp(pina_tested, pina_learned, sizeof(pina_learned))== 0 && memcmp(pinb_tested, pinb_learned, sizeof(pinb_learned))==0){
				return 1;//return 1  all connections it is the same
			}
			else{
				return 0;//return 0, it is founded errors
			}	
		}
		
		uint8_t arduwire::selftest(){
			uint8_t self_t_counter = 0;
			uint8_t counter_board = 0;
			//CLEAR ALL BITS AND DRAINS
			clear_buffer_drains();
			set_drains_outputs(1,1);
			for(uint8_t i = 0; i < 2 + counter_board; i++){
				set_enable_4067(i);//ENABLE PINS
				for(uint8_t u = 0; u < 16; u++){
					//SET THE MUX SWITCHES ICS
					set_mux(u);
					delayMicroseconds(100);
					if((PINB &(1<<PINB4))==0){//HIGH
						//SET THE DRAINS OUTPUTS 1 BIT RIGHT SIDE
						set_drains_outputs(0,1);
						delayMicroseconds(100);
						if((PINB &(1<<PINB4))!=0){//LOW
							self_t_counter++;
							if(self_t_counter==32){
								counter_board++;
								clear_buffer_drains();	
							}
						}
					}		
				}
						
			}
			return self_t_counter;			
		}