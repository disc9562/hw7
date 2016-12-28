#ifndef COMMAND_H_INCLUDED
#define COMMAND_H_INCLUDED

#include<stack>
#include<map>
#include<string>
#include<vector>
#include "utShapes.h"
#include "Document.h"
using namespace std;

class Command{
public:
    Command(){}
    virtual void Execute() = 0;
    virtual void Undo() = 0;
    virtual void Redo() = 0;
protected:
    map<string, Media*> *cmdMap;
};

class CommandManager{
public:
    CommandManager(){}
    void ExecuteCMD(Command *cmd){
        cmd->Execute();
        undocmds.push(cmd);
    }

    void RedoCMD(){
        redocmds.top()->Redo();
        undocmds.push(redocmds.top());
        redocmds.pop();
    }

    void UndoCMD(){
        undocmds.top()->Undo();
        redocmds.push(undocmds.top());
        undocmds.pop();
    }
    int getUndoSize(){
        return undocmds.size();
    }
    int getRedoSize(){
        return redocmds.size();
    }
private:
    stack<Command *> undocmds;
    stack<Command *> redocmds;
};

class AddCommand : public Command{
public:
    AddCommand(map<string, Media*> *mediaMap, string shapeMedia, string comboMedia){
        cmdMap = mediaMap;
        from = shapeMedia;
        to = comboMedia;
    }

    void Execute(){
        map<string, Media*>::iterator iter;
        iter = cmdMap->find(to);
        Media *cm = iter -> second;

        iter = cmdMap->find(from);
        cm ->add(iter -> second);
    }

    void Undo(){
        map<string, Media*>::iterator iter;
        iter = cmdMap-> find(to);
        Media *cm = iter->second;

        iter = cmdMap->find(from);
        cm->removeMedia(iter->second);
    }

    void Redo(){
        map<string, Media*>::iterator iter;
        iter = cmdMap->find(to);
        Media *cm = iter -> second;

        iter = cmdMap->find(from);
        cm ->add(iter -> second);
    }

private:
    string from;
    string to;
};
Shape * newShape(const string temp);
class DefCommand : public Command{
public:
    DefCommand(map<string, Media*> *mediaMap, string name, string args){
        cmdMap = mediaMap;
        shapeName = name;
        argv = args;
    }

    void Execute(){
        Shape * s = newShape(argv);
        if(argv.find("Circle(") != string::npos){
            s->setName(shapeName);
            cmdMap->insert(pair<string, Media*>(shapeName, new ShapeMedia(s)));
            cout << ">> " << argv << endl;
        }
        else if(argv.find("Rectangle(") != string::npos){
            s->setName(shapeName);
            cmdMap->insert(pair<string, Media*>(shapeName, new ShapeMedia(s)));
            cout << ">> " << argv << endl;
        }
        else{
            int firstIndex = argv.find('{',0);
            int endIndex = argv.find('}',0);
            string shapeName1 = argv.substr(firstIndex + 1, endIndex - (firstIndex + 1));

            stringstream ss(shapeName1);
            string sub_str;
            vector<string> name;
            ComboMedia *cm = new ComboMedia;
            while(getline(ss, sub_str, ',')){
                map<string, Media*>::iterator iter = cmdMap->find(sub_str);
                cm->add(iter -> second);
            }
            cm ->setName(shapeName);
            cmdMap->insert(pair<string, Media*>(shapeName, cm));
        }
    }

    void Undo(){
        cmdMap -> erase(shapeName);
    }

    void Redo(){
        Execute();
    }
private:
    string shapeName;
    string argv;
};
class delCommand : public Command{
public:
    delCommand(map<string, Media*> *mediaMap, vector<string> buffer){
        cmdMap = mediaMap;
        argvBuffer = buffer;
    };

    void Execute(){
        map<string, Media*>::iterator iter;
        cacheMap = *cmdMap;
        if (argvBuffer.size() == 2){
            iter = cmdMap->find(argvBuffer[1]);
            for(map<string, Media*>::iterator it = cmdMap->begin(); it != cmdMap->end(); it++){
                it -> second -> removeMedia(iter -> second);
            }
            cmdMap->erase(iter);
        }
        else if(argvBuffer.size() == 4){
            iter = cmdMap->find(argvBuffer[3]);
            Media *cm = iter -> second;

            iter = cmdMap->find(argvBuffer[1]);
            cm -> removeMedia(iter -> second);
        }
    }

    void Undo(){
        *cmdMap = cacheMap;
    }

    void Redo(){
        Execute();
    }

private:
    vector<string> argvBuffer;
    map<string, Media*> cacheMap;
};
Shape * newShape(const string temp){
    int startIndex = temp.find('(',0);
    int endIndex = temp.find(')',0);
    //cout << startIndex << ":::" << endIndex << endl;
    string argv = temp.substr(startIndex + 1, endIndex);
    stringstream ss(argv);
    string sub_str;
    vector<string> argvBuffer;

    while(getline(ss, sub_str, ','))
        argvBuffer.push_back(sub_str);

    switch(argvBuffer.size()){
        case 3:{
            stringstream toDouble1(argvBuffer[0]);
            stringstream toDouble2(argvBuffer[1]);
            stringstream toDouble3(argvBuffer[2]);
            double x, y, r;
            toDouble1 >> x;
            toDouble2 >> y;
            toDouble3 >> r;
            return new Circle(x, y, r);
        }
        case 4:{
            stringstream toDouble1(argvBuffer[0]);
            stringstream toDouble2(argvBuffer[1]);
            stringstream toDouble3(argvBuffer[2]);
            stringstream toDouble4(argvBuffer[3]);
            double x, y, l, h;
            toDouble1 >> x;
            toDouble2 >> y;
            toDouble3 >> l;
            toDouble4 >> h;
            return new Rectangle(x, y, l, h);
        }
        default:
            return nullptr;
    }
}
#endif // COMMAND_H_INCLUDED
