#include <stdio.h>
#include<string.h>
#include<stdlib.h>
#include "encode.h"
#include "types.h"
#include"common.h"

#include"decode.h"

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

    /*Copy remaining image bytes from src to stego after encoding*/

    copy_remaining_img_data(encInfo->fptr_src_image,encInfo->fptr_stego_image);


    return e_success;
}


/* Decoding image */
Status do_decoding(DecodeInfo *decInfo)
{
    if(open_decode_files(decInfo) == e_success)
    {
        printf("Done\n");
    }
    else{
        return e_failure;
    }

    rewind(decInfo->fptr_encoded_image);

    if(skip_header(decInfo->fptr_encoded_image) == e_failure)
    {
        return e_failure;
    }

    if(decode_magic_string(decInfo) == e_failure)
    {
        return e_failure;
    }

    if(decode_extn_data(decInfo) == e_failure)
    {
        return e_failure;
    }

    if(decode_secret_file_data(decInfo) == e_failure)
    {
        return e_failure;
    }

    


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

        DecodeInfo *decInfo = malloc(sizeof(DecodeInfo));
        
        
        printf("Decode\n");

        if(argc < 3 || argc > 4)
        {
            printf("Error : Please pass correct arguments\n");
            printf("Usage : ./a.out -d <file_name>.bmp [<file_name>.txt]\n");
        }
        else if( read_and_validate_decode_args(argv,decInfo) == e_failure )
        {
            
            printf("Error : Please pass correct arguments\n");
            printf("Usage : ./a.out -d <file_name>.bmp [<file_name>.txt]\n");
            return e_failure;

        }

        if(do_decoding(decInfo) == e_success)
        {
            printf("##  Decoding done successfully  ##\n");
        }



    }
    else if(check_operation_type(argv) == e_unsupported)
    {
        printf("Error : Invalid arguments\n");
        printf("usage : ./a.out -e <file_name>.bmp <file_name>.txt [<output_file_name>.bmp]\n");
    }


    
}
