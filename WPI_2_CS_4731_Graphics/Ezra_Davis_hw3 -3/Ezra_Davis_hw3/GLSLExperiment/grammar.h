#include <iostream>
#include <unordered_map>
using namespace std;

class Grammar {
public:
	Grammar(){};
  Grammar(const char * filename);


  string getPattern();
  int length();
  float rotationX();
  float rotationY();
  float rotationZ();


  static const string *listFiles();

private:
  string pattern;

  void iterate();
  void loadFile(const char *);
  
  unordered_map<char,char> rep;
  unordered_map<char, string> rules;
  int len, iter;
  float rotx, roty, rotz;
  
  static const string fileNames[];
};
