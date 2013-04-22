/*
 * clockedInterrupt.h
 *
 * Created: 4/12/2013 11:45:36 AM
 *  Author: davmo049
 */ 


#ifndef CLOCKEDINTERRUPT_H_
#define CLOCKEDINTERRUPT_H_

void clockedInterrupt_init();
void updateState(uint16_t gyro, uint8_t vRight, uint8_t vLeft);


#endif /* CLOCKEDINTERRUPT_H_ */