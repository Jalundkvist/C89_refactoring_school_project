
#ifndef BYTE_H_
#define BYTE_H_

/******************************************************************************
* Strukten Bits används för att enkelt komma åt enskilda bitar i ett 8-bitars
* register. Via användning av unionen Byte kan användaren läsa och skriva
* 8-bitars värden och sedan läsa av enskilda bitar en efter en.
* Instansvariabler bit0 - bit 7 används för att lagra varje bit i ett givet
* register, där datatypen unsigned char används. Eftersom enbart en bit
* behöver allokeras per variabel, så skrivs : 1 efter variabeldeklarationen.
* Bitarna deklareras i ordningen 0 - 7, då minne allokeras som så i C att
* minst signifikant bit placeras först (tänk första index i en array).
******************************************************************************/
struct Bits
{
	unsigned char bit0 : 1; /* Bit 0 - Minst signifikant bit. */
	unsigned char bit1 : 1; /* Bit 1. */
	unsigned char bit2 : 1; /* Bit 2. */
	unsigned char bit3 : 1; /* Bit 3. */
	unsigned char bit4 : 1; /* Bit 4. */
	unsigned char bit5 : 1; /* Bit 5. */
	unsigned char bit6 : 1; /* Bit 6. */
	unsigned char bit7 : 1; /* Bit 7 - Mest signifikant bit. */
};

/******************************************************************************
* Unionen Byte används för att läsa och skriva till ett 8-bitars register,
* samtidigt som enstaka bitar enkelt kan ett- eller nollställas samt läsas av.
* Eftersom medlemmarna allokerar samma minnesutrymme, så kan ett värde skrivas
* till den ena, så uppdateras den ändra efter detta värde. I detta fall gäller
* därmed att ett 8-bitars värde kan skrivas till medlemmen data, vilket medför
* att medlemmen bits, som utgör enskilda bitar, uppdateras till detta värde
* på binär form. Samtidigt kan en given bit ett- eller nollställas via de
* enskilda bitarna, vilket påverkar det 8-bitars värdet lagrat i data.
******************************************************************************/
union Byte
{
	unsigned char data; /* Data (en byte) för 8-bitars register. */
	struct Bits bits; /* Separata bitar för 8-bitars register. */
};

/* Externa funktioner: */
union Byte new_Byte(unsigned char data);
struct Bits new_Bits(void);

#endif /* BYTE_H_ */