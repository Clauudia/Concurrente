#include <omp.h>
#include <stdio.h>

//Definimos un tipo booleano 
typedef int bool;
#define true 1
#define false 0

//Varibales globales que usaremos 
bool flags[2] = {false,false};
int local[] ={0,1};
int turno[] = {0,1};
int turnod = 0;

int main()
{  

	//Establecemos el números de hilos, en este caso es 2.	
	int nhilos, hilo_id;
	omp_set_num_threads(2);
	
	//Implementación del algoritmo de Dekker
	printf("\n");
	printf("                  Empieza algortimo de Dekker            \n");
	printf("\n");
	//Se indica el inicio de la sección de código dónde se crearan los hilos y al final de esta los hilos se destruirán
	#pragma omp parallel private(hilo_id) shared(flags,turnod)
	{
		//Sección no crítica
		hilo_id = omp_get_thread_num(); //Obtenemos el id de los hilos

		for(int i=0; i<20; i++) //Los hilos van a iterar 20 veces.
		{
			int otro = (hilo_id+1)%2;
			flags[hilo_id] = true; //Se solicita el acceso a la secćión crítica
			int x = 0; //Variable de la cuál se hará uso para calcular la suma 
			/*
			   Desempate de cuándo ambos hilos piden acceso a la sección crítica al mismo tiempo
			*/
			while(flags[otro])
			{
				if(turnod == otro)  
				{
					flags[hilo_id] = false;
					while(turnod != hilo_id);
					flags[hilo_id]=true;
				}
			}
			//Sección crítica
			printf("***El hilo %d acaba de entrar a la sección crítica***\n", hilo_id);
			
			//Se hace la suma de los primeros 1000 números naturales
			for(int j=0; j<1000; j++)
			{
				x+=j;
			}
		printf("---El hilo %d ha salido de la sección crítica---\n", hilo_id);
		//Fin de la sección crítica
		
		turnod = otro; //Una vez que se sale de la sección crítica se libera el turno y se pone en falso la solicitud para entrar a la sección crítica 
		flags[hilo_id] = false;
		}
	}
	
	//Implementación del algoritmo de Peterson
	printf("\n");
	printf("                   Empieza algortimo de Peterson       \n");
	printf("\n");
	#pragma omp parallel private(hilo_id)
	{
		hilo_id = omp_get_thread_num();
		for(int i=0; i<20; i++)
		{
			int otro = (hilo_id+1)%2;
			flags[hilo_id] = true;
			turnod = otro; //Asignación atómica.
			int x = 0; //Variable de la cuál se hace uso para hacer la suma de los primeros 1000 naturales.
			while(flags[otro] && turnod == otro);
			//Sección crítica
			/*
				Se hace la suma
			*/
			printf("***El hilo %d acaba de entrar a la sección crítica***\n", hilo_id);
			for(int j=0; j<1000; j++)
			{
				x+=j;
			}
		printf("---El hilo %d ha salido de la sección crítica---\n", hilo_id);
			//Fin de la sección cítica
		flags[hilo_id] = false; //Se libera la bandera de acceso a la sección critica
		}
	}

	//Implementación del algoritmo de Kessels
	printf("\n");
	printf("                   Empieza algoritmo de Kessels         \n");
	printf("\n");
	#pragma omp parallel private(hilo_id)
	{
		hilo_id = omp_get_thread_num();
		for(int i=0; i<20; i++)
		{
			int otro = (hilo_id+1)%2;
			int x = 0; //Variable que se usa para la suma de los números naturales

			//Condiciones de desempate para el acceso a la sección crítica del hilo 0
			if(hilo_id == 0)
			{
				flags[0] = true;
				local[0] = turno[1];
				turno[0] = local[0];
		
				while(!(flags[1] == false || local[0] != turno[1])); 

				//Sección crítica
				printf("***El hilo %d acaba de entrar a la sección crítica***\n", hilo_id);

				/*
					Se hace la suma de los primero 1000 naturales
				*/
				for(int j=0; j<1000; j++)
				{
					x+=j;
				}
				printf("---El hilo %d ha salido de la sección crítica---\n", hilo_id);
				//Fin de la sección cítica
				flags[0] = false; //Se libera la bandera 
			} else 
			{
				//Condiciones de desempate para el acceso a la sección crítica del hilo 1
				flags[1] = true;
				local[1] = 1 - turno[0];
				turno[1] = local[1];
				while(!(flags[0] == false || local[1] == turno[0]));
				//Sección crítica
				printf("***El hilo %d acaba de entrar a la sección crítica***\n", hilo_id);
				/*
					Se hace la suma de los primeros 1000 números naturales
				*/
				for(int j=0; j<1000; j++)
				{
					x+=j;
				}
				printf("---El hilo %d ha salido de la sección crítica---\n",hilo_id);
				//Fin de la sección crítica
				flags[1] = false; //Se libera la bandera
			}
		}
	}
}