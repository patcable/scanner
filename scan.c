// This define is needed to make the nftw() function work
#define _XOPEN_SOURCE 500

// Includes for standard libraries, int structs, IO, and the nftw() function
// that will traverse the directory
#include <ftw.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

// Patrick Cable - 4055.760 - Computer Virus and Malware
// 2012-12-21 - Prof. Bo Yuan

// Scan function - this needs to be defined before int main(), so here it is.
// nftw() calls scan_tree for each file found in the structure given on the
// command line
int scan_tree(const char *fpath, const struct stat *sb, int tflag, struct FTW *ftwbuf) {
    // define file pointer, a place to store file size and a character buffer 
    FILE *pFile;
    long lSize;
    char *bbuffer;

    // only scan files (flag FTW_F from nftw() spec)
    if(tflag == FTW_F) {
        // Begin scan
        printf("Scanning %s... ", fpath);
        // Open the file, get the size, read it into memory, etc.
        pFile = fopen(fpath, "rb");
        if(pFile == NULL) {
            printf("Could not open %s\n", fpath);
            exit(1);
        }
        // go to the end of the file
        fseek(pFile, 0, SEEK_END);
        // store this as the size
        lSize = ftell(pFile);
        // set the read pointer back to the beginning of the file
        rewind(pFile);
        // allocate the size stored in lSize
        bbuffer = (char*)malloc(lSize);
        if(bbuffer == NULL) {
            printf("Could not read %s into memory\n", fpath);
            exit(1);
        }
        //read the file into the buffer
        fread(bbuffer, 1, lSize, pFile);

        // scan stuff here
        int idx, sidx;
        /* 
        ** so, we read in the virus definitions in the main function below, but 
        ** i need to use them here. i could re-read them for each file we scan
        ** but that seems like a waste of time... hmm.
        for(i = 0; i < count; ++i) {
            for(idx = 0; idx < lSize; ++idx) {
                int matched = 0;
                for(sidx = 0; sidx < strlen(sig[i]); ++sidx) {
                    if(*(sig[i]+sidx) != bbuffer[idx]) {
                        break;
                    }
                    ++matched;
                    ++idx;
                    if(matched == strlen(sig[i])) {
                        printf("%s: Contains virus %d", namelist[files]->d_name, i+1);
                        idx = lSize;
                    }
                }
            }
        }
        */
        printf("Clean!\n");

        // close file, dealloc buffer
        fclose(pFile);
        free(bbuffer);
    }
    return 0;
}

// Beginning of main program
int main (int argc, char **argv) {
    // variables - dir_path is the path, flags is or nftw,
    // and we reuse the names for file name/size/buffer.
    // i/count are counters
    // sig is where we store the information on what a virus is
    const char *dir_path = argv[1];
    int flags = 0;
    FILE * pFile;
    long lSize;
    char *buffer;
    int i;
    int count = 0;
    char *sig[1024]; // Assuming the maximum number of signatures is 1024

    // if the command isnt run with the right parameters, tell the user 
    // how to use it.
    if (argc != 3) {
        fprintf(stderr, "Usage: %s directory signature_file\n", *argv);
        exit(1);
    }

    // Read the virus defs
    pFile = fopen(argv[2], "rb");
    if(pFile == NULL) {
        printf("Could not open file.\n");
        exit(1);
    }

    //get file size
    fseek (pFile , 0 , SEEK_END);
    lSize = ftell (pFile);
    rewind (pFile);

    //allocate memory
    buffer = (char*) malloc (lSize);
    if (buffer == NULL) {
        printf("Could not allocate memory.\n");
        exit(2);
    }

    //read file into buffer
    fread(buffer, 1, lSize, pFile);

    //take the virus signatures out of the file and stuff them into
    //the 'sig' variable
    sig[0] = strtok(buffer, "\n");
    ++count;
    while (1) {
        sig[count] = strtok(NULL, "\n");
        if (sig[count] == NULL) {
            break;
        }
        ++count;
    }

    // clean up after ourselves...
    free (buffer);
    fclose (pFile);
    printf("OK.\n");

    // use nftw() to go through the tree structure
    nftw(dir_path, scan_tree, 20, flags);

    return 0;
}
