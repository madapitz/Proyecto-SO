#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <string.h>
#include <sys/types.h>
 #include <sys/wait.h>
#include <unistd.h>
#include <signal.h>
#include <sys/time.h>
#include <time.h>

struct datos
{
	FILE *arch;
	int lineas;
	char id;
};


int calcularPrimos(int num){
	if (num == 1) return 0;
	for (int i = 2; i < num; i++)
	{
		if (num % i == 0)
		{
			return 0;
		}
	}
	return 1;
}

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

void* primos(void* arg){
	struct datos *data_hilos = arg;
	char nombre_salida[20];
	sprintf(nombre_salida,"%d.txt",data_hilos->id);
	FILE *salida=fopen(nombre_salida,"w");
	int num =0;
	int booleano;
	for(int i=0; i < data_hilos->lineas;i++){
		fscanf(data_hilos->arch, "%d",&num);
		booleano = calcularPrimos(num);
		fprintf(salida, "%d %d\n", num,booleano);	
	}
	fclose(salida);
	pthread_exit(0);
}
void PrimosProcesos(FILE *arc,int lin, int nomb){   
    char nombre_salida[20];
    //*nombre_salida = nomb + '0';
    //strcat(nombre_salida,".txt");
    sprintf(nombre_salida,"%d.txt",nomb);
    FILE *salida=fopen(nombre_salida,"w");
    int num =0;
    int booleano;
    for(int i=0; i < lin; i++){
    	fscanf(arc, "%d", &num);
    	booleano = calcularPrimos(num);
		fprintf(salida, "%d %d\n", num,booleano);	
    }
    fclose(salida);
}
void catch_signal_ctrlC(int s)
{
  printf("I'm sorry dave I'm afraid I can't do that");
  exit(0);
}
int main (int argc, char** argv){
	int lineas = contarLineas(argv[1]);
	FILE *fp = fopen(argv[1],"r");	

	if(strcmp(argv[3],"-n")!=0) exit(1);

	int limite;//para convertir el arg -N en int
	if(argv[4][1]){
		limite = (argv[4][0] - '0')*10 + (argv[4][1] - '0');
	} else{
		limite = (argv[4][0] - '0');
	}

	if(limite < 1 || limite > 10) exit(1);

	clock_t start,end;
	double tiempo;
	start = clock();

	struct datos datos_t[limite - 1];
    if (strcmp("-t",argv[2])==0){
	  pthread_t tid[limite - 1];
	  for (int i = 0; i < limite; i++)
	  {
		datos_t[i].arch = fp;
		datos_t[i].id = i;

		if (i == limite - 1)
		{
			datos_t[i].lineas = (lineas/limite) + (lineas % limite);
		} else{
			datos_t[i].lineas = lineas/limite;
		}

		pthread_attr_t attr;
		pthread_attr_init(&attr);

		pthread_create(&tid[i],&attr,primos,&datos_t[i]);
	  }
	  for (int i = 0; i < limite; i++)
	  {
		pthread_join(tid[i],NULL);
	  }
    }
    if (strcmp("-p",argv[2])==0){
      int pid,pid2;
      int status;
      if ( (pid=fork()) == 0 ){
        for (int i = 0; i < limite; i++)
        {
          datos_t[i].arch = fp;
		  datos_t[i].id = i;
		  if (i == limite - 1)
		  {
			datos_t[i].lineas = (lineas/limite) + (lineas % limite);
		  } else{
			datos_t[i].lineas = lineas/limite;
		  }
		  pid2=fork();
          if (pid2 == 0)
           break;
          PrimosProcesos(datos_t[i].arch,datos_t[i].lineas,datos_t[i].id);
          signal(SIGINT, catch_signal_ctrlC);
        }
        exit(0);
      }
      else{
      	wait(&status);
      }
    }
    fclose(fp);
    
    end = clock();
    tiempo = ((double) (end - start))/ CLOCKS_PER_SEC;
    printf("El tiempo de ejecucion es %.9f\n s", tiempo);

	return(0);
}