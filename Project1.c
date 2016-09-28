#include <stdio.h>
#include <sys/types.h>

void CPU(void);
void Memory(void);

int main() {
    pid_t pid;
    pid = fork();

    if(pid == 0)
        CPU();
    else
        Memory();
}

void CPU(){
    
}

void Memory(){

}
