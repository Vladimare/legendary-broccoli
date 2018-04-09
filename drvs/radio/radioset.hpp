#ifndef _RADIOSET_HPP_
#define _RADIOSET_HPP_

#include "types_ext.hpp"


/* ��������� ������ � �������������� */
class radioset
{
public:
  /* ���������� �����, ����������� ��� ���������� ������������ � �������� */
  virtual unsigned long transmitReadyDelaySec() = 0;
  virtual unsigned long transmitReadyDelayMsec()= 0;

  /* ���������� ��������� ������������, ����� ��������� ���������         */
  /*   ���������� ���������� ac                                           */
  virtual int  turnOn(callback0* ac) = 0;

  /* ���������� ���������� ������������, ����� ��������� ����������       */
  /*   ���������� ���������� ac                                           */
  virtual int  turnOff(callback0* ac) = 0;

  /* ���������� true, ���� ������������ �������� */
  virtual bool isTurnedOn() = 0;

  /* ���������� ��������: ����������� ������, ���������� ������ PTT,  *
   *   ����� ���� �������� ���������� ����� �������� ������           */
  virtual int initTransmit(callback1* sender) = 0;
  /* ������������ ��������: ������� ������ PTT, ����������� ������,   *
   *   �����������, ��� ����� ������������� �������� �� �����         */
  virtual int finalizeTransmit() = 0;
};

#endif /*_RADIOSET_HPP_*/
