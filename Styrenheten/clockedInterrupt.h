/**
	* TSEA27 Elektronikprojekt
	*
	* IDENTIFIERING
	*
	* Modul:Styrenheten
	* Filnamn: clockedInterrupt.h
	* Skriven av: J. Källström, D. Molin, T. Norlund	
	* Datum: 2013-05-15
	* Version: 1.0
	*
	* BESKRIVNING
	*
	* Klockat avbrott som hämtar in ny sensordata, skickar sensordatan till kommunikationsenheten och uppdaterar observatören
	*/	
	


#ifndef CLOCKEDINTERRUPT_H_
#define CLOCKEDINTERRUPT_H_


/** 
 * Sätter upp register för att skapa ett avbrott med samma tidsintervall
 */
void clockedInterrupt_init();

/** 
 * Körs av avbrottet. Hämtar data från sensorenheten, skickar sensordatan till kommunikationsenheten och uppdaterar observatören
 */
void timedInterupt();


#endif /* CLOCKEDINTERRUPT_H_ */