
#include "/home/kali/Applications/sqlite-amalgamation-3410200/sqlite3.c" // may need to alter this to point to your sqlite3.c amalgamation file

#include <stdio.h>

// Fuzzing compilation:
// afl-gcc-fast dbfile_harness.c -g -fsanitize=address -o dbfile_harness

// Fast compilation (debugging purposes, no AFL instrumentation or address sanitizer):
// gcc dbfile_harness.c -g -o dbfile_harness

int main(int argc, char **argv)
{
    // Expecting argument to be a sqlite db file
    if (argc != 2)
    {
        printf("Usage: ./harness <FILENAME>\n");
        exit(1);
    }

    // Initialize the SQLite3 library
    sqlite3 *db;
    int rc = sqlite3_initialize();
    if (rc != SQLITE_OK)
    {
        printf("Failed to initialize SQLite3 library: %s\n", sqlite3_errstr(rc));
        return rc;
    }

    // Create a new database connection from the given file
    rc = sqlite3_open(argv[1], &db);
    if (rc != SQLITE_OK)
    {
        printf("Failed to create new database connection: %s\n", sqlite3_errmsg(db));
        sqlite3_close(db);
        return rc;
    }

    // Clean up
    sqlite3_close(db);
    sqlite3_shutdown();

    return 0;
}
