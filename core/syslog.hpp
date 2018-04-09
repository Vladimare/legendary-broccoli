#ifndef _SYSLOG_HPP_
#define _SYSLOG_HPP_


#define LOG_LEVEL_DISABLED      0     // ������ ���� ���������
#define LOG_LEVEL_CRITICAL      1     // ����������� ������ (���� ������, ����������, ...)
#define LOG_LEVEL_ERROR         2     // ������, �� �� �������������� (������������ ������������, ...)
#define LOG_LEVEL_ALERT         3     // ������, �� �������������� (��� ����� � ��������, ...)
#define LOG_LEVEL_WARNING       4     // �������������� (������ / �������������� ����� � ��������, ...)
#define LOG_LEVEL_INFO          5     // �������������� ��������� (�����, ����, ����������)
#define LOG_LEVEL_USER          6     // ������� �������������� �������������
#define LOG_LEVEL_HDATA         7     // ������ ������ - �������/������/������� (�� � ������)
#define LOG_LEVEL_PROCESS       8     // ������ ���������� - ��������/��������, ��������� ����-���� (�� � ������/�������)
#define LOG_LEVEL_LDATA         9     // ������ ������� ������ - �������/������/������� (������������ �� ���� � �.� �� � ������)
#define LOG_LEVEL_FLOW          10    // �����, ������, �� ��� ������������ ���
#define LOG_LEVEL_FLOW_DATA     11    // ��� ������ � ������ � �.�
#define LOG_LEVEL_SYNC          12    // ���������/������ � ��������� �������������, �������� � �.�
#define LOG_LEVEL_DEBUG         13    // �������������� ���������� ����������
#define LOG_LEVEL_DBG_ALL       14    // ������ ��� ������ (������� ������ ������ � �.�)

#ifdef __cplusplus

#include <stdarg.h>

#if defined(__ARMCC_VERSION)
# include "mutex.hpp"
# include "serial.hpp"
#endif


//#define SYSLOG_NOFLOAT                /* ���� ���������� ��� ���������, �� ������       */
                                      /*   ������� ������� printf() ����� ��������������*/
                                      /*   _printf() (��� ��������� ��������� �����),   */
                                      /*   ��� ��������� ����������� �� ������� ����    */

class syslog
{
public:

#if defined(__ARMCC_VERSION)
  syslog(unsigned int v, serialPort* sp);
#endif

  syslog(unsigned int v);
  ~syslog();

  int write   (unsigned int v, const char* fmt, ...);
  int writeva (unsigned int v, const char* fmt, va_list args);
  int writebuf(unsigned int v, const unsigned char* buf, unsigned long length);

#if defined(__ARMCC_VERSION)
  int read    (unsigned char* c);
  static int sendchar(int ch);
#endif

  void changeVerbosity(unsigned int new_v);

  static syslog* g_log;

  static const char* log_level_names[];

private:
  unsigned int verbosity;

#if defined(__ARMCC_VERSION)
  serialPort* port;
  mutex       synclock;
#endif
};

extern "C"
{

#endif /*__cplusplus */

extern int g_writelog(unsigned int v, const char* fmt, ...);

#ifdef __cplusplus
}
#endif

#endif /*_SYSLOG_HPP_*/
