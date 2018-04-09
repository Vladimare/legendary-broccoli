#include <string.h>

#include "at24c512.hpp"
#include "errors.h"
#include "lpc17xx_defs.h"

  //********************************* ��������!!!!! *****************************
  //************* ������� ������������ ������ ���� ���������� EEPROM ************

const ATOMGEOM at24c512_geom[] =
{
  {1, at24c512::TOTAL_SIZE, GEOMF_WRITE}, /* �����, ����� ���� �������� */
};

const STORAGEGEOM at24c512::geom = {at24c512_geom, sizeof(at24c512_geom) / sizeof(ATOMGEOM)};


at24c512::at24c512(extstorageCreationDisposition* escd)
{
  this->low       = escd->phy;
  this->low_count = escd->phy_count;
                                       /* ����� ����� ��������� ������                   */
  this->full_size = TOTAL_SIZE * this->low_count;
}

at24c512::~at24c512()
{
}

/* ���������� ������ � ������, ������� � ������ addr (����� ����������� � ������)                 *
 * ���� ������ ���������� ������� ��������, �� ������������� ��������� �� ��������� ��������      *
 * ���� ������ ���������� ������� ����������, �� ������ �� ������������ � ������������ ������     *
 * ��������� ���������� �� ��������� ������                                                       */
int at24c512::write(unsigned long addr, unsigned char* data, unsigned long size)
{
  unsigned long rest_to_page_end;      // ����� ���� �� ����� ��������
  unsigned long cur_addr_ic = addr;    // ������� ����� � ����������
  unsigned long cur_addr_buf = 0;      // ������� ����� � ������ ������
  int res = ERR_OK;                    // ������������ �������� ���������
  unsigned long sz = 0;                // ����� ������������ ����� ������� �2� ������


  if(!size)
    return ERR_OK;                    /* ������ ����������                              */

  if(addr >= this->full_size)
    return ERR_ADDRESS;               /* ����� ������� �� ������� ��������� ������      */

  if((addr + size) > this->full_size)
    return ERR_DATA_SIZE;             /* ������ �� ���������� � ��������� ������        */

                                      /*   ������                                       */


  while(size)                        // ����� ���� ���� ������
  {
    if(size < MAX_LENGTH_DATA_TO_WRITE)  // ���� �������� ���� ������ ��� ������ ������
      sz = size;                         // �� ����� ���������� �� ��� ��������
    else
      sz = MAX_LENGTH_DATA_TO_WRITE;     // ����� �������� ����� �������

    rest_to_page_end = PAGE_SIZE - cur_addr_ic % PAGE_SIZE;            // ������� ������� �������� �� ����� ��������

    if(rest_to_page_end < sz)                                          // ���� ����� �� ���������
      sz = rest_to_page_end;                                           // �� ������� ������� ����

    this->ucWriteBuffer[0] = EEPROM_ADDRESS;                           // ������� ���������
    this->ucWriteBuffer[1] = (cur_addr_ic >> 8) & 0xff;
    this->ucWriteBuffer[2] =  cur_addr_ic       & 0xff;
    memcpy(&this->ucWriteBuffer[3], &data[cur_addr_buf], sz);          // ���������� ������ ��� ������

    res = this->low->write(this->ucWriteBuffer, sz+3);                 // ���������� � �����
    if(res < 0)                                                  // ���� ���� ������, �� �� �����
      return res;

    this->WaitReady();

    cur_addr_buf += sz;                                                // ��������� � ������� ������������� ����� ������
    cur_addr_ic += sz;                                                 // ��������� � ���������� ������
    size -= sz;                                                        // ��������� ����� ������������ ���� �� �������� ����������
                                                                       // ���������� ����
 }

  return ERR_OK;
}

/* ��������� ������ �� ������, ������� � ������ addr (����� ����������� � ������)                 *
 * ���� ������ ���������� ������� ��������, �� ������������� ��������� �� ��������� ��������      *
 * ���� ������ ���������� ������� ����������, �� ������ �� ������������ � ������������ ������     *
 * ��������� ���������� �� ��������� ������                                                       */
int at24c512::read(unsigned long addr, unsigned char* data, unsigned long size)
{
  unsigned long rest_to_page_end;      // ����� ���� �� ����� ��������
  unsigned long cur_addr_ic = addr;    // ������� ����� � ����������
  unsigned long cur_addr_buf = 0;      // ������� ����� � ������ ������
  int res = ERR_OK;                    // ������������ �������� ���������
  unsigned long sz = 0;                // ����� ������������� ����� ������� �2� ������

  if(!size)
    return ERR_OK;                    // ������ ���������

  if(addr >= this->full_size)
    return ERR_ADDRESS;               // ����� ������� �� ������� ��������� ������

  if((addr + size) > this->full_size)
    return ERR_DATA_SIZE;             // ������ �� ���������� � ��������� ������


  while(size)                        // ������ ���� ���� ������
  {
    rest_to_page_end = PAGE_SIZE - cur_addr_ic%PAGE_SIZE;

    if(size < rest_to_page_end)
      sz = size;
    else
      sz = rest_to_page_end;

    data[cur_addr_buf] = EEPROM_ADDRESS;
    data[cur_addr_buf+1] = (cur_addr_ic>>8)&0xff;
    data[cur_addr_buf+2] = cur_addr_ic & 0xff;
    res = this->low->write(&data[cur_addr_buf], 3);
    if(res < 0)                                                  // ���� ���� ������, �� �� �����
      return res;
    res = this->low->read(&data[cur_addr_buf], sz);
    if(res < 0)                                                  // ���� ���� ������, �� �� �����
      return res;

    this->WaitReady();

    cur_addr_buf += sz;
    cur_addr_ic += sz;
    size -= sz;
 }
  return ERR_OK;
}

/* ������� ��������� ������� ������                                     */
int at24c512::erase(unsigned long addr, unsigned long size)
{
  unsigned long rest_to_page_end;      // ����� ���� �� ����� ��������
  unsigned long cur_addr_ic = addr;    // ������� ����� � ����������
  int res = ERR_OK;                    // ������������ �������� ���������
  unsigned long sz = 0;                // ����� ������������ ����� ������� �2� ������


  if(!size)
    return ERR_OK;                    /* ������ ����������                              */

  if(addr >= this->full_size)
    return ERR_ADDRESS;               /* ����� ������� �� ������� ��������� ������      */

  if((addr + size) > this->full_size)
    return ERR_DATA_SIZE;             /* ������ �� ���������� � ��������� ������        */

                                      /*   ������                                       */


  while(size)                        // ����� ���� ���� ������
  {
    if(size < MAX_LENGTH_DATA_TO_WRITE)  // ���� �������� ���� ������ ��� ������ ������
      sz = size;                         // �� ����� ���������� �� ��� ��������
    else
      sz = MAX_LENGTH_DATA_TO_WRITE;     // ����� �������� ����� �������

    this->ucWriteBuffer[0] = EEPROM_ADDRESS;                           // ������� ���������
    this->ucWriteBuffer[1] = (cur_addr_ic>>8)&0xff; 
    this->ucWriteBuffer[2] = cur_addr_ic & 0xff;
    memset(&this->ucWriteBuffer[3], EEPROM_CLEAR_VALUE, sz);          // ���������� ������ ��� ������

    rest_to_page_end = PAGE_SIZE - cur_addr_ic%PAGE_SIZE;              // ������� ������� �������� �� ����� ��������

    if(rest_to_page_end < sz)                                          // ���� ����� �� ���������
      sz = rest_to_page_end;                                           // �� ������� ������� ����

    res = this->low->write(this->ucWriteBuffer, sz+3);                 // ���������� � �����
    if(res < 0)                                                  // ���� ���� ������, �� �� �����
      return res;

    this->WaitReady();


    cur_addr_ic += sz;                                                 // ��������� � ���������� ������
    size -= sz;                                                        // ��������� ����� ������������ ���� �� �������� ����������
                                                                       // ���������� ����
 }

  return ERR_OK;
}

int at24c512::size()
{
  return this->full_size;
}

unsigned long at24c512::JEDECId()
{

  return 0;
}

void at24c512::WaitReady()
{
  ucWriteBuffer[0] = EEPROM_ADDRESS; 
  while(this->low->write(this->ucWriteBuffer, 1) < 0)
  {
  }
}
