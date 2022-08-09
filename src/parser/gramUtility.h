#ifndef GRAMUTILITY_H_
#define GRAMUTILITY_H_

#include <string>
#include <iostream>
#include "gram.h"

#ifdef _MSC_VER
#pragma warning(disable : 5033)
#endif

std::string tokenToName(int token);
void LLmessage(int expectedToken);
void LLmessage(int expectedToken, int token);
int operatorPriority(int token);

#endif /* !GRAMUTILITY_H_ */
