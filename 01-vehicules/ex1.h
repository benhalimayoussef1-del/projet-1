#ifndef EX1_H
#define EX1_H

struct moteur
{
    int puissance;
    char carburant[20];
    int cylindree;
};
typedef struct moteur MOTEUR;

struct vehicule
{
    int immatriculation;
    char marque[30];
    MOTEUR mot;
    int nbRevisions;
    float *couts;
    float coutTotal;
};
typedef struct vehicule VEHICULE;

MOTEUR saisirMoteur(void);
void afficherMoteur(MOTEUR m);
void saisirMoteur2(MOTEUR *m);
VEHICULE saisirVehicule(void);
void saisirVehicule2(VEHICULE *v);
void afficherVehicule(VEHICULE v);
void remplirTabVehicules(VEHICULE *t, int n);
void afficherTabVehicules(VEHICULE *t, int n);
float calculCoutTotal(float *couts, int nbRevisions);
void calculCoutTotal2(float *couts, int nbRevisions, float *total);
void afficherVehiculePlusCouteux(VEHICULE *t, int n);
VEHICULE *allocationVehicule(int n);
void allocationVehicule2(VEHICULE **t, int n);
void remplirTabVehiculesDyn(VEHICULE *t, int n);
void afficherTabVehiculesDyn(VEHICULE *t, int n);
void trierParCoutTotal(VEHICULE *t, int n);
void libererVehicules(VEHICULE *t, int n);
VEHICULE **allocationTabAdresses(int n);
void remplirTabAdresses(VEHICULE **t, int n);
void afficherTabAdresses(VEHICULE **t, int n);
void afficherMarquesEtCouts(VEHICULE **t, int n);
void libererTabAdresses(VEHICULE **t, int n);

#endif
