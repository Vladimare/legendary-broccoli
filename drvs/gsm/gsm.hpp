#ifndef _GSM_HPP_
#define _GSM_HPP_


#include "mutex.hpp"
#include "serial.hpp"
#include "thread.hpp"
#include "event.hpp"
#include "systimer.h"

//��������
#define GSM_AT          "AT"
#define GSM_ECHO        "E"
#define GSM_DIAL        "D"
#define GSM_HANGUP      "H"
#define GSM_BAUDRATE    "+IPR"
#define GSM_MANUFACTUR  "+CGMI"
#define GSM_MODEL       "+CGMM"
#define GSM_SOFT_VER    "+CGMR"
#define GSM_IMEI        "+CGSN"
#define GSM_INFO        "I"
#define GSM_DEFAULT     "&F"
#define GSM_SHUTDOWN    "^SMSO"
#define GSM_SIM_STATE   "^SCKS"
#define GSM_SIM_ID      "^SCID"
#define GSM_PIN         "+CPIN"
#define GSM_SET_PIN     "+CLCK"
#define GSM_CHANGE_PIN  "+CPWD"
#define GSM_REG         "+CREG"
#define GSM_SQ          "+CSQ"
#define GSM_SMS_URC     "+CNMI"
#define GSM_SMS_FORMAT  "+CMGF"
#define GSM_SMS_LIST    "+CMGL"
#define GSM_SMS_DEL     "+CMGD"
#define GSM_SMS_RD      "+CMGR"
#define GSM_SMS_WR      "+CMGS"
#define GSM_PDP_CONT    "+CGDCONT"
#define GSM_TO_CMD      "+++"
#define GSM_TO_DATA     "ATO"
#define GSM_GPRS_REG    "+CGREG"

#define GSM_DATA_SHUTDOWN   "MS OFF"

//URC ��������������� ���� ����������
#define GSM_URC_START       "^SYSSTART"
#define GSM_URC_SHUTDOWN    "^SHUTDOWN"
#define GSM_URC_RING        "RING"
#define GSM_URC_NET_REG     "+CREG"
#define GSM_URC_GPRS_REG    "+CGREG"
#define GSM_URC_SMS         "+CMTI"
#define GSM_URC_SIM         "^SCKS"
#define GSM_URC_OK          "OK"
#define GSM_URC_ERROR       "ERROR"
#define GSM_URC_CONNECT     "CONNECT"
#define GSM_URC_NO_CARRIER  "NO CARRIER"
#define GSM_URC_NO_DIALTONE "NO DIALTONE"
#define GSM_URC_BUSY        "BUSY"
#define GSM_URC_ALERTING    "ALERTING"
#define GSM_URC_DIALING     "DIALING"

//������������
#define GSM_SIM_PIN           "SIM PIN"
#define GSM_SIM_PUK           "SIM PUK"
#define GSM_SIM_PIN2          "SIM PIN2"
#define GSM_SIM_PUK2          "SIM PUK2"
#define GSM_SIM_READY         "READY"

//SMS
#define GSM_SMS_UNREAD        "REC UNREAD"
#define GSM_SMS_READ          "REC READ"
#define GSM_SMS_UNSENT        "STO UNSENT"
#define GSM_SMS_SENT          "STO SENT"
#define GSM_SMS_ALL           "ALL"

#define GSM_RESP_STEP_ECHO    0
#define GSM_RESP_STEP_DATA    1
#define GSM_RESP_STEP_CODE    2
#define GSM_RESP_STEP_SMS_TXT 3

#define GSM_SIM_REMOVED       0
#define GSM_SIM_INSERTED      1

#define GSM_AUTH_READY        0
#define GSM_AUTH_PIN          1
#define GSM_AUTH_PUK          2
#define GSM_AUTH_PIN2         3
#define GSM_AUTH_PUK2         4

#define GSM_PIN_UNBLOCK       0
#define GSM_PIN_BLOCK         1
#define GSM_PIN_STATE         2

#define GSM_REG_NOT           0
#define GSM_REG_HOME          1
#define GSM_REG_PROGRESS      2
#define GSM_REG_DENIED        3
#define GSM_REG_UNKNOWN       4
#define GSM_REG_ROAMING       5

#define GSM_SMS_FORMAT_PDU    0
#define GSM_SMS_FORMAT_TXT    1

#define GSM_CID_MAX_NUM       2


#define GSM_CR        '\r' //������� �������
#define GSM_LF        '\n' //����� ������
#define GSM_STR_SEP   '#'  //����������� �����
#define GSM_STR_FRAME '"'  //���������� ������
#define GSM_PARAM_SEP ','  //����������� ����������
#define GSM_SMS_INV   '>'  //����������� ����� ������ ������������ ���

//��������
#define GSM_CMD_TIMEOUT       100 //����� ����� ����������� ��������� 100��
#define GSM_RX_CHAR_TIMEOUT   (100  / SYSTIMER_PERIOD)//������� �� ����� ������� 100��
#define GSM_RX_LINE_TIMEOUT   (1000 / SYSTIMER_PERIOD)//������� �� ����� ������ 1000��
#define GSM_RX_HANDLE_TIMEOUT (1000 / SYSTIMER_PERIOD)//������� �� ��������� ��������� ������ 1000��
#define GSM_TX_LINE_TIMEOUT   (1000 / SYSTIMER_PERIOD)//������� �� �������� 1000��
#define GSM_CONNECT_TIMEOUT   (10000 / SYSTIMER_PERIOD)//������� �� �����������
#define GSM_RX_SZ        128 //������ ������ �� �����
#define GSM_TX_SZ        64  //������ ������ �� ��������

#define GSM_SMS_FROM_SZ   16
#define GSM_SMS_TIME_SZ   24


//����������� ����� ������� GSM
class gsm_events
{
public:
  gsm_events();
  virtual ~gsm_events();
  virtual int OnSysStart() = 0;   //��������� ���������� ������
  virtual int OnShutDown() = 0;   //���������� ���������� ������
  virtual int OnRing() = 0;       //�������� ������
  virtual int OnSMS(int num) = 0; //����� ���
  virtual int OnChangeNetState(int state) = 0; //��������� ��������� ����������� � ����
  virtual int OnChangeSIMSlotState(int state) = 0; //��������� ��������� ����� ��� �����
};

//����� ������ � GSM �������
class gsm
  : public thread
{
public:
  gsm(rs232Port *port, unsigned long pclk);
  virtual ~gsm();
  bool  getATReady();//��������� �������� ���������� ��������� AT ��������
  void  setATReady(bool on = true);//��������� ��� ����� �������� ���������� ��������� AT ��������
  int bind_events(gsm_events* events);//�������� ������������ �������
  int at(); //������� �������� ����� � gsm �������
  int setBaudrate(unsigned int baudrate=BR_115200); //��������� �������� ����� � gsm �������
  int enableSMSURC(); //��������� URC ��� ������ ������ sms ���������
  int disableSMSURC(); //����������� URC ��� ������ ������ sms ���������
  int enableRegURC(); //��������� URC ��� ��������� ��������� ����������� � ����
  int disableRegURC(); //����������� URC ��� ��������� ��������� ����������� � ����
  int enableSIMURC(); //��������� URC ��� ��������� ��������� ����� SIM �����
  int disableSIMURC(); //����������� URC ��� ��������� ��������� ����� SIM �����
  int getManufacturer(char *str);
  int getModel(char *str);
  int getSoftVer(char *str);
  int getIMEI(char *str);
  int getInfo(char *str);
  int setDefaults();
  int shutDown();
  int getSIMState();
  int getSIMId(char *str);
  int getPINAuthState();
  int enterPIN(unsigned int pin);
  int enterPUK(unsigned int puk, unsigned int new_pin);
  int getPINState();
  int setPIN(unsigned int pin);
  int clrPIN(unsigned int old_pin);
  int changePIN(unsigned int old_pin, unsigned int new_pin);

  int  getNetworkRegistration();
  int  getGPRSRegistration();
  bool isGSMRegistered();

  int getSignalQuality();
  int setSMSFormat(unsigned int format = GSM_SMS_FORMAT_TXT);
  int getSMSFormat();
  int deleteSMS(int idx);
  int readSMS(int idx, char *from, char * time, char *text);
  int writeSMS(char *to, char *text);
  int setPDP(unsigned int cid, char *apn = "", char *addr = "");
  int connectGPRS(unsigned int cid);
  int disconnectGPRS();
  int switchCmdMode();
  int switchDataMode();


protected:
  rs232Port* port;
  virtual void run();

private:
  static const threadCreationDisposition tcd;

  char          rx[GSM_RX_SZ]; //����� �� �����
  char          tx[GSM_TX_SZ]; //����� �� ��������

#define GSM_RX_FSM_URC  0 //��� ����� ����� ��������� ������ URC
#define GSM_RX_FSM_ECHO 1 //��� �������� ���
#define GSM_RX_FSM_RESP 2 //��� ������ ������
  unsigned char rx_fsm;   //������� ������
#define GSM_MODE_CMD    0
#define GSM_MODE_DATA   1
  unsigned char mode;     //����� ����� � GSM �������
  unsigned long ips;  //��������� ����� ��� ������������ �����
  bool          at_ready; //������� ���������� ��������� AT ��������
  unsigned char net_reg; //��������� ����������� � ����
  int           err;  //������ ��� �������� ����� ��������

#define GSM_EP_RX_LINE  0x01 //������� ������
#define GSM_EP_RX_FULL  0x02 //����� �������� ������ ������
#define GSM_EP_RX_EMPTY 0x04 //������� ������ ������
#define GSM_EP_RX_FREE  0x08 //�������� ����� ��������
#define GSM_EP_RX_ERR   0x10 //������ ��� ������
  eventpack     ep;   //������� ��� ������������� ������� ������/�������� ������ gsm ������

  gsm_events*   events;   //��������� �� ����� � ������������� �������
  mutex         synclock; //������ ��� ������������� ������� � ������ ������

  int readln(int timeout = GSM_RX_CHAR_TIMEOUT);
  int write(int sz);
  int getResponseCode();
  int getResponseString(char *str);
  int getURC();
  int setEcho(bool on = true);
  int excludeCmd(char *str, const char *cmd);
  int excludeParam(char *str);
  int getIntParam(char *str);
  int getStringParam(char *to, char *from);
};
#endif /*_GSM_HPP_*/
