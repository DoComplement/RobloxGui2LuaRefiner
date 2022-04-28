
#include <iostream>
#include <string>
#include <fstream>
#include <vector>

using std::string;
using std::vector;
using std::ifstream;
using std::ofstream;

int find(vector<string> &, string);
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
		if (str.substr(0, 5) == "local"){
			string tName, tClassName;
			int c1 = 1, c2 = 0;
			for (int i = -1; i++ < static_cast<int>(str.length() - 2);){
				if (i > 5 && c1){
					tName += str[i];	
					if (static_cast<int>(str[i + 1]) == 32) {	c1 = 0;	}
				}
				else if (str[i] == '"' && !c2) {	c2 = 1;	}
				if (c2) {	tClassName += str[i];	}
			}
			Names.push_back(tName); ClassNames.push_back(tClassName);
			cCheck.push_back(1);
		}
		else if (str.length() > 0){
			int check = 0; string tName;
			for (int i = -1; i++ < static_cast<int>(str.length() - 1);){
				if (check) {	Out << str[i];	}
				
				if (str[i] != '.' and !check) { tName += str[i];	}
				else if (i == static_cast<int>(tName.length())) {
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
