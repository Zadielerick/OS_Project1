#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>

void CPU(int *mem_pipe, int *cpu_pipe);
void Memory(int *mem_pipe, int *cpu_pipe);

int main() {
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
        Memory(memory_pipe, cpu_pipe);
}

void CPU(int *mem_pipe, int *cpu_pipe){
    close(mem_pipe[1]);         //Don't need output for memory
    close(cpu_pipe[0]);         //Don't need input for memory
    int PC,SP,IR,AC,X,Y;
       
}

void Memory(int *mem_pipe, int *cpu_pipe){
    close(mem_pipe[0]);         //Don't need input for CPU    
    close(cpu_pipe[1]);         //Don't need output for CPU
}
