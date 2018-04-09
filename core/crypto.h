#ifndef _CRYPTO_H_
#define _CRYPTO_H_

#define BLOCKSIZE     16      //������ ����� ��� ��������� �����������. ������� �� ��������� �����������. �� ��������
#define ROUNDS        10      //���������� ���������� ����������� �����. ������� �� ��������� �����������. �� ��������
#define BPOLY         0x1B    //������� ��� ��������� �����������, ������� ������������� ����� ��
                              //   (x ^ 8 + x ^ 4 + x ^ 3 + x + 1), �.�. (x ^ 4 + x ^ 3 + x + 1)
#define EXPANDED_KEY_SIZE (BLOCKSIZE * (ROUNDS + 1))//����� ������������ �����

extern unsigned long cr_decrypt_sector(unsigned char* blk);

#endif /*_CRYPTO_H_*/
