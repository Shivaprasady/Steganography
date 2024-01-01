#include <stdio.h>
#include<string.h>
#include<stdlib.h>
#include "encode.h"
#include "types.h"

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
    }
    else
    {
        printf("Done. found OKAY\n");
    }



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
