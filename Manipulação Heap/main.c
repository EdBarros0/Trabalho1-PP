#include <stdio.h>
#include <locale.h>
#include <stdlib.h>

typedef struct Area{

    int comeco;
    int tamanho;
    struct Area* prox;

} Area;

typedef struct Cheio{

    int nome;
    int comeco;
    int tamanho;
    struct Cheio *prox;

} Cheio;


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

void organiza_heap(int heap[], Area *lista) {
    while (lista != NULL) {
        for (int i = lista->comeco; i < lista->comeco + lista->tamanho; i++) {
            heap[i] = 0;
        }
        lista = lista->prox;
    }
}
int Search_menor(Area *lista, int x){
    if(lista == NULL){
        return 20;
    }else{
        if(lista->tamanho >= x)
            return (Search_menor(lista->prox, x) < lista->tamanho)? Search_menor(lista->prox, x) : lista->tamanho;
        else
            return Search_menor(lista->prox, x);
    }
}
int Search_maior(Area *lista, int x){
     if(lista == NULL){
        return 0;
    }else{
        if(lista->tamanho >= x)
            return (Search_maior(lista->prox, x) > lista->tamanho)? Search_maior(lista->prox, x) : lista->tamanho;
        else
            return Search_maior(lista->prox, x);
    }
}
int Search_primeiro(Area * lista, int x){
    if(lista == NULL){
        return 20;
    }else{
        if(lista->tamanho >= x)
            return (Search_primeiro(lista->prox, x) < lista->comeco)? Search_primeiro(lista->prox, x) : lista->comeco;
        else
            return Search_primeiro(lista->prox, x);
    }
}

Area *best_type(Area *lista, int x){
    if(lista != NULL){
        if(Search_menor(lista, x) == lista->tamanho){
            lista->tamanho = lista->tamanho - x;
            if(lista->tamanho == 0){
                Area *no = (Area*)malloc(sizeof(Area));
                no = lista->prox;
                free(lista);
                return no;
            }
            lista->comeco = lista->comeco + x;
        }else
            return lista->prox;
    }
    return lista;
}

Area *worst_type(Area *lista, int x){
    if(lista != NULL){
        if(Search_maior(lista, x) == lista->tamanho){
            lista->tamanho = lista->tamanho - x;
            if(lista->tamanho == 0){
                Area *no = (Area*)malloc(sizeof(Area));
                no = lista->prox;
                free(lista);
                return no;
            }
            lista->comeco = lista->comeco + x;
        }else
            return lista->prox;
    }
    return lista;
}

Area *first_type(Area *lista, int x){
    if(lista != NULL){
        if(Search_primeiro(lista, x) == lista->comeco){
            lista->tamanho = lista->tamanho - x;
            if(lista->tamanho == 0){
                Area *no = (Area*)malloc(sizeof(Area));
                no = lista->prox;
                free(lista);
                return no;
            }
            lista->comeco = lista->comeco + x;
        }else
            return lista->prox;
    }
    return lista;
}


int main()
{  
    Area* lista = (Area*)malloc(sizeof(Area));
    lista = NULL;
    lista = InserirNo(lista,0,20);
    Cheio* espaco = (Cheio*)malloc(sizeof(Cheio));
    
    while (1) {
    int heap[20];
                               //printf("\x1B[2J\x1b[H");
    printf("                Heap\n");
    for(int i=0;i<20;i++){
        heap[i]=1;
    }
    organiza_heap(heap,lista);
    printf("\n\n");
    printf("O que deseja fazer ?\n");
    break;


    }
}
