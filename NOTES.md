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
