#include "dtm.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <stdint.h>
#include <fcntl.h>
#include <termios.h>
#include <errno.h>
#include <sys/ioctl.h>


#define SIZE_ENREG 5000


void unpack_gamecube_input( t_dtm_input_data *input, char *raw_input ){
    input->b_a = raw_input[0] & 0x01 ? 1 : 0;
    input->b_b = raw_input[0] & 0x02 ? 1 : 0;
    input->b_x = raw_input[0] & 0x04 ? 1 : 0;
    input->b_y = raw_input[0] & 0x08 ? 1 : 0;
    input->b_start = raw_input[0] & 0x10 ? 1 : 0;

    input->p_left = raw_input[1] & 0x01 ? 1 : 0;
    input->p_right = raw_input[1] & 0x02 ? 1 : 0;
    input->p_down = raw_input[1] & 0x04 ? 1 : 0;
    input->p_up = raw_input[1] & 0x08 ? 1 : 0;
    input->b_z = raw_input[1] & 0x10 ? 1 : 0;
    input->b_right = raw_input[1] & 0x20 ? 1 : 0;
    input->b_left = raw_input[1] & 0x40 ? 1 : 0;

    input->ls_x = raw_input[2];
    input->ls_y = raw_input[3];
    input->cs_x = raw_input[4];
    input->cs_y = raw_input[5];
    input->t_left = raw_input[6];
    input->t_right = raw_input[7];
}

int main( int argc, char **argv ){

    t_dtm_input_data *in =(t_dtm_input_data *)malloc(sizeof(t_dtm_input_data));
    t_dtm_header *head = NULL;
    int nb_inputs = 0;
    FILE *dest = NULL;
    char gc_inputs[ 8 *SIZE_ENREG ];

    int fd, i,res, nb_read;
    char req[3], resp[8];
    struct termios toptions;
    if( argc < 3 ){
        printf("usage : ./test_duplicate <fichier_dtm> <fichier_dest>\n");
        return -1;
    }
    int a;

    /* open serial port */
    fd = open("/dev/ttyUSB0", O_RDWR | O_NOCTTY);
    if(fd < 0){
        printf("Error opening /deC/ttyUSB0\n");
    }else{
        printf("File open !\n");
    }

    printf("get current serial port settings\n");
    /* get current serial port settings */
    tcgetattr(fd, &toptions);
    /* set 9600 baud both ways */
    printf("Setting speed\n");
    cfsetispeed(&toptions, B57600);
    cfsetospeed(&toptions, B57600);
    /* 8 bits, no parity, no stop bits */
    cfmakeraw(&toptions);

    printf("commit the serial port settings\n");
    tcsetattr(fd, TCSANOW, &toptions);

    printf("Ready to read...\n");

    nb_read = 0;
    do{
        nb_read = 0;
        do{
            nb_read += read(fd, &req[ nb_read ], (3 - nb_read) );
        }while( nb_read != 3 );

        nb_read = 0;
        do{
            nb_read += read(fd, &resp[ nb_read ], (8 - nb_read) );
        }while( nb_read != 8 );
        
        
        for(i = 0; i <  8 ; ++i){
            gc_inputs[( nb_inputs * 8 ) + i] = resp[i];
            printf("%02x ", 0xFF & gc_inputs[( nb_inputs * 8)+i]);
        }
        printf("\n");
        nb_inputs++;

        if(nb_inputs % 100 == 0)
            printf("nb_inputs : %d\n", nb_inputs);
    }while(nb_inputs != SIZE_ENREG);

    printf("recording complete, dumping...\n");


    head = dtm_read_header_from_dump( argv[1] );
    if(head == NULL){
        printf("erreur extraction du header, fichier %s\n", argv[1]);
        return -1;
    }

    dest = fopen( argv[2], "w+" );
    if( dest == NULL ){
        printf("erreur a l'ouverture du fichier %s\n",argv[2]);
        return -1;
    }
    
    head->vi_count = SIZE_ENREG;
    head->input_count = SIZE_ENREG;
    head->tick_count = 800000000000;

    dtm_write_header_to_file( dest, head );

    for(i = 0 ; i < SIZE_ENREG; i+=2 ){
        unpack_gamecube_input(in , &gc_inputs[i*8] );
        res = dtm_write_input_data( dest, in );
        if( res < 0 ){
            printf("erreur ecriture\n");
        }
    }

    fclose(dest);
    return 0;
}
