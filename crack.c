#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include "md5.h"

const int PASS_LEN=20;        // Maximum any password will be
const int HASH_LEN=33;        // Length of MD5 hash strings

// Given a hash and a plaintext guess, return 1 if
// the hash of the guess matches the given hash.
// That is, return 1 if the guess is correct.
int tryguess(char *hash, char *guess)
{
    
    
    char *newhash = md5(guess, strlen(guess));
    // Compare the two hashes
    if(strcmp(newhash, hash) == 0)
    {
        free(newhash);
        return 1;
    }
    // Free any malloc'd memory
    free(newhash);
    return 0;
}

// Read in the dictionary file and return the array of strings
// and store the length of the array in size.
// This function is responsible for opening the dictionary file,
// reading from it, building the data structure, and closing the
// file.
char **read_dictionary(char *filename, int *size)
{
    int file_size = file_length(filename);
    char *passwords = malloc(file_size);
    FILE *f = fopen(filename, "r");
    if(!f)
    {
        printf("Cant open %s for reading.", filename);
        exit(1);
    }
    
    fread(passwords, 1, file_size, f);
    fclose(f);
    
    int length = 0;
    
    for(int i = 0; i < file_size; i++)
    {
        if(passwords[i] == '\n')
        {
            passwords[i] = '\0';
            length++;
        }
    }
    
    char **strings = malloc(length*sizeof(char *));
    strings[0] = passwords;
    int j = 1;
    for(int i =0; i < file_size-1; i++)
    {
        if(passwords[i] == '\0')
        {
          strings[j] = &passwords[i+1];
          j++;
        }
    }
    
    *size = length;
    return strings;
}

int file_length(char *filename)
{
    struct stat info;
    stat(filename, &info);
    return info.st_size;
}

int main(int argc, char *argv[])
{
    if (argc < 3) 
    {
        printf("Usage: %s hash_file dict_file\n", argv[0]);
        exit(1);
    }

    // Read the dictionary file into an array of strings.
    int dlen;
    char **dict = read_dictionary(argv[2], &dlen);

    // Open the hash file for reading.
    FILE *h = fopen(argv[1], "r");
    if(!h)
    {
        printf("Cant open %s for reading.", argv[1]);
        exit(1);
    }
    
    // For each hash, try every entry in the dictionary.
   
   
    
    char hashes[34];
    while(fgets(hashes, 34, h))
    {
       hashes[strlen(hashes)-1] = '\0';
        for(int i = 0; i < dlen; i++)
        {
            if(tryguess(hashes, dict[i]) == 1)
            {
                printf("%s %s\n", hashes, dict[i]);
            }
        }
        
    }
    
    // Print the matching dictionary entry.
    // Need two nested loops.
       
}