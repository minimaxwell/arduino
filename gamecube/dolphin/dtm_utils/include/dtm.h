#ifndef __DTM_H__
#define __DTM_H__

#include <stdint.h>
#include <stdio.h>
/**
 * M.Chevallier
 * lib to manipulate DTM format used by the dolphin emulator
 * */

/**
 * --------------------------- Data structures --------------------------------------
 */

typedef struct {
    char signature[4];
    char game_id[6];
    uint8_t is_wii_game;
    int8_t nb_controllers;
    uint8_t starts_from_savestate;
    long long int vi_count;
    long long int input_count;
    long long int lag_count;
    int32_t rerecord_count;
    char author[32];
    char video_backend[16];
    uint8_t md5[16];
    long long int record_time_start;
    uint8_t is_saved_config_valid;
    uint8_t is_idle_skipping_enabled;
    uint8_t is_dual_core_enabled;
    uint8_t is_progressive_scan_enabled;
    uint8_t is_dsp_hle_enabled;
    uint8_t is_fast_disc_enabled;
    int8_t cpu_core;
    uint8_t is_efb_access_enabled;
    uint8_t is_efb_copy_enabled;
    uint8_t is_copy_efb_to_texture_enabled;
    uint8_t is_efb_copy_cache_enabled;
    uint8_t is_emulate_format_changes_enabled;
    uint8_t uses_xfb_emulation;
    uint8_t uses_real_xfb_emulation;
    uint8_t is_memory_card_present;
    uint8_t is_memory_card_blank;
    int8_t nb_bongos;
    uint8_t is_sync_gpu_thread_enabled;
    char second_iso_name[40];
    uint8_t sha1_git_revision[20];
    int32_t dsp_irom_hash;
    int32_t dsp_coef_hash;
    long long int tick_count;
} t_dtm_header;
/**
 * --------------------------- Write functions --------------------------------------
 * */
 int write_header_from_existing_dump( FILE *dest_file, const char *existing_dump );

 int write_header_to_file( FILE *dest_file, const t_dtm_header header );

 /**
  * ------------------------- Read functions ----------------------------------------
  * */

 /**
  * @brief returns an allocated header data struct read from an existing dtm file.
  */
 t_dtm_header *read_header_from_dump( const char *dump_name );

 t_dtm_header *extract_header_from_raw_data( char data[255] );

 /**
  * --------------------------- Debug functions -------------------------------------
  * */

/**
 * @brief Prints the given in stdout as raw hexa data
 * @param header the raw header data to print
 * @return 0 if no err, <0 else
 * */
int print_header_hexa( char header[255] );


/**
 * @brief Decodes the header and print its content in stdout
 * @param header the header to decode and print
 * @return 0 if no err, <0 else
 * */
int print_header_raw_infos( char header[255] );

int print_header_infos( const t_dtm_header *header );

#endif
