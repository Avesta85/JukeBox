#ifndef CURLINC_OPTIONS_H
#define CURLINC_OPTIONS_H
/***************************************************************************
 *                                  _   _ ____  _
 *  Project                     ___| | | |  _ \| |
 *                             / __| | | | |_) | |
 *                            | (__| |_| |  _ <| |___
 *                             \___|\___/|_| \_\_____|
 *
 * Copyright (C) Daniel Stenberg, <daniel@haxx.se>, et al.
 *
 * This software is licensed as described in the file COPYING, which
 * you should have received as part of this distribution. The terms
 * are also available at https://curl.se/docs/copyright.html.
 *
 * You may opt to use, copy, modify, merge, publish, distribute and/or sell
 * copies of the Software, and permit persons to whom the Software is
 * furnished to do so, under the terms of the COPYING file.
 *
 * This software is distributed on an "AS IS" basis, WITHOUT WARRANTY OF ANY
 * KIND, either express or implied.
 *
 * SPDX-License-Identifier: curl
 *
 ***************************************************************************/

#ifdef  __cplusplus
extern "C" {
#endif

typedef enum {
  CURLOT_LONG,    
  CURLOT_VALUES,  
  CURLOT_OFF_T,   
  CURLOT_OBJECT,  
  CURLOT_STRING,  
  CURLOT_SLIST,   
  CURLOT_CBPTR,   
  CURLOT_BLOB,    
  CURLOT_FUNCTION 
} curl_easytype;



/* "alias" means it is provided for old programs to remain functional,
   we prefer another name */
#define CURLOT_FLAG_ALIAS (1<<0)

/* The CURLOPTTYPE_* id ranges can still be used to figure out what type/size
   to use for curl_easy_setopt() for the given id */
struct curl_easyoption {
  const char *name;
  CURLoption id;
  curl_easytype type;
  unsigned int flags;
};

CURL_EXTERN const struct curl_easyoption *
curl_easy_option_by_name(const char *name);

CURL_EXTERN const struct curl_easyoption *
curl_easy_option_by_id(CURLoption id);

CURL_EXTERN const struct curl_easyoption *
curl_easy_option_next(const struct curl_easyoption *prev);

#ifdef __cplusplus
} 
#endif
#endif 

