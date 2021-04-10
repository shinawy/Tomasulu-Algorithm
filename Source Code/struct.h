#pragma once
#include <iostream>
#include <string>
#include <vector>
using namespace std;

struct instrinfo{
    int index; 
    string instr;
    int issued; 
    int execstart;
    int execend; 
    int written; 
    

    instrinfo(){
        index=-1; 
        issued=-1; 
        execstart=-1; 
        execend=-1; 
        written=-1;
        instr="";
       
    }
    
};

struct instrdetails{
    int index; 
    int reserindex; //reservation station index
    int id;
    string inst;
    int rd;
    int rs1; 
    int rs2; 
    int imm;
    int clkcycle;
    int pc;

    instrdetails(){
        index= -1;
        reserindex=-1; 
        id=-1; 
        inst=""; 
        rd=-1;
        rs1=-1;
        rs2=-1;
        imm=0; 
        clkcycle=1;
         pc=0;
    } 

    instrdetails(int pindex,int preserindex,int pid, string pinst, int prd, int prs1, int prs2,int pimm,int pclkcycle){
        index= pindex;
        reserindex=preserindex; 
        id=pid; 
        inst=pinst; 
        rd=prd;
        rs1=prs1;
        rs2=prs2;
        imm=pimm; 
        clkcycle=pclkcycle;

    } 


};




struct reserstation{
    vector<int> id; 
    string name;
    bool busy; 
    string op; 
    int vj; 
    int vk;
    string qj;
    string qk;
    int A;
    double result;
    reserstation(){
        
        name="N";
        busy=false;
        op="";
        result=0; 
    }
    reserstation(vector<int> pid,string pname,bool pbusy, int pvj, int pvk,string pqj,string pqk, int pA,double presult){
        id=pid;
        name=pname;
        busy=pbusy;
        vj=pvj;
        vk=pvk; 
        qj=pqj;
        qk=pqk;
        A=pA;
        result=presult;
    }
    
};

struct cycleinfo{
    int cycle;
vector<instrinfo> instrinfolist;
vector<reserstation> reserlist; 
vector <string> regstatlist;


};




struct reserstationrow{
     
    string name;
    bool busy; 
    string op; 
    int vj; 
    int vk;
    string qj;
    string qk;
    int A;
};
