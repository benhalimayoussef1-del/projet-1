# Exercices de programmation C

Série d'exercices de C (structures, allocation dynamique, tris, matrices), corrigés et commentés, avec une version web pour les trois plus gros projets.

| Dossier | Contenu |
|---|---|
| `01-vehicules` | Structures imbriquées, tableaux dynamiques, tableau d'adresses, tri |
| `02-etudiants` | Étudiants et notes, tri par moyenne et par âge (`qsort`) |
| `03-clients-factures` | Clients et factures, statistiques (max, moyennes) |
| `04-suite-successive` | Plus longue suite de nombres successifs |
| `05-transposee-matrice` | Matrice dynamique et transposée |
| `06-reconciliation-sqlite` | Réconciliation banque / comptabilité avec SQLite et Python (CSV, rapport, tests) |
| `07-minuteur-pwa` | Minuteur installable, fonctionne hors ligne (Progressive Web App) |
| `08-loup-garou` | Distribution des rôles du Loup-Garou sur un seul téléphone |
| `web` | Version navigateur des projets 01 à 03 (HTML, CSS, JavaScript) |

## Compiler et lancer un programme C

```bash
cd 01-vehicules
make
./vehicules
```

Compilateur requis : `gcc`. Chaque dossier a son `Makefile` (`make clean` supprime l'exécutable).

## Lancer la version web

Ouvrez `web/index.html` dans un navigateur. Pour la mettre en ligne, activez GitHub Pages sur le dossier `web`.

Les logiques de calcul du C sont réécrites en JavaScript : le navigateur n'exécute pas le C directement.

Les principales corrections sont listées dans [CORRECTIONS.md](CORRECTIONS.md).

## Projets 06 à 08

```bash
cd 06-reconciliation-sqlite
python reconcile.py            # utilise les fichiers du dossier data/
python -m unittest             # lance le test
```

Pour `07-minuteur-pwa` et `08-loup-garou`, ouvrez `index.html`. Le mode « installable » et le hors-ligne du minuteur demandent un serveur HTTP (par exemple `python -m http.server`, ou GitHub Pages).
