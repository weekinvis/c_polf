#pragma once // Primeira guarda
#ifndef _manq_h__ // Segunda guarda
#define _manq_h__

typedef unsigned int uint; // Mais conveniente.

/**
 * @struct 
 * @brief matriz_s_b Contem os dados de uma matriz necessarios
 */
typedef struct matriz_s_b {

    double ** elementos;
    uint altura;
    uint largura;

} matriz_t_bdop;

// Funcoes exportadas
matriz_t_bdop* criar_matriz(uint altura, uint largura);
void liberar_matriz(matriz_t_bdop * matriz_existente);
void imprimir_valores(const matriz_t_bdop * matriz_lendo);
matriz_t_bdop * juntar_a_direita(const matriz_t_bdop * m1, const matriz_t_bdop * m2);
matriz_t_bdop * substituir(matriz_t_bdop * m1, matriz_t_bdop * m2);
matriz_t_bdop * elevar_elementos_matriz(matriz_t_bdop * m, uint exp);
matriz_t_bdop * ajustar_matriz_x(matriz_t_bdop * mx, uint gr);
matriz_t_bdop * transpor_matriz(const matriz_t_bdop * mt);
matriz_t_bdop * multiplica_matriz(const matriz_t_bdop * m1, const matriz_t_bdop * m2);
matriz_t_bdop * inverte_matriz(const matriz_t_bdop * m);
matriz_t_bdop * ajuste(matriz_t_bdop * x, matriz_t_bdop * y, uint grau);

#endif // _manq_h__