#ifndef CURLINC_HEADER_H
#define CURLINC_HEADER_H
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

struct curl_header {
  char *name;    
  char *value;
  size_t amount; 
  size_t index;  
  unsigned int origin; 
  void *anchor; 
};


#define CURLH_HEADER    (1<<0) 
#define CURLH_TRAILER   (1<<1) 
#define CURLH_CONNECT   (1<<2) 
#define CURLH_1XX       (1<<3) 
#define CURLH_PSEUDO    (1<<4) 

typedef enum {
  CURLHE_OK,
  CURLHE_BADINDEX,      
  CURLHE_MISSING,       
  CURLHE_NOHEADERS,     
  CURLHE_NOREQUEST,     
  CURLHE_OUT_OF_MEMORY, 
  CURLHE_BAD_ARGUMENT,  
  CURLHE_NOT_BUILT_IN   
} CURLHcode;

CURL_EXTERN CURLHcode curl_easy_header(CURL *easy,
                                       const char *name,
                                       size_t index,
                                       unsigned int origin,
                                       int request,
                                       struct curl_header **hout);

CURL_EXTERN struct curl_header *curl_easy_nextheader(CURL *easy,
                                                     unsigned int origin,
                                                     int request,
                                                     struct curl_header *prev);

#ifdef __cplusplus
} 
#endif

#endif 

