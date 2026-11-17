const $ = s => document.querySelector(s);

const ROLES = [
  { id: 'villageois', nom: 'Villageois', desc: 'Pas de pouvoir. Vous aidez le village à démasquer les loups.' },
  { id: 'loup', nom: 'Loup-garou', desc: 'Chaque nuit, avec les autres loups, vous dévorez un villageois.' },
  { id: 'voyante', nom: 'Voyante', desc: 'Chaque nuit, vous découvrez le rôle d\'un joueur.' },
  { id: 'sorciere', nom: 'Sorcière', desc: 'Une potion de vie et une potion de mort, chacune utilisable une seule fois.' },
  { id: 'protecteur', nom: 'Protecteur', desc: 'Chaque nuit, vous protégez un joueur des loups, sans choisir deux fois le même.' },
  { id: 'ancien', nom: 'Ancien', desc: 'Vous survivez à la première attaque des loups.' },
  { id: 'chasseur', nom: 'Chasseur', desc: 'Quand vous mourez, vous tirez sur le joueur de votre choix.' },
  { id: 'cupidon', nom: 'Cupidon', desc: 'La première nuit, vous désignez deux amoureux.' },
  { id: 'infect', nom: 'Infect Père des Loups', desc: 'Une fois par partie, vous infectez la victime des loups : elle devient loup.' },
];
const etat = { n: 8, c: { loup: 2, voyante: 1, sorciere: 1 } };
let jeu = [], i = 0, vu = false;

const speciaux = () => Object.values(etat.c).reduce((a, b) => a + b, 0);
const alea = () => crypto.getRandomValues(new Uint32Array(1))[0] / 2 ** 32;

function montrer(id) {
  document.querySelectorAll('.ecran').forEach(s => s.hidden = s.id !== id);
}

function rendreReglage() {
  const reste = etat.n - speciaux();
  $('#n').value = etat.n;
  $('#roles').innerHTML = ROLES.filter(r => r.id !== 'villageois').map(r =>
    `<li><div><strong>${r.nom}</strong><span>${r.desc}</span></div>
     <div class="cpt"><button type="button" data-r="${r.id}" data-d="-1" aria-label="Retirer ${r.nom}">−</button>
     <output>${etat.c[r.id] || 0}</output>
     <button type="button" data-r="${r.id}" data-d="1" aria-label="Ajouter ${r.nom}">+</button></div></li>`).join('');
  const err = reste < 0 ? 'Trop de rôles pour ce nombre de joueurs.' : !etat.c.loup ? 'Il faut au moins un loup-garou.' : '';
  $('#bilan').textContent = err || `${reste} villageois complètent la partie.`;
  $('#bilan').className = err ? 'err' : '';
  $('#start').disabled = !!err;
}

$('#n').onchange = e => { etat.n = Math.min(30, Math.max(4, +e.target.value || 4)); rendreReglage(); };
$('#roles').onclick = e => {
  const b = e.target.closest('button');
  if (!b) return;
  etat.c[b.dataset.r] = Math.max(0, (etat.c[b.dataset.r] || 0) + +b.dataset.d);
  rendreReglage();
};

function melanger(a) {   // Fisher-Yates
  for (let k = a.length - 1; k > 0; k--) {
    const j = Math.floor(alea() * (k + 1));
    [a[k], a[j]] = [a[j], a[k]];
  }
  return a;
}

$('#start').onclick = () => {
  const pioche = [];
  ROLES.forEach(r => {
    const nb = r.id === 'villageois' ? etat.n - speciaux() : (etat.c[r.id] || 0);
    for (let k = 0; k < nb; k++) pioche.push(r);
  });
  jeu = melanger(pioche); i = 0; vu = false;
  montrer('deal'); rendreCarte();
};

function rendreCarte() {
  $('#joueur').textContent = `Joueur ${i + 1} sur ${jeu.length}`;
  $('#carte').classList.toggle('retournee', vu);
  $('#consigne').textContent = vu
    ? 'Touchez la carte pour la cacher, puis passez le téléphone.'
    : 'Touchez la carte pour voir votre rôle.';
}

$('#carte').onclick = () => {
  if (!vu) {
    $('#role-nom').textContent = jeu[i].nom;
    $('#role-desc').textContent = jeu[i].desc;
    vu = true;
  } else {
    $('#role-nom').textContent = $('#role-desc').textContent = '';   // le joueur suivant ne doit rien voir
    vu = false; i++;
    if (i >= jeu.length) return montrer('fin');
  }
  rendreCarte();
};

$('#conteur').onclick = () => {
  if (!confirm('Réservé au conteur : tous les rôles vont s\'afficher. Continuer ?')) return;
  $('#liste').innerHTML = jeu.map((r, k) => `<li>Joueur ${k + 1} : ${r.nom}</li>`).join('');
  $('#liste').hidden = false;
};
$('#rejouer').onclick = () => { $('#liste').hidden = true; montrer('reglage'); rendreReglage(); };

rendreReglage();
