#ifndef formatter_h
#define formatter_h

#include <string>
#include <iostream>
#include "state.h"
#include "util.h"
using namespace std;

class Formatter {
public:
    virtual ~Formatter();
    virtual void printState(const State& st) = 0;
    virtual void printRooms(const Person& p) = 0;
    virtual void printEmployees(vector<Person*>& e) = 0;
    virtual void printEmployeesAtTime(unsigned int low1, unsigned int low2, unsigned int high1, unsigned int high2, State& st) = 0;
    virtual void printEmployeesBound(unsigned int low, unsigned int high,State& st) = 0;
    virtual void printRoomVector(vector<int> v) = 0;

    
};

class Html : public Formatter {
public:
    ~Html();
    void header();
    void footer();
    void printState(const State& st);
    void printRooms(const Person& p);
    void printEmployees(vector<Person*>& e);
    void printEmployeesAtTime(unsigned int low1, unsigned int low2, unsigned high1, unsigned int high2, State& st);
    void printEmployeesBound(unsigned int low, unsigned int high,State& st);
    void printRoomVector(vector<int> v);

    
    
};

class Plain : public Formatter {
public:
    ~Plain();
    void printState(const State& st);
    void printRooms(const Person& p);
    void printEmployees(vector<Person*>& e);
    void printEmployeesAtTime(unsigned int low1, unsigned int low2, unsigned int high1, unsigned int high2, State& st);
    void printEmployeesBound(unsigned int low, unsigned int high,State& st);
    void printRoomVector(vector<int> v);

};


#endif
