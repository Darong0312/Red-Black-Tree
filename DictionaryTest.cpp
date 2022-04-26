/****************************************************************************************
*  Darong Li, dli97
*  2022 Winter CSE101 PA8
*  DictionaryTest.cpp
*  Dictionary Test Client
*****************************************************************************************/
#include<iostream>
#include<string>
#include<stdexcept>
#include"Dictionary.h"

using namespace std;

int main(){

	string s[] = {"one","two", "three", "da", "dar", "daro", "par"};

	Dictionary A;

	for(int i = 0; i<7; i++){
		A.setValue(s[i],i+1);
	}

	cout << endl;
	cout << "test A in order string" << endl;
	cout << A << endl << endl;
	cout << "test A preorder string" << endl;
	cout << A.pre_string() << endl << endl;

	Dictionary B = A;
	cout << endl;
	cout << "Test operator=" << endl;
	cout << "A:" << endl << A << endl;
	cout << "B:" << endl << B << endl;

	Dictionary C;
	cout << endl;
	cout << "Test operator==" << endl;
	cout << (A==B?"true":"false") << endl;
	cout << (C==B?"true":"false") << endl;
	cout << (A==C?"true":"false") << endl;

	cout << endl;
	cout << "Test size()" << endl;
	cout << "A size: " << A.size() << endl;
	cout << "B size: " << B.size() << endl;
	cout << "C size: " << C.size() << endl;
	
	cout << endl;
	cout << "C=A ..." << endl;
	cout << "setting value for C..." << endl;
	C = A;
	C.setValue("something",8);
	cout << C << endl;
	cout << (C==B?"true":"false") << endl;
	cout << endl;

	cout << "Test begin(), hasCurrent(), next(), currentKey(), currentVal()" << endl;
	for(B.begin(); B.hasCurrent(); B.next()){
		string d = B.currentKey();
		int x = B.currentVal();
		cout << "key: " << d << " val: "<< x << endl;
	}
	cout << endl;

	cout << "Test end(), hasCurrent(), prev(), currentKey(), currentVal()" << endl;
	for(B.end(); B.hasCurrent(); B.prev()){
		string d = B.currentKey();
		int x = B.currentVal();
		cout << "key: " << d << " val: "<< x << endl;
	}
	cout << endl;

	cout << "Test remove()" << endl;
	A.remove("da");
	cout << "A size: " << A.size() << endl;
	cout << A << endl;

	return 0;
}
