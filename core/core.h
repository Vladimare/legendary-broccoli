#ifndef _CORE_H_
#define _CORE_H_

#ifdef __cplusplus
extern "C"
{
#endif

#include "types.h"
#include <stdint.h>
#include <time.h>

#define SYSTIMER_PERIOD   10ul        /* ������ ������������ ���������� �������, � ��     */


/* ��������� ���������� ��� ������� coreInit() */
typedef __packed struct tagCORE_SETTINGS
{
  unsigned char source;               /* �������� �������                                 *
                                       * ��� LPC13xx:                                     *
                                       *   nCLKSRC_INTRC  (0) - ���������� RC-���������   *
                                       *   nCLKSRC_PLLIN  (1) - ���� SYS PLL              *
                                       *   nCLKSRC_WDT    (2) - ���������� ������         *
                                       *   nCLKSRC_PLLOUT (3) - ����� SYS PLL             *
                                       * ��� LPC17xx:                                     *
                                       *   nCLKSRC_INTRC  (0) - ���������� RC-���������   *
                                       *   nCLKSRC_MAINOSC(1) - ������� ��������� �� XTAL *
                                       *   nCLKSRC_RTCOSC (2) - ������� ��������� �� RTCX */
  unsigned char pll0;                 /* ��� LPC13xx:                                     *
                                       *   �� ������������                                *
                                       * ��� LPC17xx:                                     *
                                       *   1 - ��������� PLL0                             */
  unsigned long pclksel0;             /* ��� LPC13xx:                                     *
                                       *   �� ������������                                *
                                       * ��� LPC17xx:                                     *
                                       *   peripheral clock select 0                      */
  unsigned long pclksel1;             /* ��� LPC13xx:                                     *
                                       *   �� ������������                                *
                                       * ��� LPC17xx:                                     *
                                       *   peripheral clock select 1                      */
  unsigned long sourcefreq;           /* ������� ���������                                *
                                       *   ��� nCLKSRC_INTRC ������ ������ ����:          *
                                       *   - 12��� � LPC13xx                              *
                                       *   -  4��� � LPC17xx                              */
  unsigned long corefreq;             /* �������� ������� ����                            */
  unsigned long pllm;                 /* �������� ��������� ��� PLL0                      *
                                       *   ���� ������������, ������ ���� � ���� pll0     *
                                       *   ������� ��������� PLL0                         */
  unsigned long plln;                 /* �������� �������� ��� PLL0                       *
                                       *   ���� ������������, ������ ���� � ���� pll0     *
                                       *   ������� ��������� PLL0                         */
  unsigned long offset;               /* �������� ��� ������� ������� ����������          */
}CORE_SETTINGS;

extern void           coreInit            (const CORE_SETTINGS* freq);
extern void           coreRegisterSVC     (unsigned char svcnum, SERVICE_DATA* svc);
extern void           coreUnregisterSVC   (unsigned char svcnum);
extern int            coreRequestSVC      (unsigned char svcnum, void* data);
extern void           coreRegisterIRQ     (unsigned char irqnum, unsigned char priority, IRQ_DATA* handler);
extern void           coreUnregisterIRQ   (unsigned char irqnum);
extern void           coreEnableIRQ       (unsigned char irqnum);
extern void           coreDisableIRQ      (unsigned char irqnum);
extern void           coreRestart         (void);
extern unsigned long  coreCurrentFrequency(void);

extern const CORE_SETTINGS core_settings;


#ifdef __cplusplus
}
#endif


#endif /*_CORE_H_*/
