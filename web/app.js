const $ = s => document.querySelector(s);
const esc = s => String(s).replace(/[&<>"]/g, c => ({ '&': '&amp;', '<': '&lt;', '>': '&gt;', '"': '&quot;' }[c]));
const nums = s => s.split(/[\s;]+/).filter(Boolean).map(x => Number(x.replace(',', '.')));
const sum = a => a.reduce((s, x) => s + x, 0);
const f2 = x => x.toFixed(2);
const data = { veh: [], etu: [], cli: [] };

function table(heads, rows) {
  if (!rows.length) return '<p class="vide">Aucune ligne pour le moment. Ajoutez-en une avec le formulaire.</p>';
  return '<div class="scroll"><table><thead><tr>' + heads.map(h => `<th>${h}</th>`).join('') +
    '</tr></thead><tbody>' + rows.map(r => '<tr>' + r.map(c => `<td>${esc(c)}</td>`).join('') + '</tr>').join('') +
    '</tbody></table></div>';
}
function msg(id, text, ok) { const el = $(id); el.textContent = text; el.className = 'msg ' + (ok ? 'ok' : 'err'); }
const form = (id, fn) => $(id).addEventListener('submit', e => { e.preventDefault(); fn(new FormData(e.target), e.target); });

/* ---------- Véhicules ---------- */
function renderVeh() {
  $('#veh-table').innerHTML = table(['Immat.', 'Marque', 'Moteur', 'Révisions', 'Coût total'],
    data.veh.map(v => [v.immat, v.marque, `${v.puissance} ch, ${v.carburant}, ${v.cyl} cyl.`, v.couts.length, f2(v.total)]));
}
form('#veh-form', (g, f) => {
  const couts = nums(g.get('couts'));
  if (couts.some(c => isNaN(c) || c < 0)) return msg('#veh-msg', 'Les coûts doivent être des nombres positifs séparés par des espaces.', false);
  data.veh.push({ immat: +g.get('immat'), marque: g.get('marque').trim(), puissance: +g.get('puissance'),
    carburant: g.get('carburant').trim(), cyl: +g.get('cyl'), couts, total: sum(couts) });
  f.reset(); msg('#veh-msg', 'Véhicule ajouté.', true); renderVeh();
});
$('#veh-sort').onclick = () => { data.veh.sort((a, b) => a.total - b.total); renderVeh(); msg('#veh-msg', 'Trié du moins cher au plus cher.', true); };
$('#veh-max').onclick = () => {
  if (!data.veh.length) return msg('#veh-msg', 'Ajoutez d\'abord un véhicule.', false);
  const m = data.veh.reduce((a, b) => b.total > a.total ? b : a);
  msg('#veh-msg', `Le plus coûteux : ${m.marque}, ${f2(m.total)} au total.`, true);
};

/* ---------- Étudiants ---------- */
const jma = d => d.split('-').reverse().join('/');
function renderEtu() {
  $('#etu-table').innerHTML = table(['Code', 'Naissance', 'Notes', 'Moyenne'],
    data.etu.map(e => [e.ce, jma(e.naiss), e.notes.length, f2(e.moy)]));
}
form('#etu-form', (g, f) => {
  const notes = nums(g.get('notes'));
  if (notes.some(n => isNaN(n) || n < 0 || n > 20)) return msg('#etu-msg', 'Les notes doivent être comprises entre 0 et 20.', false);
  data.etu.push({ ce: +g.get('ce'), naiss: g.get('naiss'), notes, moy: notes.length ? sum(notes) / notes.length : 0 });
  f.reset(); msg('#etu-msg', 'Étudiant ajouté.', true); renderEtu();
});
$('#etu-moy').onclick = () => { data.etu.sort((a, b) => b.moy - a.moy); renderEtu(); msg('#etu-msg', 'Trié par moyenne décroissante.', true); };
$('#etu-age').onclick = () => { data.etu.sort((a, b) => a.naiss.localeCompare(b.naiss)); renderEtu(); msg('#etu-msg', 'Trié du plus âgé au plus jeune.', true); };

/* ---------- Clients et factures ---------- */
function renderCli() {
  $('#cli-table').innerHTML = table(['Client', 'Factures', 'Moyenne TTC', 'TTC max', 'TTC < 100'],
    data.cli.map(c => [c.cc, c.f.length, f2(c.moy), c.f.length ? `${f2(c.max.ttc)} (facture ${c.max.cf})` : '-', c.f.filter(x => x.ttc < 100).length]));
  const avec = data.cli.filter(c => c.f.length);
  if (!avec.length) { $('#cli-stats').textContent = ''; return; }
  const best = avec.reduce((a, b) => b.moy > a.moy ? b : a);
  const top = avec.reduce((a, b) => b.max.ttc > a.max.ttc ? b : a);
  $('#cli-stats').textContent = `Meilleure moyenne TTC : client ${best.cc} (${f2(best.moy)}). Plus grand TTC : client ${top.cc} (${f2(top.max.ttc)}).`;
}
form('#cli-form', (g, f) => {
  const lignes = g.get('f').split('\n').map(l => l.trim()).filter(Boolean);
  const fact = lignes.map(l => { const [cf, ht, tva] = nums(l); return { cf, ttc: ht + tva, ok: [cf, ht, tva].every(x => !isNaN(x)) }; });
  if (fact.some(x => !x.ok)) return msg('#cli-msg', 'Chaque ligne doit contenir trois nombres : code, HT et TVA.', false);
  const moy = fact.length ? sum(fact.map(x => x.ttc)) / fact.length : 0;
  data.cli.push({ cc: +g.get('cc'), f: fact, moy, max: fact.length ? fact.reduce((a, b) => b.ttc > a.ttc ? b : a) : null });
  f.reset(); msg('#cli-msg', 'Client ajouté.', true); renderCli();
});

/* ---------- Onglets ---------- */
document.querySelectorAll('nav button').forEach(b => b.onclick = () => {
  document.querySelectorAll('nav button').forEach(x => x.classList.toggle('on', x === b));
  document.querySelectorAll('main section').forEach(s => s.hidden = s.id !== b.dataset.tab);
});
renderVeh(); renderEtu(); renderCli();
