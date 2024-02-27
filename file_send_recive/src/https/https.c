#include "https.h"

size_t write_data(void *ptr, size_t size, size_t nmemb, FILE *stream) {
    return fwrite(ptr, size, nmemb, stream);
}

char* extract_filename(const char* url) {
    const char* p = strrchr(url, '/');
    if (p) {
        const char* q = strchr(p, '?');
        size_t len = (q != NULL) ? (size_t)(q - p - 1) : strlen(p + 1);
        return strndup(p + 1, len);
    } else {
        return strdup(url);
    }
}
