#ifndef CURLINC_URLAPI_H
#define CURLINC_URLAPI_H
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

#include "curl.h"

#ifdef  __cplusplus
extern "C" {
#endif


typedef enum {
  CURLUE_OK,
  CURLUE_BAD_HANDLE,          
  CURLUE_BAD_PARTPOINTER,     
  CURLUE_MALFORMED_INPUT,     
  CURLUE_BAD_PORT_NUMBER,     
  CURLUE_UNSUPPORTED_SCHEME,  
  CURLUE_URLDECODE,           
  CURLUE_OUT_OF_MEMORY,       
  CURLUE_USER_NOT_ALLOWED,    
  CURLUE_UNKNOWN_PART,        
  CURLUE_NO_SCHEME,           
  CURLUE_NO_USER,             
  CURLUE_NO_PASSWORD,         
  CURLUE_NO_OPTIONS,          
  CURLUE_NO_HOST,             
  CURLUE_NO_PORT,             
  CURLUE_NO_QUERY,            
  CURLUE_NO_FRAGMENT,         
  CURLUE_NO_ZONEID,           
  CURLUE_BAD_FILE_URL,        
  CURLUE_BAD_FRAGMENT,        
  CURLUE_BAD_HOSTNAME,        
  CURLUE_BAD_IPV6,            
  CURLUE_BAD_LOGIN,           
  CURLUE_BAD_PASSWORD,        
  CURLUE_BAD_PATH,            
  CURLUE_BAD_QUERY,           
  CURLUE_BAD_SCHEME,          
  CURLUE_BAD_SLASHES,         
  CURLUE_BAD_USER,            
  CURLUE_LACKS_IDN,           
  CURLUE_TOO_LARGE,           
  CURLUE_LAST
} CURLUcode;

typedef enum {
  CURLUPART_URL,
  CURLUPART_SCHEME,
  CURLUPART_USER,
  CURLUPART_PASSWORD,
  CURLUPART_OPTIONS,
  CURLUPART_HOST,
  CURLUPART_PORT,
  CURLUPART_PATH,
  CURLUPART_QUERY,
  CURLUPART_FRAGMENT,
  CURLUPART_ZONEID 
} CURLUPart;

#define CURLU_DEFAULT_PORT (1<<0)       
#define CURLU_NO_DEFAULT_PORT (1<<1)    /* act as if no port number was set,
                                           if the port number matches the
                                           default for the scheme */
#define CURLU_DEFAULT_SCHEME (1<<2)     /* return default scheme if
                                           missing */
#define CURLU_NON_SUPPORT_SCHEME (1<<3) 
#define CURLU_PATH_AS_IS (1<<4)         
#define CURLU_DISALLOW_USER (1<<5)      
#define CURLU_URLDECODE (1<<6)          
#define CURLU_URLENCODE (1<<7)          
#define CURLU_APPENDQUERY (1<<8)        
#define CURLU_GUESS_SCHEME (1<<9)       
#define CURLU_NO_AUTHORITY (1<<10)      /* Allow empty authority when the
                                           scheme is unknown. */
#define CURLU_ALLOW_SPACE (1<<11)       
#define CURLU_PUNYCODE (1<<12)          
#define CURLU_PUNY2IDN (1<<13)          
#define CURLU_GET_EMPTY (1<<14)         /* allow empty queries and fragments
                                           when extracting the URL or the
                                           components */
#define CURLU_NO_GUESS_SCHEME (1<<15)   

typedef struct Curl_URL CURLU;

/*
 * curl_url() creates a new CURLU handle and returns a pointer to it.
 * Must be freed with curl_url_cleanup().
 */
CURL_EXTERN CURLU *curl_url(void);

/*
 * curl_url_cleanup() frees the CURLU handle and related resources used for
 * the URL parsing. It will not free strings previously returned with the URL
 * API.
 */
CURL_EXTERN void curl_url_cleanup(CURLU *handle);

/*
 * curl_url_dup() duplicates a CURLU handle and returns a new copy. The new
 * handle must also be freed with curl_url_cleanup().
 */
CURL_EXTERN CURLU *curl_url_dup(const CURLU *in);

/*
 * curl_url_get() extracts a specific part of the URL from a CURLU
 * handle. Returns error code. The returned pointer MUST be freed with
 * curl_free() afterwards.
 */
CURL_EXTERN CURLUcode curl_url_get(const CURLU *handle, CURLUPart what,
                                   char **part, unsigned int flags);

/*
 * curl_url_set() sets a specific part of the URL in a CURLU handle. Returns
 * error code. The passed in string will be copied. Passing a NULL instead of
 * a part string, clears that part.
 */
CURL_EXTERN CURLUcode curl_url_set(CURLU *handle, CURLUPart what,
                                   const char *part, unsigned int flags);

/*
 * curl_url_strerror() turns a CURLUcode value into the equivalent human
 * readable error string. This is useful for printing meaningful error
 * messages.
 */
CURL_EXTERN const char *curl_url_strerror(CURLUcode);

#ifdef __cplusplus
} 
#endif

#endif 

