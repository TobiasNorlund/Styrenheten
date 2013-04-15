/*
 * global.h
 *
 * definierar alla globala variabler
 * Created: 4/5/2013 11:23:22 AM
 *  Author: tobno568
 */ 

#define START_PIN PB4
#define MANUAL_AUTO_SWITCH_PIN PB5
#define MANUAL_SELECTED 1;

#ifndef GLOBAL_H_
#define GLOBAL_H_
#include <avr/io.h>
//dessa variabler kan diskuteras
uint8_t volatile mapX; //Xcoord för nuvarande ruta i kartindex
uint8_t volatile mapY; //Xcoord för nuvarande ruta i kartindex
int8_t volatile x; //offset från origo för x i nuvarande ruta mäts i 1/3 cm origo är i mitten av rutan
int8_t volatile y; //offset från origo för x i nuvarande ruta mäts i 1/3 cm
uint8_t volatile v; //hastighet i 1/2 cm/s
int8_t volatile theta; //vridning i 360/255 grader
int8_t volatile omega; //vinkelhastighet i ?? grader/sekund

uint8_t volatile route[16];
uint8_t volatile routeLength;

uint8_t volatile debugMesssageBuffer[16];
uint8_t volatile debugMesssageBufferLength; //initieras i clockedInterupt_init

uint8_t map[16][16];

uint8_t volatile mapDataToSend[16];
uint8_t volatile mapDataToSendSize; //initieras i clockedInterupt_init

uint8_t volatile avstandsensor_1;
uint8_t volatile avstandsensor_2;
uint8_t volatile avstandsensor_3;
uint8_t volatile avstandsensor_4;
uint8_t volatile avstandsensor_5;
uint8_t volatile avstandsensor_6;
uint8_t volatile avstandsensor_7;
uint8_t volatile avstandsensor_8;

#endif /* GLOBAL_H_ */