#ifndef _SWHEADERS_H_
#define _SWHEADERS_H_

#ifdef __cplusplus
extern "C"
{
#endif

#include "types.h"
#include <stdint.h>
#include <time.h>

/* ��������� ��������� �������� ��������� */
typedef __packed struct tagMainSWheader
{
  uint32_t signature;                 /* ��������� ���������                  */
  uint32_t revision;                  /* ������ ���������                     */
  uint32_t size;                      /* ������ ���������                     */
  uint32_t crc;                       /* ����������� ����� ���������          */
  time_t time;                        /* ���� � ����� ���������� ����������   *
                                       * ���������                            */
  char* descriptor;                   /* ��������� �������� ���������         */
}MainSWheader;

/* ��������� ��������� ���������� */
typedef __packed struct tagBootSWheader
{
uint32_t ID;                          /* ���������� ������������� ����������  */
uint32_t revision;                    /* ������ ���������                     */
uint32_t size;                        /* ������ ���������                     */
uint32_t crc;                         /* ����������� �����                    */
time_t time;                          /* ���� ���������� ���������� ����������*/
char* descriptor;                     /* ��������� �������� ���������         */
uint64_t MAC;                         /* ������� ����� ����������             */
uint32_t hardware;                    /* ������ ���������� ���������, �����   *
                                       * ������ �����                         */
}BootSWheader;


#define MAIN_SW_OFFSET    (0x8000)
#define BOOT_SW_OFFSET    (0x0)

#define SW_HEADER_OFFSET  (0x200)

#define bootSWheader ((BootSWheader*) (BOOT_SW_OFFSET + SW_HEADER_OFFSET))
#define mainSWheader ((MainSWheader*) (BOOT_SW_OFFSET + SW_HEADER_OFFSET))

#ifdef __cplusplus
}
#endif


#endif /*_SWHEADERS_H_*/
