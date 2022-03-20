#ifndef PACKAGE_H_
#define PACKAGE_H_

#include <nnl/common.h>
#include <stdio.h>
#include <string.h>
#include <dirent.h>
#include <stdlib.h>
#include <unistd.h>

#include <yaml.h>

/* YAML document structure:

name: glibc
version: 2.33
repo: core
is_group: false
no_source: false
source: http://ftp.gnu.org/gnu/glibc/glibc-2.33.tar.xz
deps: [

]
mkdeps: [
  
]
extras: [
  'http://www.linuxfromscratch.org/patches/lfs/10.1/glibc-2.33-fhs-1.patch',
  'https://www.iana.org/time-zones/repository/releases/tzdata2021a.tar.gz'
]

*/

typedef struct {
  char *name;
  char *version;
  char *repo;
  char *source;
  bool is_group;
  bool no_package;
  str_list deps;
  str_list mkdeps;
  str_list extras;
} nnl_package;

u32 nnl_package_to_string(nnl_package *pck, char *buffer, u32 max_size);
nnl_package *package_load(char *root, char *name);

typedef struct package_list_entry_{
  nnl_package *pck;
  struct package_list_entry_ *next;
} package_list_entry;

typedef struct{
  package_list_entry *head;
  package_list_entry *tail;
} package_list;

bool package_load_all(char *root, package_list *list);
nnl_package *package_list_find(package_list *list, char *name);
package_list_entry *package_list_add(package_list *list, nnl_package *pck);
bool package_is_installed(char *root, char *name);

#endif