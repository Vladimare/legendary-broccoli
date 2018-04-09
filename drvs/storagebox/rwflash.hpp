#ifndef _RWFLASH_HPP_
#define _RWFLASH_HPP_

#include "extstorage.hpp"

                                      /* ������� ���������� ��� ������ � ����-�������    */

//#define RWFLASH_DEBUG


#define RWFLASH_PAGE_HEADER_SIZE 4       // ������ ��������� �������� � flash ������ (� ������� ��� rwflash)

#define RWFLASHF_BE   0x01               // ����� - "���� (������) ����": 1 - ����; 0 - �� ����
#define RWFLASHF_PE   0x02               // ����� - "������� �����": 1 - �����; 0 - �� �����
#define RWFLASHF_PA   0x04               // ����� - "�������� ���������": 1 - ���������; 0 - �� ���������


class rwflash
  : public extstorage
{
public:

  rwflash(extstorage* es_flash,  const SS_INFO& ssi_flash,  const ATOMGEOM& ag_flash,
          extstorage* es_eeprom, const SS_INFO& ssi_eeprom, const ATOMGEOM& ag_eeprom);
  virtual ~rwflash();

  virtual int init();

  virtual STORAGEGEOM*  geometry() {return &this->geom;};
  virtual int write(unsigned long addr, unsigned char* data, unsigned long size);
  virtual int read (unsigned long addr, unsigned char* data, unsigned long size);
  virtual int erase(unsigned long addr, unsigned long size);
  virtual int size ();
  virtual unsigned long JEDECId();


  virtual int defragmentation();            // ������� �������������� ����� Flash ������

  #ifdef RWFLASH_DEBUG
    void ShowFlashMemory(unsigned long addr, unsigned long size);
    void ShowEepromMemory(unsigned long addr, unsigned long size);
  #endif

  unsigned long current_block_number;          // ����� �������� ������������� �����
  unsigned long current_empty_page_number;     // ����� ������� ������ ��������
  unsigned long accessible_page_count;         // ����� ��������� �������
  unsigned long actual_page_count;             // ����� ���������� �������
  unsigned long notactual_page_count;          // ����� ������������ �������


private:
  STORAGEGEOM geom;                        // ��������� flash
  ATOMGEOM    rwflash_geom;


  unsigned char* flPageBuff;                // ����� � ��� ��� �������� �������� Flash ������
  extstorage* es_fl;                        // ��������� �� flash
  extstorage* es_ee;                        // ��������� �� eeprom

  // ���������� � ��������� FLash
  unsigned long  fl_block_size;           /* ������ ����� � ������, ��� ����� ������ ���� ���������� �������� �������� */
  unsigned short fl_block_count;          /* ������ ��������� � ������ */


  unsigned long g_fl_size;                     // ������ ���������� �������� flash ������

  unsigned long RWFLASH_FLASH_START_ADDRESS;   // ����� �� flash ������ ������� rwflash
  unsigned long RWFLASH_EEPROM_START_ADDRESS;  // ����� � eeprom ������ ������� rwflash
  unsigned long RWFLASH_EEPROM_SIZE;           // ������ ������� rwflash � eeprom

};

#endif /*_RWFLASH_HPP_*/
