#ifndef COMMON_H_
#define COMMON_H_

#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include <dirent.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/stat.h>

typedef uint64_t u64;
typedef uint32_t u32;
typedef uint16_t u16;
typedef uint8_t u8;

typedef struct str_list_ {
    char *str;
    struct str_list_ *next;
} str_list;

str_list *str_list_append(str_list *l, char *s);
void str_list_free(str_list *l, bool free_head);
void str_list_copy(str_list *to, str_list *from);
str_list *str_list_from_str(char *str, char *sep);
bool create_package_path(char *root, char *name);

#endif