
/**
* huffman.cpp
* Um simples compressor de arquivos usando árvores de Huffman.
* Autor: Rodrigo Spicker
*/

#include <iostream>
#include <fstream>
#include <cstring>
#include <locale.h>

using namespace std;

#define TAM 256

struct No
{
    unsigned char caracter;
    int quantidade;
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

void preenche_tab_quantidade(unsigned char text[], unsigned int tab[])
{
    int i = 0;

    while(text[i] != '\0')
    {
        tab[text[i]]++;
        i++;
    }
}

void imprime_tab_quantidade(unsigned int tabela[])
{
    cout<<"\nTabela frequência\n";
    for(int i = 0; i < TAM; i++)
    {
        if(tabela[i] > 0)
        {
            char letra = i;
            cout<<i<<" = "<<tabela[i]<<" = "<<letra<<endl;
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
    else if(no -> quantidade < lista -> inicio -> quantidade)
    {
        no -> proximo = lista ->inicio;
        lista->inicio = no;
    }
    else
    {
        aux = lista -> inicio;
        while(aux -> proximo && aux -> proximo -> quantidade <= no -> quantidade)
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
                novo -> quantidade = tab[i];
                novo -> dir = NULL;
                novo -> esq = NULL;
                novo -> proximo = NULL;

                inserir_ordenado(lista,novo);
            }
            else
            {
                break;
            }
        }
}

void imprimir_lista(Lista *lista){
    No *aux  = lista->inicio;

    cout<<"\nLista ordenada: "<<lista->tam;
    while(aux){
        cout<<"\nLetra: "<<aux->caracter<<" Quantidade: "<<aux->quantidade;
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
            novo->quantidade = primeiro->quantidade + segundo->quantidade;
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
        if( strlen(dicionario[i]) > 0 ){
            char letra = i;
            cout<<letra<<": "<<dicionario[i]<<endl;
        }
    }
}

int calcula_tamanho_string(char **dicionario, unsigned char *texto) {
    int tam = 0, i = 0;
    while (texto[i] != '\0') {
        tam += strlen(dicionario[texto[i]]);
        i++;
    }
    return tam + 1;
}

char* codificar(char **dicionario, unsigned char *texto) {
    int i = 0;
    int tam = calcula_tamanho_string(dicionario, texto);
    char *codigo = (char*)calloc(tam, sizeof(char));
    while (texto[i] != '\0') {
        strcat(codigo, dicionario[texto[i]]);
        i++;
    }
    return codigo;
}


int main(int argc, char *argv[])
{
    setlocale(LC_ALL,"");

    cout<<"BEM-VINDO AO ALGORITMO DE HUFFMAN: "<<endl;

    unsigned char texto[] = "Vamos aprender a programar";
    cout<<"Palavra decodificada: "<<texto<<endl;
    cout<<"(Mude a variável 'texto' para conseguir outro resultado)\n\n";
    unsigned int tabela_quantidade[TAM];
    Lista lista;
    No *arvore;
    int colunas;
    char **dicionario;
    char *codificado;

    inicializa_tabela_com_zero(tabela_quantidade);
    preenche_tab_quantidade(texto, tabela_quantidade);
    imprime_tab_quantidade(tabela_quantidade);

    criar_lista(&lista);
	preencher_lista(tabela_quantidade, &lista);
	imprimir_lista(&lista);

	arvore = montar_arvore(&lista);
    cout<<"\n\nÁrvore de Huffmanz\n";
	imprimir_arvore(arvore, 0);

	colunas = altura_arvore(arvore) + 1;
	dicionario = aloca_dicionario(colunas);
	gerar_dicionario(dicionario, arvore, "", colunas);
	imprime_dicionario(dicionario);

	codificado = codificar(dicionario, texto);
	cout<<"\nTexto codificado: "<<codificado;

    return 0;
}
