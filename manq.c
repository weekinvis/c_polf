// Bibliotecas
#include "headers/manq.h"
#include <stdlib.h>
#include <stdio.h>
#include <math.h>

// Constantes
#define CODIGO_LETRA_VERMELHA_TERMINAL "\033[31m"
#define CODIGO_RESET_LETRA_TERMINAL "\033[0m"

#define EPS 1e-9

// Declaracao de funcoes

/**
 * @brief Funcao que imprime erros personalizados.
 * @param mensagem Mensagem que deseja imprimir.
 */
static void imprime_erro(const char * mensagem) {
    fprintf(stderr, "%s%s%s\n", CODIGO_LETRA_VERMELHA_TERMINAL, mensagem, CODIGO_RESET_LETRA_TERMINAL);
}

/**
 * @brief Funcao que retorna um apontador para a matriz criada
 * @param altura altura da nova matriz.
 * @param largura largura da nova matriz.
 * @return Nao NULL caso sucesso, NULL caso fracasso.
 */
matriz_t_bdop * criar_matriz(uint altura, uint largura) {

    // Crio a variavel.
    matriz_t_bdop * matriz_criada = NULL;

    // Tento alocar para a matriz.
    matriz_criada = (matriz_t_bdop*)malloc(sizeof(matriz_t_bdop));
    
    // Se o malloc falhar.
    if(matriz_criada == NULL) {
        imprime_erro("Falha no malloc.");
        return matriz_criada;
    }

    // Inicializo alguns valores.
    matriz_criada->altura = altura;
    matriz_criada->largura = largura;

    // Aloco a memoria para o eixo X da matriz.
    matriz_criada->elementos = (double**)malloc(sizeof(double*) * altura);

    // Se o malloc falhar
    if(matriz_criada->elementos == NULL) {
        imprime_erro("Falha no malloc.");
        free(matriz_criada);
        return NULL;
    }

    for(int i = 0; i < altura; i++) {
        
        // Tento alocar a memoria para o eixo Y da matriz. [Tento inicializar os valores com 0]
        matriz_criada->elementos[i] = (double*)calloc(sizeof(double), largura);
        
        // Se o malloc falhar em algum caso libero toda a matriz e retorno NULL
        if(matriz_criada->elementos[i] == NULL) {
            imprime_erro("Falha no malloc.");
            for(int j = 0; j < i; j++) {
                free(matriz_criada->elementos[j]);
                free(matriz_criada->elementos);
                free(matriz_criada);
                return NULL;
            }
        }

    }

    // Retorno a matriz criada
    return matriz_criada;

}

/**
 * @brief Funcao que imprime os valores de uma matriz
 * @param matriz_lendo Matriz que deseja imprimir os valores
 */
void imprimir_valores(const matriz_t_bdop * matriz_lendo) {

    // Se a matriz nao existir
    if(matriz_lendo == NULL) {
        imprime_erro("Tentando ler matriz nula.");
        return;
    }

    // Se a matriz existir
    for(int i = 0; i < matriz_lendo->altura; i++) {
        for(int j = 0; j < matriz_lendo->largura; j++) {
            printf("%-10.3lf ", matriz_lendo->elementos[i][j]);
        }
        printf("\n");
    }

    putc('\n', stdout);
}

/**
 * @brief Funcao que libera um apontador de uma matriz existente
 * @param matriz_existente Matriz que se deseja liberar.
 */
void liberar_matriz(matriz_t_bdop * matriz_existente) {

    // Se a matriz nao existir
    if(matriz_existente == NULL) {
        imprime_erro("Tentando liberar matriz, mas o valor eh NULL.");
        return;
    }

    // Se ela existir mas com parametros impossiveis
    if(matriz_existente->altura < 0 || matriz_existente->largura < 0) {
        imprime_erro("Matriz com dimensoes invalidas.");   
        return;
    }

    // Se ela nao possuir elementos
    if(matriz_existente->elementos == NULL) {
        imprime_erro("Nao existem elementos, mas matriz sera liberada.");
        free(matriz_existente);
        return;
    }

    for(int i = 0; i < matriz_existente->altura; i++) {
        // Se algum array nao existir
        if(matriz_existente->elementos[i] == NULL) {
            imprime_erro("Matriz nao alocada corretamente. Pulando elemento.");
            continue;
        }
        // Libero o array
        free(matriz_existente->elementos[i]);
    }

    // Libero a matriz
    free(matriz_existente);

}

/**
 * @brief Funcao para trocar duas matrizes, e liberar a nao utilizada.
 * @param m1 substituindo
 * @param m2 substituinte
 * @return m2 
 */
matriz_t_bdop * substituir(matriz_t_bdop * m1, matriz_t_bdop * m2) {
    // Caso alguma matriz seja nula
    if(m1 == NULL || m2 == NULL) {
        return m1;
    }

    // Libero a matriz nao mais utilizada
    liberar_matriz(m1);
    return m2;
}

/**
 * @brief Funcao que junta uma matriz a direita da outra.
 * @param m1 Matriz que estara a esquerda.
 * @param m2 Matriz com os valores que serao adicionados a direita.
 * @return NULL caso fracasso, nao NULL caso sucesso.
 */
matriz_t_bdop * juntar_a_direita(const matriz_t_bdop * m1, const matriz_t_bdop * m2) {
    if(m1->altura != m2->altura) {
        imprime_erro("Alturas diferentes entre as matrizes.");
        return NULL;
    }

    // Crio uma nova matriz com as dimensoes adequadas.
    // m1->altura == m2->altura.
    matriz_t_bdop * nova_matriz = criar_matriz(m1->altura, m1->largura + m2->largura);

    // Se erro em criar a nova matriz
    if(nova_matriz == NULL) {
        return NULL;
    }

    // Preencho a nova matriz com elementos de m1 e m2.
    int l;
    for(int i = 0; i < nova_matriz->altura; i++) {
        for(int j = 0; j < m1->largura; j++) {
            nova_matriz->elementos[i][j] = m1->elementos[i][j];
        }
        l = 0;
        for(int k = m1->largura; k < m1->largura + m2->largura; k++) {
            nova_matriz->elementos[i][k] = m2->elementos[i][l];
            l++;
        }
    }

    return nova_matriz;
}

/**
 * @brief Funcao que eleva os elementos de uma matriz a um expoente qualquer.
 * @param m Matriz
 * @param exp Expoente
 */
matriz_t_bdop * elevar_elementos_matriz(matriz_t_bdop * m, uint exp) {
    
    // Caso a matriz seja nula
    if(m == NULL) {
        imprime_erro("Tentando elevar elementos de uma matriz nula.");
        return NULL;
    }

    // Crio uma matriz com as dimensoes iguais
    matriz_t_bdop * matriz_nova = criar_matriz(m->altura, m->largura);

    // Caso o malloc tenha falhado
    if(matriz_nova == NULL) {
        imprime_erro("Falha no malloc.");
        return NULL;
    }

    // Elevo cada elemento
    for(int i = 0; i < m->altura; i++) {
        for(int j = 0; j < m->largura; j++) {
            matriz_nova->elementos[i][j] = pow(m->elementos[i][j], exp);
        }
    }

    // Retorno a nova matriz
    return matriz_nova;

}

/**
 * @brief Funcao que ajusta matriz com valores de X para o grau desejado
 * @param mx Matriz com os valores de X
 * @param gr Grau da funcao desejado
 */
matriz_t_bdop * ajustar_matriz_x(matriz_t_bdop * mx, const uint gr) {

    matriz_t_bdop * mg = elevar_elementos_matriz(mx, gr);
    matriz_t_bdop * mg2 = NULL;

    for(int i = gr - 1; i >= 0; i--) {
        mg2 = elevar_elementos_matriz(mx, i);
        mg = substituir(mg, juntar_a_direita(mg, mg2));
    }

    liberar_matriz(mg2);

    return mg;
}

/**
 * @brief Funcao que transpoe matriz qualquer
 * @param mt Matriz a ser transposta
 * @return NULL caso fracasso, nao NULL caso sucesso (matriz transposta)
 */
matriz_t_bdop * transpor_matriz(const matriz_t_bdop * mt) {
    if(mt == NULL) {
        imprime_erro("Tentando transpor matriz nula.");
        return NULL;
    }
    matriz_t_bdop * matriz_transposta = criar_matriz(mt->largura, mt->altura);

    if(matriz_transposta == NULL) {
        imprime_erro("Falha no malloc.");
        return NULL;
    }

    for(int i = 0; i < mt->altura; i++) {
        for(int j = 0; j < mt->largura; j++) {
            matriz_transposta->elementos[j][i] = mt->elementos[i][j];
        }
    }

    return matriz_transposta;

}

/**
 * @brief Funcao que multiplica duas matrizes
 * @param m1 Matriz 1 (a esquerda)
 * @param m2 Matriz 2 (a direita) 
 */
matriz_t_bdop * multiplica_matriz(const matriz_t_bdop * m1, const matriz_t_bdop * m2) {

    // Verificação
    if (m1 == NULL || m2 == NULL) {
        imprime_erro("Operacao com matrizes nulas.");
        return NULL;
    }

    // Restrição
    if (m1->largura != m2->altura) {
        imprime_erro("Impossivel multiplicar: largura de m1 diferente da altura de m2.");
        return NULL;
    }

    matriz_t_bdop * matriz_multiplicada = criar_matriz(m1->altura, m2->largura);

    if (matriz_multiplicada == NULL) {
        imprime_erro("Falha no malloc.");
        return NULL;
    }

    for (int i = 0; i < m1->altura; i++) {
        for (int j = 0; j < m2->largura; j++) {
            double res = 0.0;

            for (int k = 0; k < m1->largura; k++) {
                res += m1->elementos[i][k] * m2->elementos[k][j];
            }

            matriz_multiplicada->elementos[i][j] = res;
        }
    }

    return matriz_multiplicada;
}

/**
 * @brief Funcao que inverte uma matriz
 * @param m Matriz a ser invertida
 * @return NULL caso fracasso, nao NULL caso a matriz seja invertida
 */
matriz_t_bdop * inverte_matriz(const matriz_t_bdop * m) {

    // Verificações
    if (m == NULL) {
        imprime_erro("Matriz nula.");
        return NULL;
    }

    if (m->altura != m->largura) {
        imprime_erro("Matriz nao quadrada.");
        return NULL;
    }

    int n = m->altura;

    // Matriz aumentada [A | I]
    matriz_t_bdop * aug = criar_matriz(n, 2 * n);
    if (aug == NULL) {
        imprime_erro("Falha no malloc.");
        return NULL;
    }

    // Copia A e cria identidade
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            aug->elementos[i][j] = m->elementos[i][j];
        }

        for (int j = n; j < 2*n; j++) {
            aug->elementos[i][j] = (i == j - n) ? 1.0 : 0.0;
        }
    }

    // Gauss-Jordan
    for (int i = 0; i < n; i++) {

        // Pivotamento simples
        if (fabs(aug->elementos[i][i]) < EPS) {
            imprime_erro("Matriz singular (pivo zero).");
            liberar_matriz(aug);
            return NULL;
        }

        double pivo = aug->elementos[i][i];

        // Normaliza linha
        for (int j = 0; j < 2*n; j++) {
            aug->elementos[i][j] /= pivo;
        }

        // Zera coluna
        for (int k = 0; k < n; k++) {
            if (k != i) {
                double fator = aug->elementos[k][i];
                for (int j = 0; j < 2*n; j++) {
                    aug->elementos[k][j] -= fator * aug->elementos[i][j];
                }
            }
        }
    }

    // Extrai inversa
    matriz_t_bdop * inv = criar_matriz(n, n);
    if (inv == NULL) {
        imprime_erro("Falha no malloc.");
        liberar_matriz(aug);
        return NULL;
    }

    for (int i = 0; i < n; i++) { 
        for (int j = 0; j < n; j++) {
            inv->elementos[i][j] = aug->elementos[i][j + n];
        }
    }

    liberar_matriz(aug);
    return inv;
}

/**
 * @brief FINALMENTE, O ajuste (Nao verifica se ocorrer alguma falha no malloc).
 * @param x matriz com os valores de x
 * @param y matriz com os valores de y
 * @param grau o grau do polinomio
 * @return Matriz com os coeficientes caso sucesso, NULL caso fracasso.
 * @warning Essa funcao nao verifica falhas no malloc.
 */
matriz_t_bdop * ajuste(matriz_t_bdop * x, matriz_t_bdop * y, uint grau) {
    x = substituir(x, ajustar_matriz_x(x, grau));

    matriz_t_bdop * transposta_x = transpor_matriz(x);

    matriz_t_bdop * multiplicacao_transposta_normal = multiplica_matriz(transposta_x, x);

    matriz_t_bdop * inversa_transposta_normal = inverte_matriz(multiplicacao_transposta_normal);

    matriz_t_bdop * passo_final_x = multiplica_matriz(inversa_transposta_normal, transposta_x);

    free(transposta_x);
    free(multiplicacao_transposta_normal);
    free(inversa_transposta_normal);

    return substituir(passo_final_x, multiplica_matriz(passo_final_x, y));

}
