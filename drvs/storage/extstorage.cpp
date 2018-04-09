#include <string.h>

#include "compilerspec.h"
#include "extstorage.hpp"

                                      /* ��������� ����� ��������                       */
const unsigned char ss_signs[][SS_SIGNATURE_LENGTH] =
{
  {'X', 'X', 'X'},                    /* ��� �� ���������, ������� �� ���������������   */
  {'C', 'Y', 'C'},                    /* ����������� ����� (����� circbuf.*)            */
  {'R', 'A', 'W'},                    /* ����� � ������������ ��������, ��������� ������*/
                                      /*   ���������� ������������                      */
  {'T', 'R', 'E'},                    /* ������ n-�� �������                            */
  {'T', 'B', 'L'},                    /* ������� ���������������� ������� � �������� �� */
                                      /*   �������                                      */
  {'S', 'E', 'T'},                    /* ��������� ��������                             */
  {'M', 'A', 'N'}                     /* �������� ������                                */
};


#define ES_SUP_JEDEC_ID_COUNT   4
static unsigned long JEDEC_IDs[ES_SUP_JEDEC_ID_COUNT] =
{
  0x00152020,                         /* STM M25P16                                     */
  0x00162020,                         /* STM M25P32                                     */
  0x00182020,                         /* STM M25P128                                    */
  0x00148020                          /* STM M25PE80                                    */
};

static const char* JEDEC_strings[ES_SUP_JEDEC_ID_COUNT] =
{
  "STM, memtype M25P, capacity 16Mbit",
  "STM, memtype M25P, capacity 32Mbit",
  "STM, memtype M25P, capacity 128Mbit",
  "STM, memtype M25PE, capacity 8Mbit",
};


/* ������� ���������� ��������� ����������� JEDEC-�������������� flash-������ */
const char* es_JEDECDescription(unsigned long id)
{
  for(int i = 0; i < ES_SUP_JEDEC_ID_COUNT; i++)
  {                                   /* �������� ����� �� ������� ��������� JEDEC ID   */
    if(id == JEDEC_IDs[i])
      return JEDEC_strings[i];
  }

  return "unknown JEDEC id";
}
