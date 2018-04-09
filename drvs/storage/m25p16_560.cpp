#include "lpc17xx_defs.h"
#include "m25p16_560.hpp"


m25p16_560::m25p16_560(extstorageCreationDisposition* escd)
  : m25p16(escd)
{
  LPC_PINCON->PINSEL0 &= ~((0x03ul << (P0_4 * 2)) | /* ���� P0.4  - ������ WRITE PROTECT ����������*/
                           (0x03ul << (P0_6 * 2))); /* ���� P0.6  - ������ CHIP SELECT ����������  */
  LPC_PINCON->PINSEL0 |=  P0_4_GPIO | P0_6_GPIO;    /* ���� P4.28 - ������ HOLD ����������         */

  LPC_PINCON->PINSEL9 &= ~(0x03ul << ((P4_28 - 16) * 2));
  LPC_PINCON->PINSEL9 |=  P4_28_GPIO;

  LPC_GPIO0->FIODIR |= (0x01ul << P0_4) | (0x01ul << P0_6);
  LPC_GPIO0->FIOSET  = (0x01ul << P0_4) | (0x01ul << P0_6);

  LPC_GPIO4->FIODIR |= (0x01ul << P4_28);
  LPC_GPIO4->FIOSET  = (0x01ul << P4_28);
}

m25p16_560::~m25p16_560()
{
  LPC_PINCON->PINSEL0 &= ~((0x03ul << (P0_4 * 2)) | (0x03ul << (P0_6 * 2)));
  LPC_PINCON->PINSEL9 &= ~ (0x03ul << ((P4_28 - 16) * 2));
}

void m25p16_560::chip_select(unsigned char ic, bool select)
{
  if(select)
    LPC_GPIO0->FIOCLR = (0x01ul << P0_6);
  else
    LPC_GPIO0->FIOSET = (0x01ul << P0_6);
}
