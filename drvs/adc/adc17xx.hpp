#ifndef _ADC17XX_H_
#define _ADC17XX_H_

#include "lpc17xx_defs.h"
#include "types.h"
#include "tn.h"

#include "thread.hpp"
#include "types_ext.hpp"
#include "unitimer17xx.hpp"
#include "core.h"
#include "corelib.h"
#include "errors.h"
#include "syslog.hpp"

#include <stdio.h>

                                      /* ������ �������������� ������         */
struct adcChannelData
{
  unsigned char   chanNum;            /* ����� ������                         */
  unsigned long   periodCounter;      /* ���������� ����������� ��������      */
  unsigned short  value;              /* ��������� ���������� ��������        */
  unsigned short  getCounter;         /* ��������� ���������                  */
};

                                      /* ��������� �������������� ������      */
struct adcChannelSettings
{
  unsigned long period;               /* ��������� ������ ������              */
  callback1* dataReceiver;            /* ���������� ������                    */
};


                                      /* ��������� ���                        */
struct adcSettings
{
  unsigned long adcTick;              /* ������������ �������������� ����� ���*/
  unsigned long adcPclk;              /* �������� ������� ����������� �� ���  */

  unitimer17xx*     adcTimer;

  unsigned long timerPclk;            /* �������� ������� ����������� �� ���� */

  adcChannelSettings* chanSettings;   /* ��������� ������������� �������      */
  unsigned char chanCount;            /* ���������� �������                   */
};

/* ����� ����������� ����� �������������������� ������� ���*/
class adc17xx
  : public thread
{
public:
  adc17xx(LPC_ADC_TypeDef* adcphy, basicCreationDisposition* adcCD_, const adcSettings& s);
  virtual ~adc17xx();
  int setChanParam(unsigned char channel,  const adcChannelSettings& chanSettings);

protected:
  adcSettings settings;
  adcChannelData* chData;

  virtual int prepare(unsigned char channel, unsigned char* ADx, unsigned int* delay) = 0;
  virtual int finalize(unsigned char channel, unsigned int* delay) = 0;
  virtual void run();
  void setDelay(unsigned int delay);

private:
  basicCreationDisposition adcCD;
  LPC_ADC_TypeDef*  adclow;           /* CMSIS interface to low level ADC registers*/
  

  unsigned char  measureChannel;      /* ������� ������������� �����          */
  unsigned int   mr0Val;

  binder1<adc17xx> m0Binder;
  binder1<adc17xx> m1Binder;

                                      /* ���������� ���������� ���            */
  static void adcCompleteHandler(void* instance);
                                      /* ���������� ���������� �������        */
  void timerMatch0(int ch);
  void timerMatch1(int ch);

  static const threadCreationDisposition tcd;
};

#endif /*_ADC17XX_H_*/
