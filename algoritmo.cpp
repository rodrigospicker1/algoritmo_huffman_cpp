
/**
* huffman.cpp
* Um simples compressor de arquivos usando árvores de Huffman.
* Rodrigo Spicker
*/

#include <iostream>
#include <fstream>
#include <queue>
#include <bitset>
#include <cstring>
#include <ctime>
#include <map>
#include <locale.h>
#include <string>

using namespace std;

#define TAM 256

struct No
{
    unsigned char caracter;
    int frequencia;
    No *esq;
    No *dir;
    No *proximo;
};

struct Lista{
    No *inicio;
    int tam;
};

void inicializa_tabela_com_zero(unsigned int tab[])
{
    for(int i = 0; i < TAM; i++)
        tab[i] = 0;
}

void preenche_tab_frequencia(unsigned char text[], unsigned int tab[])
{
    int i = 0;

    while(text[i] != '\0')
    {
        tab[text[i]]++;
        i++;
    }
}

void imprime_tab_frequencia(unsigned int tab[])
{
    cout<<"\nTabela frequência\n";
    for(int i = 0; i < TAM; i++)
    {
        if(tab[i] > 0)
        {
            char letra = i;
            cout<<i<<" = "<<tab[i]<<" = "<<letra<<endl;
        }
    }
}

void criar_lista(Lista *lista)
{
    lista->inicio = nullptr;
    lista->tam = 0;
}

void inserir_ordenado(Lista *lista, No *no){
    No *aux;
    // a lista esta vazia?
    if(lista -> inicio == NULL)
    {
        lista -> inicio = no;
    }
    //tem freqencia menor que o incio da lista ?
    else if(no -> frequencia < lista -> inicio -> frequencia)
    {
        no -> proximo = lista ->inicio;
        lista->inicio = no;
    }
    else
    {
        aux = lista -> inicio;
        while(aux -> proximo && aux -> proximo -> frequencia <= no -> frequencia)
            aux = aux -> proximo;
        no -> proximo = aux -> proximo;
        aux -> proximo = no;

    }
    lista -> tam++;
}

void preencher_lista(unsigned int tab[],Lista *lista)
{
    int i;
    No *novo;
    for(i = 0; i < TAM; i++)
        if(tab[i]>0)
        {
            novo = (No*)malloc(sizeof(No));
            if(novo)
            {
                novo -> caracter = i;
                novo -> frequencia = tab[i];
                novo -> dir = NULL;
                novo -> esq = NULL;
                novo -> proximo = NULL;

                inserir_ordenado(lista,novo);
            }
            else
            {
                cout<<"\n ERRO ao alocar memoria em preencher_lista!\n";
                break;
            }
        }
}

void imprimir_lista(Lista *lista){
    No *aux  = lista->inicio;

    cout<<"\nLista ordenada: Tamanho: "<<lista->tam;
    while(aux){
        cout<<"\nCaracter: "<<aux->caracter<<" Frequência: "<<aux->frequencia;
        aux = aux->proximo;
    }
}

No *remove_no_inicio(Lista *lista){
    No *aux = nullptr;

    if(lista->inicio){
        aux = lista->inicio;
        lista->inicio = aux->proximo;
        aux->proximo = nullptr;
        lista->tam--;
    }

    return aux;
}

No* montar_arvore(Lista *lista)
{
    No *primeiro, *segundo, *novo;
    while(lista->tam > 1)
    {
        primeiro = remove_no_inicio(lista);
        segundo = remove_no_inicio(lista);
        novo = (No*)malloc(sizeof(No));

        if(novo){
            novo->caracter = '+';
            novo->frequencia = primeiro->frequencia + segundo->frequencia;
            novo->esq = primeiro;
            novo->dir = segundo;
            novo->proximo = nullptr;

            inserir_ordenado(lista, novo);
        }
        else{
            cout<<"\nErro ao alocar memoria em montar arvore!\n";
            break;
        }
    }
    return lista->inicio;
}

void imprimir_arvore(No *raiz, int tam)
{
    if(raiz->esq == NULL && raiz->dir == NULL){
        cout<<"Folha: "<<raiz->caracter<<"\tAltura: "<<tam<<endl;
    }else{
        imprimir_arvore(raiz->esq, tam + 1);
        imprimir_arvore(raiz->dir, tam + 1);
    }
}

int altura_arvore(No *raiz){
    int esq, dir;

    if(raiz == NULL)
        return -1;
    else
        esq = altura_arvore(raiz->esq) + 1;
        dir = altura_arvore(raiz->dir) + 1;

    if(esq > dir)
        return esq;
    else
        return dir;
}


char** aloca_dicionario(int colunas){
    char **dicionario;
    int i;

    dicionario = (char**)malloc(sizeof(char*) * TAM);

    for(i = 0; i < TAM; i++){
        dicionario[i] = (char*)calloc(colunas, sizeof(char));
    }

    return dicionario;
}

void gerar_dicionario(char **dicionario, No *raiz, char *caminho, int colunas){
    char esquerda[colunas], direita[colunas];

    if(raiz->esq == NULL && raiz->dir == NULL){
        strcpy(dicionario[raiz->caracter], caminho);
    }
    else{
        strcpy(esquerda, caminho);
        strcat(esquerda, "0");
        gerar_dicionario(dicionario, raiz->esq, esquerda, colunas);

        strcpy(direita, caminho);
        strcat(direita,"1");
        gerar_dicionario(dicionario, raiz->dir,direita,colunas);
    }
}


void imprime_dicionario(char **dicionario)
{
    cout<<"\n\nDicionário: \n";
    for(int i = 0; i < TAM; i++){
        if( strlen(dicionario[i]) > 0 )
            cout<<i<<": "<<dicionario[i]<<endl;
    }
}

int main(int argc, char *argv[])
{
    setlocale(LC_ALL,"");

    unsigned char texto[] = "Vamos aprender a programa";
    unsigned int tabela_frequencia[TAM];
    Lista lista;
    No *arvore;
    int colunas;
    char **dicionario;

    inicializa_tabela_com_zero(tabela_frequencia);
    preenche_tab_frequencia(texto, tabela_frequencia);
    imprime_tab_frequencia(tabela_frequencia);

    criar_lista(&lista);
	preencher_lista(tabela_frequencia, &lista);
	imprimir_lista(&lista);

	arvore = montar_arvore(&lista);
    cout<<"\n\nÁrvore de Huffmanz\n";
	imprimir_arvore(arvore, 0);

	colunas = altura_arvore(arvore) + 1;
	dicionario = aloca_dicionario(colunas);
	gerar_dicionario(dicionario, arvore, "", colunas);
	imprime_dicionario(dicionario);

    return 0;
}
