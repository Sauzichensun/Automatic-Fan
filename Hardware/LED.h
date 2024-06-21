#ifndef _LED_H
#define _LED_H


void On_GpioExpansion(uint8_t num);
void LED_On(uint8_t num);
void Scan_LEDState(void);
void ToleggePinsettime(void);
void BuzzeSetTimeTollde(void);
void BuzzeTimeDown(void);
void LED_Init(void);

#endif
