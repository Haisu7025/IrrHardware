/* Verify_Helper.c
 * Copyright DBBTHU 2017, tzz, yhs, wx
 *
 * Verify_Helper v1.0 C-Version
 *
 * CLASSIFICATION: CONFIDENTIAL
 */
#include "Verify_Helper.h"

char Secure_Key[16] = {-25, 3, -117, 34, -8, 117, 0, 45, 88, -6, -20, 44, -45, 122, 122, -1};

void mmcpy(unsigned char *dst, unsigned char *src, char size)
{
  char i;
  for (i = 0; i < size; i++)
    *(dst + i) = src[i];
}

void sign_message(unsigned char *raw_message, char msg_full_len)
{
  unsigned char decrypt[16];
  unsigned char temp[64];
  MD5_CTX md5;

  mmcpy(temp, raw_message, msg_full_len - 1);
  mmcpy(temp + msg_full_len - 1, Secure_Key, sizeof(Secure_Key));

  MD5Init(&md5);
  MD5Update(&md5, temp, msg_full_len - 1 + sizeof(Secure_Key));
  MD5Final(&md5, decrypt);
  *(raw_message + msg_full_len - 1) = decrypt[2];

  return;
}

char check_sign(unsigned char *signed_message, char msg_full_len)
{

  char i;
  unsigned char temp[64];

  mmcpy(temp, signed_message, msg_full_len - 1);
  mmcpy(temp + msg_full_len - 1, Secure_Key, sizeof(Secure_Key));

  sign_message(temp, msg_full_len);

  return temp[msg_full_len - 1] == signed_message[msg_full_len - 1];
}
