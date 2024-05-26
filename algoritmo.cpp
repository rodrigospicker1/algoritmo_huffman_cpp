
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

int main(int argc, char *argv[])
{
    setlocale(LC_ALL,"");

    unsigned char texto[] = "Vamos aprender a programa";
    unsigned int tabela_frequencia[TAM];

    inicializa_tabela_com_zero(tabela_frequencia);
    preenche_tab_frequencia(texto, tabela_frequencia);
    imprime_tab_frequencia(tabela_frequencia);

    return 0;
}
