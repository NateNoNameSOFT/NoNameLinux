#include <nnl/common.h>

int run_with_chroot(char *root, int (*func)(void *), void *p){
    pid_t pid = fork();

    if(pid == 0){
        chdir(root);

        if(chroot(root)){
            perror("Failed to chroot");
            return -1;
        }

        exit(func(p)); 
    }

    int stat = 0;

    if(waitpid(pid, &stat, 0) == -1)
        return 1;

    return WEXITSTATUS(stat);
}