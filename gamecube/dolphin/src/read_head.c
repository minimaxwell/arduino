#include "dtm.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main( int argc, char **argv ){
   
    if( argc < 2 ){
        printf("usage : ./read_head <fichier_dtm>\n");
        return -1;
    }

    t_dtm_header *head;
    head = read_header_from_dump( argv[1] );
    if( head == NULL){
        printf("erreur à la construction du header\n");
        return -2;
    }

    print_header_infos( head );

    free(head);

    return 0;
}
