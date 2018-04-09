#ifndef _CIRCBUF_HPP_
#define _CIRCBUF_HPP_

#include "extstorage.hpp"

/* ������ ������������ ������ */
typedef struct tagCIRCBUF_INFO
{
  unsigned char valid;                /* ����� ����������, ����������� ���              */
  unsigned long block_first;          /* ����� ������� ����������� ����� � ���������    */
  unsigned long rec_first;            /* ����� ������ ���������� ������ � ���������     */
  unsigned long rec_count;            /* ������� ���������� ������� � ������            */
  unsigned short rec_block_count;     /* ���������� �������, ������������ � ����        */
}CIRCBUF_INFO;

/* ��������� ����� ������������ ������ */
typedef struct tagCIRCBUF_BLOCKHDR
{
                                      /* ��������� ���������                            */
  unsigned char signature[SS_SIGNATURE_LENGTH];
  union
  {
    struct
    {
      unsigned char nfirst: 1;        /* 0 - ������ ���� �������� �������               */
      unsigned char nfull : 1;        /* 0 - ���� �����                                 */
    }bits;
    unsigned char byte;
  }flags;                             /* ����� ���������                                */
  unsigned long rec_first;            /* ����� ������ ������ � �����                    */
  SS_INFO       ssi;                  /* �������� ������������ ������ ��� ���������     */
  unsigned short cs[4];               /* ������ ����������� ����                        */
                                      /* ����������� ����� �������������� (� �������    */
                                      /*   � ������) ���� ��� ��� ���� ���������� ����� */
                                      /*   nfirst � nfull:                              */
                                      /*     nfull  nfirst  cs                          */
                                      /*       0       0     0                          */
                                      /*       0       1     1                          */
                                      /*       1       0     2                          */
                                      /*       1       1     3                          */
}CIRCBUF_BLOCKHDR;

/* ��������� �������� ������������ ������ */
typedef struct tagCIRCBUF_ENTRYHDR
{
                                        /* �������� ������������� ������                */
#define CIRCBUF_ENTRYHDR_UNUSED 0xFFFF  /* ������ ��������                              */
#define CIRCBUF_ENTRYHDR_USED   0x0000  /* ������ ������                                */
  unsigned short uses;                  /* ������� ������������� ������                 */
  unsigned short cs;                    /* ����������� ����� ������                     */
}CIRCBUF_ENTRYHDR;

/* ������ ��� �������� ������ / ������ */
typedef struct tagCIRCBUF_RWDATA
{
  unsigned long number;               /* �����, ����������� ��� ������, ��� ����������� */
                                      /*   ��� ������                                   */
  unsigned char* pdata;               /* ��������� �� ������                            */
}CIRCBUF_RWDATA;


class circularBuffer
{
public:
  circularBuffer(extstorage* es, const SS_INFO& ssi, const ATOMGEOM& bg);
  ~circularBuffer();

  CIRCBUF_INFO* getInfo() {return &this->hdr;}

  int init();                         /* �������������� �����, ��������� ������ �� ����,*/
                                      /*   ��� ������������� (����)������� �����        */
  int read_entry(CIRCBUF_RWDATA* rwd);/* ��������� ��������� ������                     */
                                      /* ��������� ������ � ������                      */
  int write_entry(CIRCBUF_RWDATA* rwd);

private:
  extstorage*       sphy;             /* ��������� ������� � ���������                  */
  const SS_INFO&    ssinfo;           /* �������� ���������                             */
  const ATOMGEOM&   blockgeom;        /* ��������� �����                                */
  CIRCBUF_BLOCKHDR  blockhdr;         /* ��������� ������                               */
  CIRCBUF_INFO      hdr;              /* ��������� ������                               */
  unsigned char*    entrybuf;         /* ���������� ����� ��� �������� ������           */


  int validate();                     /* ��������� ������������ ������                  */
  int create();                       /* ������� �����                                  */
                                      /* ��������� ������������ �����                   */
  int validate_blockhdr(unsigned long block);
                                      /* ������� ����                                   */
  int create_blockhdr(unsigned long block, unsigned char first, unsigned long rf);
                                      /* ��������� ��������� �����                      */
  int update_blockhdr(unsigned long block, unsigned char first, unsigned char full, unsigned long rf);
                                      /* ��������� ������                               */
  int validate_entry(unsigned long block, unsigned long entry);
                                      /* ������� ������                                 */
  int create_entry(unsigned long block, unsigned long entry, unsigned char* pdata);
};

#endif /*_CIRCBUF_HPP_*/
