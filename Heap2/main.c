#include <stdio.h>
#include <locale.h>
#include <stdlib.h>
#include <string.h>
#ifdef __linux__
    #include <unistd.h>
#elif _WIN32
    #include <windows.h>
#else

#endif
//////////////////////////////////////////////////////////////////////////////////////
void clear_screen(){
    #ifdef __linux__
        system("clear");
    #elif _WIN32
        system("cls");
    #else

    #endif
}

void MySleep(int tempoMs){
    #ifdef __linux__
        usleep(tempoMs *1000);
    #elif _WIN32
        Sleep(tempoMs);
    #else

    #endif
}
////////////////////////////////////////////////////////////////////////////////////////

int P = 20;

///////////////////////////////////////////////////////////////////////////////////////

void RemoveEspacos(char *a){
    for(int i=0; a[i] != '\0'; i++){
        if(a[i] == ' '){
            for(int j = i; a[j] != '\0';j++){
                a[j]= a[j+1];
            }
            i--;
        }
    }
}

void LimparType(char *a){
    for(int i=0; a[i] != '\0';i++){
        a[i]= '\0';
    }
}

//////////////////////////////////////////////////////////////////////////////////////
typedef struct Area{              //Areas que estão livres

    int comeco;
    int tamanho;
    struct Area* prox;

} Area;

typedef struct Cheio{           // Areas que estão cheias

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
        a->prox = InserirNoArea(a->prox,comeco,tamanho);
       }
    } else{
        Area* b = (Area*)malloc(sizeof(Area));
        b->comeco=0+tamanho;
        b->tamanho=20-tamanho;
        b->prox=NULL;
        return b;
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
        return a;
    } 
}

void organiza_heap(int *heap, Area *lista) {
    while (lista != NULL) {
        for (int i = lista->comeco; i < lista->comeco + lista->tamanho; i++) {
            heap[i] = 0;
        }
        lista = lista->prox;
    }
}
void ImprimeHeap(int *heap){
    for(int i=0;i<P;i++){
        printf(" %d ", heap[i]);
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
    lista = InserirNoArea(lista,0,0);
    Cheio* objetos = (Cheio*)malloc(sizeof(Cheio));
    objetos = NULL;
    Cheio* objeto = (Cheio*)malloc(sizeof(Cheio));
    InserirNoCheio(objeto, 0, 0);

    char type[6] = {"first"};      // String que representa tipo de implementacao deseja

    while (1) {
        int heap[P];
        clear_screen();
        printf("                            Heap\n");
        for(int i=0;i<P;i++){
            heap[i]=1;
        }
        organiza_heap(heap,lista);
        ImprimeHeap(heap);
        printf("\n\n");
        char op[100];       //String que representa qual sera a operacao feita
        printf("Deseja definir o tipo da operacao(sim/nao)? ");
        char confirm[4];
        fgets(confirm, 4,stdin);
        switch(confirm[0])
        {
            case 's':
                clear_screen();
                LimparType(type);
                printf("Digite o tipo de operacao desejada(first,best,worst,next): ");
                getchar();
                fgets(type, 6, stdin);
                type[strcspn(type,"\n")] = 0;
                break;

            case 'S':
                clear_screen();
                LimparType(type);
                printf("Digite o tipo de operacao desejada(first,best,worst,next): ");
                getchar();
                fgets(type, 6, stdin);
                type[strcspn(type,"\n")] = 0;
                break;

            case 'n':
                break;

            case 'N':
                break;
        }
        clear_screen();
        printf("Digite a operacao desejada(new a 6, del a, c = a): ");
        if(confirm[0] == 'n' || confirm[0] == 'N' || type[0] == 'w' || type[0] == 'f')
            getchar();
        fgets(op, 100, stdin);
        char aux[100];
        for(int i=0; op[i] != ' ';i++){
            aux[i] = op[i];
        }
        RemoveEspacos(op);
        char it[2];
        it[0]=op[4];
        int tam = atoi(it);
        if(strcmp(aux, "new") == 0){
            int a;
            switch(type[0])
            {
                case 'f':
                    a = first_type(lista, tam);
                    printf("%d", a);
                    break;

                case 'b':
                    a = best_type(lista, tam);
                    printf("%d", a);
                    break;

                case 'w':
                    a = worst_type(lista, tam);
                    printf("%d", a);
                    break;

                case 'n':
                    a = next_type(lista, objeto,tam);
                    printf("%d", a);
                    break;
            }
            lista = InserirNoArea(lista,a,tam);
            objetos = InserirNoCheio(objetos,a,tam);
            objeto->nome = op[3];
            objeto->comeco = a;
            objeto->tamanho = tam;
        }
    }
}
