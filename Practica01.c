#include <omp.h>
#include <stdio.h>

int main()
{
	int nhilos, hilo_id;

	//Establecemos el números de hilos, en este caso es 2.
	omp_set_num_threads(2);
	
	//Se indica el inicio de la sección de código dónde se crearan los hilos y al final de esta los hilos se destruirán
	#pragma omp parallel private(nhilos,hilo_id)
	{
		//Variables para Dekker
		int turno = 0;
		int bandera_compartida[2] = {0,0};

		//Obtenemos el id del hilo en ejecución
		hilo_id = omp_get_thread_num();

		//Obtenemos el número de hilos a ejecutarse
		nhilos = omp_get_num_threads();

		//Se imprime el número de hilo que entró a la sección crítica
		printf("El hilo %d acaba de entrar a la sección crítica \n", hilo_id);
	
	}


	//Algoritmo Dekker
	void dekker()
	{
		int bandera[1];
		int otro = (hilo_id + 1)%2;
		bandera[hilo_id] = 1; //El hilo solicita el acceso
		while(bandera[otro]);

	}

}