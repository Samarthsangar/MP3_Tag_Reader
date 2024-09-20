/*                                                    Edit Info File                                                            */
//Haeder file include
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include "types.h"
#include "Edit.h"
#include "Macro.h"

/*Function defination's*/
Status do_editing(EditInfo *ediInfo, Validate *valiInfo)          //called function
{
    //Function call to chcek Tag
    printf("INFO: Checking tag\n");
    if(Tag_chcek(ediInfo)==e_sucess)
        printf("INFO: Tag is correct\n");
    else{
        printf("INFO: Incorrect tag\n");
        return e_failure;
    }
    
    //function call to open file
    printf("INFO: ## Editing data\n");
    printf("INFO: File opening for validation\n");
    if(open_file(ediInfo)==e_sucess)
        printf("INFO: File opened sucessfully\n");
    else
    {
        printf("INFO: File opening error\n");
        return e_failure;
    }
    
    //function call for validation of song file
    if(Validate_file(ediInfo,valiInfo)==e_sucess)
        printf("INFO: File Version Validation done\n");
    else{
        printf("INFO: File Version Validation error\n");
        return e_failure;
    }
    //closing file
    fclose(ediInfo->fptr_src_image);

    //function call for editing information
    if(edit_Information(ediInfo)==e_sucess)
        printf("INFO: Editing Information sucessfully\n");
    else{
        printf("INFO: Editing Information Error\n");
        return e_failure;
    }

    return e_sucess;
}

Status Tag_chcek(EditInfo *ediInfo)                         //called function to check tag
{
    //chcek tag & return enum variable based on condition
    if((strcmp(ediInfo->Tag,"-t")==0) || (strcmp(ediInfo->Tag,"-a")==0) || (strcmp(ediInfo->Tag,"-A")==0) || (strcmp(ediInfo->Tag,"-y")==0) || (strcmp(ediInfo->Tag,"-m")==0) || (strcmp(ediInfo->Tag,"-c")==0))
        return e_sucess;
    else
        return e_failure;
}

Status open_file(EditInfo *ediInfo)                         //called function to open file
{
    //open file in Read + writing mode & storing address of opened file in file pointer
    ediInfo->fptr_src_image=fopen(ediInfo->src_image_fname,"r+");
    //checking condition & return enum variable
    if(ediInfo->fptr_src_image==NULL)                     
        return e_failure;
    else
        return e_sucess;
}

Status Validate_file(EditInfo *ediInfo, Validate *valiInfo)       //called function for validation of song file
{
    char *buff=malloc(3*sizeof(char));
    //readering first 3 byte from src file
    fread(buff,3,1,ediInfo->fptr_src_image);
    strcpy(valiInfo->header,buff);
    //checking file 3 byte is ID3 or not
    if(strcmp(buff,ID3)==0)
        printf("INFO: file ID3 validation sucessful\n");
    else
    {
        printf("INFO: Error ID3 validation\n");
        return e_failure;
    }

    //reallocation of size of buff
    buff=realloc(buff,2);
    //reading next 2 byte of data from src file
    fread(&valiInfo->ver,1,2,ediInfo->fptr_src_image);
    //checking file version
    if(valiInfo->ver==03 || valiInfo->ver==04)
        printf("INFO: File version validation sucessful\n");
    else
    {
        printf("INFO: Error version %d validation\n",valiInfo->ver);
        return e_failure;
    }
    fseek(ediInfo->fptr_src_image,1,SEEK_CUR);
    //storing the of song file
    fread(&valiInfo->file_size,1,4,ediInfo->fptr_src_image);
    free(buff);

    return e_sucess;
}

Status edit_Information(EditInfo *ediInfo)                //called function to edit detail's
{
    //creation of struct variable
    Information Infodoc;
    //creation of array of char pointer
    char *str[6]={"TPE1","TIT2","TALB","TYER","TCON","TCOM"};
    int i=strcmp(ediInfo->Tag,"-t")==0?0:strcmp(ediInfo->Tag,"-a")==0?1:strcmp(ediInfo->Tag,"-A")==0?2:strcmp(ediInfo->Tag,"-y")==0?3:strcmp(ediInfo->Tag,"-m")==0?4:strcmp(ediInfo->Tag,"-c")==0?5:-1;
    int len=strlen(ediInfo->new_detail);
    
    if(open_file(ediInfo)==e_sucess)
        printf("INFO: File opened sucessfully\n");
    else
    {
        printf("INFO: File opening error\n");
        return e_failure;
    }
    //seek first 10 byte of header
    fseek(ediInfo->fptr_src_image,10,SEEK_SET);

    //running loop checking Tag if occured replacing the information
    char *buff=malloc(4*sizeof(char));
    while(fread(buff,4,1,ediInfo->fptr_src_image)>0)
    {
        if((strcmp(buff,str[i]))==0)
        {
            fseek(ediInfo->fptr_src_image,3,SEEK_CUR);
            Infodoc.size=0;
            fread(&Infodoc.size,1,1,ediInfo->fptr_src_image);
            fseek(ediInfo->fptr_src_image,3,SEEK_CUR);
            fwrite(ediInfo->new_detail,Infodoc.size-1,1,ediInfo->fptr_src_image);
            //goto perticular label & stop loop
            goto label;
        }
        fseek(ediInfo->fptr_src_image,3,SEEK_CUR);
        Infodoc.size=0;
        fread(&Infodoc.size,1,1,ediInfo->fptr_src_image);
        fseek(ediInfo->fptr_src_image,2,SEEK_CUR);
        fseek(ediInfo->fptr_src_image,Infodoc.size,SEEK_CUR);
    }
    label :
    free(buff);
    fclose(ediInfo->fptr_src_image);
    return e_sucess;
}

