#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>

#define LINE_MAX 40

extern int errno;

void CPU(int *mem_pipe, int *cpu_pipe);
void Memory(int *mem_pipe, int *cpu_pipe, char input_file[]);
void load_instructions(int memory[], char input_file[]);

int main(int argc, char *argv[]) {
    /**
     * Checks that a file with instructions has been passed
     */
    printf("Number of arguments: %d\n", argc);
    if(argc <= 1){
        printf("Not enough arguments, need instruction file\n");
        return -1;
    }
    /**
     * memory_pipe: (from memory to CPU)
     *          [0] : input for CPU
     *          [1] : output for memory
     * cpu_pipe:    (from CPU to memory)
     *          [0] : input for memory
     *          [1] : output for CPU
     */
    int memory_pipe[2];
    int cpu_pipe[2];
    
    pipe(cpu_pipe);
    pipe(memory_pipe);

    pid_t pid;
    pid = fork();

    if(pid == 0)
        CPU(memory_pipe, cpu_pipe);
    else
        Memory(memory_pipe, cpu_pipe, argv[1]);
}

void CPU(int *mem_pipe, int *cpu_pipe){
    close(mem_pipe[1]);         //Don't need output for memory
    close(cpu_pipe[0]);         //Don't need input for memory
    int PC,SP,IR,AC,X,Y;
       
}

void Memory(int *mem_pipe, int *cpu_pipe, char input_file[]){
    close(mem_pipe[0]);         //Don't need input for CPU    
    close(cpu_pipe[1]);         //Don't need output for CPU

    /**
     * 0-999:           User Program
     * 1000-1999:       System Code
     */
    int memory[2000];

    //printf("Input Arguments: ");
    //puts(input_file); 
    load_instructions(memory, input_file);
    int i;
    for(i=0;i<10;i++){
        printf("Instruction %d: %d\n", i,memory[i]);
    }
}

/**
 * Reads instructions from file and loads them into mememory for the
 * memeory function. 
 *
 * Lines may only start with space, new line or numbers, otherwise can cause error. 
 */
void load_instructions(int memory[], char input_file[]){
    FILE *ifp;
    int errnum;
    ifp = fopen(input_file,"r");

    if (ifp == NULL) {
        errnum = errno;
        fprintf(stderr, "Value of errno: %d\n", errno);
        perror("Error printed by perror");
        fprintf(stderr, "Error opening file: %s\n", strerror(errno));
    } 
    int instr,instr_counter = 0;
    char buf[LINE_MAX];
    while (fgets(buf, sizeof buf, ifp) != NULL) {
        printf("Line: ");
        puts(buf);
        if(buf[0] == ' ' || buf[0] == '\n')
            continue;
        else if(buf[0] == '.'){
            continue;
        }
        else {
            sscanf(buf, "%d", &instr);
            memory[instr_counter] = instr;
            instr_counter++;
        }
    }
}
