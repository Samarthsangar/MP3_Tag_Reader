//Header file for view detail's
#ifndef VIEW_H
#define VIEW_H

#include "types.h" // Contains user defined types

/* 
 * Structure to store information required for
 * encoding secret file to source Image
 */
typedef struct _ViewInfo
{
    /* Source Image info */
    char *src_image_fname;
    FILE *fptr_src_image;

} ViewInfo;

//structure to store header information of file
typedef struct _Validation
{
    char header[3];
    unsigned int file_size;
    short ver;
}Validation;

//struct to store tag information
typedef struct _Info
{
    int size;
    char in[31];
}Info;
 
/*Function prototype's*/
//function to chcek operation type
operation check_operation_type(char *argv[]);

//function to decode data
Status do_decoding(ViewInfo *viewInfo, Validation *valiInfo);

//function to open file
Status open_files(ViewInfo *viewInfo);

//function for validation of song file
Status Validation_file(ViewInfo *viewInfo,Validation *valiInfo);

//function to copy information
Status Copy_Information(ViewInfo *ViewInfo, Info Infodoc[]);

//function to print information which is decoded
Status Printing_Info(Info Infodoc[], Validation *valiInfo);

#endif