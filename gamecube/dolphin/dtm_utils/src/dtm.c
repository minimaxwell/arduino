#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

#include "dtm.h"

/**
 * Lecture d'un fichier d'inputs dolphin.
 * M.Chevallier
 *
 *
 * Docs : 
 * http://tasvideos.org/DTM.html
 *
 * */

char *dtm_pack_input(t_dtm_input_data *input){
    
    char *data = (char *) malloc( 8 * sizeof(char) );
    if(data == NULL){
        printf("erreur d'allocation pack_input\n");
        return NULL;
    }

    memset( data, 0, 8 );
    data[0] |= ( ( input->b_start ? 1 : 0 ) << 0 ) |
               ( ( input->b_a ? 1 : 0 ) << 1 ) |
               ( ( input->b_b ? 1 : 0 ) << 2 ) |
               ( ( input->b_x ? 1 : 0 ) << 3 ) |
               ( ( input->b_y ? 1 : 0 ) << 4 ) |
               ( ( input->b_z ? 1 : 0 ) << 5 ) |
               ( ( input->p_up ? 1 : 0 ) << 6 ) |
               ( ( input->p_down ? 1 : 0 ) << 7 );

    data[1] |= ( ( input->p_left ? 1 : 0 ) << 0 ) |
               ( ( input->p_right ? 1 : 0 ) << 1 ) |
               ( ( input->b_left ? 1 : 0 ) << 2 ) |
               ( ( input->b_right ? 1 : 0 ) << 3 ) |
               ( ( input->change_cd ? 1 : 0 ) << 4 );
    data[2] = input->t_left;
    data[3] = input->t_right;
    data[4] = input->ls_x;
    data[5] = input->ls_y;
    data[6] = input->cs_x;
    data[7] = input->cs_y;
                
    return data;
}

t_dtm_input_data *dtm_unpack_input(char data[8]){

    t_dtm_input_data *input = NULL;
    input = (t_dtm_input_data *)malloc( sizeof(t_dtm_input_data) );
    memset( input, 0, sizeof( t_dtm_input_data ) );
    if( input == NULL ){
        printf("erreur d'allocation unpack inputs\n");
        return NULL;
    }

    input->b_start = (data[0] & 0x01) ? 1 : 0 ;
    input->b_a = (data[0] & 0x02) ? 1 : 0 ;
    input->b_b = (data[0] & 0x04) ? 1 : 0 ;
    input->b_x = (data[0] & 0x08) ? 1 : 0 ;
    input->b_y = (data[0] & 0x10) ? 1 : 0 ;
    input->b_z = (data[0] & 0x20) ? 1 : 0 ;
    input->p_up =(data[0] & 0x40) ? 1 : 0 ;
    input->p_down = (data[0] & 0x80) ? 1 : 0 ;

    input->p_left = (data[1] & 0x01) ? 1 : 0 ;
    input->p_right = (data[1] & 0x02) ? 1 : 0 ;
    input->b_left = (data[1] & 0x04) ? 1 : 0 ;
    input->b_right = (data[1] & 0x08) ? 1 : 0 ;
    input->change_cd = (data[1] & 0x10) ? 1 : 0 ;

    input->t_left = data[2];
    input->t_right = data[3];
    input->ls_x = data[4];
    input->ls_y = data[5];
    input->cs_x = data[6];
    input->cs_y = data[7];

    return input;

}

//int dtm_write_header_from_existing_dump( FILE *dest_file,  char *existing_dump ){
 //TODO
//}

int dtm_write_header_to_file( FILE *dest_file,  t_dtm_header *header ){

    char *raw_data = NULL;

    raw_data = dtm_pack_header( header );
    if( raw_data == NULL ){
        printf("erreur unpack header\n");
        return -1;
    }

    dtm_write_raw_header_to_file( dest_file, raw_data );

    free( raw_data );

    return 0;
}

int dtm_write_raw_header_to_file( FILE *dest_file, char *raw_data ){
    
    int written;
    written = fwrite( raw_data, sizeof(char), 256, dest_file );
    
    return written == 256 ? 0 : -1;
}


int dtm_write_input_data( FILE *dest_file, t_dtm_input_data *input ){
    
    char *raw_input_data = dtm_pack_input( input );
    if( raw_input_data == NULL ){
        printf("erreur packing input\n");
        return -1;
    }

    fwrite( raw_input_data, sizeof(char), 8, dest_file );

    free( raw_input_data );

    return 0;
}

t_dtm_input_data *dtm_consume_input_data( FILE *fp ){

    char raw_input_data[8];
    int n;
    t_dtm_input_data *input = NULL;

    if( !feof(fp) ){
        n = fread( raw_input_data, sizeof(char), 8, fp );
        if( n == 8){
            input = dtm_unpack_input( raw_input_data );
        }
    }

    return input;
}

t_dtm_header *dtm_consume_header(FILE *fp){

    char header_buffer[256];
    int n, nb_read = 0;
    t_dtm_header *head = NULL;

    do{
        n = fread( &header_buffer[nb_read], sizeof(char), 256, fp );
        if( n < 0 ){
            printf("erreur pendant la lecture du fichier. \n");
            return NULL;
        }
        nb_read += n;
    }while( nb_read < 256 );

    head = dtm_unpack_header( header_buffer );

    return head;
}


t_dtm_header *dtm_read_header_from_dump(  char *dump_name ){

    FILE *fp;
    t_dtm_header *head;

    fp = fopen( dump_name, "r" );
    if( fp == NULL ){
        printf("erreur a l'ouverture du fichier %s. abort. \n", dump_name);
        return NULL;
    }
   
    head = dtm_consume_header( fp );

    fclose(fp);

    return head;

}

char *dtm_pack_header( t_dtm_header *header ){

    char *raw_data = NULL;
    raw_data = (char *)malloc( 256 * sizeof( char ) );
    if( raw_data == NULL ){
        printf("erreur allocation raw data header\n");
        return NULL;
    }
    
    memset( raw_data, 0, 256 * sizeof(char) );

    memcpy( &raw_data[0], &(header->signature), 4 );
    memcpy( &raw_data[4], &(header->game_id), 6 );
    memcpy( &raw_data[10], &(header->is_wii_game), 1 );
    memcpy( &raw_data[11], &(header->nb_controllers), 1 );
    memcpy( &raw_data[12], &(header->starts_from_savestate), 1 );
    memcpy( &raw_data[13], &(header->vi_count), 8 );
    memcpy( &raw_data[21], &(header->input_count), 8 );
    memcpy( &raw_data[29], &(header->lag_count), 8 );
    memcpy( &raw_data[45], &(header->rerecord_count), 4 );
    memcpy( &raw_data[49], &(header->author), 32 );
    memcpy( &raw_data[81], &(header->video_backend), 16 );
    memcpy( &raw_data[113], &(header->md5), 16 );
    memcpy( &raw_data[129], &(header->record_time_start), 8 );
    memcpy( &raw_data[137], &(header->is_saved_config_valid), 1 );
    memcpy( &raw_data[138], &(header->is_idle_skipping_enabled), 1 );
    memcpy( &raw_data[139], &(header->is_dual_core_enabled), 1 );
    memcpy( &raw_data[140], &(header->is_progressive_scan_enabled), 1 );
    memcpy( &raw_data[141], &(header->is_dsp_hle_enabled), 1 );
    memcpy( &raw_data[142], &(header->is_fast_disc_enabled), 1 );
    memcpy( &raw_data[143], &(header->cpu_core), 1 );
    memcpy( &raw_data[144], &(header->is_efb_access_enabled), 1 );
    memcpy( &raw_data[145], &(header->is_efb_copy_enabled), 1 );
    memcpy( &raw_data[146], &(header->is_copy_efb_to_texture_enabled), 1 );
    memcpy( &raw_data[147], &(header->is_efb_copy_cache_enabled ), 1 );
    memcpy( &raw_data[148], &(header->is_emulate_format_changes_enabled), 1 );
    memcpy( &raw_data[149], &(header->uses_xfb_emulation), 1 );
    memcpy( &raw_data[150], &(header->uses_real_xfb_emulation), 1 );
    memcpy( &raw_data[151], &(header->is_memory_card_present), 1 );
    memcpy( &raw_data[152], &(header->is_memory_card_blank), 1 );
    memcpy( &raw_data[153], &(header->nb_bongos), 1 );
    memcpy( &raw_data[154], &(header->is_sync_gpu_thread_enabled), 1 );
    memcpy( &raw_data[169], &(header->second_iso_name), 40 );
    memcpy( &raw_data[209], &(header->sha1_git_revision), 20 );
    memcpy( &raw_data[229], &(header->dsp_irom_hash), 4 );
    memcpy( &raw_data[233], &(header->dsp_coef_hash), 4 );
    memcpy( &raw_data[237], &(header->tick_count), 8 );

    return raw_data;
}

t_dtm_header *dtm_unpack_header( char buff[256] ){
    
    t_dtm_header *head;

    head = (t_dtm_header *) malloc( sizeof( t_dtm_header ) );
    if(head == NULL){
        printf( "erreur alloc t_dtm_header\n" );
        return NULL;
    }
    memset( head, 0, sizeof(t_dtm_header) );
    strncpy( head->signature, &buff[0], 4 );
    strncpy( head->game_id, &buff[4], 6 );
    head->is_wii_game = buff[10];
    head->nb_controllers = buff[11];
    head->starts_from_savestate = buff[12];
    memcpy( &(head->vi_count), &buff[13], 8 );
    memcpy( &(head->input_count), &buff[21], 8);
    memcpy( &(head->lag_count), &buff[29], 8 );
    memcpy( &(head->rerecord_count), &buff[45], 4 );
    strncpy( head->author, &buff[49], 32);
    strncpy( head->video_backend, &buff[81], 16 );
    memcpy( &(head->md5), &buff[113], 16 );
    memcpy( &(head->record_time_start), &buff[129], 8);
    head->is_saved_config_valid = buff[137];
    head->is_idle_skipping_enabled = buff[138];
    head->is_dual_core_enabled = buff[139];
    head->is_progressive_scan_enabled = buff[140];
    head->is_dsp_hle_enabled = buff[141];
    head->is_fast_disc_enabled = buff[142];
    head->cpu_core = buff[143];
    head->is_efb_access_enabled = buff[144];
    head->is_efb_copy_enabled = buff[145];
    head->is_copy_efb_to_texture_enabled = buff[146];
    head->is_efb_copy_cache_enabled = buff[147];
    head->is_emulate_format_changes_enabled = buff[148];
    head->uses_xfb_emulation = buff[149];
    head->uses_real_xfb_emulation = buff[150];
    head->is_memory_card_present = buff[151];
    head->is_memory_card_blank = buff[152];
    head->nb_bongos = buff[153];
    head->is_sync_gpu_thread_enabled = buff[154];
    strncpy( head->second_iso_name, &buff[169], 40 );
    memcpy( &(head->sha1_git_revision), &buff[209], 20 );
    memcpy( &(head->dsp_irom_hash), &buff[229], 4 );
    memcpy( &(head->dsp_coef_hash), &buff[233], 4 );
    memcpy( &(head->tick_count), &buff[237], 8 );
   
    return head;
}

int dtm_print_header_hexa( char data[256] ){
    int i,j;
    for(i = 0 ; i < 16 ; i++){
        for(j = 0 ; j < 16 ; j++){
            printf("%02x ",(0xFF & data[ j + i*16 ] ));
        }
        printf("\n");
    }

    return 0;
}

int dtm_print_header_raw_infos( char header[256] ){
    
    t_dtm_header *head;

    head = dtm_unpack_header( header );
    if( head == NULL ){
        printf("erreur lors de l'extraction des infos du header\n");
        return -1;
    }

    dtm_print_header_infos( head );

    free( head );

    return 0;

}

int dtm_print_header_infos(  t_dtm_header *head ){

    int i;

    printf( "Signature : %.3s %d\n", &head->signature[0], head->signature[3] );
    printf( "Game id : %s\n", head->game_id);
    printf( "Is Wii game : %s\n", head->is_wii_game ? "yes" : "no");
    printf( "Nb controllers : %d\n", head->nb_controllers );
    printf( "Starts from savestate : %s\n", head->starts_from_savestate ? "yes" : "no");
    printf( "VI count : %lld\n", head->vi_count );
    printf( "input count : %lld\n", head->input_count );
    printf( "Lag count : %lld\n", head->lag_count );
    printf( "Rerecord count : %d\n", head->rerecord_count );
    printf( "Author : %s\n", head->author );
    printf( "Video backend : %s\n", head->video_backend );

    printf( "Game MD5 : " );
    for(i=0; i<16; ++i)
        printf("%02x ", (0xFF & head->md5[i]) );
    printf("\n");

    printf( "Start timestamp : %lld\n", head->record_time_start );
    printf( "Is saved config valid : %s\n", head->is_saved_config_valid ? "yes" : "no");
    printf( "Idle skipping : %s\n", head->is_idle_skipping_enabled ? "yes" : "no");
    printf( "Dual core : %s\n", head->is_dual_core_enabled ? "yes" : "no");
    printf( "Progressive scan : %s\n", head->is_progressive_scan_enabled ? "yes" : "no");
    printf( "DSP HLE : %s\n", head->is_dsp_hle_enabled ? "yes" : "no");
    printf( "Fast disc : %s\n", head->is_fast_disc_enabled ? "yes" : "no");
    printf( "CPU core : %d\n", head->cpu_core);
    printf( "EFB access : %s\n", head->is_efb_access_enabled ? "yes" : "no");
    printf( "EFB copy : %s\n", head->is_efb_copy_enabled? "yes" : "no");
    printf( "Copy EFB to texture : %s\n", head->is_copy_efb_to_texture_enabled ? "yes" : "no");
    printf( "EFB copy cache : %s\n", head->is_efb_copy_cache_enabled ? "yes" : "no");
    printf( "Emulate format changes : %s\n", head->is_emulate_format_changes_enabled ? "yes" : "no");
    printf( "XFB emulation : %s\n", head->uses_xfb_emulation ? "yes" : "no");
    printf( "Real XFB emulation : %s\n", head->uses_real_xfb_emulation ? "yes" : "no");
    printf( "Memory card : %s\n", head->is_memory_card_present? "yes" : "no");
    printf( "Memory card blank : %s\n", head->is_memory_card_blank ? "yes" : "no");
    printf( "Nb bongos : %d\n", head->nb_bongos);
    printf( "Sync GPU thread : %s\n", head->is_sync_gpu_thread_enabled ? "yes" : "no");
    printf( "Second ISO name : %s\n", head->second_iso_name);
    
    printf( "Git revision : " );
    for(i=0; i<20; ++i)
        printf("%02x ", (0xFF & head->sha1_git_revision[i]) );
    printf("\n");

    printf( "DSP IROM hash : %u\n", head->dsp_irom_hash );
    printf( "DSP coef hash%u\n", head->dsp_coef_hash );
    printf( "Tick count : %lld\n", head->tick_count );

    return 0;
}

int dtm_print_inputs(  char *dump_name ){

    FILE *fp;
    t_dtm_header *head;
    int n,i;

    fp = fopen( dump_name, "r" );
    if( fp == NULL ){
        printf("erreur a l'ouverture du fichier %s. abort. \n", dump_name);
        return -1;
    }
   
    head = dtm_consume_header( fp );

    char data[8];
    while( !feof( fp ) ){
        n = fread( data, sizeof(char), 8, fp );
        if(n != 8){
            printf("Incomplete line\n");
        }
        for(i = 0; i < 8 ; i++){
            printf("%02x ", (0xFF & data[i] ) );
        }
        printf("\n");
    }

    free(head);
    fclose(fp);

    return 0;


}
