#ifndef DOCUMENT_H_INCLUDED
#define DOCUMENT_H_INCLUDED

#include<iostream>
#include<fstream>
#include <string>
#include <vector>
using namespace std;

class Document
{
public :
    string openDocument(const string name){
	if(!canOpenDocument(name))
		throw std::string("file is not existed.");
	openFile(name);
	return readFile();
    }
    void writeDocument(string docName, vector<string> output){
        openFileWrite(docName);
        writeFile(output);
    }
protected :
    virtual void openFile(const string name) = 0;
    virtual bool canOpenDocument(const string name) = 0;
    virtual string readFile() = 0;
    virtual void openFileWrite(string docName) = 0;
    virtual void writeFile(vector<string> output) = 0;
};

class MyDocument : public Document{
public:
    void openFile(const std::string name){
        file.open(name, ios::in);
    }

    bool canOpenDocument(const std::string name){
        file.open(name, ios::in);
        if(!file)
            return false;
        else
            return true;
    }

    string readFile(){
        std::string content( (std::istreambuf_iterator<char>(file) ),(std::istreambuf_iterator<char>()));
        return content;
    }
    void openFileWrite(string docName){
        file.open(docName, ios::out);
    }
    void writeFile(vector<string> output){
        for(string s:output){
            file << s << endl;
        }
        file.close();
    }
private:
    fstream file;
};
#endif // DOCUMENT_H_INCLUDED
