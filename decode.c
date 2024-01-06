#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include"decode.h"
#include"types.h"
#include"common.h"



/* Read and validating file passed */
Status read_and_validate_decode_args(char *argv[],DecodeInfo *decInfo)
{
    char *temp;
    int extension_check;

    temp = strstr(argv[2],".bmp");
    if(temp == NULL)
    {
        return e_failure;
    }

    extension_check = strcmp(temp,".bmp");
    if(extension_check != 0)
    {
        return e_failure;
    }

    decInfo->encoded_image_fname = argv[2];

    if(argv[3] != NULL)
    {
        temp = strstr(argv[3],".txt");
        if(temp == NULL)
        {
            return e_failure;
        }

        extension_check = strcmp(temp,".txt");
        if(extension_check != 0)
        {
            return e_failure;
        }

        decInfo->output_file_fname = argv[3];
    }

    if(argv[3] == NULL)
    {
        decInfo->output_file_fname = "Secret_file.txt";

    }

    return e_success;
}

Status open_decode_files(DecodeInfo *decInfo)
{
    printf("Opening required files\n");


    /*Opening source file(Encoded image)*/

    printf("Opening %s file\n",decInfo->encoded_image_fname);
    decInfo->fptr_encoded_image = fopen(decInfo->encoded_image_fname, "r");

    if(decInfo->fptr_encoded_image == NULL)
    {
        printf("Error : Unable to open file %s\n",decInfo->encoded_image_fname);

        return e_failure;
    }

    // Opening output file
    decInfo->fptr_output_file  = fopen(decInfo->output_file_fname, "w");

    if(decInfo->fptr_output_file == NULL)
    {
        printf("Error : Unable to open file %s\n",decInfo->output_file_fname);
    
        return e_failure;
    }



}
