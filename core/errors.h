#ifndef _ERRORS_H_
#define _ERRORS_H_

#include "errno.h"

#ifdef TNKERNEL_PORT_CORTEXM3
# include "tn.h"
#endif

                                      /* ����� ������                                   */
#define ERR_OK                  0     /* ��� ������, �������� ��������� �������         */
#define ERR_DEVICE_OFF        (-256)  /* ���������� ��������� ��� �� ����������������   */
#define ERR_DEVICE_ON         (-257)  /* ���������� ��������                            */
#define ERR_DEVICE_BUSY       (-EBUSY)/* ���������� ������                              */
#define ERR_DEVICE_NOINIT     (-ENXIO)/* ���������� �� ����������������                 */
#define ERR_DEVICE_NOSYNC     (-260)  /* ���������� �� ����������������                 */
#define ERR_DEVICE_NOLINK     (-261)  /* ���������� �� ��������                         */
#define ERR_BUFFER_TOO_SMALL  (-262)  /* ����� ������� ���                              */
#define ERR_BUFFER_NOT_EXIST  (-263)  /* ����� �� �����                                 */
#define ERR_DATA_CORRUPTED    (-264)  /* ������ ����������, �������� ����������� �����  */
                                      /*   ��� ������������ ����������                  */
#define ERR_DATA_SIZE         (-265)  /* �������� ������ ������                         */
#define ERR_HOST_UNKNOWN      (-266)  /* ������� ����� �� ������������ �����            */
#define ERR_HOST_UNREGISTERED (-267)  /* ������� ����� �� ������������ �����, ���� �����*/
                                      /*   ���� ���������������                         */
#define ERR_PARAM             (-EINVAL)/* ������������ �������� ���������               */
#define ERR_ADDRESS           (-269)  /* ������������ �������� ������                   */
#define ERR_NOT_SUPPORTED     (-ENODEV)/* �������� �� ��������������                    */
#ifdef TNKERNEL_PORT_CORTEXM3         /* �������                                        */
# define ERR_TIMEOUT          TERR_TIMEOUT
#else
# define ERR_TIMEOUT          (-ETIMEOUT)
#endif
#define ERR_TERMINATED        (-272)  /* ���������� ��������                            */

//������ GSM
#define ERR_GSM_ERR           (-400)  /* GSM ����� ������ ERROR                         */
#define ERR_GSM_NO_CARRIER    (-401)  /* ����� GSM ������ "���� �������"                */
#define ERR_GSM_MODE          (-402)  /* �� ������ ����� ������, �������� ���� �����    */
                                      /* ������� ������� � ����� � ������ ������        */
#define ERR_GSM_NOT_REG       (-403)  /* �� ��������������� � ����                      */

                                      /* ������ ��������� ����������������� ��������    */
#define ERR_SS_TYPE           (-1000) /* �������� ��� ���������                         */
#define ERR_SS_BADHDR         (-1001) /* ��������� ��������� �� ������������� ��������� */
#define ERR_SS_DAMAGED        (-1002) /* ��������� ����������                           */
#define ERR_SS_RECREATED      (-1003) /* ��������� ������� ��������                     */
#define ERR_SS_EMPTY          (-1004) /* ��������� �����                                */
#define ERR_SS_FULL           (-1005) /* ��������� ��������� ���������                  */
#define ERR_SS_BLOCK_DAMAGED  (-1006) /* ���� ��������� ���������                       */
#define ERR_SS_BLOCK_NOTEXIST (-1007) /* ���� ��������� �� ����������                   */
#define ERR_SS_ENTRY_DAMAGED  (-1008) /* ������� ��������� ���������                    */
#define ERR_SS_ENTRY_EMPTY    (-1009) /* ������� ��������� ����                         */
#define ERR_SS_ENTRY_NOEMPTY  (-1010) /* ������� ��������� �� ����                      */
#define ERR_SS_ENTRY_NOTEXIST (-1011) /* ������� ��������� �� ����������                */
#define ERR_SS_ENTRY_NOACTUAL (-1012) /* ������� ��������� �� ��������                  */
#define ERR_SS_ATOM_NOTEXIST  (-1013) /* ���������� �����                               */

                                      /* ������ ���������������� ���� ������            */
#define ERR_UDB_NOT_FOUND     (-1100) /* ������������ �� ������                         */
#define ERR_UDB_ALREADY_EXIST (-1101) /* ������������ ��� ����������                    */
#define ERR_UDB_NODE_OVF_OK   (-1102) /* ���� ���������� ������ ���������� (������ ���, */
                                      /*   �������� ������� ��������� �������), �����   */
                                      /*   ��������� �������� ��������� ���������       */
                                      /*   ������� ����                                 */
#define ERR_UDB_NODE_OVF_ERR  (-1103) /* ���� ���������� ������ ���������� ����������   */
                                      /*   ���������, ������� �� ���������, ���������   */
                                      /*   ������� ��������� ����                       */
#define ERR_UDB_NODE_NOOVF    (-1104) /* ���� ���������� ������ �� ����������, �������  */
                                      /*   ���� �� ����� ���� ���������                 */
#define ERR_UDB_NODE_NOT_LEAF (-1105) /* ���� ���������� ������ �� �������� ������,     */
                                      /*   ������� ������� ������������ ������          */
#define ERR_UDB_PROFILE_BAD   (-1106) /* ��������� � ��������������� �������            */
                                      /*   ��������������                               */
#define ERR_UDB_GEOMETRY_BAD  (-1107) /* �� �� ����� �������� � ��������� ��������      */
                                      /*   ���������                                    */

#endif /*_ERRORS_H_*/
