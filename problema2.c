#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <string.h>


struct datos
{
	FILE *entrada;
	int numLineasCadaThread;
	int numPrimos;
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

void* primos(void* args){
	struct datos *data = (struct datos*) args;
	int num = 0;
	for (int i = 0; i < data->numLineasCadaThread; i++)
	{
		fscanf(data->entrada,"%d", &num);
		if(calcularPrimos(num)==1) data->numPrimos+=1;
	}
	pthread_exit(0);
}

int main(int argc, char **argv){
	int lineas = contarLineas(argv[1]);
	FILE *entrada = fopen(argv[1],"r");
	if(entrada == NULL){
		printf("Error, archivo vacio\n");
		exit(0);
	}
	if(strcmp(argv[2],"-n") != 0){
		printf("El comando %s no es valido \n", argv[2]);
		exit(0);
	}

	int nt;

	if(argv[3][1]){
		nt = (argv[3][0] - '0')*10 + (argv[3][1] - '0');
	} else{
		nt= (argv[3][0] - '0');
	}

	if(nt<1 || nt>10){
		printf("El numero de hilos debe estar entre 1 y 10\n");
		exit(0);
	}

	struct datos data[nt];
	pthread_t tid[nt];
	for (int i = 0; i < nt; i++)
	{
		data[i].entrada = entrada;
		data[i].numPrimos = 0;

		if (i == nt - 1)
		{
			data[i].numLineasCadaThread = (lineas/nt) + (lineas % nt);
		} else{
			data[i].numLineasCadaThread = lineas/nt;
		}

		pthread_attr_t attr;
		pthread_attr_init(&attr);

		pthread_create(&tid[i],&attr,primos,&data[i]);
	}

	for (int k = 0; k < nt; k++)
	{
		pthread_join(tid[k],NULL);
	}

	fclose(entrada);
	int numPrimos = 0;
	for (int j = 0; j < nt; j++)
	{
		numPrimos += data[j].numPrimos;
	}
	printf("Numero de primos en el archivo: %d\n", numPrimos);
	return 0;
}