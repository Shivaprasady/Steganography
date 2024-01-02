#include <stdio.h>
#include<string.h>
#include "encode.h"
#include "types.h"
#include"common.h"

/* Function Definitions */

/* Get image size
 * Input: Image file ptr
 * Output: width * height * bytes per pixel (3 in our case)
 * Description: In BMP Image, width is stored in offset 18,
 * and height after that. size is 4 bytes
 */
uint get_image_size_for_bmp(FILE *fptr_image)
{
    uint width, height;
    // Seek to 18th byte
    fseek(fptr_image, 18, SEEK_SET);

    // Read the width (an int)
    fread(&width, sizeof(int), 1, fptr_image);
    printf("width = %u\n", width);

    // Read the height (an int)
    fread(&height, sizeof(int), 1, fptr_image);
    printf("height = %u\n", height);

    // Return image capacity
    return width * height * 3;
}



/* To get the size of secret file */
uint get_file_size(FILE *fptr)
{
    fseek(fptr, 0, SEEK_END);
    uint size = ftell(fptr);
    if(size == 0)
    {
        printf("File is empty\n");
    }
    printf("Done. Not empty\n");
    return size;
}



Status read_and_validate_encode_args(int argc,char *argv[], EncodeInfo *encInfo)
{
   
    char *temp;
    int extension_check;


    /*Checking 3nd argument is source.bmp file*/

    temp = strstr(argv[2],".bmp");                      //Search for .bmp in 3rd argument
    if(temp == NULL)                                    //if 3rd argument is invalid (source.bmp)
    {
        printf("Error : invalid %s\n",argv[2]);
        printf("Usage : <file.name>.bmp\n");
        return e_failure;
    }
    extension_check = strcmp(temp,".bmp");              //Comparing string 
    if(extension_check == 0)                            //3rd argument valid (source.bmp)
    {
        encInfo->src_image_fname = argv[2];

        /*Checking 4th argument is secret.txt file */

        temp = strstr(argv[3],".txt");                  //Search for .txt in 4th argument
        if(temp == NULL)                                //if 4th argument is invalid (secret.txt)
        {
            printf("Error : invalid %s\n",argv[3]);
            printf("Usage : <file.name>.txt\n");
            return e_failure;
        }
        extension_check = strcmp(temp,".txt");
        if(extension_check == 0)
        {
            encInfo->secret_fname = argv[3];


            /*checking outputfile name is passed*/ 
            
            if(argc == 4)
            {
                printf("Output file is not mentioned. Creating Steged_img.bmp as default\n");
                encInfo->stego_image_fname = "Steged_img.bmp";
                return e_success;
            }
            else
            {
                temp = strstr(argv[4],".bmp");
                if(temp == NULL)
                {                    
                    printf("Error : invalid %s\n",argv[4]);
                    printf("Usage : <file.name>.bmp\n");
                    return e_failure;
                }
                extension_check = strcmp(temp,".bmp");
                if(extension_check == 0)
                {
                    encInfo->stego_image_fname = argv[4];
                    return e_success;
                }
                else
                {
                    printf("Error : invalid %s\n",argv[4]);
                    printf("Usage : <file.name>.bmp\n");
                    return e_failure;
                }
            }

        }
        else
        {
            printf("Error : invalid %s\n",argv[3]);
            printf("Usage : <file.name>.txt\n");
        }


    }
    else                                                 //if 3rd argument is invalid (source.bmp)
    {
        printf("Error : invalid %s\n",argv[2]);
        printf("Usage : <file.name>.bmp\n");
    }
}



/* 
 * Get File pointers for i/p and o/p files
 * Inputs: Src Image file, Secret file and
 * Stego Image file
 * Output: FILE pointer for above files
 * Return Value: e_success or e_failure, on file errors
 */
Status open_files(EncodeInfo *encInfo)
{
    // Src Image file
    encInfo->fptr_src_image = fopen(encInfo->src_image_fname, "r");
    // Do Error handling
    if (encInfo->fptr_src_image == NULL)
    {
    	perror("fopen");
    	fprintf(stderr, "ERROR: Unable to open file %s\n", encInfo->src_image_fname);

    	return e_failure;
    }

    // Secret file
    encInfo->fptr_secret = fopen(encInfo->secret_fname, "r");
    // Do Error handling
    if (encInfo->fptr_secret == NULL)
    {
    	perror("fopen");
    	fprintf(stderr, "ERROR: Unable to open file %s\n", encInfo->secret_fname);

    	return e_failure;
    }

    // Stego Image file
    encInfo->fptr_stego_image = fopen(encInfo->stego_image_fname, "w");
    // Do Error handling
    if (encInfo->fptr_stego_image == NULL)
    {
    	perror("fopen");
    	fprintf(stderr, "ERROR: Unable to open file %s\n", encInfo->stego_image_fname);

    	return e_failure;
    }

    // No failure return e_success
    return e_success;
}

Status check_capacity(EncodeInfo *encInfo)
{
    /*Get image size */

    encInfo->image_capacity = get_image_size_for_bmp(encInfo->fptr_src_image);


    FILE *fptr = encInfo->fptr_secret;
    printf("Checking for %s size\n",encInfo->secret_fname);
    encInfo->secret_file_capacity = get_file_size(fptr);
    printf("Checking for %s capacity to handle %s\n",encInfo->src_image_fname,encInfo->secret_fname);
    if(encInfo->image_capacity > (54 + (encInfo->secret_file_capacity + 4 + strlen(MAGIC_STRING) + 4 ) * 8));
    {
        return e_success;
    }
    return e_failure;
}



/*Data to image*/

Status encode_data_to_image(const char *data, int size, FILE *fptr_src_image, FILE *fptr_stego_image)
{

    char arr[8];
    for(int i = 0 ; i < size ; i++)
    {
        fread(arr,8,1,fptr_src_image);
        encode_byte_to_lsb(data[i],arr);
        fwrite(arr,8,1,fptr_stego_image);
    }

    return e_success;
}



/* Encoding bytes to lsb of image bytes*/

Status encode_byte_to_lsb(char data, char *image_buffer)
{
    for(int i = 0 , j = 7; i < 8 ; i++,j--)
    {
        unsigned char mask = (data >> j) & 1;
        image_buffer[i] = (image_buffer[i] & ~1) | mask;
    }
}



/* Copying header to destination image */

Status copy_bmp_header(FILE *fptr_src_image,FILE *fptr_stego_image)
{
    printf("Copying image header\n");

    char byte;
    for(int i = 0 ; i < 54 ; i++)
    {
        byte = getc(fptr_src_image);
        putc(byte,fptr_stego_image);
    }

    printf("Done\n");

    return e_success;
}


/* Copying magic string to destination image */
Status encode_magic_string(const char *magic_string,EncodeInfo *encInfo)
{
    unsigned char byte;
    int size = strlen(magic_string);

    printf("Encoding magic string signature\n");
    
    Status check = encode_data_to_image(magic_string,size,encInfo->fptr_src_image,encInfo->fptr_stego_image);



    if(check == e_success)
    {
        printf("Done\n");
    }
}

/*Encode secret files extension */

Status encode_secret_file_extn(const char *file_extn, EncodeInfo *encInfo)
{
    int size = strlen(file_extn);


    printf("Encoding secret file extension\n");

    Status check = encode_data_to_image(file_extn,size,encInfo->fptr_src_image,encInfo->fptr_stego_image);



    if(check == e_success)
    {
        printf("Done\n");
    }
}
