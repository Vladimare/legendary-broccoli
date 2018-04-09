#ifndef _GSM587_HPP_
#define _GSM587_HPP_

#include "gsm.hpp"


#define GSM_STARTUP_TIMEOUT 10000 
#define SIM1  0
#define SIM2  1

class gsm_587
  : public gsm
{
public:
  gsm_587(rs232Port *port, unsigned long pclk);
  virtual ~gsm_587();

  int  on (unsigned char simnum);     //��������� ������
  void off(void);                     //���������� ������
  void sim(unsigned char num = SIM1); //����� ������������ SIM �����

private:
  void pinSetup(bool setDefault = false);
};


#endif /*_GSM587_HPP_*/
