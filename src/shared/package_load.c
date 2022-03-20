#include <nnl/package.h>

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

//at start it will be in INIT state, when it finds a key (such as name) it will be in the KEY state
//once it hits a value such as ([name:] glibc) it will be in the VALUE state
typedef enum{
    INIT, KEY, VALUE, DEP, MKDEP, EXTRA
} parser_state;

void set_field_value(nnl_package *pck, char *name, char *value){
    if(!strcmp(name, "name"))
        pck->name = strdup(value);
    else if(!strcmp(name, "version"))
        pck->version = strdup(value);
    else if(!strcmp(name, "repo"))
        pck->repo = strdup(value);
    else if(!strcmp(name, "source"))
        pck->source = strdup(value);
    else if(!strcmp(name, "is_group"))
        pck->is_group = !strcmp(value, "true");
    else if(!strcmp(name, "no_package"))
        pck->no_package = !strcmp(value, "true");
    else if(!strcmp(name, "deps"))
        str_list_append(&pck->deps, value);
    else if(!strcmp(name, "mkdeps"))
        str_list_append(&pck->mkdeps, value);
    else if(!strcmp(name, "extras"))
        str_list_append(&pck->extras, value);
}

nnl_package *parse_package(char *filename){
    FILE *fp = fopen(filename, "r");

    if(!fp){
        perror("Failed to open package file\n");
        return 0;
    }

    yaml_parser_t parser;
    yaml_token_t token;

    if(!yaml_parser_initialize(&parser)){
        perror("Failed to init parser!\n");
        fclose(fp);
        return 0;
    }

    yaml_parser_set_input_file(&parser, fp);

    nnl_package *pck = calloc(1, sizeof(nnl_package));

    if(!pck){
        perror("Failed to allocate package!\n");
        return 0;
    }

    parser_state state = INIT;
    char cur_field[1024];

    do{
        yaml_parser_scan(&parser, &token);

        switch(token.type){
            case YAML_KEY_TOKEN: {
                state = KEY;
            } break;

            case YAML_VALUE_TOKEN:{
                state = VALUE;
            } break;

            case YAML_SCALAR_TOKEN:{
                char *data = token.data.scalar.value;

                if(state == KEY)
                    snprintf(cur_field, sizeof(cur_field) - 1, "%s", data);
                else if(state == VALUE)
                    set_field_value(pck, cur_field, data);
            } break;
        }
    } while(token.type != YAML_STREAM_END_TOKEN);

    fclose(fp);

    return pck;
}

nnl_package *package_load(char *root, char *name){
    char full_path[1024];
    snprintf(full_path, sizeof(full_path) - 1, "%s/repo/%c/%s/.pck", root, *name, name);

    nnl_package *pck = parse_package(full_path);

    pck->installed = package_is_installed(root, name);

    snprintf(full_path, sizeof(full_path) - 1, "%s/repo/%c/%s", root, *name, name);
    pck->repo_path = strdup(full_path);


    return pck;
}

