
#include "struct.h"
#include "func.h"
#include <fstream>
const int Zero=0;





int main(){

    int index=0; 
    ifstream infile; 
    infile.open("input.txt");
    vector<instrdetails> instrvect;
    vector<instrdetails> instrvectcopied; 
    vector<int> lswqueue;// this is keeping track of the index of the instruction only (it is uinque for each instruction)
    vector<instrinfo> instrinfovect; 
    vector <int> writequeue;
    vector<double> mem (4096,0);
    mem[50]=9;
    
    vector<double> regs(8,0);
    vector <string> regstat(8,"0");
    regs[0]=Zero;
    
    for (int i=1;i<8;i++){
        regs[i]=i;
    }
   

    while(!infile.eof()){
        
        
        string temp="";

        getline(infile, temp);
        if (temp.length()<2)
            break;

        cout<<temp<<endl;
        instrdetails tempinstdet; 
        tempinstdet=create_instr(temp);
       
        tempinstdet.index=index;
        instrvect.push_back(tempinstdet);
        // if (tempinstdet.inst=="lw" || tempinstdet.inst=="sw")
        //     lswqueue.push_back(index);
        
        instrinfo temp1;
        temp1.index=index; 
        temp1.instr=temp;
        instrinfovect.push_back(temp1);
        index++;
    
    }

    instrvectcopied=instrvect;
   
    

    
    
    

    reserstation load1({1},"load1",false,-1,-1,"","",0,-1);
    reserstation load2({1},"load2",false,-1,-1,"","",0,-1);
    reserstation store1({2},"store1",false,-1,-1,"","",0,-1);
    reserstation store2({2},"store2",false,-1,-1,"","",0,-1);
    reserstation beq({3},"BEQ",false,-1,-1,"","",0,-1);
    reserstation Jalr({4,5},"Jalr",false,-1,-1,"","",0,-1);
    reserstation arith1({6,7,8},"arith1",false,-1,-1,"","",0,-1);
    reserstation arith2({6,7,8},"arith2",false,-1,-1,"","",0,-1);
    reserstation Div({9},"Div",false,-1,-1,"","",0,-1);
    vector<reserstation> reservect; 
    reservect.push_back(load1);
    reservect.push_back(load2);
    reservect.push_back(store1);
    reservect.push_back(store2);
    reservect.push_back(beq);
    reservect.push_back(Jalr);
    reservect.push_back(arith1);
    reservect.push_back(arith2);
    reservect.push_back(Div);

    
    cycleinfo mycycle;
    
    int pc=0;int cycle=1; bool branchencountered=false; int branchnum=0; int missnum=0;int branchindex=0; int pcjal=instrvect.size();
    int pcjal2=instrvect.size();
     int pcbranch=0; bool lastcycle=true;int numofdoneinstr(0);
     bool firstinst=true;
    while (cycle<13){
        lastcycle=true;
        cout<<"cycle: "<<cycle<<"pc:"<<pc<<endl;
         if (pc<pcjal2){
             
            issuenewinstr(branchencountered,branchnum,branchindex,lswqueue,instrvectcopied,instrvect,pc,cycle,regs,regstat,instrinfovect,reservect);
        
        }
        // else {
        //     cout<<"pc is out of range\n";
        //     exit(1);
        // }
        // cout<<"before entering the loop:\n";
        // for (int i=0;i<instrinfovect.size(); i++)
        //     printinstrinfo(instrinfovect[i]);
        
        for(int i=0;i<instrvect.size();i++){
            // cout<<"Executing.................\n";
            // cout<<"cycle: "<<cycle<<"pc:"<<pc<<endl;
            // cout<<"instr: "<<instrvect[i].inst<<endl;
            executeinstr(pcjal2,pcjal,branchencountered,missnum,branchindex,instrvect,instrvect[i],pc,cycle,instrinfovect,regs,reservect,lswqueue,mem, writequeue);
        }
        cout<<"WRITE QUEUE: "<<endl;
        for (int i=0;i<writequeue.size();i++){
                cout<<writequeue[i]<<endl;

        }
        if (writequeue.size()>0)
            write_back(numofdoneinstr,writequeue,instrvect,  reservect,pc, cycle, instrinfovect, regs, regstat,  mem);
        
        


        //pc=pcbranch;
        mycycle.instrinfolist=instrinfovect;
        mycycle.reserlist=reservect;
        mycycle.regstatlist=regstat;
        mycycle.cycle=cycle;
        printcycle(mycycle);
        for (int i=0;i<instrinfovect.size();i++){
            if  (instrinfovect[i].issued==-1 || instrinfovect[i].execstart==-1 || instrinfovect[i].execend==-1 || instrinfovect[i].written==-1)
                lastcycle=false;
        }
        if (lastcycle)
            break;
        cycle++;
        
        
        

    }

    cout<<"beginning"<<endl;
    for (int i=0;i<regs.size();i++){
        cout<<"Reg"<<i<<": "<<regs[i]<<"\t";
    }
    cout<<endl;
    cout<<"IPC: "<<double (numofdoneinstr)/cycle<<endl;
    cout<<"branchencounter: "<<branchnum<< "\tmiss: "<<missnum<<endl;
    if (branchnum!=0)
        cout<<"miss ratio: "<< double(missnum)/branchnum * 100<<endl;
    cout<<"lswqueue: "<<lswqueue.size()<<endl; 
    for (int i=0;i<lswqueue.size();i++)
        cout<<lswqueue[i]<<endl;;
cout<<"end\n";
// printinstrdetails (instrvect[2]);


     //cout<<stationavailable(reservect,instrvect[1])<<endl;
    //printcycle(mycycle);

    // for(int i=0; i<instrinfovect.size();i++){
    //     printinstrinfo(instrinfovect[i]);
    // }

    // for (int i=0;i<reservect.size();i++)
    //     printreserrow(reservect[i]);
   
        // for (int i=0;i<instrvect.size();i++)
        // printinstrdetails(instrvect[i]);

        // for (int i=0;i<lswqueue.size();i++)
        // printinstrdetails(lswqueue[i]);
        // for (int i=0;i<instrinfovect.size();i++)
        //     printinstrinfo(instrinfovect[i]);

    // string str="mine x3,x2,x4  "; 
    // instrdetails mine;
    // mine=create_instr(str);
    // printinstrdetails(mine);










return 0; 
}