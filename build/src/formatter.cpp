#include <string>
#include <iostream>
#include "state.h"
#include "formatter.h"
using namespace std;

Formatter::~Formatter() {}

Html::~Html() {}

void Html::printState(const State& st) {
    header();
    cout << "<tr><th>Employee</th><th>Guest</th></tr>" << endl;
    
    vector<Person*> employees;
    vector<Person*> guests;
    
    for(vector<Person*>::const_iterator eit = st.employees.begin(); eit != st.employees.end(); ++eit) {
        if((*eit)->in_gallery) {
            employees.push_back(*eit);
        }
    }
    for(vector<Person*>::const_iterator git = st.guests.begin(); git != st.guests.end(); ++git) {
        if((*git)->in_gallery) {
            guests.push_back(*git);
        }
    }
    
    sort_people(employees);
    sort_people(guests);
    
    // Iterate over employees and guests. We need to display them side by side.
    vector<Person*>::const_iterator eit = employees.begin();
    vector<Person*>::const_iterator git = guests.begin();
    vector<Person*>::const_iterator eit_end = employees.end();
    vector<Person*>::const_iterator git_end = guests.end();
    while(eit != eit_end || git != git_end) {
        string estr = "";
        string gstr = "";
        
        if(eit != eit_end) {
            estr = (*eit)->name;
            ++eit;
        }
        if(git != git_end) {
            gstr = (*git)->name;
            ++git;
        }
        cout << "<tr><td>" << estr << "</td><td>" << gstr << "</td></tr>" << endl;
    }
    cout << "</table><table>" << endl;
    
    vector<Room*> rooms = st.rooms;
    sort_rooms(rooms);
    
    cout << "<tr><th>Room ID</th><th>Occupants</th></tr>" << endl;
    for(vector<Room*>::const_iterator rit = rooms.begin(); rit != rooms.end(); ++rit) {
        vector<Person*> people = (*rit)->people;
        sort_people(people);
        
        vector<Person*>::iterator pit = people.begin();
        vector<Person*>::iterator pit_end = people.end();
        if(pit == pit_end) {
            continue;
        }
        cout << "<tr><td>" << (*rit)->room << "</td><td>";
        
        while(pit != pit_end) {
            cout << (*pit)->name;
            ++pit;
            
            // Print a comma if this isn't the last person
            if(pit != pit_end) {
                cout << ",";
            }
        }
        cout << "</td></tr>" << endl;
    }
    
    footer();
}

void Html::printRooms(const Person& p) {
    header();
    cout << "<tr><th>Rooms</th></tr>" << endl;
    for(vector<int>::const_iterator rit = p.rooms.begin(); rit != p.rooms.end(); ++rit) {
        cout << "<tr><td>" << (*rit) << "</td></tr>" << endl;
    }
    footer();
}

void Html::printEmployees(vector<Person*>& e) {
    sort_people(e);
    header();
    cout << "<tr><th>Employees</th></tr>" << endl;
    for(vector<Person*>::const_iterator eit = e.begin(); eit != e.end(); ++eit) {
        cout << "<tr><td>" << (*eit)->name << "</td></tr>" << endl;
    }
    footer();
}

void Html::header() {
    cout << "<html><body><table>" << endl;
}
void Html::footer() {
    cout << "</table></body></html>" << endl;
}

void Html::printEmployeesAtTime(unsigned int low1, unsigned int low2, unsigned int high1, unsigned int high2, State& st){
    vector<Person*> peeps;
    int valid = 0;
    
    if(low1 > high1 || low2 > high2){
        cout << INVALID_STR << endl;
    } else {
        //TODO implement function to print members during certain time
        for(vector<Person*>::const_iterator eit = st.employees.begin(); eit != st.employees.end(); ++eit) {
            bool k = st.inGallery(low1, high1, *eit);
            //FIXME: lexographic and comma seperated
            if(k){
                k = st.inGallery(low2, high2, *eit);
                if(!k){
                    peeps.push_back(*eit);
                    valid = 1;
                }
            }
        }
        if(valid){
            sort_people(peeps);
            header();
            cout << "<tr><th>Employees</th></tr>" << endl;
            int count = 0;
            for(vector<Person*>::iterator rit = peeps.begin(); rit != peeps.end(); ++rit) {
                cout << "<tr><td>" << (*rit)->name << "</td></tr>" << endl;
            }
            footer();
        }
    }
}

void Html::printEmployeesBound(unsigned int low, unsigned int high,State& st){
    vector<Person*> peeps;
    bool valid = false;
    if(low > high){
        cout << INVALID_STR << endl;
    } else {
        for(vector<Person*>::const_iterator eit = st.employees.begin(); eit != st.employees.end(); ++eit) {
            bool k = st.inGallery(low, high, *eit);
            //FIXME: lexographic and comma seperated
            if(k){
                // cout << (*eit)->name << endl;
                peeps.push_back(*eit);
                valid = true;
            }
        }
    }
    if(valid){
        sort_people(peeps);
        int count = 0;
        header();
        cout << "<tr><th>Employees</th></tr>" << endl;
        
        for(vector<Person*>::iterator rit = peeps.begin(); rit != peeps.end(); ++rit) {
            cout << "<tr><td>" << (*rit)->name << "</td></tr>" << endl;
        }
        footer();
    }
}

Plain::~Plain() {}

void Plain::printState(const State& st) {
    vector<Person*> employees;
    vector<Person*> guests;
    
    for(vector<Person*>::const_iterator eit = st.employees.begin(); eit != st.employees.end(); ++eit) {
        if((*eit)->in_gallery) {
            employees.push_back(*eit);
        }
    }
    for(vector<Person*>::const_iterator git = st.guests.begin(); git != st.guests.end(); ++git) {
        if((*git)->in_gallery) {
            guests.push_back(*git);
        }
    }
    
    sort_people(employees);
    sort_people(guests);
    
    vector<Person*>::const_iterator eit = employees.begin();
    while(eit != employees.end()) {
        cout << (*eit)->name;
        ++eit;
        
        if(eit != employees.end()) {
            cout << ",";
        }
    }
    cout << endl;
    
    vector<Person*>::const_iterator git = guests.begin();
    while(git != guests.end()) {
        cout << (*git)->name;
        ++git;
        
        if(git != guests.end()) {
            cout << ",";
        }
    }
    cout << endl;
    
    vector<Room*> rooms = st.rooms;
    sort_rooms(rooms);
    
    for(vector<Room*>::const_iterator rit = rooms.begin(); rit != rooms.end(); ++rit) {
        vector<Person*> people = (*rit)->people;
        sort_people(people);
        
        vector<Person*>::iterator pit = people.begin();
        vector<Person*>::iterator pit_end = people.end();
        if(pit == pit_end) {
            continue;
        }
        cout << (*rit)->room << ": ";
        
        while(pit != pit_end) {
            cout << (*pit)->name;
            ++pit;
            
            // Print a comma if this isn't the last person
            if(pit != pit_end) {
                cout << ",";
            }
        }
        cout << endl;
    }
    
}

void Plain::printRooms(const Person& p) {
    vector<int>::const_iterator rit = p.rooms.begin();
    while(rit != p.rooms.end()) {
        cout << (*rit);
        ++rit;
        
        // Print a comma if this isn't the last person
        if(rit != p.rooms.end()) {
            cout << ",";
        }
    }
    cout << endl;
}

void Plain::printEmployees(vector<Person*>& e) {
    sort_people(e);
    for(vector<Person*>::const_iterator eit = e.begin(); eit != e.end(); ++eit) {
        cout << (*eit)->name << endl;
    }
}

void Plain::printEmployeesAtTime(unsigned int low1, unsigned int low2, unsigned int high1, unsigned int high2, State& st){
    vector<Person*> peeps;
    int valid = 0;
    
    if(low1 > high1 || low2 > high2){
        cout << INVALID_STR << endl;
    } else {
        //TODO implement function to print members during certain time
        for(vector<Person*>::const_iterator eit = st.employees.begin(); eit != st.employees.end(); ++eit) {
            bool k = st.inGallery(low1, high1, *eit);
            //FIXME: lexographic and comma seperated
            if(k){
                k = st.inGallery(low2, high2, *eit);
                if(!k){
                    peeps.push_back(*eit);
                    valid = 1;
                }
            }
        }
        if(valid){
            sort_people(peeps);
            int count = 0;
            for(vector<Person*>::iterator rit = peeps.begin(); rit != peeps.end(); ++rit) {
                if(count++ != 0){
                    cout << ",";
                }
                cout << (*rit)->name;
            }
            cout << endl;
        }
    }
    
}

void Plain::printEmployeesBound(unsigned int low, unsigned int high,State& st){
    vector<Person*> peeps;
    bool valid = false;
    if(low > high){
        cout << INVALID_STR << endl;
    } else {
        for(vector<Person*>::const_iterator eit = st.employees.begin(); eit != st.employees.end(); ++eit) {
            bool k = st.inGallery(low, high, *eit);
            //FIXME: lexographic and comma seperated
            if(k){
                // cout << (*eit)->name << endl;
                peeps.push_back(*eit);
                valid = true;
            }
        }
    }
    if(valid){
        sort_people(peeps);
        int count = 0;
        for(vector<Person*>::iterator rit = peeps.begin(); rit != peeps.end(); ++rit) {
            if(count++ != 0){
                cout << ",";
            }
            cout << (*rit)->name;
        }
        cout << endl;
    }
}


void Plain::printRoomVector(vector<int> v){
    int count = 0;
    for(vector<int>::iterator rit = v.begin(); rit != v.end(); ++rit) {
        if(count++ != 0){
            cout << ",";
        }
        cout << (*rit);
    }
    cout << endl;
}

void Html::printRoomVector(vector<int> v){
    header();
    cout << "<tr><th>Rooms</th></tr>" << endl;
    
    for(vector<int>::iterator rit = v.begin(); rit != v.end(); ++rit) {
        cout << "<tr><td>" << (*rit) << "</td></tr>" << endl;
    }
    footer();
}


