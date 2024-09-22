#include "define.h"
#include "autotests.h"
#include "ams.h"
#include "amp.h"
#include "frame.h"
#include "usb.h"


int main(){
    char** titres = (char**) malloc (100* sizeof(char*));
    for(int i=0 ; i<100; i++)
        titres[i] = (char*) malloc (100 * sizeof(char));
    int nbsong;
    char init_frame[(INIT_FRAME_MAX_SIZE)] = "";
    char frame[TICK_FRAME_SIZE] = "";  

    //---------------------------------------------------------------

    //UTILISATION 1 : partir d'une playlist existante
    
    //exeAMP("ma_playlist2.amp", titres, &nbsong);

    //UTILISATION 2 : Créer un fichier ams à partir d'un fichier texte

    nbsong = 1;
    createAMS("physical.txt", "physical.ams");
    titres[0] = "physical.ams";

    //----------------------------------------------------------------

    testReadAMP();
    testReadAMS();
    testFrame();
    testCreateAMS();

    FILE* matlab_file = fopen("physical.frm", "wb+");      //Ouverture en binaire pour éviter les \r supplémentaires

    for(int i = 0; i < nbsong; i++){

        char melodie[MAX_NUMBER_TICKS * 30] = "";
        printf("titre : %s\n", titres[i]);
        s_song song = readAMS(titres[i]);
        createInitFrame(song, init_frame);
        strcpy(melodie, init_frame);

        for(int j = 0; j < song.nTicks; j++){
            
            createTickFrame(song.tickTab[j], frame);
            strcat(melodie, frame);
            strcpy(frame, "");

        }

        strcat(melodie, "\0");
        fprintf(matlab_file, "%s", melodie);        //Ecriture dans le fichier .frm
        strcpy(melodie, "");
        
    }

    for(int i=0 ; i<100; i++)
        free(titres[i]);
    free(titres);
    fclose(matlab_file);


    return 0;
}


