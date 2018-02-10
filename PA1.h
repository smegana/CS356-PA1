#ifndef PA1_H
#define PA1_H

#include <string>
using namespace std;


string blockencrypt(string, string);
string blockdecrypt(string, string);
string stream(string, string);
string swap(string, string);
string readfile(string);
string stringxor(string, string);
void writefile(string, string);


#endif
