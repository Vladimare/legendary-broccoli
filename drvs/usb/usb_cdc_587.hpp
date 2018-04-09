#ifndef _USB_CDC_587_HPP_
#define _USB_CDC_587_HPP_

#include "usb_cdc.hpp"

/* ������� usb_cdc ��� 587 ����� */
class usb_cdc_587
  : public usb_cdc
{
public:
  usb_cdc_587(LPC_USB_TypeDef* phy, usbCreationDisposition* cd);
  virtual ~usb_cdc_587();

private:
                                      /* ������������� ���������� ��          *
                                       * �����������                          */
  void initDescriptors();
                                      /* ����������� USB ����������� ��� 587  */
                                      /* �����                                */
  static const unsigned char deviceDescriptor[];
  static const unsigned char configurationDescriptor[];
  static const unsigned char stringDescriptor[];
};


#endif /*_USB_CDC_587_HPP_*/
