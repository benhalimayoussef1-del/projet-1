const $ = s => document.querySelector(s);
let total = 300, restant = 300, fin = 0, tick = null, audio = null;

const fmt = s => String(Math.floor(s / 60)).padStart(2, '0') + ':' + String(s % 60).padStart(2, '0');

function maj() {
  $('#temps').textContent = fmt(restant);
  $('#temps').classList.toggle('fini', restant === 0);
  $('#barre').style.width = (total ? 100 * (total - restant) / total : 0) + '%';
  document.title = tick ? fmt(restant) + ' - Minuteur' : 'Minuteur';
}
function pause() { clearInterval(tick); tick = null; $('#go').textContent = 'Démarrer'; maj(); }
function regler(s) { pause(); total = restant = Math.max(0, Math.floor(s)); maj(); }

function bip() {
  try {
    audio = audio || new AudioContext();
    [0, 0.35, 0.7].forEach(t => {
      const o = audio.createOscillator(), g = audio.createGain();
      o.frequency.value = 880; o.connect(g); g.connect(audio.destination);
      g.gain.setValueAtTime(0.2, audio.currentTime + t);
      g.gain.setValueAtTime(0, audio.currentTime + t + 0.2);
      o.start(audio.currentTime + t); o.stop(audio.currentTime + t + 0.25);
    });
  } catch (e) { /* son indisponible : la vibration suffit */ }
  if (navigator.vibrate) navigator.vibrate([200, 100, 200, 100, 400]);
}

function demarrer() {
  if (restant <= 0) return;
  audio = audio || new AudioContext();   // créé pendant le clic, sinon le navigateur bloque le son
  fin = Date.now() + restant * 1000;     // on se base sur l'heure de fin : pas de dérive
  tick = setInterval(() => {
    restant = Math.max(0, Math.ceil((fin - Date.now()) / 1000));
    maj();
    if (restant === 0) { pause(); bip(); }
  }, 250);
  $('#go').textContent = 'Pause';
}

$('#go').onclick = () => tick ? pause() : demarrer();
$('#raz').onclick = () => regler(total);
document.querySelectorAll('.presets button').forEach(b => b.onclick = () => regler(+b.dataset.s));
$('#reglage').addEventListener('submit', e => {
  e.preventDefault();
  regler((+$('#min').value || 0) * 60 + (+$('#sec').value || 0));
});

if ('serviceWorker' in navigator) navigator.serviceWorker.register('sw.js');
maj();
