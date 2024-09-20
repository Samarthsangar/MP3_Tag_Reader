/*----------------------------------------------------------------------------------------------------------------------------------------*/
/*                                                                      MP3 Tag Reader                                                    */
/*----------------------------------------------------------------------------------------------------------------------------------------*/
/*
Name : Samarth Rajendra Sangar.
Date : 19/09/2024
Projcet name : Mp3 tag Reader
               MP3 tag reader is a software which will read and display MP3 (ID3) tag information from MP3 files. The software will be 
               desktop based and not web based.
*/
//Header file Included
#include<stdio.h>
#include<string.h>
#include "View.h"
#include "Edit.h"
#include "types.h" 

void error();             //function prototype to print error

int main(int argc, char *argv[])
{
    if(argc==1)           //check CLA & based on condition print error
    {
        printf("INFO: Less argument in Command Line\n");
        error();
        return e_failure;
    }
    else if(argc==2)
    {
        if(strcmp(argv[1],"--help")==0)              //printinh help option
        {
            printf("-------------------------------------Help----------------------------------------\n");
            printf("1. -v : To view detail like : ./a.out -v Src_mp3_file\n");
            printf("2. -e : To edit detail like : ./a.out -e -t/-a/-A/-y/-m/-c New_detail Src_mp3_file\n");
            printf("2.0. Edit option :\n2.1. -t : Title/song name\n2.2. -a : Artist Name\n2.3. -A : Album\n2.4. -y : Year\n2.5. -m : Content type\n2.6. -c : Compressor\n");
            printf("---------------------------------------------------------------------------------\n");
            return e_sucess;
        }
        else
        {
            printf("INFO: Invalid Entry get help\n");
            error();
            return e_failure;
        }
    }

    if(check_operation_type(argv)==e_edit)                 //function call to check operation type
    {
        if(argc<5)                                         //chcek some condition's & printing error
        {
            printf("INFO: Less CLA or data to edit details\n");
            error();
            return e_failure;
        }
        if(strstr(argv[2],"-y")!= NULL)
        {
            if(strlen(argv[3])>4 || argc>5)
            {
                printf("INFO: Many CLA or data to edit year\n");
                error();
                return e_failure;
            }
        }

        EditInfo ediInfo;                 //creation of structure variable
        Validate valiInfo;                //creation of structure variable
        ediInfo.Tag=argv[2];              //storing CLA into the struct members
        if(argc==5)                       //check CLA
        {
            char *ch1=strchr(argv[4],'.');
            if(ch1==NULL || strcmp(ch1,".mp3")!=0 )
            {
                printf("INFO: mp3 file error\n");
                error();
                return e_failure;
            }
            
            ediInfo.src_image_fname=argv[4];
            strcpy(ediInfo.new_detail,argv[3]);
        }
        else                                    //if to many CLA then do following
        {
            int i=4,len=0;
            strcpy(ediInfo.new_detail,argv[3]);
            strcat(ediInfo.new_detail," ");
            len+=strlen(argv[3])+1;
            char *ch1=strchr(argv[i],'.');
            while(ch1==NULL || strcmp(ch1,".mp3")!=0)
            {
                len+=strlen(argv[i])+1;
                strcat(ediInfo.new_detail,argv[i]);
                strcat(ediInfo.new_detail," ");
                if(len>30)                                      //checking size of editing info
                {
                    printf("INFO: More information you give (more then 30 byte)\n");
                    error();
                    return e_failure;
                }
                i++;
                if(argv[i]==NULL)                                 //checking condition
                {
                    printf("INFO: mp3 file error\n");
                    error();
                    return e_failure;
                }
                ch1=strchr(argv[i],'.');
            }
            if((argc-1)>i)                                      //check condition & print error
            {
                printf("INFO: To many argument's in CLA\n");
                error();
                return e_failure;
            }
            ediInfo.src_image_fname=argv[i];              //store src file name in struct member
        }

        if(do_editing(&ediInfo,&valiInfo)==e_sucess)      //call function
            printf("INFO: Done\n");
        else
            printf("INFO: Failure\n");
    }
    else if(check_operation_type(argv)==e_view)           //function call to check operation type
    {
        if(argc>3)                                        //check some condition's 
        {
            printf("INFO: To many argument's in CLA for decoading !\n");
            error();
            return e_failure;
        }
        char *ch1=strchr(argv[2],'.');
        if(ch1==NULL || strcmp(ch1,".mp3")!=0 )                   //check file is mp3 or not
        {
            printf("INFO: mp3 file error\n");
            error();
            return e_failure;
        }
        ViewInfo viewInfo;                                  //creation of structure variable
        Validation valiInfo;                                //creation of structure variable
        viewInfo.src_image_fname=argv[2];                   //storing src file name in struct member

        printf("INFO: File's Opening for decoding\n");
        if(do_decoding(&viewInfo,&valiInfo)==e_sucess)      //function call for decoding info
            printf("INFO: Done\n");
        else
            printf("INFO: Failure\n");
    }
    else                                                   //print invalid entry
    {
        printf("INFO: Invalid Entry !\n");
        error();
        return e_failure;
    }
    
    return e_sucess;
}

operation check_operation_type(char *argv[])               //called function to chcek operation
{   
    //checking type & return enum variable
    if(strcmp(argv[1],"-e")==0)
        return e_edit;
    else if(strcmp(argv[1],"-v")==0)
        return e_view;
    else
        return e_unsupported;
}

void error()                                              //called function
{
    //printing prompt if error occoured & if error function called
    printf("-----------------------Error--------------------------\n");
    printf("1. -v : To view detail like : ./a.out -v Src_mp3_file\n");
    printf("2. -e : To edit detail like : ./a.out -e -t/-a/-A/-y/-m/-c New_detail Src_mp3_file\n");
    printf("3. ./a.out --help : to get help\n");
    printf("------------------------------------------------------\n");
}