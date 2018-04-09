#ifndef _CMX469_560_HPP_
#define _CMX469_560_HPP_

#include "cmx469.hpp"


/* ���������� ������ cmx469 �� ����� 560 (�����-3) */
class cmx469_560
  : public cmx469
{
public:
  cmx469_560(stat_events* se);
  virtual ~cmx469_560();

protected:
                                      /* ����� �������������� ��������� ��� ����������� */
  virtual void pinSetup(bool setDefault = false);
                                      /* ���������� / ������ ��������� ����������       */
  virtual void rxEnable(unsigned char enable = 1);
                                      /* ���������� / ������ ����������� ����������     */
  virtual void txEnable(unsigned char enable = 1);
                                      /* ���������� 0, ���� ������� CARRIER_DETECT ���, *
                                       *   ����� ���������� 1                           */
  virtual unsigned char isCarrierDetected();
                                      /* ���������� �� ����� ���                        */
  virtual void txbit(unsigned int bit);
  virtual unsigned char rxbit();      /* ��������� � ����� ���                          */

private:
  static void txSyncIrq(void* instance);
  static void rxSyncIrq(void* instance);
};

#endif /*_CMX469_560_HPP_*/
