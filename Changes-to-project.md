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
    
  
3. enum to typedef enum -> to avoid repeating use of word "enum". Removed the use of word "enum" in all files except for typedef enum.
	3.1 enum bool fixed to typedef enum
	3.2 enum IO_port fixed to typedef enum
  	3.3 enum TimerSelection fixed to typedef enum
  	3.4 enum DataType fixed to typedef enum

4. struct to typedef struct -> to avoid repeating use of word "struct". Removed the use of word "struct" in all files except for initial declaration of struct.
	3.1 TempSensor
	3.2 Button
  	3.3 Led
	
	
		Led changes.
5. Troubleshooting LED struct.
	5.1 Updated struct
	5.2 Void functions made static and struct uses function pointers instead.
	5.3 removed pointers "self"
	5.4 BIT-macro removed
