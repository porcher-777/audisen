#include "define.h"
#include "ams.h"

//--- readAMS --------------------------------------------------------------
// Fonction qui va lire les données du fichier .ams et les mettre dans un strcuture s_song
// \param filename (pointeur sur char), le nom du fichier .ams à lire
// \return mySong (structure song), la chanson et toutes les infos de celle-ci : titre, TPM, nombre de ticks, notes

s_song readAMS(char* fileName){
	FILE* myfile = fopen(fileName, "r+");
	char tmp[MAX_SIZE_TITLE] = "";
    char tmp2[MAX_SIZE_TITLE] = "";
	char line[MAX_SIZE_LINE] = "";
	int current_note = 0;
	int current_tick = 0;
	s_song mySong = {0, 0, "", {0, 0, 0, 0, 0}};	//Chanson vide 

	if( !(myfile == NULL) ){
		fgets(mySong.title, 40, myfile);
		fgets(tmp, 3, myfile);

		mySong.tpm = atoi(tmp) * 2;         //Récupération du BPM
		for(int j = 0; j < 4; j++){
			fgets(line, 185, myfile);		//Sauter les lignes inutiles
		}
        

		while( fgets(line, sizeof(line), myfile) != NULL ){		//Parcours du fichier
			for(int i = 0; i < strlen(line); i++){

				if(line[i] == 'x'){
					mySong.tickTab[current_tick].accent = 0;
					mySong.tickTab[current_tick].note[current_note] = (i-3)/3 + 1;      //Sur le fichier .ams 1 note = 3 caractères "|  "
					current_note++;
				}

				if(line[i] == '^'){
					mySong.tickTab[current_tick].accent = 1;
					mySong.tickTab[current_tick].note[current_note] = (i-3)/3 + 1;
					current_note++;
				}
				
			}
			current_tick++;
			current_note = 0;
		}
		mySong.nTicks = current_tick;
	}
	mySong.title[strlen(mySong.title) - 1] = '\0';      //Marque la fin de la chaîne de caractères
	return mySong;
}

//--- createAMS --------------------------------------------------------------
// Fonction qui va lire les données du fichier .txt et les mettre dans le format d'un fichier .ams
// \param txtFileName (pointeur sur char), le nom du fichier .txt à lire
// \param txtFileName (pointeur sur char), le nom du fichier .ams dans lequel on va écrire
// \no return

void createAMS(char* txtFileName, char* amsFileName){
    FILE* txt= fopen(txtFileName, "r");
    FILE* ams= fopen(amsFileName, "w+");
    if(ams !=NULL && txt !=NULL) {
        char *chaine = malloc(sizeof(char) * MAX_SIZE_TITLE);

        //Récupération du titre et de sa taille
        fgets(chaine, MAX_SIZE_TITLE, txt);
        fprintf(ams, "%s", chaine);
		int taille_titre = strlen(chaine);

        //Récupération du tempo et de sa position
        fgets(chaine, 4, txt);
        for(int i=0;i<4;i++){
            if(chaine[i]=='\n')
                chaine[i] = '\0';
        }
        fprintf(ams,"%s\n\n", chaine);
		int taille_tempo = strlen(chaine) + taille_titre;

		fseek(txt, taille_tempo, 0);

        //Mise en place du cadrillage du haut
        fprintf(ams,"    ");
        for(int i=1;i<61;i++){
            fprintf(ams,"%02d ", i);
        }
        fprintf(ams,"\n");

        //Permet de conserver la position
        long debut_txt = ftell(txt);
        long debut_ams = ftell(ams);
		//printf("position ams %d", debut_txt);

        //Déterminer le nombre de trames
        int nb_ligne=0;
        char tampon[189];
        while(fgets(tampon,189,txt) != NULL){
            nb_ligne++;
        }
        nb_ligne--;
        free(chaine);

        //Créer le graillage du fichier
        fseek(txt, debut_txt,0);
        for(int j=0;j<nb_ligne;j++){
            fprintf(ams,"%03d", j+1);
            fprintf(ams,"|");
            for (int i=0;i<60;i++){
                fprintf(ams,"  |");
            }
            fprintf(ams,"\n");
        }
        fseek(ams, debut_ams, 0);

        //Rempli le ams et lis les infos du txt
        long position = ftell(ams);

        //Lecture
        for(int j=0;j<nb_ligne;j++) {
            char tab_note[][4] = {"", "", "", ""};
            char tab_rythme[][4] = {"", "", "", ""};
            fscanf(txt, "%s %c", tab_note[0], tab_rythme[0]);

            int k=1;
            while(k<4 && strcmp(tab_note[k],"\n")!=0 ) {
                fscanf(txt, ",%s %c", tab_note[k], tab_rythme[k]);
                if(strlen(tab_note[k]) < 2)
                    k=4;
                k++;
            }
            //printf( "%s %s, %s %s, %s %s, %s %s\n", tab_note[0],tab_rythme[0], tab_note[1],tab_rythme[1], tab_note[2],tab_rythme[2], tab_note[3],tab_rythme[3]);

            //Ecriture
            fseek(ams, position,0);
            for (int i = 0; i < 4; i++) {
                if (strcmp(tab_rythme[i],"") != 0) {

                    int position_car = convertisseur(tab_note[i]);
					//printf("tabnote = %s| %d\n", tab_note[i], position);
                    fseek(ams,4+3*(position_car-1),SEEK_CUR);

                    fputc('^',ams);

                    if(strcmp(tab_rythme[i],"R")==0){
                        for(int l=1;l<8;l++){
                            fseek(ams,185,SEEK_CUR);    //185 est le nombre de caractères dans une ligne
                            fputc('x',ams);
                        }
                        fseek(ams,position,0);
                    }
                    if(strcmp(tab_rythme[i],"B")==0){
                        for(int l=1;l<4;l++){
                            fseek(ams,185,SEEK_CUR);    //185 est le nombre de caractères dans une ligne
                            fputc('x',ams);
                        }
                        fseek(ams,position,0);
                    }
                    if(strcmp(tab_rythme[i],"N")==0){
						for(int i=0;i<2;i++){
							fseek(ams,185,SEEK_CUR);    //185 est le nombre de caractères dans une ligne
							fputc('x',ams);
						}
                        fseek(ams,position,0);
                    }
                    if(strcmp(tab_rythme[i],"C")==0 || strcmp(tab_rythme[i],"D")==0){
                        fseek(ams,position,0);
                    }
                }
            }
            position += 186;        //Ligne suivante
        }
    }
        fclose(txt);
        fclose(ams);

}

//--- convertisseur --------------------------------------------------------------
// Fonction qui va convertir les notes lues en un nombre correspondant (v. tableau de fréquence dans la doc)
// \param international (pointeur sur char), la note au format international
// \return (int), la valeur numérique de la position de la note dans le tableau de fréquence

int convertisseur(char* international){
    if(strcmp(international,"C1")==0)
        return 1;
    if(strcmp(international,"C1#")==0)
        return 2;
    if(strcmp(international,"D1")==0)
        return 3;
    if(strcmp(international,"D1#")==0)
        return 4;
    if(strcmp(international,"E1")==0)
        return 5;
    if(strcmp(international,"F1")==0)
        return 6;
    if(strcmp(international,"F1#")==0)
        return 7;
    if(strcmp(international,"G1")==0)
        return 8;
    if(strcmp(international,"G1#")==0)
        return 9;
    if(strcmp(international,"A1")==0)
        return 10;
    if(strcmp(international,"A1#")==0)
        return 11;
    if(strcmp(international,"B1")==0)
        return 12;
    if(strcmp(international,"C2")==0)
        return 13;
    if(strcmp(international,"C2#")==0)
        return 14;
    if(strcmp(international,"D2")==0)
        return 15;
    if(strcmp(international,"D2#")==0)
        return 16;
    if(strcmp(international,"E2")==0)
        return 17;
    if(strcmp(international,"F2")==0)
        return 18;
    if(strcmp(international,"F2#")==0)
        return 19;
    if(strcmp(international,"G2")==0)
        return 20;
    if(strcmp(international,"G2#")==0)
        return 21;
    if(strcmp(international,"A2")==0)
        return 22;
    if(strcmp(international,"A2#")==0)
        return 23;
    if(strcmp(international,"B2")==0)
        return 24;
    if(strcmp(international,"C3")==0)
        return 25;
    if(strcmp(international,"C3#")==0)
        return 26;
    if(strcmp(international,"D3")==0)
        return 27;
    if(strcmp(international,"D3#")==0)
        return 28;
    if(strcmp(international,"E3")==0)
        return 29;
    if(strcmp(international,"F3")==0)
        return 30;
    if(strcmp(international,"F3#")==0)
        return 31;
    if(strcmp(international,"G3")==0)
        return 32;
    if(strcmp(international,"G3#")==0)
        return 33;
    if(strcmp(international,"A3")==0)
        return 34;
    if(strcmp(international,"A3#")==0)
        return 35;
    if(strcmp(international,"B3")==0)
        return 36;
    if(strcmp(international,"C4")==0)
        return 37;
    if(strcmp(international,"C4#")==0)
        return 38;
    if(strcmp(international,"D4")==0)
        return 39;
    if(strcmp(international,"D4#")==0)
        return 40;
    if(strcmp(international,"E4")==0)
        return 41;
    if(strcmp(international,"F4")==0)
        return 42;
    if(strcmp(international,"F4#")==0)
        return 43;
    if(strcmp(international,"G4")==0)
        return 44;
    if(strcmp(international,"G4#")==0)
        return 45;
    if(strcmp(international,"A4")==0)
        return 46;
    if(strcmp(international,"A4#")==0)
        return 47;
    if(strcmp(international,"B4")==0)
        return 48;
    if(strcmp(international,"C5")==0)
        return 49;
    if(strcmp(international,"C5#")==0)
        return 50;
    if(strcmp(international,"D5")==0)
        return 51;
    if(strcmp(international,"D5#")==0)
        return 52;
    if(strcmp(international,"E5")==0)
        return 53;
    if(strcmp(international,"F5")==0)
        return 54;
    if(strcmp(international,"F5#")==0)
        return 55;
    if(strcmp(international,"G5")==0)
        return 56;
    if(strcmp(international,"G5#")==0)
        return 57;
    if(strcmp(international,"A5")==0)
        return 58;
    if(strcmp(international,"A5#")==0)
        return 59;
    if(strcmp(international,"B5")==0)
        return 60;
}