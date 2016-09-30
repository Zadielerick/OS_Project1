#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <time.h>

#define LINE_MAX 40

extern int errno;

void CPU(int *mem_pipe, int *cpu_pipe);
void Memory(int *mem_pipe, int *cpu_pipe, char input_file[]);
void load_instructions(int memory[], char input_file[]);
void mem_read(int *mem_pipe,int *cpu_pipe, int *mem);
void mem_write(int *mem_pipe, int *mem);

int main(int argc, char *argv[]) {
    /**
     * Checks that a file with instructions has been passed
     */
    if(argc <= 1){
        printf("Not enough arguments, need instruction file\n");
        return -1;
    }
    /**
     * memory_pipe: (from CPU to memory)
     *          [0] : input for memory
     *          [1] : output for CPU
     * cpu_pipe:    (from memory to CPU)
     *          [0] : input for CPU
     *          [1] : output for memory
     */
	srand(time(NULL));
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
    close(mem_pipe[0]);         
    close(cpu_pipe[1]);         
    int PC=0,SP=0,IR=0,AC=0,X=0,Y=0;
	char read_buf[20]="0";
	char write_buf[20]="";
	do{
		write_buf[0] = '1';
		write(mem_pipe[1], write_buf, (strlen(write_buf)+1));
		
		sprintf(write_buf, "%d", PC);
		PC++;
		write(mem_pipe[1], write_buf, (strlen(write_buf)+1));
		read(cpu_pipe[0], read_buf,sizeof(read_buf));
		IR = atoi(read_buf);
		switch(IR){
			case 1:
				write(mem_pipe[1], "1", 2);

				sprintf(write_buf, "%d", PC);
				PC++;
				write(mem_pipe[1], write_buf, (strlen(write_buf)+1));
				read(cpu_pipe[0], read_buf, sizeof(read_buf));

				AC = atoi(read_buf);
				break;
			case 2:
				write(mem_pipe[1], "1", 2);

				sprintf(write_buf, "%d", PC);
				PC++;
				write(mem_pipe[1], write_buf, (strlen(write_buf)+1));
				read(cpu_pipe[0], read_buf, sizeof(read_buf));

				write(mem_pipe[1], "1", 2);
				write(mem_pipe[1], read_buf, (strlen(read_buf)+1));
				read(cpu_pipe[0], read_buf, sizeof(read_buf));
				
				AC = atoi(read_buf);
				break;
			case 3:
				write(mem_pipe[1], "1", 2);

				sprintf(write_buf, "%d", PC);
				PC++;
				write(mem_pipe[1], write_buf, (strlen(write_buf)+1));
				read(cpu_pipe[0], read_buf, sizeof(read_buf));

				write(mem_pipe[1], "1", 2);
				write(mem_pipe[1], read_buf, (strlen(read_buf)+1));
				read(cpu_pipe[0], read_buf, sizeof(read_buf));
				
				write(mem_pipe[1], "1", 2);
				write(mem_pipe[1], read_buf, (strlen(read_buf)+1));
				read(cpu_pipe[0], read_buf, sizeof(read_buf));
				
				AC = atoi(read_buf);	
				break;
			case 4:
				write(mem_pipe[1], "1", 2);

                sprintf(write_buf, "%d", PC);
                PC++;
                write(mem_pipe[1], write_buf, (strlen(write_buf)+1));
                read(cpu_pipe[0], read_buf, sizeof(read_buf));
				
				sprintf(write_buf, "%d", atoi(read_buf) + X); 

                write(mem_pipe[1], "1", 2);
				write(mem_pipe[1], write_buf, (strlen(write_buf)+1));
                read(cpu_pipe[0], read_buf, sizeof(read_buf));
				
				AC = atoi(read_buf);	
				break;
			case 5:
				write(mem_pipe[1], "1", 2);

                sprintf(write_buf, "%d", PC);
                PC++;
                write(mem_pipe[1], write_buf, (strlen(write_buf)+1));
                read(cpu_pipe[0], read_buf, sizeof(read_buf));
				
				sprintf(write_buf, "%d", atoi(read_buf) + Y); 

                write(mem_pipe[1], "1", 2);
				write(mem_pipe[1], write_buf, (strlen(write_buf)+1));
                read(cpu_pipe[0], read_buf, sizeof(read_buf));
				
				AC = atoi(read_buf);	
				
				break;
			case 6:
				write(mem_pipe[1], "1", 2);

				sprintf(write_buf, "%d", SP + X);
				write(mem_pipe[1], write_buf, (strlen(write_buf)+1));
				read(cpu_pipe[0], read_buf, sizeof(read_buf));
				
				AC = atoi(read_buf);
				break;
			case 7:
				write(mem_pipe[1], "1", 2);

				sprintf(write_buf, "%d", PC);
				PC++;
				write(mem_pipe[1], write_buf, (strlen(write_buf)+1));
				read(cpu_pipe[0], read_buf, sizeof(read_buf));

				write(mem_pipe[1], "1", 2);
				write(mem_pipe[1], read_buf, (strlen(read_buf)+1));
				read(cpu_pipe[0], read_buf, sizeof(read_buf));
			
					
				write(mem_pipe[1], "2", 2);
				write(mem_pipe[1], read_buf, (strlen(read_buf)+1));
				sprintf(write_buf, "%d", AC);
				write(mem_pipe[1], write_buf, (strlen(write_buf)+1));	
				break;
			case 8:
				AC = (rand() % 100) + 1;
				break;
			case 9:
				write(mem_pipe[1], "1", 2);

				sprintf(write_buf, "%d", PC);
				PC++;
				write(mem_pipe[1], write_buf, (strlen(write_buf)+1));
				read(cpu_pipe[0], read_buf, sizeof(read_buf));
				if(atoi(read_buf) == 1)
					printf("%d",AC);				
				else if(atoi(read_buf) == 2)
					printf("%hhu\n",AC);
				else
					printf("Port number %d unrecognized!\n", atoi(read_buf));
				break;
			case 10:
				AC = AC + X;
				break;
			case 11:
				AC = AC + Y;
				break;
			case 12:
				AC = AC - X;
				break;
			case 13:
				AC = AC - Y;
				break;
			case 14:
				X = AC;
				break;
			case 15:
				AC = X;
				break;
			case 16:
				Y = AC;
				break;
			case 17:
				AC = Y;
				break;
			case 18:
				SP = AC;
				break;
			case 19:
				AC = SP;
				break;
			case 20:
				write(mem_pipe[1], "1", 2);

				sprintf(write_buf, "%d", PC);
				PC++;
				write(mem_pipe[1], write_buf, (strlen(write_buf)+1));
				read(cpu_pipe[0], read_buf, sizeof(read_buf));
				
				PC = atoi(read_buf);		
				break;
			case 21:
				write(mem_pipe[1], "1", 2);

				sprintf(write_buf, "%d", PC);
				PC++;
				write(mem_pipe[1], write_buf, (strlen(write_buf)+1));
				read(cpu_pipe[0], read_buf, sizeof(read_buf));
				
				if(AC == 0){
					PC = atoi(read_buf);
				}
				break;
			case 22:
				write(mem_pipe[1], "1", 2);

				sprintf(write_buf, "%d", PC);
				PC++;
				write(mem_pipe[1], write_buf, (strlen(write_buf)+1));
				read(cpu_pipe[0], read_buf, sizeof(read_buf));
				
				if(AC != 0){
					PC = atoi(read_buf);
				}
					
				break;
			case 23:
				//Jump to address
				write(mem_pipe[1], "1", 2);

				sprintf(write_buf, "%d", PC);
				PC++;
				write(mem_pipe[1], write_buf, (strlen(write_buf)+1));
				read(cpu_pipe[0], read_buf, sizeof(read_buf));
				
				write(mem_pipe[1], "2", 2);
				sprintf(write_buf, "%d", SP);
				write(mem_pipe[1], write_buf, (strlen(write_buf)+1));
				SP--;
				sprintf(write_buf, "%d", PC);
				write(mem_pipe[1], write_buf, (strlen(write_buf)+1));
				
				PC = atoi(read_buf);		
				break;
			case 24:
				//Read Address from stack
				write(mem_pipe[1], "1", 2);
				SP--;
				sprintf(write_buf, "%d", SP);
				write(mem_pipe[1], write_buf, (strlen(write_buf)+1));
				read(cpu_pipe[0], read_buf, sizeof(read_buf));

				PC = atoi(read_buf);
				break;
			case 25:
				X++;
				break;
			case 26:
				X--;
				break;
			case 27:
				write(mem_pipe[1], "2", 2);
				sprintf(write_buf, "%d", SP);
				write(mem_pipe[1], write_buf, (strlen(write_buf)+1));
				SP--;
				sprintf(write_buf, "%d", AC);
				write(mem_pipe[1], write_buf, (strlen(write_buf)+1));
				break;
			case 28:
				write(mem_pipe[1], "1", 2);
				SP--;
				sprintf(write_buf, "%d", SP);
				write(mem_pipe[1], write_buf, (strlen(write_buf)+1));
				read(cpu_pipe[0], read_buf, sizeof(read_buf));

				AC = atoi(read_buf);
				
				break;
			case 29:
				
				break;
			case 30:
				break;
			case 50:
				break;
			default:
				printf("Unrecognized Instruction... Exiting Program...");
				write(mem_pipe[1],"0",2);
				IR = 50;
				break;	
		}		
	} while(IR != 50);	
	write(mem_pipe[1],"0",2); 
}

void Memory(int *mem_pipe, int *cpu_pipe, char input_file[]){
    close(mem_pipe[1]);             
    close(cpu_pipe[0]);         

    /**
     * 0-999:           User Program
     * 1000-1999:       System Code
     */
    int memory[2000];
	
    load_instructions(memory, input_file);
    
	//int i;
    //for(i=0;i<10;i++){
    //    printf("Instruction %d: %d\n", i,memory[i]);
    //}

	int read_byte;
	char read_buf[2]; 
	do{
		read_byte = read(mem_pipe[0], read_buf, sizeof(read_buf));
		switch(read_buf[0]) {
			case '0':
				printf("Exiting Program...\n"); 
				break;
			case '1':
				printf("READ\n");
				mem_read(mem_pipe, cpu_pipe, memory);
				break;
			case '2':
				printf("WRITE\n");
				mem_write(mem_pipe, memory);
				break;
			default:	
				printf("Error: Unrecognized Request -> %c. Exiting Program...\n", read_buf[0]);
				read_buf[0] = '0';
				break;
		}
	} while(read_buf[0] != '0');
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
        if(buf[0] == ' ' || buf[0] == '\n')
            continue;
        else if(buf[0] == '.'){
			instr_counter = atoi(&buf[1]);
            continue;
        }
        else {
            sscanf(buf, "%d", &instr);
            memory[instr_counter] = instr;
            instr_counter++;
        }
    }
}

void mem_read(int *mem_pipe, int *cpu_pipe, int *mem){
	char read_buf[20];
	read(mem_pipe[0], read_buf, sizeof(read_buf));
	int address = atoi(read_buf);
	char data[20];
	sprintf(data, "%d", mem[address]);
	write(cpu_pipe[1], data, (strlen(data)+1));
}

void mem_write(int *mem_pipe, int *mem){
	char read_buf[20];
	int address, data;
	read(mem_pipe[0], read_buf, sizeof(read_buf));
	address = atoi(read_buf);
	read(mem_pipe[0], read_buf, sizeof(read_buf));
	data = atoi(read_buf);
	mem[address] = data; 
}
