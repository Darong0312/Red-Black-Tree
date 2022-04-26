/****************************************************************************************
*  Darong Li, dli97
*  2022 Winter CSE101 PA8
*  Order.cpp
*  main program for pa8 (RED BLACK TREE)
*****************************************************************************************/
#include <iostream>
#include <string>
#include <stdexcept>
#include <fstream>

#include "Dictionary.h"

using namespace std;

int main(int argc, char* argv[]){
	ifstream in;
	ofstream out;

	if( argc != 3 ){
		cerr << "Usage: " << argv[0] << " <input file> <output file>" << endl;
		return(EXIT_FAILURE);
	}

	// open files for reading and writing 
   
	in.open(argv[1]);
	if( !in.is_open() ){
		cerr << "Unable to open file " << argv[1] << " for reading" << endl;
		return(EXIT_FAILURE);
	}

	out.open(argv[2]);
	if( !out.is_open() ){
		cerr << "Unable to open file " << argv[2] << " for writing" << endl;
		return(EXIT_FAILURE);
	}

	Dictionary D;

	string line;
	int line_count = 1;

	while( getline(in,line) ){
		D.setValue(line,line_count);
		line_count++;
	}

	out << D << endl;

	out << D.pre_string() << endl;

	in.close();
	out.close();
	return 0;
}

