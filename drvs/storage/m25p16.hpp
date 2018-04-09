#ifndef _M25P16_HPP_
#define _M25P16_HPP_

#include "extstorage.hpp"


#define WIP     0x01                  /* ��� WIP (write in progress) � �������� ������� */


class m25p16
  : public extstorage
{
public:
                                      /* ���������                                      */
  static const int TOTAL_SIZE = 2097152;/* ������ ���������� � ������                   */
  static const int PAGE_COUNT = 8192;   /* ���������� �������                           */
  static const int PAGE_SIZE  = 256;    /* ������ �������� � ������                     */
  static const int SECTOR_SIZE= 65536;  /* ������ ������� � ������                      */


  m25p16(extstorageCreationDisposition* escd);
  virtual ~m25p16();

  virtual const STORAGEGEOM* geometry(){return &this->geom;}

  virtual int write(unsigned long addr, unsigned char* data, unsigned long size);
  virtual int read (unsigned long addr, unsigned char* data, unsigned long size);
  virtual int erase(unsigned long addr, unsigned long size);
  virtual int size ();
  virtual unsigned long JEDECId();

protected:
  virtual void chip_select(unsigned char ic, bool select = true) = 0;

private:
  static const STORAGEGEOM geom;

  serialPort*   low;
  unsigned char low_count;
  unsigned long full_size;

  void write_enable (unsigned char ic);
  void write_disable(unsigned char ic);
  void waitready    (unsigned char ic);
};

extern const ATOMGEOM m25p16_geom[];

#endif /*_M25P16_HPP_*/
