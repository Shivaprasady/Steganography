#include"types.h"        //Contains user defined types

typedef struct _DecodeInfo
{
    /*Stego image info*/
    char *encoded_image_fname;
    FILE *fptr_encoded_image;


    /*output file(secterfile) info*/
    char *output_file_fname;
    FILE *fptr_output_file;

} DecodeInfo;

/*  Read and validate files for decode  */
Status read_and_validate_decode_args(char *argv[],DecodeInfo *decInfo);

/*  perform the decoding  */
Status do_decoding(DecodeInfo *decInfo);

/*  Get FILE pointer i/p and o/p files    */
Status open_decode_files(DecodeInfo *decInfo);

/*  Skip header data    */
Status skip_header(FILE *fptr);

/*  Decoding magic string   */
Status decode_magic_string(DecodeInfo *decInfo);

/*  Decode extension size   */
Status decode_extn_size(char *arr,int *size);

/*  Decode extension    */
Status decode_extn_data(DecodeInfo *decInfo);

/*  Decode secret file size     */
Status decode_secret_file_size(DecodeInfo *decInfo);

/*  Decode Secret file data     */
Status decode_secret_file_data();


/*
   set of common functions
*/

/*  Decode data from image */
Status decode_data_from_image();


/*  Decode bits from bytes  */
Status decode_bit_from_byte();

/*  Decode size from lsb */
Status decode_size_from_lsb();



