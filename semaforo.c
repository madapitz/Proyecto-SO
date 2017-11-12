#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <sys/ipc.h> 
#include <sys/sem.h>
union semun{ 
	int val; 
	struct semid_ds * buf; 
	short int *array; 
	struct seminfo *__buf; 
};
int contarLineas(char *nombre){
	FILE *archivo = fopen(nombre,"r");
	if (archivo ==NULL) return 0;
	int cont = 0;
	int ch;
	int charenlin = 0;
	while((ch = fgetc(archivo)) != EOF) {
		if(ch=='\n'){
			cont++;
			charenlin = 0;
		} else{
			charenlin++;
		}
	}
	if(charenlin > 0){
		cont++;
	}
	fclose(archivo);
	return cont;
}
int buscar_letra (char cad[10], char c)
{   
     int i=0;
     int bandera=0;
     while ((cad[i]!='0')&(bandera==0))
     {
       if (cad[i]==(char)c){
            bandera=1;
       }
       i++;
     }
     return bandera;
}
int main (int argc, char** argv){
	key_t Llave; 
	int semaforo; 
	struct sembuf Proceso; 
	Llave = ftok ("/home/gabrielbaron/Documentos", 313); 
	semaforo = semget (Llave, 10, 0600 | IPC_CREAT); 
	semctl (semaforo, 0, SETVAL, 0); 
	Proceso.sem_num = 0; 
	Proceso.sem_op = 1; 
	Proceso.sem_flg = 0;
	int lineas = contarLineas(argv[1]);
	printf("%d\n",lineas );
	int pid,pid2;
	int status;
	FILE *fp = fopen(argv[1],"r");
	char *lin;
	char *l="";
	char *c=malloc(sizeof(char));
	char *arg[4];
	if ( (pid=fork()) == 0 ){
	 for (int i=0; i<lineas; i++){
	  //FILE *salida=fopen("salida.txt","w");
	  lin=fgets(l,3,fp);
	  //int bandera=buscar_letra(lin,'L');
	  *c= i + '0';
	  if (lin[2]=='L'){
      	arg[0] = "left.c";
      	
      }
      else{
        arg[0] = "right.c";
      }
      arg[1] = "salida.txt";
      arg[2] = c; 
      arg[3] = 0;
      pid2=fork();
      if (pid2 == 0)
           break;
      semop (semaforo, &Proceso, 1);
      execv("right.c",arg);
	 }
	}
	 else{
	 	wait(&status);
	 }
	//fclose(salida);
	fclose(fp);
}