#ifndef __HTTPS_H
#define __HTTPS_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <curl/curl.h>

size_t write_data(void *ptr, size_t size, size_t nmemb, FILE *stream);
char* extract_filename(const char* url);

#endif //__HTTPS_H
