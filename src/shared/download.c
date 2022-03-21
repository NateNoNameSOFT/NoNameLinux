#include <nnl/download.h>

typedef struct{
    FILE *fp;
    size_t dl_total;
} dl_status;

static size_t write_data(void *ptr, size_t size, size_t nmeb, void *stream){
    dl_status *status = stream;
    size_t written = fwrite(ptr, size, nmeb, status->fp);

    status->dl_total += written;

    printf("\r%ld KB        ", status->dl_total / 1024);
    fflush(stdout);

    return written;
}

bool download_file(char *url, char *filename){
    bool success = true;

    CURL *curl_handle = curl_easy_init();

    curl_easy_setopt(curl_handle, CURLOPT_URL, url);
    curl_easy_setopt(curl_handle, CURLOPT_VERBOSE, 0L);
    // Fake ourselfs as being a browser
    curl_easy_setopt(curl_handle, CURLOPT_USERAGENT, "Mozilla/4.0 (compatible; MSIE 6.0; Windows NT 5.1; SV1; .NET CLR 1.0.3705; .NET CLR 1.1.4322");
    curl_easy_setopt(curl_handle, CURLOPT_NOPROGRESS, 1L);
    curl_easy_setopt(curl_handle, CURLOPT_FOLLOWLOCATION, 1L);
    curl_easy_setopt(curl_handle, CURLOPT_WRITEFUNCTION, write_data);
    curl_easy_setopt(curl_handle, CURLOPT_FAILONERROR, 1L);

    FILE *pagefile = fopen(filename, "wb");

    if(!pagefile){
        perror("Failed to open download file");
        curl_easy_cleanup(curl_handle);
        return false;
    }

    dl_status status;
    status.dl_total = 0;
    status.fp = pagefile;

    curl_easy_setopt(curl_handle, CURLOPT_WRITEDATA, &status);

    // try 5 times
    for(int i = 0; i < 5; i++){
        if(curl_easy_perform(curl_handle)){
            success = false;
            printf("Failure on try %d\n", i + 1);
        } else {
            success = true;
        }

        if(success)
            break;

        //wait 1 second before retrying
        sleep(1);
    }

    fclose(pagefile);

    if(!success)
        remove(filename);

    printf("\n");

    return success;
}