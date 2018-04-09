#ifndef _CMX469_HPP_
#define _CMX469_HPP_

#include "event.hpp"
#include "serial.hpp"


/******************************************************************************
 ************* ����������� ������������ �������� ��������������� **************
 ******************************************************************************/
#define RuC_CARRIER_HYST_BITS       5 /* ���������� �����, ������� ������� ������������ *
                                       *   ����� ���������� ������� CARRIER_DETECT      *
                                       * ���� ������ �� �����������������, �� �����     *
                                       *   ������ ������������                          */
#define RuC_PREAMBLE_DIFFS_COUNT    5 /* ������������ ���������� �����, �� �������      *
                                       *   �������� ��������� ����� ���������� ��       *
                                       *   ���������                                    */


/****************************************************************************************
 ************************************* type defines *************************************
 ****************************************************************************************/

/* ������� ����� ��� ������ � ���������������� CMX469                                   *
 * ��� ������ ���������� ����� ������ ���� ��������� ���� �����-��������� �� cmx469,    *
 *   � ������� ������������ ������ ����������� ����������� ������ ������ � ������       *
 * ����� �������� ����������� ���������� ��������� ��������� serialPort, ������, ��     *
 *   ����, �������� ��� ������� ����������: ������ ����������� � ���������� �������     *
 * �������� ����� ������������ ���������� writeXXX. � ������ ������� ����� ��� �������� *
 *   ����������� ��������� {0xAA 0xAA 0x22 0xE4 0x6B}, � � ����� - ��� ������� ����     *
 *   ������ ����� ����� ��������� ���������� ����������������������� ����� ��������     *
 * ���� ��� �������� �����, ������� ��������� � ������ �������� ������ �����. � ����    *
 *   ������ ��������� ������������� ��������� ������� CARRIER_DETECT. ���� ���� ������  *
 *   ���������, �� ������� �� ��������� ������. ���� ������ ��������������, �� �������  *
 *   �������� ����������� ������� ������� ����� � ������� ��������� {0x22 0xE4 0x49}.   *
 *   ��� ����������� ��������� ������� �������� ��������� ��� ����������� ���� �������� *
 *   �� 12 ���. ������ ����� ����� ��������� ����� ��������, �� �������� ����������     *
 *   �������� ����. ����� �������, ��������� ������ ��������� ������ ���������� �����.  *
 * ���������� ����� ��������� ���������� ������� CARRIER_DETECT �� �����, ����� ���     *
 *   RuC_CARRIER_HYST_BITS ���. ��������������� ���������� ������� CARRIER_DETECT       *
 *   ������������ ���������.                                                            *
 * ��� ���������� ��������� ������� ��������� �������� ���������� �������� ��������� c  *
 *   ���������, �� �� �����, ��� � RuC_PREAMBLE_DIFFS_COUNT �����                       *
 * �������� ��������� ���� ����������� �� ���������� ������, � �������� �� ������ �     *
 *   ������� ������� readXXX() ������ ������� (�.�., ���� �� ���������� ������ ����     *
 *   ������, �� ������ CARRIER_DETECT ��� �� ������, �� ������ readXXX ����� ���������� *
 *   ������� ���������)                                                                 *
 * ������ ��� ��� ������ ������ ����� ���������� ����� ���������. ���� ��� ���� �����   *
 *   �� ����, �� ���������������� ������� ���������� ������, ��������� ����� �����������*
 *   ��������                                                                           */
class cmx469
  : public serialPort
{
public:
  class stat_events
  {                                   /* ��������� � ����������� ���������� ������      */
  public:
    /* ���������� ������ ��� ��� ������ ��������� */
    virtual void preamble_hit() = 0;

    /* ���������� ��� ���������� �������� lostblocks */
    virtual void block_missed(unsigned long lost_total) = 0;
  };

  cmx469(stat_events* se);
  virtual ~cmx469();

  /******************************* ���������� serialPort ********************************/
  virtual int init      (const portSettings& ps);
  virtual const portSettings& settings();

  virtual int read      (unsigned char* buf, int  bufsz);
  virtual int readAsync (unsigned char* buf, int  bufsz, callback2* ac);
  virtual int cancelReadAsync (int reason);
  virtual int readTimed (unsigned char* buf, int* bufsz, int timeout);

  virtual int write     (unsigned char* buf, int  bufsz);
  virtual int writeAsync(unsigned char* buf, int  bufsz, callback2* ac);
  virtual int cancelWriteAsync(int reason);
  virtual int writeTimed(unsigned char* buf, int* bufsz, int timeout);

  virtual int purge     (unsigned char queue);
  virtual int sendchar  (unsigned char c);

protected:
                                      /* ����� �������������� ��������� ��� ����������� */
  virtual void pinSetup(bool setDefault = false) = 0;
                                      /* ���������� / ������ ��������� ����������       */
  virtual void rxEnable(unsigned char enable = 1) = 0;
                                      /* ���������� / ������ ����������� ����������     */
  virtual void txEnable(unsigned char enable = 1) = 0;
                                      /* ���������� 0, ���� ������� CARRIER_DETECT ���, *
                                       *   ����� ���������� 1                           */
  virtual unsigned char isCarrierDetected() = 0;
                                      /* ���������� �� ����� ���                        */
  virtual void txbit(unsigned int bit) = 0;
  virtual unsigned char rxbit() = 0;  /* ��������� � ����� ���                          */

  void rxSync();                      /* ���������� rx-���������������                  */
  void txSync();                      /* ���������� tx-���������������                  */

private:
  stat_events*  stath;                /* ���������� ����������                          */
  CIRC_BUF128   inbuf;                /* �������� �����                                 */
  unsigned long lostblocks;           /* ������� ���������� ������� (����� ��� ������,  */
                                      /*   �� ��� ����� �� ��������)                    */
  unsigned char carrierHystCntr;      /* ������� ����������� ������� CARRIER_DETECT     */

#define CMX469EVT_TXCOMPLETE  0x01
#define CMX469EVT_RXCOMPLETE  0x02
  eventpack      epack;               /* ����� ������� ��������                         */

  int            txerr;               /* ��������� ����������� ������                   */
  unsigned char* txbuf;               /* ������� ����� ��� ��������                     */
  unsigned long  txbufsz;             /* ������ �������� ������ ��� ��������            */
  unsigned long  txbufrest;           /* ���������� ��� �� ������������ ������          */
  callback2*     txcompleter;         /* ���������� ���������� ����������� ������       */
  unsigned char  txpreambling;        /* 1 - ���� �������� ���������, ������� txbufrest */
                                      /*   �������� ���������� ���������� ��� ��������  */
                                      /*   ���� ���������                               */
  unsigned char  txbits;              /* ������� ������������ ���                       */
  unsigned long  txbitstream;         /* ������� ������ �� ��������                     */

  int            rxerr;               /* ��������� ������������ ������                  */
  unsigned char* rxbuf;               /* ������� �������� �����                         */
  unsigned long  rxbufsz;             /* ������ �������� ��������� ������               */
  unsigned long  rxbufread;
  callback2*     rxcompleter;         /* ���������� ���������� ������������ ������      */
  unsigned char  rxpreambling;        /* 1 - ���� ����� ���������, ����� �� ����������� */
  unsigned char  rxbits;              /* ������� �������� ���                           */
  unsigned long  rxbitstream;         /* ������� ������ �� �����                        */

                                      /* ��������� ����� ��� �������� ����� ���������   */
                                      /*   ��������                                     */
  void finalizeTx(int bytesSent, int status);
                                      /* ��������� ����� ��� �������� ����� ���������   */
                                      /*   ������                                       */
  void finalizeRx(int bytesRead, int status);
                                      /* ��������� �����, ������������ ��� timed-������ */
  void onTimedWriteComplete(int bytesSent, int err);
                                      /* ��������� �����, ������������ ��� timed-������ */
  void onTimedReadComplete(int bytesRead, int err);

                                      /* ��������� �� �����                             */
  static const unsigned long rxpreamble;
                                      /* ��������� �� ��������                          */
  static const unsigned char txpreamble[5];
};

#endif /*_CMX469_HPP_*/
