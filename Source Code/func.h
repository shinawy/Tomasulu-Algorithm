#pragma once
#include <iostream>
#include <string>
#include <vector>
#include <sstream>
using namespace std; 


 
vector<string> parse(string  str, char delim)
{
    // construct a stream from the string
    std::stringstream ss(str);
    vector<string> out;
    std::string s;
    while (std::getline(ss, s, delim)) {
        out.push_back(s);
    }
    return out; 
}
 

void printinstrinfo(instrinfo a){
    cout<<"instrinfo: "<<"\t";
    cout<<"index: "<<a.index<<"\t";
    cout<<"Instr: "<<a.instr<<"\t";
     cout<<"issued: "<<a.issued<<"\t";
      cout<<"execstart: "<<a.execstart<<"\t";
       cout<<"execend: "<<a.execend<<"\t";
        cout<<"written: "<<a.written<<endl;

}


void printinstrdetails(instrdetails a){
    cout<<"instrDetails: "<<"\t";
    cout<<"index: "<<a.index<<"\t";
    cout<<"Reserindex: "<<a.reserindex<<"\t";
     cout<<"id: "<<a.id<<"\t";
      cout<<"inst: "<<a.inst<<"\t";
       cout<<"rd: "<<a.rd<<"\t";
        cout<<"rs1: "<<a.rs1<<"\t";
        cout<<"rs2: "<<a.rs2<<"\t";
        cout<<"immediate: "<<a.imm<<"\t";
        cout<<"clkcycle: "<<a.clkcycle<<endl;

}

void printreservation(reserstation res){
    cout<<"Reservation Station: "<<endl;
    cout<<"ID inst: "; 
    for (int i=0;i<res.id.size();i++)
        cout<<res.id[i]<<",";
    cout<<"\t"<<"Name: "<<res.name<<"\t"<<"busy: "<<res.busy<<endl;

}

void printreserrow(reserstation res){
//cout<<"Reservation Station: "<<endl;
    cout<<"Name: "<<res.name;
    cout<<"\tResult: "<<res.result; 
    cout<<"\tbusy: "<<res.busy;
     cout<<"\tOP: "<<res.op; 
    cout<<"\tVj: "<<res.vj;
     cout<<"\tVk: "<<res.vk; 
    cout<<"\tQj: "<<res.qj;
    cout<<"\tQk: "<<res.qk;
    cout<<"\tA: "<<res.A<<endl;
   


}

void printcycle(cycleinfo cycleinfolist){

    int cycle; 
    cycle=cycleinfolist.cycle;
    
    vector<instrinfo> instrinfovect;
    instrinfovect=cycleinfolist.instrinfolist;
    vector<reserstation> reservect;
    reservect=cycleinfolist.reserlist; 
    vector<string> Rs;
    Rs=cycleinfolist.regstatlist;

    cout<<"\t\t\t\tCycle"<<cycle<<endl;
    cout<<"InstructionStatus:\n";
    for(int i=0; i<instrinfovect.size();i++){
        printinstrinfo(instrinfovect[i]);
    }

    cout<<"\nReservationStations:\n";
    for (int i=0;i<reservect.size();i++)
        printreserrow(reservect[i]);
    
    cout<<"\nRegisterstatus:\n";
    for (int i=0;i<8;i++){
        cout<<"R"<<i<<": "<<Rs[i]<<"\t\t";
    }
    cout<<endl;


}


int stationavailable(vector<reserstation> reservect, instrdetails instrdet){
    for (int i=0;i<reservect.size();i++){
        for (int j=0;j<reservect[i].id.size();j++){
            if ((reservect[i].id)[j]==instrdet.id)
                if (reservect[i].busy==false){
                    return i;
                }
        }
    }
    return -1; 
}



string lowercase (string str){
    for (int i=0;i<str.length();i++)
        str[i]= tolower(str[i]);
    
    return str; 

}

void validateinstr(instrdetails instr){
    if (instr.rd>7||instr.rd<-1||instr.rs1>7||instr.rs1<-1||instr.rs2>7||instr.rs2<-1){
        cout<<"REGISTER OUT OF RANGE PLEASE SPECIFY ONE WITHIN R0-R7\n";
        exit(1);
    }

    if (instr.imm>63 || instr.imm<-64){
        cout<<"IMM OUT OF RANGE PLEASE SPECIFY IMM WITHIN -64:63\n";
        exit(1);
    }

    bool flag=false; 
    string instrarr [9]={"lw","sw","beq","jalr", "ret", "add","neg","addi", "div"};
    for (int i=0; i<9;i++)
        if (instr.inst==instrarr[i])
            flag=true;
    
    if (flag==false){
        cout<<"Instruction \""<<instr.inst<<"\" Not defined"<<endl;
        exit(1);
    }
        

}

string ignorewhite(string str){
string newstr="";

for (int i=0;i<str.length();i++){
    if (str[i]!=' ')        
        newstr=newstr+str[i];

}
return newstr;
}

instrdetails create_instr(string str){
    vector<string> initstr; 
    initstr=parse(str,' ');
    // initstr.push_back("nhls");
    // initstr.push_back("fasjkdkl"); 
    instrdetails instrdet;
    instrdet.inst=lowercase(initstr[0]);
    string restinst="";
    for (int i=1; i<initstr.size();i++)
        restinst=restinst+ initstr[i];

    //cout<<"this is restinst: "<<restinst<<endl; 
    //restinst=ignorewhite(restinst);

    // cout<<"this is the instruction details: "<<endl;

    // cout<<"instruction: "<<instrdet.inst<<endl; 
    // cout<<"Restinst: "<<restinst<<endl;

    vector<string> instroperand; 
    vector<string> instroperand2; 
    vector<string> instroperand3; 


    //this is to handle the lw and sw instructions
    if (instrdet.inst=="lw" || instrdet.inst=="sw"){
        
        
        instroperand=parse(restinst,',');
        //cout<<"instroperand size: "<<instroperand.size()<<endl;
        if (instroperand.size()>2){
            cout<<"TOO MANY OPERANDS FOR THE INSTRUCTION \""<<str<<"\"\n";
            exit(1);
        }

        if (instroperand[0].length()>2){
            cout<<"WRONG Value FOR THE DESTINATION REGISTER Place IN THE INSTRUCTION \""<<str<<"\"\n";
            exit(1);
        } 
        if (instroperand[0][0]!='R' && instroperand[0][0] != 'r'){
            cout<<"WRONG NOTATION FOR THE DESTINATION REGISTER Place IN THE INSTRUCTION \""<<str<<"\"\n";
            exit(1);

        }
        else{
            if (instrdet.inst=="lw"){
                instrdet.clkcycle=2; 
                instrdet.rd= int(instroperand[0][1])-48;
                instrdet.id=1;
            }
            else if(instrdet.inst=="sw"){
                instrdet.clkcycle=1; 
                instrdet.rs2=int(instroperand[0][1])-48;
                instrdet.id=2;
            }
        }


        instroperand2=parse(instroperand[1],'(');
        if (instroperand2.size()==0){
                cout<<"Missing parentheses in the instruction \""<<str<<"\" \n";
                exit(1);
            }
        
        instrdet.imm=stoi(instroperand2[0]);
        //instrdet.imm=0;
        
        if (instroperand2[1][0]!='R' && instroperand2[1][0] != 'r'){
            cout<<"WRONG NOTATION FOR THE SOURCE REGISTER IN THE INSTRUCTION \""<<str<<"\"\n";
            exit(1);

        }
        else{
            instroperand3=parse(instroperand2[1],')');
            if (instroperand3.size()==0){
                cout<<"Missing parentheses in the instruction \""<<str<<"\" \n";
                exit(1);
            }
            if (instroperand3[0].length()>2){
                cout<<"WRONG Source REGISTER IN THE INSTRUCTION \""<<str<<"\"\n";
                exit(1);
            }
                
            instrdet.rs1=int(instroperand3[0][1])-48; //to compensate for the ASCII 
        }
        
        validateinstr(instrdet);
        return instrdet;

    }


//this is to handle the beq and addi instructions
    else if (instrdet.inst=="beq" || instrdet.inst=="addi"){
        
        
        instroperand=parse(restinst,',');
        //cout<<"instroperand size: "<<instroperand[2]<<endl;
        if (instroperand.size()>3){
            cout<<"TOO MANY OPERANDS FOR THE INSTRUCTION \""<<str<<"\"\n";
            exit(1);
        }
        if (instroperand.size()<3){
            cout<<"TOO FEW OPERANDS FOR THE INSTRUCTION \""<<str<<"\"\n";
            exit(1);
        }

        if (instroperand[0].length()>2){
            cout<<"WRONG Value FOR THE DESTINATION REGISTER Place IN THE INSTRUCTION \""<<str<<"\"\n";
            exit(1);
        } 
        if (instroperand[0][0]!='R' && instroperand[0][0] != 'r'){
            cout<<"WRONG NOTATION FOR THE DESTINATION REGISTER Place IN THE INSTRUCTION \""<<str<<"\"\n";
            exit(1);

        }

        if (instroperand[1].length()>2){
            cout<<"WRONG Value FOR THE Source REGISTER Place IN THE INSTRUCTION \""<<str<<"\"\n";
            exit(1);
        } 
        if (instroperand[1][0]!='R' && instroperand[1][0] != 'r'){
            cout<<"WRONG NOTATION FOR THE SOURCE REGISTER Place IN THE INSTRUCTION \""<<str<<"\"\n";
            exit(1);

        }

        if (instrdet.inst=="beq"){
            instrdet.rs1=int(instroperand[0][1])-48;
            instrdet.rs2=int(instroperand[1][1])-48;
            instrdet.id=3;
            instrdet.clkcycle=1; 
        }
        else if (instrdet.inst=="addi"){
            instrdet.rd=int(instroperand[0][1])-48;
            instrdet.rs1=int(instroperand[1][1])-48;
            instrdet.id=8;
            instrdet.clkcycle=2; 
        }
       
        
        instrdet.imm=stoi(instroperand[2]);
        //cout<<instrdet.imm<<endl;
        validateinstr(instrdet);
        return instrdet;
    }

 //this is to handle the add and div instructions
    else if (instrdet.inst=="add" || instrdet.inst=="div"){
        
        if (instrdet.inst=="add"){
            instrdet.id=6;
            instrdet.clkcycle=2; 
        }
        else if (instrdet.inst=="div"){
            instrdet.id=9;
            instrdet.clkcycle=8; 
        }
        
        instroperand=parse(restinst,',');
        //cout<<"instroperand size: "<<instroperand.size()<<endl;
        if (instroperand.size()>3){
            cout<<"TOO MANY OPERANDS FOR THE INSTRUCTION \""<<str<<"\"\n";
            exit(1);
        }
        if (instroperand.size()<3){
            cout<<"TOO FEW OPERANDS FOR THE INSTRUCTION \""<<str<<"\"\n";
            exit(1);
        }

        if (instroperand[0].length()>2){
            cout<<"WRONG Value FOR THE DESTINATION REGISTER Place IN THE INSTRUCTION \""<<str<<"\"\n";
            exit(1);
        } 
        if (instroperand[0][0]!='R' && instroperand[0][0] != 'r'){
            cout<<"WRONG NOTATION FOR THE DESTINATION REGISTER Place IN THE INSTRUCTION \""<<str<<"\"\n";
            exit(1);

        }

        if (instroperand[1].length()>2){
            cout<<"WRONG Value FOR THE Source REGISTER Place IN THE INSTRUCTION \""<<str<<"\"\n";
            exit(1);
        } 
        if (instroperand[1][0]!='R' && instroperand[1][0] != 'r'){
            cout<<"WRONG NOTATION FOR THE SOURCE REGISTER Place IN THE INSTRUCTION \""<<str<<"\"\n";
            exit(1);

        }

        if (instroperand[2].length()>2){
            cout<<"WRONG Value FOR THE Source REGISTER Place IN THE INSTRUCTION \""<<str<<"\"\n";
            exit(1);
        } 
        if (instroperand[1][0]!='R' && instroperand[1][0] != 'r'){
            cout<<"WRONG NOTATION FOR THE SOURCE REGISTER Place IN THE INSTRUCTION \""<<str<<"\"\n";
            exit(1);

        }

        instrdet.rd=int(instroperand[0][1])-48;
        instrdet.rs1=int(instroperand[1][1])-48;
        instrdet.rs2=int(instroperand[2][1])-48;
    
        validateinstr(instrdet);
        return instrdet;
    }

//this is to handle the neg 
    else if (instrdet.inst=="neg"){
        instrdet.clkcycle=2; 
        instrdet.id=7;
        instroperand=parse(restinst,',');
        cout<<"instroperand size: "<<instroperand.size()<<endl;
        if (instroperand.size()>2){
            cout<<"TOO MANY OPERANDS FOR THE INSTRUCTION \""<<str<<"\"\n";
            exit(1);
        }
        if (instroperand.size()<2){
            cout<<"TOO FEW OPERANDS FOR THE INSTRUCTION \""<<str<<"\"\n";
            exit(1);
        }

        if (instroperand[0].length()>2){
            cout<<"WRONG Value FOR THE DESTINATION REGISTER Place IN THE INSTRUCTION \""<<str<<"\"\n";
            exit(1);
        } 
        if (instroperand[0][0]!='R' && instroperand[0][0] != 'r'){
            cout<<"WRONG NOTATION FOR THE DESTINATION REGISTER Place IN THE INSTRUCTION \""<<str<<"\"\n";
            exit(1);

        }

        if (instroperand[1].length()>2){
            cout<<"WRONG Value FOR THE Source REGISTER Place IN THE INSTRUCTION \""<<str<<"\"\n";
            exit(1);
        } 
        if (instroperand[1][0]!='R' && instroperand[1][0] != 'r'){
            cout<<"WRONG NOTATION FOR THE SOURCE REGISTER Place IN THE INSTRUCTION \""<<str<<"\"\n";
            exit(1);

        }

        

        instrdet.rd=int(instroperand[0][1])-48;
        instrdet.rs1=int(instroperand[1][1])-48;
        
    
        validateinstr(instrdet);
        return instrdet;
    }

 //this is to handle the jalr 
    else if (instrdet.inst=="jalr"){
        instrdet.clkcycle=1; 
        instrdet.id=4;
        instroperand=parse(restinst,',');
        cout<<"instroperand size: "<<instroperand.size()<<endl;
        if (instroperand.size()>1){
            cout<<"TOO MANY OPERANDS FOR THE INSTRUCTION \""<<str<<"\"\n";
            exit(1);
        }
        if (instroperand.size()<1){
            cout<<"TOO FEW OPERANDS FOR THE INSTRUCTION \""<<str<<"\"\n";
            exit(1);
        }

        if (instroperand[0].length()>2){
            cout<<"WRONG Value FOR THE DESTINATION REGISTER Place IN THE INSTRUCTION \""<<str<<"\"\n";
            exit(1);
        } 
        if (instroperand[0][0]!='R' && instroperand[0][0] != 'r'){
            cout<<"WRONG NOTATION FOR THE DESTINATION REGISTER Place IN THE INSTRUCTION \""<<str<<"\"\n";
            exit(1);

        }

        

        

        instrdet.rs1=int(instroperand[0][1])-48;
        
    
        validateinstr(instrdet);
        return instrdet;
    }

//this is to handle the ret 
    else if (instrdet.inst=="ret"){
        instrdet.clkcycle=1; 
        
        instroperand=parse(restinst,',');
        cout<<"instroperand size: "<<instroperand.size()<<endl;
        if ( restinst!=""){
            cout<<"TOO MANY OPERANDS FOR THE INSTRUCTION \""<<str<<"\"\n";
            exit(1);
        }
        instrdet.id=5;
        
        
    
        validateinstr(instrdet);
        return instrdet;
    }

//this is to handle NOT supported instruction
else{
cout<<"Instruction \""<<instrdet.inst<<"\" Not defined"<<endl;
        exit(1);
}

}

void issuenewinstr(bool& branchencountered, int& branchnum,int& branchindex,vector<int>& lswqueue,vector<instrdetails> instrvectcopied,vector<instrdetails>& instrvect,int& pc,int cycle, vector<double>& Regs,vector<string>& Rs,vector<instrinfo>& instrinfovect, vector<reserstation>& reservect){
cout<<"pc:"<<pc<<endl;

string instr= instrvect[pc].inst;


//printinstrdetails(instrvect[pc]);

int stindex=stationavailable(reservect,instrvect[pc]);

    //checking if the pc out of range;
    
    if (pc<0 ||pc>instrvect.size()-1){
        cout<<"PC OUT OF RANGE. TRYING TO ACCESS AN INSTRUCTION THAT IS NOT THERE\n";
        exit(1);
    }

    if (stindex!=-1){
        instrvect[pc]=instrvectcopied[pc];
        instrinfovect[pc].issued=-1;
        instrinfovect[pc].execstart=-1;
        instrinfovect[pc].execend=-1;
        instrinfovect[pc].written=-1;

    instrvect[pc].pc=pc;
    reservect[stindex].busy=true; 
    reservect[stindex].op=instr;
    instrvect[pc].reserindex=stindex; 
    instrinfovect[pc].issued=cycle;
    

        if (instr=="beq"){
            branchencountered=true; 
            branchindex=instrvect[pc].index;
            branchnum++;

            if(Rs[instrvect[pc].rs1]!="0"){
            reservect[stindex].qj=Rs[instrvect[pc].rs1];
            reservect[stindex].vj=0;
            }
            else{
            reservect[stindex].vj=Regs[instrvect[pc].rs1];
            reservect[stindex].qj="0";
            }

            if(Rs[instrvect[pc].rs2]!="0"){
            reservect[stindex].qk=Rs[instrvect[pc].rs2];
            reservect[stindex].vk=0;
            }
            else{
            reservect[stindex].vk=Regs[instrvect[pc].rs2];
            reservect[stindex].qk="0";
            }


        }

        else if (instr=="add"|| instr=="div"){
            cout<<"the add is working\n";

            Rs[instrvect[pc].rd]=reservect[stindex].name;
            if(Rs[instrvect[pc].rs1]!="0"){
            reservect[stindex].qj=Rs[instrvect[pc].rs1];
            reservect[stindex].vj=0;
            }
            else{
            reservect[stindex].vj=Regs[instrvect[pc].rs1];
            reservect[stindex].qj="0";
            }

            if(Rs[instrvect[pc].rs2]!="0"){
            reservect[stindex].qk=Rs[instrvect[pc].rs2];
            reservect[stindex].vk=0;
            }
            else{
            reservect[stindex].vk=Regs[instrvect[pc].rs2];
            reservect[stindex].qk="0";
            }


        }

        else if (instr=="addi"){

            Rs[instrvect[pc].rd]=reservect[stindex].name;
            if(Rs[instrvect[pc].rs1]!="0"){
            reservect[stindex].qj=Rs[instrvect[pc].rs1];
            reservect[stindex].vj=0;
            }
            else{
            reservect[stindex].vj=Regs[instrvect[pc].rs1];
            reservect[stindex].qj="0";
            }

            reservect[stindex].vk= instrvect[pc].imm;


        }

        else if (instr=="neg"){

            Rs[instrvect[pc].rd]=reservect[stindex].name;
            if(Rs[instrvect[pc].rs1]!="0"){
            reservect[stindex].qj=Rs[instrvect[pc].rs1];
            reservect[stindex].vj=0;
            }
            else{
            reservect[stindex].vj=Regs[instrvect[pc].rs1];
            reservect[stindex].qj="0";
            }
        }

        else if (instr=="jalr"){
            branchindex=instrvect[pc].index;
            branchencountered=true;
            if(Rs[instrvect[pc].rs1]!="0"){
            reservect[stindex].qj=Rs[instrvect[pc].rs1];
            reservect[stindex].vj=0;
            }
            else{
            reservect[stindex].vj=Regs[instrvect[pc].rs1];
            reservect[stindex].qj="0";
            }
        }

        else if (instr=="ret"){
            branchindex=instrvect[pc].index;
            branchencountered=true;

        }

        else if (instr=="lw"){
            lswqueue.push_back(pc);

            Rs[instrvect[pc].rd]=reservect[stindex].name;
            if(Rs[instrvect[pc].rs1]!="0"){
            reservect[stindex].qj=Rs[instrvect[pc].rs1];
            reservect[stindex].vj=0;
            }
            else{
            reservect[stindex].vj=Regs[instrvect[pc].rs1];
            reservect[stindex].qj="0";
            }

            reservect[stindex].A= instrvect[pc].imm;


        }

        else if (instr=="sw"){

            // Rs[instrvect[pc].rd]=reservect[stindex].name;
            lswqueue.push_back(pc);
            if(Rs[instrvect[pc].rs1]!="0"){
            reservect[stindex].qj=Rs[instrvect[pc].rs1];
            reservect[stindex].vj=0;
            }
            else{
            reservect[stindex].vj=Regs[instrvect[pc].rs1];
            reservect[stindex].qj="0";
            }

            if(Rs[instrvect[pc].rs2]!="0"){
            reservect[stindex].qk=Rs[instrvect[pc].rs2];
            reservect[stindex].vk=0;
            }
            else{
            reservect[stindex].vk=Regs[instrvect[pc].rs2];
            reservect[stindex].qk="0";
            }
            reservect[stindex].A= instrvect[pc].imm;
        }
    pc++;
    //cout<<"pc after adding: "<<pc<<endl;
    
    }

}





void executeinstr( int& pcjal2,int& pcjal,bool& branchencountered,int& missnum,int& branchindex,vector<instrdetails>& instrvect,instrdetails& instrdet,int&pc,int cycle,vector<instrinfo>& instrinfovect,vector<double>& regs ,vector<reserstation>& reservect, vector<int>& lswqueue, vector<double>& mem, vector<int>& writequeue){
string instr=instrdet.inst; 
int index=instrdet.index; 
int reserindex=instrdet.reserindex;
cout<<"Reservation index: "<<reserindex<<endl;
    //cout<<"instrinfovect[index].issued: "<<instrinfovect[index].issued<<"\tindex: "<<index<<endl;
    if (instrinfovect[index].issued!=-1 &&(branchencountered==false||index<=branchindex) && instrinfovect[index].issued!=cycle ){
        
              
        if(instr=="lw"){

            if (reservect[reserindex].qj=="0" && lswqueue[0]==index){
                 
                if(instrinfovect[index].execstart==-1){
                    instrinfovect[index].execstart=cycle;
                    reservect[reserindex].A=reservect[reserindex].A+reservect[reserindex].vj;
                    if (reservect[reserindex].A>4095 || reservect[reserindex].A<0){
                        cout<<"MEMORY ADDRESS OUT OF RANGE, WE HAVE ONLY FROM 0-4095 ADDRESSES\n";
                        exit(1);
                    }
                    instrdet.clkcycle--; 
                } 
                else{
                    //cout<<"it enters the loop: "<<instrdet.clkcycle<<endl;
                    if (instrdet.clkcycle==1){
                        reservect[reserindex].result=mem[reservect[reserindex].A];
                        instrdet.clkcycle--; 
                    }
                    
                    else if(instrdet.clkcycle==0){

                        if(instrinfovect[index].execend==-1){
                            instrinfovect[index].execend=cycle;
                            lswqueue.erase(lswqueue.begin());
                            writequeue.push_back(index);
                        }
                        
                    }
                }

            }
        }

        else if(instr=="sw"){
            if (reservect[reserindex].qj=="0" && lswqueue[0]==index){
                if(instrinfovect[index].execstart==-1){
                    instrinfovect[index].execstart=cycle;
                    reservect[reserindex].A=reservect[reserindex].A+reservect[reserindex].vj;
                    if (reservect[reserindex].A>4095 || reservect[reserindex].A<0){
                        cout<<"MEMORY ADDRESS OUT OF RANGE, WE HAVE ONLY FROM 0-4095 ADDRESSES\n";
                        exit(1);
                    }
                    instrdet.clkcycle--; 
                } 
                else if(instrdet.clkcycle==0){
                    if(instrinfovect[index].execend==-1){
                            instrinfovect[index].execend=cycle;
                            lswqueue.erase(lswqueue.begin());
                            writequeue.push_back(index);
                    }
                }

            }
        }

        else if(instr=="beq"){
            
            if (reservect[reserindex].qj=="0" && reservect[reserindex].qk=="0"){
                cout<<"happylife:\n";
                
                if(instrinfovect[index].execstart==-1){
                    instrinfovect[index].execstart=cycle;    
                    instrdet.clkcycle--; 
                }

                else if(instrinfovect[index].execend==-1){
                                

                        if(instrdet.clkcycle==0){
                        instrinfovect[index].execend=cycle;
                        
                        if (reservect[reserindex].vj==reservect[reserindex].vk){
                            pc=instrdet.pc+instrdet.imm;
                            
                            missnum++;
                            for(int k=index+1;k<instrinfovect.size();k++){
                                if(instrinfovect[k].issued==-1)
                                    break;
                                else{
                                    instrinfovect[k].issued=-1;
                                    reservect[instrvect[k].reserindex].busy=false;
                                    reservect[instrvect[k].reserindex].op="";
                                    reservect[instrvect[k].reserindex].qj="";
                                    reservect[instrvect[k].reserindex].qk="";
                                    reservect[instrvect[k].reserindex].vj=-1;
                                    reservect[instrvect[k].reserindex].vk=-1;
                                    reservect[instrvect[k].reserindex].result=-1;
                                }

                            }                      
                        }
                        branchencountered=false;
                        writequeue.push_back(index);

                        
                    } 
                                

                                
                 }   

            }
        }

         else if(instr=="jalr"){
            if (reservect[reserindex].qj=="0"){
                if(instrinfovect[index].execstart==-1){
                    instrinfovect[index].execstart=cycle;
                    
                     
                        
                    instrdet.clkcycle--; 
                } 
                else if (instrinfovect[index].execend==-1){
                     if(instrdet.clkcycle==0){
                    pc=reservect[reserindex].vj;
                    pcjal=pc;
                        regs[1]=instrdet.pc+1;
                       
                        
                       //reservect[reserindex].busy=false;
                        for(int k=index+1;k<instrinfovect.size();k++){
                            if(instrinfovect[k].issued==-1)
                                break;
                            else{
                                 instrinfovect[k].issued=-1;
                                reservect[instrvect[k].reserindex].busy=false;
                                reservect[instrvect[k].reserindex].op="";
                                reservect[instrvect[k].reserindex].qj="";
                                reservect[instrvect[k].reserindex].qk="";
                                reservect[instrvect[k].reserindex].vj=-1;
                                reservect[instrvect[k].reserindex].vk=-1;
                                reservect[instrvect[k].reserindex].result=-1;
                            }
                        }                      
                    
                    branchencountered=false;

                    if(instrinfovect[index].execend==-1){
                            instrinfovect[index].execend=cycle;
                            writequeue.push_back(index);
                            
                    }
                }
            }

            }
        }

        else if(instr=="ret"){
            
                if(instrinfovect[index].execstart==-1){
                    instrinfovect[index].execstart=cycle;
                    
                    
                        
                    instrdet.clkcycle--; 
                } 
            if(instrinfovect[index].execend==-1){
                 if(instrdet.clkcycle==0){
                    pc=regs[1];
                    pcjal2=pcjal;
                       
                        
                       
                        for(int k=index+1;k<instrinfovect.size();k++){
                            if(instrinfovect[k].issued==-1)
                                break;
                            else{
                                instrinfovect[k].issued=-1;
                                reservect[instrvect[k].reserindex].busy=false;
                                reservect[instrvect[k].reserindex].op="";
                                reservect[instrvect[k].reserindex].qj="";
                                reservect[instrvect[k].reserindex].qk="";
                                reservect[instrvect[k].reserindex].vj=-1;
                                reservect[instrvect[k].reserindex].vk=-1;
                                reservect[instrvect[k].reserindex].result=-1;
                                }

                        }                      
                    
                    branchencountered=false;
                    if(instrinfovect[index].execend==-1){
                            instrinfovect[index].execend=cycle;
                            writequeue.push_back(index);
                            
                    }
                } 
            }           
        }

        else if(instr=="neg"){
            
            cout<<"reserindex: "<<reserindex<<endl;
            if (reservect[reserindex].qj=="0"){
                cout<<"Entered the neg exec\n";
                if(instrinfovect[index].execstart==-1){
                    
                    instrinfovect[index].execstart=cycle;                
                    instrdet.clkcycle--; 
                } 
                
                else if(instrdet.clkcycle==0){
                    
                    if(instrinfovect[index].execend==-1){
                            instrinfovect[index].execend=cycle;
                            reservect[reserindex].result= 0-reservect[reserindex].vj;
                            writequeue.push_back(index);
                            
                    }
                }
                else{ instrdet.clkcycle--; }

            }
            
           
        }

        else if(instr=="add"||instr=="addi"){
            
            if (reservect[reserindex].qj=="0"&&reservect[reserindex].qk=="0"){
                if(instrinfovect[index].execstart==-1){
                    
                    instrinfovect[index].execstart=cycle;                
                    instrdet.clkcycle--; 
                    
                } 
                
                else if(instrdet.clkcycle==0){
                    if(instrinfovect[index].execend==-1){
                            instrinfovect[index].execend=cycle;
                            reservect[reserindex].result= reservect[reserindex].vj+reservect[reserindex].vk;
                            writequeue.push_back(index);
                            
                    }
                }
                else{instrdet.clkcycle--; }

            }
        }

        else if(instr=="div"){
            if (reservect[reserindex].qj=="0"&&reservect[reserindex].qk=="0"){
                if(instrinfovect[index].execstart==-1){
                    instrinfovect[index].execstart=cycle;                
                    instrdet.clkcycle--; 
                } 
                
                else if(instrdet.clkcycle==0){
                    if(instrinfovect[index].execend==-1){
                            instrinfovect[index].execend=cycle;
                            reservect[reserindex].result= reservect[reserindex].vj / reservect[reserindex].vk;
                            writequeue.push_back(index);
                            
                    }
                }
                else{instrdet.clkcycle--; }

            }
        }



    }


}



void write_back(int& numofdeneinstructions,vector<int>& writequeue, vector<instrdetails>& instrvect, vector<reserstation>& reservect,int&pc,int cycle,vector<instrinfo>& instrinfovect,vector<double>& regs,vector<string>& Rs, vector<double>& mem){

    int index=writequeue[0];
    string instr=instrvect[index].inst;
    int reserindex=instrvect[index].reserindex;
    if (instrinfovect[index].execend!=-1 && instrinfovect[index].execend!=cycle ){

        if (instr=="sw"){
            if (reservect[reserindex].qk=="0"){
                int address= reservect[reserindex].A;
                mem[address]= reservect[reserindex].vk;
                instrinfovect[index].written=cycle;
                numofdeneinstructions++;
                writequeue.erase(writequeue.begin());
                reservect[reserindex].busy=false;
                reservect[reserindex].op="";
                reservect[reserindex].qj="";
                reservect[reserindex].qk="";
                reservect[reserindex].vj=-1;
                reservect[reserindex].vk=-1;
                reservect[reserindex].A=0;
                reservect[reserindex].result=-1;

            }
        }

        else if (instr=="beq"||instr=="jalr"||instr=="ret"){
                instrinfovect[index].written=cycle;
                numofdeneinstructions++;
                writequeue.erase(writequeue.begin());
                reservect[reserindex].busy=false;
                reservect[reserindex].op="";
                reservect[reserindex].qj="";
                reservect[reserindex].qk="";
                reservect[reserindex].vj=-1;
                reservect[reserindex].vk=-1;
                reservect[reserindex].A=0;
                reservect[reserindex].result=-1;
        }

        else {
            instrinfovect[index].written=cycle;
            numofdeneinstructions++;
            writequeue.erase(writequeue.begin());
            Rs[instrvect[index].rd]="0";
            if (instrvect[index].rd!=0)
                regs [instrvect[index].rd]=reservect[reserindex].result;
            for (int i=0; i<instrvect.size();i++){
                if (instrinfovect[i].issued!=-1){
                    int intreserindex=instrvect[i].reserindex;
                   
                         
                        if (reservect[intreserindex].qj==reservect[reserindex].name){                        
                        reservect[intreserindex].qj="0";
                        reservect[intreserindex].vj=reservect[reserindex].result;                            
                        }           
                                   
                        
                        if (reservect[intreserindex].qk==reservect[reserindex].name){                        
                        reservect[intreserindex].qk="0";
                        reservect[intreserindex].vk=reservect[reserindex].result;                            
                        }                   

                }

            }
            reservect[reserindex].busy=false;
            reservect[reserindex].op="";
            reservect[reserindex].qj="";
            reservect[reserindex].qk="";
            reservect[reserindex].vj=-1;
            reservect[reserindex].vk=-1;
            reservect[reserindex].A=0;
            reservect[reserindex].result=-1;


        }
        










    }




}