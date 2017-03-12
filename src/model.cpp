//
// Created by huan on 3/12/17.
//

#include <string>
#include <sstream>
#include <vector>
#include <map>
#include <iostream>
#include "model.h"

using namespace std;

brute_model::brute_model() {
    data = "";
    order = 0;
}

// Brute force character generator
brute_model::brute_model(string s, int k) {
    data = s;
    order = k;
}

/**
 * Generate random text based on Markov model using brute force method
 * @param size  the size of the random text
 * @return string   it is the final result of generated text
 */
string brute_model::generate(int sz) {
    // copy first k characters to back to simulate wrap-around
    string working_data = data + data.substr(0, order);

    // pick random k-character substring as initial seed
    int start = rand() % data.length();
    string seed = working_data.substr(start, order);

    // maintain a vector of char as the candidate char for the next word sequence
    vector<char> list;

    string answer;
    answer.reserve(sz);

    for (int i = 0; i < sz; i++) {
        list.clear();
        size_t pos = working_data.find(seed);
        // while there are still matching sequence, push the candidate char to the list, and attempt to
        // match any other potential sequence until there is no more
        while (pos != string::npos && pos < data.length()) {
            char c = working_data[pos + order];
            list.push_back(c);
            pos = working_data.find(seed, pos + 1);
        }
        // random pick a char from the candidate.
        char c = list[rand() % list.size()];
        answer.push_back(c);
        // seed now becomes the new sequence
        seed = seed.substr(1) + c;
    }
    return answer;
}

Markov_model_using_Map::Markov_model_using_Map(string s, int k) {
    data = s;
    order = k;
}

/**
 * Generate the map for map based Markov Model
 */
void Markov_model_using_Map::mapping() {
    vector<char> list;
    char c;
    string seed;
    size_t pos;

    string working_data = data + data.substr(0, order);

    // rewrite: this works but not obvious to understand
//    for (int i = 0; i < data.size(); i++) {
//        list.clear();
//        seed = working_data.substr(i, order);
//
//        if (!word_map.count(seed)) {
//            pos = working_data.find(seed);
//            while (pos != string::npos && pos < data.length()) {
//                char c = working_data[pos + order];
//                list.push_back(c);
//                pos = working_data.find(seed, pos + 1);
//            }
//            word_map[seed] = list;
//        }
//    }

    for (int i = 0; i < data.size(); i++) {
        seed = working_data.substr(i, order);
        c = working_data[i + order];
        word_map[seed].push_back(c);
    }
}

string Markov_model_using_Map::generate(int sz) {
    string working_data = data + data.substr(0, order);

    int start = rand() % data.length();

    vector<char> list;
    string seed = working_data.substr(start, order);
    string answer;
    answer.reserve(sz);

    for (int i = 0; i < sz; i++) {
        list = word_map[seed];
        char c = list[rand() % list.size()];
        answer.push_back(c);
        seed = seed.substr(1) + c;
    }

    return answer;
}

Markov_model_for_Words::Markov_model_for_Words(string s, int k) {
    data = s;
    order = k;
    whitespace_counter = 0;
}

void Markov_model_for_Words::mapping() {
    vector<string> list;
    string seed;
    string modi_seed;
    size_t pos = 0;
    map<string, vector<string>>::iterator it;

    // copy first k words to back to simulate wrap-around
    for (int i = 0; i < order; i++) {
        pos = data.find(' ', pos + 1);
    }
    string working_data = data.substr(0, pos + 1) + data + data.substr(0, pos + 1); // include the ending space
    pos = 0;

    // count how many white space (ie. how many words) in the doc
    for (char &c: data) {
        if (c == ' ')
            whitespace_counter++;
    }

    size_t initialpos = 0;
    int seedlength;
    for (int i = 0; i < whitespace_counter; i++) {
        initialpos = pos = 0;
        list.clear();

        for (int j = 0; j < i; j++) { //find where to start each loop
            initialpos = working_data.find(' ', initialpos + 1);
        }
        pos = initialpos; // let pos = initialpos and the next loop is used
        for (int j = 0; j < order; j++) {// to find the substr of the seed.
            pos = working_data.find(' ', pos + 1);
        }
        //if (pos != string::npos) { // if the seed is found, keep going, otherwise skip

        seedlength = pos - initialpos;
        seed = working_data.substr(initialpos, seedlength);// format: " xxxx xxxx xxxx" (k=3)
        it = Map.find(seed);
        if (it == Map.end()) {
            modi_seed = seed + " "; // to include the first whitespace after the seed.
            /*
            do this because of the situation like seed = "years and 40" will match
            "years and 400", which will cause a bug finding the subsequent word.
            */
            initialpos = working_data.find(modi_seed);
            while ((initialpos != string::npos) && (initialpos < data.length())) {
                pos = working_data.find(' ', initialpos + seedlength + 1);
                if (pos != string::npos) {
                    string s = working_data.substr(initialpos + seedlength, pos - initialpos - seedlength);
                    //cout << "s = \"" << s << "\"" << endl;
                    list.push_back(s);
                }
                initialpos = working_data.find(modi_seed, initialpos + 1);
                //string foo;
                //getline(cin, foo);
            }
            Map[seed] = list;
        }
        //}
    }
}

string Markov_model_for_Words::generate(int sz) {
    size_t pos = 0;
    // copy first k words to back to simulate wrap-around
    for (int i = 0; i < order; i++) {
        pos = data.find(' ', pos + 1);
    }
    string working_data = data + data.substr(0, pos);

    size_t initialpos = 0;
    // pick random k-words substring as initial seed
    int start = rand() % whitespace_counter;
    for (int i = 0; i < start; i++) {
        initialpos = working_data.find(' ', initialpos + 1);
    }
    pos = initialpos;
    for (int i = 0; i < order; i++) {
        pos = working_data.find(' ', pos + 1);
    }
    string seed = working_data.substr(initialpos, pos - initialpos);

    vector<string> list;

    string answer = "";
    map<string, vector<string>>::iterator it;
    for (int i = 0; i < sz; i++) {
        pos = 0;

        // FOR DEBUG
        it = Map.find(seed);
        if (it == Map.end()) {
            cerr << "\"" << seed << "\" not found" << endl;
            exit(1);
        }


        list = Map.find(seed)->second;
        string s = list[rand() % list.size()];
        answer += s;
        //cout << "s = " << s << endl;
        //cout << "answer = "<<answer << endl;
        pos = seed.find(' ', 1);
        //cout << "Pos = " << pos << endl;

        seed = seed.substr(pos) + s;
        //cout << "now seed =" << seed << "\"" << endl;
    }

    return answer;
}