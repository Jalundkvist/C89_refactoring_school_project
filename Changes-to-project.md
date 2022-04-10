# Projekt-1-ELA21
Inlämningsuppgift för inbyggda system 1.

Dokumentation av ändringar i projektet

1. Updated toolchain from GNU C89 -> C99.
2. Troubleshooting serial functions. 
  2.1 Baudrate set to 115220 KBPS, should be 9600 KBPS. - Fixed.
  2.2 Macro WAIT_FOR_PREVIOUS_TRANSMISSION_TO_FINISH, 
      removed the use of BIT macro.
  2.3 Macro ENABLE_SERIAL_TRANSMISSION activated wrong bit. 
      Is now TXEN0 (Transmitter Enable 0). - Fixed
  2.4 Macro SIZE changed from 5 -> 100. 
      Increases the capacity of string from "4 + \0" to "99 + \0". Fixed
      
      Serial_print now working, integer and unsigned not working.
  2.5 Macro SET_TRANSMISSION_SIZE added to set size of transmission.
  
