#include "parse.h"
using namespace std;


string Parse::serialize(const Entry* e){
    try {
        ostringstream oss;
        oss << e->time;
        oss << ":";
        oss << e->room;
        oss << ":";
        oss << e->is_employee;
        oss << ":";
        oss << e->type;
        oss << ":";
        oss << e->name;
    
        return oss.str();
    } catch(...) {
        return nullptr;
    }
    
}

bool Parse::deserialize(const string& s, Entry* entry){
    string token = "";
    istringstream stream(s);
    int x;

    token = delimit(stream);
    entry->time = strtoul(token.c_str(), NULL, 0);
    
    token = delimit(stream);
    x = strtol (token.c_str(), NULL, 0);
    entry->room = NO_ROOM;
    if(x >= 0) {
        entry->room = x;
    }
    
    token = delimit(stream);
    x = strtoul (token.c_str(), NULL, 0);
    if(x == 0) {
        entry->is_employee = false;
    } else if(x == 1) {
        entry->is_employee = true;
    } else {
        return false;
    }
    
    token = delimit(stream);
    x = strtoul (token.c_str(), NULL, 0);
    if(x < 0 || x > 4) {
        return false;
    }
    entry->type = x;
    
    token = delimit(stream);
    if(token.length() == 0 || !is_alpha(token)) {
        return false;
    }
    entry->name = token;
    
    return true;
}

string Parse::delimit(istringstream& stream){
    char inp;
    string temp = "";
    while(stream >> inp){
        if(inp == ':')
            break;
        temp += inp;
    }
    return temp;
}

bool Parse::insert(State& state, Entry *e){
    int x = e->type;
    bool ret = false;
    switch (x) {
        case ENTER_GALLERY:
            ret = state.enterGallery(e->time, e->name, e->is_employee);
            break;
        case ENTER_ROOM:
            ret = state.enterRoom(e->time, e->name, e->is_employee, e->room);
            break;
        case LEAVE_ROOM:
            ret = state.exitRoom(e->time, e->name, e->is_employee, e->room);
            break;
        case LEAVE_GALLERY:
            ret = state.exitGallery(e->time, e->name, e->is_employee);
            break;
    }
    return ret;
}

















