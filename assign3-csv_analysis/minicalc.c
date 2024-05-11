// Olivia Pereira 260985600
// COMP 206 Assignment 3

#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <math.h>
#include <limits.h>
#include <float.h>

// functions

int gcd(int a, int b) {
    /* This algorithm iterates through all the numbers less than or equal to the smaller of the two 
    values inputted by the user, and checks whether it is a divisor of both a and b by evaluating 
    the modulo. If it is a possible divisor, the GCD is updated. We finish with the highest divisor 
    at the end of the loop. 

    Structure taken from scaler.com: https://www.scaler.com/topics/gcd-of-two-numbers-in-c/ */

    int i=0, min = a, gcd = 1; // setting our initial values
    if (a > b) min = b; // correcting in case b is smaller
    
    // looping through to find the highest divisor possible
    for (i = 1; i <= min; i++) {
        if (a%i == 0 && b%i == 0) gcd = i;
    }

    return gcd;
}

char* anagram(char *S1, char *S2) {
    /* This algorithm evaluates if the strings are of the same length, then counts the occurence of 
    each letter in the alphabet in an array. This is done by adding occurences from the first string, 
    then subtracting occurences from the second string. In the end, the array used for counting each 
    letter should be filled with zeros if the two are anagrams. */

    // creating an array to count instances of each letter in the alphabet
    int count[26] = {0}; 

    // algorithm: take letter in the ith position, normalize it to range [0, 26], and log it in count
    int i = 0;
    
    while (S1[i] != '\0' && S2[i] != '\0') {

        // checking if there are non-lowercase letters in either word
        if (!islower(S2[i]) || !islower(S2[i])) {
            fprintf(stderr, "Error: anagram requires lowercase inputs.\n");
            return "error";
        }
        
        count[S1[i] - 'a']++;    // add counts from first word
        count[S2[i] - 'a']--;    // subtract counts from second word

        i++;
    }

    // checking that both words are the same length (ith position should be null in both after loop)
    if (S1[i] != '\0' || S2[i] != '\0') return "false";

    // verifying the array is all zeroes (means same char occurences in each word)
    for (int i = 0; i < 26; i++) {
        if (count[i] != 0) return "false";
    }

    return "true";
}

int conversionSuccess(char *input, char *endptr) {
    // checking if conversion to int or double was successful by making sure the endptr was not pointed
    // at any invalid characters once conversion terminated.
    if (input == endptr || *endptr != '\0') {
        fprintf(stderr, "Error: arguments must integer for + and gcd, and double for sqrt.\n");
        return 0;
    }
    return 1;
}

int main(int argc, char *argv[]) {

    // checking for at least one argument, i.e. argc is at least 3 (program, operation, argument)
    if (argc < 3) {
        fprintf(stderr, "Error: Minicalc requires at least one argument. \n");
        return 1;
    }

    // saving choice of operation (first user input)
    char *operation = argv[1];


    // Operation 1: addition
    if (strcmp(operation, "+") == 0) {

        // checking for number of arguments i.e. argc = 4 (program, operation, arg1, arg2)
        if (argc != 4) {
            fprintf(stderr, "Error: + requires exactly 2 arguments.\n");
            return 3;
        }

        // converting inputted numbers to integers and checking for successful conversion
        char *endptr1, *endptr2;
        int N1 = strtol(argv[2], &endptr1, 10), N2 = strtol(argv[3], &endptr2, 10);
        if (!conversionSuccess(argv[2], endptr1) || !conversionSuccess(argv[3], endptr2)) return 4;

        // checking for possible overflow
        if (N1 > INT_MAX - N2) {
            fprintf(stderr, "Warning: Overflow detected\n");
            printf("%d\n", INT_MAX);
            return 8; 
        }

        // add the two numbers
        printf("%d\n", N1 + N2);
        return 0;
    }

    // Operation 1: greatest common divisor
    else if (strcmp(operation, "gcd") == 0) {

        // checking for number of arguments, i.e. argc => 4 (program, operation, arg1, arg2 ...)
        if (argc < 4) {
            fprintf(stderr, "Error: gcd requires at least 2 arguments.\n");
            return 3;
        }

        // computing the gcd by calling our above function iteratively. 
        //ex: given gcd 8 12 6 2 -> gcd(8, 12) = 4; gcd (4, 6) = 2, gcd(2, 2) = 2 
        
        // setting first argument as initial value for the result and checking if it is a pos integer
        char *endptr;
        int result = strtol(argv[2], &endptr, 10);
        if (!conversionSuccess(argv[2], endptr)) return 4; 

        if (result <= 0) {
            fprintf(stderr, "Error: arguments must all be greater than zero.\n");
            return 7;
        }

        // iteratively applying gcd to rest of inputs after checking their validity
        for (int i = 3; i < argc; i++) {
            char *endptr;
            int N = strtol(argv[i], &endptr, 10);
            if (!conversionSuccess(argv[i], endptr)) return 4;

            if (N <= 0) {
                fprintf(stderr, "Error: arguments must all be greater than zero.\n");
                return 7; 
            }

            else result = gcd(result, N);
        }

        // printing final result
        printf("%d\n", result);
        return 0;
    }

    // Operation 3: square root 
    else if (strcmp(operation, "sqrt") == 0) {

        // checking for number of arguments, i.e. argc = 3 (program, operation, argument)
        if (argc != 3) {
            fprintf(stderr, "Error: sqrt requires exactly 1 argument.\n");
            return 3;
        }
        
        // converting the input to a double
        char *endptr;
        double A = strtod(argv[2], &endptr);
        if (!conversionSuccess(argv[2], endptr)) return 4;

        // checking that the input is greater than zero
        if (A < 0) {
            fprintf(stderr, "Error: Input to sqrt must be greater than zero.\n");
            return 5;
        }

        // printing final result
        printf("%f\n", sqrt(A));
        return 0;
    }

    // Operation 4: anagram tester
    else if (strcmp(operation, "anagram") == 0) {

        // checking for number of arguments, i.e. argc = 4 (program, operation, arg1, arg2)
        if (argc != 4) {
            fprintf(stderr, "Error: anagram requires exactly 2 arguments.\n");
            return 3;
        }

        // saving our two strings and their lengths in variables
        char *S1 = argv[2];
        char *S2 = argv[3];
        char *result = anagram(S1, S2);

        // returning corresponding error code if they were found to contain non-lowercase letters
        if (result == "error") return 6;

        else {
            printf("%s\n", result);   // printing result
            return 0;
        }
    }

    // Invalid operation inputted
    else {
        fprintf(stderr, "Error: First argument must be one of the following: +, gcd, sqrt, anagram.\n");
        return 2;
    }
}