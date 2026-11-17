# Corrections apportées

## 01-vehicules (ex1.c, ex1.h)
- `#include <ex1.h>` devient `#include "ex1.h"` (fichier local).
- `saisirMoteur` et `saisirVehicule` ne retournaient rien : elles retournent maintenant la structure.
- `couts` est un `float *` mais était alloué avec `sizeof(int)` : corrigé en `sizeof(float)`.
- `s=+ v->couts[i]` (affecte) devient une vraie somme ; `s` n'était pas initialisée.
- `coutTotal` est maintenant la **somme** des coûts (le code divisait par le nombre de révisions, ce qui donnait une moyenne).
- `calculCoutTotal` réallouait `couts` et écrasait les données : elle fait seulement la somme.
- `remplirTabVehicules` redemandait `n` dans une copie locale : le paramètre est utilisé tel quel.
- `afficherVehiculePlusCouteux` : `%c` remplacé par `%s`, `imax` n'est plus non initialisé.
- `allocationVehicule` et `allocationTabAdresses` ne retournaient pas le pointeur.
- `afficherMarquesEtCouts` triait `*t` (un seul élément) au lieu du tableau de pointeurs.
- `scanf("%s")` est limité à la taille des tableaux (risque de dépassement).
- Saisies protégées contre les valeurs invalides ; ajout d'un `main.c` de démonstration.
- Tris faits avec `qsort`. Le tableau d'adresses est trié en déplaçant des pointeurs.
- **Retiré du `.h`** : les fonctions de matrices et de cinéma (`allocation_mat`, `remplir_cinema`, etc.) étaient déclarées mais jamais écrites, donc le programme ne pouvait pas être lié. À me renvoyer si tu veux que je les écrive.

## 02-etudiants (ex1_1.c)
- Tris remplacés par `qsort` avec comparateurs.
- Adresses affichées avec `%p` au lieu de `%d`.
- Tests de `malloc` et de `scanf`, numérotation des notes à partir de 1.

## 03-clients-factures (ex.c)
- Code découpé en fonctions (`saisirClient`, `afficherClient`).
- Division par zéro évitée quand un client n'a aucune facture.
- Mémoire des factures libérée (elle ne l'était pas).
- Le client à meilleure moyenne est trouvé même si toutes les moyennes sont nulles.

## 04 et 05 (ex7.c, ex8.c)
Gardés tels quels : ils compilent sans avertissement et la logique est correcte.
