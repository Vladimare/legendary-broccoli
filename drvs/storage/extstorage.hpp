#ifndef _EXTSTORAGE_HPP_
#define _EXTSTORAGE_HPP_

#if defined(__ARMCC_VERSION)
# include "serial.hpp"
#endif

/************************ ���� ������ ��� ������ � ����-������� *************************/

                                      /* ����� ��������, ���������  ��� �����           */
#define GEOMF_PROGRAM     0x01        /* ��������� ���������������� (���� 1->0)         */
#define GEOMF_ERASE       0x02        /* ��������� �������� (���� 0->1)                 */
#define GEOMF_WRITE       0x04        /* ��������� ������ (�������� + ����������������) */

#define ES_WREN           0x06        // ���������� ������
#define ES_WRDI           0x04        // ������ ������

#define ES_RDID           0x9F        // ������ ��������������

#define ES_RDSR           0x05        // ������ �������� ���������
#define ES_WRSR           0x01        // ������ �������� ���������
 
#define ES_RD             0x03        // ������
#define ES_RDF            0x0B        // ������� ������

#define ES_PW             0x0A        // ������ ��������
#define ES_PP             0x02        // ���������������� ��������

#define ES_PE             0xDB        // �������� ��������

#define ES_BE             0xC7        // ������ ��������
#define ES_BE4            0x20        // �������� ����� 4K
#define ES_BE32           0x52        // �������� ����� 32K
#define ES_SE             0xD8        // �������� �������


                                      /* �������� ����� ����-������                     */
typedef struct tagATOMGEOM
{
  unsigned long size;                 /* ������ ����� � ������                          */
  unsigned long count;                /* ����� ���������� ������ � ������               */
  unsigned long flags;                /* ����� �����                                    */
}ATOMGEOM;

                                      /* �������� ��������� ����-������                 */
typedef struct tagSTORAGEGEOM
{
  const ATOMGEOM* atoms;              /* ������ ������                                  */
  unsigned long   atomsCount;         /* ���������� ������ � ������                     */
}STORAGEGEOM;

#if defined(__ARMCC_VERSION)
                                      /* ������, ����������� ��� �������� ��������      */
                                      /*   ���������� ����-������                       */
struct extstorageCreationDisposition
  : public basicCreationDisposition
{
  serialPort*   phy;                  /* ������ �������������� �����������, �������     */
  unsigned char phy_count;            /*   ���������� ������ �������                    */
};

#endif

                                      /* ��������� ��� ������ � ����-�������            */
class extstorage
{
public:
  virtual const STORAGEGEOM*  geometry() = 0;
  virtual int write(unsigned long addr, unsigned char* data, unsigned long size) = 0;
  virtual int read (unsigned long addr, unsigned char* data, unsigned long size) = 0;
  virtual int erase(unsigned long addr, unsigned long size) = 0;
  virtual int size ()             = 0;
  virtual unsigned long JEDECId() = 0;
};

/************** ���� ��� ��������� ����������������� ������ � ����-������� **************/

#define SS_SIGNATURE_LENGTH 3         /* ����� ��������� ���������                      */

/* �������� ��������� ������ �� ���� (structured storage info) */
typedef struct tagSS_INFO
{
#define SSTYPE_UNDEFINED    0         /* ��� �� ���������, ������� �� ���������������   */
#define SSTYPE_CIRCULAR     1         /* ����������� ����� (����� circbuf.*)            */
#define SSTYPE_RAW          2         /* ����� � ������������ ��������, ��������� ������*/
                                      /*   ���������� ������������                      */
#define SSTYPE_TREE         3         /* ������ n-�� �������                            */
#define SSTYPE_TABLE        4         /* ������� ���������������� ������� � �������� �� */
                                      /*   �������                                      */
#define SSTYPE_SETTINGS     5         /* ��������� ��������                             */
#define SSTYPE_MEMMAN       6         /* �������� ������                                */
#define SSTYPE_UDB          7         /* �� �������������                               */

#define SSTYPE_RWFLASH_FLASH  8       /*                                                */
#define SSTYPE_RWFLASH_EEPROM 9       /*                                                */


  unsigned char type;                 /* ��� ���������                                  */
  unsigned char reserved[3];          /* ������, ������������ �� ������� 4 ����         */
  unsigned long record_size;          /* ������ ������ �������� ��������� � ������      */
  unsigned long block_size;           /* ������ ����� � ������, ��� ����� ������ ����   */
                                      /*   ���������� �������� ��������                 */
  unsigned short block_start;         /* ���������� ����� �����, � �������� ����������  */
                                      /*   ��������� � ������, ��������� � 0            */
  unsigned short block_count;         /* ������ ��������� � ������                      */
}SS_INFO;


extern const char* es_JEDECDescription(unsigned long id);
extern const unsigned char ss_signs[][SS_SIGNATURE_LENGTH];


#endif /*_EXTSTORAGE_HPP_*/
