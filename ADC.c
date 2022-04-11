// Inkluderar egen headerfil.
#include "ADC.h"

// Statiska funktioner:
static void print_temperature(TempSensor* self);
static void ADC_read(TempSensor* self);

/******************************************************************************
* Funktionen new_TempSensor används för implementering av en temperatursensor 
* ansluten till någon av analoga pinnar A0 - A5 via ett objekt av strukten
* TempSensor. Ingående argument PIN utgör en pekare till aktuellt PIN-nummer. 
* Först skapas ett objekt av structen TempSensor döpt self. Därefter initieras
* objektets instansvariabler, följt av att det initierade objektet returneras.
******************************************************************************/
TempSensor new_TempSensor(uint8_t PIN)
{
	TempSensor self;
	self.PIN = PIN;
	self.ADC_result = 0x00;
	self.temperature.rounded = 0x00;
	self.print_temperature = print_temperature;
	self.ADC_read = ADC_read;
	return self;
}

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
*							temperatur = 100 * Uin - 50,
*
* där Uin är den beräknade analoga inspänningen (0 - 5 V).
*
* Slutligen transmitteras den beräknade temperaturen till vår PC via anrop av
*  funktionen serial_print_integer, som möjliggör sammansättning av text och 
* heltal samt seriell överföring. Därmed transmitteras textstycktet 
* "Temperature: %d degrees", där %d är formatspecificerare för heltal och 
* ersätts med avläst rumstemperatur.
******************************************************************************/
static void print_temperature(TempSensor* self)
{
	float Uin;
	ADC_read(self);
	Uin = VCC * (float)(self->ADC_result / ADC_MAX);
	self->temperature.rounded = (float)(100 * Uin - 50 + 0.5);
	serial_print_integer("Temperature: %d degrees Celcius\n", self->temperature.rounded);
	return;
}

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
 ******************************************************************************/
static void ADC_read(TempSensor* self)
{
	ADMUX = ((1 << REFS0)|self->PIN);
	START_AD_CONVERSION;
	WAIT_FOR_AD_CONVERSION_COMPLETE;
	RESET_ADC_INTERRUPT_FLAG;
	self->ADC_result = ADC;
}