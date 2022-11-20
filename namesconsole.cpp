/***************************************************************************
 * nameservercpp  -  Program to serve of last name statistics
 *
 * copyright : (C) 2018,2022 by Jim Skon, Kenyon College
 *
 * This program create an index US Census name
 * Data on the frequency of names in response to requestes.  
 * It then allows you to look up any name, giving the 10 closest matches
 * 
 *
 ***************************************************************************/
#include <stdlib.h>
#include <sys/stat.h>
#include <iostream>
#include <fstream>
#include <map>
#include <algorithm>
#include "NameMap.h"
#include "NameEntry.h"

using namespace std;

const string path = "namedata/";
const string lastFile = path+"dist.all.last";
const string maleFile = path+"dist.male.first";
const string femaleFile = path+"dist.female.first";


/* Server main line,create name MAP, wait for and serve requests */
int main() {
    // Build the name maps
    NameMap lastMap(lastFile);
    NameMap femaleMap(femaleFile);
    NameMap maleMap(maleFile);
    vector<NameEntry> matchList;

    NameEntry entry;
    string name;
    char sel;
    // Check for map creation success
    if (!lastMap.NameMapSuccess() || !femaleMap.NameMapSuccess() || !maleMap.NameMapSuccess()) {
        cout << "failure to load name data!" << endl;
        exit(1);
    }
    cout << "Name data loaded!" << endl;
    cout << "Look up US name statistics" << endl;
    /* See what user wants */
    do {

        cout << "Select one: " << endl << "  1. A Last name" << endl
                << "  2. Female first names" << endl << "  3. Male First Names" << endl
                << "  0. End Program." << endl << "Select:";
        cin >> sel;

        if (sel == '1' || sel == '2' || sel == '3') {

            cout << "Ender name to check:";
            cin >> name;

            /* Convert verse to upper case*/
            transform(name.begin(), name.end(), name.begin(), ::toupper);

            // Try to find the record
            switch (sel) {
                case '1':
                    matchList = lastMap.getMatches(name);
                    break;
                case '2':
                    matchList = femaleMap.getMatches(name);
                    break;
                case '3':
                    matchList = maleMap.getMatches(name);
                    break;
                default:
                    cout << "Invalid option \"" << sel << "\"." << endl; 
            }
 
            cout << "10 best matches:" << endl;
            cout << "Name\t%\tRank" << endl;
            for (int i = 0; i < matchList.size(); i++) {
                cout << matchList.at(i).name << "\t" <<  matchList.at(i).percent << "\t" << matchList.at(i).rank << endl;
            }

        }
    } while (sel != '0');
}
