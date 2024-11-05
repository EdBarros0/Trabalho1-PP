#include <stdio.h>
#include <locale.h>
#include <stdlib.h>

typedef struct Area{

    int comeco;
    int tamanho;
    struct Area* prox;

} Area;

Area* InserirNo(Area* a, int comeco, int tamanho ){

    
    if(a!=NULL){
        a->prox = InserirNo(a->prox, comeco, tamanho);
    } else {
        
        Area* b = (Area*)malloc(sizeof(Area));

        b->comeco = comeco;
        b->tamanho = tamanho;
        b->prox = NULL;
        return b;
    } 
    return a;
}


int main()
{  
    Area* lista = (Area*)malloc(sizeof(Area));
    lista = NULL;
    lista = InserirNo(lista,0,20);
    
    printf("%d\n\n",lista->tamanho);

    while (1) {

                            //printf("\x1B[2J\x1b[H");
    printf("Heap");
    printf("O que deseja fazer ?");
    break;


    }
}
