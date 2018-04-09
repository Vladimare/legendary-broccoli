#ifndef _GPS_HPP_
#define _GPS_HPP_

#include <math.h>
#include <time.h>

#include "event.hpp"
#include "mutex.hpp"
#include "serial.hpp"
#include "thread.hpp"
#include "types_ext.hpp"


#define GPS_RECEIVE_TIMEOUT 500       /* ������� �� ����� ������� �� GPS-������, ����   *
                                       *   ���������� �������                           */
#define GPS_VALID_TIMEOUT   3000      /* �������, ����� ������������ �������� ������    *
                                       *   GPS ��������� ����������                     *
                                       * ������ �������� ������������ �� ���������      *
                                       *   �������� ������� GPS                         *
                                       * ������� �������� � ����� ���������� �������    */

                                      /* ��������� ������ RMC-�������                   */
struct rmcData
{
           char source;               /* �������� �������                               */
                                      /*   'P' - ������ GPS                             */
                                      /*   'L' - ������ �������                         */
                                      /*   'N' - ��������������� ������                 */
           char valid;                /* ������ ������:                                 */
                                      /*   '�' - ������ ����������                      */
                                      /*   'V' - ������ ������������                    */
           char mode;                 /* ����� ������:                                  */
                                      /*   'A' - ����������                             */
                                      /*   'D' - ����������������                       */
                                      /*   'E' - �������������                          */
                                      /*   'N' - ������������� ������                   */
  unsigned long date;                 /* ���������� ���� date ������� ���������� �� rmc */
                                      /*   �������, � �������������� � ���� ����������� */
                                      /*   �����. ��������, � 21 ���� �������� 211230   */
                                      /*   ����� ��������������� 21 ������� 2030 ����   */
                                      /* ��� double ��������� ���������� �� rmc-������  */
                                      /*   � ��� ���������, ��� ��������                */
         double time;                 /* ����� �������� ��������������                  */
         double latitude;             /* ������, ������������� �������� �������������   */
                                      /*   ��������� ���������                          */
         double longitude;            /* �������, ������������� �������� �������������  */
                                      /*   ������ ���������                             */
         double speed;                /* �������� � �����, ��� � RMC-�������            */
         double trackAngle;           /* ������� ����                                   */
         double declination;          /* ��������� ��������� (����� �� ������)          */
};


class gps
  : public thread
{
public:
  gps();
  virtual ~gps();

  void init(serialPort* port, unsigned long pclk);


  void getGPS(rmcData* gd)
  {
    this->synclock.lock(TN_WAIT_INFINITE);
    *gd = this->actualGPS;
    this->synclock.unlock();
  }

  unsigned long getUTC()
  {
    this->synclock.lock(TN_WAIT_INFINITE);

    double        t = this->actualGPS.time;
    unsigned long d = this->actualGPS.date;

    this->synclock.unlock();

    return GPS2UTC(d, t);
  }

  static unsigned long GPS2UTC(unsigned long date, double time)
  {
    struct tm dt;
    unsigned long inttime = floor(time);

    dt.tm_sec =   inttime % 100;
    dt.tm_min =  (inttime % 10000) / 100;
    dt.tm_hour=   inttime / 10000;
    dt.tm_mday=   date / 10000;
    dt.tm_mon = ((date % 10000) / 100) - 1;
    dt.tm_year=  (date % 100) + 2000 - 1900;

    return mktime(&dt);
  }


  void setCallback_onGPSUpdated(callback0* cb)
  {
    this->synclock.lock(TN_WAIT_INFINITE);
    this->gpsUpdated = cb;
    this->synclock.unlock();
  }

  void setCallback_onGPSValid(callback0* cb)
  {
    this->synclock.lock(TN_WAIT_INFINITE);
    this->gpsValid = cb;
    this->synclock.unlock();
  }

  void setCallback_onGPSInvalid(callback0* cb)
  {
    this->synclock.lock(TN_WAIT_INFINITE);
    this->gpsInvalid = cb;
    this->synclock.unlock();
  }

                                      /* ���������� ���������� � ������ ����� �����     */
                                      /*   ������� �� ������ �����������                */
                                      /* ����� �������� � ������� rmc                   */
  static unsigned long distance (double lat1, double long1, double lat2, double long2);
                                      /* ��������� �������� � ����� � �������� � �� / � */
  static unsigned long speed    (double speed_miles);

  static double rad(double a);        /* �������������� �������������� ���������� �     */
                                      /*   ������� rmc � �������                        */

                                      /* ������ ����� � ������ (40000000 / (2 * pi))    */
  static const int    EARTH_RADIUS= 6371000;
  static const double PI;

protected:
  virtual void run();

private:
  bool          isValid;              /* true - ���� ���������� ���������               */
  mutex         synclock;             /* ������ ��� ������������� ������� � ������ ������ */
  serialPort*   sourcePort;           /* ����-�������� NMEA-�������                     */

  bool          isValidPacket;        /* ������� ������ ���������� RMC-�������          */
  unsigned long baudrateTimer;        /* ������ �� ����� �������� ������                */
  unsigned long baudrateStartAt;      /* �������� systimer, ����� ��� ����� ����        */
                                      /*   baudrateTimer                                */
  unsigned char baudrateIndex;        /* ������ � ������� baudrates, ���������������    */
                                      /*   ������� ��������                             */

#define GPS_CHAR_RECEIVED   0x01      /* ������ ��������� ������                        */
  eventpack     events;               /* ������� gps-�������                            */

#define NMEAFS_UNDEFINED        0x00  /* ��������� �� ����������                        */
#define NMEAFS_HEADER           0x01  /* ������ ����������� ������ '$', ����� ��������� */
#define NMEAFS_TIME             0x02  /* ����� ������� �������� ���������               */
#define NMEAFS_VALID            0x03  /* ����� ����������                               */
#define NMEAFS_LATITUDE         0x04  /* ����� ������                                   */
#define NMEAFS_LATSPHERE        0x05  /* ��������� �� ������                            */
#define NMEAFS_LONGITUDE        0x06  /* ����� �������                                  */
#define NMEAFS_LONGSPHERE       0x07  /* ��������� �� �������                           */
#define NMEAFS_SPEED            0x08  /* ����� �������������� ������������ ��������     */
#define NMEAFS_TRACKANGLE       0x09  /* ����� �������� ����                            */
#define NMEAFS_DATE             0x0A  /* ����� ����                                     */
#define NMEAFS_DECLINATION      0x0B  /* ����� ���������� ���������                     */
#define NMEAFS_DECLINATION_SIGN 0x0C  /* ����� ����� ���������� ���������               */
#define NMEAFS_MODE             0x0D  /* ����� ������ ������                            */
#define NMEAFS_CRC              0x0E  /* ������ ����������� ������ '*', ����� CRC       */
  unsigned char nmeastate;            /* ��������� �������� ������� NMEA-������         */
  unsigned char nmeabuf[32];          /* ����� ��� NMEA-������                          */
  int           nmeacount;            /* ���������� ������ � ������                     */
  unsigned char nmeachecksum;         /* ����������� �����                              */

  rmcData       actualGPS;            /* ������� ���������� ������ GPS                  */
  rmcData       tempGPS;              /* ������������� ������ GPS, ������������ ���     */
                                      /*   ���������� �������� ��� ������� NMEA-������� */

                                      /* ����������� ����������� �� GPS-�������         */
  callback0*    gpsUpdated;           /* ������ ����������                              */
  callback0*    gpsValid;             /* ���������� ��������������                      */
  callback0*    gpsInvalid;           /* ���������� ����������                          */

  static const threadCreationDisposition tcd;

  void onCharReceived(int count, int err);
  bool gpsParseStep(unsigned char c); /* ��� �������, ���������� true, ���� �� ���������*/
                                      /*   ���� ���������� ���������� ������ GPS        */
  void processDataToken();

#define MARKER_COUNT    3             /* ������ �������������� ��������                 */
  static const char* markers[MARKER_COUNT];

#define BAUDRATE_COUNT  4             /* ������ �������������� ��������� ������ �       */
                                      /*   GPS-�������, ��� ��������������� ����������� */
                                      /*   ��������                                     */
  static const unsigned long baudrates[BAUDRATE_COUNT];
};

#endif /*_GPS_HPP_*/
