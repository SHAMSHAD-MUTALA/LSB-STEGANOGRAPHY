#ifndef DECODE_H
#define DECODE_H

#include "types.h" // Contains user-defined types
#include "common.h"

#define MAX_SECRET_BUF_SIZE 1
#define MAX_IMAGE_BUF_SIZE (MAX_SECRET_BUF_SIZE * 8)
#define MAX_FILE_SUFFIX 4

typedef struct _DecodeInfo
{
    /* Source Image info */
    char *stego_fname;
    FILE *fptr_stego;
    char image_data[MAX_IMAGE_BUF_SIZE];

    /* Decode File Info */
    char *decode_fname;
    FILE *fptr_decode;
    int extn_size;
    uint size_secretfile;

} DecodeInfo;

/* Decoding function prototypes */

/* Check operation type */
OperationType check_operation_type(char *argv[]);

/* Read and validate Decode args from argv */
Status read_and_validate_decode_args(char *argv[], DecodeInfo *decInfo);

/* Perform the decoding */
Status do_decoding(DecodeInfo *decInfo);

/* Get File pointers for i/p and o/p files */
Status open_decode_files(DecodeInfo *decInfo);

/* Decode Magic String */
Status decode_magic_string(DecodeInfo *decInfo);

/* Decode Secret file Extention Size*/
Status decode_secret_file_extn_size(DecodeInfo *decInfo);

/*Decode secret file extention */
Status decode_secret_extn(DecodeInfo *decInfo);

/*Decode secret file size*/
Status decode_secret_file_size(DecodeInfo *decInfo);

/*Decode secret file data*/
Status decode_secret_file_data(DecodeInfo *decInfo);

#endif // DECODE_H