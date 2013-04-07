/*
 * global.h
 *
 * definierar alla globala variabler
 * Created: 4/5/2013 11:23:22 AM
 *  Author: tobno568
 */ 


#ifndef GLOBAL_H_
#define GLOBAL_H_

//dessa variabler kan diskuteras
unsigned char mapX; //Xcoord för nuvarande ruta i kartindex
unsigned char mapY; //Xcoord för nuvarande ruta i kartindex
char x; //offset från origo för x i nuvarande ruta mäts i 1/3 cm origo är i mitten av rutan
char y; //offset från origo för x i nuvarande ruta mäts i 1/3 cm
unsigned char v; //hastighet i 1/2 cm/s
unsigned char theta; //vridning i 360/255 grader
unsigned char omega; //vinkelhastighet i ?? grader/sekund

unsigned char route[16];
unsigned char route[16];

#endif /* GLOBAL_H_ */