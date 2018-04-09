#ifndef _SSP_HPP_
#define _SSP_HPP_

#include "lpc17xx_defs.h"
#include "core.h"
#include "mutex.hpp"
#include "serial.hpp"


struct sspCreationDisposition
  : public serialCreationDisposition
{
#define SSP_FORMAT_SPI  0
#define SSP_FORMAT_SSI  1
#define SSP_FORMAT_MWI  2

  unsigned char format;
  unsigned char cpol;
  unsigned char cpha;
};


class ssp
  : public serialPort
{
public:
  ssp(LPC_SSP_TypeDef* phy, sspCreationDisposition* creationDisposition);
  ~ssp();



  virtual int init      (const portSettings& ps);
  virtual const portSettings& settings() {return this->ps;}
  virtual int read      (unsigned char* buf, int  bufsz);
  virtual int readAsync (unsigned char* buf, int  bufsz, callback2* ac);
  virtual int readTimed (unsigned char* buf, int* bufsz, int timeout);
  virtual int write     (unsigned char* buf, int  bufsz);
  virtual int writeAsync(unsigned char* buf, int  bufsz, callback2* ac);
  virtual int writeTimed(unsigned char* buf, int* bufsz, int timeout);
  virtual int cancelReadAsync (int reason);
  virtual int cancelWriteAsync(int reason);
  virtual int purge     (unsigned char queue);
  virtual int sendchar  (unsigned char c);

  static void pinSetupSSP0_0(unsigned char setDefault);
  static void pinSetupSSP0_1(unsigned char setDefault);
  static void pinSetupSSP1_0(unsigned char setDefault);
  static void pinSetupSSP1_1(unsigned char setDefault);

private:
  #define RX_CHAR   0x01
  #define TX_EMPTY  0x02
  unsigned char*    buf;              // ��������� �� ����� ��� ������/�������� ������
  int               count;            // ������� �����������/������������ ������
  int               rx;               // ������� ����������� ������
  int               tx;               // ������� ������������ ������
  int               readed;
  int               writed;
  unsigned char     isTx;             // ������� �������� ������
  callback2*        completer; 
  unsigned char     irqNum;           // ����� ����������
  portSettings      ps;               // ��������� �����
  LPC_SSP_TypeDef*  low;              // ��������� �� �������� ������������� ����� SSP
  PIN_SETUP         pinSetupProc;     // ��������� �� ����� ���������������� ����� SSP
  CIRC_BUF64        inbuf;            // ���������� ����������� ����� �� ������ ������
  TN_EVENT          evts;             // ���� TN_EVENT
  unsigned long     pwrMask;          // ������� ����� ��������� ������� ������������� ����� SSP

  void onTimedComplete(int bytes, int err);
  static void irqh(ssp* _this);
};

#ifdef _MUTEX_HPP_

class ssp_safe
  : public ssp
{
public:
  ssp_safe(LPC_SSP_TypeDef* phy, sspCreationDisposition* cd);
  virtual ~ssp_safe();

  /******************************* ���������� serialPort ********************************/
  virtual int init      (const portSettings& ps);
  //virtual const portSettings& settings();
  virtual int read      (unsigned char* buf, int  bufsz);
  virtual int readAsync (unsigned char* buf, int  bufsz, callback2* ac);
  virtual int readTimed (unsigned char* buf, int* bufsz, int timeout);
  virtual int write     (unsigned char* buf, int  bufsz);
  virtual int writeAsync(unsigned char* buf, int  bufsz, callback2* ac);
  virtual int writeTimed(unsigned char* buf, int* bufsz, int timeout);
  virtual int cancelReadAsync (int reason);
  virtual int cancelWriteAsync(int reason);
  virtual int purge     (unsigned char queue);
  virtual int sendchar  (unsigned char c);


private:
  mutex lock;

};

#endif /*_MUTEX_HPP_*/

#endif /*_SSP_HPP_*/
