#include <stdio.h>
#include <locale.h>
#include <stdlib.h>
#include <string.h>

////////////////////////////LIMPAR TELA/////////////////////////////////////

void clear_screen(){
    #ifdef __linux__
        system("clear");
    #elif _WIN32
        system("cls");
    #else

    #endif
}

//////////////////////////////TAMANHO DO HEAP//////////////////////////////////////////

int P = 20;

/////////////////////////////FUNÇÕES AUXILIARES////////////////////////////////////////

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

/////////////////////////////ESTRUTURAS////////////////////////////////////

typedef struct Area{              //Areas que estão livres

    int comeco;
    int tamanho;
    struct Area* prox;

} Area;

typedef struct Cheio{           // Areas que estão cheias

    char nome[97];
    int comeco;
    int tamanho;
    struct Cheio *prox;

} Cheio;

/////////////////////////INSERÇÕES/////////////////////////////////////

Area* InserirNoArea(Area* a, int comeco, int tamanho ){
    if(a!=NULL){
       if(comeco == a->comeco){
            a->comeco = comeco + tamanho;
            a->tamanho -= tamanho;
            if(tamanho == 0){
                Area *b = a->prox;
                free(a);
                return b;
            }
       }else {
        a->prox = InserirNoArea(a->prox,comeco,tamanho);
       }
    } else{
        Area* b = (Area*)malloc(sizeof(Area));
        b->comeco=comeco;
        b->tamanho=tamanho;
        b->prox=NULL;
        return b;
    }
    return a;
}

Cheio* InserirNoCheio(Cheio* a, int comeco, int tamanho, char nome[] ){
    if(a==NULL){

        Cheio* b = (Cheio*)malloc(sizeof(Cheio));
        strcpy(b->nome,nome);
        b->comeco = comeco;
        b->tamanho = tamanho;
        b->prox = NULL;
        return b;

    } else {
        a->prox = InserirNoCheio(a->prox, comeco, tamanho, nome);
        return a;
    } 
}

Cheio* InserirObjeto(Cheio* a,int comeco, int tamanho){
    if(a == NULL){
        Cheio * b = (Cheio*)malloc(sizeof(Cheio));
        b->comeco = comeco;
        b->tamanho = tamanho;
        return b;
    }else{
        free(a);
        Cheio * b = (Cheio*)malloc(sizeof(Cheio));
        b->comeco = comeco;
        b->tamanho = tamanho;
        return b;
    }
}

////////////////////////////FUNÇÕES PARA HEAP////////////////////////////////////

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

void ExibirCheio(Cheio *a){
    if(a != NULL){
        printf("\n\n Nome: %s \n Comeco: %d \n Final: %d \n Tamanho: %d \n", a->nome, a->comeco, (a->comeco + a->tamanho), a->tamanho);
        ExibirCheio(a->prox);
    }
}

/////////////////////////////FUNÇÕES PROCURADORAS/////////////////////////////////

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
            if((Search_maior(lista->prox, x) > lista->tamanho))
                return Search_maior(lista->prox, x);
            else
                return lista->tamanho;
        else
            return Search_maior(lista->prox, x);
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

////////////////////////////FUNÇÕES ESCOLHAS DE TIPO///////////////////////////////////////

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

/////////////////////////////FUNÇÕES PARA REMOÇÃO///////////////////////////////////////
Area* EspacoAntes(Area* a,int comeco){
    if(a == NULL){
        return NULL;
    }else{
        if((a->comeco + a->tamanho + 1) == comeco){
            return a;
        }else 
            a = EspacoAntes(a->prox, comeco);
    }
}

Area* EspacoDepois(Area* a,int comeco, int tamanho){
    if(a == NULL){
        return NULL;
    }else{
        if((a->comeco) == comeco + tamanho){
            return a;
        }else 
            a = EspacoAntes(a->prox, comeco);
    }
}

Area* RemoverArea(Area* a,int comeco,int tamanho){
    if(EspacoAntes(a,comeco) != NULL){
        Area* b = (Area*)malloc(sizeof(Area));
        b = EspacoAntes(a,comeco);
        b->tamanho += tamanho;
        return a;
    }
    else if(EspacoDepois(a, comeco, tamanho) != NULL){
        Area* b = (Area*)malloc(sizeof(Area));
        b = EspacoDepois(a,comeco, tamanho);
        b->comeco -= tamanho;
        b->tamanho += tamanho;
        return a;
    }
    else{
       a = InserirNoArea(a, comeco, tamanho);
       return a;
    }
}

Area *OrdenarAreas(Area *lista) {
        if (lista == NULL || lista->prox == NULL) {
        // Lista vazia ou com apenas um elemento já está ordenada
        return lista;
    }

    Area *ordenado = NULL; // Nova lista ordenada

    while (lista != NULL) {
        Area *atual = lista;
        lista = lista->prox; // Remove o nó atual da lista original

        // Insere o nó atual na posição correta da lista ordenada
        if (ordenado == NULL || atual->comeco < ordenado->comeco) {
            atual->prox = ordenado;
            ordenado = atual;
        } else {
            Area *temp = ordenado;
            while (temp->prox != NULL && temp->prox->comeco < atual->comeco) {
                temp = temp->prox;
            }
            atual->prox = temp->prox;
            temp->prox = atual;
        }
    }

    return ordenado;
}

Cheio *RemoverCheio(Cheio *a, int comeco) {
    
    if (a == NULL) {
        return NULL;
    }

    
    if (a->comeco == comeco) {
        Cheio *prox = a->prox; 
        free(a);               
        return RemoverCheio(prox, comeco); 
    }

    
    a->prox = RemoverCheio(a->prox, comeco);
    return a;
}

Cheio *ProcuraRemover(Cheio *ob, char b[]){
    if(ob == NULL){
        return NULL;
    }else{
        if(strcmp(ob->nome,b)==0){
            return ob;
        }else{
            return ProcuraRemover(ob->prox,b);
        }
    }
}
//////////////////////////MAIN//////////////////////////////////////

int main()
{  
    setlocale(LC_ALL,"Portuguese");
    Area* lista = (Area*)malloc(sizeof(Area));
    lista = NULL;
    lista = InserirNoArea(lista,0,20);
    Cheio* objetos = (Cheio*)malloc(sizeof(Cheio));
    objetos = NULL;
    Cheio* objeto = (Cheio*)malloc(sizeof(Cheio));
    objeto = NULL;
    

    char type[6] = {"first"};      

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
        char op[100];       
        printf("Qual o operação deseja fazer(implementar, tipo, exibir, ajuda, sair)? ");
        char confirm[20];
        fgets(confirm, 20,stdin);
       

        if(confirm[0] == 'i' || confirm[0] == 'I'){

            clear_screen();
            printf("Digite a operacao desejada(new a 6, del a, c = a): ");
            fgets(op, 100, stdin);
            char aux[100];
            for(int i=0; op[i] != ' ';i++){
                aux[i] = op[i];
            }
            RemoveEspacos(op);
            op[strcspn(op,"\n")]=0;
            
            if(strcmp(aux, "new") == 0){
                char nome[97];
                strcpy(nome,op+3);
                for(int i=0;nome[i]!='\0';i++){
                    if(nome[i]>47 && nome[i]<58){
                        nome[i]='\0';
                    }
                }
                char it[2];
                it[0]=op[4];
                int tam = atoi(it);
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
                objetos = InserirNoCheio(objetos,a,tam,nome);
                objeto = InserirObjeto(objeto,a,tam);
            }else if(strcmp(aux, "del") == 0){
                char nome[97];
                strcpy(nome,op+3);
                Cheio *b = (Cheio*)malloc(sizeof(Cheio));
                b = ProcuraRemover(objetos, nome);
                lista = RemoverArea(lista,b->comeco,b->tamanho);
                objetos = RemoverCheio(objetos,b->comeco);
                lista = OrdenarAreas(lista);
            }else{

                char aux2[100];
                int k = 0;
                for(int i=0; op[i] != '\0';i++){
                    if(op[i] == '='){
                        for(int j = i+1; op[j]!='\0';j++){
                            aux2[k]=op[j];
                            k++;
                        }
                    }
                }
                aux2[k]='\0';
                Cheio *b = (Cheio*)malloc(sizeof(Cheio));
                b = ProcuraRemover(objetos, aux2);
                objetos = InserirNoCheio(objetos, b->comeco, b->tamanho, aux2);
                free(b);
            }

        }

        if(confirm[0] == 't' || confirm[0] == 'T'){

            clear_screen();
            LimparType(type);
            printf("Digite o tipo de operacao desejada(first,best,worst,next): ");
            fgets(type, 6, stdin);
            
        }

        if(confirm[0] == 'e' || confirm[0] == 'E'){

            clear_screen();
            printf("                            Heap\n");
            ImprimeHeap(heap);
            printf("\n\n\n");
            ExibirCheio(objetos);
            getchar();

        }

        if(confirm[0] == 'a' || confirm[0] == 'A'){

            clear_screen();
            printf("\n     Bem-vindo a area de ajuda    \nAqui voce encontra o que alguns comandos fazem, para facilitar sua navegação:\n\n");
            printf("     Qual o operação deseja fazer?     " );
            printf("\nimplementar: O comando 'implementar' te possibilita ir para area de implementação onde poderá alocar um novo espaço ao heap ou deletar um espaço do heap,\nalém de poder fazer atribuições.");
            printf("\ntipo: O comando 'tipo' te possibilita mudar o tipo de implementação feito podendo variar entre: first, best, worst, next.");
            printf("\nexibir: O comando 'exibir' te possibilita exibir todos o espaços alocados no heap com suas devidas informações.");
            printf("\najuda: O comando 'ajuda' te possibilita entrar no menu de ajuda em que estamos.");
            printf("\nsair: O comando 'sair' te possibilita fechar o programa.\n\n\n");
            printf("Implementar:\n\n");
            printf("     Digite a operação desejada:     ");
            printf("\nnew: O comando 'new' aloca um novo espaço no heap, sua sintaxe é: new 'nome' 'tamanho'");
            printf("\ndel: O comando 'del' exclui um alocação ja existente no heap, sua sintaxe é: del 'nome'");
            printf("\natribuição: tambem é possivel fazer a atribuição, para isso voce deverá usar a seguinte sintexe: 'nome_do_novo_a_ser_atribuido' = 'nome_de_qual_deseja_atribuir'\n\n\n");
            printf("Tipo:\n\n");
            printf("     Digite o tipo de operacao desejada:     ");
            printf("\nfirst: O camando 'first' define o tipo de alocação para first.");
            printf("\nbest: O camando 'best' define o tipo de alocação para best.");
            printf("\nworst: O camando 'worst' define o tipo de alocação para worst.");
            printf("\nnext: O camando 'next' define o tipo de alocação para next.\n\n\n");
            getchar();

        }
        if(confirm[0] == 's' || confirm[0] == 'S'){

            clear_screen();
            printf("Obrigado por usar nosso programa :)\n\n");
            printf("Aperte qualquer tecla para fecha-lo: ");
            getchar();
            clear_screen();
            break;
        }
    }
    free(lista);
    free(objetos);
    free(objetos);
    return 0;
}