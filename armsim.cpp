// If any build/compile errors occur make sure your ide supports c++v11 or use this command -std=c++11 
// This program reads a file called code.txt for arm assembly language instructions and loads them into an appropiate data structure for easy handling
// write instructions in this format for best results
//		add R1,R2,R3
//		sub R4,R1,R5
//		ldr r5,[r3,#32]
//		beq -96
// generally operation space 1stop comma 2ndop comma 3rdop enter

#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <stdlib.h>
using namespace std;
class program;
class instruction;
class strqueue {// used as a buffer to save info along the pipeline
	private:
	string *quearray; 
	int queuesize; 
	int front;
	int rear;
	int numitems;
	public: 
	strqueue(int s=5){
		quearray=new string[s];
		queuesize=s;
		front=-1;
		rear=-1;
		numitems=0;
	}
	void show(){
		int i=(front+1)%queuesize,j=(rear+1)%queuesize;
		while(i!=j){
			cout<<quearray[i]<<" ";
			i=(i+1)%queuesize;
		}
	}
	void enqueue(string x){// expands by five elements at a time
		string *ptr;
		if(isfull()){
			ptr=new string[queuesize+5];
			for(int i=0;i<queuesize;i++){
				*(ptr+i)=quearray[i];
			}
			delete [] quearray;
			quearray=ptr;
			queuesize=queuesize+5;
			rear=(rear+1)%queuesize;
			quearray[rear]=x;
			numitems++;			
		}
		else{
			rear=(rear+1)%queuesize;
			quearray[rear]=x;
			numitems++;
		}
	}
	string getfn(int x){ // gives a copy of element n items behind the front
		string num;
		if(isempty())
			cout<<"the queue is empty ";
		else{
			x=(front+x+1)%queuesize;
			num=quearray[x];
		}
		return num;
		
	}
	void editfn(string x,int p){ //edit index of queue
		int sp;
		if(isempty())
			cout<<"out of range ";
		else{
			sp=(front+p+1)%queuesize;
			quearray[sp]=x;
		}
	}
	string dequeue(){
		string num="";
		if(isempty())
			cout<<"the queue is empty ";
		else{
			front=(front+1)%queuesize;
			num=quearray[front];
			numitems--;
		}
		return num;
	}
	void editfront(string x){
		int i=(front+1)%queuesize;
		quearray[i]=x;
	}
	void editback(string x){
		int i=(rear+1)%queuesize;
		quearray[i]=x;
	}
	string getfront(){
		int i=(front+1)%queuesize;
		return quearray[i];
	}
	string getback(){
		int i=(rear+1)%queuesize;
		return quearray[i];
	}
	bool isempty(){
		if(numitems)
			return false;
		return true;
	}
	bool isfull(){
		if(numitems<queuesize)
			return false;
		return true;
	}
	int getpipesize(){
		return numitems;
	}
	void clear(){
		front=-1;
		rear=-1;
		numitems=0;
	}
	~strqueue(){
		delete [] quearray;
	}
};
string toBinary(int n) {//convert decimal to binary
    string r;
    while(n!=0) {r=(n%2==0 ?"0":"1")+r; n/=2;}
    return r;
}
string signextend(unsigned int maxlen, string input, bool signb){ // sign extension 
		while(input.length()<maxlen){ 
			if(signb)
				input="0"+input;
			else
				input="1"+input;
		}
	return input;
}
string twos_complement(string number){ // used for negative representation
	for(unsigned int i=0;i<number.size();i++){
        if (number[i]=='1')
            number[i]='0';
        else
			number[i]='1';
		} // ones complment complete now to add 1
	for(unsigned int i=number.size()-1;i>=0;i--){
		if(number[i]=='0'){
			number[i]='1';
			break;
		}
		else
			number[i]='0';
		 }	   
    return number;
}
char getHexCharacter(string str){ // converts a binary string to its hexadecimal value
	if(str.compare("1111") == 0) return 'F';
	else if(str.compare("1110") == 0) return 'E';
	else if(str.compare("1101")== 0) return 'D';
	else if(str.compare("1100")== 0) return 'C';
	else if(str.compare("1011")== 0) return 'B';
	else if(str.compare("1010")== 0) return 'A';
	else if(str.compare("1001")== 0) return '9';
	else if(str.compare("1000")== 0) return '8';
	else if(str.compare("0111")== 0) return '7';
	else if(str.compare("0110")== 0) return '6';
	else if(str.compare("0101")== 0) return '5';
	else if(str.compare("0100")== 0) return '4';
	else if(str.compare("0011")== 0) return '3';
	else if(str.compare("0010")== 0) return '2';
	else if(str.compare("0001")== 0) return '1';
	else if(str.compare("0000")== 0) return '0';
	else if(str.compare("111")== 0) return '7';
	else if(str.compare("110")== 0) return '6';
	else if(str.compare("101")== 0) return '5';
	else if(str.compare("100")== 0) return '4';
	else if(str.compare("011")== 0) return '3';
	else if(str.compare("010")== 0) return '2';
	else if(str.compare("001")== 0) return '1';
	else if(str.compare("000")== 0) return '0';
	else if(str.compare("11")== 0) return '3';
	else if(str.compare("10")== 0) return '2';
	else if(str.compare("01")== 0) return '1';
	else if(str.compare("00")== 0) return '0';
	else if(str.compare("1")== 0) return '1';
	else if(str.compare("0")== 0) return '0';
	else return 'X';// denotes that a non binary string called this function
}
string gethexrow(string rowresult){// converts a binay string to hexadecimal string
	string endresult = "";
	for(unsigned int i = 0; i < rowresult.length(); i = i+4)	{
		endresult += getHexCharacter(rowresult.substr(i,4));
	}
	return endresult;
}
string inttobin(int input){ // converts an int to binary returns a 32bit bin string 
		string temp=toBinary(input);
		if(input>=0){// positve
			temp=signextend(32,temp,true);
		}
		else{// negative
			temp=twos_complement(temp);
			temp=signextend(32,temp,false);
		}
	return temp;
}
string inttohex(int input){ // convert a decimal to hex string 32bits
	string temp=inttobin(input);
	temp=gethexrow(temp);
	return temp;	
}
string byteatoff(int x,string input){// only for hex strings
	string temp="";
	if((x>=0)&&(x<4)){
		switch(x){
			case 0:
			temp=input[0];
			temp=temp+input[1];
			break;
			case 1:
			temp=input[2];
			temp=temp+input[3];
			break;
			case 2:
			temp=input[4];
			temp=temp+input[5];
			break;
			case 3:
			temp=input[6];
			temp=temp+input[7];
			break;
		}
	}
	return temp;
}
// some global variables
int regfile[15]; // 15 registers
int flagz=0; // zero flag for cmp
int mem[4194305]; // 16mb /4 =4194304 single storage areas  
class instruction // holds a single instruction
{
friend class program;
protected:
	string opcode;
	string op1;
	string op2;
	string op3;
	string op4;
	string binary;
	int numterms; // number of operands including opcode
public:
	instruction(){	
		opcode="novalue";
		op1="novalue";
		op2="novalue";
		op3="novalue";
		op4="novalue";
		binary="novalue";
		numterms=0;
		}
void calstage(int stage){// calculates information regarding the stage of an instruction
	string temp1,temp2,temp3;
	int temp4,temp5;
	if(stage==1){//instruction fetch
		cout<<endl;
		if((opcode!="nop")||(binary!="unsupported instruction")){
			cout<<"instruction fetch stage\n";
				if(numterms>=1)
					cout<<opcode<<" ";
				if(numterms>=2)
					cout<<op1<<" ";
				if(numterms>=3)
					cout<<op2<<" ";
				if(numterms>=4)
					cout<<op3<<" ";
				if(numterms>=5)
					cout<<op4<<" ";			
		cout<<"\nBinary encoding "<<binary<<endl;		
		cout<<"PC value "<<regfile[15]<<endl;
		}
		else{
			cout<<"instruction fetch stage empty\n";
			cout<<"PC value "<<regfile[15]<<endl;
		}
		// call animations for pc+4 and instruction fetch line (same for all cases
	}
	else if(stage==2){
		cout<<endl;
		if((opcode!="nop")||(binary!="unsupported instruction")){
		cout<<"instruction decode stage \n";
				if(numterms>=1)
					cout<<opcode<<" ";
				if(numterms>=2)
					cout<<op1<<" ";
				if(numterms>=3)
					cout<<op2<<" ";
				if(numterms>=4)
					cout<<op3<<" ";
				if(numterms>=5)
					cout<<op4<<" ";			
		cout<<"\nBinary encoding "<<binary<<endl;		
		cout<<"PC value "<<regfile[15]-1<<endl;
		if((opcode=="MOV")||(opcode=="mov")){
			
		// call related draw functions here 
		
		}
		else if((opcode=="CMP")||(opcode=="cmp")){
	
		// call related draw functions here 
		}
		else if((opcode=="AND")||(opcode=="and")){
	
		// call related draw functions here 
		}
		else if((opcode=="ORR")||(opcode=="orr")){
			
		// call related draw functions here 
		}
		else if((opcode=="ADD")||(opcode=="add")){
			
		// call related draw functions here 
		}
		else if((opcode=="SUB")||(opcode=="SUB")){
			
		// call related draw functions here 
		}
		else if((opcode=="LDR")||(opcode=="ldr")){
			
		// call related draw functions here 
		}
		else if((opcode=="STR")||(opcode=="str")){
		
		// call related draw functions here 
		}					
		else if((opcode=="BEQ")||(opcode=="beq")){
				
		// call related draw functions here 
		}
		else if((opcode=="BL")||(opcode=="bl")){
			
		// call related draw functions here 
		}
	}
		else{
			cout<<"instruction decode stage empty\n";
		}
	}
	else if(stage==3){// execute
		cout<<endl;
		if((opcode!="nop")||(binary!="unsupported instruction")){
			cout<<"instruction execute stage \n";
				if(numterms>=1)
					cout<<opcode<<" ";
				if(numterms>=2)
					cout<<op1<<" ";
				if(numterms>=3)
					cout<<op2<<" ";
				if(numterms>=4)
					cout<<op3<<" ";
				if(numterms>=5)
					cout<<op4<<" ";			
		cout<<"\nBinary encoding "<<binary<<endl;		
		cout<<"PC value "<<regfile[15]-2<<endl;
		if((opcode=="AND")||(opcode=="and")){
		temp1=op1.substr(1,3); //register destination
		temp2=op2.substr(1,3); // src1
		temp3=op3.substr(1,3); //op2
		temp4=regfile[stoi(temp2)-1];
		temp5=regfile[stoi(temp3)-1];
		temp4=temp5&temp4;
		regfile[stoi(temp1)-1]=temp4;
		
		// call related draw functions here 
		
		}
		else 	if((opcode=="ORR")||(opcode=="orr")){
		temp1=op1.substr(1,3); //register destination
		temp2=op2.substr(1,3); // src1
		temp3=op3.substr(1,3); //op2
		temp4=regfile[stoi(temp2)-1];
		temp5=regfile[stoi(temp3)-1];
		temp4=temp5|temp4;
		regfile[stoi(temp1)-1]=temp4;
		
		// call related draw functions here 
		
	}
	else 	if((opcode=="ADD")||(opcode=="add")){
		temp1=op1.substr(1,3); //register destination
		temp2=op2.substr(1,3); // src1
		temp3=op3.substr(1,3); //op2
		temp4=regfile[stoi(temp2)-1];
		temp5=regfile[stoi(temp3)-1];
		temp4=temp5+temp4;
		regfile[stoi(temp1)-1]=temp4;
		
		// call related draw functions here 
		
	}
	else 	if((opcode=="SUB")||(opcode=="sub")){
		temp1=op1.substr(1,3); //register destination
		temp2=op2.substr(1,3); // src1
		temp3=op3.substr(1,3); //op2
		temp4=regfile[stoi(temp2)-1];
		temp5=regfile[stoi(temp3)-1];
		temp4=temp4-temp5;
		regfile[stoi(temp1)-1]=temp4;
		
		// call related draw functions here 
		
	}
	else 	if((opcode=="CMP")||(opcode=="cmp")){
		temp1=op1.substr(1,3); // src1
		temp2=op2.substr(1,3); // src2
		temp4=regfile[stoi(temp2)-1];
		temp5=regfile[stoi(temp1)-1];
		if(temp4==temp5)
			flagz=1;
		else
			flagz=0;
		// call related draw functions here 
		
	}	
	else 	if((opcode=="MOV")||(opcode=="mov")){
		temp1=op1.substr(1,3); // rd
		temp2=op2.substr(1,3); // src1
		temp4=regfile[stoi(temp2)-1];
		regfile[stoi(temp1)-1]=temp4;

		// draw calls here
		
		}
		else 	if((opcode=="LDR")||(opcode=="ldr")){
		temp1=op1.substr(1,3); // rd
		temp2=op2.substr(1,3); // base
		temp3=op3.substr(1,9);// offset
		temp4=regfile[stoi(temp2)-1]+stoi(temp3);// calculated address
		regfile[stoi(temp1)-1]=mem[temp4-1];// result stored
		// call related draw functions here 
		}
		else if((opcode=="STR")||(opcode=="str")){
		temp1=op1.substr(1,3); // rd
		temp2=op2.substr(1,3); // base
		temp3=op3.substr(1,9);// offset
		temp4=regfile[stoi(temp2)-1]+stoi(temp3);// calculated address
		mem[temp4-1]=regfile[stoi(temp1)-1];// result stored
		// call related draw functions here 
		}
		else if((opcode=="BL")||(opcode=="bl")){
		temp1=op1.substr(1,9); // ofsett
		temp2=op1.substr(0,1);// sign
		if(temp2=="+"){
			regfile[15]+=stoi(temp1);
		}
		else if(temp2=="-") {
			regfile[15]-=stoi(temp1);
		}
		
		// call related draw functions here 
	}
	else if((opcode=="BEQ")||(opcode=="beq")){
		temp1=op1.substr(1,9); // ofsett
		temp2=op1.substr(0,1);// sign
		if(flagz==1){
			if(temp2=="+"){
				regfile[15]+=stoi(temp1);
			}
			else if(temp2=="-") {
				regfile[15]-=stoi(temp1);
			}
		}
		// call related draw functions here 
		}
	}
	else{
			cout<<" execute stage empty\n";
		}
}
	else if(stage==4){//memory
		cout<<endl;
		if((opcode!="nop")||(binary!="unsupported instruction")){
		cout<<"memory stage \n";
				if(numterms>=1)
					cout<<opcode<<" ";
				if(numterms>=2)
					cout<<op1<<" ";
				if(numterms>=3)
					cout<<op2<<" ";
				if(numterms>=4)
					cout<<op3<<" ";
				if(numterms>=5)
					cout<<op4<<" ";			
		cout<<"\nBinary encoding "<<binary<<endl;		
		cout<<"PC value "<<regfile[15]-3<<endl;
		if((opcode=="LDR")||(opcode=="ldr")){//dt
				
				// call related draw functions here 
		}
		else if((opcode=="STR")||(opcode=="str")){//dt
				
				// draw calls for :
				//read/write adrresss
				// write data going into the memory	
		}
		else {// non dt instructions
			
	
			// draw calls for :
			// both wires that skip the memory cell and go straight into the write back stage
		}
	}
	else{
		cout<<"Memory stage empty\n";
	}
	}
	else if(stage==5){//write back
		cout<<endl;
		if((opcode!="nop")||(binary!="unsupported instruction")){
		cout<<"write back stage \n";
				if(numterms>=1)
					cout<<opcode<<" ";
				if(numterms>=2)
					cout<<op1<<" ";
				if(numterms>=3)
					cout<<op2<<" ";
				if(numterms>=4)
					cout<<op3<<" ";
				if(numterms>=5)
					cout<<op4<<" ";			
		cout<<"\nBinary encoding "<<binary<<endl;		
		cout<<"PC value "<<regfile[15]-4<<endl;
		temp1=binary.substr(0,6);
		if(temp1=="111000"){// result source =1
			 // call related draw functions here 
		}
		else {// result source = 0
			
			// call related draw functions here 
		}
	}
	else{
		cout<<"write back stage empty\n";
	}
	}
}
	void transform(string input){ // need to break up string into parts and place into appropiate string
		istringstream ss(input);
		string token, temp;
		if(getline(ss, token, ' ')){
			numterms++;
			opcode = token;
		}
		if(getline(ss, token, ',')){
			numterms++;
			op1 = token;
		}
		if(getline(ss, token, ',')){
			numterms++;
			op2 = token;
		}
		if(getline(ss, token, ',')){
			numterms++;
			op3 = token;
		}
		if(getline(ss, token, ',')){
			numterms++;
			op4 = token;
		}
		binary.clear();
		temp.clear();
		if((opcode=="ADD")||(opcode=="add")){ // instruction decoding happens to every instruction so why not sooner
			binary+="1110000";// same for chosen dp instructions in our implementation fisrt 4bits for condition 2 function and 1 immediate
			binary+="01000";// opcode for addition plus 1 bit for setting flags
			temp=op2.substr(1,3); //register source 1
			temp=toBinary(stoi(temp)); // convert to binary 
			temp=signextend(4,temp,true);// sign extend to 4 bits with zeros
			binary+=temp; // append to string
			temp=op1.substr(1,3); //register destination
			temp=toBinary(stoi(temp)); // convert to binary 
			temp=signextend(4,temp,true);// sign extend to 4 bits with zeros
			binary+=temp; // append to string
			temp=op3.substr(1,3); //register source 2
			temp=toBinary(stoi(temp)); // convert to binary 
			temp=signextend(12,temp,true);// sign extend to 12 bits with zeros (positive)
			binary+=temp;
		}
		else if((opcode=="SUB")||(opcode=="sub")){
			binary+="1110000";// same for chosen dp instructions in our implementation fisrt 4bits for condition 2 function and 1 immediate
			binary+="00100";// opcode for subtraction plus 1 bit for setting flags
			temp=op2.substr(1,3); //register source 1
			temp=toBinary(stoi(temp)); // convert to binary 
			temp=signextend(4,temp,true);// sign extend to 4 bits with zeros
			binary+=temp; // append to string
			temp=op1.substr(1,3); //register destination
			temp=toBinary(stoi(temp)); // convert to binary 
			temp=signextend(4,temp,true);// sign extend to 4 bits with zeros
			binary+=temp; // append to string
			temp=op3.substr(1,3); //register source 2
			temp=toBinary(stoi(temp)); // convert to binary 
			temp=signextend(12,temp,true);// sign extend to 12 bits with zeros (positive)
			binary+=temp;
		}
		else if((opcode=="AND")||(opcode=="and")){
			binary+="1110000";// same for chosen dp instructions in our implementation fisrt 4bits for condition 2 function and 1 immediate
			binary+="00000";// opcode for logical and plus 1 bit for setting flags
			temp=op2.substr(1,3); //register source 1
			temp=toBinary(stoi(temp)); // convert to binary 
			temp=signextend(4,temp,true);// sign extend to 4 bits with zeros
			binary+=temp; // append to string
			temp=op1.substr(1,3); //register destination
			temp=toBinary(stoi(temp)); // convert to binary 
			temp=signextend(4,temp,true);// sign extend to 4 bits with zeros
			binary+=temp; // append to string
			temp=op3.substr(1,3); //register source 2
			temp=toBinary(stoi(temp)); // convert to binary 
			temp=signextend(12,temp,true);// sign extend to 12 bits with zeros (positive)
			binary+=temp;
		}
		else if((opcode=="ORR")||(opcode=="orr")){
			binary+="1110000";// same for chosen dp instructions in our implementation fisrt 4bits for condition 2 function and 1 immediate
			binary+="11000";// opcode for logical or plus 1 bit for setting flags
			temp=op2.substr(1,3); //register source 1
			temp=toBinary(stoi(temp)); // convert to binary 
			temp=signextend(4,temp,true);// sign extend to 4 bits with zeros
			binary+=temp; // append to string
			temp=op1.substr(1,3); //register destination
			temp=toBinary(stoi(temp)); // convert to binary 
			temp=signextend(4,temp,true);// sign extend to 4 bits with zeros
			binary+=temp; // append to string
			temp=op3.substr(1,3); //register source 2
			temp=toBinary(stoi(temp)); // convert to binary 
			temp=signextend(12,temp,true);// sign extend to 12 bits with zeros (positive)
			binary+=temp;
		}
		else if((opcode=="CMP")||(opcode=="cmp")){
			binary+="1110000";// same for chosen dp instructions in our implementation fisrt 4bits for condition 2 function and 1 immediate
			binary+="10101";// opcode for compare plus 1 bit for setting flags set in this case
			temp=op1.substr(1,3); //register source 1 rn
			temp=toBinary(stoi(temp)); // convert to binary 
			temp=signextend(4,temp,true);// sign extend to 4 bits with zeros
			binary+=temp; // append to string
			binary+="0000"; // append to string // has no destination only updates the flags rd
			temp=op2.substr(1,3); //register source 2
			temp=toBinary(stoi(temp)); // convert to binary 
			temp=signextend(12,temp,true);// sign extend to 12 bits with zeros (positive)
			binary+=temp;
		}
		else if((opcode=="MOV")||(opcode=="mov")){
			binary+="1110000";// same for chosen dp instructions in our implementation fisrt 4bits for condition 2 function and 1 immediate
			binary+="11010";// opcode for mov plus 1 bit for setting flags 
			binary+="0000"; // append to string // has no source 1
			temp=op1.substr(1,3); //register destination
			temp=toBinary(stoi(temp)); // convert to binary 
			temp=signextend(4,temp,true);// sign extend to 4 bits with zeros
			binary+=temp; // append to string
			temp=op2.substr(1,3); //register source 2
			temp=toBinary(stoi(temp)); // convert to binary 
			temp=signextend(12,temp,true);// sign extend to 12 bits with zeros (positive)
			binary+=temp;
		}
		else if((opcode=="LDR")||(opcode=="ldr")){ // ldr r5,[r3,#32]
			binary+="111001";// same for chosen dt instructions in our implementation fisrt 4bits for condition 2 function and 1 immediate
			binary+="011000";// opcode for ldr plus 1 bit for setting flags 
			temp=op2.substr(2,4); //register source 1
			temp=toBinary(stoi(temp)); // convert to binary 
			temp=signextend(4,temp,true);// sign extend to 4 bits with zeros
			binary+=temp; // append to string
			temp=op1.substr(1,3); //register destination
			temp=toBinary(stoi(temp)); // convert to binary 
			temp=signextend(4,temp,true);// sign extend to 4 bits with zeros
			binary+=temp; // append to string
			temp=op3.substr(1,9); //register source 2
			temp.pop_back(); // removes the bracket
			temp=toBinary(stoi(temp)); // convert to binary 
			temp.pop_back(); // removes the last  bit of the immediate
			temp.pop_back(); // ^ same as above
			temp=signextend(12,temp,true);// sign extend to 12 bits with zeros (positive)
			binary+=temp;
			op3.pop_back(); // a little trimming
			op2=op2.substr(1,4);  // a little trimming
		}
		else if((opcode=="STR")||(opcode=="str")){ // str r5,[r3,#32]
			binary+="111001";// same for chosen dt instructions in our implementation fisrt 4bits for condition 2 function and 1 immediate
			binary+="011001";// opcode for str plus 1 bit for setting flags 
			temp=op2.substr(2,4); //register source 1
			temp=toBinary(stoi(temp)); // convert to binary 
			temp=signextend(4,temp,true);// sign extend to 4 bits with zeros
			binary+=temp; // append to string
			temp=op1.substr(1,3); //register destination
			temp=toBinary(stoi(temp)); // convert to binary 
			temp=signextend(4,temp,true);// sign extend to 4 bits with zeros
			binary+=temp; // append to string
			temp=op3.substr(1,9); //register source 2
			temp.pop_back(); // removes the bracket
			temp=toBinary(stoi(temp)); // convert to binary 
			temp.pop_back(); // removes the last  bit of the immediate
			temp.pop_back(); // ^ same as above
			temp=signextend(12,temp,true);// sign extend to 12 bits with zeros (positive)
			binary+=temp;
			op3.pop_back();   // a little trimming
			op2=op2.substr(1,4);  // a little trimming
		}
		else if((opcode=="BEQ")||(opcode=="beq")){ //  eg beq -96 minus means subtract from pc
			binary+="00001010";// same for chosen branch instructions in our implementation fisrt 4bits for condition 3 for 101 and 1 for link 
			temp=op1.at(0);
			if(temp=="+"){// add to pc
				temp=op1.substr(1,10);//get value
				temp=toBinary(stoi(temp)); // convert to binary
				temp.pop_back(); // removes the last  bit of the immediate
				temp.pop_back(); // ^ same as above
				temp=signextend(24,temp,true); // sign extend to 24 bits positively
			}
			else{ // subtract from pc
				temp=op1.substr(1,10);//get value
				temp=toBinary(stoi(temp)); // convert to binary
				temp=twos_complement(temp); // calculate 2s complement
				temp.pop_back(); // removes the last  bit of the immediate
				temp.pop_back(); // ^ same as above
				temp=signextend(24,temp,false); // sign extend to 24 bits negative
			}
			binary+=temp;	
		}
		else if((opcode=="BL")||(opcode=="bl")){ //  eg bl +96 minus means add to pc
			binary+="00001011";// same for chosen branch instructions in our implementation fisrt 4bits for condition 3 for 101 and 1 for link 
			temp=op1.at(0);
			if(temp=="+"){// add to pc
				temp=op1.substr(1,10);//get value
				temp=toBinary(stoi(temp)); // convert to binary
				temp.pop_back(); // removes the last  bit of the immediate
				temp.pop_back(); // ^ same as above
				temp=signextend(24,temp,true); // sign extend to 24 bits positively
			}
			else{ // subtract from pc
				temp=op1.substr(1,10);//get value
				temp=toBinary(stoi(temp)); // convert to binary
				temp=twos_complement(temp); // calculate 2s complement
				temp.pop_back(); // removes the last  bit of the immediate
				temp.pop_back(); // ^ same as above
				temp=signextend(24,temp,false); // sign extend to 24 bits negative
			}
			binary+=temp;	//append
		}
		else if((opcode=="NOP")||(opcode=="nop")){ // specific for catching missing socks
			binary="0";							// stalls everything for the purpose of delay
			binary=signextend(32,binary,true); // zeros of 32 bits			
		}
		else{
			binary="unsupported instruction"; // program should not crash but state it doesnt implement a specific instruction
		}
	}	
	void display(){
		if(numterms>=1)
			cout<<opcode<<" ";
		if(numterms>=2)
			cout<<op1<<" ";
		if(numterms>=3)
			cout<<op2<<" ";
		if(numterms>=4)
			cout<<op3<<" ";
		if(numterms>=5)
			cout<<op4<<" ";
		cout<<"\t#terms = "<<numterms<<" "<<binary<<endl;			
	}
	~instruction(){
		//cout<<"instruction terminated\n";
	}
};
class program { // holds a bunch of instructions 
protected:
	instruction* dataarray;
	int length; //numberof elements
	int size;// amount of allocated space
public:
	program(int x){
		dataarray = new instruction[x];
		length = 0;
		size = x;
	}
	program(const program& rhs){ // copyconstructor
		if (this != &rhs) {
			length=rhs.length;
			size=rhs.size;
			dataarray= new instruction[size];
			for(int i=0;i<length;i++){
				dataarray[i]=rhs.dataarray[i];
			}			// Deallocate, allocate new space, copy values...
		}
	}
	void MakeEmpty(){
		length = 0;
	}
	bool IsFull(){
		return (length == size);
	}
	int GetLength(){
		return length;
	}
	void InsertItem(string item){ // insert data line by line from input file
		if (IsFull()){
			instruction *ptr = new instruction[size + 5];
			for (int i = 0; i<size; i++){
				ptr[i] = dataarray[i];
			}
			delete[] dataarray;
			dataarray = ptr;
			size = size + 5;
			dataarray[length].transform(item);
			length++;
		}
		else{
			dataarray[length].transform(item);
			length++;
		}
	}
	void InsertItem(instruction item){ // copy items
		if (IsFull()){
			instruction *ptr = new instruction[size + 5];
			for (int i = 0; i<size; i++){
				ptr[i] = dataarray[i];
			}
			delete[] dataarray;
			dataarray = ptr;
			size = size + 5;
			dataarray[length] = item;
			length++;
		}
		else{
			dataarray[length] = item;
			length++;
		}
	}
	instruction getitem(int index){ // array like access (copy not alias)
		instruction temp;
		if ((index >= 0) && (index<length)){
			temp = dataarray[index];
		}
		return temp;
	}
	void DeleteItem(int index){
		for (int j = index; j<length - 1; j++){
			dataarray[j] = dataarray[j + 1];
		}
		length--;
	}
	void Show(){
		for (int i = 0; i<length; i++){
			dataarray[i].display();
		}
	}
	void ShowSize(){
		cout << "size is " << length;
	}
	program& operator=(const program &rhs) {// Only do assignment if RHS is a different object from this.
		if (this != &rhs) {
			delete [] dataarray;
			length=rhs.length;
			size=rhs.size;
			dataarray= new instruction [size];
			for(int i=0;i<length;i++){
				dataarray[i]=rhs.dataarray[i];
			}// Deallocate, allocate new space, copy values...
		}
    return *this;
  }
bool pipeinfo(int index){ // calculates some info regarding index th and previous 4 instructions
	instruction temp;
	static int turnsleft=4;
	if(index<length){// instructions left before finishing
		if(index==0){// first instruction pipe empty
			temp=this->getitem(index);
			temp.calstage(1);						  
		}
		else if(index==1){// 2 instructions in pipe
			temp=this->getitem(index);
			temp.calstage(1);
			temp=this->getitem(index-1);
			temp.calstage(2);				
		}
		else if(index==2){// 3 instructions in pipe
			temp=this->getitem(index);
			temp.calstage(1);
			temp=this->getitem(index-1);
			temp.calstage(2);	
			temp=this->getitem(index-2);
			temp.calstage(3);	
		}
		else if(index==3){// 4 instructions in pipe
			temp=this->getitem(index);
			temp.calstage(1);
			temp=this->getitem(index-1);
			temp.calstage(2);	
			temp=this->getitem(index-2);
			temp.calstage(3);
			temp=this->getitem(index-3);
			temp.calstage(4);
		}
		else {// pipe full 5 instructions
			temp=this->getitem(index);
			temp.calstage(1);
			temp=this->getitem(index-1);
			temp.calstage(2);	
			temp=this->getitem(index-2);
			temp.calstage(3);
			temp=this->getitem(index-3);
			temp.calstage(4);
			temp=this->getitem(index-4);
			temp.calstage(5);
		}
	}
	else{// need to execute the last 4 stages of last instruction
		if(turnsleft==4){
			index=length-1;
			temp=this->getitem(index);
			temp.calstage(2);
			temp=this->getitem(index-1);
			temp.calstage(3);	
			temp=this->getitem(index-2);
			temp.calstage(4);
			temp=this->getitem(index-3);
			temp.calstage(5);
			turnsleft--;
		}
		else if(turnsleft==3){
			index=length-1;
			temp=this->getitem(index);
			temp.calstage(3);
			temp=this->getitem(index-1);
			temp.calstage(4);	
			temp=this->getitem(index-2);
			temp.calstage(5);
			turnsleft--;
		}
		else if(turnsleft==2){
			index=length-1;
			temp=this->getitem(index);
			temp.calstage(4);
			temp=this->getitem(index-1);
			temp.calstage(5);	
			turnsleft--;
		}
		else if(turnsleft==1){
			index=length-1;
			temp=this->getitem(index);
			temp.calstage(5);
			turnsleft--;
			cout<<"\n------------------------------------------------------------\n";
			cout<<"Program execution complete\n";
			return false; // denotes no more lines to execute
		}
	}
	cout<<"\n------------------------------------------------------------\n";
	return true;
}
	~program(){
		delete [] dataarray;
	}
};
program file2mem(5); // holds instructions in a list
int main(){
	flagz=0;
	fstream filereader;
	string temp;
	filereader.open("code.txt",ios::in);
	if(!filereader){
		cout<<"Error reading input file(instruction file)\n";
	}
	else{
		int enteries=0;
		while(1){
				getline(filereader,temp);
				if(filereader.eof()){
					break;
				}
				enteries++;
				file2mem.InsertItem(temp);
		}
		cout<<enteries<<" lines of code have been succesfully loaded into main memory\n";
	}
	filereader.close();
	file2mem.Show();
	for(int i=0,k=0;i<4194304;i++){ 
		mem[i]=k;
		k+=4;
	}
	for(int i=0;i<15;i++){
		regfile[i]=i+11;
	}

	cout<<" _________________________________"<<endl;
	cout<<"| Address     |  0 |  1 |  2 |  3|"<<endl;
	for(int i=0;i<30;i++){
		temp=inttohex(mem[i]);
		cout<<"| "<<temp<<"    | "<<byteatoff(0,temp)<<" | "<<byteatoff(1,temp)<<" | "<<byteatoff(2,temp)<<" | "<<byteatoff(3,temp)<<"|"<<endl;		
	}
	cout<<"----------------------------------"<<endl;
	cout<<"______________________________"<<endl;
	cout<<"|Register Number\tvalue|"<<endl;
	for(int i=0;i<15;i++){
		cout<<"| R"<<i+1<<"\t\t\t   "<<regfile[i]<<"|\n"; // prints table for memory
	}
	cout<<"------------------------------"<<endl;
	cout<<"value of flagz before code execution ";
	if(flagz){
		cout<<"true"<<endl;
	}
	else{
		cout<<"false"<<endl;
	}
	regfile[15]=0;// all code starts from first line
	while(file2mem.pipeinfo(regfile[15])){
		regfile[15]++;
		int spacer;
		cin>>spacer;
	}
	cout<<"value of flagz after code execution ";
	if(flagz==1){
		cout<<"true"<<endl;
	}
	else{
		cout<<"false"<<endl;
	}
	cout<<" _________________________________"<<endl;
	cout<<"| Address     |  0 |  1 |  2 |  3|"<<endl;
	for(int i=0;i<30;i++){
		temp=inttohex(mem[i]);
		cout<<"| "<<temp<<"    | "<<byteatoff(0,temp)<<" | "<<byteatoff(1,temp)<<" | "<<byteatoff(2,temp)<<" | "<<byteatoff(3,temp)<<"|"<<endl;		
	}
	cout<<"----------------------------------"<<endl;
	cout<<"______________________________"<<endl;
	cout<<"|Register Number\tvalue|"<<endl;
	for(int i=0;i<15;i++){
		cout<<"| R"<<i+1<<"\t\t\t   "<<regfile[i]<<"|\n"; // prints table for memory
	}
	cout<<"------------------------------"<<endl;
	return 0;
}
