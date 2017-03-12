//
// Created by huan on 3/12/17.
//

#ifndef MARKOV_MODEL_MODEL_H
#define MARKOV_MODEL_MODEL_H

#include <string>
#include <map>
#include <unordered_map>
#include <vector>
using namespace std;

class brute_model {
public:
    brute_model();
    brute_model(string s, int k);

    virtual string generate(int sz);

protected:
    string data;
    int order;
};

class Markov_model_using_Map :public brute_model {
public:
    Markov_model_using_Map(string s, int k);

    void mapping();
    string generate(int sz);

private:
    map<string, vector<char>> Map;
};

class Markov_model_for_Words :public brute_model {
public:
    Markov_model_for_Words(string s, int k);

    void mapping();
    string generate(int sz);

private:
    map<string, vector<string>> Map;

    int whitespace_counter; //word counter
};

#endif //MARKOV_MODEL_MODEL_H
