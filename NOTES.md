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

## 2026-10-29 (gio) · S1

**Fatto**
- Ripasso a memoria: `approxEqual` con `std::abs`, previsione di `x++`, `~/bin/righe.sh`, `find … -print0 | xargs -0 wc -l`, job control (`kill %1`), revert in conflitto in `~/git-lab/ripasso-29`, lettura di `git diff main <branch>`.
- learncpp 7.1–7.5 + quiz 7.x in `~/cpp-lab/cap7`; `scope.cpp` con `-Wshadow`.
- ROS 2 Understanding services: `ros2 service list -t`, `ros2 interface show turtlesim/srv/Spawn`, `/spawn` di `tartaruga2`, `topic pub --once` su `/tartaruga2/cmd_vel`, `/clear`.
- Tag in `~/git-lab/tag-29`: `v0.1.0` annotato su `HEAD~1`, tag leggero `prova`.
- learncpp 12.1–12.6 (riferimenti) + `~/cpp-lab/riferimenti`: `zohCoeffs` e `passo` con `double&`, `x(5) = 1.99991`, `errMax = 2.2e-16` (= eps, ZOH esatto); `passoCopia` lascia `xc = 0`.

**Da rivedere**

*C++*
- `std::max(x, y)` = il più grande tra due valori; `std::abs(x)` = distanza da zero (seconda volta).
- `x++` restituisce il valore vecchio, `++x` quello nuovo; `x` viene incrementata subito.
- Shadowing: la variabile esterna è nascosta, non persa. `-Wshadow` avvisa del rischio di modificare quella sbagliata.
- Dichiarazione (`.h`) = firma con `;`. Definizione (`.cpp`) = firma senza `;` seguita da `{ … }`.
- Una funzione `void` non fa `return valore;`: il risultato esce assegnandolo al parametro `&`.
- A un `double&` si passa una variabile, mai un numero (`1.0` è un rvalue). `const T&` accetta tutto.
- Tipi fondamentali per valore; `const T&` per i tipi costosi da copiare; `T&` se la funzione deve modificare l'argomento.
- Il tempo è indice × Ts: `(k + 1) * Ts`, non `k + 1` (seconda volta).
- Un file per ruolo: `main.cpp` per il `main`, `primo.cpp` per le definizioni.

*CMake*
- Il nome in `add_executable(NOME …)` va ripetuto identico in ogni `target_…(NOME …)`.
- `CMakeLists.txt` con la L maiuscola: Linux distingue maiuscole e minuscole.
- `Cannot find source file` = il file non c'è o ha un altro nome: `ls -l`.

*Shell*
- `[ "$1" ]` è vero se l'argomento c'è; per "manca" serve `[ -z "$1" ]`.
- `exit 2` = stato di uscita; `>&2` = output su stderr; `>2` = scrive in un file chiamato `2`.
- `wc -l < "$1"` dà solo il numero; `wc -l` senza file legge dalla tastiera.
- `xargs` è un comando, senza trattino. `.` = cartella corrente come argomento; `pwd` = comando che la stampa.

*Git*
- `git revert` vuole un hash o `HEAD~N`, mai il messaggio del commit. `git log --oneline -1` mostra solo l'ultimo.
- `fatal: not a git repository` = cartella sbagliata: guardare il prompt.
- `git tag -a NOME -m "MESSAGGIO" COMMIT`: prima il nome del tag nuovo, il messaggio solo con `-m`.
- Un commit prima = `HEAD~1` (tilde, AltGr + ì), non `HEAD-1`.

*ROS 2*
- Topic = flusso continuo, un verso, senza risposta (come una linea di segnale in Simulink). Servizio = richiesta → risposta, una volta (come chiamare una funzione).

**Rimandato**
- Pro Git 2.6 (Tagging) + compiti 3–5 sui tag (`git tag -n`, `git diff v0.1.0 HEAD`, cancellare `prova`): ven 30/10.
- learncpp 8.13–8.15 + Hi-Lo: sabato 31/10.

## 2026-10-30 (ven) · S1

**Fatto**
- Ripasso a memoria: previsione `x++`/`++x`, `zohStep` con `double&` (x(1) = 1.72933, errore < 1e-12), `~/bin/controlla.sh` (stderr + `exit 2`), job control, `git revert HEAD~N` in `~/git-lab/ripasso-30`, ours/theirs nel rebase.
- ROS 2 Understanding parameters: `ros2 param list/get/set/dump`, `background_r` = 150, `~/ros2-lab/turtlesim.yaml`, riavvio con `--ros-args --params-file`.
- learncpp 16.1–16.6, 16.8 (`std::vector`, `std::size_t`, range-based for) + `~/cpp-lab/vettori`: `media` = −0.166667, `maxAbs` = 4.
- Pro Git 2.6 Tagging + compiti 3–5 in `~/git-lab/tag-29`: `git tag -n`, `git diff v0.1.0 HEAD`, `git tag -d prova`.
- `~/cpp-lab/simula-vettori` (`sim.h`/`sim.cpp`/`main.cpp`, namespace `sim`, CMake con `-Wall -Wextra -Wshadow`): `tempi`, `simula`, `analitica`, `errMax`.
  - Ts = 0.1, N = 50: x(5) = 1.99991, errMax ZOH = 2.2e-16, errMax Eulero = 0.0804.
  - Eulero con orizzonte 5 s: N = 25/50/100/200 → 0.1787 / 0.0804 / 0.0384 / 0.0188 (si dimezza con Ts: primo ordine).

**Da rivedere**

*C++*
- Dopo il passo k lo stato è x[k+1], al tempo `(k + 1) * Ts` (terza volta, corretto da solo).
- Nel `.cpp` niente `;` dopo la firma della definizione (terza volta).
- Un'operazione da fare una volta (dividere per n) va dopo il ciclo, non dentro.
- In un "massimo di |…|" si confronta e si salva la stessa quantità: `std::abs(v[j])`, non `v[j]`.
- `std::vector<double> x(N)` crea N elementi: tra tonde va la lunghezza totale (`u.size() + 1`, non `1`).
- Il ciclo che riempie un vettore usa `v.size()` di quel vettore (`k < n` lasciava `t[n] = 0` → errMax = valore finale).
- `v` = vettore intero, `v[k]` = un elemento: `std::cout` vuole un elemento, `errMax`/`simula`/`analitica` il vettore.
- Range-based for: il tipo è quello dell'elemento (`double&` per modificare), il nome è libero e vive solo nel ciclo.
- Ciclo a indice se serve la posizione `k`; range-based for se serve solo l'elemento.
- Dentro un ciclo usare solo le variabili del giro (`NB`, `tB`), non quelle del blocco precedente.
- Una variabile locale non prende il nome della funzione (`media` dentro `media`).

*Errori del compilatore e a runtime*
- `expected ';' before X` → manca il `;` nella riga prima di X.
- `expected unqualified-id before '{'` → `;` di troppo dopo la firma nella riga sopra.
- `no match for 'operator<<' … std::vector` → sto stampando un vettore intero.
- Con un muro di `note:` leggere solo la prima riga `error:`.
- `malloc(): corrupted …` = scrittura fuori da un vettore fatta prima di quel punto; `-g -fsanitize=address` dà la riga.

*Shell*
- `syntax error near unexpected token 'fi'` → manca `then`, o il `;` prima di `then`, o gli spazi in `[ … ]`.
- Shebang `#!/bin/bash` sulla riga 1, senza spazi prima.
- `cd: No such file or directory` → la cartella non esiste: `mkdir -p`.

*Git*
- Rebase: ours = main (dove Git è seduto), theirs = il mio commit; nel merge è il contrario (seconda volta).
- Prima di modificare: `git status` → main pulito → `git pull` → `git switch -c NOME`.
- `git diff A B`: due nomi (branch, tag, hash, `HEAD~N`), mai tre; `git` + spazio + sottocomando.
- Tag annotato = oggetto con autore, data, messaggio; tag leggero = solo puntatore. `git push origin NOMETAG` o `--tags`.
- Prima di `git tag -d` rileggere il nome (cancellato `v0.1.0` per sbaglio, ricreato su `HEAD~1`).

*ROS 2*
- Parametro = configurazione del nodo (il K del blocco Gain); topic = segnale che scorre; servizio = richiesta → risposta.
- `ros2 param dump` stampa YAML (redirect `>` per salvarlo); `param load` = nodo già avviato; all'avvio `--ros-args --params-file FILE`.

**Rimandato**
- learncpp 8.13–8.15 + Hi-Lo: sabato 31/10.

## 2026-10-31 (sab) · S1

**Fatto**
- Ripasso: `~/bin/esiste.sh FILE` (stderr + `exit 2` se manca l'argomento, `0` se il file esiste, `1` se no); messaggio conventional per il commit del 30/10: `docs(notes): add 30 Oct log`.
- learncpp 8.13–8.15 (`Random.h`) + `~/cpp-lab/random/dado.cpp`: 6000 tiri, conteggi ~1000 per faccia.
- Quiz 8.x es. 3 Hi-Lo in `~/cpp-lab/hilo`: 7 tentativi, `playAgain` con `do-while`.

**Da rivedere**

*Shell*
- Prompt `~/bin/esiste.sh$` = sono dentro una cartella con quel nome. Gli script si creano con `nano ~/bin/nome.sh`, non con `mkdir`.
- `ls -l` su un file = una riga; su una cartella = `total` + contenuto.
- `rmdir` cancella solo cartelle vuote. `mv A B` con `B` cartella mette `A` dentro `B`, non lo rinomina.
- `source` solo per `~/.bashrc`: su uno script con `exit` chiude il terminale (ero finito in PowerShell, `wsl` per tornare).
- `>&2` = stderr; `>2` = crea un file chiamato `2` (seconda volta).
- `[ … ]` e `test …` sono lo stesso comando: mai insieme.
- I segnaposti maiuscoli di `man` (`FILE`) si sostituiscono con il valore vero, qui `"$1"` (seconda volta).
- `nano nome` apre il file nella cartella corrente; se non c'è ne crea uno vuoto. Per modificare uno script in `~/bin`: `nano ~/bin/nome.sh`.
- `which nome` vuoto = file mancante, non eseguibile, o cartella non nel `PATH`.
- `bash -x script argomenti` mostra ogni riga con le variabili sostituite: primo strumento quando il risultato è sbagliato.

*C++*
- `… has not been declared` = dichiarazione non vista: controllare `#include "File.h"` e che il `.h` non sia vuoto (`touch` crea file vuoti, `wc -l` per controllare).
- `do { … } while (condizione);`: `;` solo alla fine, un solo `while` dopo la `}`.
- Con più errori correggo solo il primo e ricompilo. `expected X before Y` = l'errore è appena prima di Y.
- `if`/`else` senza graffe controllano una sola istruzione: `return true;` fuori dalle graffe veniva eseguito sempre. L'indentazione non conta.
- `std::cin >> ch` con `char` legge un carattere alla volta: `76` = due risposte.
- Caratteri tra apici singoli: `'y'` è un `char`, `"y"` una stringa.

*Git*
- Conventional commit = `tipo(scope): descrizione`; per NOTES sempre `docs(notes): …`. L'hash lo genera Git.
- Senza modifiche `git commit` risponde `nothing to commit`: prima `git status`.

**Rimandato**
- learncpp 13.1, 13.7–13.10 (struct) + `~/cpp-lab/struct-zoh`: dom 1/11, insieme a NOTES e PR.

## 2026-11-03 (mar) · S2

**Fatto**
- Ripasso a memoria: `tempo.cpp`, previsione `for` senza graffe (`-Wmisleading-indentation`), `righe2.sh`, `mv`, `source` vs `bash`, tag annotato `v0.2.0` su `HEAD~1`, ours/theirs nel rebase.
- ROS 2 Understanding actions: `ros2 action list -t/info/send_goal --feedback`, `interface show turtlesim/action/RotateAbsolute`.
- learncpp 13.1, 13.7–13.10 + `~/cpp-lab/struct-zoh`: a = 0.818730753, b = 0.362538494, x(5) = 1.9999092.
- Eigen 3.4.0 installato (`libeigen3-dev`).

**Da rivedere**
- Senza graffe `for`/`if`/`else` comandano una sola istruzione: graffe sempre (seconda volta).
- `source` esegue nel terminale corrente (un `exit` lo chiude); `bash script.sh` in un processo figlio.
- `mv ORIGINE DESTINAZIONE`: due argomenti, niente parole in più.
- Azione: goal e result = servizi, feedback = topic. Cancel = lo chiede il client; abort = lo decide il server.
- I tasti del teleop vanno premuti nel pannello del teleop; `f` cancella la rotazione.
- Struct: i valori tra `{ }` riempiono i membri nell'ordine di dichiarazione; i restanti prendono il default.
- Passare una struct per `const Nome&`; `Nome&` solo per modificarla. Con `Nome p;` senza `{}` i membri senza default non sono inizializzati.
- L'output deve seguire il formato atteso (ordine dei membri, spazi).

## 2026-11-04 (mer) · S2

**Fatto**
- Ripasso a memoria: struct + `for` senza graffe, `maxAbsErr` (2.5), `~/bin/conta-righe.sh` (exit 2/1/0), quoting `'…'` vs `"…"`, sequenza dopo il merge, revert in conflitto.
- ROS 2 logging: livelli FATAL → DEBUG, `/rosout` (topic), rqt_console, `--ros-args --log-level WARN`.
- learncpp 14.1–14.5 + `~/cpp-lab/classe-zoh`: classe `FirstOrder` con `step`, `state() const`, membri `private`. a = 0.818730753, b = 0.362538494, x(5) = 1.9999092, errore 0.
- Pro Git 7.7 Reset Demystified + `~/git-lab/reset-03`: `--soft`, `--mixed`, `--hard`, squashing, `git reset FILE`.
- learncpp 14.9–14.11: costruttore con member initializer list in classe-zoh; prova `-Wreorder` (b = −4e+214 con l'ordine sbagliato).

**Da rivedere**

*Git*
- soft = HEAD; mixed = HEAD + index; hard = HEAD + index + cartella. `git reset HASH` senza opzione = `--mixed`.
- Dopo `--mixed` serve di nuovo `git add` prima del commit.
- `--hard` distrugge le modifiche non committate: prima `git status`.
- Prima di `branch -d`/`-D`: `git diff main <branch>`. Il branch remoto si cancella su GitHub; `fetch --prune` pulisce solo i riferimenti locali.
- In un'operazione in corso: `git add FILE` + `git <operazione> --continue`, mai `git commit`.

*Shell*
- `"feat: riga $4"` → `$4` è il 4° argomento (vuoto): il testo si scrive senza `$` (terza volta).
- Il percorso di un file è relativo alla cartella del prompt.
- Tilde: se AltGr + ì non funziona, `HEAD^`, hash o `$HOME`.

*C++*
- Le funzioni membro si chiamano sull'oggetto (`sys.step()`), non sulla classe.
- La classe tiene lo stato, chi la usa tiene la storia (vettore nel `main`).
- Dichiarato un costruttore, il costruttore di default sparisce.
- I membri si inizializzano nell'ordine di dichiarazione, non della lista: `-Wreorder` e `-Wuninitialized` non si ignorano.
- L'etichetta di una stampa dice cosa si stampa (errore ≠ analitica).

*ROS 2*
- `/rosout` è un topic: senza risposta = topic (seconda volta). "Feedback" solo per le azioni.

**Rimandato**
- struct-zoh: formato di stampa e riga `q{2.0}`.

## 2026-11-05 (gio) · S2

**Fatto**
- Ripasso: C++ 1–2, `mv`, `source` vs `bash`, `git reset HEAD^`.
- GoogleTest Primer: Basic Concepts, Assertions, Simple Tests, `EXPECT_NEAR`.
- `FirstOrderZoh` in control-core (`include/control_core/first_order_zoh.h`) + 2 test gtest, 3/3 ctest, PR mergiata (`6db9ab0`).

**Da rivedere**

*C++*
- `TEST(Suite, Nome)`: prima la suite, poi il nome del test.
- Funzioni membro sull'oggetto (`g.apply`), non sulla classe (seconda volta).
- Una variabile dichiarata nel `for` non esiste dopo la `}`: la stampa va dentro il ciclo.
- Il nome dell'argomento passato non deve coincidere con quello del parametro.
- `x++` restituisce il valore vecchio, `++x` quello nuovo (seconda volta).

*Shell / Git*
- I file di test vanno in `tests/`: controllare la cartella del prompt prima di `nano`.
- Il messaggio di commit non è un nome di file.
- `git log` funziona solo dentro il repo giusto.

**Rimandato**
- ROS 2 launch e NOTES 5/11: ven 6/11.

## 2026-11-06 (ven) · S2

**Fatto**
- Ripasso: `k++`/`++k`, `gain.cpp` (oggetto vs classe, scope del `for`), `mv`, `primo-arg.sh`, `git reset HEAD^` = `--mixed` (corretto senza aiuto).
- ROS 2 Launch (Concepts + tutorial): `ros2 launch turtlesim multisim.launch.py`, due turtlesim in namespace diversi.
- learncpp 11.6, 11.7, 11.9 + `~/cpp-lab/template-zoh`: `quadrato<T>`, `simulaN<N>`, x(1) = 1.72933, x(5) = 1.99991.
- Pro Git 2.3 Viewing the Commit History: `-p`, `--stat`, `--since`, `-S`, `-- percorso`.
- PR NOTES 5/11 mergiata (`d0129bb`).
- Eigen: The Matrix class + Matrix and vector arithmetic. Van Loan su carta per il doppio integratore (M³ = 0).
- `~/cpp-lab/eigen-doppio` (CMake + `find_package(Eigen3)`): x(5) = [12.5; 5], errori 7e-15 e 2e-15.

**Da rivedere**

*C++ / Eigen*
- Dopo k passi il tempo è `k * Ts` (e `N * Ts`), non `k`/`N` (terza volta).
- `x(i)` in Eigen = elemento i-esimo, non "x al tempo i": la soluzione analitica è una formula a parte.
- Tra `< >` vanno i valori noti in compilazione (`simulaN<50>`), tra `( )` gli argomenti a runtime.

*Git*
- `git switch -d` = detach, non delete. Per cancellare: `git branch -d`/`-D`. `HEAD detached` → `git switch main`.
- Dopo Rebase and merge l'hash cambia: `git diff main <branch>` vuoto → `git branch -D`.
- `--` da solo = separatore tra opzioni e percorsi; `--stat` = opzione lunga.

*ROS 2*
- Nome del topic = namespace + nome (`/turtlesim1/turtle1/cmd_vel`). `pub`/`echo`/`list` sono verbi di `ros2 topic`.
- Launch file: Python, XML, YAML.

## 2026-11-07 – 2026-11-09 · S2/S3

- 7/11 riposo (piano non fatto). 8–9/11: learncpp 13.13 (class templates).

## 2026-11-10 (mar) · S3

**Fatto**
- Ripasso: prevedi10 (k*Ts, x(1) = velocità), zoh10 (-Wreorder, oggetto vs classe, graffe), `firma.sh`, `mv`, `git reset --mixed HEAD~1`, `switch -d` + stash.
- Eigen: Block operations + `MatrixBase::exp()`; `~/cpp-lab/vanloan`: Ad = [1 0.1; 0 1], Bd = [0.005; 0.1].
- ROS 2 rosbag2: record/info/play su turtlesim (`tartaruga2`: cmd_vel 82 msg, pose ~64 Hz).
- Pro Git 5.3 cherry-pick + `~/git-lab/cherry-10`.
- `StateSpace<Nx,Nu>` con ZOH Van Loan in control-core, Eigen in CMake e CI, 2 test doppio integratore (5/5), PR mergiata.

**Da rivedere**

*Git*
- `HEAD~1` (tilde) o `HEAD^`, mai `HEAD-1` (seconda volta).
- `git reset X` = torna su X (destinazione); `git revert X` = annulla X.
- `git switch -d` = detach, non delete (seconda volta).
- Con modifiche non committate Git può rifiutare lo switch: prima `git status`, poi commit o stash.
- Cherry-pick crea un commit nuovo (hash diverso: padre diverso). Conflitto: `git add` + `git cherry-pick --continue`.

*Shell*
- `source` con `exit` chiude il terminale; gli script con `bash` o con il loro nome (terza volta).

*C++ / Eigen / CMake*
- `.block(i, j, p, q)`: inizio + dimensione, indici da 0.
- Van Loan: il blocco I nasce da u̇ = 0 (ZOH), non da y = u.
- `.exp()` e le operazioni Eigen sono espressioni pigre: risultato in `Eigen::Matrix<…>`, mai `auto`.
- `(A * B).exp()` ≠ `A * B.exp()`: parentesi.
- `template <int Nx, int Nu>`: tra `< >` vanno numeri, non tipi.
- `add_executable(NOME tests/file.cpp)`: il sorgente con l'estensione.
- Un argomento vuole un valore, non un tipo: `Eigen::Matrix<double, 2, 1>::Zero()`.
- Copiando una riga controllare che sia arrivata intera fino al `;`.

*ROS 2*
- Teleop = un messaggio per tasto, solo con la sua finestra attiva.
- `record` si ferma con Ctrl+C; `play` finisce da solo e pubblica lui i messaggi.

**Rimandato**
- Test contro scipy (`tools/ref_state_space.py`): dom 15/11 se possibile, altrimenti lun 16/11.
- RK4 e cart-pole: S4.

## 2026-11-16 (lun) · S4

**Fatto**
- Ripasso a memoria: `blocco.cpp` (`.block`), `errori.cpp` (3 errori), `~/bin/esci.sh` (source vs bash), `~/bin/conta-ext.sh` (find | wc -l), commit su main senza branch riparato in `~/git-lab/main-16`, `git switch -d HEAD~1`.
- Van Loan su carta per A = [0 1; -2 -3], B = [0; 1]: Mᵏ = [Aᵏ Aᵏ⁻¹B; 0 0], e^{At} con Cayley–Hamilton, Bd = ∫₀ᵀ e^{Aτ} dτ B. Ts = 0.1: Ad(0,1) = Bd(1) = 0.0861066649579777, Bd(0) = 0.0045279585030314.
- `tools/ref_state_space.py` (scipy `cont2discrete`, zoh) + test `MatchesScipyZoh` e `StepResponseMatchesAnalytic` (x(5) = [0.4932847530; 0.0066925471], tol 1e-10). 7/7 ctest.

**Da rivedere**

*C++ / Eigen*
- Si inizializza con un valore, non con un tipo: `Eigen::Matrix<double, 2, 2>::Zero()`, con `::`, `Zero` senza s e `()` (terza volta).
- `template <int N>`: tra `< >` un numero (`2`), non un tipo (`double`) (seconda volta).
- `auto` su un'espressione Eigen = espressione pigra, non matrice; il tipo degli elementi non c'entra. Sempre `Eigen::Matrix<…>` (seconda volta).
- `extended character … is not valid in an identifier` = carattere non ASCII copiato (`−`, `“ ”`). Cercarlo con `grep -nP '[^\x00-\x7F]' FILE`. Il segno meno si riscrive a mano.

*Test*
- Copiando un test si cambiano per primi i dati (A, B, Ts) e i commenti.
- Test fallito: leggere i valori in `evaluates to` (Ad = [1 0.1; 0 1] = doppio integratore).
- Valori di riferimento = costanti nel test + commento con lo script che li genera.

*Shell*
- Se la domanda chiede cosa stampa un comando, rispondere con l'output esatto (`ciao`, `3`) (quarta volta).
- `|` = l'output diventa input del comando dopo; `$(cmd)` = l'output diventa testo nella riga di comando.
- `wc -l` senza argomenti conta le righe dalla pipe; `wc -l FILE` quelle di FILE (seconda volta).
- Per contare file non serve `-exec`. `find .` scende anche in `build/`.

*Git*
- `git reset X` sposta il branch su cui sono: prima vado sul branch da correggere, X = destinazione (seconda volta).
- Commit su main senza branch: `switch -c NOME` → `switch main` → `reset --hard origin/main` → `switch NOME` → `push`.

*Matematica*
- ∫₀ᵀ e^{-aτ} dτ = (1 - e^{-aT})/a, positivo per a > 0; il segno del coefficiente resta davanti.

**Consolidati** (1 volta su 2 senza indizi): `.block`, `HEAD~1`, `switch -d`.

**Rimandato**
- rosbag record/play e piano S4 in NOTES: mar 17/11.

## 2026-11-17 (mar) · S4

Giornata ridotta (colloquio di lavoro).

**Fatto**
- Ripasso: `zeri.cpp` (`::Zero()`, `template <int>`), `blocco17`, `wc -l`, grep non ASCII, commit su main spostati su `feat/x`, rosbag a parole.
- learncpp 20.6 Introduction to lambdas.

**Da rivedere**

*C++ / Eigen*
- `::Zero()` con `::`, non `.` (quarta volta).
- `template <int N>`: si chiama con un numero (`<3>`), non con un tipo (terza volta).

*Shell*
- `wc -l` conta i `\n`: `printf 'a\nb\nc'` → 2 (terza volta).
- `grep -nP '[^\x00-\x7F]' FILE` è una ricetta da copiare, non da sapere a memoria.

*Git*
- `git reset` sul branch da correggere, con destinazione `HEAD~N` o hash, mai un messaggio di commit.
- `HEAD~2` con la tilde; per spostare anche i file serve `--hard`.
- `git switch -c` solo per un branch nuovo, senza spazio tra `-` e `c`.

*ROS 2*
- rosbag `record`/`play` invertiti (seconda volta).

**Rimandato**
- learncpp 20.7, lambda-17, amend + reflog, RK4: fatti il 18/11.

## 2026-11-18 (mer) · S4

**Fatto**
- Ripasso: `zeri18.cpp` (3 errori corretti), `blocco18` (`.block` giusto), `wc -l` con e senza `\n` finale, `echo "righe: $(cat FILE | wc -l)"` (alternativa: `wc -l < FILE`), `~/git-lab/main-18` (`switch -c feat/y` → `switch main` → `reset --hard HEAD~2`, al primo colpo).
- learncpp 20.7 Lambda captures + `~/cpp-lab/lambda-17`: per valore 0.1073741824, per riferimento 0.3486784401.
- Pro Git 7.6 Changing the Last Commit + 7.1 RefLog Shortnames + `~/git-lab/amend-18` (amend del messaggio, amend con `--no-edit`, recupero con `HEAD@{1}`).
- `rk4Step<Nx,Nu>` in control-core (`include/control_core/rk4.h`) + 3 test: `ScalarDecayOneStep` (0.9048375, tol 1e-15), `FourthOrderConvergence` (rapporto ≈ 16.7), `MatchesStateSpaceZoh` (tol 1e-9). 10/10 ctest, PR mergiata.
- Cart-pole su carta: Lagrange, equazioni non lineari, equilibri (θ = 0, π; x qualsiasi), linearizzazione: A(2,3) = -0.981, A(4,3) = 21.582, B = [0; 1; 0; -2], autovalori 0, 0, ±4.646.

**Da rivedere**

*C++ / Eigen*
- Una funzione passata come parametro si chiama: `h*f(x)`, non `h*f`.
- Integrazione: lo stato è input e output del passo, `x = passo(x)`; x0 serve solo a inizializzare (due volte oggi).
- `f{…}` inizializza, `f(…)` chiama: `std::exp(-1.0)`.
- `x(0)` di una matrice Eigen è un `double`: gli scalari vanno in `double`, non in `Matrix<1,1>`.
- Una variabile `const` non si assegna con `<<`: si dichiara senza `const`, poi `<<`.
- In `<<` si separa solo con virgole; il `;` chiude l'istruzione (non è MATLAB).
- `Matrix<…> v{}` in Eigen NON azzera: valori casuali. Per gli zeri si usa `::Zero()`.
- `sys.state()(0)`: prima la chiamata, poi l'indice.
- Le dimensioni delle lambda vanno controllate: `u` è `<1,1>`, `x` è `<2,1>`.
- Lambda con Eigen: tipo di ritorno esplicito `-> Eigen::Matrix<…>`, altrimenti restituisce un'espressione pigra.
- `Eigen::Matrix<…> x = 1.0;` non compila: si dichiara, poi `x << 1.0;`.

*Shell*
- Pipe: il comando che produce l'output va a sinistra di `|`, `wc -l` a destra.

*Git*
- `git switch -d HEAD~1` stacca HEAD (non il commit); si torna con `git switch main`, non con `reset` (terza volta).
- `switch` sposta HEAD, `reset` sposta il branch.
- Dopo `git add` serve l'amend (o il commit): verificare con `git show --stat`.
- `reset --hard` cancella anche lo staging; il reflog recupera solo i commit.
- `~` va indietro nella storia; per tornare a un commit perso si usa `HEAD@{n}` o l'hash dal reflog.

*ROS 2*
- `record` = ascolta e registra, si ferma con Ctrl+C; `play` = pubblica e finisce da solo (terza volta).

**Consolidati**: `.block` (2 su 2 senza indizi), `HEAD~N` nel reset.

**Rimandato**
- `cart_pole.h` in C++ + test equilibrio e linearizzazione: gio 19/11.
- S4 in ritardo: smart pointer, move, optional/variant, DARE/LQR, Kalman, LESO, Python/pybind11 da ripianificare.


## 2026-11-19 (gio) · S4

**Fatto**
- Ripasso: `euler19.cpp` (3 errori, terzo con indizio), `scalare19.cpp`, `echo "header: $(find include … | wc -l)"` = 3, `.cpp` in `.` vs `tests` (7 vs 4), `~/git-lab/ripasso-19` (`switch -d` + ritorno su main, recupero con `HEAD@{1}`).
- learncpp 7.9 (inline), 12.7–12.10 (puntatori), 19.1 (new/delete) + `~/cpp-lab/puntatori-19`: x(5) = 1.99991, `nullptr` gestito, leak di 24 byte visto con ASan senza `delete`.
- Pro Git 7.10 (File Annotation, Binary Search): `git blame -L 24,24` e `git log -S` su `state_space.h` → `fa5199d`.
- `cart_pole.h` (dinamica non lineare + linearizzazione in alto) + 4 test: equilibri in alto/in basso (tol 1e-12), A e B contro differenze centrali (ε = 1e-6, tol 1e-6), energia con RK4 (h = 1e-3, 1 s, tol 1e-8). 14/14 ctest, PR mergiata.

**Da rivedere**

*C++ / Eigen*
- Lambda con Eigen: sempre `-> Eigen::Matrix<…>`, altrimenti restituisce un'espressione pigra come `auto` (terza volta, con indizio).
- `x0 << 1.0, 2.0;` su una riga è corretto: l'a capo non conta, il comma initializer riempie riga per riga.
- Un ingresso solo letto si passa per `const Tipo&`: il `const` non è un errore.
- `xp(j)` è uno scalare, `xp` il vettore; `j` è l'indice, non la perturbazione: `xp(j) = xp(j) + eps`.
- Su una matrice servono due indici `A(i, j)`; `A(j)` = indice lineare per colonne (solo la colonna 0 per j < 4).
- Un test che passa può non controllare niente (`Bn` a zero, `A(j)`): farlo fallire apposta una volta.
- Gli argomenti si abbinano per posizione: `invalid initialization of reference of type X from Y` = ordine sbagliato.
- `unused variable` su variabili delle formule = la formula non le usa: rileggerla.
- `X has not been declared` per un namespace = manca `namespace … { }` nell'header (o si chiude troppo presto).
- Le variabili locali di una funzione non esistono fuori (`c` di `cartPoleDynamics`).
- Puntatori: `p->metodo()` = `(*p).metodo()`; controllare `nullptr` prima; ogni `new` con il suo `delete`, altrimenti ASan: `detected memory leaks`.

*Shell*
- `-type f`: trattino sull'opzione, non sull'argomento.
- `paths must precede expression` → `find PERCORSO -opzione ARGOMENTO`, `'*.h'` subito dopo `-name`.
- `find .` scende anche in `build/`: dare la cartella precisa (seconda volta).
- Prima di `nano` guardare la cartella del prompt: file del ripasso creati dentro control-core (seconda volta).

*Git*
- `git blame` = ultimo commit di ogni riga che c'è oggi; `git log -S 'testo'` = commit che aggiungono o tolgono il testo, anche se oggi non c'è più.
- `-L 24,24` per una riga; con un solo numero va fino in fondo al file.

**Consolidati** (prima volta giusta senza indizi, se ripetuto escono): `h * f(x)`, stato aggiornato nel ciclo, `HEAD@{1}` dopo un reset.

**Rimandato**
- learncpp 22.1–22.5 + `~/cpp-lab/unique-19`: ven 20/11.
- Include guard in `rk4.h`: `fix(core): add include guard to rk4.h`.

## 2026-11-20 (ven) · S4

**Fatto**
- Ripasso a memoria: `lambda20.cpp` (2 errori corretti), previsione `A(1)` / `A(0, 1)` / `A(1, 0)` = `3 2 3`, `echo "test: $(find ./tests -type f -name '*.cpp' | wc -l)"` = 5, `~/bin/conta-cpp.sh` (stderr + `exit 2`, verificato con `2>/dev/null`), blame vs log -S, reset vs revert.
- learncpp 22.1–22.5 (smart pointer, r-value reference, move, `std::move`, `std::unique_ptr`) + `~/cpp-lab/unique-20`: x(5) = 1.99991, `p1` nullo dopo il move, `p2 = p1` → `use of deleted function`.
- Pro Git 7.2 Interactive Staging + `~/git-lab/patch-20`: due commit separati con `git add -p`.
- `fix(core)`: include guard in `rk4.h` e `cart_pole.h`, più `#include <Eigen/Dense>` e `<cmath>` in `cart_pole.h`. PR mergiata.
- DARE su carta: dimensioni (K 1×4, S 1×1), autovalori di A_d = e^{λTs}: 1, 1, 1.0974, 0.9113.
- `tools/ref_dare.py` (scipy `solve_discrete_are`): cart-pole, Ts = 0.02, Q = diag(1, 1, 10, 1), R = 0.1 → diag(P) = [90.973, 86.567, 1280.164, 55.355], K = [-2.7194, -4.9479, -41.6098, -9.3753].
- `dlqr<Nx,Nu>` in `include/control_core/dlqr.h` (iterazione di Riccati, `ldlt().solve`) + 3 test: `MatchesScipyDare` (P tol 1e-8, K tol 1e-10), `SatisfiesDare` (residuo < 1e-8), `ClosedLoopConverges` (‖x‖ < 1e-9 dopo 1000 passi). 17/17 ctest, PR mergiata.
- Fallimento apposta con R = 1: falliscono `MatchesScipyDare` e `ClosedLoopConverges` (‖x‖ = 7.6e-8, max |λ| = 0.984); `SatisfiesDare` passa.

**Da rivedere**

*C++ / Eigen*
- `no match for call to (…lambda(TIPI ATTESI)…) (TIPI PASSATI)`: confrontare le due liste una posizione alla volta (`u` `<2,1>` invece di `<1,1>`, seconda volta).
- Risultato sbagliato: guardare una componente alla volta e stampare i suoi ingressi prima di toccare la formula.
- `p1` = puntatore, `*p1` = oggetto, `p1->metodo()` = metodo dell'oggetto. Una funzione che vuole `Tipo&` riceve `*p1`.
- `std::move` è solo un cast: lo spostamento lo fa chi riceve (costruttore o `operator=` con `T&&`). Dopo il move un `unique_ptr` è `nullptr`.
- I commenti dello scheletro sono matematica: `ᵀ` → `.transpose()`, spazio tra fattori → `*`.
- P e K sono parametri `&`: si assegnano, non si ridichiarano (shadowing, seconda volta).
- K = (R + BdᵀPBd)⁻¹ BdᵀPAd con il P finale in entrambi i fattori: ricalcolare S.
- Continue e discrete in variabili diverse: `A`/`B` locali, `Ad = ss.Ad();` / `Bd = ss.Bd();` nei parametri.
- In un'espressione come A⁻¹·b non si calcola l'inversa: `S.ldlt().solve(b)` (`ldlt` per matrici simmetriche definite positive).
- `(Pn - P).cwiseAbs().maxCoeff()` = massimo |·| senza cicli; il risultato è un `double`.
- Vettore riga `(1, N)`: il primo indice è sempre 0 (`K(0, j)`, non `K(j, j)`).

*Test*
- Il riferimento viene da uno strumento indipendente (scipy), mai dall'output del codice testato.
- `ASSERT_…` per le condizioni senza cui il test non ha senso, `EXPECT_…` per i valori.
- Ogni `TEST` è una funzione separata: quello che scrivo in uno non vale negli altri.
- Test di coerenza (residuo ≈ 0) = errori nell'algoritmo; test contro un riferimento = anche errori nei dati. Servono tutti e due.

*Git*
- `blame` = righe che ci sono oggi; `log -S 'testo'` = commit che aggiungono o tolgono il testo (seconda volta).
- Commit solo locale → `reset`; commit già pushato su `main` → `revert`.
- `git diff` = cartella vs staging; `git diff --staged` = staging vs HEAD.
- Dopo `git add -p`, sempre `git diff --staged` prima del commit: se è vuoto o contiene troppo, non committare.

*Shell*
- `echo "$(cmd)"` = `cmd`: `echo` serve solo per aggiungere testo.

*Matematica*
- Stabilità discreta: |λ| < 1. Se A ha autovalore λ, A_d = e^{A·Ts} ha e^{λ·Ts}.

**Consolidati**: `A(j)` indice lineare per colonne, `find` con la cartella precisa, `-type f`.

**Rimandato**
- Ripianificazione di S4: Kalman, LESO, optional/variant, Python/pybind11.

## 2026-11-21 (sab) · S4

Riposo. Il piano del 21 è stato fatto il 22.

## 2026-11-22 (dom) · S4

Recupero del piano di sabato.

**Fatto**
- Ripasso a memoria: `ripasso21.cpp` (3 errori, 2 con indizio) → stampa `3`; `unique21.cpp` (previsione `1 5` / `7`); `find ~/control-core/include/control_core -type f -name "*.h" | wc -l` = 5; `wc -l` con e senza `\n` finale (2 e 1); `git diff` vs `git diff --staged` in `~/git-lab/diff-21`; `git log -S '#include <cmath>'` → `93b6084` (con 2 indizi).
- LQR sul cart-pole non lineare (`tests/test_dlqr.cpp`): `StabilizesNonlinearCartPole` (θ₀ = 0.1, 1000 passi Ts = 0.02, 10 sotto-passi RK4 h = 0.002, u₀ = 4.160979870, ‖x(20 s)‖ < 1e-9) e `NonlinearCloseToLinearForSmallAngle` (5 s, 1e-5 < max|θ − θ_lin| < 1e-3; NumPy: 2.19e-4). 19/19, PR mergiata.
- Fallimento apposta con θ₀ = 1.2: u₀ = 49.9318 (= 41.6098 · 1.2), `x.norm()` = `nan`, perché il K lineare non regge l'angolo grande.
- Pro Git 3.5 Remote Branches + `~/git-lab/remote-22` (server bare + due cloni): `push -u`, `fetch`, `branch -r`, `branch -vv` con `ahead 1`, `push origin --delete`, `fetch --prune`.
- Kalman su carta: caso scalare A = C = Q = R = 1 → a regime P⁻² − P⁻ − 1 = 0, P⁻ = φ, P⁺ = L = 1/φ = 0.6180339887; un passo da x₀ = 0, P₀ = 1, y = 3 → P⁻ = 2, L = 2/3, x⁺ = 2, P⁺ = 2/3.
- `KalmanFilter<Nx,Nu,Ny>` in `include/control_core/kalman.h` (predict/update, `S.ldlt().solve`) + `tests/test_kalman.cpp`: `OneStepByHand` (tol 1e-15), `ScalarConvergesToGoldenRatio` (100 passi, tol 1e-12). 21/21.
- Fallimento apposta con `- m_Rn`: x = 6, P = −2, L = 2; nel secondo test P va in un ciclo di periodo 3 e non converge.

**Da rivedere**

*C++ / Eigen*
- Un parametro `&` si assegna, non si ridichiara: `Tipo K{};` dentro la funzione = shadowing, errore (terza volta).
- `auto g = [](…) -> … { … };` è corretto: l'`=` non è un errore.
- Tipi attesi vs passati nella lambda: `u` `<2,1>` invece di `<1,1>` (terza volta).
- Dopo `std::move(p1)`, `p1 == nullptr` è vero → stampa `1` (seconda volta).
- `expected unqualified-id before 'for'` → guardare la riga sopra: `,` invece di `;` alla fine della dichiarazione.
- Parametri della lambda solo letti: `const Tipo&`, altrimenti non si legano ai temporanei come `x + h*k1/2`.
- Tutto quello che sta nella libreria ha il prefisso `control_core::` (`rk4Step`).
- Copiando un ciclo si cambiano subito il nome del contatore e i limiti (`k`/1000 → `i`/10).
- `EXPECT_…(…)` finisce con `;`.
- Membri con il nome dello stato, non del valore iniziale: `m_x{ x0 }`, `m_P{ P0 }`, non `m_x0`.
- Copia-incolla nella lista di inizializzazione: cambiare anche il nome del membro (`m_Bd` ripetuto).
- `ᵀ` → `.transpose()`: `m_Ad*m_P*m_Ad.transpose()`. Con matrici quadrate l'errore compila lo stesso (terza volta).
- Il C++ distingue maiuscole e minuscole: `S`, non `s`.
- Riportare tutti i fattori della formula: `(I − L C) P`, il `P` finale mancava.
- `update(u)` invece di `update(y)`: con tipi uguali (`<1,1>`) lo scambio compila, e lo trova solo un test con valori diversi da zero.
- Un membro calcolato dentro la classe (`m_L`) si inizializza nella lista con `::Zero()`, perché non è un parametro.

*Shell*
- (nessun errore)

*Git*
- `git log -g` = reflog; `git log -S 'testo'` = commit che aggiungono o tolgono il testo (terza volta, con indizi).
- `-u` = push + imposta l'upstream; il branch remoto può non esistere prima.
- `git add` vuole un file cambiato, non la cartella del repo.
- Prima del commit guardare il branch in `git status`: commit fatto su `main` invece che su `feat/x`, riparato con `switch -c` / `switch main` / `reset --hard origin/main` / `switch`.
- Verso il server `origin NOME` con lo spazio; `origin/NOME` è la foto locale (`log`, `reset`).
- `origin/main` si sposta con `fetch`, `pull` e `push`, mai con un commit.
- `branch -vv`: ahead/behind rispetto all'ultimo fetch.

**Consolidati**: `K(0, j)` su vettore riga, `find` con la cartella precisa + `wc -l`, `wc -l` conta i `\n`, `diff` vs `diff --staged`, `push origin --delete`, `fetch --prune`.

**Rimandato**
- Tag di Git (già studiati il 29–30/10): nel ripasso di lunedì 23/11.
- Kalman sul cart-pole contro scipy (DARE duale), LESO, optional/variant, Python/pybind11: piano di S5, lunedì 23/11.

## 2026-11-23 (lun) · S5

**Fatto**
- Ripasso a memoria: `cov23.cpp` (shadowing, `;` mancante, `A` invece di `Aᵀ`, tutti e 3 senza indizi) → `6 3` / `3 3`; `move23.cpp` (previsione `1 0` / `8` giusta); `echo "TEST: $(grep -r 'TEST(' ~/control-core/tests | wc -l)"` = 21; `~/bin/ntest.sh` (stderr + `exit 2`, `exit 1` se zero file); tag annotato `v0.1.0` in `~/git-lab/tag-23` (prima leggero, rifatto); `git log -S 'ldlt().solve' --oneline -- include/control_core/dlqr.h` → `c34db3a`.
- DARE duale su carta: a = Adᵀ, b = Cᵀ, q = Qn, r = Rn → X = P⁻; L = P⁻Cᵀ(CP⁻Cᵀ + Rn)⁻¹, P⁺ = (I − LC)P⁻; `dlqr(Adᵀ, Cᵀ, Qn, Rn)` dà Kᵀ = Ad·L (guadagno del predittore).
- `tools/ref_kalman.py`: C misura x e θ, Qn = 1e-4·I, Rn = 1e-3·I → L(0,0) = 0.2842066473, L(3,1) = 0.8120211506, diag(P⁺) = [2.842e-4, 5.320e-3, 3.104e-4, 1.077e-2].
- Fallimento apposta in Python con `a = Ad`: `LinAlgError: Failed to find a finite solution` (modo con λ_d = 1 non stabilizzabile da Cᵀ, rank 3 invece di 4).
- `Kalman.CartPoleMatchesScipyDualDare` (1000 passi, y = u = 0, L tol 1e-10, diag P⁺ tol 1e-12): con (I − LC)P fallisce, L(0,0) = 1 e P fino a 2.6e26.
- Forma di Joseph in `kalman.h`: P⁺ = (I − LC)P⁻(I − LC)ᵀ + L Rn Lᵀ → 22/22, test scalari compresi.

**Da rivedere**

*C++ / Eigen*
- `std::cout` stampa un `bool` come `1` (vero) / `0` (falso); `std::boolalpha` per `true`/`false`.
- Il `return` di `main` è lo stato di uscita: `0` = successo, come `exit 0` e `$?`. Non è un `bool`.
- Dopo `std::move(p1)` la proprietà passa a `p2`: `p1 == nullptr` è vero (terza volta, spiegazione invertita).
- `&` va nei parametri, non nelle variabili locali: `declared as reference but not initialized`.
- Errore Eigen lungo: cercare `required from here` (riga del mio codice) e `Dst`/`Src` (le due dimensioni). `YOU_MIXED_MATRICES_OF_DIFFERENT_SIZES` = dimensioni diverse.
- Copiando una riga (Qn → Rn) si cambia subito anche la dimensione (`Identity()` 4×4 su Rn 2×2).
- `P = A*P*A.transpose()` funziona (il prodotto usa un temporaneo); `P = P.transpose()` no → `P.transposeInPlace()`.
- Dopo una modifica: salvare, `cmake --build build`, poi `ctest`. Output identico alla cifra = codice vecchio.

*Python*
- D ha le righe di C e le colonne di B (`(2, 1)` con C 2×4).
- Ogni variabile va definita: identità = `np.eye(n)`.
- `f"{x:.17f}"` vale per un numero; per gli array `np.set_printoptions(precision=17)`.

*Shell*
- `[ "$num" -eq 0 ]`: `$` per leggere la variabile e operatore separato da spazi. `[ num=0 ]` è sempre vero (terza volta).
- In `echo` le variabili, non testo fisso: `"$1: $num"`.
- Dentro le virgolette ogni spazio viene stampato (`tests : 7`, `21 `).

*Git*
- `git tag NOME` crea un tag leggero; per una release `git tag -a NOME HASH -m "…"`. `git show` deve mostrare `Tagger:`.
- Tag già pushato da correggere: `git tag -d NOME` + `git push origin --delete NOME`, poi ricrearlo.
- Sul server `git tag` non distingue leggero da annotato: controllare con `git show`.
- `git log -S 'testo'` vuole il testo esatto; una riga per commit = `--oneline`; dopo `--` uno spazio e il file relativo al repo (quarta volta).

*Matematica*
- (Xᵀ)ᵀ = X: trasponendo un prodotto, un fattore già trasposto torna normale.
- DARE duale: a = Adᵀ, b = Cᵀ. Il DARE dà P⁻; `gain()` usa P⁻, `covariance()` dopo `update()` è P⁺.
- P e L non dipendono da y e u: nel test si usa y = u = 0.
- (I − LC)P è simmetrica solo con L esatto; gli arrotondamenti rompono la simmetria e il modo instabile (|λ| = 1.0974) la amplifica. Joseph = X P Xᵀ + L Rn Lᵀ è simmetrica e semidefinita positiva per qualsiasi L. In 1×1 il problema non si vede.

**Consolidati**: shadowing del parametro `&` (corretto senza indizi), `grep -r … | wc -l` dentro `$(…)`, `exit 2` su stderr, `push origin --delete`.

**Piano S5 rivisto** (si tiene tutto)

| Giorno | Contenuto |
|---|---|
| Mar 24 | LESO + learncpp 12.15 `std::optional` |
| Mer 25 | learncpp 25.1–25.4, 25.7 + interfaccia `Controller` (virtual vs template) |
| Gio 26 | Python: NumPy for MATLAB users, dataclasses, typing, pytest, pyproject → `tools/` come pacchetto |
| Ven 27 | pybind11: binding di `dlqr` + pytest contro `solve_discrete_are` |
| Sab 28 | MPC condensato in NumPy + OSQP Python come riferimento |
| Lun 30 + mar 1/12 | MPC C++ su OSQP + benchmark, CI (sanitizer, clang-tidy, clang-format), release v0.1.0 |
| Mer 2/12 | Inizio P2 (ROS 2); recupero sab 5/12 |
| S12 | iLQR/DDP in NumPy come primo task, prima di Nav2 (prima se in anticipo) |

**Rimandato**
- LESO e `std::optional`: mar 24/11.

## 2026-11-24 (mar) · S5

Giornata ridotta (colloquio).

**Fatto**
- Ripasso a memoria: `move24.cpp` (previsione `1 false` / `8` giusta); `fix24.cpp` (riferimento `&` su un'espressione, `Identity()` 3×3 su una matrice 2×2, `;` mancante; il `const` del parametro segnalato per sbaglio) → `1 4` / `4 5`; `~/bin/contatest.sh` (2 errori: `grep` senza file, `-l`) → `test_kalman.cpp: 3 test`, `exit 1`, `exit 2`; `echo "header: $(find … -name '*.h' | wc -l)"` = 6; `git log -S 'IKC' --oneline` → `b5c8c45` (senza `-- percorso`); `~/git-lab/tag-23`: `v0.1.0` non esisteva, `ok` = tag leggero, `v0.1.0` annotato ricreato su `HEAD`.
- learncpp 12.15 `std::optional` + `~/cpp-lab/optional-24` (3 errori: risultato scartato, `'\n'` mancante, contatore da 0) → `3`, `nessuna radice`, `44`, `-1`, `-1`.
- LESO su carta (indizi sui passi 3 e 4): Φ e Γ esatti (A³ = 0), e⁺ = (Φ − LC)e, l₁ dal coefficiente di λ², l₂ e l₃ da somma e differenza delle altre due equazioni, limite T → 0 = T·[3ω₀, 3ω₀², ω₀³].
- `tools/ref_leso.py` (Ackermann): β = 0.951229424500714, L = [0.14631172649785792, 7.07770501325522, 116.00418067888495], differenza con la formula chiusa 1.42e-14.
- `Leso` in `include/control_core/leso.h` (3 errori: `beta` e `C` non dichiarate, `const` prima del tipo invece che dopo le parentesi) → `3712f90` su `main`.
- `tests/test_leso.cpp`: `DiscretizationMatchesStateSpace` (1e-12), `GainMatchesAckermann` (1e-10), `ErrorMatrixHasTriplePole` (< 1e-12). 25/25.
- Branch `feat/leso` "out-of-date" dopo Rebase and merge: nuovo branch `feat/leso-tests` da `main` + `cherry-pick` del commit dei test (`25ff70a`), PR #31 chiusa, `feat/leso` cancellato in locale e sul server.

**Concetti chiave**
- LESO: ẍ = d + b₀u, z = [x, ẋ, d]. Φ = [1 T T²/2; 0 1 T; 0 0 1], Γ = [T²/2; T; 0]. Predittore: ẑ⁺ = Φẑ + Γb₀u + L(y − Cẑ); con y = Cz l'errore è e⁺ = (Φ − LC)e, senza u.
- Polo triplo in β = e^{−ω₀T}: l₁ = 3(1 − β), l₂ = (1 − β)²(5 + β)/(2T), l₃ = (1 − β)³/T². Per T → 0: L ≈ T·L_c, con L_c = [3ω₀, 3ω₀², ω₀³].
- Ackermann (osservatore): L = φ(Φ) O⁻¹ [0 0 1]ᵀ, con O = [C; CΦ; CΦ²]. Polo triplo verificato con (Φ − LC − βI)³ = 0, non con `eigenvalues()`.
- `std::optional<T>`: `return std::nullopt;` (o `return {};`) = nessun valore, `has_value()`, `*r`, `value_or(x)`. Equivale a `[]` + `isempty` in MATLAB.

**Da rivedere**

*C++ / Eigen*
- Una funzione che restituisce un valore non modifica i suoi argomenti: `f(x);` butta via il risultato, serve `Tipo r{ f(x) };` (come `r = f(x)` in MATLAB).
- Contatore "k-esimo passo" (1..N): `for (int k{ 1 }; k <= N; ++k)`. Con `k{ 0 }; k < N` i giri sono N lo stesso, ma `k` vale uno in meno (43 invece di 44).
- Variabile nuova = tipo + nome (`double beta{ … };`, `Eigen::Matrix<double, 1, 3> C;` + `<<`); senza tipo è un'assegnazione a qualcosa che non esiste.
- Metodo che non modifica l'oggetto: `Tipo nome() const { … }`, con `const` DOPO le parentesi. `const Tipo nome()` rende const solo il valore restituito.
- Oggetto con nome: `control_core::Leso leso(50.0, 2.0, 0.001);`. Senza nome è un temporaneo distrutto a fine riga.
- `const Tipo&` nel parametro è una promessa della funzione, non un requisito per chi chiama: un argomento non `const` si passa sempre a un `const&`; il contrario no.
- Un riferimento non `const` non si lega a un'espressione come `A + A.transpose()`; `&` va nei parametri, non nelle variabili locali (seconda volta).
- `YOU_MIXED_MATRICES_OF_DIFFERENT_SIZES` → `required from here` e le due dimensioni (seconda volta).
- `;` mancante: l'errore compare sulla riga dopo.
- Dopo `std::move(p1)`, `p1 == nullptr` è vero; `std::boolalpha` resta attivo su `std::cout` fino a `std::noboolalpha`, anche nelle istruzioni successive.
- `*r` su un `optional` vuoto è comportamento indefinito: prima `has_value()` (nel test `ASSERT_TRUE`, poi `EXPECT_EQ(*r, …)`).

*Test*
- Il riferimento del test è `L` di Ackermann, non `L_formula`: mai dalla formula che si sta testando.
- Le ultime cifre di un riferimento possono cambiare con l'ordine delle operazioni: la tolleranza (1e-10) ne tiene conto.

*Shell*
- `grep PATTERN` senza file legge da stdin e resta in attesa: il file va dopo il pattern (`"$1"`).
- `grep -l` = nomi dei file che contengono il pattern; senza `-l` = le righe. Per contare: `grep … | wc -l`.
- `[ "$N" -eq 0 ]`: virgolette anche se N è sempre un numero.

*Git*
- Dopo "Rebase and merge" il branch è chiuso: GitHub riscrive i commit con hash nuovi, quindi altri commit sullo stesso branch lo rendono "out-of-date". Lavoro nuovo = branch nuovo da `main` aggiornato. Se è già successo: branch nuovo + `cherry-pick`, chiudere la PR vecchia, `git branch -D` sul branch vecchio (`-D` perché Git non lo vede mergiato).
- Non cliccare "Update branch" (merge commit nel branch).
- `fatal: ambiguous argument 'NOME': unknown revision` = NOME non esiste: prima `git tag` / `git branch` per vedere i nomi veri. Non inventare opzioni (`-lw`).
- `git tag -n` non distingue leggero da annotato: `git show NOME` e la riga `Tagger:`. Senza hash, `git tag -a` va su `HEAD`.
- `git log -S 'testo' --oneline -- percorso/relativo`: il `-- percorso` limita al file (quinta volta).

**Consolidati**: `grep … | wc -l` dentro `$(…)`, `find` con la cartella precisa + `-type f`, `exit 1` / `exit 2` su stderr, previsione `std::move` + `nullptr`.

**Rimandato**
- Test `Leso.EstimatesConstantDisturbance` e `Leso.FirstStepWithinTolerance` (`firstStepWithin` con `std::optional`, attesi 252 e nessun valore entro 200) + fallimento apposta con `/T` in l₂: mer 25/11.
- Pro Git 7.1 Commit Ranges + `~/git-lab/range-24`: mer 25/11.

**Domani**: test 4–5 del LESO, poi learncpp 25.1–25.4, 25.7 e interfaccia `Controller` (virtual vs template).

## 2026-11-25 (mer) · S5

**Fatto**
- Ripasso a memoria: `fix25.cpp` (`sum` senza tipo, `const int value()`, risultato di `doubled` scartato trovati da solo; contatore 1..N con indizio) → `13 20`; `pred25.cpp` (previsione `true 3` / `false -1` giusta); `~/bin/contaexpect.sh` (argomenti di `grep` invertiti) → `tests/test_leso.cpp: 10 EXPECT`, `exit 1`, `exit 2`; `echo "file con matrici 3x: $(grep -r -l … ~/control-core/tests | wc -l)"` = 1 (prima con `find | grep -r`: 20); `git log -S 'std::exp(-omega0*Ts)' --oneline -- include/control_core/leso.h` → `3712f90`; `~/git-lab/amend-25`: `reset --hard HEAD@{1}` → `4d146cc` "B originale".
- learncpp 24.1–24.5, 24.7 (ereditarietà) + 25.1–25.2 + `~/cpp-lab/virtual-25`: senza `virtual` → `5 8 8`, con `virtual` → `5 8 5`.
- Pro Git 7.1 Commit Ranges + `~/git-lab/range-25`: `main..feat` = D C, `feat..main` = E, `--left-right main...feat` = `< E`, `> D`, `> C`, `^main feat` = `feat --not main`.
- `tools/ref_leso.py` con d = −3, u = 0.5: primo passo con |d̂ − d| < 1e-3 = 252, max|ẑ − z| a 1000 passi = 3.44e-13; con 200 passi `None` (errore 8.48e-3).
- `Leso.EstimatesConstantDisturbance` (1000 passi, tol 1e-9) e `Leso.FirstStepWithinTolerance` (`firstStepWithin` con `std::optional`: 252 con 1000 passi, nessun valore con 200). 27/27, `9b83cfb` su `main`.
- Fallimento apposta con `(2*Ts*Ts)` in l₂: falliscono 4 test (l₂ = 7077.7, `(N³)` = 5e4, stime `nan`, `r1` vuoto).

**Concetti chiave**
- Derivata: `class D : public B`, costruttore `D(…) : B{ … }, m_x{ … } {}`. Si costruisce prima la base, poi la derivata; si distrugge al contrario. I `private` della base non si vedono: si usano i getter (`k()`, senza oggetto davanti dentro un metodo).
- Senza `virtual` decide il tipo del riferimento (statico, in compilazione); con `virtual` decide il tipo dell'oggetto (dinamico, a run time). La firma è la stessa in base e derivata.
- LESO con d costante: e⁺ = (Φ − LC)e, polo triplo in β = e^{−ω₀T} = 0.9512; |d̂ − d| < 1e-3 al passo 252 (≈ 12.6/ω₀). Con l₂ sbagliato |λ(Φ − LC)| = 2.816 > 1 → instabile.
- Range: `A..B` = raggiungibili da B ma non da A; `A...B` = da uno solo dei due. `origin/main..HEAD` = cosa entra in `main` con la PR; `main..origin/main` dopo `fetch` = cosa porta `pull`.

**Da rivedere**

*C++*
- Costruttore della base nella lista di inizializzazione: `: Gain{ k }`, poi i membri della derivata.
- Funzione non-`void`: ogni percorso finisce con `return` (`control reaches end of non-void function`).
- `;` mancante: l'errore compare sulla riga dopo (`expected ',' or ';' before …`) (seconda volta).
- Ogni `TEST` dichiara le sue costanti: `d` di `firstStepWithin` non esiste nel test.
- Copiando una riga si cambia subito il nome (`r1` → `r2`) (terza volta).
- `double` ed `Eigen::Matrix<double, 1, 1>` sono tipi diversi: `u` per `leso.step`, `uv` per `plant.step`.
- Stato esteso z = [x, ẋ, d]: d è `z(2)`. Con `x0(1) = d` il test passava, ma verificava la cosa sbagliata.
- Contatore 1..N: `k{ 1 }; k <= N`; cambiando l'inizio si cambia anche la condizione (terza volta).
- `std::optional` vuoto = `std::nullopt`; "nullo" (`nullptr`) si dice dei puntatori.

*Test*
- Un test verde va controllato leggendo i valori: `./build/test_X --gtest_filter=…` mostra lo `std::cout` che `ctest` nasconde.

*Python*
- `@` = prodotto matriciale (`*` di MATLAB); con gli scalari `*` (`.*` di MATLAB).

*Shell*
- `./` = cartella corrente, `~/` = home, solo all'inizio del percorso. Lavorare sotto `~`, non in `/mnt/c`.
- `grep PATTERN FILE`: prima il pattern, poi il file (terza volta).
- `grep -r PATTERN CARTELLA` cerca nel contenuto; `find … | grep` cerca nei nomi. `grep -r` senza cartella cerca in `.` (anche `build/`).

*Git*
- `git log -S 'testo' --oneline -- percorso` (sesta volta; giusto al secondo tentativo).
- `git reflog` senza argomenti per la lista completa; i numeri `HEAD@{n}` si spostano a ogni operazione.

**Consolidati**: `grep -l` + `wc -l` dentro `$(…)`, `exit 1` / `exit 2`, `reset` dal reflog.

**Rimandato**
- learncpp 25.3, 25.4, 25.7 e interfaccia `Controller` (virtual vs template): da ricollocare.

**Domani**: Python oltre la base (NumPy for MATLAB users, dataclasses, typing, pytest, pyproject) → `tools/` come pacchetto con pytest.

## 2026-11-26 (gio) · S5

**Fatto**
- Ripasso a memoria: `fix26.cpp` (`;` dopo `m_lim{}` trovato da solo; costruttore della base `: Gain{ k }` e `return y;` con indizi) → `3 5 -5`; `pred26.cpp` previsto `6 5 -1` invece di `3 5 -1` (risultato di `doubled(a)` scartato), con `k{ 0 }; k < N` → `4` giusto; `grep 'TEST(' … | wc -l` = 5; `grep -r -l 'std::optional' … | wc -l` = 1 (senza `-l`: 3 righe); `~/control-core/build/test_leso` lanciato da `/tmp` (con indizi); `git log -S 'ldlt' --oneline -- include/control_core/dlqr.h` → `c34db3a` al primo colpo; `git log --oneline HEAD~3..HEAD` (dato da Claude).
- NumPy for MATLAB users + `~/py-lab/np-26/np26.py`: 6 traduzioni giuste dopo correzioni (`solve` con indizi, stampe dopo la vista, `np.zeros((3, 3))`, assegnazione del blocco `M[0:2, 1:3] = …`).
- Python tutorial 9.3, `dataclasses`, cheat sheet di mypy + `dc26.py`: `__repr__`, `s.A[0, 0] = 5.0` permesso con `frozen`, `Ts="ciao"` accettato, `FrozenInstanceError` su `s.Ts = 1.0`.
- pytest Get Started + `assert_allclose` + `test_np26.py` (2 passed, dopo aver corretto A nei dati del test).
- Pro Git 2.2 Ignoring Files / Removing Files + `~/git-lab/ignore-26`: `.gitignore`, `git rm --cached` (`-r` per le cartelle), commit sistemato con `--amend` → `d9080b0`, `git ls-files` = `.gitignore`, `a.py`.
- Pacchetto `ccref`: `pyproject.toml` (setuptools, `where = ["python"]`), venv `.venv`, `pip install -e '.[test]'` (pytest 9.2, NumPy 2.5), `.gitignore` aggiornato prima del primo `git add`.
- `LinearSystem` (dataclass frozen, controllo delle dimensioni in `__post_init__`), `cart_pole_upright`, `zoh` (Van Loan con `expm`, giusto al primo colpo), `dlqr`, `leso_gain` + `leso_gain_ackermann`. `env -u PYTHONPATH pytest -v`: 9 passed (zoh vs `cont2discrete` 1e-12, K vs test C++ 1e-10, residuo DARE < 1e-9, |λ| max < 1, LESO vs Ackermann 1e-10, limite T → 0 con rtol 1e-4).

**Concetti chiave**
- NumPy ↔ MATLAB: indici da 0, nelle slice l'inizio scende di 1 e la fine resta uguale (`1:2` → `0:2`); `@` = `*` di MATLAB, `*` = `.*`; `np.linalg.solve(A, b)` = `A\b`; elemento = `K[i, j]`. Una slice è una vista (`.copy()` per una copia); a sinistra dell'`=` scrive, a destra legge.
- `@dataclass` genera `__init__`, `__repr__`, `__eq__`; `__post_init__` gira dopo `__init__`. `frozen=True` vieta di riassegnare i campi, non di modificare il contenuto di un array. I tipi annotati non vengono controllati a run time.
- Van Loan: e^{[A B; 0 0]·Ts} = [Ad Bd; 0 I]. DARE: P = Q + AᵀPA − AᵀPB(R + BᵀPB)⁻¹BᵀPA, K = (R + BᵀPB)⁻¹BᵀPA. LESO per T → 0: L/T → [3ω₀, 3ω₀², ω₀³].
- `.gitignore` vale solo per i file non tracciati: un file già tracciato si toglie con `git rm --cached` (resta sul disco). `dir/` = solo cartelle, `/x` = solo nella radice.
- venv: su Ubuntu il Python di sistema è di apt (PEP 668), quindi `pip` solo nel venv. `source .venv/bin/activate` perché modifica la shell corrente. Con ROS attivo, `PYTHONPATH` fa caricare a pytest i plugin di ROS → `env -u PYTHONPATH pytest`.

**Da rivedere**

*Python*
- Indentazione: 4 spazi, mai tab (`TabError`); il corpo di `def`/`if` va più a destra della riga sopra. `~/.nanorc` con `tabstospaces`.
- `if …:` con i due punti; una funzione senza `return` restituisce `None`.
- Ogni funzione e ogni test usa solo le sue variabili (`Ad_ref`, `Bd`, `beta` non definiti) — copia-incolla dei nomi (quarta volta).
- I test vengono raccolti solo se il nome inizia con `test_`: controllare `collected N items`.
- `np.exp` per gli scalari, `scipy.linalg.expm` per le matrici.
- Residuo = lato destro − lato sinistro: segni come nella formula e `- P` finale.
- `K[0, j]`, non `K[[0][0]]`; meglio confrontare la matrice intera con `assert_allclose(K, K_ref, rtol=0, atol=…)`.
- `assert_allclose`: tolleranza = atol + rtol·|desired|; con i default, vicino a 0 vale 0 → sempre `rtol=0, atol=…`.
- `np.zeros((3, 3))` per una matrice; numeri decimali (`1.0`) per array `float`. Il meno copiato da un testo deve essere `-` (ASCII).
- `@dataclass` va importato: `from dataclasses import dataclass`.

*C++*
- Costruttore della base nella lista di inizializzazione: `: Gain{ k }, m_lim{ lim }`; `virtual` riguarda le chiamate, non la costruzione.
- Funzione non-`void`: ogni percorso finisce con `return` (seconda volta).
- `doubled(a);` butta via il risultato; `a` passato per valore non cambia (seconda volta).

*Shell*
- `grep -l` = nomi dei file; senza `-l` = righe.
- Un eseguibile si lancia con il suo percorso: `./nome` dalla cartella corrente, `~/control-core/build/test_leso` da altrove. `ls` guarda, `mv` sposta.

*Git*
- Range: `git log --oneline HEAD~3..HEAD` — il più recente a destra, tilde attaccata al nome, nessuna opzione.
- `git commit -m "…" FILE` committa solo FILE; senza percorso, tutto l'indice.
- `--amend` sostituisce l'ultimo commit (hash nuovo): solo se non pushato.

**Consolidati**: `grep PATTERN FILE | wc -l`, `log -S … --oneline -- percorso` (primo colpo, da confermare la prossima volta).

**Rimandato**
- Blocco 9: `tools/ref_dare.py` riscritto con `ccref` + `diff` con l'output vecchio: ven 27/11 mattina.
- learncpp 25.3, 25.4, 25.7 + interfaccia `Controller`: ven 27/11.

**Domani**: `ref_dare.py` con `ccref`, learncpp 25.3–25.7 + `Controller`, poi pybind11 (binding di `dlqr` testato con pytest contro `ccref.dlqr`).

## 2026-11-27 (ven) · S5

**Fatto**
- Ripasso a memoria: `fix27.cpp` (costruttore della base `: Offset{ c }`, `;` mancante, `return` finale trovati da solo; valore del `return` = `c()` con spiegazione) → `2.5 1 0`; `pred27.cpp` → `6 1 5 -1` (valori giusti, `true` invece di `1`; `r2` = optional vuoto, `std::nullopt`); `grep 'zoh(' … | wc -l` = 5, `grep -l 'import numpy' …/*.py | wc -l` = 4 (con `-r` superfluo); `~/control-core/build/test_dlqr` lanciato da `~/py-lab` al primo colpo (5 test); `git log HEAD~4..HEAD` (giusto, senza `--oneline`); `~/git-lab/amend-27`: `git add b.py` + `git commit --amend --no-edit` → `9706b7b` con 2 file (con indizi: prima pensato a editor e `reset`).
- learncpp 25.3, 25.4, 25.7 + 3 domande (corrette: meccanismo di `override`, motivo del distruttore virtuale, `Controller` per valore).
- `~/cpp-lab/controller-27`: interfaccia `Controller`, `PController`/`PIController` `final` con `override`, `runVirtual` e `runTemplate` → `0.666667 0.958434`, `1.01652` senza `reset()`, `0.958434` dopo `reset()`. Fallimenti apposta: tipo astratto (con `note` delle virtuali pure), `const` → `marked 'override', but does not override` + `PController` astratta a cascata, `cannot derive from 'final' base`.
- Pro Git 7.3 Cleaning your Working Directory + `~/git-lab/clean-27`: `-n` → `note.txt`; `-n -d` → `+ tmp/`; `-n -d -x` → `+ build/`, `mod.so`; `-f -d` → restano `a.py build mod.so`, `status` vuoto.
- Pulizia branch locali `gone` (verifica con `git log --oneline main | grep '<messaggio>'` → `cb8ceff`): cancellati con `-D` `docs/notes-22`, `feat/kalman`, `feat/leso-tests`, `test/lqr-nonlinear`. Branch `feat/pybind-dlqr` da `main`.
- `tools/ref_dare.py` con `ccref` (`zoh`, `cart_pole_upright`, `dlqr`): `diff` con l'output vecchio vuoto.
- pybind11 (Installing, First steps, Building with CMake, Eigen, Python types) + 3 domande (risposte date da Claude).
- Modulo `ccref._cc`: pybind11 3.1.0, Python 3.12.3 del venv; opzione `CONTROL_CORE_BUILD_PYTHON` (OFF di default, CI invariata), `.so` in `python/ccref/`, `*.so` in `.gitignore`. `dlqr_cart_pole(Ad, Bd, Q, R, max_iter=100000, tol=1e-12)` → `(ok, P, K)`; doc con forme `"[4, 4]"`, `"[4, 1]"`, `"[1, 1]"`.
- `tests_py/test_bind_dlqr.py`: `test_binding_matches_ccref` (K atol 1e-10 dopo `reshape(1, 4)`, P atol 1e-8), `test_wrong_shape_raises` (A 3×3 → `TypeError`), `test_not_converged` (`max_iter=10` → `ok` falso). `pytest`: 12 passed; `ctest`: 27/27.
- Fallimento apposta con `tol = 1e-2` di default: fallisce solo `test_binding_matches_ccref`, errore massimo su K = 4.1e-3 (relativo 4.9e-4).

**Concetti chiave**
- `override` = il compilatore verifica che la funzione sovrascriva una virtuale della base con la stessa firma (`const` compreso), altrimenti errore. Senza, nasce una funzione nuova in silenzio e tramite `Base&` parte quella della base. `final` sulla classe vieta di derivare, sul metodo vieta di sovrascrivere.
- Distruttore: `virtual ~Base() = default;` (o `{}`, non tutti e due). Senza `virtual`, `delete` tramite `Base*` / `unique_ptr<Base>` chiama solo `~Base()`: comportamento indefinito, la parte derivata non viene distrutta (leak dei suoi membri).
- Classe astratta = almeno una virtuale pura `= 0`; interface class = solo virtuali pure, nessun membro, distruttore virtuale. `Controller&` sì (si lega a un oggetto derivato esistente), `Controller` per valore no (non si istanzia).
- virtual = una sola funzione, `compute` scelta a run time (vtable). template = una copia per ogni tipo usato (qui 1: `runTemplate<PIController>`), scelta in compilazione.
- pybind11: il nome in `PYBIND11_MODULE(_cc, m)` genera `PyInit__cc` e deve coincidere con `pybind11_add_module(_cc …)`, che dà il nome al `.so`. NumPy → `const Eigen::Matrix<…, N, M>&` fissa: copia con controllo della forma (`TypeError`). I vettori Eigen tornano 1-D (`K.shape == (4,)`). Un template si espone solo istanziato (`dlqr<4, 1>`).
- `git clean`: `-n` prova a vuoto, `-f` cancella (obbligatorio), `-d` anche le cartelle non tracciate, `-x` anche gli ignorati. Tocca solo i file non tracciati: i file committati non vengono mai cancellati.
- `tol` sulla differenza tra due iterate di Riccati ≠ errore finale su K: con `tol = 1e-2`, K sbaglia di 4e-3.

**Da rivedere**

*C++*
- Funzione non-`void`: dopo una serie di `if` con `return` serve il `return` finale per il caso "nessun `if`" (quarta volta).
- Derivata: intestazione `class D final : public B`; ogni membro usato nel costruttore va dichiarato in `private` con `{}`.
- Più parametri = **un** costruttore `(kp, ki, Ts)` con `: m_kp{ kp }, m_ki{ ki }, m_Ts{ Ts }`; tre costruttori `(double)` hanno la stessa firma.
- Membro con valore iniziale fisso (`m_integral`): `double m_integral{};`, non va nel costruttore.
- Variabile locale: tipo + nome (`double e{ r - y };`).
- Una derivata che non fa override di tutte le virtuali pure resta astratta.
- `const` va dopo la lista dei parametri di un **metodo**; un costruttore non può essere `const` (`constructors may not be cv-qualified`).
- In una cascata di errori si corregge il **primo**.
- `'…'` = un carattere, `"…"` = stringa; un nome in un commento non è una variabile (`converged` → `ok`).
- Parametri della funzione al posto dei numeri scritti a mano (`maxIter`, `tol`), altrimenti `unused parameter`.
- `cout << bool` stampa `1`/`0`.

*pybind11 / CMake*
- `py::arg("Ad")`: il nome tra virgolette.
- `pybind11_add_module(_cc sorgente.cpp)`: prima il nome, poi i sorgenti; le parole dei commenti dello scheletro non vanno ricopiate.

*Python*
- `np.diag([1.0, 1.0, 10.0, 1.0])`: un solo array; `R = np.array([[0.1]])`, non una lista.
- Negazione: `not ok` (non `!`); `and`, `or`.
- `pytest.raises(TypeError)` nel `with`, la chiamata indentata dentro; non passare l'eccezione ad altre funzioni.
- Nomi uguali a quelli dichiarati (`K_ref`, non `Kref`): copia-incolla dei nomi (quinta volta).

*Shell*
- `grep -r` solo con una cartella; con un glob di file basta `grep -l`.

*Git*
- `--amend` prende l'**indice**: prima `git add`, poi `git commit --amend --no-edit` (tiene il messaggio). Solo se non è pushato.
- `git log --oneline` per una riga per commit.
- `git clean -d` = cartelle, `-x` = ignorati.
- Dopo ogni merge su GitHub: `git switch main`, `git pull`, `git branch -D <branch>`. Verifica con il messaggio: `git log --oneline main | grep '<messaggio>'`.

**Consolidati**: eseguibile lanciato con il percorso completo da un'altra cartella, `grep -l … | wc -l` dentro `$(…)`, sintassi del range `HEAD~4..HEAD`.

**Domani**: sabato, MPC lineare condensato in NumPy sul cart-pole + OSQP Python come riferimento.

## 2026-11-28 (sab) · S5

**Fatto**
- Ripasso a memoria: `fix28.cpp` (`const` mancante nella firma con `override`, `return u;` finale; trovati da solo) → `2 3 -3`; `pred28.cpp` previsione `2 10 20` / `1` giusta (spiegazione da correggere: riferimento, non puntatore; `f` virtuale segue l'oggetto); `echo "test Python: $(grep … | wc -l)"` = 12 (con `-r` superfluo); `grep -l 'np.linalg.solve' …/*.py | wc -l` = 2; `git log -S 'PYBIND11_MODULE' --oneline -- python/bindings/bind_dlqr.cpp` → `e395519` al primo colpo; `~/git-lab/amend-28`: `git add b.py` + `git commit --amend --no-edit` senza indizi (2 file, 1 commit); `git clean -n -d` (prima con un percorso per file).
- `main` aggiornato, `feat/pybind-dlqr` cancellato; branch `feat/mpc-ref`; `osqp` 1.1.3 nel venv e in `pyproject.toml`.
- Teoria: slide di Bemporad "Linear MPC" 36–41 (caso senza vincoli, forma condensata, QP) e 76–77 (MPC = LQR); OSQP Solver + interfaccia Python. 3 domande: 1 in parte giusta, 2 e 3 spiegate da Claude.
- `python/ccref/mpc.py`: `prediction_matrices`, `condensed_cost`, `mpc_unconstrained`, `mpc_box_osqp`.
- `tests_py/test_mpc.py`: predizione vs simulazione (1e-12), u₀ = −Kx₀ = 4.160979870360416 (1e-10), OSQP = senza vincoli con u_max = 10 (1e-6), u_max = 2: |U| ≤ 2 e u₀ = 2. 4 passed. Con u_max = 2 i primi 8 ingressi sono saturi, u₈ = 1.68797697.

**Concetti chiave**
- X = S_x x₀ + S_u U, S_x = [A; …; A^N], blocco (r, j) di S_u = A^{r−j}B per j ≤ r (0 sopra la diagonale). Q̄ = blkdiag(Q, …, Q, P), R̄ = blkdiag(R, …, R). H = 2(S_uᵀQ̄S_u + R̄), F = 2S_uᵀQ̄S_x, U* = −H⁻¹Fx₀.
- H dipende solo da A, B, Q, R, P, N ed è costante; nel ciclo cambia solo q = Fx(t): `setup` una volta, poi `update(q=…)` + `solve`. Warm start = OSQP parte dalla soluzione del passo prima. La `P` di OSQP è la nostra H, non il P della DARE.
- Con P = soluzione della DARE (x_NᵀPx_N = costo ottimo da N a ∞) la ricorsione di Riccati resta in P: u₀* = −Kx₀ per ogni N. x₀ᵀQx₀ è costante rispetto a U: cambia il valore del minimo, non il punto.
- OSQP: min ½zᵀPz + qᵀz, l ≤ Az ≤ u; P = solo triangolo superiore in CSC (`sparse.triu`); box su u con A = I. Precisione ~eps_abs/eps_rel, non di macchina: test con atol 1e-6. OSQP 1.x: `polishing` (non `polish`), `solve(raise_error=False)` + controllo di `res.info.status`.

**Da rivedere**

*Python / NumPy*
- Nel ciclo interno l'indice che cambia deve comparire nella slice: colonne di S_u con `j*nu:(j+1)*nu`, non `r` (altrimenti si scrive sempre il blocco diagonale).
- Funzioni di NumPy con il prefisso: `np.kron`, `np.linalg.matrix_power` (`NameError: name 'kron' is not defined`).
- Una funzione senza `return` restituisce `None` → `cannot unpack non-iterable NoneType object` (seconda volta; in C++: `return` finale, quinta volta).
- `np.diag([1.0, 1.0, 10.0, 1.0])`: decimali, altrimenti array di interi.
- Prima di lanciare pytest: `ls tests_py` per vedere che il file esiste (era stato chiuso senza salvarlo).
- Un test verde va controllato: il test deve contenere l'`assert`, non solo il commento.

*C++*
- Il `const` dopo le parentesi fa parte della firma: senza, `override` non trova la funzione.
- Tramite `Base&`: funzione virtuale → tipo dell'oggetto; non virtuale → tipo del riferimento (`b.g()` = 10, `d.g()` = 20). `b` è un riferimento, non un puntatore.

*Shell*
- `grep -r` solo con una cartella; con `*.py` basta `grep` / `grep -l` (seconda volta).
- Dentro le virgolette anche un a capo viene stampato.
- `python: command not found` = venv non attivo: `source .venv/bin/activate` in ogni terminale nuovo.

*Git*
- `git clean -n -d` senza percorsi considera tutti i file non tracciati; un percorso limita la pulizia.
- Dopo `git branch -D` il branch nuovo va creato: `git switch -c NOME`, poi `git branch` per verificare.

**Consolidati**: `log -S … --oneline -- percorso` (secondo primo colpo), `--amend` partendo dall'indice, `grep -l … | wc -l`.

**Rimandato**
- `simulate_mpc_box` (ciclo chiuso con `update(q=…)`, 250 passi, ‖x‖ ≈ 7.49e-3) + `test_closed_loop_box`: lun 30.
- Fallimento apposta senza P terminale (u₀ ≈ 1.977 invece di 4.161): lun 30.

**Domani**: lunedì 30 (3 h): ciclo chiuso e fallimento apposta, poi teoria MPC sparso vs condensato e API C di OSQP.
