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


/**
 *Structure repressnting input data. It does take more place than the packed version, 
 * but it helps for conversion and readability.
 * */
typedef struct {
    uint8_t b_start;    // start button
    uint8_t b_a;        // A button
    uint8_t b_b;        // B button
    uint8_t b_x;        // X button
    uint8_t b_y;        // Y button
    uint8_t b_z;        // Z button
    uint8_t p_up;       // D-pad up
    uint8_t p_down;     // D-pad down
    uint8_t p_left;     // D-pas left
    uint8_t p_right;    // D-pad right
    uint8_t b_left;     // Left trigger end-of-course button
    uint8_t b_right;    // Right trigger end-of-course button
    uint8_t change_cd;  // Change CD ( ??? )
    uint8_t t_left;     // Left trigger ( 0 : not pushed, 256 : fully pushed )
    uint8_t t_right;    // Right trigger
    uint8_t ls_x;       // Left-stick X axis ( 1 : full left, 256 : full right )
    uint8_t ls_y;       // Left-stick Y axis ( 1 : full down, 256 : full up )
    uint8_t cs_x;       // C-stick X axis ( cf left stick )
    uint8_t cs_y;       // C-stick Y axis ( cf left stick )
} t_dtm_input_data;

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
 * ------------------------- Input handling functions ------------------------------
 * */

char *dtm_pack_input( t_dtm_input_data *input ); 

t_dtm_input_data *dtm_unpack_input( char data[8] );

/**
 * ------------------------- Header handling functions ------------------------------
 * */

char *dtm_pack_header( t_dtm_header *header );

t_dtm_header *dtm_unpack_header( char data[256] );

/**
 * --------------------------- Write functions --------------------------------------
 * */
// int dtm_write_header_from_existing_dump( FILE *dest_file,  char *existing_dump );

 int dtm_write_header_to_file( FILE *dest_file,  t_dtm_header *header );

 int dtm_write_raw_header_to_file( FILE *dest_file, char *raw_data );

 int dtm_write_input_data( FILE *dest_file, t_dtm_input_data *input );
 /**
  * ------------------------- Read functions ----------------------------------------
  * */

 /**
  * @brief returns an allocated header data struct read from an existing dtm file.
  */
 t_dtm_header *dtm_read_header_from_dump(  char *dump_name );

 t_dtm_header *dtm_consume_header( FILE *fp );

 t_dtm_input_data *dtm_consume_input_data( FILE *fp );
 /**
  * --------------------------- Debug functions -------------------------------------
  * */

/**
 * @brief Prints the given in stdout as raw hexa data
 * @param header the raw header data to print
 * @return 0 if no err, <0 else
 * */
int dtm_print_header_hexa( char header[256] );


/**
 * @brief Decodes the header and print its content in stdout
 * @param header the header to decode and print
 * @return 0 if no err, <0 else
 * */
int dtm_print_header_raw_infos( char header[256] );

int dtm_print_header_infos(  t_dtm_header *header );

int dtm_print_inputs(  char *dump_name );
#endif
