#ifndef __MAIN_RESOURCE_H__
#define __MAIN_RESOURCE_H__

//Padrão
#include <iostream>
#include <string>
#include <limits>
#include <vector>
#include "municipios.h"

//Projeto
int inputVerify(std::string vl);
void display_menu();
void displayHelp();
int readOption(int& option, bool& cont, int& arg);
bool executePathConfiguration();
bool executeDBConfiguration();

#endif //__MAIN_RESOURCE_H__
