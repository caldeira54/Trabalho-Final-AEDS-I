#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string>
#define MaxTam 1000

using namespace std;

typedef struct TipoItem{
    int id;
} TipoItem;

typedef struct TipoElemento *Apontador;

typedef struct TipoElemento{
    TipoItem item;
    struct TipoElemento *prox;
}TipoElemento;

typedef struct TipoLista{
    Apontador primeiro, ultimo;
}TipoLista;

typedef struct Pilha{
    TipoItem itens[MaxTam];
    int topo;
} TipoPilha;

typedef struct fila{
    TipoItem itens[MaxTam + 1];
    int frente, tras;
} TipoFila;

typedef struct sacola{
    bool eh_fila = true;
    bool eh_pilha = true;
    bool eh_lista = true;
    TipoLista lista;
    TipoFila fila;
    TipoPilha pilha;
}TipoSacola;

////////////////////////LISTA INICIO//////////////////////////////

void criaListaVazia(TipoLista *lista){
  lista->primeiro = (Apontador) malloc (sizeof (TipoElemento));
  lista->ultimo = lista->primeiro;
  lista->ultimo->prox = NULL;
}

bool verificaListaVazia(TipoLista *lista){
  if(lista->primeiro == lista->ultimo){
      return true;
  }else{
      return false;
  }
}

void insereListaPrimeiro(TipoLista *lista, TipoItem *item){
    Apontador aux;
    aux = lista->primeiro->prox;
    lista->primeiro->prox = (Apontador) malloc (sizeof (TipoElemento));
    lista->primeiro->prox->prox = aux;
    lista->primeiro->prox->item = *item;
}

void insereAposItem(TipoLista *lista, TipoItem *item, int idEleX){
    Apontador aux, pAux, ele_x;
    pAux = lista->primeiro->prox;
    while (pAux != NULL){
        if(pAux->item.id == idEleX){
            ele_x = pAux;
            aux = ele_x->prox;
            ele_x->prox = (Apontador) malloc (sizeof (TipoElemento));
            ele_x->prox->prox = aux;
            ele_x->prox->item = *item;
            break;
        }
        pAux = pAux->prox;
    }
}

void insereListaUltimo(TipoLista *lista, TipoItem *item){
  lista->ultimo->prox = (Apontador) malloc (sizeof (TipoElemento));
  lista->ultimo = lista->ultimo->prox;
  lista->ultimo->item = *item;
  lista->ultimo->prox = NULL;
}

void removeListaUltimo(TipoLista *lista, TipoItem *item){
    Apontador aux, anterior, x;

    if (verificaListaVazia(lista)){
        return ;
    }

    x = lista->primeiro;
    while(x != NULL){
        if(x->prox == lista->ultimo){
            anterior = x;
            break;
        }
        x = x->prox;
    }

    aux = lista->ultimo;
    lista->ultimo = anterior;
    *item = aux->item;
    lista->ultimo->prox = NULL;
    free(aux);
}

////////////////////////LISTA FIM///////////////////////////////

////////////////////////PILHA INICIO//////////////////////////////

void fazPilhaVazia(TipoPilha *pilha){
    pilha->topo = 0;
}

bool verificaPilhaVazia(TipoPilha *pilha){
    if(pilha->topo == 0){
        return true;
    }else{
        return false;
    }
}

void empilha(TipoPilha *pilha, TipoItem item){
    if(pilha->topo == MaxTam){
        printf("Pilha Cheia");
    }else{
        pilha->itens[pilha->topo] = item;
        pilha->topo++;
    }
}

void desempilha(TipoPilha *pilha, TipoItem *item){
    if(verificaPilhaVazia(pilha)){
        printf("A pilha esta vazia");
        return;
    }else{
        pilha->topo--;
        *item = pilha->itens[pilha->topo];
    }
}

////////////////////////PILHA FIM///////////////////////////////

////////////////////////FILA INICIO///////////////////////////////

void fazFilaVazia(TipoFila *fila){
    fila->frente = 0;
    fila->tras = 0;
}

bool verificaFilaVazia(TipoFila *fila){
    if(fila->frente == fila->tras){
        return true;
    }else{
        return false;
    }
}

bool verificaFilaCheia(TipoFila *fila){
    if (((fila->tras + 1) % (MaxTam + 1)) == fila->frente){
        return true;
    }else{
        return false;
    }
}

void enfileira(TipoFila *fila, TipoItem item){
    if (verificaFilaCheia(fila)){
        printf("Fila Cheia");
        return;
    }else{
        fila->itens[fila->tras] = item;
        fila->tras = (fila->tras + 1) % (MaxTam + 1);
    }
}

void desenfileira(TipoFila *fila, TipoItem *item){
    if(verificaFilaVazia(fila)){
        printf("Fila Vazia");
        return;
    }else{
        *item  = fila->itens[fila->frente];
        fila->frente = (fila->frente + 1) % (MaxTam + 1);
    }
}

////////////////////////FILA FIM///////////////////////////////

///////////////////////////Insere ordenado INICIO////////////////////////////////////

void insereListaOrdenado(TipoLista *lista, TipoItem *item){
    Apontador aux,x,anterior = NULL;

    if(verificaListaVazia(lista)){
        insereListaUltimo(lista,item);
        return;
    }

    x = lista->primeiro;
    while(x->prox != NULL){
        if(x->prox->item.id > item->id){
            anterior = x;
            break;
        }
        x = x->prox;
    }
    if(anterior == NULL){
        insereListaUltimo(lista,item);
        return;
    }
    if(anterior == lista->primeiro){
        insereListaPrimeiro(lista,item);
        return;
    }
    insereAposItem(lista,item,anterior->item.id);
}

///////////////////////////Insere ordenado FIM////////////////////////////////////

///////////////////////// Operações na Sacola INICIO /////////////////////////////////

void sacola_insere(TipoSacola *sacola,int id){
    TipoItem item;
    item.id = id;

    insereListaOrdenado(&(sacola->lista),&item);
    enfileira(&(sacola->fila),item);
    empilha(&(sacola->pilha),item);
}

void sacola_remove(TipoSacola *sacola,int id){
    TipoItem item;
    item.id = id;

    if(sacola->eh_pilha == true){
        if(verificaPilhaVazia(&(sacola->pilha))){
            sacola->eh_pilha = false;
        } else {
            desempilha(&(sacola->pilha),&item);
            if(item.id != id){
                sacola->eh_pilha = false;
            }
        }
    }

    if(sacola->eh_fila == true){
        if(verificaFilaVazia(&(sacola->fila))){
            sacola->eh_fila = false;
        } else {
            desenfileira(&(sacola->fila),&item);
            if(item.id != id){
                sacola->eh_fila = false;
            }
        }
    }

    if(sacola->eh_lista == true){
        if(verificaListaVazia(&(sacola->lista))){
            sacola->eh_lista = false;
        } else {
            removeListaUltimo(&(sacola->lista),&item);
            if(item.id != id){
                sacola->eh_lista = false;
            }
        }
    }
}

void sacola_resultado(TipoSacola *sacola,FILE **arq){
    *arq = fopen("Resultado parte 1.txt","a");
    int cont = 0;
    if(arq != NULL){
        if(sacola->eh_lista == true){
            cont++;
        }
        if(sacola->eh_pilha == true){
            cont++;
        }
        if(sacola->eh_fila == true){
            cont++;
        }
        if(sacola->eh_pilha == true && sacola->eh_lista == true){
            cont++;
        }
        if(sacola->eh_fila == true && sacola->eh_lista == true){
            cont++;
        }

        if(cont == 0){
            fprintf(*arq,"Nenhuma!\n");
        } else if(cont == 1){
            if(sacola->eh_lista == true){
                fprintf(*arq,"Lista de Prioridade!\n");
            }
            if(sacola->eh_pilha == true){
                fprintf(*arq,"Pilha!\n");
            }
            if(sacola->eh_fila == true){
                fprintf(*arq,"Fila!\n");
            }
        } else{
            fprintf(*arq,"Sei nao!\n");
        }
    }

    fclose(*arq);
}

void sacola_reseta(TipoSacola *sacola){
    criaListaVazia(&(sacola->lista));
    fazFilaVazia(&(sacola->fila));
    fazPilhaVazia(&(sacola->pilha));
    sacola->eh_lista = true;
    sacola->eh_pilha = true;
    sacola->eh_fila = true;
}

///////////////////////// Operações na Sacola FIM /////////////////////////////////

int main(){

    TipoSacola sacola;
    TipoLista lista;
    TipoItem item;
    FILE *arq;
    FILE *arq_saida;
    char nome[1000];
    int num,quant_linhas,cont;
    int numero[1000],instrucao[1000];
    char linha[1000];

    cout << "Digite o nome do arquivo a ser aberto com a extensao : ";
    cin >> nome;

    arq = fopen(nome,"r");
    if(arq != NULL){
        cout << "Arquivo aberto com sucesso!" << endl;
    } else {
        cout << "Nao foi possivel abrir o arquivo!" << endl;
    }

    while(fgets(linha,999,arq)){
        // Cria uma pilha,lista e fila vazias
        // Volta os valores dos booleanos para true
        sacola_reseta(&sacola);
        cont = 0;
        quant_linhas = atoi(linha);
        for(int i = 0;i < quant_linhas;i++){
            fgets(linha,999,arq);
            instrucao[i] = atoi(linha);
            numero[i] = atoi(&(linha[2]));
            if(instrucao[i] == 1){
                // Adiciona na sacola
                sacola_insere(&sacola,numero[i]);
            } else if(instrucao[i] == 2) {
                // Remove da sacola
                sacola_remove(&sacola,numero[i]);
            }
        }
        // Checa os 3 booleanos e decide o tipo
        sacola_resultado(&sacola,&arq_saida);
    }

    fclose(arq);

    return 0;
}
