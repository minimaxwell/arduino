#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

/**
 * Lecture d'un fichier d'inputs dolphin.
 * M.Chevallier
 *
 *
 * Docs : 
 * http://tasvideos.org/DTM.html
 *
 * */

void dump_header( char buff[255] ){
    
    int i,j;
    // hex dump

    printf("----------- Header -----------------\n");
    printf("--------- Hex dump -----------------\n\n");
    for(i = 0 ; i < 16 ; i++){
        for(j = 0 ; j < 16 ; j++){
            printf("%02x ",(0xFF & buff[ j + i*16 ] ));
        }
        printf("\n");
    }

    printf("\n--------- fields --------------------\n");
    
    printf(" Signature : %.3s %u\n", &buff[0], buff[3]);
    printf(" Game ID : %.6s\n", &buff[4]);
    printf(" Wii game : %s\n", ( buff[10] ? "yes" : "no" ) );
    printf(" Nb controllers : %d\n", buff[11]);
    printf(" Start from savestate : %s\n", ( buff[12] ? "yes" : "no" ) );

    long long int vi_count;
    memcpy(&vi_count, &buff[13], 8);
    printf(" VI count : %lld\n", vi_count );

    long long int input_count;
    memcpy(&input_count, &buff[21], 8);
    printf(" Input count : %lld\n", input_count);

    long long int lag_count;
    memcpy(&lag_count, &buff[29], 8);
    printf(" Lag count : %lld\n", lag_count);
    // 8 octets unused
    printf(" Nb rerecord : %d\n", 
            (int) ( buff[45] << 24 | buff[46] << 16 | buff[47] << 8 | buff[48] ) );
    printf(" Author : %.32s\n",&buff[49]);
    printf(" Video backend : %.16s\n", &buff[81]);
    printf(" Game MD5 : ");
    for(i=113; i<113+16; i++)
        printf("%02x",( 0xFF & buff[i]));
    printf("\n");

    long long int timestamp;
    memcpy(&timestamp, &buff[129], 8);
    printf(" Timestamp enreg. : %lld\n", timestamp);

    printf(" Saved config valid : %s\n", ( buff[137] ? "yes" : "no" ) );
    printf(" Idle skipping : %s\n", ( buff[138] ? "yes" : "no" ) );
    printf(" Dual core : %s\n", ( buff[139] ? "yes" : "no" ) );
    printf(" Progressive scan : %s\n", ( buff[140] ? "yes" : "no" ) );
    printf(" DSP HLE : %s\n", ( buff[141] ? "yes" : "no" ) );
    printf(" Fast disc speed : %s\n", ( buff[142] ? "yes" : "no" ) );
    printf(" CPU core : %d\n", buff[143] );
    printf(" EFB access : %s\n", ( buff[144] ? "yes" : "no" ) );
    printf(" EFB copy : %s\n", ( buff[145] ? "yes" : "no" ) );
    printf(" Copy EFB to texture : %s\n", ( buff[146] ? "yes" : "no" ) );
    printf(" EFB copy cache : %s\n", ( buff[147] ? "yes" : "no" ) );
    printf(" Emulate format change : %s\n", ( buff[148] ? "yes" : "no" ) );
    printf(" XFB emulation : %s\n", ( buff[149] ? "yes" : "no" ) );
    printf(" Real XFB emulation : %s\n", ( buff[150] ? "yes" : "no" ) );
    printf(" Memory card : %s\n", ( buff[151] ? "yes" : "no" ) );
    printf(" Memory card blank : %s\n", ( buff[152] ? "yes" : "no" ) );
    printf(" Nb bongos : %d\n", buff[153] );
    printf(" Sync GPU thread : %s\n", ( buff[154] ? "yes" : "no" ) );

    printf(" Name of second disc : %.40s\n", &buff[169] );
    printf(" SHA-1 git revision : ");
    for(i=209; i<209+20; i++)
        printf("%02x",( 0xFF & buff[i]));
    printf("\n");
    printf(" DSP IROM Hash : %d\n", 
            (int) ( buff[229] << 24 | buff[230] << 16 | buff[231] << 8 | buff[232] ) );
    printf(" DSP IROM Hash : %d\n", 
            (int) ( buff[233] << 24 | buff[234] << 16 | buff[235] << 8 | buff[236] ) );

    long long int tick_count;
    memcpy(&tick_count, &buff[237], 8);
    printf(" Tick count : %lld\n", tick_count );


}

int main(int argc, char **argv){
    
    char header_buffer[ 255 ];
    // ouvrir le fichier
    if( argc < 2 ){
        printf("Usage : ./dtm_dump <nom_fichier>\n");
        return -1;
    }
    // lire le header
    
    FILE *fp;
    int n, nb_read = 0;

    fp = fopen( argv[1], "r" );
    if( fp == NULL ){
        printf("erreur a l'ouverture du fichier %s. abort. \n", argv[1]);
    }
   
    do{
        n = fread( &header_buffer[nb_read], sizeof(char), 255, fp );
        if( n < 0 ){
            printf("erreur pendant la lecture du fichier. \n");
            return -2;
        }
        nb_read += n;
    }while( nb_read < 255 );

    dump_header(header_buffer);

    // lire le body
    return 0;
}   
