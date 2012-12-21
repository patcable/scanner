#define _XOPEN_SOURCE 500
#include <ftw.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

// Scan function
int scan_tree(const char *fpath, const struct stat *sb, int tflag, struct FTW *ftwbuf) {
    // define file pointer, size and buffer 
    FILE *pFile;
    long lSize;
    char *bbuffer;

    // only scan files (flag FTW_F)
    if(tflag == FTW_F) {
        printf("Scanning %s... ", fpath);
        // Open the file, get the size, read it into memory, etc.
        pFile = fopen(fpath, "rb");
        if(pFile == NULL) {
            printf("Could not open %s\n", fpath);
            exit(1);
        }
        fseek(pFile, 0, SEEK_END);
        lSize = ftell(pFile);
        rewind(pFile);
        bbuffer = (char*)malloc(lSize);
        if(bbuffer == NULL) {
            printf("Could not read %s into memory\n", fpath);
            exit(1);
        }
        fread(bbuffer, 1, lSize, pFile);

        // scan stuff here
        int idx, sidx;
        /*
        // So - I need to get char *sig up here. how?
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

int main (int argc, char **argv) {
    const char *dir_path = argv[1];
    int flags = 0;
    FILE * pFile;
    long lSize;
    char *buffer;
    int i;
    int count = 0;
    char *sig[1024]; // Assuming the maximum number of signatures is 1024

    if (argc != 3) {
        fprintf(stderr, "Usage: %s directory signature_file\n", *argv);
        exit(1);
    }
    printf("DEBUG: Reading %s into memory... ", argv[2]);
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

    //parse signatures
    sig[0] = strtok(buffer, "\n");
    ++count;
    while (1) {
        sig[count] = strtok(NULL, "\n");
        if (sig[count] == NULL) {
            break;
        }
        ++count;
    }
    free (buffer);
    fclose (pFile);
    printf("Done!\n");

    // use nftw() to go through the tree structure, use up to 20 fd's
    nftw(dir_path, scan_tree, 20, flags);

    return 0;
}
