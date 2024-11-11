// obs : Caso alguem queira alterar algo do projeto, Sempre criem comentários na Linha maior . EX : se criar uma função, criem um
// comentário para especificar para que serve a função

#include <stdio.h>
#include <locale.h>
#include <stdlib.h>

typedef struct Area{    // struct da área (lista)

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


Area* InserirNo(Area* a, int comeco, int tamanho ){ // insere um novo bloco de memória na lista de áreas livres

    
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

void organiza_heap(int heap[], Area *lista) {   // marca a área de memória representada pela lista como livre 
                                                // (cada indice de heap que pertence ao bloco apontado por lista é marcado por 0)
    while (lista != NULL) {
        for (int i = lista->comeco; i < lista->comeco + lista->tamanho; i++) {
            heap[i] = 0;
        }
        lista = lista->prox;
    }
}
int Search_menor(Area *lista, int x){   // retorna o menor bloco livre que é grande o suficiente pra alocar um bloco de tamanho X
    if(lista == NULL){
        return 20;
    }else{
        if(lista->tamanho >= x)
            return (Search_menor(lista->prox, x) < lista->tamanho)? Search_menor(lista->prox, x) : lista->tamanho;
        else
            return Search_menor(lista->prox, x);
    }
}
int Search_maior(Area *lista, int x){   // mesma coisa, só que com o maior bloco livre
     if(lista == NULL){
        return 0;
    }else{
        if(lista->tamanho >= x)
            return (Search_maior(lista->prox, x) > lista->tamanho)? Search_maior(lista->prox, x) : lista->tamanho;
        else
            return Search_maior(lista->prox, x);
    }
}
int Search_primeiro(Area * lista, int x){   // um search_menor, mas com o primeiro bloco livre disponivel
                                            // para o tamanho de x, considerando a posição inicial do comeco
    if(lista == NULL){
        return 20;
    }else{
        if(lista->tamanho >= x)
            return (Search_primeiro(lista->prox, x) < lista->comeco)? Search_primeiro(lista->prox, x) : lista->comeco;
        else
            return Search_primeiro(lista->prox, x);
    }
}

Area *best_type(Area *lista, int x){    //menor bloco que cabe x -> atualiza o tamanho e o comeco
                                        // ou remove da lista se seu tamanho chegar a zero
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

Area *worst_type(Area *lista, int x){   // encontra o maior bloco disponivel para x -> ajusta ou remove o bloco da lista
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

Area *first_type(Area *lista, int x){       // escolhe o primeiro bloco da lista grande o suficiente para x
                                            // atualiza ou remove o bloco apos a locação
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

Area *next_type(Area *lista, int x){    // variação do first type, mas com uma busca que avança de um ponto
                                       // na lista, retornando ao inicio apenas se chegar ao final sem encontrar um bloco adequado
                                       
    Area *ultima_posicao = NULL ;   // onde a função parou na última chamada, para começar daqui na próxima
    
    if (ultima_posicao == NULL || ultima_posicao -> tamanho < x) {  // se a ult posicao = nula || sem espaco, reinicia a busca
        ultima_posicao = lista ;
    }
    
    Area *atual = ultima_posicao ;  // começa a busca de onde parou
    do {
        if (atual -> tamanho >= x){ // tem espaço suificente pra x ?
            
            atual -> tamanho = atual -> tamanho - x;    // ajusta o tamanho e o inicio
            atual -> comeco = atual -> comeco + x;
            
            
            // armazena a posição atual como o novo ponto de parada
            
            ultima_posicao = (atual -> tamanho == 0) ? atual -> prox : atual;
            
            if (atual -> tamanho == 0){
                Area* proximo = atual ->prox;
                free(atual);
                return proximo ;
            }
            return atual ;
        }
        atual = atual -> prox;  // passa pro proximo bloco
        
        if (atual == NULL){     // se chegar ao fim, volta ao inicio da lista
            atual = lista ;
        }
    } while (atual!= ultima_posicao);   // continua ate encontrar um bloco adequado ou ate completar uma volta na lista
    
    return NULL // se nenhum bloco tiver espaço suficiente, retorna NULL
}

// nesta função de cima, o do while é usado para garantir que o laço execute pelo menos uma vez
// antes de verificar a condição de parada, para garantir a primeira execução e o retorno ao inicio



    /////////////////////////////////////// -- ///////////////////////////////////// MAIN
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
