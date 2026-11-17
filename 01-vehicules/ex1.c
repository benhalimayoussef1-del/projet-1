#include <stdio.h>
#include <stdlib.h>
#include "ex1.h"

/* ---------- Saisie securisee ---------- */
static void viderBuffer(void)
{
    int c;
    while ((c = getchar()) != '\n' && c != EOF)
        ;
}

static int lireEntier(const char *msg)
{
    int x, r;
    printf("%s", msg);
    while ((r = scanf("%d", &x)) != 1) {
        if (r == EOF)
            exit(EXIT_FAILURE);
        viderBuffer();
        printf("Valeur invalide, recommencez : ");
    }
    return x;
}

static float lireReel(const char *msg)
{
    float x;
    int r;
    printf("%s", msg);
    while ((r = scanf("%f", &x)) != 1) {
        if (r == EOF)
            exit(EXIT_FAILURE);
        viderBuffer();
        printf("Valeur invalide, recommencez : ");
    }
    return x;
}

/* Lit un mot sans depasser la taille du tableau destination */
static void lireMot(const char *msg, char *dest, int taille)
{
    char fmt[16];
    snprintf(fmt, sizeof fmt, "%%%ds", taille - 1);
    printf("%s", msg);
    if (scanf(fmt, dest) != 1)
        exit(EXIT_FAILURE);
}

static int cmpCout(const void *a, const void *b)
{
    float x = ((const VEHICULE *)a)->coutTotal;
    float y = ((const VEHICULE *)b)->coutTotal;
    return (x > y) - (x < y);
}

static int cmpCoutPtr(const void *a, const void *b)
{
    return cmpCout(*(VEHICULE *const *)a, *(VEHICULE *const *)b);
}

/* ---------- Moteur ---------- */
void saisirMoteur2(MOTEUR *m)
{
    m->puissance = lireEntier("Puissance du moteur : ");
    lireMot("Type de carburant : ", m->carburant, sizeof m->carburant);
    m->cylindree = lireEntier("Nombre de cylindres : ");
}

MOTEUR saisirMoteur(void)
{
    MOTEUR m;
    saisirMoteur2(&m);
    return m;
}

void afficherMoteur(MOTEUR m)
{
    printf("Puissance : %d\nCarburant : %s\nCylindres : %d\n",
           m.puissance, m.carburant, m.cylindree);
}

/* ---------- Couts ---------- */
float calculCoutTotal(float *couts, int nbRevisions)
{
    float s = 0;
    for (int i = 0; i < nbRevisions; i++)
        s += couts[i];
    return s;
}

void calculCoutTotal2(float *couts, int nbRevisions, float *total)
{
    *total = calculCoutTotal(couts, nbRevisions);
}

/* ---------- Vehicule ---------- */
void saisirVehicule2(VEHICULE *v)
{
    v->immatriculation = lireEntier("Immatriculation : ");
    lireMot("Marque : ", v->marque, sizeof v->marque);
    printf("Caracteristiques du moteur :\n");
    saisirMoteur2(&v->mot);
    do {
        v->nbRevisions = lireEntier("Nombre de revisions : ");
    } while (v->nbRevisions < 0);

    v->couts = NULL;
    if (v->nbRevisions > 0) {
        v->couts = malloc(v->nbRevisions * sizeof(float));
        if (!v->couts)
            exit(EXIT_FAILURE);
    }
    for (int i = 0; i < v->nbRevisions; i++) {
        printf("Cout de la revision %d", i + 1);
        v->couts[i] = lireReel(" : ");
    }
    calculCoutTotal2(v->couts, v->nbRevisions, &v->coutTotal);
}

VEHICULE saisirVehicule(void)
{
    VEHICULE v;
    saisirVehicule2(&v);
    return v;
}

void afficherVehicule(VEHICULE v)
{
    printf("Immatriculation : %d\nMarque : %s\n", v.immatriculation, v.marque);
    afficherMoteur(v.mot);
    printf("Nombre de revisions : %d\n", v.nbRevisions);
    for (int i = 0; i < v.nbRevisions; i++)
        printf("  Cout de la revision %d : %.2f\n", i + 1, v.couts[i]);
    printf("Cout total des revisions : %.2f\n\n", v.coutTotal);
}

/* ---------- Tableau statique / dynamique de vehicules ---------- */
void remplirTabVehicules(VEHICULE *t, int n)
{
    for (int i = 0; i < n; i++) {
        printf("--- Vehicule %d ---\n", i + 1);
        saisirVehicule2(&t[i]);
    }
}

void afficherTabVehicules(VEHICULE *t, int n)
{
    for (int i = 0; i < n; i++) {
        printf("--- Vehicule %d ---\n", i + 1);
        afficherVehicule(t[i]);
    }
}

void afficherVehiculePlusCouteux(VEHICULE *t, int n)
{
    int imax = 0;
    if (n <= 0)
        return;
    for (int i = 1; i < n; i++)
        if (t[i].coutTotal > t[imax].coutTotal)
            imax = i;
    printf("Vehicule le plus couteux : %s (cout total %.2f)\n",
           t[imax].marque, t[imax].coutTotal);
}

VEHICULE *allocationVehicule(int n)
{
    /* calloc : couts vaut NULL partout, donc libererVehicules reste sur meme en cas d'abandon */
    VEHICULE *t = calloc(n, sizeof(VEHICULE));
    if (!t)
        exit(EXIT_FAILURE);
    return t;
}

void allocationVehicule2(VEHICULE **t, int n)
{
    *t = allocationVehicule(n);
}

void remplirTabVehiculesDyn(VEHICULE *t, int n)
{
    remplirTabVehicules(t, n);
}

void afficherTabVehiculesDyn(VEHICULE *t, int n)
{
    afficherTabVehicules(t, n);
}

void trierParCoutTotal(VEHICULE *t, int n)
{
    qsort(t, n, sizeof(VEHICULE), cmpCout);
}

void libererVehicules(VEHICULE *t, int n)
{
    if (!t)
        return;
    for (int i = 0; i < n; i++) {
        free(t[i].couts);
        t[i].couts = NULL;
    }
    free(t);
}

/* ---------- Tableau d'adresses ---------- */
VEHICULE **allocationTabAdresses(int n)
{
    VEHICULE **t;
    if (n <= 0)
        return NULL;
    t = malloc(n * sizeof(VEHICULE *));
    if (!t)
        exit(EXIT_FAILURE);
    for (int i = 0; i < n; i++) {
        t[i] = calloc(1, sizeof(VEHICULE));
        if (!t[i])
            exit(EXIT_FAILURE);
    }
    return t;
}

void remplirTabAdresses(VEHICULE **t, int n)
{
    for (int i = 0; i < n; i++) {
        printf("--- Vehicule %d ---\n", i + 1);
        saisirVehicule2(t[i]);
    }
}

void afficherTabAdresses(VEHICULE **t, int n)
{
    for (int i = 0; i < n; i++)
        afficherVehicule(*t[i]);
}

void afficherMarquesEtCouts(VEHICULE **t, int n)
{
    /* On trie les pointeurs : plus rapide que de deplacer les structures */
    qsort(t, n, sizeof(VEHICULE *), cmpCoutPtr);
    for (int i = 0; i < n; i++)
        printf("%s : cout total %.2f\n", t[i]->marque, t[i]->coutTotal);
}

void libererTabAdresses(VEHICULE **t, int n)
{
    if (!t)
        return;
    for (int i = 0; i < n; i++) {
        if (t[i]) {
            free(t[i]->couts);
            free(t[i]);
        }
    }
    free(t);
}
