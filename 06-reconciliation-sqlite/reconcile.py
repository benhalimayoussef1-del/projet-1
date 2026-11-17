"""Reconciliation bancaire : compare un releve de banque et la comptabilite avec SQLite."""
import argparse
import csv
import sqlite3
from collections import Counter
from pathlib import Path

DEFAUT = Path(__file__).parent / "data"

SCHEMA = """
CREATE TABLE banque (ref TEXT PRIMARY KEY, date TEXT, libelle TEXT, montant REAL NOT NULL);
CREATE TABLE compta (ref TEXT PRIMARY KEY, date TEXT, libelle TEXT, montant REAL NOT NULL);
"""

# Deux LEFT JOIN plutot qu'un FULL OUTER JOIN : compatible avec toutes les versions de SQLite
REQUETE = """
SELECT b.ref AS ref, b.montant AS m_banque, c.montant AS m_compta,
       CASE WHEN c.ref IS NULL THEN 'MANQUANT_COMPTA'
            WHEN ROUND(b.montant, 2) <> ROUND(c.montant, 2) THEN 'ECART'
            ELSE 'OK' END AS statut
FROM banque b LEFT JOIN compta c ON c.ref = b.ref
UNION ALL
SELECT c.ref, NULL, c.montant, 'MANQUANT_BANQUE'
FROM compta c LEFT JOIN banque b ON b.ref = c.ref
WHERE b.ref IS NULL
ORDER BY ref
"""


def creer_base(chemin=":memory:"):
    con = sqlite3.connect(chemin)
    con.executescript("DROP TABLE IF EXISTS banque; DROP TABLE IF EXISTS compta;" + SCHEMA)
    return con


def charger_csv(con, table, chemin):
    """Charge un CSV (ref, date, libelle, montant). `table` vient du code, jamais de l'utilisateur."""
    with open(chemin, newline="", encoding="utf-8") as f:
        lignes = [(r["ref"].strip(), r["date"], r["libelle"], float(r["montant"]))
                  for r in csv.DictReader(f)]
    con.executemany(f"INSERT INTO {table} VALUES (?, ?, ?, ?)", lignes)


def reconcilier(con):
    return con.execute(REQUETE).fetchall()


def ecrire_rapport(lignes, chemin):
    with open(chemin, "w", newline="", encoding="utf-8") as f:
        w = csv.writer(f)
        w.writerow(["ref", "montant_banque", "montant_compta", "statut", "ecart"])
        for ref, mb, mc, statut in lignes:
            ecart = round(mb - mc, 2) if mb is not None and mc is not None else ""
            w.writerow([ref, mb, mc, statut, ecart])


def afficher(lignes):
    compte = Counter(l[3] for l in lignes)
    print(f"{len(lignes)} operations : " + ", ".join(f"{k} = {v}" for k, v in sorted(compte.items())))
    for ref, mb, mc, statut in lignes:
        if statut != "OK":
            print(f"  {statut:16} {ref:6} banque={mb} compta={mc}")


def main():
    p = argparse.ArgumentParser(description=__doc__)
    p.add_argument("banque", nargs="?", default=DEFAUT / "banque.csv")
    p.add_argument("compta", nargs="?", default=DEFAUT / "compta.csv")
    p.add_argument("-o", "--rapport", default="rapport.csv", help="fichier CSV du rapport")
    p.add_argument("--db", default=":memory:", help="fichier SQLite a conserver (sinon base en memoire)")
    a = p.parse_args()

    con = creer_base(a.db)
    try:
        charger_csv(con, "banque", a.banque)
        charger_csv(con, "compta", a.compta)
    except sqlite3.IntegrityError as e:
        raise SystemExit(f"Reference en double dans un fichier : {e}")
    except (OSError, KeyError, ValueError) as e:
        raise SystemExit(f"Fichier illisible ou colonne manquante : {e}")

    lignes = reconcilier(con)
    afficher(lignes)
    ecrire_rapport(lignes, a.rapport)
    print(f"Rapport ecrit dans {a.rapport}")


if __name__ == "__main__":
    main()
