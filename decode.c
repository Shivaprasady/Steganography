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
    {/*
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
*/
        decInfo->output_file_fname = argv[3];
    }

    if(argv[3] == NULL)
    {
        decInfo->output_file_fname = "NULL";

    }
    return e_success;
}


/* Opening input and output files*/
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

    /*
    // Opening output file
    if(strcmp(decInfo->output_file_fname,"NULL") == 0)
    {
        return e_success;
    }
    
    decInfo->fptr_output_file  = fopen(decInfo->output_file_fname, "w");

    if(decInfo->fptr_output_file == NULL)
    {
        printf("Error : Unable to open file %s\n",decInfo->output_file_fname);
    
        return e_failure;
    }
*/
    return e_success;

}


/*Skip the first 54 bytes(header) of BMP file*/
Status skip_header(FILE *fptr)
{
    fseek(fptr, 54, SEEK_SET);
}


/*decoding magic string from image*/
Status decode_magic_string(DecodeInfo *decInfo)
{
    printf("Decoding magic string signature\n");
    char magic_string[2];
    if(decode_data_from_image(decInfo->fptr_encoded_image,2,&magic_string) == e_success)
    {
        if(strcmp(magic_string,"#*") == 0)
        {
            printf("Done\n");
        }
    }


}

/*  Decoding file extension   */
Status decode_extn_data(DecodeInfo *decInfo)
{
    int size;
    char arr[32];
    fread(arr,32,1,decInfo->fptr_encoded_image);

    //  Extension size
    decode_extn_size(arr,&size);

    size++;                         //To add null character at last.
    char extension[size];

    decode_data_from_image(decInfo->fptr_encoded_image,size-1,&extension);

    extension[size-1] = '\0';



    /* Opening remaining required files */

    printf("Opening remaining required files \n");

    if(strcmp(decInfo->output_file_fname,"NULL") == 0)
    {
        char output[13+size]; 
        strcpy(output,"secret_output");
    
        for(int i = 13 ,j = 0; i < (13+size) ; i++,j++)
        {
            output[i] = extension[j];
        }

        decInfo->output_file_fname = output;

        decInfo->fptr_output_file = fopen(decInfo->output_file_fname, "w");
        if(decInfo->fptr_output_file == NULL)
        {
            printf("Error : Unable to open file %s\n",decInfo->output_file_fname);    
            return e_failure;
        }


        decInfo->fptr_output_file = fopen(decInfo->output_file_fname, "w");
        printf("Opened %s file\n",decInfo->output_file_fname);
        if(decInfo->fptr_output_file == NULL)
        {
            printf("Error : Unable to open file %s\n",decInfo->output_file_fname);    
            return e_failure;
        }
    }
    else
    {
        char *temp;

        temp = strstr(decInfo->output_file_fname,".");

        if(strcmp(temp,extension) == 0)
        {
            decInfo->fptr_output_file  = fopen(decInfo->output_file_fname, "w");

            if(decInfo->fptr_output_file == NULL)
            {
                printf("Error : Unable to open file %s\n",decInfo->output_file_fname);    
                return e_failure;
            }
            return e_success;
        }

        char c = '.';
        int pos = strchr(decInfo->output_file_fname,c) - decInfo->output_file_fname;



        char output[pos+size];
        strcpy(output,decInfo->output_file_fname);
        for(int i = pos, j = 0; i < (pos+size+1) ; i++,j++)
        {
            output[i] = extension[j];
        
        }


        decInfo->output_file_fname = output;
        
        decInfo->fptr_output_file = fopen(decInfo->output_file_fname, "w");
        //printf("Opened %s file\n",decInfo->output_file_fname);
        if(decInfo->fptr_output_file == NULL)
        {
            printf("Error : Unable to open file %s\n",decInfo->output_file_fname);    
            return e_failure;
        }
    }


    printf("Done\n");

    return e_success;

}




/*  Decoding file extension size    */
Status decode_extn_size(char *arr,int *size)
{
    *size = 0;
    int mask = 0;
    for(int i = 0 , j = 31; i < 32 ; i++,j--)
    {
        mask = arr[i] & 1;
        //printf("%d\n",mask);
        *size = (*size) | (mask << j);
    }

}


/*decode secret file data*/
Status decode_secret_file_data(DecodeInfo *decInfo)
{

    /*Decoding file size */
    int size;
    char arr[32];


    printf("Decoding secret file size \n");

    fread(arr,32,1,decInfo->fptr_encoded_image);
    decode_extn_size(arr,&size);                    //code for decoding size both extension and secret file size is same

    printf("Done\n");


    /*decoding secret message to output file */
    printf("Decoding secret file data\n");
    char secret_data[size];
    if(decode_data_from_image(decInfo->fptr_encoded_image,size,&secret_data) == e_success)
    {
        fwrite(secret_data,1,sizeof(secret_data),decInfo->fptr_output_file);
    }
    printf("Done\n");

    return e_success;
    
}




Status decode_data_from_image(FILE *fptr_encoded_image,int size,char *data)
{
    char arr[8];
    char temp = 0;
    for(int i = 0 ; i < size ; i++)
    {
        fread(arr,8,1,fptr_encoded_image);
        decode_bit_from_byte(arr,&temp);
        data[i] = temp;
    }
    return e_success;
}

Status decode_bit_from_byte(char *arr,char *temp)
{
    *temp = 0;
    unsigned char mask;
    for(int i = 0 ,j = 7; i < 8 ; i++,j--)
    {
        mask = arr[i] & 1;
        *temp = (*temp) | (mask << j);
    }

    return e_success;
}
