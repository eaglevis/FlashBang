/*
 * common.h
 *
 *  Created on: 08-09-2011
 *      Author: eagle
 */

#ifndef COMMON_H_
#define COMMON_H_

typedef void (*fptr)();

typedef struct {
	uint8_t time;
	fptr function;
} timeTrigger;

void initTicker();
uint8_t registerCallback(uint8_t ticks, void* function);
void checkCallback();




void modeMaster(void);
void modeSlave(void);

void setChan1();
void setChan2();
void setChan3();
void setChan4();
void setChan5();

void slaveReset();
void slaveChan5();
void slaveChan4();
void slaveChan3();
void slaveChan2();
void slaveChan1();

#define LED C, 5

#define F_TRIG B, 1
#define F_QUENCH B, 2



#define GLUE(a, b) a##b

#define SET_(what, p, m) GLUE(what, p) |= (1 << (m))
#define CLR_(what, p, m) GLUE(what, p) &= ~(1 << (m))
#define TOG_(what, p, m) GLUE(what, p) ^= (1 << (m))
#define GET_(/* PIN, */ p, m) ( GLUE(PIN, p) & (1 << (m)) )
#define SET(what, x) SET_(what, x)
#define CLR(what, x) CLR_(what, x)
#define TOG(what, x) TOG_(what, x)
#define GET(x) GET_(x)



#endif /* COMMON_H_ */
