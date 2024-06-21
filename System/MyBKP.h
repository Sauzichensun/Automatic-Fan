#ifndef _MYBKP_H
#define _MYBKP_H

void My_BKPInit(void);
void BKP_WriteData(uint16_t BKP_DR,uint8_t data);
void BKP_RreadData(uint16_t BKP_DR,uint8_t *data);
void Primitive_DataInit(void);


#endif
