#include <stdio.h>
#include "encode.h"
#include "decode.h"
#include "types.h"

int main(int argc, char *argv[])
{
    if (argc < 2)
    {
        printf("Please pass the proper Arguments\n");
    }
    else if (argc < 3)
    {
        printf("Error: Unable To Perform Operation. Arguments Should be > 3\n");
        printf("For Encoding Arguments Should be:\n./a.out -e <sourcefile.bmp> <secretfile.txt> [outputfile.bmp]\n");
        printf("For Decoding Arguments should be:\n./a.out -d <sourcefile.bmp> [outputfile.txt]\n");
    }
    else
    {
        int type = check_operation_type(argv);
        if (type == e_encode)
        {
            printf("You have chosen Encoding\n");

            EncodeInfo encInfo;
            uint img_size;

            if (read_and_validate_encode_args(argv, &encInfo) == e_success)
            {
                if (open_files(&encInfo) == e_failure)
                {
                    return 1;
                }
                else
                {
                    if (do_encoding(&encInfo) == e_success)
                    {
                        printf("Encoding is Successfull !\n");
                    }
                    else
                    {
                        printf("ERROR: Encoding failed\n");
                    }
                }
            }
        }
        else if (type == e_decode)
        {
            printf("You have chosen Decoding\n");
            DecodeInfo decInfo;

            if (read_and_validate_decode_args(argv, &decInfo) == e_success)
            {
                if (do_decoding(&decInfo) == e_success)
                {
                    printf("Decoding is Successfull !\n");
                }
                else
                {
                    printf("Decoding Failed\n");
                }
            }
        }
        else
        {
            printf("Error: Unable To Perform Operation. Arguments Should be > 3\n");
            printf("For Encoding Arguments Should be:\n./a.out -e <sourcefile.bmp> <secretfile.txt> [outputfile.bmp]\n");
            printf("For Decoding Arguments should be:\n./a.out -d <sourcefile.bmp> [outputfile.txt]\n");
        }
    }
    return 0;
}