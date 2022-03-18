/* NoName Package Creator */

#include <nnl/package.h>

#include <stdio.h>
#include <string.h>
#include <dirent.h>
#include <stdlib.h>
#include <unistd.h>
#include <argp.h>
#include <libgen.h>
#include <sys/stat.h>
#include <sys/wait.h>

static char doc[] = "NN Package Creator";
static char args_doc[] = "Argument Details";

static struct argp_option options[] = {
    { "root", 'r', "path", 0, "Alternative root filesystem"},
    { "url", 'u', "url", 0, "Source URL"},
    { "package-name", 'p', "name", 0, "Package name"},
    { "version", 'v', "version", 0, "Version Information"},
    { "group", 'g', 0, 0, "Package is a group"},
    { "no-package", 'q', 0, 0, "No package to download for install"},
    { "deps", 'd', "dep names", 0, "Dependancy Names (Comma Seperated)"},
    { "mkdeps", 'm', "mkdep names", 0, "Make Dependancy Names (Comma Seperated)"},
    { "extras", 'e', "extra names", 0, "Extra Files (Comma Seperated)"},
    { "with-install", 'i', 0, 0, "Create install.sh"},
    {0}
};

typedef struct {
    char *root;
    bool with_install;
    nnl_package pck;
} nnpkc_args;

static error_t parse_opt(int key, char *arg, struct argp_state *state){
    nnpkc_args *args = state->input;

    switch(key){
        case 'r':
            args->root = arg;
            break;
        case 'u':
            args->pck.source = arg;
            break;
        case 'p':
            args->pck.name = arg;
            break;
        case 'v':
            args->pck.version = arg;
            break;
        case 'g':
            args->pck.is_group = true;
            break;
        case 'q':
            args->pck.no_package = arg;
            break;
        case 'i':
            args->with_install = true;
            break;
        case 'd':{
            str_list *l = str_list_from_str(arg, ",");
            str_list_copy(&args->pck.deps, l);
            str_list_free(l, true);
            break;
        }
        case 'm':{
            str_list *l = str_list_from_str(arg, ",");
            str_list_copy(&args->pck.mkdeps, l);
            str_list_free(l, true);
            break;
        }
        case 'e':{
            str_list *l = str_list_from_str(arg, ",");
            str_list_copy(&args->pck.extras, l);
            str_list_free(l, true);
            break;
        }
        default:
            return ARGP_ERR_UNKNOWN;
    }

    return 0;
}

bool load_pck_from_source(nnpkc_args *args){
    char filename[512];
    strcpy(filename, basename(args->pck.source));

    // glibc-2.33.tar.xz <- Stop at .t
    char *end = strstr(filename, ".t");

    if(!end)
        return false;

    *end = 0;

    // find the very last dash in the filename
    end = strrchr(filename, '-');

    if(!end)
        return false;

    // version is the part after the last dash
    char *ver = end + 1;
    *end = 0;

    args->pck.name = strdup(filename);
    args->pck.version = strdup(ver);

    return true;
}

static struct argp argp = { options, parse_opt, args_doc, doc, 0, 0, 0};

int main(int argc, char *argv[]){
    nnpkc_args args = {0};

    printf("NNL - NoName Package Creator v1.0.0\n\n");

    argp_parse(&argp, argc, argv, 0, 0, &args);

    if(!args.root){
        args.root = "/usr/share/nnpkc/repo";
        char *root = getenv("NNPKC_ROOT");

        if(root)
            args.root = root;
    }

    if(!args.pck.name){
        if(!args.pck.source){
            printf("Must provide URL or name!\n");
            return -1;
        }

        if(!load_pck_from_source(&args)){
            printf("Failed to parse source URL\n");
            return -1;
        }
    }

    char pck_str[1024];
    nnl_package_to_string(&args.pck, pck_str, 1024);

    char pck_file[1024];
    char build_file[1024];
    sprintf(pck_file, "%s/repo/%c/%s/.pck", args.root, args.pck.name[0], args.pck.name);
    sprintf(build_file, "%s/repo/%c/%s/.build", args.root, args.pck.name[0], args.pck.name);

    if(access(pck_file, F_OK) == 0){
        fprintf(stderr, "Package Already Exists!\n");
        return -1;
    }

    if(!create_package_path(args.root, args.pck.name)){
        return -1;
    }

    FILE *fp = fopen(pck_file, "w");

    if(!fp){
        fprintf(stderr, "failed to create pck file\n");
        return -1;
    }

    fwrite(pck_str, strlen(pck_str), 1, fp);
    fclose(fp);

    fp = fopen(build_file, "w");

    if(!fp){
        fprintf(stderr, "failed to create build file\n");
        return -1;
    }

    fprintf(fp, "./configure --prefix=/usr\n"
                "make\n"
                "make DESTDIR=$P install\n"
    );

    fclose(fp);

    pid_t vi = fork();

    if(vi == -1){
        printf("Failed to fork process: %d\n", errno);
        return -1;
    }

    if(vi == 0){
        //in vi child process

        if(!execlp("/usr/bin/vim", "vim", build_file, NULL)){
            printf("Failed to execute vim. %d\n", errno);
            return -1;
        }

        exit(EXIT_SUCCESS);
    } else{
        waitpid(vi, NULL, 0);
    }

    if(args.with_install){
        char path[1024];
        sprintf(path, "%s/repo/%c/%s/.install", args.root, args.pck.name[0], args.pck.name);
        
        if(mkdir(path, 0775)){
            fprintf(stderr, "Failed to create installer path\n");
            return -1;
        }

        sprintf(path, "%s/repo/%c/%s/.install/install.sh", args.root, args.pck.name[0], args.pck.name);    

        vi = fork();

        if(vi == -1){
            printf("Failed to fork process: %d\n", errno);
            return -1;
        }

        if(vi == 0){
            if(!execlp("/usr/bin/vim", "vim", path, NULL)){
                printf("Failed to execute vim. %d\n", errno);
                return -1;
            }

            exit(EXIT_SUCCESS);
        } else{
            waitpid(vi, NULL, 0);
        }
    }

    return 0;
}