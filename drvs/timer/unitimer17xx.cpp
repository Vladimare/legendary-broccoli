#include "unitimer17xx.hpp"
#include "core.h"
#include "errors.h"


//������ ������� �����.
const unsigned int unitimer17xx::masks[4] = {CHANNEL_1, CHANNEL_2, CHANNEL_3, CHANNEL_4};

//�����������
unitimer17xx::unitimer17xx(LPC_TIM_TypeDef* phy, const timerCreationDisposition& creationDisposition)
{
  //������������� �����.
  this->irqNum = creationDisposition.irqNumber;
  this->low = phy;
  this->pconpMask = creationDisposition.pwrMask;

  //����������� ���������� Timer.
  IRQ_DATA my_irq;
  my_irq.h = unitimer17xx::irqh;
  my_irq.instance = this;
  coreRegisterIRQ(irqNum, creationDisposition.irqPriority, &my_irq);

  //�������� ����� TN_EVENT
  this->evts.id_event = 0;
  tn_event_create(&this->evts, TN_EVENT_ATTR_MULTI, 0);

  //��������� ������� ������������� ����� Timer 0/1/2/3
  LPC_SC->PCONP |= this->pconpMask;

}


//����������
unitimer17xx::~unitimer17xx()
{
  coreUnregisterIRQ(irqNum);            //�������������� ����������
  tn_event_delete(&this->evts);         //�������� ����� TN_EVENT
  LPC_SC->PCONP &= ~(this->pconpMask);  //��������� ������� ������������� ����� Timer 0/1/2/3.
}


//������������� �������
int unitimer17xx::init(const timerSettings& ts)
{
  //������������� ����� ������
  this->ts = ts;

  //������������� �������
  this->low->PR = this->ts.prescalerValue;//�������� ������������
  this->low->IR = 0xff;                   //����� ����������

  //��������� �� �������
  for(unsigned char i=0; i<4; i++)
  {
    this->channelInit(i, ts.channels[i]);
  }

  return ERR_OK;
}

//���������� ��������� �������
timerSettings& unitimer17xx::settings()
{
  return this->ts;
}

//���������� ��������� ������ ������.
int unitimer17xx::channelInit(unsigned char channel, const timerChannelSettings& cs)
{
  unsigned int channelFlags = 0;
  this->low->MCR &= ~(0x01 << (MR0I + (channel * 3)));//��������� ���������� �� ������� ������

  (&(this->low->MR0))[channel] = cs.matchValue;//���������� �������� � ������� ����������

  //��������� �������� ����������
  if(cs.flags & RESET_ON_MATCH)
    channelFlags |= (0x01 << (MR0R + (channel * 3)));
  else
    channelFlags &= ~(0x01 << (MR0R + (channel * 3)));

  if(cs.flags & STOP_ON_MATCH)
    channelFlags |= (0x01 << (MR0S + (channel * 3)));
  else
    channelFlags &= ~(0x01 << (MR0S + (channel * 3)));

  if(cs.flags & ENABLED)
    channelFlags |= (0x01 << (MR0I + (channel * 3)));
  else
    channelFlags &= ~(0x01 << (MR0I + (channel * 3)));

  //���������� �����
  this->low->MCR |= channelFlags;

  return ERR_OK;
}

//��������� �������� ���������� �� ������.
int unitimer17xx::channelInit(unsigned char channel, unsigned long match)
{
  (&(this->low->MR0))[channel] = match;
  return ERR_OK;
}

int unitimer17xx::channelInitAddTC(unsigned char channel, unsigned long match)
{
  __disable_irq();
  (&(this->low->MR0))[channel] = this->low->TC + match;
  __enable_irq();

  return ERR_OK;
}

//���������� ��������� ������.
timerChannelSettings& unitimer17xx::channelSettings(unsigned char channel)
{
  return this->ts.channels[channel];
}

//�������� �����.
int unitimer17xx::channelEnable(unsigned char channel)
{
  this->low->MCR |= (0x01 << (MR0I + (channel * 3)));
  return ERR_OK;
}

//��������� �����
int unitimer17xx::channelDisable(unsigned char channel)
{
  this->low->MCR &= ~(0x01 << (MR0I + (channel * 3)));
  return ERR_OK;
}

//��������� ������
int unitimer17xx::start()
{
  this->low->TCR |= (0x01ul << CNTR_ENABLE);
  return ERR_OK;
}

//���������� ������. �������� ������� ��������� �� ��������.
int unitimer17xx::stop()
{
  this->low->TCR &= ~(0x01 << CNTR_ENABLE);
  return ERR_OK;
}

//�������� �������� ������� ���������.
int unitimer17xx::reset()
{
  this->low->TCR |= (0x01 << CNTR_RESET);
  this->low->TCR &= ~(0x01 << CNTR_RESET);
  return ERR_OK;
}

//��������� �������, ����� ������� ��������, � ������.
int unitimer17xx::restart()
{
  this->low->TCR = (0x01 << CNTR_ENABLE) | (0x01ul << CNTR_RESET);
  this->low->TCR = (0x01 << CNTR_ENABLE);
  return ERR_OK;
}

//���������� �������� �������� ��������.
unsigned long unitimer17xx::tickCount()
{
  return this->low->TC;
}

//��������� ��-�� �������� ������� ������������
unsigned long unitimer17xx::prescalerCount()
{
  return this->low->PC;
}

//���������� ������� �������� ������� �������
unsigned long unitimer17xx::timerFreq()
{
  return this->ts.pclk;
}

//��������� ���������� ���������� ������ �� ��������� ����� ��������.
int unitimer17xx::channelWait(unsigned char channel, unsigned long timeout)
{
  unsigned int flagsReg;

  this->low->MCR &= ~(0x01 << (MR0I + (channel * 3)));//��������� ���������� �� ������

  if(timeout < 4)
    return ERR_PARAM;

  __disable_irq();
  (&(this->low->MR0))[channel] = this->low->TC + timeout; //���������� ����������� ��������
  __enable_irq();

  tn_event_clear(&this->evts, ~(masks[channel])); //������� ����.
  this->low->MCR |= (0x01 << (MR0I + (channel * 3))); //��������� ���������� �� ������
  tn_event_wait(&this->evts, masks[channel], TN_EVENT_WCOND_AND, &flagsReg, TN_WAIT_INFINITE);//��������

  //���������� ����� ������
  this->low->MCR &= ~((0x01 << (MR0I + (channel * 3))) | (0x01 << (MR0R + (channel * 3))) | (0x01 << (MR0S + (channel * 3))));

  return ERR_OK;
}

//������ �� ����� ���������� ��� ������������.
int unitimer17xx::channelBind(unsigned char channel, callback1* cb)
{
  this->asyncCB[channel] = cb;

  return ERR_OK;
}

//���������� ����������.
void unitimer17xx::irqh(void* _this)
{
  unitimer17xx* timer = (unitimer17xx*)_this;
  unsigned long IRflag = timer->low->IR;

  timer->low->IR = 0xff;
  for(unsigned i = 0; i < 4; i++)
  {
    if(IRflag & (0x01ul << i))
    {
      tn_event_iset(&timer->evts, masks[i]);
      if(timer->asyncCB[i] == NULL)
        continue;
      else
        (*(timer->asyncCB[i]))(i);
    }
  }
}
