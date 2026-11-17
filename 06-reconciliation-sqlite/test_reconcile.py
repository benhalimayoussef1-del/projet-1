import unittest
from reconcile import creer_base, reconcilier


class TestReconciliation(unittest.TestCase):
    def test_statuts(self):
        con = creer_base()
        con.executemany("INSERT INTO banque VALUES (?,?,?,?)",
                        [("A", "d", "x", 10), ("B", "d", "x", 20), ("C", "d", "x", 5)])
        con.executemany("INSERT INTO compta VALUES (?,?,?,?)",
                        [("A", "d", "x", 10), ("B", "d", "x", 21), ("D", "d", "x", 7)])
        statuts = {r[0]: r[3] for r in reconcilier(con)}
        self.assertEqual(statuts, {"A": "OK", "B": "ECART",
                                   "C": "MANQUANT_COMPTA", "D": "MANQUANT_BANQUE"})


if __name__ == "__main__":
    unittest.main()
