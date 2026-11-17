#include <stdio.h>
#include <stdlib.h>

typedef struct { int cf; float montantHT, montantTVA, montantTTC; } FACTURE;
typedef struct { int cc; int nb_factures; FACTURE *factures; float moyenne; } CLIENT;

static void verifier(int ok)
{
    if (!ok) {
        fprintf(stderr, "Saisie invalide.\n");
        exit(EXIT_FAILURE);
    }
}

static void saisirClient(CLIENT *c)
{
    float somme = 0;
    printf("Code client : ");
    verifier(scanf("%d", &c->cc) == 1);
    printf("Nombre de factures : ");
    verifier(scanf("%d", &c->nb_factures) == 1 && c->nb_factures >= 0);

    c->factures = NULL;
    if (c->nb_factures > 0) {
        c->factures = malloc(c->nb_factures * sizeof(FACTURE));
        if (!c->factures) {
            perror("malloc");
            exit(EXIT_FAILURE);
        }
    }
    for (int j = 0; j < c->nb_factures; j++) {
        FACTURE *f = &c->factures[j];
        printf("Facture %d - code : ", j + 1);
        verifier(scanf("%d", &f->cf) == 1);
        printf("Facture %d - montant HT : ", j + 1);
        verifier(scanf("%f", &f->montantHT) == 1);
        printf("Facture %d - TVA : ", j + 1);
        verifier(scanf("%f", &f->montantTVA) == 1);
        f->montantTTC = f->montantHT + f->montantTVA;
        somme += f->montantTTC;
    }
    c->moyenne = c->nb_factures > 0 ? somme / c->nb_factures : 0;
}

static void afficherClient(const CLIENT *c, int i)
{
    printf("\nClient %d (code %d) : %d facture(s)\n", i + 1, c->cc, c->nb_factures);
    for (int j = 0; j < c->nb_factures; j++) {
        const FACTURE *f = &c->factures[j];
        printf("  Facture %d : HT %.2f, TVA %.2f, TTC %.2f\n",
               f->cf, f->montantHT, f->montantTVA, f->montantTTC);
    }
    printf("  Moyenne TTC : %.2f\n", c->moyenne);
}

int main(void)
{
    int n, i_moy = -1, i_max = -1, j_max = -1;
    float max_moy = 0, max_ttc = 0;
    CLIENT *tab;

    printf("Nombre de clients : ");
    verifier(scanf("%d", &n) == 1 && n > 0);
    tab = malloc(n * sizeof(CLIENT));
    if (!tab) {
        perror("malloc");
        return EXIT_FAILURE;
    }

    for (int i = 0; i < n; i++) {
        printf("\n--- Client %d ---\n", i + 1);
        saisirClient(&tab[i]);
    }
    for (int i = 0; i < n; i++)
        afficherClient(&tab[i], i);

    printf("\n=== Statistiques ===\n");
    for (int i = 0; i < n; i++) {
        const CLIENT *c = &tab[i];
        int j_cmax = -1, nb_petites = 0;
        float c_max = 0;

        for (int j = 0; j < c->nb_factures; j++) {
            float ttc = c->factures[j].montantTTC;
            if (j_cmax < 0 || ttc > c_max) {
                c_max = ttc;
                j_cmax = j;
            }
            if (ttc < 100)
                nb_petites++;
        }
        if (j_cmax >= 0)
            printf("Client %d : TTC max %.2f (facture %d), %d facture(s) sous 100\n",
                   i + 1, c_max, j_cmax + 1, nb_petites);
        else
            printf("Client %d : aucune facture\n", i + 1);

        if (i_moy < 0 || c->moyenne > max_moy) {
            max_moy = c->moyenne;
            i_moy = i;
        }
        if (j_cmax >= 0 && (i_max < 0 || c_max > max_ttc)) {
            max_ttc = c_max;
            i_max = i;
            j_max = j_cmax;
        }
    }
    printf("\nMeilleure moyenne TTC : %.2f (client %d)\n", max_moy, i_moy + 1);
    if (i_max >= 0)
        printf("Plus grand TTC : %.2f (client %d, facture %d)\n", max_ttc, i_max + 1, j_max + 1);

    for (int i = 0; i < n; i++)
        free(tab[i].factures);
    free(tab);
    return 0;
}
