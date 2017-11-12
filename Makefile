all:	
	gcc proyecto.c -l pthread -o proyecto
	gcc problema2.c -l pthread -o problema
        gcc semaforo.c -o semaforo
        gcc left.c -o left
        gcc right.c -o right
clean:	
	rm -rf *o proyecto
	rm -rf *o problema2
        rm -rf *o semaforo
        rm -rf *o left
        rm -rf *o right
