#ifndef _RTC17xx_HPP_
#define _RTC17xx_HPP_

#include "lpc17xx_defs.h"
#include "types.h"
#include "types_ext.hpp"


struct rtcCreationDisposition
  : public basicCreationDisposition
{
  unsigned long pclk;                 /* �������� �������, ���������� �� ����������     */
                                      /*   ������ �����                                 */
  unsigned long valid_timeout;        /* ������� ����������, ���                        */
};


class rtc17xx
{
public:
                                      /* UTC-����� �� ���������: 00:00:00 01.10.2010    */
  static const unsigned long RTC_UTC_START = 0x4CA52480;


  rtc17xx(LPC_TIM_TypeDef* phy, rtcCreationDisposition* rcd);
  ~rtc17xx();

  int syncPhase();                    /* ������ ������������� ���� ������� RTC          */
  int setRTC  (unsigned long  utc);
  int getRTC  (unsigned long* utc, unsigned long* msec);
  int setValid(bool valid = true);
  int setAlarm(unsigned long  utc, unsigned long  msec, callback0* acb);

  static rtc17xx* g_rtc;

private:
  struct svc_data                     /* ������ RTC-�������                             */
  {
#define RTCSVC_CALLID_SYNC        0   /* ������������� ���� ����� RTC                   */
#define RTCSVC_CALLID_SYNC_SKIP   1   /* �������� ������� ������������� �����           */
#define RTCSVC_CALLID_SECOND      2   /* �������� ������ �������                        */
#define RTCSVC_CALLID_SET         3   /* ��������� �����                                */
#define RTCSVC_CALLID_GET         4   /* ������ �����                                   */
#define RTCSVC_CALLID_VALIDUP     5   /* �������������� ���������� ���������������      */
#define RTCSVC_CALLID_VALIDDOWN   6   /* ������ ���������� ���������������              */
#define RTCSVC_CALLID_BINDALARM   7   /* ��������� ����������                           */
#define RTCSVC_CALLID_RUNALARM    8   /* ������������ ����������                        */
    unsigned char callID;             /* ������������� ������                           */
    unsigned long utc;                /* ����� utc, ��� ��������� �� ����               */
    unsigned long msec;               /* ����� ms, ��� ��������� �� ����                */
    void* data;                       /* ��������� �� ������, ��������� ������          */
                                      /*   ������������ ������������� ��������          */
  };

                                      /* ����� �����                                    */
#define RTCF_SYNC_PHASE   0x01        /* ���� ����������������                          */
#define RTCF_SYNC_UTC     0x02        /* ����� UTC ����������������                     */
  unsigned long flags;
  unsigned long prescaler_corrector;  /* ����������� ��� ��������� ������������         */

  unsigned long utc;                  /* ������� �������� ������� � ��������� �� ���    */
                                      /* �������� �� �������� � ������� ��������        */
                                      /*   ����������� �������                          */
  unsigned long valid_top;            /* ������ ��������� ����������                    */
  unsigned long valid_tout;           /* ������� ����������                             */


  unsigned long alarmutc;             /* ������� ������������ ����������                */
  unsigned long alarmmsec;            /* ������������ ������������ ����������           */
  callback0* alarmer;                 /* ���������� ����������                          */

  LPC_TIM_TypeDef* low;               /* �������� �������                               */
  unsigned long pconpMask;            /* ����� ����������� ������� � �������� PCONP     */
  unsigned long irqnum;               /* ����� ���������� ����������� �������           */

                                      /* ���������� ���������� ����������� ������� RTC  */
  static void rtch        (void* instance);
  static int  rtc_service (void* instance, void* d);

  __inline void utc_inc()
  {
    this->utc++;
    if(this->utc > this->valid_top)
      this->flags &= ~RTCF_SYNC_UTC;
  }
};

#endif /*_RTC17xx_HPP_*/
