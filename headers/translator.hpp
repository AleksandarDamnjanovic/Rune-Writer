#ifndef TRANSLATOR_HPP
#define TRANSLATOR_HPP

#include <string>
#include <string.h>
#include <iostream>

#ifdef __cplusplus
    extern "C"{
#endif

void getLatin(char* source, char* destination);
void getRunic(char* source, char* destination);

#ifdef __cplusplus
    }
#endif

void textCorrection();
void runicCorrection();
void getSource(char* source);
void writeToDestination(char* destination, char* fontAddress, bool toRunic);

#endif