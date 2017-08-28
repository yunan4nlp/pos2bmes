#include <iostream>
#include <fstream>
#include <vector>
#include <string>

using namespace std;

#include "Utf.h"

struct Instance {
	vector<string> words;
	vector<vector<string> > chars;
	vector<string> pos;
	int size;

	void resize(int size) {
		words.resize(size);
		chars.resize(size);
		pos.resize(size);
		this->size = size;
	}

	void save(ofstream &out) const {
		for (int idx = 0; idx < size; idx++) {
			const vector<string>& curchar = chars[idx];
			const string& curpos = pos[idx];
			int charsize = curchar.size();
			if (charsize == 1)
				out << curchar[0] << "_s_" << curpos << " ";
			else {
				for (int idy = 0; idy < charsize; idy++) {
					if(idy == 0)
						out << curchar[idy] << "_b_" << curpos << " ";
					if(idy == charsize - 1)
						out << curchar[idy] << "_e_" << curpos << " ";
					if(idy != 0 && idy != charsize - 1)
						out << curchar[idy] << "_m_" << curpos << " ";
				}
			}
		}
		out << endl;
	}
};


void read_pos(const char* path, vector<Instance>& insts);
void split_bychar(const string& str, vector<string>& vec, const char separator);
void write_bmes(const char* path, vector<Instance>& insts);

int main(int argc, char* argv[]) {
	vector<Instance> insts;
	read_pos(argv[1], insts);
	write_bmes(argv[2], insts);
	cout << "transfer format ok" << endl;
	return 0;
}

void split_bychar(const string& str, vector<string>& vec, const char separator = ' ') {
	//assert(vec.empty());
	vec.clear();
	string::size_type pos1 = 0, pos2 = 0;
	string word;
	while ((pos2 = str.find_first_of(separator, pos1)) != string::npos) {
		word = str.substr(pos1, pos2 - pos1);
		pos1 = pos2 + 1;
		if (!word.empty())
			vec.push_back(word);
	}
	word = str.substr(pos1);
	if (!word.empty())
		vec.push_back(word);
}

void read_pos(const char* path, vector<Instance>& insts) {
	insts.clear();
	ifstream file(path);
	if (file.is_open()) {
		string line;
		vector<string> info;
		while (getline(file, line)) {
			split_bychar(line, info, ' ');
			Instance inst;
			int sent_len = info.size();
			inst.resize(sent_len);
			for (int idx = 0; idx < sent_len; idx++) {
				const string& curinfo = info[idx];
				int pos = curinfo.find_last_of("_");
				const string& curword = curinfo.substr(0, pos);
				inst.words[idx] = curword;
				inst.pos[idx] = curinfo.substr(pos + 1, -1);
				getCharactersFromUTF8String(curword, inst.chars[idx]);
			}
			insts.push_back(inst);
		}
	}
	else
		cout << "open file error" << endl;
}

void write_bmes(const char* path, vector<Instance>& insts) {
	ofstream out(path);
	if(out.is_open()){
		for (auto it = insts.begin(); it != insts.end(); it++)
			it->save(out);
		out.close();
	}
}
