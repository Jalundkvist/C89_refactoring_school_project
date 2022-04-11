// Inkluderar egen headerfil.
#include "ADC.h"

// Statiska funktioner:
static void print_temperature(TempSensor* self);
static void ADC_read(TempSensor* self);

/******************************************************************************
* Funktionen new_TempSensor anv�nds f�r implementering av en temperatursensor 
* ansluten till n�gon av analoga pinnar A0 - A5 via ett objekt av strukten
* TempSensor. Ing�ende argument PIN utg�r en pekare till aktuellt PIN-nummer. 
* F�rst skapas ett objekt av structen TempSensor d�pt self. D�refter initieras
* objektets instansvariabler, f�ljt av att det initierade objektet returneras.
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
* Funktionen print_temperature anv�nds f�r att l�sa av rumstemperaturen och 
* skriva till v�r PC. F�rst l�ses temperatursensorn av och resultatet lagras 
* i konstanten ADC_result. D�refter ber�knas motsvarande analoga insp�nning Uin 
* med f�ljande formel:
*							Uin = Vcc * ADC_result / ADC_MAX,
*
* d�r ADC_result �r resultatet fr�n senaste AD-omvandling (0 - 1023).
*
* D�refter ber�knas motsvarande temperatur i grader Celcius via f�ljande formel:
*
*							temperatur = 100 * Uin - 50,
*
* d�r Uin �r den ber�knade analoga insp�nningen (0 - 5 V).
*
* Slutligen transmitteras den ber�knade temperaturen till v�r PC via anrop av
*  funktionen serial_print_integer, som m�jligg�r sammans�ttning av text och 
* heltal samt seriell �verf�ring. D�rmed transmitteras textstycktet 
* "Temperature: %d degrees", d�r %d �r formatspecificerare f�r heltal och 
* ers�tts med avl�st rumstemperatur.
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
* Funktionen ADC_read anv�nds f�r att l�sa av temperatursensorn och returnera 
* resultatet. F�rst v�ljs analog kanal f�r avl�sning, vilket i detta fall �r 
* temperatorsensorns PIN A1 (PORTC1), samtidigt som AD-omvandlaren s�tts till 
* att matas med intern matningssp�nning. D�refter aktiveras AD-omvandlaren och 
* startas med l�gsta m�jliga frekvens (125 kHz) f�r h�gsta m�jliga precision. 
* D�refter v�ntar vi till att AD-omvandlingen �r slutf�rd, vilket signaleras 
* via AD-omvandlarens interrupt-flagga ADIF (ADC Interrupt Flag), som d� blir
* ettst�lld. F�r att sedan �terst�lla ADIF inf�r n�sta AD-omvandlaren s� 
* ettst�lls denna, f�ljt av att avl�st resultat returneras vid �terhoppet.
 ******************************************************************************/
static void ADC_read(TempSensor* self)
{
	ADMUX = ((1 << REFS0)|self->PIN);
	START_AD_CONVERSION;
	WAIT_FOR_AD_CONVERSION_COMPLETE;
	RESET_ADC_INTERRUPT_FLAG;
	self->ADC_result = ADC;
}