#include "types.h"
#include "logfile.h"
#include "state.h"
#include "parse.h"
#include "formatter.h"
#include "util.h"
#include <unistd.h>
#include <iostream>

using namespace std;

int main(int argc, char **argv) {
    // ArgParser
    extern char *optarg;
    extern int optind;
    opterr = 0;
    string name="", key="", lower1="",lower2="",upper1="",upper2="";
    int c, alist=0, base=0, time=0, together=0,room=0,employee=0, guest=0,state=0,html=0;
    vector<string> names;
    while ((c = getopt(argc, argv, "K:L:U:BHSIRTAE:G:")) != -1) {
        switch (c) {
            case 'T':
                time = 1;
                break;
            case 'B':
                base = 1;
                break;
            case 'K':
                key = optarg;
                break;
            case 'E':
                if(!employee && !guest){
                    employee = 1;
                    name = optarg;
                }
                else{
                    names.push_back(optarg);
                }
                break;
            case 'G':
                if(!employee && !guest){
                    guest = 1;
                    name = optarg;
                }
                else{
                    names.push_back(optarg);
                }
                break;
            case 'A':
                alist = 1;
                break;
            case 'L':
                if (lower1.empty()) {
                    lower1 = optarg;
                }
                else if (lower2.empty()){
                    lower2 = optarg;
                }
                else{
                    cout << INVALID_STR << endl;
                    return -1;
                }
                break;
            case 'U':
                if (upper1.empty()) {
                    upper1 = optarg;
                }
                else if (upper2.empty()){
                    upper2 = optarg;
                }
                else{
                    cout << INVALID_STR << endl;
                    return -1;
                }
                break;
            case 'R':
                room = 1;
                break;
            case 'H':
                html = 1;
                break;
            case 'S':
                state = 1;
                break;
            case 'I':
                together = 1;
                break;
            default:
                cout << INVALID_STR << endl;
                return -1;
        }
    }
    
    string fn = argv[argc-1];
    
    if(fn.empty()){
        cout << INVALID_STR<<endl;
        return -1;
    }
    
    //SAFETY CHECKS
    if(fn == lower1 || fn == lower2 || fn == name || fn == key || fn == upper1 || fn == upper2) {
        cout << INVALID_STR << endl;
        return -1;
    }
    
    if( key.empty() || !is_alphanumeric(key)) {
        cout << INVALID_STR << endl;
        return -1;
    }
    
    if((employee && guest)) {
        cout << INVALID_STR << endl;
        return -1;
    }
    
    // ENSURE OPTIONS NOT SELECTED TOGETHER
    if((state && (room || time || together || alist || base))){
        cout << INVALID_STR << endl;
        return -1;
    }
    
    if((room && (time || together || alist || base))){
        cout << INVALID_STR << endl;
        return -1;
    }
    
    if((time && ( together || alist || base))){
        cout << INVALID_STR << endl;
        return -1;
    }
    
    if((together && (alist || base))){
        cout << INVALID_STR << endl;
        return -1;
    }
    if((alist && (base))){
        cout << INVALID_STR << endl;
        return -1;
    }
    
    if(base){
        if(lower1.empty() || lower2.empty() || upper1.empty() || upper2.empty()){
            cout << INVALID_STR << endl;
            return -1;
        }
    }
    
    if(alist){
        if(lower1.empty() || upper1.empty()){
            cout << INVALID_STR << endl;
            return -1;
        }
    }
    
    if(room){
        if(!employee && !guest){
            cout << INVALID_STR << endl;
            return -1;
        }
    }
    
    if(time && html){
        cout << INVALID_STR << endl;
        return -1;
    }
    
    //END CHECKS
    if(!name.empty()){
        if(!is_alpha(name)){
            cout << INVALID_STR << endl;
            return -1;
        }
    }
    
    if(!is_alphanumeric(key)){
        cout << INVALID_STR << endl;
        return -1;
    }
    
    
    LogFile lf;
    int x = lf.open(fn, key, true);
    if(!x){
        cout << INVALID_STR << endl;
        return -1;
    }
    
    if(x == -1){
        cout <<  SECERR_STR  << endl;
        return -1;
    }
    
    State st;
    
    if(together){
        typedef struct room{
            int peeps;
            int room;
        }rooms;
        vector<rooms*> loop;
        vector<int> print;
        names.push_back(name);
        
        int interesting = names.size();
        while(lf.hasNext()) {
            string buf = lf.readEntry();
            Entry e;
            if(!Parse::deserialize(buf, &e)) {
                cout << INTERR_STR << endl;
                return -1;
            }
            if(!Parse::insert(st, &e)) {
                cout << INTERR_STR << endl;
                //cout << e->type;
                return -1;
            }
            if(e.room == -1)
                continue;
            int found = 0;
            for(int i = 0; i < names.size(); ++i){
                if(e.name == names[i]){
                    found = 1;
                }
            }
            if(found){
                int new_room = 0;
                for(int zebra = 0; zebra < loop.size(); ++zebra){
                    if(e.room == loop[zebra]->room){
                        Person* p =  st.getPerson(e.name, e.is_employee);
                        if(p->room_times.size() % 2 != 0){
                            loop[zebra]->peeps += 1;
                            if(loop[zebra]->peeps == interesting){
                                print.push_back(loop[zebra]->room);
                            }
                            new_room = 1;
                        }
                        else {
                            loop[zebra]->peeps -= 1;
                            new_room = 1;
                        }
                    }
                }
                if(!new_room){
                    rooms* temp = new rooms;
                    temp->peeps = 1;
                    temp->room = e.room;
                    loop.push_back(temp);
                    if(interesting == 1){
                        print.push_back(temp->room);
                    }
                }
            }
        }
        // for(int i =0; i < print.size(); ++i){
        //     cout << print[i] << endl;
        // }
        sort(print.begin(), print.end());
        Formatter* fmt;
        if(html) {
            fmt = new Html();
        } else {
            fmt = new Plain();
        }
        fmt->printRoomVector(print);
        delete fmt;
        return 0;
    }
    
    
    while(lf.hasNext()) {
        string buf = lf.readEntry();
        Entry e;
        if(!Parse::deserialize(buf, &e)) {
            cout << INTERR_STR << endl;
            return -1;
        }
        if(!Parse::insert(st, &e)) {
            cout << INTERR_STR << endl;
            //cout << e->type;
            return -1;
        }
    }
    
    Formatter* fmt;
    if(html) {
        fmt = new Html();
    } else {
        fmt = new Plain();
    }
    int ret = 0;
    
    
    // Output
    if (state){
        //print current state of log
        
        fmt->printState(st);
    }
    else if(room){
        //print all rooms person has been in
        Person* p =  st.getPerson(name, employee);
        fmt->printRooms(*p);
    }
    else if(time){
        //print how much time a person has spent in the gallery
        // FIXME: Move output to formatter
        Person* p =  st.getPerson(name, employee);
        if (p != nullptr) {
            int total;
            total = p->gallery_times[0];
            if(p->in_gallery){
                total = (st.last_time - total);
            }
            else{
                total = p->gallery_times[1] - total;
            }
            cout << total << endl;
        }
    }
    else if(alist){
        // print employees in gallery during certain time frame
        
        unsigned int low = strtoul(lower1.c_str(), NULL, 0);
        unsigned int high = strtoul(upper1.c_str(), NULL, 0);
        fmt->printEmployeesBound(low,high, st);
    } else if(base){
        unsigned int low1 = strtoul(lower1.c_str(), NULL, 0);
        unsigned int high1 = strtoul(upper1.c_str(), NULL, 0);
        unsigned int low2 = strtoul(lower2.c_str(), NULL, 0);
        unsigned int high2 = strtoul(upper2.c_str(), NULL, 0);
        fmt->printEmployeesAtTime(low1,low2, high1, high2,  st);
        
    }
    
    
    delete fmt;
    return ret;
}


