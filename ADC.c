#include "ADC.h" /* Inkluderar egen headerfil. */

/******************************************************************************
* Funktionen new_TempSensor används för implementering av en temperatursensor 
* ansluten till någon av analoga pinnar A0 - A5 via ett objekt av strukten
* TempSensor. Ingående argument PIN utgör en pekare till aktuellt PIN-nummer. 
* Först allokeras minne för ett nytt objekt av strukten TempSensor döpt self.
* Ifall minnesallokeringen misslyckas så avslutas funktionen direkt. Annars
* initieras objektets instansvariabler, följt av att det initierade objektet
* returneras.
******************************************************************************/
struct TempSensor* new_TempSensor(unsigned char* PIN)
{
	struct TempSensor* self = (struct TempSensor*)malloc(sizeof(struct TempSensor));
	
	if (!self)
	{
		 return NULL;
	}
	
	(*self).PIN = *PIN;
	(*self).ADC_result = 0x00;
	(*self).temperature.rounded = 0x00;
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
void print_temperature(struct TempSensor* self)
{
	float Uin;
	ADC_read(&(*self).ADC_result);
	Uin = VCC * ((*self).ADC_result / ADC_MAX);
	(*self).temperature.rounded = (int)(100 * Uin - 50 + 0.5);
	serial_print_integer("Temperature: %d degrees Celcius\n", &((*self).temperature.rounded));
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
void ADC_read(unsigned short* ADC_result)
{
	SELECT_ANALOG_CHANNEL;
	START_AD_CONVERSION;
	WAIT_FOR_AD_CONVERSION_COMPLETE;
	RESET_ADC_INTERRUPT_FLAG;
	*ADC_result = ADC;
	return;
}
