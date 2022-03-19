/* NoName Package Manager */

#include <stdio.h>
#include <nnl/package.h>

int main(int argc, char *argv[]){

    printf("NNL NoName Package Manager v1.0.0\n");

    nnl_package *pck = package_load("/nnl/NoNameLinux", "glibc");

    if(!pck){
        printf("Cant find package!\n");
        return -1;
    }

    printf("Package found: %s %s\n", pck->name, pck->version);
    printf("Source: %s\n", pck->source);
    printf("Extras: \n");
    
    for(str_list *l = &pck->extras; l && l->str; l = l->next){
        printf("\t: %s\n", l->str);
    }

    return 0;
}