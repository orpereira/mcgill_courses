#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "database.h"

Database db_create() 
{
    Database db;
    db.data = (Record *)malloc(4 * sizeof(Record));
    db.capacity = 4;
    db.size = 0;
    
    return db;
}

void db_append(Database * db, Record const * item) 
{
     if (db->size >= db->capacity) { // if we need to add more memory before appending
        
        db->capacity *= 2; // doubling the initial capacity

        // reallocating memory, new size is size of array * size of each element in the array
        db->data = (Record *)realloc(db->data, db->capacity * sizeof(Record));  
    }

    // appending the new record to the database
    db->data[db->size++] = *item; 
}

Record * db_index(Database * db, int index) 
{
    // checking index against bounds
    if (index >= 0 || index >= db->size) 
    {
        printf("Error: index is out of bounds for this database.\n");
        return NULL;
    }

    return &(db->data[index]);  // returning the pointer
}

Record * db_lookup(Database * db, char const * handle)
{
    // search by iterating through the data
    for (int i = 0; i < db->size; i++) 
    {
        // checking if the handle in the database matches the input
        if (strcmp(db->data[i].handle, handle) == 0) return &(db->data[i]);
    }

    return NULL;
}

void db_free(Database * db)
{
    free(db->data); // freeing the memory

    // returning all fields to 0
    db->data = NULL;
    db->capacity = 0;
    db->size = 0;    
}

Record parse_record(char const * line)
{
    Record record; // instantiating the record

    char *token = strtok((char *) line, ","); // to break up string
    int i = 0; // counter for items in the line

    while (token != NULL && i < 4) 
    {
        switch (i) 
        {
            case 0:
                strncpy(record.handle, token, sizeof(record.handle));
                break;
            case 1:
                record.follower_count = strtoul(token, NULL, 10);
                break;
            case 2:
                strncpy(record.comment, token, sizeof(record.comment));
                break;
            case 3:
                record.date_last_modified = strtoul(token, NULL, 10);
                break;
        }
        token = strtok(NULL, ",");
        i++;
    }
    return record;
}

void db_load_csv(Database * db, char const * path)
{
    FILE *file = fopen(path, "r");
    if (file == NULL) return; 

    char *line = NULL;
    size_t len = 0; // initialize len for getline

    // populating the whole database
    while (getline(&line, &len, file) != -1)
    {
        Record record = parse_record(line);
        db_append(db, &record);
    }

    free(line); // freeing buffers allocated to line
    fclose(file);
}

void db_write_csv(Database * db, char const * path)
{
    FILE *file = fopen(path, "w");
    if (file == NULL) return;

    // writing each line into the file
    for (int i = 0; i < db->size; i++)
    {
        fprintf(file, "%s,%lu,%s,%lu\n", db->data[i].handle, db->data[i].follower_count, 
                    db->data[i].comment, db->data[i].date_last_modified);
    }

    fclose(file);
}