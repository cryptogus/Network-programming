//https://curl.se/libcurl/c/http-post.html
#include <stdio.h>
#include <curl/curl.h>

int main(void) {
    CURL *curl;
    CURLcode res;

    // 업로드할 파일의 경로
    const char *file_path = "file1.txt";

    // libcurl 초기화
    curl_global_init(CURL_GLOBAL_DEFAULT);

    // curl 핸들 초기화
    curl = curl_easy_init();
    if(curl) {
        // HTTPS URL 설정
        curl_easy_setopt(curl, CURLOPT_URL, "http://localhost/upload.php");

        // 파일 업로드 설정
        curl_mime *mime = curl_mime_init(curl);
        curl_mimepart *part = curl_mime_addpart(mime);

        // 파일 데이터 추가
        curl_mime_filedata(part, file_path);

        // HTTP POST 요청 설정
        curl_easy_setopt(curl, CURLOPT_MIMEPOST, mime);

        // HTTP 요청 수행
        res = curl_easy_perform(curl);

        // 에러 체크
        if(res != CURLE_OK)
            fprintf(stderr, "curl_easy_perform() failed: %s\n", curl_easy_strerror(res));

        // MIME 구조체 정리
        curl_mime_free(mime);

        // curl 핸들 정리
        curl_easy_cleanup(curl);
    }

    // libcurl 종료
    curl_global_cleanup();
    return 0;
}
