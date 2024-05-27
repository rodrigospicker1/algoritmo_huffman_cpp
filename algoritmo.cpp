
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

int main(int argc, char *argv[])
{
    setlocale(LC_ALL,"");

    unsigned char texto[] = "Vamos aprender a programa";
    unsigned int tabela_frequencia[TAM];
    Lista lista;

    inicializa_tabela_com_zero(tabela_frequencia);
    preenche_tab_frequencia(texto, tabela_frequencia);
    imprime_tab_frequencia(tabela_frequencia);

    criar_lista(&lista);
	preencher_lista(tabela_frequencia, &lista);
	imprimir_lista(&lista);

    return 0;
}
