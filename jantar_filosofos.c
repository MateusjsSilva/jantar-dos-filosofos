/******************************************************************************
*   autor: Mateus José da Silva
*   data: 14/01/2023
********************************************************************************/

#include <pthread.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

/********* constantes *********/

#define quant 5                             // quantidade de filosofos
#define esquerda (i + quant - 1) % quant    // id do filósofo(a esquerda)
#define direita (i + 1) % quant             // id do filósofo(a direita)

/********* variaveis globais *********/

pthread_mutex_t mutex_hashi[quant];
pthread_mutex_t mutex_filosofos;
pthread_t filosofos[quant];
pthread_cond_t cond[quant];
int comendo[quant] = {0};           // indica quem está comendo

/********* prototipos *********/

void* Filosofos(void* arg);

/* função principal */
int main() {
    int i, status;
    
    /* inicia os mutexes */
	for (i = 0; i < quant; i++) {
		pthread_mutex_init(&mutex_hashi[i], NULL);
		pthread_cond_init(&cond[i], NULL);
	}
	
	pthread_mutex_init(&mutex_filosofos, NULL);
	
	for (i = 0; i < quant; i++) {
	    int* arg = (int*) malloc(sizeof(int));
        *arg = i;
		status = pthread_create(&filosofos[i], NULL, Filosofos, arg);
		if(status){
            printf("Ocorreu um erro ao criar a thread %d\n",  i + 1);
            return EXIT_FAILURE;
        }
	}
	
	for (i = 0; i < quant; i++) {
		pthread_join(filosofos[i], NULL);
	}
	
	/* destroi antes de sair */
    pthread_mutex_destroy(&(mutex_filosofos));
    for(int cont = 0 ; cont < quant ; cont++ ){
        pthread_mutex_destroy(&(mutex_hashi[cont]));
    }
    pthread_exit(NULL);

	return EXIT_SUCCESS;
}

/* função que simula a vida de um filósofo */
void* Filosofos(void* arg) {
    
    int i = *((int*) arg);  // repassa o id do filósofo
    int r;
    
    while (1) {
        
        printf("Filosofo %d: está pensando\n", i + 1);
        r = (rand() % 5 + 1);
        sleep(r); // gasta um tempo em segundos
        
        pthread_mutex_lock(&mutex_hashi[esquerda]);
        printf("Filosofo %d: pegou o garfo %d\n", i + 1, esquerda + 1);
        
        pthread_mutex_lock(&mutex_hashi[direita]);
        printf("Filosofo %d: pegou o garfo %d\n", i + 1, direita + 1);
        
        pthread_mutex_lock(&mutex_filosofos);
        comendo[i] = 1;
    
        /* mostra na tela os filosofos que estão comendo no momento */
        printf("Filosofos comendo ao mesmo tempo: ");
        
        for(int j = 0; j < quant; j++){
            if(comendo[j] == 1){
                printf("%d ",j + 1);
            }
        }
        printf("\n");
        
        pthread_mutex_unlock(&mutex_filosofos);
        r = (rand() % 5 + 1);
        sleep(r); // gasta um tempo em segundos
        
        pthread_mutex_unlock(&mutex_hashi[esquerda]);
        printf("Filosofo %d: soltou o garfo %d\n", i + 1, esquerda + 1);
        comendo[i] = 0;
        r = (rand() % 5 + 1);
        sleep(r); // gasta um tempo em segundos   
        
        pthread_mutex_unlock(&mutex_hashi[direita]);
        printf("Filosofo %d: soltou o garfo %d\n", i + 1, direita + 1);
        r = (rand() % 5 + 1);
        sleep(r); // gasta um tempo em segundos
    }
}