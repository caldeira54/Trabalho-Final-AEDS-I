#include <iostream>
#include <string>
#include <stdlib.h>
#include <stdbool.h>
#define MaxTam 1000

using namespace std;

typedef struct TipoItem{
    int id;
} TipoItem;

typedef struct Pilha{
    TipoItem itens[MaxTam];
    int topo;
} TipoPilha;

typedef struct fila{
    TipoItem itens[MaxTam + 1];
    int frente, tras;
} TipoFila;

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
        printf("A pilha esta vazia\n");
        return;
    }else{
        pilha->topo--;
        *item = pilha->itens[pilha->topo];
    }
}

void fazFilaVazia(TipoFila *fila){
    fila->frente = 0;
    fila->tras = 0;
}

bool verificaFilaCheia(TipoFila *fila){
    if (((fila->tras+1) % (MaxTam+1)) == fila->frente){
        return true;
    }else{
        return false;
    }
}

void Enfileira(TipoFila *fila, TipoItem item){
    if (verificaFilaCheia(fila)){
        printf("Fila Cheia");
        return;
    }else{
        fila->itens[fila->tras] = item;
        fila->tras = (fila->tras + 1) % (MaxTam+1);
    }
}

void define_ultimo_item(TipoPilha *pilha){
    TipoItem item,item2;
    TipoPilha pilha_aux;
    fazPilhaVazia(&pilha_aux);
    desempilha(pilha,&item2);
    while(!verificaPilhaVazia(pilha)){
        desempilha(pilha,&item);
        empilha(&pilha_aux,item);
    }
    empilha(pilha,item2);
    while(!verificaPilhaVazia(&pilha_aux)){
        desempilha(&pilha_aux,&item);
        empilha(pilha,item);
    }
}

int getTamanho(TipoPilha *pilha){
    return pilha->topo;
}

void grava_no_arquivo(FILE **arq,TipoPilha *pilha,TipoFila *fila){
    *arq = fopen("Resposta parte 2.txt","a");
    if(*arq != NULL){
        fprintf(*arq,"Cartas descartadas : ");
        for(int i = fila->frente;i < fila->tras;i++){
            if(i == fila->frente){
                fprintf(*arq,"%d",fila->itens[i].id);
            } else {
                fprintf(*arq,", ");
                fprintf(*arq,"%d",fila->itens[i].id);
            }
        }
        fprintf(*arq,"\n");
        fprintf(*arq,"Cartas remanescente : ");
        for(int i = pilha->topo - 1;i >= 0;i--){
            fprintf(*arq,"%d",pilha->itens[i].id);
        }
        fprintf(*arq,"\n");
        fprintf(*arq,"\n");
    }

    fclose(*arq);
}

int main(){

    FILE *arq;
    FILE *arq_saida;
    TipoItem item;
    TipoPilha pilha;
    TipoFila fila;
    int tamanho;
    char nome [1000];
    char aux[1000];

    cout << "Digite o nome do arquivo a ser aberto com a extensao : ";
    cin >> nome;

    arq = fopen(nome,"r");

    if(arq != NULL){
        cout << "Arquivo aberto com sucesso!" << endl;
    } else {
        cout << "Nao foi possivel abrir o arquivo" << endl;
    }

    do{
        fazPilhaVazia(&pilha);
        fazFilaVazia(&fila);
        fgets(aux,1000,arq);
        tamanho = atoi(aux);
        for(int i = tamanho; i >= 1; i--){
            item.id = i;
            empilha(&pilha,item);
        }
        while(getTamanho(&pilha) > 1){
            desempilha(&pilha,&item);
            Enfileira(&fila,item);
            define_ultimo_item(&pilha);
        }

        if(tamanho != 0){
            grava_no_arquivo(&arq_saida,&pilha,&fila);
        }
    }while(tamanho != 0);

    fclose(arq);

    return 0;
}
