#include <stdio.h>
#include<string.h>
#include<stdlib.h>
#include "encode.h"
#include "types.h"
#include"common.h"

OperationType check_operation_type(char *argv[])
{
    if(argv[1] == NULL)
    {
        return e_unsupported;
    }
    else if(strcmp(argv[1],"-e") == 0)
    {
        return e_encode;
    }
    else if(strcmp(argv[1],"-d") == 0)
    {
        return e_decode;
    }
    else
    {
        return e_unsupported;
    }
}

Status do_encoding(EncodeInfo *encInfo)
{
    /* opening given files */
    Status file_ptr = open_files(encInfo);
    if (file_ptr == e_failure)
    {
        printf("ERROR: %s function failed\n", "open_files" );
        return e_failure;
    }            
    else
    {
        printf("SUCCESS: %s function completed\n", "open_files" );
    }
    
    
    /*check capacity */
    Status capacity = check_capacity(encInfo);

    if(capacity == e_failure)
    {

        printf("Done. found OKAY\n");
        printf("%s file is too large to encode in %s \n",encInfo->secret_fname,encInfo->src_image_fname);
        return e_failure;
    }

    printf("Done. found OKAY\n");


    /*Copying header file*/

    copy_bmp_header(encInfo->fptr_src_image,encInfo->fptr_stego_image);

    /*encoding magic string*/
    encode_magic_string(MAGIC_STRING,encInfo);


    /*Encoding secret file extension */
    const char *file_extn = strstr(encInfo->secret_fname,".");
    encode_secret_file_extn(file_extn,encInfo);


    /*Encoding secret file size*/

    encode_secret_file_size(encInfo->secret_file_capacity,encInfo);

    /*Encoding secret file data*/

    encode_secret_file_data(encInfo);

    return e_success;
}

int main(int argc, char *argv[])
{

    EncodeInfo *encInfo = malloc(sizeof(EncodeInfo));

    uint img_size;


        

    if(check_operation_type(argv) == e_encode)
    {
        printf("Encode\n");
        if(argc > 5 || argc < 4)
        {
            printf("Error : Please pass correct arguments\n");
            printf("usage : ./a.out -e <file_name>.bmp <file_name>.txt [<output_file_name>.bmp]\n");
            return e_failure;
        }
        if(argc >= 4)
        {

            Status validate_argu = read_and_validate_encode_args(argc,argv,encInfo);

            if(validate_argu == e_failure)
            {
                
                printf("Error : Please pass correct arguments\n");
                printf("usage : ./a.out -e <file_name>.bmp <file_name>.txt [<output_file_name>.bmp]\n");
                return e_failure;
            }

            /*all arguments passed are valid perform the encoding  */
            printf("##    Encoding procedure started     ##\n");

            if(do_encoding(encInfo) == e_success)
            {
                printf("Encoding done successfully\n");
            }


            
        }

    }
    else if(check_operation_type(argv) == e_decode)
    {
        printf("Decode\n");
    }
    else if(check_operation_type(argv) == e_unsupported)
    {
        printf("Error : Invalid arguments\n");
        printf("usage : ./a.out -e <file_name>.bmp <file_name>.txt [<output_file_name>.bmp]\n");
    }


    
}
