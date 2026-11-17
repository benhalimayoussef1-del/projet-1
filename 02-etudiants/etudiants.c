#include <stdio.h>
#include <stdlib.h>

typedef struct { int jour, mois, annee; } DATE;
typedef struct { int ce; DATE dateNaiss; int nbNotes; float *notes; } INFO;
typedef struct { INFO inf; float moyenne; } ETUDIANT;

static void verifier(int ok)
{
    if (!ok) {
        fprintf(stderr, "Saisie invalide.\n");
        exit(EXIT_FAILURE);
    }
}

void saisirEtudiant(ETUDIANT *e)
{
    float somme = 0;
    printf("Code etudiant : ");
    verifier(scanf("%d", &e->inf.ce) == 1);
    printf("Date de naissance (jj mm aaaa) : ");
    verifier(scanf("%d %d %d", &e->inf.dateNaiss.jour,
                   &e->inf.dateNaiss.mois, &e->inf.dateNaiss.annee) == 3);
    printf("Nombre de notes : ");
    verifier(scanf("%d", &e->inf.nbNotes) == 1 && e->inf.nbNotes >= 0);

    e->inf.notes = NULL;
    if (e->inf.nbNotes > 0) {
        e->inf.notes = malloc(e->inf.nbNotes * sizeof(float));
        if (!e->inf.notes) {
            perror("malloc");
            exit(EXIT_FAILURE);
        }
    }
    for (int i = 0; i < e->inf.nbNotes; i++) {
        printf("Note %d : ", i + 1);
        verifier(scanf("%f", &e->inf.notes[i]) == 1);
        somme += e->inf.notes[i];
    }
    e->moyenne = e->inf.nbNotes > 0 ? somme / e->inf.nbNotes : 0;
}

void afficherEtudiant(ETUDIANT e)
{
    printf("%d  %02d/%02d/%04d  moyenne %.2f\n", e.inf.ce, e.inf.dateNaiss.jour,
           e.inf.dateNaiss.mois, e.inf.dateNaiss.annee, e.moyenne);
}

/* Moyenne decroissante */
static int cmpMoyenne(const void *x, const void *y)
{
    float a = ((const ETUDIANT *)x)->moyenne;
    float b = ((const ETUDIANT *)y)->moyenne;
    return (a < b) - (a > b);
}

/* Du plus age au plus jeune */
static int cmpAge(const void *x, const void *y)
{
    const DATE *a = &((const ETUDIANT *)x)->inf.dateNaiss;
    const DATE *b = &((const ETUDIANT *)y)->inf.dateNaiss;
    if (a->annee != b->annee) return a->annee - b->annee;
    if (a->mois != b->mois) return a->mois - b->mois;
    return a->jour - b->jour;
}

void trierParMoyenne(ETUDIANT *tab, int n) { qsort(tab, n, sizeof(ETUDIANT), cmpMoyenne); }
void trierParAge(ETUDIANT *tab, int n) { qsort(tab, n, sizeof(ETUDIANT), cmpAge); }

static void afficherTous(ETUDIANT *tab, int n)
{
    for (int i = 0; i < n; i++) {
        printf("Etudiant %d : ", i + 1);
        afficherEtudiant(tab[i]);
    }
}

int main(void)
{
    int n;
    ETUDIANT *tab, **tabAdr;

    printf("Nombre d'etudiants : ");
    verifier(scanf("%d", &n) == 1 && n > 0);

    tab = malloc(n * sizeof(ETUDIANT));
    tabAdr = malloc(n * sizeof(ETUDIANT *));
    if (!tab || !tabAdr) {
        perror("malloc");
        return EXIT_FAILURE;
    }

    for (int i = 0; i < n; i++) {
        printf("\n--- Etudiant %d ---\n", i + 1);
        saisirEtudiant(&tab[i]);
    }

    printf("\nListe saisie :\n");
    afficherTous(tab, n);

    trierParMoyenne(tab, n);
    printf("\nTri par moyenne :\n");
    afficherTous(tab, n);

    trierParAge(tab, n);
    printf("\nTri par age :\n");
    afficherTous(tab, n);

    /* Un pointeur ne s'affiche pas avec %d mais avec %p */
    printf("\nAdresses :\n");
    for (int i = 0; i < n; i++) {
        tabAdr[i] = &tab[i];
        printf("Etudiant %d : %p\n", i + 1, (void *)tabAdr[i]);
    }

    for (int i = 0; i < n; i++)
        free(tab[i].inf.notes);
    free(tab);
    free(tabAdr);
    return 0;
}
