#ifndef AUDISEN2024_AUTOTEST_H
#define AUDISEN2024_AUTOTEST_H

#include "define.h"

void printAutoTestsResults(char* testName, float mark[], int coeff[], int nTests);
void testCreateAMS();
void testReadAMS();
void testReadAMP();
void testFrame();

#endif //AUDISEN2024_AUTOTEST_H