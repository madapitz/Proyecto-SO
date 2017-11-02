#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>


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

void* primos(void* argc){
	FILE *salida=fopen("salida.txt","w");
	int num =0;
	int booleano;
	while(!feof(argc)){
		fscanf(argc, "%d",&num);
		booleano = calcularPrimos(num);
		fprintf(salida, "%d %d\n", num,booleano);	
	}
	fclose(salida);
	pthread_exit(0);
}

int main (int argc, char** argv){
	FILE *fp = fopen(argv[1],"r");	

	pthread_t tid;
	pthread_attr_t attr;
	pthread_attr_init(&attr);

	pthread_create(&tid,&attr,primos,fp);
	pthread_join(tid,NULL);

	fclose(fp);
}