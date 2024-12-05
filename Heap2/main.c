#include <stdio.h>
#include <locale.h>
#include <stdlib.h>

typedef struct Area{              //Areas que estãop livres

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


Area* InserirNoArea(Area* a, int comeco, int tamanho ){
    if(a!=NULL){
       if(comeco == a->comeco){
            a->comeco = comeco + tamanho;
            a->tamanho -= tamanho;
       }else {
        InserirNoArea(a->prox,comeco,tamanho);
       }
    } else{
        Area* b = (Area*)malloc(sizeof(Area));
        b->comeco=0;
        b->tamanho=20;
        b->prox=NULL;
    }
    return a;
}

Cheio* InserirNoCheio(Cheio* a, int comeco, int tamanho ){
    if(a==NULL){

        Cheio* b = (Cheio*)malloc(sizeof(Cheio));

        b->comeco = comeco;
        b->tamanho = tamanho;
        b->prox = NULL;
        return b;

    } else {
        a->prox = InserirNoCheio(a->prox, comeco, tamanho);
    } 
    return a;
}

void organiza_heap(int *heap[], Area *lista) {
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
        if(lista->tamanho > x)
            return (Search_menor(lista->prox, x) < lista->tamanho)? Search_menor(lista->prox, x) : lista->tamanho;
        else if( lista->tamanho == x)
            return lista->tamanho; 
        else
            return Search_menor(lista->prox, x);
    }
}
int Search_maior(Area *lista, int x){
    if(lista == NULL){
        return 0;
    }else{
        if(lista->tamanho >= x)
            return (Search_menor(lista->prox, x) > lista->tamanho)? Search_menor(lista->prox, x) : lista->tamanho;
        else
            return Search_menor(lista->prox, x);
    }
}


int Search_primeiro(Area * lista, int x){
    if(lista == NULL){
        return -1;
    }else{
        if(lista->tamanho >= x)
            return lista->tamanho;
        else
            return Search_primeiro(lista->prox, x);
    }
}

int Search_next(Area * lista,Cheio *objeto, int x){
    if(objeto == NULL ){
        return Search_primeiro(lista, x);
    }else{
        if(lista == NULL){
            return -1;
        }
        if(lista->comeco >= (objeto->comeco + objeto->tamanho))
            return Search_primeiro(lista, x);
        else{
            return Search_next(lista->prox,objeto,x);
        }
    }
}

int best_type(Area *lista, int x){
    if(lista == NULL){
        return 0;
    }else{
        if(Search_menor(lista,x) == lista->tamanho){
            return lista->comeco;
        }else{
            return best_type(lista->prox,x);
    }
    }
}

int worst_type(Area *lista, int x){
   if(lista == NULL){
        return 0;
    }else{
        if(Search_maior(lista,x) == lista->tamanho){
            return lista->comeco;
        }else{
            return worst_type(lista->prox,x);
    }
    }
}

int first_type(Area *lista, int x){
    if(lista == NULL){
        return 0;
    }else{
        if(Search_primeiro(lista,x) == lista->tamanho){
            return lista->comeco;
        }else{
            return first_type(lista->prox,x);
    }
    }
}

Area *next_type_aux(Area *lista,Cheio *objeto){
    if(objeto == NULL){
        return lista;
    }else{
            if(lista->comeco >= (objeto->comeco + objeto->tamanho))
                return lista;
            else{
                return next_type_aux(lista->prox,objeto);
            }
    }
}

int next_type(Area *lista,Cheio *objeto, int x){
        if(Search_next(lista, objeto, x) == -1){
            return first_type(lista, x);
        }else{
            return first_type(next_type_aux(lista,objeto), x);
        }
}

int main()
{  
    Area* lista = (Area*)malloc(sizeof(Area));
    lista = NULL;
    lista = InserirNoArea(lista,0,20);
    Cheio* objetos = (Cheio*)malloc(sizeof(Cheio));
    objetos = NULL;
    Cheio objeto;
    
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
