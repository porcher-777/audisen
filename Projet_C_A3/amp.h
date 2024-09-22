#ifndef AUDISEN2024_AMP_H
#define AUDISEN2024_AMP_H

#include "define.h"


FILE* initAMP(char* filename);

void readAMP(FILE* pf, char * song_filename);

void closeAMP(FILE* pf);

void stringProcessor(char* song_filename);

void underscoreRemover(char* song_filename);

void exeAMP(char* playlist_filename, char** titres, int* nbsong);

#endif //AUDISEN2024_AMP_H