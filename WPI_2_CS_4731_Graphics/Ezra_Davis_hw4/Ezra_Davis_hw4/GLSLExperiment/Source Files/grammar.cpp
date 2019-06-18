#include "..\\Header Files\\grammar.h"

#define LINESZ 512

const string Grammar::fileNames[] = {
	".\\lsys_files\\lsys1.txt",
	".\\lsys_files\\lsys2.txt",
	".\\lsys_files\\lsys3.txt",
	".\\lsys_files\\lsys4.txt"
};

const string *Grammar::listFiles() {
	return fileNames;
}

Grammar::Grammar(const char * filename) {
  Grammar::loadFile(filename);
  
  if(!string(filename).compare(".\\lsys_files\\lsys3Dhilbert.txt" ) ){
	  cout << "hilbert:" << endl;
	  iter =2;
  }

  iterate();
  
  if(!string(filename).compare(".\\lsys_files\\lsys3Dhilbert.txt" )) {// ".\\lsys_files\\lsys#.txt"
	cout << "hilbert: " << getPattern().c_str() << endl;
  }
}

void Grammar::loadFile(const char * filename) {
  cout << filename <<endl;
  FILE * file;
  fopen_s(&file, filename, "r");
  
  if(file == NULL) {
	  cout << "Invalid file: " << filename;
	  exit(1);
  }
  
  char buff[LINESZ];
  
  fgets(buff, LINESZ, file);
  
  
  while(buff[0] == '#') {
    fgets(buff, LINESZ, file);
  }
  

                            sscanf_s(buff, "len: %d", &len);
  fgets(buff, LINESZ,file); sscanf_s(buff, "iter: %d", &iter);
  fgets(buff, LINESZ,file); sscanf_s(buff, "rot: %f %f %f", &rotx, &roty, &rotz);
  
  cout << "len: " << len << "\titer: " << iter << "\trot: " << rotx << " " << roty << " " << rotz << endl;

  //rep lines
  fgets(buff, LINESZ,file);
  while(buff[0] == 'r' && buff[1] == 'e' && buff[2] == 'p' && buff[3] == ':')
  {
    char key, value;

    //sscanf_s(buff, "rep: %c,%c", &key, &value);
	//cout << '.';
	key = buff[5];
	value = buff[7] ? buff[7] : ' ';//Null to ' ' - replace with nothing

    rep[key] = value;
    
	cout << "rep: " << key << "," <<(int) value << endl;

    fgets(buff, LINESZ, file);
  }

  //start line
  pattern = buff+7; //7 for 'start: '
  
  cout << "start: " << string(pattern).c_str() << endl;

  //production rules
  while(fgets(buff, LINESZ,file) != NULL) {
    char key;

    sscanf_s(buff,  "%c:", &key);

	string value = buff+2;

    rules[key] = (value);

	cout << key << ":" << value.c_str() << endl;
  }
  //End file
  cout << "closing file" << endl;
  fclose(file);
}

void Grammar::iterate() {
  
  while(iter > 0) {
    iter--;
    string temp = "";
    for(unsigned int i=0; i<pattern.length(); i++) {
	  char &c = pattern[i];
      if(rules.find(c) != rules.end())
        temp += rules[c];
      else
        temp += c;
    }
    pattern = temp;
  }
  
  for(unsigned int i=0; i<pattern.length(); i++) {
	char &c = pattern[i];
    if(rep.find(c) != rep.end()) {
      pattern[i] = rep[c];
	}
  }
  
}

string Grammar::getPattern() {
  
  return pattern;
}

int Grammar::length() {
	return len;
}

float Grammar::rotationX() {
	return rotx;
}

float Grammar::rotationY() {
	return roty;
}

float Grammar::rotationZ() {
	return rotz;
}
