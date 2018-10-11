/*
 * Touchscreen.h
 *
 * Created: 2018/9/23 21:16:53
 *  Author: 123
 */ 


#ifndef TOUCHSCREEN_H_
#define TOUCHSCREEN_H_


extern void setupADC();
extern uint16_t detect(void);
extern uint16_t touch_screenX(void);
extern uint16_t touch_screenY(void);


#endif /* TOUCHSCREEN_H_ */