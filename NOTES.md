# NOTES

Diario del percorso di 8 settimane: cosa ho imparato, cosa blocca, regole da ricordare.

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
