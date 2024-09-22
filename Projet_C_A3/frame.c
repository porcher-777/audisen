#include "frame.h"
#include "define.h"

//--- createInitFrame --------------------------------------------------------------
// Fonction qui prendre une chanson et convertir son nom, tempo et nombre de ticks en frame d'initialisation utilisable par le stm32
// \param frame (pointeur sur char), la frame complète produite par la fonction
// \param mySong (s_song), prend une chanson pour en extraire les données
// \return void

void createInitFrame(s_song mySong, char* frame){
    strcpy(frame, "#");
	strcat(frame,mySong.title);             //Ecrit le titre dans la frame
    strcat(frame,",");
    char* tmp=malloc(sizeof(char)*3);
    sprintf(tmp,"%d",mySong.tpm);
    strcat(frame,tmp);                      //Ecrit le tpm dans la frame

    strcat(frame,",");
    char* nTicks=malloc(sizeof(char)*2);
    sprintf(nTicks,"%d",mySong.nTicks);
    strcat(frame,nTicks);                   //Ecrit le nombre de ticks dans la frame

    strcat(frame,"*");
    free(nTicks);
    free(tmp);
    int i=1;

    int checksum=0;
    while (frame[i]!='*'){                  //Boucle pour calculer le checksum
        checksum^=frame[i];
        i++;
    }
    char* hexa=malloc(sizeof(char)*2);
    sprintf(hexa,"%02x",checksum);
    strcat(frame,hexa);
    free(hexa);
    strcat(frame,"\r\n");
}

//--- createTickFrame --------------------------------------------------------------
// Fonction qui prend un ticks et le convertis en frame utilisable par le stm32
// \param frame (pointeur sur char), la frame complète produite par la fonction
// \param myTick (s_tick), prend un tick pour en extraire les données
// \return void
void createTickFrame(s_tick myTick, char* frame){
     frame[0]='#';
     frame[1]='0';
     frame[2]=',';
     frame[3]=(myTick.accent%10)+0x30;
     for(int i=0;i<4;i++){                          //Boucle pour les virgules
        frame[4+i*3]=',';
     }
     for(int i=0;i<4;i++){                          //Boucle pour les ticks
        frame[5+i*3]=(myTick.note[i]/10)+0x30;
        frame[6+i*3]=(myTick.note[i]%10)+0x30;
     }
     int checksum=0;
     for (int i=1;i<=15;i++){
        checksum^=frame[i];                         //Boucle pour calculer le checksum
    }
    char* nombre=malloc(sizeof(char)*2);
    sprintf(nombre,"%02x",checksum);
     frame[16]='*';
     frame[17]=nombre[0];
     frame[18]=nombre[1];
     frame[19]='\r';
     frame[20]='\n';
     free(nombre);
/*
    strcpy(frame, "#");
    strcat(frame,"0");
    strcat(frame,",");
    char* nombre=malloc(sizeof(char)*2);
    sprintf(nombre,"%1d",myTick.accent);
    strcat(frame,nombre);
    for(int i=0;i<4;i++){
        strcat(frame,",");
        sprintf(nombre,"%02d",myTick.note[i]);
        strcat(frame,nombre);
    }
    strcat(frame,"*");
   int i=1;
    int checksum=0;
    while (frame[i]!='*'){
        checksum^=frame[i];
        i++;
    }
    sprintf(nombre,"%02x",checksum);
    strcat(frame,nombre);
    strcat(frame,"\r\n");
    free(nombre);
    */
}

//--- createTickFrameSnaper --------------------------------------------------------------
// Fonction qui prend un ticks et le convertis en frame utilisable par le stm32 avec l'option snaper
// \param frame (pointeur sur char), la frame complète produite par la fonction
// \param myTick (s_tick), prend un tick pour en extraire les données
// \return void
void createTickFrameSnaper(s_tick myTick, char* frame){
    frame[0]='#';
    frame[1]='1';
    frame[2]=',';
    frame[3]=(myTick.accent%10)+0x30;
    for(int i=0;i<4;i++){                           //Boucle pour les virgules
        frame[4+i*3]=',';
    }

    for(int i=0;i<4;i++){                           //Boucle pour les ticks
        frame[5+i*3]=(myTick.note[i]/10)+0x30;
        frame[6+i*3]=(myTick.note[i]%10)+0x30;
    }
    int checksum=0;
    for (int i=1;i<=15;i++){
        checksum^=frame[i];                         //Boucle pour calculer le checksum
    }
    char* nombre=malloc(sizeof(char)*2);
    sprintf(nombre,"%02x",checksum);
    frame[16]='*';
    frame[17]=nombre[0];
    frame[18]=nombre[1];
    frame[19]='\r';
    frame[20]='\n';
    free(nombre);
}

