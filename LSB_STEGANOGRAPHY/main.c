#include <stdio.h>
#include <string.h>
#include "encode.h"
#include "decode.h"
#include "types.h"

int main(int argc, char *argv[])
{
    // Check operation type
    if (argc < 3)
    {
        printf("Usage:\n");
        printf("Encoding: ./a.out -e beautiful.bmp secret.txt stego_img.bmp\n");
        printf("Decoding: ./a.out -d stego_img.bmp output.txt\n");
        return 1;
    }

    OperationType optype = check_operation_type(argv[1]);
    
    if (optype == e_encode)
    {
        EncodeInfo encInfo;
        if (read_and_validate_encode_args(argv, &encInfo) == e_success)
        {
            printf("Starting Encoding\n");
            if (do_encoding(&encInfo) == e_success)
            {
                printf("Encoding completed successfully\n");
            }
            else
            {
                printf("Encoding failed\n");
                return 1;
            }
        }
        else
        {
            printf("Encoding validation failed\n");
            return 1;
        }
    }
    else if (optype == e_decode)
    {
        DecodeInfo decInfo;
        if (read_and_validate_decode_args(argv, &decInfo) == d_success)
        {
            printf("Starting Decoding\n");
            if (do_decoding(&decInfo) == d_success)
            {
                printf("Decoding completed successfully\n");
            }
            else
            {
                printf("Decoding failed\n");
                return 1;
            }
        }
        else
        {
            printf("Decoding validation failed\n");
            return 1;
        }
    }
    else
    {
        printf("Unsupported operation\n");
        return 1;
    }

    return 0;
}