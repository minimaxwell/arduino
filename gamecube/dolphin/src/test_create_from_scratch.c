
#include "dtm.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main( int argc, char **argv ){

    t_dtm_input_data *in;
    int done = 0, res;
    int nb_inputs = 0;
    FILE *fp = NULL;
    FILE *dest = NULL;

    if( argc < 2 ){
        printf("usage : ./test_duplicate <fichier_dtm> <fichier_dest>\n");
        return -1;
    }

    fp = fopen(argv[1], "r+");
    if( fp == NULL ){
        printf("erreur a l'ouverture du fichier %s\n",argv[1]);
        return -1;
    }

    dest = fopen( argv[2], "w+" );
    if( fp == NULL ){
        printf("erreur a l'ouverture du fichier %s\n",argv[2]);
        return -1;
    }

    t_dtm_header *head = NULL;
    head = dtm_consume_header( fp );
    if( head == NULL){
        printf("erreur à la construction du header\n");
        return -2;
    }
    head->vi_count = 3000;
    head->tick_count = 3000000000;
    dtm_print_header_infos( head );

    dtm_write_header_to_file( dest, head );

    free(head);


    do{
        in = dtm_consume_input_data( fp );
        if( in == NULL ){
            done = 1;
        }else{
            nb_inputs++;
            if( nb_inputs > 700 && nb_inputs < 705 ){
                in->b_a = 1;
            }
            res = dtm_write_input_data( dest, in );
            if( res < 0 ){
                printf("erreur ecriture\n");
            }
        }
        free(in);
    }while( !done );

    printf("nb inputs : %d\n", nb_inputs);

    fclose(fp);
    fclose(dest);
    return 0;
}
