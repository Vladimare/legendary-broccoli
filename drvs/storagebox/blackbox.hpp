#ifndef _BLACKBOX_HPP_
#define _BLACKBOX_HPP_

#include "circbuf.hpp"

#include "upiobo_data.hpp"

class blackbox
{
public:
  blackbox();
  ~blackbox();

  /* �������������� ������ ���� */
  int init(extstorage* es, const SS_INFO& ssi, const ATOMGEOM& ag);

  /* ��������� ������ � ������ ����, ���������� ��� ������                          *
   * ���� ��������� number ���������, �� ���������� ����� �����, ����������� ������ */
  int addEntry(unsigned char* entry, unsigned long* number = 0);

  /* ��������� �� ������� ����� ������ � ��������� �������, ���������� ��� ������ */
  /* ���� ��������� utc ���������, �� ���������� ����� ����� ������ � ������ ���� */
  int getEntry(unsigned char* entry, unsigned long  number, unsigned long* utc = 0);

  /* ��������� ����� ������� �� ������� ����� �� ���������� ���������� ��������� */
  int getEntry(unsigned long tbegin, unsigned long tend, callback2* entry_receiver);

  /* ��������� ���������� ������ ������ ������� �� ������� */
  int getEntryStop();

  /* ���������� ����� ���������� ������� � ������� */
  __inline unsigned long totalEntries() { return this->cbuf->getInfo()->rec_count;}

  /* ���������� ����� ������ ������ � ������� */
  __inline unsigned long firstEntry()   { return this->cbuf->getInfo()->rec_first;}

  /* ���������� ����� ��������� ������ � ������� */
  __inline unsigned long lastEntry()    { return (this->firstEntry() + this->totalEntries() - 1);}

private:
  circularBuffer* cbuf;

  CIRCBUF_RWDATA rwd;

  /* ���������� ����� */
  unsigned char* intbuf;

  /* ������� ����������� ���������� */
  int breakFlag;

  /* �������� ����� ������ ������ ���������� ��������� */
  int findBegin(unsigned long tbegin, unsigned long* value);
};

#endif /*_BLACKBOX_HPP_*/
