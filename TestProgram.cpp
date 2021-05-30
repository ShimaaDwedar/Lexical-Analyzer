#include "TestProgram.h"
#include <iostream>
#include <fstream>
#include <bits/stdc++.h>
#include <regex>
#include <iterator>
using namespace std;
ofstream file;
bool split_spc (int i, string str){
     if (i==0 || i == str.length()-1) return 0;
     return (isdigit(str[i-1]) ||isalpha(str[i-1])) && (isdigit(str[i+1]) ||isalpha(str[i+1]));
}
string removeSpaces(string str){
	string new_str="";
	for (int i = 0; i<str.length(); i++) {
		if (str.at(i) != ' ' || (str.at(i) == ' ' && split_spc(i, str)) )
			new_str.push_back(str.at(i));
	}
	return new_str;
}



pair<bool,string> check_regex(string x, map<string, int> next)
{
    pair<bool, string> output = {false,""};
    for (auto item : next)
    {
        string y = "["+item.first+"]";
        regex temp (y);
        if (regex_match(x, temp)) // matched
        {
            output.first = true;
            output.second += item.first;
            break;
        }
    }
    return output;
}

void validation(int line_num, map<int, DFA_Graph> graph, string word,vector <char> punctuation, vector <string> keyWords) {
    int state_id = 1;
    int last_accepted_state = 0; //id of accepted
    int last_state; // last id reached
    bool flag = true;
    int new_start = 0;
    file << "shimaaaa" << endl;
    for (int i = 0; i < word.size(); i++)
    {
        map<string, int> next = graph[state_id].next_state;
        string temp="";
        temp+= word[i];

        pair<bool, string> result = check_regex(temp, next);
        if (result.first) //found the char in DFA.
        {
            if (graph[next[result.second]].acceptance_state){
                    last_accepted_state = next[result.second];
                    new_start = i+1 ;
            }

            last_state=next[result.second];
            state_id=next[result.second];
        }
        else
        {
            flag = false;
            if (i == 0){
                if(find(punctuation.begin(), punctuation.end(), temp[0]) != punctuation.end()){
                    file << temp << endl;
                }else{
                    file <<"ERROR on line " << line_num<<endl;
                }if (i+1 < word.length()){
                    validation(line_num, graph,  word.substr(i+1),punctuation,keyWords);
                    return;
                }else
                    return;
            }else{

                if(find(keyWords.begin(), keyWords.end(), temp) != keyWords.end()){
                    file << temp << endl;
                }
                else if (last_accepted_state > 0){
                    file <<graph[last_accepted_state].name << endl;
                }
                validation(line_num, graph,  word.substr(new_start ),punctuation,keyWords);
                return;
            }
        }
    }
    if (flag&&last_accepted_state==last_state)
    {
        file <<graph[last_accepted_state].name << endl;
    }
    else{
        //should call Recovary function .
        file <<graph[last_accepted_state].name << endl;
    }
}
void Parse(int line_num, map<int, DFA_Graph> graph, vector <char> punctuation, vector <string> keyWords, vector <string> splitted )
{
    file.open("output.txt", ofstream::app);
    for(auto it:splitted){
        if (count (punctuation.begin(), punctuation.end(), it[0])){
            file << it << endl;
        }else if (count (keyWords.begin(),keyWords.end(), it)){
            file << it << endl;
        }else{
            validation(line_num, graph,it,punctuation,keyWords);
        }
    }
    file.close();
}


void split(int line_num, string program, vector <char> punctuation, vector <string> keyWords, map<int, DFA_Graph> graph) {
    vector <string> splitted;
    string token;
    for (int i =0; i < program.length(); i++){
        if (program[i] == ' '){
            if(token.size()>0){
                splitted.push_back(token);
                token = "";
            }
        }else if (count(punctuation.begin(), punctuation.end(), program[i])){
            if(token.size()>0){
                splitted.push_back(token);
                token = "";
            }
            splitted.push_back(token+program[i]);
            token = "";
        }else{
            token += program[i];
            if ( i == program.length()-1)
                splitted.push_back(token);
        }
    }

    Parse(line_num, graph,punctuation, keyWords,splitted);

}
