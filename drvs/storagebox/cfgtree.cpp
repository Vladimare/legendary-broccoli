#include "cfgtree.hpp"
#include "errors.h"
#include "corelib.h"
#include <string.h>

/* ����������� */
cfgtree::cfgtree(const SS_INFO& ssi, extstorage* estrg)
{
  this->bufInt  = 0;
  this->bHeaders= 0;
  this->bCount	= 0;

  this->layout  = ssi;
  this->carrier = estrg;
}

/* ���������� */
cfgtree::~cfgtree()
{
  if (this->bufInt != NULL) 
    delete  this->bufInt;

  if (this->bHeaders != NULL)
    for(int i = 0; i < this->bCount; i++)
      if (this->bHeaders[i].cache != NULL)
        delete this->bHeaders[i].cache;
}

/* ������������� ������ ������������ � ��������� ����� ������ */
int cfgtree::init(CFGENTRY* headers, int size)
{
  int i, offset, ret = ERR_OK, maxSize;

  this->bHeaders  = new CFGHDR[size];

  this->bCount    = size;
                                      /* ��������� ��������                   */
  offset = layout.block_size * (layout.block_start);
  maxSize = 0;

                                      /* ��������� ���������� �������         *
                                       * ����������                           */
  for( i = 0; i < size; i++)
  {
    this->bHeaders[i].number  = headers[i].blk_number;
                                      /* ��������� �������� ��� ������� ����� */
    this->bHeaders[i].lookup  = offset;
    this->bHeaders[i].size    = headers[i].blk_sz;
    this->bHeaders[i].lev     = 1 + (headers[i].blk_flags & 3);
    offset += this->bHeaders[i].size + 5;/* ���� ��������� � CRC16            */

                                      /* ���. ������������ ������ ����� ������*/
    if (maxSize  < this->bHeaders[i].size) 
      maxSize = this->bHeaders[i].size;

                                      /* ����������� ������ � ��� ������ ���  *
                                       * ��� ������ ������� �������           *
                                       *  ������������                        */
    if(CFG_CACHED & headers[i].blk_flags)
      this->bHeaders[i].cache = new unsigned char [bHeaders[i].size];
    else
      this->bHeaders[i].cache = NULL;
  }
                                      /* ���� ������ + CRC + ���������        */
  this->bufInt = new unsigned char[maxSize + 3 + 2];


                                      /* �������� ���������� ������           */
  for( i = 0; i < size; i++)
  {
                                      /* ������ ����                          */
    ret = this->carrier->read(this->bHeaders[i].lookup, bufInt, this->bHeaders[i].size + 5);
    if (ret < ERR_OK) return ret;

    unsigned short currentBsize;
    currentBsize = bufInt[1];
    currentBsize |= ( 0x1f & bufInt[2]) << 8;
                                      /* �������� ������                      */
    if (currentBsize != this->bHeaders[i].size)
    {
      ret = ERR_DATA_CORRUPTED;
      break;
    }

    unsigned char  lev;
    lev = 1 + (0x3 & (bufInt[2] >> 5));
                                      /* �������� ������ �����������          */
    if (lev != this->bHeaders[i].lev)
    {
      ret = ERR_DATA_CORRUPTED;
      break;
    }

                                      /* �������� ������ �����                */
    if (bufInt[0]  != this->bHeaders[i].number)
    {
      ret = ERR_DATA_CORRUPTED;
      break;
    }
                                      /* ���������� ����������� �����         */
    unsigned short crc, crcStrored;

    crc = crc16(bufInt, 3 + currentBsize, 0);

    crcStrored = bufInt[3 + currentBsize];
    crcStrored |= bufInt[3 + currentBsize + 1] << 8;

                                      /* �������� ����������� �����           */
    if (crc != crcStrored)
    {
      ret = ERR_DATA_CORRUPTED;
      break;
    }
                                      /* ���� ���� ������� ������������, ��   * 
                                       * �������� �� ���������� �����         */
    if(this->bHeaders[i].cache != NULL)
      memcpy(this->bHeaders[i].cache, &this->bufInt[3], this->bHeaders[i].size);
  }
  return ret;
}

/* ����� ������ ����� �� ������� */
int cfgtree::index2num(unsigned long index)
{
  int i, j, lev;
  unsigned long curIndex = 0;

  for( i = 0; i < this->bCount; i++)
  {
                                      /* ��������� ������ ��� �������� �����  */
    lev = this->bHeaders[i].lev;
                                      /* �������� ��� �� ������� ��������     *
                                       * ������                               *
                                       * ������� ������ � ������� ������.     */
    for( j = 0; j <= (4 - lev) ; j++)
      curIndex &= ~(0xff << (8 * (3 - j)));
                                      /* ��������� ������� �������� ������    */
    curIndex |= (this->bHeaders[i].number << (8 * (lev - 1)));

    if (curIndex == index)            /* ������ ?                             */
      return i;                       /* ����� ���������� ������              */
  }
  return ERR_ADDRESS;                 /* ������ � ��������� �������� ���      */
}

/* ������������ ���������, ��� ������ �� ������� ��������  */
int cfgtree::packHeadr(int num)
{
  this->bufInt[0] = this->bHeaders[num].number;
  this->bufInt[1] = (unsigned char)this->bHeaders[num].size;
  this->bufInt[2] = 0x1f & (this->bHeaders[num].size >> 8);
  this->bufInt[2] |= (this->bHeaders[num].lev - 1) << 5;
  if (this->bHeaders[num].cache != NULL)
    this->bufInt[2] |= 0x80;
  return ERR_OK;
}

/* ������ ������������ 
�������� ����� �� ������ ����� ������ ���� �� ����� ������, 
�� ������� ������ ����������� ����������.
��� bufsz = 0 ����� �� ����������, � �������� ����������� ����� */
int cfgtree::read(unsigned long index, unsigned char* bufExt, int bufsz)
{
  int bN, ret;
  CFGHDR* curHeader;

  bN = index2num(index);
  if (bN < ERR_OK)
    return bN;                        /* ����                                 */

  curHeader = &this->bHeaders[bN];

  if (bufsz > curHeader->size)        /* ������������ ������                  */
    bufsz = curHeader->size;

  if(curHeader->cache != NULL)
  {
                                      /* ���� ������ ����������� � ���, �� �� *
                                       * �� ����� ��������������              */
    memcpy(bufExt, curHeader->cache, bufsz);
  }else
  {
                                      /* ������ ��������� ����                */
    ret = this->carrier->read(curHeader->lookup, bufInt, curHeader->size + 5);
    if (ret < ERR_OK) return ret;
                                      /* ���������� ����������� �����         */
    unsigned short crc, crcStrored;

    crc = crc16(bufInt, 3 + curHeader->size, 0);

    crcStrored = bufInt[3 + curHeader->size];
    crcStrored |= bufInt[3 + curHeader->size + 1] << 8;

                                      /* �������� ����������� �����           */
    if (crc != crcStrored)
    {
      return ERR_DATA_CORRUPTED;
    }
                                      /* ������ ���������, ������� �������    *
                                       * ������                               */
    memcpy(bufExt, &this->bufInt[3], bufsz);
  }
  return ERR_OK;
}

/* ������ ������������ */
int cfgtree::write(unsigned long index, unsigned char* bufExt, int bufsz)
{
  int blockNumber, ret;
  CFGHDR* curHeader;

  blockNumber = index2num(index);
  if (blockNumber < ERR_OK)
    return blockNumber;               /* ����                                 */

  curHeader = &this->bHeaders[blockNumber];

  if (bufsz > curHeader->size)
  {                                   /* ������ �����, ������ - ���           */
    return ERR_DATA_SIZE;
  }

  this->packHeadr(blockNumber);       /* �������� ���������                   */

                                      /* �������� ���������� ����� ������     */
  memcpy(&this->bufInt[3], bufExt, bufsz); 

                                      /* ���� ���������� ����� ������ �����   */
                                      /* ������, ���������� �������           */
  if (bufsz != curHeader->size)
  {
    ret = this->carrier->read(
            curHeader->lookup + 3 + bufsz,
            bufInt,
            curHeader->size - bufsz);
    if (ret < ERR_OK) return ret;
  }

                                      /* ������������� CRC                    */
  unsigned short crc = crc16(bufInt, 3 + this->bHeaders[blockNumber].size, 0);

                                      /* ��������� ����� CRC � ����� ������   */
  bufInt[3 + curHeader->size]     = (unsigned char)crc;
  bufInt[3 + curHeader->size + 1] = crc >> 8;

                                      /* �����, ����� ��� - ������, ��������� */
                                      /* � ����������� �����                  */
  ret = this->carrier->write(curHeader->lookup, bufInt, curHeader->size + 5);
  if (ret < ERR_OK) return ret;

  return ERR_OK;
}
