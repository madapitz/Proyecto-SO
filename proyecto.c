#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <string.h>


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
	char *nombre_salida = malloc(sizeof(char));
	*nombre_salida = data_hilos->id + '0';
	//strcpy(nombre_salida, ch);
	strcat(nombre_salida,".txt");
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

int main (int argc, char** argv){
	int lineas = contarLineas(argv[1]);
	FILE *fp = fopen(argv[1],"r");	
	int limite = argv[3][0] - '0'; //para convertir el arg -N en int
	limite -=1;
	struct datos datos_t[limite - 1];

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

	fclose(fp);
}