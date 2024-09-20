/*-----------------------------------------------------------------------------------------------------------------------------------*/
/*                                                           View detail's C file                                                    */
/*-----------------------------------------------------------------------------------------------------------------------------------*/
//Header file include
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include "types.h"
#include "View.h"
#include "Macro.h"

/*Function defination's*/

Status open_files(ViewInfo *viewInfo)                        //called function to open file
{
    //Open src file in read mode & storing opened file address in file pointer 
    viewInfo->fptr_src_image=fopen(viewInfo->src_image_fname,"r");
    //chcek condition & return enum member
    if(viewInfo->fptr_src_image==NULL)
        return e_failure;
    else
        return e_sucess;
}

Status do_decoding(ViewInfo *viewInfo, Validation *valiInfo)  //called function for decoding info
{
    //function call to open file
    if(open_files(viewInfo)==e_sucess)
        printf("INFO: File opened sucessfully\n");
    else
    {
        printf("INFO: File opening error\n");
        return e_failure;
    }

    //function call for validation of song file
    if(Validation_file(viewInfo,valiInfo)==e_sucess)
        printf("INFO: File Version Validation done\n");
    else{
        printf("INFO: File Version Validation error\n");
        return e_failure;
    }
    //close file
    fclose(viewInfo->fptr_src_image);

    //creation of structure variable of array type
    Info Infodoc[6];
    //function call to copy information 
    if(Copy_Information(viewInfo,Infodoc)==e_sucess)
        printf("INFO: Copied Information sucessfully\n");
    else{
        printf("INFO: Copied Information Error\n");
        return e_failure;
    }

    //function call to print information
    Printing_Info(Infodoc,valiInfo);

    return e_sucess;
}

Status Validation_file(ViewInfo *viewInfo,Validation *valiInfo)      //called function for validation of song file
{
    //memory allocation
    char *buff=malloc(3*sizeof(char));
    //reading first 3 byte &chceking with macro
    fread(buff,3,1,viewInfo->fptr_src_image);
    strcpy(valiInfo->header,buff);
    if(strcmp(buff,ID3)==0)
        printf("INFO: file ID3 validation sucessful\n");
    else
    {
        printf("INFO: Error ID3 validation\n");
        return e_failure;
    }

    //realloction size of buff
    buff=realloc(buff,2);
    //read next 2 byte to check version of file
    fread(&valiInfo->ver,1,2,viewInfo->fptr_src_image);
    if(valiInfo->ver==03 || valiInfo->ver==04)
        printf("INFO: File version validation sucessful\n");
    else
    {
        printf("INFO: Error version %d validation\n",valiInfo->ver);
        return e_failure;
    }
    fseek(viewInfo->fptr_src_image,1,SEEK_CUR);
    //read next 4 byte & store size of song file
    fread(&valiInfo->file_size,1,4,viewInfo->fptr_src_image);
    free(buff);

    return e_sucess;
}

Status Copy_Information(ViewInfo *viewInfo, Info Infodoc[])         //called function to copy information
{
    //creation of array of char pointer
    char *str[6]={"TPE1","TIT2","TALB","TYER","TCON","TCOM"};
    int i=0;
    //function call to open file
    if(open_files(viewInfo)==e_sucess)
        printf("INFO: File opened sucessfully\n");
    else
    {
        printf("INFO: File opening error\n");
        return e_failure;
    }
    
    label :
    //rewind file from 0 index
    rewind(viewInfo->fptr_src_image);
    //seek first 10 byte of header
    fseek(viewInfo->fptr_src_image,10,SEEK_SET);

    //running loop & checking every tag 
    char *buff=malloc(4*sizeof(char));
    while(fread(buff,4,1,viewInfo->fptr_src_image)>0 && i!=6)
    {
        if((strcmp(buff,str[i]))==0)
        {
            //if tag found then fetch infortion & storing into structure memnber of array type
            fseek(viewInfo->fptr_src_image,3,SEEK_CUR);
            Infodoc[0].size=0;
            fread(&Infodoc[0].size,1,1,viewInfo->fptr_src_image);
            fseek(viewInfo->fptr_src_image,3,SEEK_CUR);
            fread(Infodoc[i].in,Infodoc[0].size-1,1,viewInfo->fptr_src_image);
            //increse i value & goto label to start from begining
            i++;
            goto label;
        }
        //if tag not equal then skill that tag information
        fseek(viewInfo->fptr_src_image,3,SEEK_CUR);
        Infodoc[0].size=0;
        fread(&Infodoc[0].size,1,1,viewInfo->fptr_src_image);
        fseek(viewInfo->fptr_src_image,2,SEEK_CUR);
        fseek(viewInfo->fptr_src_image,Infodoc[0].size,SEEK_CUR);
    }
    //free buff
    free(buff);
    //close file
    fclose(viewInfo->fptr_src_image);
    return e_sucess;
}

Status Printing_Info(Info Infodoc[], Validation *valiInfo)         //called function to printing deatil's
{
    //before print info system clear
    system("clear");
    //printing detail's of tag information
    printf("\n---------------------------------------Mp3 Tag Reader----------------------------------------\n");
    printf("\nHeader : %s\tVersion : v2.%d\tFile Size : [%.2f MB]\n",valiInfo->header,valiInfo->ver,((float)valiInfo->file_size/1073741824));
    printf("\n----------------------------------------Information------------------------------------------\n");
    printf("\n1. Artist Name : %s\n2. Title/Song name : %s\n3. Album : %s",Infodoc[0].in,Infodoc[1].in,Infodoc[2].in);
    printf("\n4. Year : %s\n5. Content type : %s\n6. Compressor : %s\n",Infodoc[3].in,Infodoc[4].in,Infodoc[5].in);
    printf("\n---------------------------------------------------------------------------------------------\n");

    return e_sucess;
}
