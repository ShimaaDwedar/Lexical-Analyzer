#ifndef TESTPROGRAM_H
#define TESTPROGRAM_H

#include <iostream>
#include <vector>
#include <map>
#include "minimization.h"
//#include "DFA.h"
#include <bits/stdc++.h>
using namespace std;


void split (int line_num, string program, vector <char> punctuation, vector <string> keyWords, map<int, DFA_Graph> graph);
void validation(int line_num, map<int, DFA_Graph> graph, string word,vector <char> punctuation, vector <string> keyWords);

#endif
