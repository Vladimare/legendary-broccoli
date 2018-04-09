#include <string.h>

#include "blackbox.hpp"
#include "errors.h"
#include "rtc17xx.hpp"
#include "syslog.hpp"


blackbox::blackbox()
{
}

blackbox::~blackbox()
{
  delete this->cbuf;
  delete this->intbuf;
}

int blackbox::init(extstorage* es, const SS_INFO& ssi, const ATOMGEOM& ag)
{
  this->cbuf    = new circularBuffer(es, ssi, ag);
  this->intbuf  = new unsigned char[lifesignal_base::LSBIN_MAX_SZ + sizeof (unsigned long)];

  return this->cbuf->init();
}

int blackbox::addEntry(unsigned char* entry, unsigned long* number)
{
  int err = ERR_OK;

  this->rwd.number = 0;               /* ������� ������ ��� ������                      */
  this->rwd.pdata  = this->intbuf;

  unsigned long utc = 0;
  if(rtc17xx::g_rtc)
  {                                   /* ���� ����������                                */
    err = rtc17xx::g_rtc->getRTC(&utc, 0);
    if((err != ERR_OK) && (err != ERR_DEVICE_NOSYNC))
      return err;
  }

                                      /* ��������� ����� � ������                       */
  memcpy(this->intbuf, (unsigned char *) &utc, sizeof(unsigned long));
  memcpy(this->intbuf + sizeof (unsigned long), entry, lifesignal_base::LSBIN_MAX_SZ);
                                      /* �����                                          */
  err = this->cbuf->write_entry(&this->rwd);
  if(err != ERR_OK)
  {
    if(syslog::g_log)
      syslog::g_log->write(LOG_LEVEL_ALERT, "blackbox: failed to add entry (err == %d)\r\n", err);

    return err;
  }

  if(number)
    *number = this->rwd.number;       /* ���� ���� ��������� �� �����, �������� � ����  */
                                      /*   ����� ����� ������                           */

  if(syslog::g_log)
    syslog::g_log->write(LOG_LEVEL_FLOW, "blackbox: entry %010d added\r\n", this->rwd.number);

  return ERR_OK;
}

int blackbox::getEntry(unsigned char* entry, unsigned long  number, unsigned long* utc)
{
  int err = ERR_OK;

  this->rwd.number = number;          /* ������� ������ ��� ������                      */
  this->rwd.pdata  =  this->intbuf;
                                      /* ������                                         */
  err = this->cbuf->read_entry(&this->rwd);
  if(err != ERR_OK)
    return err;
                                      /* ��������� �����, ���� ���������                */
  if(utc)
    memcpy((unsigned char*)utc, this->intbuf, sizeof(unsigned long));
                                      /* �������� ������ �� ��������� �� ����� �������  */
  memcpy(entry, this->intbuf + sizeof(unsigned long), lifesignal_base::LSBIN_MAX_SZ);

  return ERR_OK;
}

/* �������� ����� ������ ���������� ��������� � ������� 
  ����������� ����� ����� ��������� ����� 2-� ������� �������
  ��������, ������ �������� 2 3 4 8 9 10
  ������ tbegin = 5 ������ ������ ��������� ������ ������ 8,
  ��� ��� 5 ��������� ����� 4 � 8 
  */
int blackbox::findBegin(unsigned long tbegin, unsigned long* value)
{
  unsigned long first, last, center;
  unsigned long utcFirst, utcLast, utcCenter;
  int ret;

                                      /* �������������� ��������� �����                 */
  first  = firstEntry();
  last   = lastEntry();
                                      /* ������ ��������� �������� ������� �� ��������  * 
                                       * ��� ��� �� ������� �������� ���� ������        * 
                                       * ���� ����������� ����� ������� ��������� ������*/
  while (1)
  {
                                      /* �� ������ ���������� ����� ����������          */
    if(this->breakFlag)
      return ERR_TERMINATED;
                                      /* �������� �������� ������� � ������ ���������   */
    ret = this->getEntry(intbuf, first, &utcFirst);
    if (ret < ERR_OK) 
      return ret;

                                      /* �� ������ ���������� ����� ����������          */
    if(this->breakFlag)
      return ERR_TERMINATED;
                                      /* �������� �������� ������� � ����� ���������    */
    ret = this->getEntry(intbuf, last, &utcLast);
    if (ret < ERR_OK) 
      return ret;
                                      /* ��������� ������������ ������ ���������        */
    if ((tbegin <utcFirst) || (utcLast < tbegin))
      return ERR_SS_ENTRY_NOTEXIST;

                                      /* ��������� �� ��������� ����������, �������� ���* 
                                       * ������� � �� ������ ������� � ����             */
    if (utcFirst == tbegin)
    {
      *value = first;
      return ERR_OK;
    }
    if (utcLast == tbegin)
    {
      *value = last;
      return ERR_OK;
    }

                                      /* ��������� ������� ���������� �����             */
    if ((last - first) <= 1)
    {
                                      /* �������� �� ������� �������� � ��������?       */
      if ((utcFirst < tbegin) && (tbegin < utcLast))
      {
        *value = last;
        return ERR_OK;
      }
      else
        return ERR_SS_ENTRY_NOTEXIST;
    }
    else
    {                                 /* ��������� ����� ��������                       */

                                      /* ������ �������� ������������ ��������          */
      center = first + (last - first) / 2;

                                      /* �� ������ ���������� ����� ���������           */
      if(this->breakFlag)
        return ERR_TERMINATED;
                                      /* �������� �������� ������� � �������� ��������� */
      ret = this->getEntry(intbuf, center, &utcCenter);
      if (ret < ERR_OK) 
        return ret;

                                      /* �������� ����� �������� ���������� �����       */
                                      /* ������������ � ��������� ��������              */
      if (utcCenter < tbegin)
      {
        first = center;
      }
      else
        last = center;
    }
  } 
}

/*  */
int blackbox::getEntryStop()
{
  this->breakFlag = 1;                /* ������� ����������� ���������� ������� �����   */
  return ERR_OK;
}

/*  */
int blackbox::getEntry(unsigned long tbegin, unsigned long tend, callback2* entry_receiver)
{
  if (this->totalEntries() == 0) return ERR_SS_EMPTY;
  
  unsigned long first, last, i, utc;
  int ret;
                                      /* ���� ������ ����������, �� ��� ������� ��      * 
                                       * ����������� ����������                         */
  this->breakFlag = 0;
                                      /* ���� ������ ���������                          */
  ret = this->findBegin(tbegin, &first);
  if (ret < ERR_OK) return ret;

                                      /* ���������� ��������� ��������                  */
  last = lastEntry();
                                      /* �������� �������                               */
  for (i = first; i <= last; i++)
  {
                                      /* �� ������ ���������� ����� ���������           */
    if(this->breakFlag)
      return ERR_TERMINATED;
                                      /* ��������� ������� ������                       */
    this->rwd.number = i;
    this->rwd.pdata  = this->intbuf;
    ret = this->cbuf->read_entry(&this->rwd);
    if(ret != ERR_OK)
      return ret;
                                      /* ��������� �����                                */
    memcpy((unsigned char *) utc, this->intbuf, sizeof(unsigned long));
                                      /* ��������� ��������� ���������� ���������       */
    if (utc > tend) break;
                                      /* �������� ��������� �������� � callback         */
    if (entry_receiver)
      (*entry_receiver)((unsigned long) (intbuf + sizeof(unsigned long)), i);
  }

  return ERR_OK;
}
