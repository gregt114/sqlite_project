
#include "../build/sqlite-autoconf-3410200/sqlite3.c" // may need to alter this to point to your sqlite3.c amalgamation file

#include <stdio.h>

// Fuzzing compilation:
// afl-gcc-fast harness.c -g -fsanitize=address -o harness

// Fast compilation (debugging purposes, no AFL instrumentation or address sanitizer):
// gcc harness.c -g -o harness

// How long the query in the file can be
const size_t QUERY_LEN = 512;

int main(int argc, char **argv)
{
    // Expecting argument to be a file containg sql queries
    if (argc != 2)
    {
        printf("Usage: ./harness <FILENAME>\n");
        exit(1);
    }

    // Read data from file into buffer
    char query[QUERY_LEN];
    int fd = open(argv[1], O_RDONLY);
    if (fd < 0)
    {
        printf("ERROR: could not open file\n");
        exit(1);
    }
    size_t num_read = read(fd, query, QUERY_LEN - 1);
    query[num_read] = '\0'; // add null terminator
    close(fd);

    // Initialize the SQLite3 library
    sqlite3 *db;
    int rc = sqlite3_initialize();
    if (rc != SQLITE_OK)
    {
        printf("Failed to initialize SQLite3 library: %s\n", sqlite3_errstr(rc));
        return rc;
    }

    // Create a new database connection
    rc = sqlite3_open(":memory:", &db);
    if (rc != SQLITE_OK)
    {
        printf("Failed to create new database connection: %s\n", sqlite3_errmsg(db));
        sqlite3_close(db);
        return rc;
    }

    // Execute query
    rc = sqlite3_exec(db, query, NULL, NULL, NULL);
    if (rc != SQLITE_OK)
    {
        printf("ERORR: query failed\n");
        sqlite3_close(db);
        exit(1);
    }

    // Clean up
    sqlite3_close(db);
    sqlite3_shutdown();

    return 0;
}
