// Bibliotecas
#include <stdio.h>
#include <stdbool.h>
#include "headers/manq.h"

// Constantes
#define SUCESSO       0
#define FALHA_MALLOC -1

/**
 * @brief Funcao principal (ponto de entrada) do codigo.
 * @param argc Numero de argumentos passados pela linha de comando
 * @param argv Valor dos argumentos passados pela linha de comando
 * @return SUCESSO
 */
int main(int argc, int argv) {
    uint nval;

    (void)scanf("%u%*c", &nval);

    matriz_t_bdop * matriz_x = NULL, * matriz_y = NULL;

    matriz_x = criar_matriz(nval, 1);
    matriz_y = criar_matriz(nval, 1);

    if(matriz_x == NULL || matriz_y == NULL) {
        return FALHA_MALLOC;
    }
    
    for(int i = 0; i < matriz_x->altura; i++) {
        for(int j = 0; j < matriz_x->largura; j++) {
            (void)scanf("%lf", &(matriz_x->elementos[i][j]));
        }
    }

    for(int i = 0; i < matriz_y->altura; i++) {
        for(int j = 0; j < matriz_x->largura; j++) {
            (void)scanf("%lf", &(matriz_y->elementos[i][j]));
        }
    }

    imprimir_valores(ajuste(matriz_x, matriz_y, 2));

    /* Se chegou aqui eh porque correu tudo bem. */
    return SUCESSO;
}