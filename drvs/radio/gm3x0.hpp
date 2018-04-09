#ifndef _GM3x0_HPP_
#define _GM3x0_HPP_

#include "radioset.hpp"
#include "timer.hpp"


#define GM3x0_UP_OFF_RELAY_PERIOD 3000/* ������������ ���������� ���� ����������        */
                                      /*   �������� ������������ ��� ���������          */
                                      /*   ������������, � ��                           */
#define GM3x0_UP_READY_PERIOD     3000/* ����� ���������� ������������ � ������ �����   */
                                      /*   ��������� �������, � ��                      */
#define GM3x0_UP_REREADY_PERIOD   500 /* ������ ��������� �������� ����������           */
                                      /*   ������������ � ������, ���� ��� �� ������    */
                                      /*   ����� ��������� ��������� ������� ���������� */
#define GM3x0_DOWN_OFF_DELAY      1000/* �������� ����� ����������� ���� ����������     */
                                      /*   �������� ������������ ��� ����������         */
                                      /*   ������������, � ��                           */
#define GM3x0_DOWN_READY_PERIOD   1000/* ����� ���������� ������������, � ��            */
#define GM3x0_DOWN_REREADY_PERIOD 500 /* ������ ��������� �������� ����� ����������     */
                                      /*   ������������                                 */
#define GM3x0_CH1_OFF_DELAY       10  /* �������� ����������� ������� CH1 �����         */
                                      /*   ��������� ������� CH0, � ��                  */
#define GM3x0_PTT_ON_DELAY        190 /* �������� ��������� ������� PTT �����           */
                                      /*   ����������� ������� CH1, � ��                */
#define GM3x0_SEND_DELAY          10  /* �������� ������ �������� ����� ���������       */
                                      /*   ������� PTT, � ��                            */
#define GM3x0_CH0_OFF_DELAY       10  /* �������� ����������� ������� CH0 �����         */
                                      /*   ��������� ������� CH1, � ��                  */


class gm3x0
  : public radioset
{
public:
  gm3x0(timer* t, unsigned long pclk);
  virtual ~gm3x0();

  /*************************** ���������� ���������� radioset ***************************/
  virtual unsigned long transmitReadyDelaySec();
  virtual unsigned long transmitReadyDelayMsec();
  virtual int  turnOn(callback0* ac);
  virtual int  turnOff(callback0* ac);
  virtual bool isTurnedOn() = 0;      /* ���� ����� ������ ���� ��������� � ������ - �������*/
  virtual int  initTransmit(callback1* sender);
  virtual int  finalizeTransmit();

protected:
  virtual inline void onOffButtonBlock  () = 0;
  virtual inline void onOffButtonUnblock() = 0;

  virtual inline void onPower           () = 0;
  virtual inline void offPower          () = 0;

  virtual inline void activateCH0       () = 0;
  virtual inline void deactivateCH0     () = 0;
  virtual inline void activateCH1       () = 0;
  virtual inline void deactivateCH1     () = 0;
  virtual inline void activatePTT       () = 0;
  virtual inline void deactivatePTT     () = 0;

  virtual inline bool isVoiceChannel    () = 0;

private:
  timer*      tmr;
  callback1*  sender;
  callback0*  onCompleter;
  callback0*  offCompleter;

#define GM3x0F_POWERING_ON  0x00000001
#define GM3x0F_POWERING_OFF 0x00000002
#define GM3x0F_TRANSMITTING 0x00000004
  unsigned long flags;

  binder1<gm3x0> bindAdapter;         /* ��������������� ����� ��� ������������� �������*/
                                      /*   � �������� ������� ��������� ������ �������  */

  void upStage1Handler(int);          /* ������ ���� ��������� ������������             */
  void upStage2Handler(int);          /* ������ ���� ��������� ������������             */
  void downStage1Handler(int);        /* ������ ���� ���������� ������������            */
  void downStage2Handler(int);        /* ������ ���� ���������� ������������            */
  void transmitStage1Handler(int);    /* ������ ���� ���������� ��������                */
  void transmitStage2Handler(int);    /* ������ ���� ���������� ��������                */
  void finalizeStage1Handler(int);    /* ������ ���� ���������� ��������                */
};

#endif /*_GM3x0_HPP_*/
