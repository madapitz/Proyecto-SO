#include <sys/types.h> 
#include <sys/ipc.h> 
#include <sys/sem.h>
#include <stdio.h> 
#include <string.h>
union semun{ 
  int val; 
  struct semid_ds * buf; 
  short int *array; 
  struct seminfo *__buf; 
  };
  int main(const char *path, char *const argv[]){ 
    printf("JIIIIIImmy\n");
    key_t Llave; 
    int semaforo; 
    struct sembuf Proceso; 
    int i = 0 ; 
    printf("PPC\n");
    Llave = ftok ("/home/gabrielbaron/Documentos", 313); 
    semaforo = semget (Llave, 10, 0600 | IPC_CREAT); 
    semctl (semaforo, 0, SETVAL, 0); 
    Proceso.sem_num = 0; 
    Proceso.sem_op = -1; 
    Proceso.sem_flg = 0;
    FILE *salida=fopen(argv[1],"w");
    semop (semaforo, &Proceso, 1);
    fprintf(salida, "%s %s\n", argv[2],"R");
    fclose(salida);
  }