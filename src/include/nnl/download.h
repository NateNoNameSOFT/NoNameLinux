#include <curl/curl.h>
#include <stdint.h>
#include <stdbool.h>
#include <unistd.h>

bool download_file(char *url, char *filename);