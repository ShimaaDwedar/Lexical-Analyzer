#include "input_rules.h"
#include "parse_rules.h"
#include "minimization.h"
#include "TestProgram.h"
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <bits/stdc++.h>
using namespace std;
bool visit[1000];
void display (state st){
    visit[st.id] = 1;
   // cout << st.id <<" " << st.transition.size()<<endl;
    for (auto edg: st.transitions){
        cout << std::setw(4) << st.id <<"       "<< (edg.next)->id<< "       ";
        cout << std::setw(4) << edg.input_symbol << "       "<<(edg.next)->accepted<<"   ";
        cout << std::setw(4) <<(edg.next)->accepted_language <<endl;
        if (!visit[edg.next->id]){
            display (*(edg.next));
        }
    }
}

void display_compined_NFA(automata automata){
    cout <<"        ...................."<<endl;
    cout <<"        |Final Combined NFA|"<<endl;
    cout <<"        ...................."<<endl;
    cout << std::left;
    cout << "FROM       TO       INPUT       Accepted\n" ;
    display(*automata.start);
    memset(visit, 0, sizeof(visit));
    cout <<"-----------------------------\n"<<endl;
}

void extract(string line) {
	vector<string> parts;
	if ((line.find('=') < line.length()) && (line.at(line.find('=')-1) != '\\') && !(line.find(':') < line.length())){
        cout << line << "\n";
        line = parse.removeExtraSpaces(line);
		parts = parse.splits(line, "=");
		automata a ;
		a = cal.language_NFA(parts[1],Languages);
		Languages.push_back({parts[0],a});
	}else if (line.find(':') < line.length()) {
	    cout << line << "\n";
		line = parse.removeExtraSpaces(line);
		parts = parse.splits(line, ":");
		automata a ;
		a = cal.language_NFA (parts[1],Languages);
		patterns.push_back({parts[0],a});
	}
	else if(line[0]=='{'){
		line.erase(remove(line.begin(), line.end(), '{'), line.end());
		line.erase(remove(line.begin(), line.end(), '}'), line.end());
		parts = parse.splits(line, " ");
		for (int i = 0;i < parts.size(); i++) {
				keyWords.push_back(parts[i]);
		}
	}else if(line[0]=='['){
        line = parse.removeSpaces(line);
		line.erase(remove(line.begin(), line.end(), '['), line.end());
		line.erase(remove(line.begin(), line.end(), ']'), line.end());
		line.erase(remove(line.begin(), line.end(), ' '), line.end());
		for (int i = 0;i < line.size();i++) {
                if (line[i]!='\\' || (line[i]=='\\'  && line[i+1] == '\\' )){
                    punctuation.push_back(line[i]);
                }
		}
	}

}
int main(){
	fstream my_file;
	my_file.open("rules.txt", ios::in);

	if (!my_file)
		cout << "No such file";
	else {
		string line;
		while (getline(my_file, line)) { //read data from file object and put it into string.
			extract(line);
		}
	}
	my_file.close();
	state *s=new state ;
	automata final_result = cal.combine(patterns,s);

	display_compined_NFA(final_result);

    map<int, DFA_Graph> Final_Graph=Subset_Construction(final_result.start);

    int line_num = 0;
    my_file.open("test.txt", ios::in);
    if (!my_file)
		cout << "No such file";
	else {
		string line;
		while (getline(my_file, line)) {
           line_num++;
           split(line_num,line, punctuation, keyWords,Final_Graph);
		}

	}
	my_file.close();


	return 0;
}
