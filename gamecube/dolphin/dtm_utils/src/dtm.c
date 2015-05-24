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

t_dtm_header *read_header_from_dump( const char *dump_name ){

    FILE *fp;
    int n, nb_read = 0;
    char header_buffer[255];
    t_dtm_header *head;

    fp = fopen( dump_name, "r" );
    if( fp == NULL ){
        printf("erreur a l'ouverture du fichier %s. abort. \n", dump_name);
        return NULL;
    }
   
    do{
        n = fread( &header_buffer[nb_read], sizeof(char), 255, fp );
        if( n < 0 ){
            printf("erreur pendant la lecture du fichier. \n");
            return NULL;
        }
        nb_read += n;
    }while( nb_read < 255 );

    fclose(fp);

    head = extract_header_from_raw_data( header_buffer );

    return head;

}

t_dtm_header *extract_header_from_raw_data( char buff[255] ){
    
    t_dtm_header *head;

    head = (t_dtm_header *) malloc( sizeof( t_dtm_header ) );
    if(head == NULL){
        printf( "erreur alloc t_dtm_header\n" );
        return NULL;
    }

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

int print_header_hexa( char data[255] ){
    int i,j;
    for(i = 0 ; i < 16 ; i++){
        for(j = 0 ; j < 16 ; j++){
            printf("%02x ",(0xFF & data[ j + i*16 ] ));
        }
        printf("\n");
    }

    return 0;
}

int print_header_raw_infos( char header[255] ){
    
    t_dtm_header *head;

    head = extract_header_from_raw_data( header );
    if( head == NULL ){
        printf("erreur lors de l'extraction des infos du header\n");
        return -1;
    }

    print_header_infos( head );

    free( head );

    return 0;

}

int print_header_infos( const t_dtm_header *head ){

    int i;

    printf( "Signature : %.3s %d\n", &head->signature[0], head->signature[3] );
    printf( "Game id : %s\n", head->game_id);
    printf( "Is Wii game : %s\n", head->is_wii_game ? "yes" : "no");
    printf( "Nb controllers : %d\n", head->nb_controllers );
    printf( "Starts from savestate : %s\n", head->starts_from_savestate ? "yes" : "no");
    printf( "VI count : %lld\n", head->vi_count );
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

