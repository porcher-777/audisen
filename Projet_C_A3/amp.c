#include "define.h"
#include "amp.h"


//--- initAMP --------------------------------------------------------------
// Fonction qui va ouvrir le fichier de la playlist et retourner une erreur si l'ouverture a échoué
// \param filename (pointeur sur char), le nom du fichier de la playlist
// \return pf (pointeur sur FILE) si le fichier .amp est ouvert, NULL sinon

FILE* initAMP(char* filename){
    FILE * pf = NULL;
    pf = fopen(filename, "r+");

    if (pf != NULL)
        return pf;
    else{
        printf("Impossible d'ouvrir le fichier %s", filename);
        return NULL;
    }
}

//--- stringProcessor --------------------------------------------------------------
// Fonction qui va traiter le titre de chaque musique pour remplacer les charactères spéciaux par un underscore
// et le mettre en minuscules
// \param song_filename (pointeur sur char), la variable dans laquelle le titre d'une musique sera stocké
// \no return 

void stringProcessor(char* song_filename){
    char tmp[100] = "";

    for(int i = 0; i < strlen(song_filename); i++){
        tmp[i] = tolower(song_filename[i]);     //Mise en minuscules

        if(!(tolower(song_filename[i]) >= 97 && tolower(song_filename[i]) <= 122) && (song_filename[i] != '\n')) //![a-z] regex
            tmp[i] = '_';
        
    }
    underscoreRemover(tmp);     //Enlever les underscores inutiles 
    if(tmp[strlen(tmp) - 1] == '\n')
        tmp[strlen(tmp) - 1] = '\0';
    strcpy(song_filename, tmp);
}

//--- underscoreRemover --------------------------------------------------------------
// Fonction qui va détecter les underscores superflus et les retirer
// \param song_filename (pointeur sur char), la variable dans laquelle le titre de la musique avec les underscores est stocké
// \no return 

void underscoreRemover(char* song_filename){
    int offset = 0, j = 0;
    char final[100] = ""; 
    for(int i = 0; i < strlen(song_filename); i++){
        
        if(song_filename[i + offset] == '_'){
            while(song_filename[i + offset] == '_')
                i++;
            i--;
        }

        final[j] = song_filename[i];
        j++;
    }
    strcpy(song_filename, final);
}

//--- readAMP --------------------------------------------------------------
// Fonction qui va lire le fichier de playlist ligne par ligne et stocké chaque titre à chaque appel 
// \param pf (pointeur sur FILE), le fichier de la playlist ouvert précédemment 
// \param song_filename (pointeur sur char), la variable dans laquelle le titre d'une musique sera stocké
// \no return 

void readAMP(FILE* pf, char* song_filename){
    fgets(song_filename, MAX_SIZE_LINE, pf);
    stringProcessor(song_filename);
    strcat(song_filename, ".ams\0");
}

//--- closeAMP --------------------------------------------------------------
// Fonction qui va fermer le fichier de la playlist ouvert précédemment s'il existe
// \param pf (pointeur sur FILE), le fichier de la playlist ouvert précédemment 
// \no return 

void closeAMP(FILE* pf){
    if(!(access(pf, 0)))    //Vérifie l'existence du fichier (0 = F_OK)
        fclose(pf);
}

//--- exeAMP --------------------------------------------------------------
// Fonction qui va ouvrir le fichier, stocker chaque .ams dans un tableau puis fermer le fichier
// \param playlist_filename (pointeur sur char), le nom de la playlist (.amp) qui va être ouverte
// \param titres (pointeur sur char*), un tableau de chaînes de caractères qui accueillera les titres de la playlist (.ams)
// \param nbsong (pointeur sur int), le nombre de chansons présentes dans la playlist (.amp)
// \no return 

void exeAMP(char* playlist_filename, char** titres, int* nbsong){
    char line[190] = "";
    int current_song = 0;

    FILE* playlist_file = initAMP(playlist_filename);
    while( !feof(playlist_file) ){
        readAMP(playlist_file, line);
        strcpy(titres[current_song], line);
        current_song++;
    }

    *nbsong = current_song;
    closeAMP(playlist_file);
}