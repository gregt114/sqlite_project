
#include "sqlite3.c"

#include <stdio.h>

// Fuzzing compilation:
// afl-gcc-fast harness.c -g -fsanitize=address -o harness

// Fast compilation (debugging purposes, no AFL instrumentation or address sanitizer):
// gcc harness.c -g -o harness

// How many bytes of data to read in from file
const size_t DATA_LEN = 256;

// In memory fuzzing via arg passing
int main(int argc, char **argv)
{

    // Expecting argument to be a sql file
    if (argc != 2)
    {
        printf("Usage: ./harness <SQL QUERY FILE>\n");
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
    read(fd, data, DATA_LEN);
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
    sprintf(query, "INSERT INTO test VALUES(%s, %s, %s);\0", data, data, data);
    rc = sqlite3_exec(db, query, NULL, NULL, NULL);
    if (rc != SQLITE_OK)
    {
        printf("ERORR: insert query failed\n");
        sqlite3_close(db);
        exit(1);
    }

    // Read from table
    rc = sqlite3_exec(db, "SELECT * from table;", NULL, NULL, NULL);
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