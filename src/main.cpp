#include <iostream>
#include <fstream>
#include <sstream>
#include <ctime>
#include "model.h"

using namespace std;

void init_random() {
    // use this to get the same random sequence every time
    //srand(1234);

    // use this to get a different random sequence every time
    srand(time(NULL));
}

int main(int argc, char **argv) {
    string infile, model_type;
    int k, size;
    clock_t t1, t2;

    // allow command line "markov filename k size model", or partial
    // command line, prompting for missing items
    if (argc > 1) infile = string(argv[1]);
    else {
        cout << "Enter a filename: ";
        cin >> infile;
    }

    if (argc > 2) k = atoi(argv[2]);
    else {
        cout << "Enter Markov model order (k): ";
        cin >> k;
    }

    if (argc > 3) size = atoi(argv[3]);
    else {
        cout << "Enter size of output text: ";
        cin >> size;
    }

    if (argc > 4) model_type = string(argv[4]);
    else {
        cout << "Enter method (brute (b), map (m), word (w)): ";
        cin >> model_type;
    }

    // get text from input file
    size_t found = infile.find('.');
    if (found != string::npos)
        infile = infile.substr(0, found);

    ifstream in(infile + ".txt");
    if (!in) {
        cerr << "Error opening input file \"" << infile << ".txt\"!" << endl;
        exit(EXIT_FAILURE);
    }
    // get all strings; extra whitespace will be ignored
    ostringstream text;
    while (!in.eof()) {
        string s;
        in >> s;
        text << s << ' ';
    }
    in.close();

    // initialize random number generator
    init_random();

    // create model and output result
    cout << "RESULT:" << endl;
    if (model_type == "brute" || model_type == "b" || model_type == "B") {
        brute_model model(text.str(), k);
        t1 = clock();
        string result = model.generate(size);
        t2 = clock();
        cout << result << endl;
        cout << "Generated " << size << " characters in " << (t2 - t1) / double(CLOCKS_PER_SEC) << " seconds." << endl;
    } else if (model_type == "map" || model_type == "m" || model_type == "M") {
        Markov_model_using_Map model(text.str(), k);
        t1 = clock();
        model.mapping();
        t2 = clock();
        cout << endl << "Mapping completed in " << (t2 - t1) / double(CLOCKS_PER_SEC) << " seconds." << endl << endl;

        t1 = clock();
        string result = model.generate(size);
        t2 = clock();

        cout << result << endl;
        cout << "Generated " << size << " characters in " << (t2 - t1) / double(CLOCKS_PER_SEC) << " seconds." << endl;
    } else if (model_type == "word" || model_type == "w" || model_type == "W") {
        Markov_model_for_Words model(text.str(), k);
        t1 = clock();
        model.mapping();
        t2 = clock();
        cout << endl << "Mapping completed in " << (t2 - t1) / double(CLOCKS_PER_SEC) << " seconds." << endl << endl;

        t1 = clock();
        string result = model.generate(size);
        t2 = clock();
        cout << result << endl;
        cout << "Generated " << size << " words in " << (t2 - t1) / double(CLOCKS_PER_SEC) << " seconds." << endl;
    } else {
        cout << "That model is not yet implemented, sorry." << endl;
        return -1;
    }

    return 0;
}
