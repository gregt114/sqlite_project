
#include "../build/sqlite-autoconf-3410200/sqlite3.c" // may need to alter this to point to your sqlite3.c amalgamation file

#include <stdio.h>

// Fuzzing compilation:
// afl-gcc-fast blob_harness.c -g -fsanitize=address -o blob_harness

// Fast compilation (debugging purposes, no AFL instrumentation or address sanitizer):
// gcc blob_harness.c -g -o blob_harness

// How long the data in the file can be
const size_t DATA_LEN = 256;

int main(int argc, char **argv)
{
    // Expecting argument to be a file containg sql queries
    if (argc != 2)
    {
        printf("Usage: ./harness <FILENAME>\n");
        exit(1);
    }

    // Read data from file into buffer
    char data[DATA_LEN];
    int fd = open(argv[1], O_RDONLY);
    if (fd < 0)
    {
        printf("ERROR: could not open file\n");
        exit(1);
    }
    size_t num_read = read(fd, data, DATA_LEN - 1);
    data[num_read] = '\0'; // add null terminator
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

    // Create a table
    rc = sqlite3_exec(db, "CREATE TABLE test (INTEGER x, TEXT t, BLOB b);", NULL, NULL, NULL);
    if (rc != SQLITE_OK)
    {
        printf("ERORR: table creation failed\n");
        sqlite3_close(db);
        exit(1);
    }

    // Insert fuzzed data into each field
    char query[2048];
    sprintf(query, "INSERT INTO test VALUES(%s, %s, %s);", data, data, data);
    rc = sqlite3_exec(db, query, NULL, NULL, NULL);
    if (rc != SQLITE_OK)
    {
        printf("ERORR: insert query failed\n");
        sqlite3_close(db);
        exit(1);
    }

    // Read from table
    rc = sqlite3_exec(db, "SELECT * from test;", NULL, NULL, NULL);
    if (rc != SQLITE_OK)
    {
        printf("ERORR: read query failed\n");
        sqlite3_close(db);
        exit(1);
    }

    // Clean up
    sqlite3_close(db);
    sqlite3_shutdown();

    return 0;
}
