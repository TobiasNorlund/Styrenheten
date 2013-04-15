/*
 * global.h
 *
 * definierar alla globala variabler
 * Created: 4/5/2013 11:23:22 AM
 *  Author: tobno568
 */ 


#ifndef GLOBAL_H_
#define GLOBAL_H_
#include <avr/io.h>
//dessa variabler kan diskuteras
uint8_t mapX; //Xcoord för nuvarande ruta i kartindex
uint8_t mapY; //Xcoord för nuvarande ruta i kartindex
int8_t x; //offset från origo för x i nuvarande ruta mäts i 1/3 cm origo är i mitten av rutan
int8_t y; //offset från origo för x i nuvarande ruta mäts i 1/3 cm
uint8_t v; //hastighet i 1/2 cm/s
int8_t theta; //vridning i 360/255 grader
int8_t omega; //vinkelhastighet i ?? grader/sekund

uint8_t route[16];
uint8_t routeLength;

uint8_t debugMesssageBuffer[16];
uint8_t debugMesssageBufferLength; //initieras i clockedInterupt_init

uint8_t map[16][16];

uint8_t mapDataToSend[16];
uint8_t mapDataToSendSize; //initieras i clockedInterupt_init

uint8_t avstandsensor_1;
uint8_t avstandsensor_2;
uint8_t avstandsensor_3;
uint8_t avstandsensor_4;
uint8_t avstandsensor_5;
uint8_t avstandsensor_6;
uint8_t avstandsensor_7;
uint8_t avstandsensor_8;

#endif /* GLOBAL_H_ */