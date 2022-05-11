# Projekt-1-ELA21
Inlämningsuppgift för inbyggda system 1.

Dokumentation av ändringar i projektet

1. Updated toolchain from GNU C89 -> C99.

Serial file changes 

2.Troubleshooting serial functions. 
	
	2.1 Baudrate set to 115220 KBPS, should be 9600 KBPS. - Fixed.
	2.1.2 F_CPU typo -> Was 160Mhz, updated to 16Mhz.
	2.2 Macro WAIT_FOR_PREVIOUS_TRANSMISSION_TO_FINISH, 
      	removed the use of BIT macro.
	2.3 Macro ENABLE_SERIAL_TRANSMISSION activated wrong bit. 
      Is now TXEN0 (Transmitter Enable 0). - Fixed
	2.4 Macro SIZE changed from 5 -> 100. 
      Increases the capacity of string from "4 + \0" to "99 + \0". Fixed
      
	Serial_print now working, integer and unsigned not working.
      
	2.5 Macro SET_TRANSMISSION_SIZE added to set size of transmission.
	2.6 Improper use of sprintf function in print_integer and unsigned function.
      Functions now prints properly, first variable in sprintf should be empty string.
  
	
 	serial_print_integer & serial_print_unsigned now working.
	
	2.7 Check if pointers is necessary in serial, if not. - remove unecessary pointers.
  	2.7.1 write_byte no longer uses pointer. Uses one bit of data/send
    2.7.2 Macro update -> CARRIAGE_RETURN & END_TRANSMISSION
    2.7.3 Removed pointers from print_integer and print_unsigned. 
          updated long & unsigned long to int32_t & uint32_t.
    
		
	Serial is now updated now works as intended, review comments later.
    
  
3. enum to typedef enum -> to avoid repeating use of word "enum". 
	 Removed the use of word "enum" in all files except for typedef enum.

		3.1 enum bool fixed to typedef enum
		3.2 enum IO_port fixed to typedef enum
		3.3 enum TimerSelection fixed to typedef enum
		3.4 enum DataType fixed to typedef enum

4. struct to typedef struct -> to avoid repeating use of word "struct". 
	 Removed the use of word "struct" in all files except for initial declaration of struct.
	 
		4.1 TempSensor
		4.2 Button
		4.3 Led
	
5. Troubleshooting LED struct.

		5.1 Updated struct
		5.2 Void functions made static and struct uses function pointers instead.
		5.3 removed pointers "self"
		5.4 BIT-macro removed
	
		Led functions working
		
6. Troubleshooting Button struct.

		5.1 Updated struct
		5.2 Void functions made static and struct uses function pointers instead.
		5.3 removed pointers "self"
		5.4 BIT-macro removed
		5.5 subtract 8 from PIN if PIN is between 8-13 (PORTB).

		Buttons now working.

7. Troubleshooting Timer struct.

		7.1 Macro: DISABLE_TIMER1, incorrect. Cleared TIMSK0 instead of TIMSK1.
		7.2 Macro: INIT_TIMER1 was incorrect, missed setting CS10 AND WGM12.
		Used only WGM11 which is used for PWM-mode.
		7.3 BIT-macro removed.
		7.4 Updated struct
		7.5 Updated files using timer functions.

		Timer now working
		
8. Troubleshooting ADC for temperature measurment.

		8.1 Macro: SELECT_ANALOG_CHANNEL removed, did not include PIN for ADC_read.
		8.2 Functions made static, ADC struct now uses function pointers.
		8.3 ADDED: Function init_ADC to initialize the AD-converter
		8.4 BIT-macro removed.
		8.5 CHANGED: print_temperature now holds variables instead of ADC struct.
		As the variables are not needed after print and variables are freed from stack.

		Program is now completely updated from C89 to C99.
	**********************************************************************************	
	TODO: Improvements.

		-> Add vector struct
		-> Add dynamic timer struct
		-> Add button to switch between temperature units.
		-> Add 2 7-segment displays to visualize temperature.
		-> Add Watchdog Timer
	
9. Added Vector struct

		9.1 Made vector struct use function pointers.

10. Added DynamicTimer struct

		10.1 DynamicTimer uses Timer and Vector struct
		10.2 Updated interrupts and setup file to use DynamicTimer for timer1.
		10.3 Timing for temperature reading now updates dynamically depending on
		the average time between actuations of button on PIN 13.
	
11. Added struct for 7-segment displays.

		11.1 Displays uses PIN 2-8 for the segments and PIN 10-11 for displays.

12. Implemented Watchdog-timer

		12.1 Watchdogtimer now working and uses a 2S timer before it timeouts.
		12.2 WDT works as intended and resets in projects main-loop
		
