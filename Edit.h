//Haeder file for edit detail's
#ifndef EDIT_H
#define EDIT_H

#include "types.h" // Contains user defined types

/* 
 * Structure to store information required for
 * encoding secret file to source Image
 */
typedef struct _EditInfo
{
    /* Source Image info */
    char *src_image_fname;
    FILE *fptr_src_image;

    char *Tag;
    char new_detail[31];

}EditInfo;

/*struct to store the header information*/
typedef struct _Validate
{
    char header[3];
    unsigned int file_size;
    short ver;
}Validate;

/*structure to store Edit info*/
typedef struct _Information
{
    int size;
    char in[31];
}Information;
 
/*function prototypes*/
//check operation type
operation check_operation_type(char *argv[]);

//function call to edit detail's
Status do_editing(EditInfo *ediInfo, Validate *valiInfo);

//chceking tag 
Status Tag_chcek(EditInfo *ediInfo);

//open file function
Status open_file(EditInfo *ediInfo);

//validation of song file function
Status Validate_file(EditInfo *ediInfo,Validate *valiInfo);

//edit information function
Status edit_Information(EditInfo *ediInfo);


#endif