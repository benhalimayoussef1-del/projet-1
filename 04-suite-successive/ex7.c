#include <stdio.h>
#include <stdlib.h>

int main(void)
{
    int n, i;
    int *t;
    int cour, max;

    /* Contrôle de saisie : n entre 10 et 20 */
    do {
        printf("Donner n (entre 10 et 20) : ");
        scanf("%d", &n);
        if (n < 10 || n > 20)
            printf("Erreur : n doit etre compris entre 10 et 20.\n");
    } while (n < 10 || n > 20);

    /* Allocation dynamique */
    t = (int *)malloc(n * sizeof(int));
    if (t == NULL) {
        printf("Erreur d'allocation memoire.\n");
        return 1;
    }

    /* Remplissage */
    for (i = 0; i < n; i++) {
        printf("t[%d] = ", i);
        scanf("%d", &t[i]);
    }

    /* Affichage */
    printf("\nContenu du tableau : ");
    for (i = 0; i < n; i++)
        printf("%d ", t[i]);
    printf("\n");

    /* Plus longue suite de nombres successifs */
    cour = 1;
    max = 1;
    for (i = 1; i < n; i++) {
        if (t[i] == t[i - 1] + 1)
            cour++;
        else
            cour = 1;

        if (cour > max)
            max = cour;
    }

    printf("Taille de la plus longue suite de nombres successifs : %d\n", max);

    /* Libération de la mémoire */
    free(t);
    return 0;

    
}