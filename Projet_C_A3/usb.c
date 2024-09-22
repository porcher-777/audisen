//
// Created by ISEN on 13/02/2024.
//

#include "define.h"
#include "frame.h"
#include "ftd2xx.h"
#include "usb.h"

//--- initUSB --------------------------------------------------------------
// Fonction qui va ouvrir la connexion et déterminer le nombre de bits/s, la longueur des octets envoyés et le flux de contrôle
// \no param
// \return ftHandle (FT_HANDLE), la connexion USB ouverte et paramétrée

FT_HANDLE initUSB(){
    printf("Je rentre dans initUSB\n");
    FT_HANDLE ftHandle;
    FT_STATUS ftStatus;

    ftStatus = FT_Open(0, &ftHandle);   //Indice 0 = device 0 
    if (ftStatus != FT_OK){
        printf("USB PAS OK\n");
        closeUSB(ftHandle);
        return NULL;
    }

    ftStatus = FT_SetBaudRate(ftHandle, 9600);
    if (ftStatus != FT_OK){
        printf("Baud rate pas OK\n");
        closeUSB(ftHandle);
        return NULL;
    }

    ftStatus = FT_SetFlowControl(ftHandle, FT_FLOW_NONE, 0, 0); //Pas de flow de controle
    if (ftStatus != FT_OK){
        printf("FlowControl pas OK\n");
        closeUSB(ftHandle);
        return NULL;
    }

    ftStatus = FT_SetDataCharacteristics(ftHandle, FT_BITS_8, FT_STOP_BITS_1, FT_PARITY_NONE);
    if (ftStatus != FT_OK){
        printf("Characteristics pas OK\n");
        closeUSB(ftHandle);
        return NULL;
    }
        
    printf("full USB OK\n");
    return ftHandle;
}

//--- closeUSB --------------------------------------------------------------
// Fonction qui va fermer la connexion USB
// \param ftHandle (FT_HANDLE), la connexion USB à fermer
// \no return 

void closeUSB(FT_HANDLE ftHandle){
    printf("Je rentre dans closeUSB\n");
    FT_Close(ftHandle);
}

//--- writeUSB --------------------------------------------------------------
// Fonction qui va envoyer les ticks sur la carte STM32
// \param frame (pointeur sur char), l'entièreté des ticks à envoyer
// \param ftHandle (FT_HANDLE), la connexion USB sur laquelle on va écrire
// \no return 

void writeUSB(char* frame, FT_HANDLE ftHandle){
    printf("Je rentre dans writeUSB\n");
    FT_STATUS ftStatus;
    DWORD BytesWritten;

    //printf("strlenframe %lld\n", strlen(frame));

    ftStatus = FT_Write(ftHandle, frame, strlen(frame), &BytesWritten);
    if ( ftStatus != FT_OK )
        printf("L'ecriture dans l'USB a echoue, %d\n", (int)BytesWritten);   
    if ( ftStatus == FT_OK )  
        printf("L'ecriture dans l'USB a reussi, %d\n", (int)BytesWritten);
}