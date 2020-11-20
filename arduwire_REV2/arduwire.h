#ifndef arduwire_h
#define arduwire_h


#include "Arduino.h"

class arduwire {
	
	public:
		
		arduwire(uint8_t test_points);
		void led_pass_on();					
		void set_cbl_passed(bool state);						
		bool get_cbl_passed();							
		void led_pass_off();								
		void led_fail_on();					
		void led_fail_off();					
		void led_pass_flip();				
		void led_fail_flip();					
		void all_led_off();		
		void buzzer_on();							
		void buzzer_cbl_passed();						
		void buzzer_cbl_failed();			 	
		void beep_probe();				
		void buzzer_off();													
		uint8_t scanner();																												 			  								
		void high_impedance_mux();																							
		void set_mux(uint8_t x);													
		bool learn();					
		void init();												
		void led_on_learn();					
		void led_off_learn();	
		uint8_t get_pina_learned(uint8_t pin);										
		uint8_t get_pinb_learned(uint8_t pinx);												
		bool get_learned_cable();		
		uint8_t get_connections_tested();					
		bool test();
		void set_last_pin(uint8_t num);
		uint8_t get_last_pin();
		byte calculate_crc8(const byte *data, size_t dataLength);
		uint8_t get_crc8_pina();
		uint8_t get_crc8_pinb();
		uint8_t get_crc8_conn();
		bool cable_pres();
		void beep();
		uint8_t get_connections_learned();
		uint8_t get_total_tp();
		uint8_t selftest();
			
												
	private:
	
		void clear_buffer_drains();	
		void set_enable_4067(uint8_t pin);
		void set_drains_outputs(bool status_bit, int clk_pulse1);
		void release_latch_drains();	
		uint8_t num_total_tp;
		uint8_t last_pin = 0;
		uint8_t enbl_pins = 0;
		uint8_t pina_learned[32];
		uint8_t pinb_learned[32];
		uint8_t pina_tested[32];
		uint8_t pinb_tested[32];
		uint8_t num_total_connections_arduwire;
		uint8_t num_connections_tested = 0;
		uint8_t num_connections_learned = 0;
		bool cbl_passed = false;
		bool cable_learned = false;					
};

#endif