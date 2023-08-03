#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <limits.h>
#include <time.h>

int main ( int argc, char *argv[] )
{   
    if ( argc != 4 ) /* argc should be 2 for correct execution */
    {
        /* We print argv[0] assuming it is the program name */
        printf( "usage: %s filename\n", argv[0] );
    }
    else 
    {
        char *path = argv[1];
        char filename[15];
        char filename_enc[25];
        char *action;
        action = argv[2];
        char *i = argv[3];
        if(strcmp(action, "e") == 0)
        {
            snprintf(filename, 15, "efile%s.txt", i);
            FILE *file = fopen(filename, "w" );
            char *msg = "Hello!\nThis is a file ready to be encrypted.\nBye!";
            fwrite(msg,1,strlen(msg),file);
            fclose( file );
        }
        else if (strcmp(action, "ee") == 0)
        {
            snprintf(filename, 15, "efile%s.txt", i);
            FILE *file = fopen(filename, "w+" );

            snprintf(filename_enc, 25, "efile%s.txt.encrypt", i);
            FILE *file_enc = fopen(filename_enc, "w+" );
            char *msg = "";
            fwrite(msg,1,strlen(msg),file_enc);
            fclose( file );

            fclose( file_enc );
        }
        else if (strcmp(action, "c") == 0)
        {
            snprintf(filename, 15, "cfile%s.txt", i);
            FILE *file = fopen(filename, "w" );
            char *msg = "Hello!\nThis is a file ready to be used.\nBye!";
            fwrite(msg,1,strlen(msg),file);
            fclose( file );
        }
    }
    argc -= optind;
	argv += optind;	
}