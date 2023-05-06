
# sqlite Vulnerability Research

## Authors
- Max Sonderegger
- Ethan Ritchie
- Greg Turnberg

## Setup
- Run `setup.sh` to download, extract, and compile SQLite3 with debug symbols, AFL++ instrumentation, and ASAN
    - To fuzz this file, you can run `afl-fuzz -i in -o out -- ./sqlite3`, although it is recommended that `harness` is fuzzed instead since it is much faster
- `harness.c` is the source code for a harness that accepts a file from the command line and executes the SQL query in the file.
    - `harness` is the compiled binary of `harness.c`, which includes debug symbols, AFL++ instrumentation, and ASAN. It is included here since compiling it can take a while. It can be recompiled with `afl-gcc-fast harness.c -g -fsanitize=address -o harness`. A harness is used for fuzzing rather than the sqlite shell since the shell significantly slows down the fuzzing process.
- `blob_harness.c` is the source code for a harness that accepts a file from the command line, creates a table, and reads the data from the file into BLOB fields in the created table. This is so we can target our fuzzing towards the BLOB serialization.
    - `blob_harness` is the compiled binary of `blob_harness,c`. I precompiled it and uploaded it here since compilation can take a while. It includes debug symbols, ASAN, and AFL++ instrumentation. It can be recompiled with `afl-gcc-fast blob_harness.c -g -fsanitize=address -o blob_harness`
- Note that you will likely need to change the `#include` statements in `blob_harness.c` and `harness.c` if you want to recompile them so that they point to your `sqlite3.c` amalgamation file

## Todo
- Create better corpus (more examples, longer, more varied queries)
- Some type of static analysis
    - Joern?
    - Source/sink analysis?
