#include <stdio.h>
#include "ex1.h"

int main(void)
{
    int n;
    VEHICULE *t;
    VEHICULE **ta;

    printf("Nombre de vehicules : ");
    if (scanf("%d", &n) != 1 || n <= 0)
        return 1;

    /* 1) Tableau dynamique de structures */
    t = allocationVehicule(n);
    remplirTabVehiculesDyn(t, n);
    trierParCoutTotal(t, n);
    afficherTabVehiculesDyn(t, n);
    afficherVehiculePlusCouteux(t, n);
    libererVehicules(t, n);

    /* 2) Tableau d'adresses */
    printf("\nMeme chose avec un tableau d'adresses :\n");
    ta = allocationTabAdresses(n);
    remplirTabAdresses(ta, n);
    afficherMarquesEtCouts(ta, n);
    libererTabAdresses(ta, n);
    return 0;
}
