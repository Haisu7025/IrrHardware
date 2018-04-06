/* Verify_Helper.h
 * Copyright DBBTHU 2017, tzz, yhs, wx
 *
 * Verify_Helper v1.0 C-Version
 *
 * CLASSIFICATION: CONFIDENTIAL
 */

#ifndef __VERIFY_HELPER_H
#define __VERIFY_HELPER_H

#include "md5.h"
#include "sys.h"

void sign_message(unsigned char *raw_message, char msg_full_len);
char check_sign(unsigned char *signed_message, char msg_full_len);

#endif
