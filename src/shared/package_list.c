#include <nnl/package.h>

package_list_entry *package_list_add(package_list *list, nnl_package *pck){
    if(!pck){
        fprintf(stderr, "Invalid Package!\n");
        return 0;
    }

    package_list_entry *child = calloc(1, sizeof(package_list_entry));

    //if first item in the list is null
    if(!list->head){
        list->head = list->tail = child;
    } else{
        list->tail->next = child;
        list->tail = child;
    }

    child->next = NULL;
    child->pck = pck;

    return child;
}

nnl_package *package_list_find(package_list *list, char *name){
    for(package_list_entry *e = list->head; e != NULL; e = e->next){
        if(!strcmp(name, e->pck->name)){
            return e->pck;
        }
    }

    return NULL;
}

bool package_load_all(char *root, package_list *list){
    char *base_dirs = "abcdefghijklmnopqrstuvwxyz0123456789";

    char *b = base_dirs;

    while(*b){
        char full_path[1024];
        snprintf(full_path, sizeof(full_path) - 1, "%s/repo/%c", root, *b);

        struct dirent *entry;
        DIR *dp = opendir(full_path);

        if(dp){
            while((entry = readdir(dp))){
                if(entry->d_name[0] == '.'){
                    //skip ., .., and hidden directorys
                    continue;
                }

                if(!package_list_add(list,package_load(root, entry->d_name))){
                    return false;
                }
            }
        }

        closedir(dp);

        b++;
    }

    return true;
}

bool package_is_installed(char *root, char *name);