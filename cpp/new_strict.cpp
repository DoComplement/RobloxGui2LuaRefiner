// follows "strict" (untampered) input format

#include <iostream>
#include <fstream>
#include <vector>

using std::ifstream;
using std::ofstream;
using std::vector;
using std::string;

void append_class(const string line, vector<vector<string>> &classes){
	classes.push_back(vector<string>{
		line.substr(0, line.find(' ')),
		string(line.begin() + line.find('\"'), line.end() - 1)
	});
}

size_t find_idx(const string name, vector<vector<string>> classes){
	for(size_t idx{}; idx < classes.size(); ++idx) if(classes[idx][0]==name) return idx;
	return classes.size();
}

string concat(vector<string> TBL, char split, char end){
	string TMP{};
	for(string STR : TBL) TMP += (STR + (STR != TBL.back() ? split : end));
	return TMP;
}

// edit these filenames (they are, somewhat, not case sensitive)
const string INPUT_FILENAME{"input.lua"};
const string OUTPUT_FILENAME{"output.lua"};

int main(){
	
	ifstream IN(INPUT_FILENAME);
	
	vector<vector<string>> classes{};		// name,class
	vector<vector<string>> properties{};	// assignments
	vector<string> parents{};				// parents
	vector<string> OTHER{};					// everything else

	string line{};
	while(getline(IN,line)){
		if(!line.size()||line.substr(0,2)=="--") continue;
		if(line.substr(0,5)=="local"&&line.substr(6,8)!="function"){
			append_class(line.substr(6), classes);
			parents.push_back(string{});
			properties.push_back(vector<string>{});
			continue;
		};
		
		string name{line.substr(0, line.find('.'))};
		if(name==line){
			OTHER.push_back(line);
			continue;
		}
		
		size_t idx{find_idx(name,classes)};
		if(idx==classes.size()){
			OTHER.push_back(line);
			continue;
		}
		
		string assn{string(line.begin() + name.size() + 1, line.end())};
		if(assn.substr(0,6)=="Parent") parents[idx] = assn.substr(assn.find("= ") + 2);
		else properties[idx].push_back(assn);
	};
	
	IN.close();
	
	ofstream OUT(OUTPUT_FILENAME);
	OUT << "\n-- executes \"new_inst\" format\n"
		<< "local function new_inst(class,parent,props)\n"
		<< "\tlocal inst = Instance.new(class);\n"
		<< "\tfor prop,val in next,props do\n"
		<< "\t\tinst[prop] = val;\n\tend;\n"
		<< "\tinst.Parent = parent;\t-- appease Instance.new listeners\n"
		<< "\treturn inst;\nend;\n\n"
		<< "-- instances, edit toward further efficiency\n";
	
	
	for(size_t idx{}; idx < classes.size(); ++idx)
		OUT << "local " << classes[idx][0] << " = new_inst(" << classes[idx][1] << ", "
			<< parents[idx] << ", {" << concat(properties[idx], ',', '}') << ");\n";
	
	OUT << "\n-- everything else\n" << concat(OTHER, '\n', '\0');
	OUT.close();
};
