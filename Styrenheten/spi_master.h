#ifndef SPI_MASTER_H
#define SPI_MASTER_H

#include <avr/interrupt.h>

//exchange databit
#define CMD_EXCHANGE_DATA        0xE0 // 111 0 0000

#define SPI_BUFFERSIZE        	 16 // 111 0 0000

#define START 1
#define END 0

/*
Ställer in alla register för att agera som master.
*/
void SPI_MASTER_init();

/*
Skriver direkt. Returnerar 0 för fel, 1 för lyckad sparning.
*/
uint8_t SPI_MASTER_write(uint8_t *msg, uint8_t type, uint8_t len);

/*
Läser direkt. Returnerar 0 för fel, 1 för lyckad läsning.
*/
uint8_t SPI_MASTER_read(uint8_t *msg, uint8_t* type, uint8_t *len);

/*
Väljer enhet som skall vara aktiv på bussen. Denna funktion kommer endast att finnas på styrenheten som är
master. Styrpinnar för slaveselect kommer vara hårdkodat. */
void SPI_set_kom(uint8_t t);
void SPI_set_sensor(uint8_t t);


#endif
