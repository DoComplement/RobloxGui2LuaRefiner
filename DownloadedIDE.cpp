
// Updated to ignore comments or blank spaces in any logical place

#include <iostream>
#include <string>
#include <fstream>
#include <vector>

using std::string;
using std::vector;
using std::ifstream;
using std::ofstream;

int find(vector<string> &, string);
void r(string &, ofstream &);
int LINE = 0;

int main(){
	
	ifstream In("Input.txt");	// input file
	ofstream Out("Output.txt");	// output file 
	
	Out << "\nlocal function createInstance(class, props)\n"
		<< "\tlocal inst = Instance.new(class)\n"
		<< "\tfor i, v in pairs(props) do"
		<< "\n\t\tinst[i] = v\n\tend\n\n\treturn inst\nend\n\n";
	
	vector<string> Names, ClassNames;
	vector<int> cCheck;
	string str, pastName = "nil";
	while(getline(In, str)) { LINE++;
		int x = 0;
		for (int i = -1; i++ < static_cast<int>(str.length() - 1);) {
			if (str[i] == ' ' || str[i] == '\t') {	x++;	}
			else { break; }
		}
		if (str[x] == '-' && str[x + 1] == '-') {	continue;	}
		else if (str.substr(x, x + 5).substr(0, 5) == "local"){
			string tName, tClassName;
			int c1 = 1, c2 = 0;
			for (int i = (x - 1); i++ < static_cast<int>(str.length() - 2);){
				if (i > (x + 5) && c1 && str[i] != ' '){ 
					tName += str[i];	
					if (str[i + 1] == ' ') {	c1 = 0;	}
				}
				else if (str[i] == '"' || str[i] == ')') {	c2++;	}
				if (c2 && c2 < 3) {	tClassName += str[i];	}
			}
			Names.push_back(tName); ClassNames.push_back(tClassName);
			cCheck.push_back(1);
		}
		else if (str.length() > 0){
			int check = 0; string tName, tPose;
			for (int i = (x - 1); i++ < static_cast<int>(str.length() - 1);){
				if (check) {	tPose += str[i];
					if ((str[i + 1] == '-' && str[i + 2] == '-') || i == static_cast<int>(str.length() - 1))
					{	r(tPose, Out); break;	}
				}
				if (str[i] != '.' and !check) {	tName += str[i];	}
				else if ((i - x) == static_cast<int>(tName.length())) {
					if (pastName != "nil")
					{	Out << ((pastName == tName) ? (", ") : ("})\n"));	}
					check = 1;
					
					if (cCheck.at(find(Names, tName))) {
						Out << "local " << tName << " = createInstance("
							 << ClassNames.at(find(Names, tName)) << ", \{";
						cCheck.at(find(Names, tName)) = 0;
					}
				}
			}
			pastName = tName;	
		}
	}	
	Out << "})";
	
	In.close();
	Out.close();
	
	return 0;
}

int find(vector<string> &obj, string val){
	int temp = -1;
	for (int i = -1; i++ < static_cast<int>(obj.size() - 1);)
	{	if (obj.at(i) == val) {	temp = i;	}}
	if (temp == -1) {
		std::cout << "Some sort of error has occurred..."
				  << "Current Line: " << LINE;
		exit(EXIT_FAILURE);
	}
	return temp;
}

void r(string &obj, ofstream &Out){
	int count = 0;
	for (int i = static_cast<int>(obj.length()); i-- > 0;) {
		if (obj[i] == ' ' || obj[i] == '\t') {	count++;	} 
		else {	break;	}
	}
	Out <<  obj.substr(0, obj.length() - count);
}
