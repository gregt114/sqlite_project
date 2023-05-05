
# sqlite Vulnerability Research

## Authors
- Max Sonderegger
- Ethan Ritchie
- Greg Turnberg

## Setup
- Run `setup.sh` to download, extract, and compile SQLite3 with debug symbols, AFL++ instrumentation, and ASAN
    - To fuzz this file, you should run `afl-fuzz -i in -o out -- ./sqlite3`
- `blob_harness.c` is the source code for a harness that accepts a file from the command line, creates a table, and reads the data from the file into BLOB fields in the created table. This is so we can target our fuzzing towards the BLOB serialization.
- `blob_harness` is the compiled binary containing the harness and all of SQLite. I precompiled it and uploaded it here since compilation can take a while. It includes debug symbols, ASAN, and AFL++ instrumentation. It can be recompiled with `afl-gcc-fast blob_harness.c -g -fsanitize=address -o blob_harness`
    - Note that you will likely need to change the `#include` statement in `blob_harness.c` so that it points to your `sqlite3.c` amalgamation file

## Todo
- Create better corpus (more examples, longer, more varied queries)
- Maybe make harness for executing queries; fuzzing the shell is pretty slow (~50 cases/sec)
- Some type of static analysis
    - Joern?
    - Source/sink analysis?
