#include "types.h"
#include "logfile.h"
#include "state.h"
#include "parse.h"
#include <unistd.h>
#include <iostream>
#include <sstream>
using namespace std;

int batch(string& filename){
    ifstream file;
    file.open(filename);
    if (!file){
        return 1;
    }

    string line;
    vector<string*> commands;
    while(getline(file, line)){
        string *temp = new string(line);
        commands.push_back(temp);
    }
    
    
    //FIXME inefficient
    for(int i = 0; i < commands.size(); ++i){
        istringstream iss2(*commands[i]);
        string timestamp="", name="", key="", room="", fn="";
        int employee=0, guest=0,arrival=0, leaving=0;
        
        string k;
        while (iss2 >> k){
            if(k == "-K"){
                iss2 >> key;
                continue;
            }
            if(k == "-T"){
                iss2 >> timestamp;
                continue;
            }
            if(k == "-G"){
                iss2 >> name;
                guest = 1;
                continue;
            }
            if(k == "-E"){
                iss2 >> name;
                employee = 1;
                continue;
            }
            if(k == "-A"){
                arrival = 1;
                continue;
            }
            if(k == "-L"){
                leaving = 1;
                continue;
            }
            if(k == "-R"){
                iss2 >> room;
                continue;
            }
            if(k == "-B"){
                cerr << INVALID_STR << endl;
                return -1;
            }
        }
        fn = k;
        if(fn == timestamp || fn == name || fn == key || fn == room) {
            cerr << INVALID_STR << endl;
            continue;
        }
        
        
        if(timestamp.empty() || key.empty() || name.empty()) {
            cerr << INVALID_STR << endl;
        }
        
        if((employee && guest) || (arrival && leaving)) {
            cerr << INVALID_STR << endl;
            continue;
        }
        if(arrival && (!employee && !guest && room.empty())) {
            cerr << INVALID_STR << endl;
            continue;
        }
        
        if(leaving && (!employee && !guest && room.empty())) {
            cerr << INVALID_STR << endl;
            continue;
        }

        LogFile lf;
        int x = lf.open(fn, key, false);
        if(!x){
            cerr << INVALID_STR << endl;
            continue;
        }
        
        if(x == -1){
            cerr << SECERR_STR << endl;
            continue;
        }

        State st;
        
       //reading log catching up state
        while(lf.hasNext()) {
            string buf = lf.readEntry();
            Entry e;
            if(!Parse::deserialize(buf, &e)) {
                cerr << INTERR_STR << endl;
                continue;
            }
            if(!Parse::insert(st, &e)) {
                cerr << INTERR_STR << endl;
                continue;
            }
        }
    
        Entry e2;
        e2.time = strtoul(timestamp.c_str(), NULL, 0);
        e2.name = name;
        
        if(guest)
            e2.is_employee = false;
        else
            e2.is_employee = true;
        
        if (arrival) {
            if(room.empty()){
                e2.type = ENTER_GALLERY;
                e2.room = NO_ROOM;
            }
            else{
                e2.type = ENTER_ROOM;
                e2.room = strtoul(room.c_str(), NULL, 0);
            }
        }
        
        if (leaving) {
            if(room.empty()){
                e2.type = LEAVE_GALLERY;
                e2.room = NO_ROOM;
            }
            else{
                e2.type = LEAVE_ROOM;
                e2.room = strtoul(room.c_str(), NULL, 0);
            }
        }

        if(!Parse::insert(st, &e2)) {
            cerr << INTERR_STR << endl;
            continue;
        } else {
            string logger = Parse::serialize(&e2);
            lf.appendEntry(logger);
            lf.close();
        }
    }
    return 0;
}
