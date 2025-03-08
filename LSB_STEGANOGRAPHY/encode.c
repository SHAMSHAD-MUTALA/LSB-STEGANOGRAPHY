#include <stdio.h>
#include <string.h>
#include "encode.h"
#include "common.h"
#include "types.h"

/* Function Definitions */

OperationType check_operation_type(char *argv[])
{
    return strcmp(argv[1], "-e") == 0 ? e_encode : strcmp(argv[1], "-d") == 0 ? e_decode : e_unsupported;
}

Status read_and_validate_encode_args(char *argv[], EncodeInfo *encInfo)
{
    if (strcmp(strstr(argv[2], "."), ".bmp") == 0)
    {
        encInfo->src_image_fname = argv[2];
    }
    else
    {
        return e_failure;
    }

    char *temp = strstr(argv[3], ".");
    strcpy(encInfo->extn_secret_file, temp);
    if (strcmp(temp, ".txt") == 0)
    {
        encInfo->secret_fname = argv[3];
    }
    else if (strcmp(temp, ".sh") == 0)
    {
        encInfo->secret_fname = argv[3];
    }
    else if (strcmp(temp, ".c") == 0)
    {
        encInfo->secret_fname = argv[3];
    }
    else
    {
        return e_failure;
    }
    if (argv[4] != NULL)
    {
        if ((strcmp(strstr(argv[4], "."), ".bmp")) == 0)
        {
            encInfo->stego_image_fname = argv[4];
        }
        else
            return e_failure;
    }
    else
        encInfo->stego_image_fname = "stego.bmp";

    return e_success;
}

uint get_image_size_for_bmp(FILE *fptr_image)
{
    uint width, height;
    fseek(fptr_image, 18, SEEK_SET);
    fread(&width, sizeof(int), 1, fptr_image);
    fread(&height, sizeof(int), 1, fptr_image);
    return width * height * 3;
}

Status check_capacity(EncodeInfo *encInfo)
{
    int len = strlen(MAGIC_STRING);

    encInfo->image_capacity = get_image_size_for_bmp(encInfo->fptr_src_image);

    uint size_secret_file = get_file_size(encInfo->fptr_secret);
    encInfo->size_secret_file = size_secret_file;

    uint encode_size = 54 + (len + 4 + 4 + strlen(encInfo->extn_secret_file) + size_secret_file) * 8;

    if (encInfo->image_capacity > encode_size)
    {
        return e_success;
    }
    else
    {
        return e_failure;
    }
}

uint get_file_size(FILE *fptr)
{
    fseek(fptr, 0, SEEK_END);
    int file_size = ftell(fptr);
    fseek(fptr, 0, SEEK_SET);
    return file_size;
}

Status copy_bmp_header(FILE *fptr_src_image, FILE *fptr_dest_image)
{
    char buffer[54];
    rewind(fptr_src_image);
    rewind(fptr_dest_image);

    fread(buffer, 54, 1, fptr_src_image);
    fwrite(buffer, 54, 1, fptr_dest_image);

    return e_success;
}

Status encode_magic_string(const char *magic_string, EncodeInfo *encInfo)
{
    int magicstr_len = strlen(MAGIC_STRING);
    encode_data_to_image(magic_string, magicstr_len, encInfo->fptr_src_image, encInfo->fptr_stego_image);
    return e_success;
}

Status encode_data_to_image(const char *data, int size, FILE *fptr_src_image, FILE *fptr_stego_image)
{
    char image_buffer[8];
    for (int i = 0; i < size; i++)
    {
        fread(image_buffer, 8, 1, fptr_src_image);
        encode_byte_to_lsb(data[i], image_buffer);
        fwrite(image_buffer, 8, 1, fptr_stego_image);
    }
    return e_success;
}

Status encode_byte_to_lsb(char data, char image_buffer[])
{
    for (int i = 7; i >= 0; i--)
    {
        int get = (data & 1 << i) >> i;
        image_buffer[7 - i] = image_buffer[7 - i] & (~1);
        image_buffer[7 - i] = image_buffer[7 - i] | get;
    }
    return e_success;
}

Status encode_file_extn_size(char *file_extn, EncodeInfo *encInfo)
{
    char image_buffer[32];
    fread(image_buffer, 32, 1, encInfo->fptr_src_image);
    long size = strlen(file_extn);
    encode_int_to_lsb(size, image_buffer);
    fwrite(image_buffer, 32, 1, encInfo->fptr_stego_image);
    return e_success;
}

Status encode_int_to_lsb(long size, char image_buffer[])
{
    for (int i = 31; i >= 0; i--)
    {
        int get = (size & 1 << i) >> i;
        image_buffer[31 - i] = image_buffer[31 - i] & (~1);
        image_buffer[31 - i] = image_buffer[31 - i] | get;
    }
    return e_success;
}

Status encode_secret_file_extn(const char *file_extn, EncodeInfo *encInfo)
{
    encode_data_to_image(file_extn, strlen(file_extn), encInfo->fptr_src_image, encInfo->fptr_stego_image);
    return e_success;
}

Status encode_secret_file_size(uint size_secret_file, EncodeInfo *encInfo)
{
    char image_buffer[32];
    fread(image_buffer, 32, 1, encInfo->fptr_src_image);
    encode_int_to_lsb(size_secret_file, image_buffer);
    fwrite(image_buffer, 32, 1, encInfo->fptr_stego_image);
    return e_success;
}

Status encode_secret_file_data(EncodeInfo *encInfo)
{
    char sec_file[encInfo->size_secret_file];
    rewind(encInfo->fptr_secret);
    fread(sec_file, encInfo->size_secret_file, 1, encInfo->fptr_secret);

    if (encode_data_to_image(sec_file, encInfo->size_secret_file, encInfo->fptr_src_image, encInfo->fptr_stego_image) == e_success)
    {
        return e_success;
    }
    return e_failure;
}

Status copy_remaining_img_data(FILE *fptr_src_image, FILE *fptr_stego_image)
{
    char ch;
    while (fread(&ch, 1, 1, fptr_src_image) != 0)
    {
        fwrite(&ch, 1, 1, fptr_stego_image);
    }
    return e_success;
}

Status open_files(EncodeInfo *encInfo)
{
    encInfo->fptr_src_image = fopen(encInfo->src_image_fname, "r");
    if (encInfo->fptr_src_image == NULL)
    {
        return e_failure;
    }

    encInfo->fptr_secret = fopen(encInfo->secret_fname, "r");
    if (encInfo->fptr_secret == NULL)
    {
        return e_failure;
    }

    encInfo->fptr_stego_image = fopen(encInfo->stego_image_fname, "w");
    if (encInfo->fptr_stego_image == NULL)
    {
        return e_failure;
    }

    return e_success;
}

Status do_encoding(EncodeInfo *encInfo)
{
    if (check_capacity(encInfo) == e_success)
    {
        if (copy_bmp_header(encInfo->fptr_src_image, encInfo->fptr_stego_image) == e_success)
        {
            if (encode_magic_string(MAGIC_STRING, encInfo) == e_success)
            {
                if (encode_file_extn_size(encInfo->extn_secret_file, encInfo) == e_success)
                {
                    if (encode_secret_file_extn(encInfo->extn_secret_file, encInfo) == e_success)
                    {
                        if (encode_secret_file_size(encInfo->size_secret_file, encInfo) == e_success)
                        {
                            if (encode_secret_file_data(encInfo) == e_success)
                            {
                                if (copy_remaining_img_data(encInfo->fptr_src_image, encInfo->fptr_stego_image) == e_success)
                                {
                                    return e_success;
                                }
                            }
                        }
                    }
                }
            }
        }
    }
    return e_failure;
}