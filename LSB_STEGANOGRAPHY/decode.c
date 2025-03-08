#include <stdio.h>
#include <string.h>
#include "common.h"
#include "types.h"
#include "decode.h"

Status read_and_validate_decode_args(char *argv[], DecodeInfo *decInfo)
{
    if (strcmp(strstr(argv[2], "."), ".bmp") == 0)
    {
        decInfo->stego_fname = argv[2];
    }
    else
    {
        return e_failure;
    }

    if (argv[3] != NULL)
    {
        decInfo->decode_fname = argv[3];
    }
    else
    {
        strcpy(decInfo->decode_fname, "decode");
    }

    return e_success;
}

Status open_decode_files(DecodeInfo *decInfo)
{
    decInfo->fptr_stego = fopen(decInfo->stego_fname, "r");
    if (decInfo->fptr_stego == NULL)
    {
        return e_failure;
    }
    return e_success;
}

char decode_lsb_to_char(DecodeInfo *decInfo)
{
    char buffer[8];
    fread(buffer, 8, 1, decInfo->fptr_stego);
    char ch = 0;
    for (int i = 0; i < 8; i++)
    {
        ch = ch | (buffer[i] & 1);
        if (i != 7)
            ch = ch << 1;
    }
    return ch;
}

Status decode_magic_string(DecodeInfo *decInfo)
{
    int magic_len = strlen(MAGIC_STRING);
    fseek(decInfo->fptr_stego, 54, SEEK_SET);
    char magic_str[5];
    int i;
    for (i = 0; i < magic_len; i++)
    {
        magic_str[i] = decode_lsb_to_char(decInfo);
    }
    magic_str[i] = '\0';
    if (strcmp(magic_str, MAGIC_STRING) == 0)
    {
        return e_success;
    }
    else
    {
        return e_failure;
    }
}

int decode_lsb_to_int(DecodeInfo *decInfo)
{
    char intbuffer[32];
    int size = 0;
    fread(intbuffer, 32, 1, decInfo->fptr_stego);
    for (int i = 0; i < 32; i++)
    {
        size = size | (intbuffer[i] & 1);
        if (i != 31)
            size = size << 1;
    }
    return size;
}

Status decode_secret_file_extn_size(DecodeInfo *decInfo)
{
    decInfo->extn_size = decode_lsb_to_int(decInfo);
    return e_success;
}

Status decode_secret_extn(DecodeInfo *decInfo)
{
    char extention[decInfo->extn_size + 1];
    for (int i = 0; i < decInfo->extn_size; i++)
    {
        extention[i] = decode_lsb_to_char(decInfo);
    }
    extention[decInfo->extn_size] = '\0';
    char *temp = strstr(decInfo->decode_fname, ".");
    if (temp != NULL)
    {
        int i = 0;
        char str[100];
        while (decInfo->decode_fname[i] != '.')
        {
            str[i] = decInfo->decode_fname[i];
            i++;
        }
        str[i] = '\0';
        strcpy(decInfo->decode_fname, str);
    }
    strcat(decInfo->decode_fname, extention);

    decInfo->fptr_decode = fopen(decInfo->decode_fname, "w");
    if (decInfo->fptr_decode == NULL)
    {
        return e_failure;
    }

    return e_success;
}

Status decode_secret_file_size(DecodeInfo *decInfo)
{
    decInfo->size_secretfile = decode_lsb_to_int(decInfo);
    return e_success;
}

Status decode_secret_file_data(DecodeInfo *decInfo)
{
    char ch;
    for (int i = 0; i < decInfo->size_secretfile; i++)
    {
        ch = decode_lsb_to_char(decInfo);
        fwrite(&ch, 1, 1, decInfo->fptr_decode);
    }
    return e_success;
}

Status do_decoding(DecodeInfo *decInfo)
{
    if (open_decode_files(decInfo) == e_success)
    {
        if (decode_magic_string(decInfo) == e_success)
        {
            if (decode_secret_file_extn_size(decInfo) == e_success)
            {
                if (decode_secret_extn(decInfo) == e_success)
                {
                    if (decode_secret_file_size(decInfo) == e_success)
                    {
                        if (decode_secret_file_data(decInfo) == e_success)
                        {
                            return e_success;
                        }
                    }
                }
            }
        }
    }
    return e_failure;
}