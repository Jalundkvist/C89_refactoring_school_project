// Inkluderingsdirektiv: 
#include "Timer.h"

// Statiska funktioner:
static void Timer_on(Timer* self);
static void Timer_off(Timer* self);
static void Timer_toggle(Timer* self);
static void Timer_count(Timer* self);
static bool Timer_elapsed(Timer* self);
static void Timer_clear(Timer* self);
static void Timer_reset(Timer* self);
static void Timer_set(Timer* self, const uint32_t delay_time);
static void init_timer(const TimerSelection timerSelection);
static inline uint32_t get_required_interrupts(const uint32_t delay_time);

/******************************************************************************
* Funktionen new_Timer anv�nds f�r att skapa och initiera objekt av strukten 
* Timer. Ing�ende argument timerSelection anv�nds f�r att v�lja vilken av
* timerkretsar Timer 0 - 2 som skall anv�ndas, medan delay_time utg�rs av
* den f�rdr�jningstid som timern skall r�kna till, m�tt i millisekunder.
*
* Ett nytt objekt av strukten Timer initieras som d�ps till self.
* Struktens medlemmar initieras;
* enabled s�tts till false f�r att indikera att timer-kretsen �r avst�ngd
* vid start, vald timerkrets sparas via medlemmen timerSelection,
* antalet exekverade avbrott s�tts till noll vid start
* medan antalet avbrott som kr�vs f�r specificerad f�rdr�jningstid
* ber�knas via anrop av funktionen get_required_interrupts, d�r ing�ende
* argument delay_time passeras som parameter. Returv�rdet fr�n detta anrop,
* vilket �r ber�knat antalet avbrott som kr�vs f�r specificerad f�rdr�jningtid,
* lagras sedan via medlemmen
* required_interrupts. Slutligen returneras det nu initierade objektet self,
* som �r redo att anv�ndas f�r implementering av en given timerkrets.
******************************************************************************/
Timer new_Timer(const TimerSelection timerSelection, const uint32_t delay_time)
{
	Timer self;
	
	self.enabled = false;
	self.timerSelection = timerSelection;
	self.executed_interrupts = 0x00;
	self.required_interrupts = get_required_interrupts(delay_time);
	init_timer(self.timerSelection);
	
	self.on = Timer_on;
	self.off = Timer_off;
	self.toggle =Timer_toggle;
	self.count = Timer_count;
	self.elapsed = Timer_elapsed;
	self.clear = Timer_clear;
	self.reset = Timer_reset;
	self.set =  Timer_set;
	
	return self;
}

/******************************************************************************
* Funktionen Timer_on anv�nds f�r att aktivera en given timer. Ing�ende
* argument self utg�r en pekare till ett timerobjekt, vars medlem 
* timerSelection unders�ks f�r att ta reda p� vilken timerkrets som skall
* aktiveras. Aktuell timerkrets aktiveras, f�ljt av att medlemmen enabled 
* s�tts till true f�r att indikera att timern i fr�ga nu �r aktiverad.
******************************************************************************/
static void Timer_on(Timer* self)
{
	if (self->timerSelection == TIMER0)
	{
		ENABLE_TIMER0;
	}
	
	else if (self->timerSelection == TIMER1)
	{
		ENABLE_TIMER1;
	}
	
	else if (self->timerSelection == TIMER2)
	{
		ENABLE_TIMER2;
	}
	self->enabled = true;
	return;
}

/******************************************************************************
* Funktionen Timer_off anv�nds f�r att inaktivera en given timer. Ing�ende
* argument self utg�r en pekare till ett timerobjekt, vars medlem
* timerSelection unders�ks f�r att ta reda p� vilken timerkrets som skall
* inaktiveras. Aktuell timerkrets inaktiveras, f�ljt av att medlemmen enabled
* s�tts till false f�r att indikera att timern i fr�ga nu �r inaktiverad.
******************************************************************************/
void Timer_off(Timer* self)
{
	if (self->timerSelection == TIMER0)
	{
		DISABLE_TIMER0;
	}
	
	else if (self->timerSelection == TIMER1)
	{
		DISABLE_TIMER1;
	}
	
	else if (self->timerSelection == TIMER2)
	{
		DISABLE_TIMER2;
	}
	self->enabled = false;
	return;
}

/******************************************************************************
* Funktionen Timer_toggle anv�nds f�r att toggla aktivering av en given timer.
* Ing�ende argument self utg�r en pekare till ett timerobjekt, vars medlem
* enabled unders�ks f�r att ta reda p� om timern just nu �r aktiverad eller 
* inte. Om timern f�r tillf�llet �r aktiverad (enabled �r true), s� st�ngs den
* av via funktionen Timer_off, som anropas via pekaren off. D�remot om timern 
* f�r tillf�llet �r inaktiverad (enabled �r false), s� st�ngs den av via 
* funktionen Timer_on, som anropas via pekaren on.
******************************************************************************/
static void Timer_toggle(Timer* self)
{
	if (self->enabled)
	{
		Timer_off(self);
	}
	
	else
	{
		Timer_on(self);
	}
	
	return;
}

/******************************************************************************
* Funktionen Timer_count anv�nds f�r att r�kna antalet exekverade avbrott som
* har �gt rum. Om timern i fr�ga �r aktiverad s� r�knas antalet exekverade
* avbrott upp via inkrementering av medlemmen executed_interrupts. Annars
* om timern �r inaktiverad sker ingen uppr�kning, s� att timern inte av 
* misstag skall l�pa ut och orsaka avbrott n�r den �r avst�ngd.
******************************************************************************/
static void Timer_count(Timer* self)
{
	if (self->enabled)
	{
		self->executed_interrupts++;
	}
	
	return;
}

/******************************************************************************
* Funktionen Timer_elapsed anv�nds f�r att unders�ka ifall en given timer 
* har l�pt ut och i s� fall nollst�lla timern. Ing�ende argument self utg�r en
* pekare till ett timerobjekt. F�r att ta reda p� ifall timern har l�pt ut s�
* unders�ks antalet exekverade avbrott, vilket lagras av timerobjektets medlem 
* executed_interrupts) mot antalet avbrott som kr�vs f�r aktuell f�rdr�jning 
* som timern skall medf�ra, vilket lagras av medlemmen required_interrupts. 
*
* Om timern har l�pt ut s� kommer antalet exekverade avbrott �verstiga eller 
* vara lika med antalet avbrott som kr�vs f�r f�rdr�jningen. Om detta �r fallet
* s� nollst�lls antalet exekverade avbrott, vilket motsvarar att timern
* nollst�lls, f�ljt av att true returneras f�r att indikera att timern har 
* har l�pt ut. Annars om timern inte har l�pt ut s� returneras false utan att
* timern nollst�lls. 
******************************************************************************/
static bool Timer_elapsed(Timer* self)
{
	if (self->executed_interrupts >= self->required_interrupts)
	{
		self->executed_interrupts = 0x00;
		return true;
	}
	
	return false;
}

/******************************************************************************
* Funktionen Timer_clear anv�nds f�r att nollst�lla en given timer. Ing�ende
* argument self utg�r en pekare till ett timerobjekt, vars medlem 
* executed_interrupts nollst�lls f�r att d�rigenom nollst�lls timerkretsen.
******************************************************************************/
static void Timer_clear(Timer* self)
{
	self->executed_interrupts = 0x00;
	return;
}


/******************************************************************************
* Funktionen Timer_reset anv�nds f�r att �terst�lla en given timer, vilket
* inneb�r att timern st�ngs av och nollst�lls. F�rst st�ngs timern av via
* funktionen Timer_off, som anropas via pekaren off. D�refter nollst�lls antalet
* exekverade avbrott f�r att nollst�lls timern, vilket sker via nollst�llning
* av timerobjektets medlem executed_interrupts.
******************************************************************************/
static void Timer_reset(Timer* self)
{
	self->off(self);
	self->executed_interrupts = 0x00;
	return;
}
/******************************************************************************
* Funktionen Timer_set anv�nds f�r att uppdatera f�rdr�jningstiden p� en given
* timer. Ing�ende argument self utg�rs av en pekare till ett timerobjekt, 
* medan delay_time utg�r den nya f�rdr�jningstiden, m�tt i millisekunder.
* Timerns f�rdr�jningstid, som lagras av medlemmen required_interrupts, 
* uppdateras via anrop av funktionen get_required_interrupts, d�r ing�ende
* argument delay_time passeras som ing�ende argument.
******************************************************************************/
static void Timer_set(Timer* self, const uint32_t delay_time)
{
	self->required_interrupts = get_required_interrupts(delay_time);
	return;
}

/******************************************************************************
* Funktionen init_timer anv�nds f�r att initiera en given timerkrets f�r en
* uppr�kningstid d�r ett avbrott sker var 1.024:e millisekund vid uppr�kning
* till 256, d� overflow- eller CTC-avbrott sker, beroende p� anv�nd timerkrets. 
* Ing�ende argument timerSelection indikerar vilken timer som skall initieras.
*
* F�rst aktiveras avbrott globalt via ettst�llning av I-flaggan i statusregistret
* SREG. Denna bit m�ste alltid vara ettst�lld f�r att timergenererade avbrott
* skall kunna implementeras. Sedan initeras aktuell timerkrets, d�r de 8-bitars 
* timerkretsarna Timer 0 samt Timer 2 initieras i Normal Mode, vilket inneb�r 
* uppr�kning tills overflow sker (efter uppr�kning till 256, som inte ryms i 
* 8-bitars register).
* 
* F�r den 16-bitars timerkretsen Timer 1 anv�nds CTC Mode (Clear Timer On 
* Compare), d�r maxv�rdet f�r uppr�kning s�tts till 256 f�r att matcha de
* andra tv� timerkretsarna, s� att avbrott sker lika ofta f�r respektive 
* timer. Eftersom Timer 1 vid uppr�kning till 256 �r l�ngt fr�n full anv�nds
* d�rmed CTC Mode, d�r timern nollst�lls automatiskt vid uppr�kning till
* f�rvalt maxv�rde, vilket i detta fall �r 256.
******************************************************************************/
static void init_timer(const TimerSelection timerSelection)
{
	if (timerSelection == TIMER0)
	{
		INIT_TIMER0;
	}
	
	else if (timerSelection == TIMER1)
	{
		INIT_TIMER1;
		SET_TIMER1_LIMIT;
	}
	
	else if (timerSelection == TIMER2)
	{
		INIT_TIMER2;
	}
	
	return;
}

/******************************************************************************
* Funktionen get_required_interrupts anv�nds f�r att ber�kna och antalet
* avbrott som kr�vs f�r en given f�rdr�jningstid. Ing�ende argument delay_time
* utg�rs av specificerad f�rdr�jningstid m�tt i millisekunder. Antalet avbrott
* som kr�vs f�r aktuell f�rdr�jning ber�knas som kvoten av specificerad 
* f�rdr�jningstid genom tiden mellan varje avbrott, vilket i detta fall �r
* satt till 1.024 ms, h�r implementerat via makrot INTERRUPT_TIME. Antalet
* avbrott som kr�vs f�r specificerad f�rdr�jningstid avrundas till n�rmaste
* heltal via anrop av funktionen round_to_integer och returneras sedan.
******************************************************************************/
static inline uint32_t get_required_interrupts(const uint32_t delay_time)
{
	return (uint32_t)(delay_time / INTERRUPT_TIME + 0.5);
}