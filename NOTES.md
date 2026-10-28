# NOTES

Diario del percorso S0 + 12 settimane (21/10/2026 - 17/01/2027): cosa ho imparato, cosa blocca, regole da ricordare.

## 2026-10-21 (mer) · S0

**Fatto**
- Setup Ubuntu 24.04 in WSL2, g++ 13, CMake.
- Missing Semester lezioni 1–2 (The Shell, Shell Tools and Scripting): esercizi marco/polo e "esegui finché non fallisce".
- Chiave SSH su GitHub.
- Repo `control-core`: CMakeLists con libreria INTERFACE C++17, GoogleTest via FetchContent, test smoke, GitHub Actions verde, badge CI nel README, primo merge via PR (#1).

**Da rivedere**
- Teoria delle lezioni 1–2; esercizi 4–5 della lezione 2 rimasti indietro (fatti il 22/10).

## 2026-10-22 (gio) · S0

**Fatto**
- Ripasso lezioni 1–2: script `~/bin/conta.sh` corretto (conta gli `.html` sotto una cartella).
- Missing Semester L2, es. 4: zip ricorsivo degli `.html` con nomi con spazi:
  `find . -type f -name '*.html' | xargs -d '\n' zip html.zip`
  (variante: `find … -print0 | xargs -0 …`).
- Missing Semester L2, es. 5: `~/bin/recent.sh [N]`, gli N file modificati più di recente:
  `find . -type f -printf '%T@ %p\n' | sort -nr | head -n "$n" | cut -d' ' -f2-`
- ROS 2 Jazzy + Gazebo Harmonic installati e verificati in WSL2: talker C++ / listener Python, `gz sim shapes.sdf`.
  - RTF Gazebo: ~95–96 % con `shapes.sdf`.
  - Rendering: motore di default (ogre2), nessun workaround necessario in WSLg.

**Da rivedere**
- `while IFS= read -r -d '' f` con `find -print0`: non ancora chiaro del tutto.

### Regole da ricordare

**Variabili e sostituzioni**

| Scrivi | Bash vede |
|---|---|
| `n` | il testo "n" |
| `$n` | il valore della variabile `n` |
| `$(cmd)` | l'output del comando `cmd` |
| `1` / `$1` | il testo "1" / il primo argomento dello script |

- Assegnazione senza spazi: `nome=valore`. Con `nome = valore` bash esegue il comando `nome`.
- Assegnare una variabile non basta: va letta con `"$nome"` dove serve (shellcheck SC2034 = variabile mai usata).
- `[` è un comando: `[ "$n" -eq 0 ]`, con gli spazi.
- Chiudere `if`/`for` dove finisce il ramo (`fi`, `done`), non in fondo al file.
- Ogni `'` aperto va chiuso; dentro gli apici `\` è letterale. Prompt `>` = apice o virgoletta non chiusi → Ctrl+C.

**find, xargs, sort, cut**
- `find` è già ricorsivo (niente `-r`). Le opzioni hanno il trattino, i valori no: `-type f -name '*.html'`. Una parola senza trattino è presa come cartella di partenza.
- Il separatore deve essere lo stesso ai due lati della pipe: `\n` ↔ `xargs -d '\n'`; `-print0` ↔ `xargs -0` / `read -d ''`.
- Mai `for f in $(find …)`: spezza i nomi sugli spazi.
- `sort` di default confronta testo carattere per carattere (`1000` < `999`); per i numeri serve `-n`.
- In `man`, le parole maiuscole (`N`, `FILE`) sono segnaposti. `cut -f2-` = dal campo 2 fino alla fine.

**Ambiente**
- Codice e repo sotto `~` (filesystem Linux), non in `/mnt/c`: lì è lento, `chmod` non si comporta come su Linux e gli editor Windows possono salvare con fine riga CRLF (`file script.sh` per controllare).
- `No such file or directory` su uno script → percorso sbagliato: controllare con `ls -l` e guardare il prompt (`~$` vs `/mnt/c/...$`).
- Errori su stderr con `>&2`; stati di uscita: 0 ok, 1 nessun risultato, 2 uso sbagliato.

## 2026-10-23 (ven) · S0

**Fatto**
- Missing Semester L5 (Command-line Environment): job control (`Ctrl+Z`, `jobs`, `bg`, `fg`, `kill %1`), segnali e `trap`.
  - `~/bin/ferma.sh`: a Ctrl+C stampa i giri fatti ed esce con 0 (`trap '…' INT`).
  - `pidwait` in `~/.bashrc`: aspetta un PID con `kill -0 "$1"`; test `sleep 5 & pidwait $!; echo finito`.
- tmux: sessione `lab`, pannelli, detach/attach; ssh: `~/.ssh/config` per GitHub, `ssh -T git@github.com` (uscita 1 = ok, niente shell). Alias `gs='git status'`.
- control-core: flusso completo da solo (branch → PR → check build → Rebase and merge → pull → pulizia branch). NOTES aggiornato a S0 + 12 settimane.
- `~/git-conflitti`: conflitto di merge (marcatori, `git add`, `git commit`), conflitto di rebase (`git add`, `git rebase --continue`), giro a memoria.
- `git stash`: pop senza conflitto, pop con conflitto (+ `git stash drop`), `stash -u` per i file non tracciati.

**Da rivedere**
- Durante un rebase "ours" = main e "theirs" = il mio commit (inverso rispetto al merge).
- Dopo Rebase and merge l'hash su main cambia: `git branch -d` può rifiutarsi anche se il contenuto è già su main.
- Gli stash si accumulano a pila: `pop` prende solo l'ultimo.

### Regole da ricordare (23/10)

**Shell**
- Un file scritto da solo sulla riga di comando viene eseguito (`~/.bashrc` → Permission denied). Per modificarlo `nano`, per caricarlo `source`.
- Modificare `~/.bashrc` non cambia il terminale aperto finché non faccio `source ~/.bashrc`.
- `kill %1` = job 1, `kill 1` = PID 1 (stessa trappola di `n` vs `$n`). `$!` = PID dell'ultimo processo in background.
- Mai cancellare la cartella in cui mi trovo: prima `cd ~`, poi `rm -rf`. `Unable to read current working directory` = sono in una cartella cancellata → `cd` con il percorso completo.

**Git**
- Ogni comando Git inizia con `git` (`pull` da solo non esiste).
- Dopo il merge su GitHub: prima `git switch main`, poi `git pull`. Mai `pull` sul branch della PR già cancellato.
- Prima di `git branch -D`: `git diff main <branch>` deve essere vuoto. Poi `git fetch --prune` per i branch remoti vecchi.
- Conflitto: cancellare tutti e tre i marcatori e tenere da ciascun lato le righe giuste. Controllo: `grep -c '<<<<<<<' file` = 0.
- Rebase in conflitto: `git add` + `git rebase --continue`, non `git commit`.
- `git stash` senza `-u` lascia i file non tracciati ma prende le modifiche ai file tracciati: prima guardare `git status`. Dopo `pop`, `git stash list` deve essere vuoto.
- Conventional commits: tipo minuscolo (`fix:`, non `FIX:`).

## 2026-10-24 (sab) · S0

**Fatto**
- learncpp capitoli 1 (C++ Basics), 2 (Functions and Files), 4 (Fundamental Data Types), esercizi in `~/cpp-lab`.

**Da rivedere**
- nomi, esercizi, per ora poca roba

## 2026-10-26 (lun) · S1

**Fatto**
- Quiz di ripasso S0 a memoria: 1 giusta su 6 senza aiuto, 3 con un indizio, 2 a metà.

**Da rivedere**
- `$nome` = valore della variabile, `$(cmd)` = output del comando, `nome` = testo.
- Rebase in conflitto: `git add` + `git rebase --continue`, mai `git commit`.
- Dopo il merge su GitHub: `git switch main`, poi `git pull`.
- `kill %1` = job 1, `kill 1` = PID 1.

## 2026-10-27 (mar) · S1

**Fatto**
- learncpp 8.12 + quiz 8.x es. 1–2 (isPrime, con variante).
- ROS 2 turtlesim: nodi, `ros2 node list/info`, remap `__node:=`, servizio `/spawn` da rqt (stringhe tra apici).
- PR docs/s1-plan mergiata; conflitto di rebase in `~/git-lab/rebase-27` risolto con `git add` + `git rebase --continue`.
- `~/cpp-lab/primo_ordine`: ZOH vs Eulero vs analitica; con Ts > 2τ Eulero diverge, ZOH no.
- learncpp 5.1–5.6: variabili `const`, letterali, sistemi numerici, regola as-if, espressioni costanti, `constexpr` (usato in `primo_ordine`).

**Da rivedere**
- learncpp 8.13–8.15 + quiz 8.x es. 3 (Hi-Lo): rimandati a sabato 31/10.
- isPrime: condizione del `for` scritta al contrario e caso `x < 2` dimenticato.
- `std::exp(-Ts / tau)`, non `e exp(1 - Ts/tau)`. ZOH: a = e^(−Ts/τ), b = K·(1 − a): K fuori dall'esponenziale.
- Passare `k * Ts` (il tempo), non `k` (l'indice).
- Copiare solo i comandi, mai il testo descrittivo sulla stessa riga.
- `git diff main <branch>`: righe `-` non bloccano la cancellazione, righe `+` sì.

## 2026-10-28 (mer) · S1

**Fatto**
- Ripasso a memoria: ciclo `for`, correzione `zohA`/`zohB`, `~/bin/quanti.sh`, job control, lettura di `git diff`, comandi dopo il merge.
- ROS 2 Understanding topics: `ros2 topic list/echo/info/pub/hz`, `ros2 interface show`, rqt_graph (togliere la spunta Hide: Debug per vedere `/_ros2cli_…`). `/turtle1/pose` a 62,5 Hz = timer di 16 ms.
- learncpp cap. 6 (operatori) + `~/cpp-lab/confronti`: `approxEqual` assoluto + relativo (6.7). `0.1 + 0.2 == 0.3` → false; `a1 == a2` → true con Ts = 0.1, ma per caso.
- `git revert` con conflitto in `~/git-lab/revert-28b`.
- CMake: primo `CMakeLists.txt` in `~/cpp-lab/primo-cmake`; `cmake -S . -B build` (configura) e `cmake --build build` (compila).
- `~/cpp-lab/primo-multi`: `primo.h` / `primo.cpp` / `main.cpp`, `namespace primo`, CMake con due `.cpp`. ZOH errore 0 rispetto all'analitica; Eulero con Ts = 2.5 diverge a segno alterno (a = −1.5).

**Da rivedere**
- `$PATH` è una variabile di sistema, non il primo argomento (`$1`).
- `'…'` = tutto letterale; `"…"` = le variabili vengono sostituite. Percorsi sempre tra virgolette: `"$1"`.
- `git diff main` confronta main con la cartella; `git diff main <branch>` confronta i due branch. Prima di `-D` servono due nomi.
- Revert in conflitto: `git add` + `git revert --continue`, mai `git commit` (primo tentativo chiuso con `git commit -m "Fixed Ki"`).
- `git status` dice quale operazione è in corso e se `git add` è già fatto.
- Tra double mai `==`: confronto con tolleranza.
- `*.cpp` prende tutti i `.cpp`: con più versioni (più `main`) si compila un file alla volta.
- Un blocco `cat > file << 'EOF'` si incolla nel terminale, non dentro nano.
- `std::max(x, y)` vuole due argomenti; per il valore assoluto si usa `std::abs`.
- Il C++ distingue maiuscole e minuscole: `k` ≠ `K`.
- Errori del compilatore: leggere solo le righe `error:` (file:riga:colonna), le `note:` dopo.
- CMake ricompila solo se il file è salvato davvero: controllare l'orario con `ls -l`.

**Rimandato**
- Quiz learncpp cap. 7 al ripasso del 29/10; finire di leggere 7.1–7.5.
- learncpp 8.13–8.15 + Hi-Lo: sabato 31/10.
