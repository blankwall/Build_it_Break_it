#include "types.h"
#include "logfile.h"
#include "state.h"
#include "parse.h"
#include "batch.cpp"
#include "util.h"
#include <unistd.h>
#include <iostream>
#include <sstream>


using namespace std;

int main(int argc, char **argv) {
    // ArgParser
    extern char *optarg;
    extern int optind;
    opterr = 0;
    string timestamp="", name="", key="", room="", batch_ ="";
    int c, employee=0, guest=0,arrival=0, leaving=0;
    while ((c = getopt(argc, argv, "AT:K:E:G:LR:B:")) != -1) {
        switch (c) {
            case 'T':
                timestamp = optarg;
                break;
            case 'K':
                key = optarg;
                break;
            case 'E':
                employee = 1;
                name = optarg;
                break;
            case 'G':
                guest = 1;
                name = optarg;
                break;
            case 'A':
                arrival = 1;
                break;
            case 'L':
                leaving = 1;
                break;
            case 'R':
                room = optarg;
                break;
            case 'B':
                batch_ = optarg;
                break;
            default:
                cout << INVALID_STR << endl;
                return -1;
        }
    }
    
    if(!batch_.empty()){
        if(!timestamp.empty() || !key.empty() || !name.empty()||employee||guest||arrival||leaving){
            cout << INVALID_STR << endl;
            return -1;
        }
        return batch(batch_);
    }
    
    string fn = argv[argc-1];
    if(fn == timestamp || fn == name || fn == key || fn == room) {
        cout << INVALID_STR << endl;
        return -1;
    }
    
    if(timestamp.empty() || key.empty() || name.empty()) {
        cout << INVALID_STR << endl;
        return -1;
    }
    
    if((employee && guest) || (arrival && leaving)) {
        cout << INVALID_STR << endl;
        return -1;
    }
    if(arrival && (!employee && !guest && room.empty())) {
        cout << INVALID_STR << endl;
        return -1;
    }
    
    if(leaving && (!employee && !guest && room.empty())) {
        cout << INVALID_STR << endl;
        return -1;
    }
    
    if(!is_alpha(name)){
        cout << INVALID_STR << endl;
        return -1;
    }
    
    if(!is_alphanumeric(key)){
        cout << INVALID_STR << endl;
        return -1;
    }
    
    
    LogFile lf;
    int x = lf.open(fn, key, false);
    if(!x){
        cout << INVALID_STR << endl;
        return -1;
    }
    
    if(x == -1){
        cout <<  SECERR_STR  << endl;
        return -1;
    }
    
    State st;
    
    //reading log catching up state
    while(lf.hasNext()) {
        string buf = lf.readEntry();
        Entry e;
        if(!Parse::deserialize(buf, &e)) {
            cout << INTERR_STR << endl;
            return -1;
        }
        if(!Parse::insert(st, &e)) {
            cout << INTERR_STR << endl;
            return -1;
        }
    }
    
    Entry e2;
    e2.time = strtoul(timestamp.c_str(), NULL, 0);
    e2.room = NO_ROOM;
    e2.is_employee = guest ? false:true;
    e2.type = -1;
    e2.name = name;
	
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
        cout << INVALID_STR << endl;
        return -1;
    } else {
        string logger = Parse::serialize(&e2);
        lf.appendEntry(logger);
        lf.close();
    }
    
    return 0;
}
