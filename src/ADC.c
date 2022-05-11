// Inkluderar egen headerfil.
#include "ADC.h"

// Statiska funktioner:
static void print_temperature(TempSensor* self);
static uint16_t ADC_read(TempSensor* self);
static void init_ADC(void);

/******************************************************************************
* Funktionen new_TempSensor används för implementering av en temperatursensor 
* ansluten till någon av analoga pinnar A0 - A5 via ett objekt av strukten
* TempSensor. Ingående argument PIN utgör en pekare till aktuellt PIN-nummer. 
* Först skapas ett objekt av structen TempSensor döpt self. Därefter initieras
* objektets instansvariabler, följt av att det initierade objektet returneras.
*
* Inparameter	: uint8_t PIN.
* Returnerar	: initialized objekt of TempSensor.
******************************************************************************/
TempSensor new_TempSensor(uint8_t PIN)
{
	TempSensor self;
	self.PIN = PIN;
	self.rounded_temp = 0x00;
	init_ADC();
	self.print_temperature = print_temperature;
	self.ADC_read = ADC_read;
	return self;
} // End of function new_TempSensor.

/******************************************************************************
* Funktionen print_temperature används för att läsa av rumstemperaturen och 
* skriva till vår PC. Först läses temperatursensorn av och resultatet lagras 
* i konstanten ADC_result. Därefter beräknas motsvarande analoga inspänning Uin 
* med följande formel:
*							Uin = Vcc * ADC_result / ADC_MAX,
*
* där ADC_result är resultatet från senaste AD-omvandling (0 - 1023).
*
* Därefter beräknas motsvarande temperatur i grader Celcius via följande formel:
*
*							temp = 100 * Uin - 50,
*
* där Uin är den beräknade analoga inspänningen (0 - 5 V).
*
* Slutligen transmitteras den beräknade temperaturen till vår PC via anrop av
* funktionen serial_print_integer, som möjliggör sammansättning av text och 
* heltal samt seriell överföring. Därmed transmitteras textstycktet.
*
* Inparameter	: --
* Returnerar	: --
******************************************************************************/
static void print_temperature(TempSensor* self)
{
	const uint16_t ADC_result = self->ADC_read(self);
	const double Uin = VCC * (double)(ADC_result)/ADC_MAX;
	const double temp = (100 * Uin - 50);
	const int8_t rounded_temp = (int8_t)(temp + 0.5);
	self->rounded_temp = rounded_temp;
	serial_print_integer("Temperature: %d degrees Celcius\n", rounded_temp);
	return;
} // End of function print_temperature.

 /******************************************************************************
* Funktionen ADC_read används för att läsa av temperatursensorn och returnera 
* resultatet. Först väljs analog kanal för avläsning, vilket i detta fall är 
* temperatorsensorns PIN A1 (PORTC1), samtidigt som AD-omvandlaren sätts till 
* att matas med intern matningsspänning. Därefter aktiveras AD-omvandlaren och 
* startas med lägsta möjliga frekvens (125 kHz) för högsta möjliga precision. 
* Därefter väntar vi till att AD-omvandlingen är slutförd, vilket signaleras 
* via AD-omvandlarens interrupt-flagga ADIF (ADC Interrupt Flag), som då blir
* ettställd. För att sedan återställa ADIF inför nästa AD-omvandlaren så 
* ettställs denna, följt av att avläst resultat returneras vid återhoppet.
*
* Inparameter	: Object of struct TempSensor.
* Returnerar	: uint16_t.
 ******************************************************************************/
static uint16_t ADC_read(TempSensor* self)
{
	ADMUX = ((1 << REFS0)|self->PIN);
	ADCSRA = (1 << ADEN) | (1 << ADSC) | (1 << ADPS2) | (1 << ADPS1) | (1 << ADPS0);
	while ((ADCSRA & (1 << ADIF)) == 0) ;
	ADCSRA = (1 << ADIF);
	return ADC;
} // End of function ADC_read.

 /******************************************************************************
* Funktionen init_ADC används för att initiera AD-omvandlaren då första
* AD-omvandlingen kan vara felaktig.
* 
* Inparameter	: --
* Returnerar	: --
 ******************************************************************************/
static void init_ADC(void)
{
	ADMUX = (1 << REFS0);
	ADCSRA = (1 << ADEN) | (1 << ADSC) | (1 << ADPS2) | (1 << ADPS1) | (1 << ADPS0);
	while ((ADCSRA & (1 << ADIF)) == 0) ;
	ADCSRA = (1 << ADIF);
} // End of function init_ADC.