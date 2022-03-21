#include <nnl/common.h>

#define MAX_ARGS 20

/*
    -exe: binary to run
    -log_file: optional file to log stdout/stderr or NULL
    - ...: char * arguments for the exe files
*/
int exec_command(char *exe, char *log_file, ...){
    int pipe_fds[2]; //r/w FD's for pipe

    if(pipe(pipe_fds)){
        perror("Failed to pipe");
        return -1;
    }

    pid_t pid = fork();

    if(pid == -1){
        perror("Failed to fork\n");
        return -1;
    }

    if(pid == 0){
        //in child process
        char *args[MAX_ARGS] = {0};

        dup2(pipe_fds[1], STDOUT_FILENO);
        dup2(pipe_fds[1], STDERR_FILENO);

        close(pipe_fds[1]);

        va_list valist;
        va_start(valist, log_file);

        args[0] = exe;

        char *p = 0;
        for(int i = 1; p = va_arg(valist, char *); i++){
            if(i >= MAX_ARGS){
                fprintf(stderr, "Max args for exec_command hit!\n");
                break;
            }

            args[i] = p;
        }

        va_end(valist);

        execvp(exe, args); // never returns

        perror("Error executing command!\n");

        exit(-1);
    }

    //in parent process
    char read_buffer[1024];
    size_t r = 0;
    int log_fd = 0;

    close(pipe_fds[1]);

    if(log_file){
        log_fd = open(log_file, O_RDWR | O_CREAT, S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH);
    }

    while((r = read(pipe_fds[0], read_buffer, sizeof(read_buffer)))){
        write(STDOUT_FILENO, read_buffer, r);

        if(log_fd){
            write(log_fd, read_buffer, r);
        }
    }

    if(log_fd){
        close(log_fd);
    }

    close(pipe_fds[0]);

    int status = -1;
    if(waitpid(pid, &status, 0) == -1)
        return EXIT_FAILURE;

    return WEXITSTATUS(status);
}