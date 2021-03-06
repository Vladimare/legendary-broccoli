#ifndef __CFGTREE__HPP__
#define __CFGTREE__HPP__

#include "extstorage.hpp"

                                      /* ���� ����������� �� �� ��� ����      *
                                       * ���������������� ������ �����������  *
                                       * � ���                                */
#define   CFG_CACHED (0x80)

                                      /* ��������� ����������� 1 ���� ������  *
                                       * ������������ ��� �������� ������     */
typedef struct tagCFGENTRY
{
  unsigned char blk_number;
  unsigned char blk_flags;            /* 7 ��� - ������� ������������ � ���   */
                                      /* 1 - �����������, 0 - ���             */
                                      /* 2 �� ���� - �������                  */
  unsigned short blk_sz;
} CFGENTRY;


/* ����� ��� �������� � ������� � ���������� ���������� � ����������������� ���� 
�������� ��� ���
1. "ID"
1.1 "USER"
1.2 "PASS"
1.2.1 "MIN LEN"
1.2.2 "TRY COUNT"
1.3 "CHANNEL 2 HOST"
1.3.1 "RF"
1.3.1.1 "BASE FREQ"
1.3.1.2 "BAND WITDH"
1.3.1.3 "POWER"
1.3.1.4 "DURATION"
1.3.1.5 "MIN RSSI"
2. "41"
3. "PHONE NUM"

���� ������������ ����� ��������� ������������ ����� ������ ������������� ������,
������ ����� ������ �������� ��� ������������� � ������������� ��� ������� �����
����� ���������� � ������� ������� � ���� ��.��.��.��, ������ ����������� ��� 
unsigned long, ������� ������ � ������� ������. 
�������� ������ ������������ "RF" 0x00010301

��� ������ ������������ ��������� ������ ��������� ����� ������� ������������� 
����������� ������ � ���. �������� ������ � ������� - �������� ������, ������ 
������ �� ������, ������ ������ ���������� ������������ � ����� � �� ������� 
��������. � ��� ����������� ������ ������, ��� ��������� � ����������� �����

������ ����� �� ������� ������ ����� ��������� ���������
[0] - ����� �����
[1] - ��. ���� ������ ����� (N)
 
[2].0:4 - ��. ����� ������ ����� ������
[2].5:6 - ������� �����.
[2].8 - ������� ������������ � ���

[3 + N] - ���� ������, ����� �������������.

CRC16 ����������� ��� ���� � 0 �� 3 + N
[N + 3] - CRC16 ��. ����
[N + 3 + 1] - CRC16 ��. ����

*/
class cfgtree
{
public:
  cfgtree(const SS_INFO& ssi, extstorage* estrg);
  ~cfgtree();

                                      /* ������������� ������ ������������   */
  int init(CFGENTRY* headers, int size);

                                      /* ������ ����� ������������           */
  int read(unsigned long index, unsigned char* buf, int bufsz);

                                      /* ������ ����� ������������           */
  int write(unsigned long index, unsigned char* buf, int bufsz);
private:

  SS_INFO     layout;                 /* �������� ������� �������� ��������  * 
                                       * ������������ ��� �������� ������ ����*/

  extstorage* carrier;                /* ��������� ��� ������� � ��������    */
                                      /* �������� ������                     */

                                      /* ���������� ��������� ����������� 1  *
                                       * ���� ������ ������������            */
  typedef struct tagCFGHDR
  {
    unsigned char number;             /* ����� ����� �� ��������� ������     */
    unsigned char lev;                /* ������� ����������� (1 - 4)         */
    unsigned short size;              /* ������ ����� ������                 */

    unsigned char * cache;            /* ��������� �� ����� ����� ������     */
                                      /* �� �� ������� ������������� ������. */

    unsigned long lookup;             /* �������� ����� ������ �� �������    *
                                       * ��������, ����������� �� ������     *
                                       * ��������, � �� ������               */
  } CFGHDR;

  CFGHDR* bHeaders;                   /* ������� ���������� ������ ����������.*/
  int bCount;                         /* ���������� ������� � ������ ������. */
  unsigned char* bufInt;              /* ���������� ����� ��� ������ � �������*/
                                      /* � �������� ���������                */

  int index2num(unsigned long index); /* ����� ������ ����� �� �������       */
  int packHeadr(int num);             /* ������������ ���������, ��� ������  */
                                      /* �� ������� ��������                 */
};

#endif //__CFGTREE__HPP__
