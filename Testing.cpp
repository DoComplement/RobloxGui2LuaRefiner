
// Update to ignore connect-function calls	(use somevar = checkStr(str, "MouseButton", x) at each iteration of the while loop, then pass each iteration until checkStr(str, "end)", x) )

// There may be instances where the Instantiation will have something besides local, Instance.new, GetService, etc.
	// Known Case: Drawing.new <- potential solution:	have a vector for the type of object (Instance, Drawing, etc.) and input that into the createInstance function

//	Update to work with multiple headers

// (sometime late april 2022) Updated to ignore comments or blank spaces in any logical place
// (5/9/22) Updated to work with updated input format (new gui-to-lua with table of instances)

#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <windows.h>	// delete lines 9, 23, 24, 34, 92, and 93 if this interferes with compilation

using std::string;
using std::vector;
using std::ifstream;
using std::ofstream;

int find(vector<string> &, string);
void r(string, ofstream &, string, vector<string> &, int = 0);
int checkInst(string, int);
bool checkStr(string, string, int);
string low(string);
bool inc(string, int &);
int LINE = 0, l = 0;  // second variable is the character "l"

bool ret(char a, int num){
	char set[] = {' ', '\t', ')', '"', ']'};
	for (int i = -1; i++ < (num - 1);)
	{	if (a == set[i])	{	return true;	}}
	return false;
}

int main(){	
	std::cout << "Program Running...\n\n";
	
	LARGE_INTEGER frequency, t1, t2;
	QueryPerformanceFrequency(&frequency);
	
	ifstream In("Input.txt");	// input file
	ofstream Out("Output.txt");	// output file 
	
	Out << "\nlocal function createInstance(class, props)\n"
		<< "\tlocal inst = Instance.new(class)\n"
		<< "\tfor i, v in pairs(props) do"
		<< "\n\t\tinst[i] = v\n\tend\n\n\treturn inst\nend\n\n";
		
	QueryPerformanceCounter(&t1);
	
	vector<string> Names, ClassNames, Functs, Else;
	vector<int> cCheck, wCheck;
	string str, pastName = "nil", header;
	int v = 0;
	
	while(getline(In, str)) { LINE++; 
		int x = 0;
		for (int i = -1; i++ < static_cast<int>(str.length() - 1);) {
			if (str[i] == ' ' || str[i] == '\t') {	x++;	}
			else { break; }
		}
		
		int o = inc(str, v);
		if (v || o) {	r(str + ((o) ? ("\n\n") : ("\n")), Out, "", Functs, 1); continue;	}

		if (str[x] == '-' && str[x + 1] == '-') {	continue;	}
		else if (checkStr(str, ":Get", x)) {	Out << str << std::endl; continue;	}
		else if (checkInst(str, x)) {
			string tName, tClassName;
			int c1 = 1, c2 = 0;
			for (int i = (x - 1); i++ < static_cast<int>(str.length() - 2);){
				if (((i > (x + 5) && checkInst(str, x) == 1) || checkInst(str, x) == 2) && c1){ 
					tName += str[i];	
					if (str[i + 1] == ' ') {	c1 = 0;	}
				}
				else if (str[i] == '"' || str[i] == ')') {	c2++;	}
				if (c2 && c2 < 3) {	tClassName += str[i];	}
			}
			if (tClassName == "") {	header = tName;	l = tName.length();	}
			else {	 
				Names.push_back(tName); 
				ClassNames.push_back(tClassName);	
				cCheck.push_back(1);
				wCheck.push_back(1);
			}
		}
		else if (str.length() > 0){
			int check = 0, s1 = (l > 0) ? (l + 1) : (0);
			string tName, tPose;
			
			for (int i = (x - 1 + s1); i++ < static_cast<int>(str.length() - 1);){
				if (check) {	tPose += str[i];
					if (((str[i + 1] == '-' && str[i + 2] == '-') || i == static_cast<int>(str.length() - 1)) && wCheck.at(find(Names, tName))) 
					{	r(tPose, Out, header, Functs); break;	}
				}
				if (str[i] != '.' and !check) {	tName += str[i];	}
				else if ((i - x - s1) == static_cast<int>(tName.length())) {
					if (pastName != "nil" && wCheck.at(find(Names, tName))){
						if (pastName == tName) {	Out << ", ";	} 
						else {	Out << "})\n"; wCheck.at(find(Names, pastName)) = 0; }
					}
					check = 1;
					
					if (cCheck.at(find(Names, tName))) {
						Out << "local " << tName << " = createInstance("
							 << ClassNames.at(find(Names, tName)) << ", \{";
						cCheck.at(find(Names, tName)) = 0;
					}
				}
			}
			if (tName.length() > 0) {pastName = tName; }	
		}
	}	
	Out << "})\n\n";
	
	for (string str : Functs)
	{	Out << str;	}
	
	QueryPerformanceCounter(&t2);
	std::cout << "Program completed in " << (t2.QuadPart - t1.QuadPart) * 1000.00 / frequency.QuadPart << " ms.";
	
	In.close();
	Out.close();
	
	return 0;
}

int find(vector<string> &obj, string val){
	int temp = -1;
	
	for (int i = -1; i++ < static_cast<int>(obj.size() - 1);)
	{	if (obj.at(i) == val) {	temp = i;	}}
	
	// NOTE: This only checks for instantiation-related errors
	// There may be other unnoticed errors: 
	// Check with a lua executor which has input validation for correct output if unsure
	if (temp == -1) {	
		std::cout << "Some sort of error has occurred..."
				  << "Current Line: " << LINE;
		exit(EXIT_FAILURE); 
	}
	return temp;
}

void r(string obj, ofstream &Out, string str, vector<string> &f, int c){
	int count = 0, len1 = str.length(), len2 = obj.length();
	
	for (int i = static_cast<int>(len2); i-- > 0;) {
		if (obj[i] == ' ' || obj[i] == '\t') {	count++;	} 
		else {	break;	}
	}
	if (c){	f.push_back(obj.substr(0, len2 - count));	}
	else{
		if (obj.substr(0, 6) == "Parent" && len1 > 0){
			for (int i = -1; i++ < static_cast<int>(len2 - len1 - count);)
				if (obj.substr(i, i + len1).substr(0, len1) == str){
					Out << obj.substr(0, i) + obj.substr(i + len1 + 1, len2 - count);
					return;
				}
		}
		Out << obj.substr(0, len2 - count); 
	}
}

int checkInst(string str, int x){
	if (str.substr(x, x + 5).substr(0, 5) == "local"){	return 1;	}
	if (checkStr(str, ".new(\"", x)){	return 2;	}
	return false;
}

bool checkStr(string str, string c, int x){
	for (int i = (x - 1); i++ < static_cast<int>(str.length() - c.length());){
		if (str.substr(i, i + x + c.length()).substr(0, c.length()) == c)
			return true;
	}
	return false;
}

string low(string str){
	for (int i = -1; i++ < static_cast<int>(str.length() - 1);)
	{	str[i] = tolower(str[i]);	}	
	return str;	
}

bool inc(string str, int &num){	int t = 0;	
	for (int i = -1; i++ < static_cast<int>(str.length() - 1);){
		if (str[i] == '-' && str[i + 1] == '-') {	break;	}
		else if ((str.substr(i, i + 8).substr(0, 8) == "function" && (ret(str[i - 1], 3) || str[i - 1] == '(' || i == 0) &&
				(ret(str[i + 8], 2) || str[i + 8] == '(' || (i + 8) == static_cast<int>(str.length()))) ||
				(low(str.substr(i, i + 2).substr(0, 2)) == "do" && (ret(str[i - 1], 5) || i == 0) && 
				(ret(str[i + 2], 2) || (i + 2) == static_cast<int>(str.length()))) ||
				(low(str.substr(i, i + 4).substr(0, 4)) == "then" && (ret(str[i - 1], 5) || i == 0) &&
				(ret(str[i + 4], 2) || (i + 4) == static_cast<int>(str.length()))))
		{	num++;	}	
		if (num > 0) {	t = 1;	}
		if ((low(str.substr(i, i + 3).substr(0, 3)) == "end" && (ret(str[i - 1], 3) || i == 0) && 
		   (ret(str[i + 3], 3) || str[i + 3] == ';' || (i + 3) == static_cast<int>(str.length()))) || 
		   (low(str.substr(i, i + 6).substr(0, 6)) == "elseif" &&  (ret(str[i - 1], 3) || i == 0) &&
		   (ret(str[i + 6], 2) || (i + 6) == static_cast<int>(str.length()))))
		{	num--;	}
	}
	return (!num && t);
}
