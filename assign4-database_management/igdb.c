#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include <time.h>
#include <errno.h>

#include "database.h"

// USER INPUT VALIDATION FUNCTIONS

void reset_variables(char *handle, char *followers) {
    handle[0] = '\0';
    followers[0] = '\0';
}

int valid_commaless(char *input) {
    while (*input != '\0') {
        if (*input == ',') return 0;
        input++;
    }
    return 1;
}

int valid_followers(char * followers_str) {
    char *endptr;
    errno = 0;

    unsigned long int followers = strtoul(followers_str, &endptr, 10);

    if ((errno == ERANGE && (followers == ULONG_MAX || followers == 0)) || endptr == followers_str || *endptr != '\0') {
        printf("Error: follower count must be a valid positive integer.\n");
        return 0;
    }

    return 1;
}

int is_valid_handle(Database * db, char *handle) {   
    // handles can only be max. 32 characters
    if (strlen(handle) > 32) {
        printf("Error: handle too long.\n");
        return 0;
    }

    // handles must start with @ and contain at least one character after that, without commas
    if (handle[0] != '@' || handle[1] == '\0' || !valid_commaless(handle)) {
        printf("Error: handle must be of format @HANDLE and cannot contain commas.\n");
        return 0;
    }

    // check if the handle is already taken
    if (db_lookup(db, handle) != NULL) {
        printf("Error: handle %s already exists.\n", handle);
        return 0;
    }

    return 1; 
}

int valid_existing_record(Record *existing_record, char *handle) {
    // check if there is a record with the handle already
    if (existing_record == NULL) {
        printf("Error: no entry with handle %s.\n", handle);
        return 0;
    }
    return 1;
}


// HELPER FUNCTIONS

char * get_comment() {
    // prompt user to input a comment, then prepare it for use in database
    printf("Comment > ");

    char *comment = malloc(64 * sizeof(char)); // allocating memory for the comment
    fgets(comment, 64, stdin);
    
    comment[strcspn(comment, "\n")] = '\0'; // replacing the newline character with a string terminator

    return comment;
}

void update_cft(Record *existing_record, char *comment, long unsigned int followers)
{
    // updates comment, # followers and last modified time for a given record
    strncpy(existing_record->comment, comment, sizeof(existing_record->comment) - 1);
    existing_record->comment[sizeof(existing_record->comment) - 1] = '\0';
    existing_record->follower_count = followers;
    existing_record->date_last_modified = time(NULL);
}

char * convert_time(long unsigned int to_convert) {

    char *readable_time = malloc(20 * sizeof(char));
    struct tm *local_time = localtime(&to_convert);
    strftime(readable_time, 20, "%Y-%m-%d %H:%M", local_time);

    return readable_time;
}

void list(Database *db) {
    printf("HANDLE          | FOLLOWERS | LAST MODIFIED    | COMMENT\n");
    printf("----------------|-----------|------------------|--------------------------------\n");

    for (int i = 0; i < db->size; ++i)
 {
        // parse through each record in the database and print it
        Record *record = &(db->data[i]);
         
        // convert unix time -> readable time
        char *readable_time = convert_time(record->date_last_modified);
        printf("%-15s | %-9lu | %-16s | %s\n", record->handle, record->follower_count, readable_time, record->comment);
        free(readable_time);
    }
}

int add_record(Database *db, char *handle, char * followers_str) {
    // check that valid handle and follower count were given by user
    if (!is_valid_handle(db, handle) || !valid_followers(followers_str)) return 1;

    unsigned long int followers = strtoul(followers_str, NULL, 10);

    // prompting comment input
    char * comment = get_comment();

    if (!valid_commaless(comment)) {
        printf("Error: comment cannot contain commas.\n");
        free(comment);
        return 1;
    }

    // creating the new record, populating it with the info, and adding it to the database
    Record *new_record = malloc(sizeof(Record));
    strncpy(new_record->handle, handle, sizeof(new_record->handle));
    update_cft(new_record, comment, followers);
    db_append(db, new_record);

    free(comment);
    return 0;
}

void update_record(Database *db, char *handle, char * followers_str) {
    Record *existing_record = db_lookup(db, handle);
    if (!valid_existing_record(existing_record, handle)) return;

    unsigned long int followers = strtoul(followers_str, NULL, 10);

    // get and validate comment
    char * comment = get_comment();

    if (!valid_commaless(comment)) {
        free(comment);
        return ;
    }

    update_cft(existing_record, comment, followers);
    free(comment);
}

void save_database(Database *db, char *path) {
    // writes the database to a csv file
    db_write_csv(db, path);
    printf("Wrote %d records.\n", db->size);
}

// -------------------------------------------------------------------------------------------------
// MAIN PROGRAM (BUSINESS LOGIC)

int main_loop(Database * db) {
    printf("Loaded %d records.\n", db->size);

    int unsaved_changes = 0;
    char command[100], handle[33], followers[20]; // extra byte for null terminator in handle

    while (1) {

        printf("> ");
        fgets(command, sizeof(command), stdin);
        command[strcspn(command, "\n")] = '\0';

        if (strcmp(command, "list") == 0) list(db);

        else if (strncmp(command, "add", 3) == 0) { 
            int input = sscanf(command + 4, "%s %s", handle, followers);
            if (input > 2 || input < 1) printf("Error: usage: add HANDLE FOLLOWERS\n");
            else if (add_record(db, handle, followers) == 0) unsaved_changes = 1;

            reset_variables(handle, followers);

        } 

        else if (strncmp(command, "update ", 7) == 0) {
            if (sscanf(command + 7, "%s %s", handle, followers) == 2){
                update_record(db, handle, followers);
                unsaved_changes = 1;
            }

            else printf("Error: usage: update HANDLE FOLLOWERS\n");

            reset_variables(handle, followers);
        }

        else if (strcmp(command, "save") == 0) {
            save_database(db, "database.csv");
            unsaved_changes = 0;
        }

        else if (strcmp(command, "exit") == 0 && unsaved_changes != 0) {
            printf("Error: you did not save your changes. Use `exit fr` to force exiting anyway.\n");      
        }

        else if (strcmp(command, "exit fr") == 0) break;

        else if (strcmp(command, "exit") == 0 && unsaved_changes == 0) break;

        else printf("Invalid input. Usages: list, add HANDLE FOLLOWERS, update HANDLE FOLLOWERS, save, exit.\n");

    }

    db_free(db); // freeing the memory used for the database
    return 0;
}

int main() {
    Database db = db_create();
    db_load_csv(&db, "database.csv");
    return main_loop(&db);
}