#include "https.h"

void usage(void) {
    printf("usage: https-request <File URL>");
}

int main(int argc, char *argv[]) {
    CURL *curl;
    FILE *fp;
    CURLcode res;
    const char *fileURL;

    if (argc != 2) {
        usage();
        return 1;
    }
    fileURL = argv[1];
    curl_global_init(CURL_GLOBAL_DEFAULT);

    curl = curl_easy_init();
    if(curl) {
        
        curl_easy_setopt(curl, CURLOPT_URL, fileURL);
        
        const char* filename = extract_filename(fileURL);
        
        fp = fopen(filename, "wb");
        if(fp == NULL) {
            fprintf(stderr, "Failed to open output file!\n");
            return 1;
        }

        curl_easy_setopt(curl, CURLOPT_WRITEDATA, fp);

        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_data);

        res = curl_easy_perform(curl);

        if(res != CURLE_OK)
            fprintf(stderr, "curl_easy_perform() failed: %s\n", curl_easy_strerror(res));

        fclose(fp);

        curl_easy_cleanup(curl);

        free((void*)filename);
    }

    curl_global_cleanup();
    return 0;
}
