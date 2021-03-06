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

bool package_list_add_deps(nnpkm_context *ctx, str_list *packages, bool is_dep){
    for(str_list *l = packages; l && l->str; l = l->next){
        nnl_package *pck = package_list_find(&ctx->all_packages, l->str);

        if(!pck){
            fprintf(stderr, "Invailid Package: %s\n", l->str);
            return false;
        }

        if(package_list_find(&ctx->packages, pck->name)){
            //already exists in the install or build package list
            return true;
        }

        if(!package_list_add_deps(ctx, &pck->deps, true)){
            fprintf(stderr, "Failed to add deps for package %s\n", l->str);
            return false;
        }

        if(ctx->rebuild){
            if(!package_list_add_deps(ctx, &pck->mkdeps, true)){
                fprintf(stderr, "Failed to add mkdeps for package %s\n", l->str);
                return false;
            }
        }

        package_list_entry *e = NULL;

        if(is_dep && (!pck->installed || ctx->rebuild_deps)){
            e = package_list_add(&ctx->packages, pck);
        } else if(!is_dep && (!pck->installed || ctx->rebuild)){
            e = package_list_add(&ctx->packages, pck);
        }

        if(e)
            e->pck->is_dep = is_dep;
    }
    
    return true;
}

bool package_load_context(nnpkm_context *ctx, str_list *packages){
    package_load_all(ctx->pckm_base, &ctx->all_packages);

    return package_list_add_deps(ctx, packages, false);
}