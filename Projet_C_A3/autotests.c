
#include "define.h"
#include "autotests.h"
#include "ams.h"
#include "amp.h"
#include "frame.h"
#define AMP_NB_TESTS 6

/* Version uniquement pour le \r\n
void convert_rl(char* buffer)
{
	char *p = strrchr(buffer, '\r');
	if(p && p[1]=='\n' && p[2]=='\0') {
	    p[0] = '\n';
	    p[1] = '\0';
	}
}
*/

/*
// Version capable de tout, courte, sympa pour les étudiants, pas parfaite (pas opti et ne prends pas en compte le cas avec un \n ou \r qui serait justifiable en dehors des fins de chaines (pas un problème pour ce projet)
void convert_rl(char* buffer)
{
	for(int i=0;buffer[i]!='\0';i++)
	{ 
		if(buffer[i]=='\r' || buffer[i]=='\n') buffer[i] = '\0';
	}
}

*/

// Version opti qui gère \r\n tout comme \n tout en ne touchant pas aux eventuels \n ou autre en milieu de chaine etc.
void convert_rl(char* buffer)
{
	int taille = 0;
	int i = 0;

	if (buffer!=NULL)
	{
		taille = strlen(buffer)+1; //+1 car caractère de fin de chaine pas inclus dans strlen
		if (taille > 2) // on évite de parcourir tout le fichier au cas où il y ait un \n ou \r pertinent en milieu de chaine
		{               // on ne traite que deux cas : une chaine >= à 3 en tout (probablement un \r\n\'0' et une chaine > à 2 en tout, par exemple un \n\'0'
			for(i=taille-3;i<taille;i++)
			{
				if(buffer[i] == '\r' || buffer[i] == '\n')
				{
					buffer[i] = '\0';
				}
			}
		}
		else if (taille > 1) // Si la chaine est vraiment trop courte on ne regarde que le dernier caractere dans notre boucle for. (équivalent à tester uniquement buffer[taille-2])
		{	
			for(i=taille-2;i<taille;i++)
			{
				if(buffer[i] == '\r' || buffer[i] == '\n')
				{
					buffer[i] == '\0';
				}
			}
		}
		else{
			printf("convert_rl : la chaine considérée est trop courte\n");
		}
	}
	else
	{
		printf("convert_rl : la chaine considérée ne semble pas exister (pointeur NULL)\n");
	}
}


// fonction de base pour afficher les résultats de tests
void printAutoTestsResults(char* testName, float mark[], int coeff[], int nTests){
	int i=0;
	int cpt=0;
	int max=0;
	printf("---- Autotest results of block %s ----\n",testName);
	for(i=0;i<nTests;i++){
		printf("--> test %d : %.0f/%d\n",i,mark[i]*coeff[i],coeff[i]);
		cpt+=mark[i]*coeff[i];
		max +=coeff[i];
	}
	printf("Finish autotest of block %s => total score : %.1f %%\n",testName,(float)cpt/((float)max)*100);

}

void testReadAMS(){

	s_song mySong;
	s_song mySongRef;
	int flag=0;
	float mark[5]={0};
	int coeff[5]={1,1, 1, 1, 3};
	int i=0;
	int k=0;
	int cpt=0;
	int tpm;
	int accent;
	int nTicks;
	int note[4];
	char title[MAX_SIZE_TITLE];
	//FILE* pf;
	int array[16][4]={{11, 23, 27, 35},
	{11, 18, 0, 0},
	{11, 23, 0, 0},
	{11, 27, 0, 0},
	{11, 32, 44, 0},
	{11, 18, 0, 0},
	{11, 30, 42, 0},
	{11, 18, 0, 0},
	{11, 23, 27, 35},
	{11, 18, 0, 0},
	{11, 23, 0, 0},
	{11, 27, 0, 0},
	{11, 32, 44, 0},
	{11, 18, 0, 0},
	{11, 30, 42, 0},
	{11, 18, 0, 0}};
	strcpy(mySongRef.title,"Bohemian Rhapsody");
	mySongRef.tpm=144;
	mySongRef.nTicks=16;
	mySongRef.tickTab[0].accent=1;
	
	
	for(k=0;k<16;k++){
		memcpy(mySongRef.tickTab[k].note,array[k],sizeof(int)*5);
		mySongRef.tickTab[k].accent=1;
	}

	
	// Test 1 : vérifier que la fonction renvoie une structure song vide quand le fichier ams n'existe pas
	mySong = readAMS(".ams");
	
	if (mySong.tpm==0 && strcmp(mySong.title,"")==0){
		for(i=0;i<MAX_NUMBER_TICKS;i++){
			if(mySong.tickTab[i].accent==0){
				for(k=0;k<4;k++){
					if(mySong.tickTab[i].note[k]==0){
						cpt++;
					}
				}
			}

		}
	}

	
	if(cpt==4*MAX_NUMBER_TICKS){
		mark[0]=1;
	}else{
		mark[0]=0;
	}

	
	// Test 2
	cpt=0;
	mySong = readAMS("bohemian_rhapsody.ams");
	//pf = fopen("bohemian_rhapsody.ref","r");

	
	//if(pf!=NULL){
		//printf("opening file\n");
		//fgets(title,MAX_SIZE_TITLE,pf);
		
		convert_rl(mySongRef.title);
		convert_rl(mySong.title);
		
		//fscanf(pf,"%d",&tpm);
		if(strcmp(mySongRef.title,mySong.title)==0){
            mark[1]=1;
        }else{
            mark[1]=0;
            fprintf(stderr,"Error ReadAMS on title\n");
        }

        cpt=0;
		if(mySongRef.tpm==mySong.tpm){
            mark[2]=1;
        }else{
            mark[2]=0;
            fprintf(stderr,"Error ReadAMS on TPM\n");
        }
		i=0;

		for(i=0;i<mySong.nTicks;i++){
			//fscanf(pf,"%d\t %d\t %d\t %d\t %d\n",&accent,&note[0],&note[1],&note[2],&note[3]);
			//printf("%d\t %d\t %d\t %d\t %d",accent,note[0],note[1],note[2],note[3]);
			//getchar();
			if(mySong.tickTab[i].accent==mySongRef.tickTab[i].accent){
				for(k=0;k<4;k++){
					if(mySong.tickTab[i].note[k] == mySongRef.tickTab[i].note[k]){
						//printf("1 : %d | 2 : %d\n", mySong.tickTab[i].note[k], mySongRef.tickTab[i].note[k]);
						cpt++;
						//printf("cpt : %d\n", cpt);
					}else{
                        fprintf(stderr,"Error ReadAMS on note %d\n", i);
                    }
				}
			}
		}
		//fscanf(pf,"%d",&nTicks);
		if(mySong.nTicks==mySongRef.nTicks){
            mark[3]=1;
        }else{
            mark[3]=0;
            fprintf(stderr,"Error ReadAMS on N TICK %d/%d\n", nTicks, mySong.nTicks);
        }


		//fclose(pf);

	//}
	//else{
	//printf("ERREUR, pas d'ouverture du fichier 'bohemian_rhapsody.ref'\n");}
	printf("nbT : %d", mySong.nTicks);
	if(cpt==(mySong.nTicks*4)){
		mark[4]=1;
	}else{
        fprintf(stderr,"Error ReadAMS on TICK %d/%d\n", cpt, mySong.nTicks*4);
		mark[4]=0;
	}

	printAutoTestsResults("ReadAMS",mark,coeff,5);
}

void testReadAMP(){
    int cpt=0;
    int i=0;
    int nbtest = 0;
    float mark[AMP_NB_TESTS]={0};
    int coeff[AMP_NB_TESTS] = {1,1,1,3,1,1};

    int testnb = 0; // numéro du test en cours;
    char filename[100]="Playlist_autotest.amp";
    char wrongfile[100]="dtfhesjs.ams";

    // TEST 1 : on verifie que le pointeur est NULL si le fichier amp n'existe pas
    FILE * pf = initAMP(wrongfile);
    if (pf == NULL){
        mark[nbtest] =1; // premier test de fichier vide passé
        #ifdef DEBUG
        printf("test %d AMP ok\n", nbtest);
        #endif
    }else{
    	fclose(pf);
    }

    nbtest ++;
    cpt = 0;

    // TEST 2 : on verifie que le fichier Playlist_autotest.amp s'ouvre bien
    pf = fopen("Playlist_autotest.amp","r");
    if(pf!=NULL){
    	fprintf(pf,"Bohemian Rhapsody\nKnockin’On Heaven’s Door\nClocks\nImagine");
    	fclose(pf);
    }


    pf = initAMP(filename);
	
	
	if (pf == NULL)
	{
		printf("Erreur d'ouverture du fichier AMP\n");
	}
	else
	{
   // if (pf != NULL){
        mark[nbtest]=1;
        #ifdef DEBUG
        printf("test %d AMP ok\n", nbtest);
        #endif
    //}

		char songfilename[MAX_SIZE_TITLE];
		int songnumber = 0;
		while(!feof(pf)){ // read amp file line by line
			readAMP(pf, songfilename);
			nbtest ++;
			if (!strcmp("bohemian_rhapsody.ams", songfilename) && nbtest == 2){
				mark[nbtest]=1;
				#ifdef DEBUG
				printf("test %d AMP ok\n", nbtest);
				#endif
			}
			if (!strcmp("knockin_on_heaven_s_door.ams", songfilename) && nbtest == 3){
				mark[nbtest]=1;
				#ifdef DEBUG
				printf("test %d AMP ok\n", nbtest);
				#endif
			}
			if (!strcmp("clocks.ams", songfilename) && nbtest == 4){
				mark[nbtest]=1;
				#ifdef DEBUG
				printf("test %d AMP ok\n", nbtest);
				#endif
			}
			if (!strcmp("imagine.ams", songfilename) && nbtest == 5){
				mark[nbtest]=1;
				#ifdef DEBUG
				printf("test %d AMP ok\n", nbtest);
				#endif
			}

			printf("%s\n", songfilename);

		}
	}
    printAutoTestsResults("ReadAMP",mark,coeff,AMP_NB_TESTS);

}


void testFrame() {
    int flag=0;
    float mark[4]={0};
    int coeff[4]={1,1,1,1};
    char initFrame[INIT_FRAME_MAX_SIZE];
    char tickFrame[TICK_FRAME_SIZE];
    s_tick myTick;
    int i=0;

    // INIT FRAMERef[]
    char initFrameRef1[INIT_FRAME_MAX_SIZE];
    char initFrameRef2[INIT_FRAME_MAX_SIZE];
    char tickFrameRef1[TICK_FRAME_SIZE];
    char tickFrameRef2[TICK_FRAME_SIZE]; 
    sprintf(initFrameRef1,"#Bohemian Rhapsody,144,16*17\r\n");
    sprintf(initFrameRef2,"#Buhemian Rhapsody,144,16*0d\r\n");
    sprintf(tickFrameRef1,"#0,1,11,23,27,34*2e\r\n");
    sprintf(tickFrameRef2,"#0,1,11,18,00,00*24\r\n");

    s_song mySong;
    // Bohemian Rhapsody, test simple
    mySong.tpm = 144;
    mySong.nTicks = 16;
    strncpy(mySong.title, "Bohemian Rhapsody", MAX_SIZE_TITLE);

    createInitFrame(mySong, initFrame);
    //printf("initFrame=%s\n",initFrame);
    //printf("initFrameRef=%s\n",initFrameRef);

    if(strcmp(initFrame,initFrameRef1)==0){
    	mark[0]=1;
    }else{
        for(i = 0; i< INIT_FRAME_MAX_SIZE; i++){
            printf("%2d", initFrameRef1[i]);
        }
        for(i = 0; i< INIT_FRAME_MAX_SIZE; i++){
            printf("%2d", initFrame[i]);
        }
    	mark[0]=0;
    }

    // Bohemian Rhapsody, test sur le 0 padding du checksum
    mySong.tpm = 144;
    mySong.nTicks = 16;
    strncpy(mySong.title, "Buhemian Rhapsody", MAX_SIZE_TITLE);
    createInitFrame(mySong, initFrame);
    //printf("initFrame=%s\n",initFrame);
    //printf("initFrameRef=%s\n",initFrameRef);

    if(strcmp(initFrame,initFrameRef2)==0){
    	mark[1]=1;
    }else{
    	mark[1]=0;
    }


    // TICK FRAME 1
    myTick.accent = 1;
    myTick.note[0]=11;
    myTick.note[1]=23;
    myTick.note[2]=27;
    myTick.note[3]=34;
    createTickFrame(myTick, tickFrame);
     if(strcmp(tickFrame,tickFrameRef1)==0){
    	mark[2]=1;
    }else{
    	mark[2]=0;
    }

    // TICK FRAME 2
    myTick.accent = 1;
    myTick.note[0]=11;
    myTick.note[1]=18;
    myTick.note[2]=00;
    myTick.note[3]=00;
	
    createTickFrame(myTick, tickFrame);
     if(strcmp(tickFrame,tickFrameRef2)==0){
    	mark[3]=1;
    }else{
    	mark[3]=0;
    }

    printAutoTestsResults("Frame",mark,coeff,4);
}

void testCreateAMS(){
	FILE* pf1=NULL;
	FILE* pf2=NULL;
	int nCorrectLines= 0;
	int nLines=0;
	char buffer[MAX_SIZE_LINE];
	char buffer_ref[MAX_SIZE_LINE];
    float mark[2]={0};
	int coeff[2]={1,9};
	char txtFileName[] = "bohemian_rhapsody.txt";
	char amsFileName[] = "test.ams";
	char refFileName[] = "bohemian_rhapsody.ams";
	
	
	pf1=fopen(amsFileName,"r");
	if(pf1!=NULL){
		printf("Erreur : DESTRUCTION NECESSAIRE DU FICHIER TEST.ams avant les autotests ! \n");
		fclose(pf1);
	}
	
	createAMS(txtFileName,amsFileName);

	pf1=fopen(amsFileName,"r");
	if(pf1==NULL){
		mark[0]=0;
	}else{
		mark[0]=1;
	}
	pf2 = fopen(refFileName,"r");
	if(pf2==NULL){
		printf("in testCreateAMS, ref file name cannot be opened!\n");
	}

	if(pf1!=NULL && pf2!=NULL){
		while(!feof(pf2)){
			fgets(buffer_ref,MAX_SIZE_LINE,pf2);
			nLines++;
			if(!feof(pf1)){
				fgets(buffer,MAX_SIZE_LINE,pf1);
				convert_rl(buffer);
				convert_rl(buffer_ref);
				if(strcmp(buffer,buffer_ref)==0){
					nCorrectLines+=1;
				}
			}
		}
		if(nLines==nCorrectLines){
			mark[1]=1;
		}else{

			mark[1]=(float)nCorrectLines/(float)nLines;
            printf("CREATEAMS : nlines%d/%d\n", nCorrectLines, nLines);
		}

	}
   printAutoTestsResults("CreateAMS",mark,coeff,2);
   fclose(pf2);

}