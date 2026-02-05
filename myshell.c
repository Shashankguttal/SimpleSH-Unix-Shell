#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <fcntl.h>

#define MAX_WORDS 64
#define BUFFER_SIZE 1024

// FR-5: Professional Interface functions
void show_welcome() {
    printf("**********************************************\n");
    printf("* Welcome to SimpleSH v1.0                   *\n");
    printf("* Type 'help' for available commands         *\n");
    printf("**********************************************\n");
}

void show_help() {
    printf("\n--- SimpleSH Help Menu ---\n");
    printf("Built-ins: cd [dir], exit, help\n");
    printf("I/O Redirection: > (overwrite), >> (append), < (input)\n");
    printf("Piping: cmd1 | cmd2\n\n");
}

// Function to handle command execution with redirection (FR-4 & FR-5)
void execute_command(char **args) {
    for (int i = 0; args[i] != NULL; i++) {
        // FR-4.1: Output Redirection (Overwrite)
        if (strcmp(args[i], ">") == 0) {
            int fd = open(args[i+1], O_WRONLY | O_CREAT | O_TRUNC, 0644);
            if (fd < 0) { perror("open"); exit(1); }
            dup2(fd, STDOUT_FILENO);
            close(fd);
            args[i] = NULL;
        } 
        // FR-4.2: Output Redirection (Append)
        else if (strcmp(args[i], ">>") == 0) {
            int fd = open(args[i+1], O_WRONLY | O_CREAT | O_APPEND, 0644);
            if (fd < 0) { perror("open"); exit(1); }
            dup2(fd, STDOUT_FILENO);
            close(fd);
            args[i] = NULL;
        }
        // FR-5.1: Input Redirection
        else if (strcmp(args[i], "<") == 0) {
            int fd = open(args[i+1], O_RDONLY);
            if (fd < 0) { perror("open"); exit(1); }
            dup2(fd, STDIN_FILENO);
            close(fd);
            args[i] = NULL;
        }
    }
    if (execvp(args[0], args) == -1) {
        perror("Execution failed");
        exit(1);
    }
}

int main() {
    char input[BUFFER_SIZE];
    show_welcome();

    while (1) {
        printf("simplesh> ");
        if (!fgets(input, BUFFER_SIZE, stdin)) break;
        input[strcspn(input, "\n")] = 0;

        // Check for FR-6: Piping (|)
        char *pipe_pos = strchr(input, '|');
        if (pipe_pos != NULL) {
            *pipe_pos = '\0';
            char *cmd1_str = input;
            char *cmd2_str = pipe_pos + 1;

            int pipefd[2];
            if (pipe(pipefd) == -1) { perror("pipe"); continue; }

            pid_t p1 = fork();
            if (p1 == 0) { // First command (Writer)
                dup2(pipefd[1], STDOUT_FILENO);
                close(pipefd[0]); close(pipefd[1]);
                
                char *args1[MAX_WORDS];
                int k = 0;
                char *t = strtok(cmd1_str, " ");
                while(t) { args1[k++] = t; t = strtok(NULL, " "); }
                args1[k] = NULL;
                execute_command(args1);
            }

            pid_t p2 = fork();
            if (p2 == 0) { // Second command (Reader)
                dup2(pipefd[0], STDIN_FILENO);
                close(pipefd[0]); close(pipefd[1]);

                char *args2[MAX_WORDS];
                int k = 0;
                char *t = strtok(cmd2_str, " ");
                while(t) { args2[k++] = t; t = strtok(NULL, " "); }
                args2[k] = NULL;
                execute_command(args2);
            }

            close(pipefd[0]); close(pipefd[1]);
            waitpid(p1, NULL, 0); waitpid(p2, NULL, 0);
            continue;
        }

        // Standard command parsing (FR-1, FR-2)
        char *args[MAX_WORDS];
        int i = 0;
        char *token = strtok(input, " ");
        while (token && i < MAX_WORDS - 1) {
            args[i++] = token;
            token = strtok(NULL, " ");
        }
        args[i] = NULL;

        if (!args[0]) continue;

        // FR-3: Built-in Commands (Executed in Parent)
        if (strcmp(args[0], "exit") == 0) break;
        if (strcmp(args[0], "help") == 0) { show_help(); continue; }
        if (strcmp(args[0], "cd") == 0) {
            if (!args[1] || chdir(args[1]) != 0) perror("cd failed");
            continue;
        }

        // External Command Execution (FR-1, FR-2)
        if (fork() == 0) {
            execute_command(args);
        } else {
            wait(NULL); // Parent waits (FR-2.3)
        }
    }
    printf("Goodbye!\n");
    return 0;
}
