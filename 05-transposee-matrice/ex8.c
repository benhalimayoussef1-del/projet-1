#include <stdio.h>
#include <stdlib.h>

/* 1. Allocation d'une matrice dynamique de l lignes et c colonnes */
int **allocation(int l, int c)
{
    int i, j;
    int **M = (int **)malloc(l * sizeof(int *));
    if (M == NULL)
        return NULL;

    for (i = 0; i < l; i++)
    {
        M[i] = (int *)malloc(c * sizeof(int));
        if (M[i] == NULL)
        {
            /* en cas d'échec : libérer ce qui a déjà été alloué */
            for (j = 0; j < i; j++)
                free(M[j]);
            free(M);
            return NULL;
        }
    }
    return M;
}

/* 2.a Remplissage */
void remplir_mat(int **M, int l, int c)
{
    int i, j;
    for (i = 0; i < l; i++)
        for (j = 0; j < c; j++)
        {
            printf("M[%d][%d] = ", i, j);
            scanf("%d", &M[i][j]);
        }
}

/* 2.b Affichage */
void afficher_mat(int **M, int l, int c)
{
    int i, j;
    for (i = 0; i < l; i++)
    {
        for (j = 0; j < c; j++)
            printf("%4d", M[i][j]);
        printf("\n");
    }
}

/* 3. Transposée : crée une nouvelle matrice c x l, l'affiche et la retourne */
int **transposee(int **M, int l, int c)
{
    int i, j;
    int **T = allocation(c, l);   /* la transposée a c lignes et l colonnes */
    if (T == NULL)
        return NULL;

    for (i = 0; i < c; i++)
        for (j = 0; j < l; j++)
            T[i][j] = M[j][i];

    printf("Transposee :\n");
    afficher_mat(T, c, l);
    return T;
}

/* Libération de la mémoire */
void liberer(int **M, int l)
{
    int i;
    for (i = 0; i < l; i++)
        free(M[i]);
    free(M);
}

int main()
{
    int l, c;
    int **A, **T;

    printf("Nombre de lignes : ");
    scanf("%d", &l);
    printf("Nombre de colonnes : ");
    scanf("%d", &c);

    A = allocation(l, c);
    if (A == NULL)
    {
        printf("Erreur d'allocation\n");
        return 1;
    }

    remplir_mat(A, l, c);
    printf("Matrice A :\n");
    afficher_mat(A, l, c);

    T = transposee(A, l, c);

    liberer(A, l);
    if (T != NULL)
        liberer(T, c);   /* T a c lignes */

    return 0;
}