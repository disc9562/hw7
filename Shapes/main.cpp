#include "..\cppunitlite\TestHarness.h"
#include "utShapes.h"
#include "Document.h"
#include "Command.h"
#include "conio.h"
#include <stdio.h>
#include <string.h>
#include <string>
#include <iostream>
#include <vector>
#include <map>
#include <thread>
#include <chrono>
#include <windows.h>

#define DELIMS " \t\r\n"
#define HIGH_ORDER_BIT 0x8000
#define VK_Z 0x5A
#define VK_Y 0x59
using namespace std;

Shape * newShape(const string temp);
void ReUnCommand(CommandManager *cm);
/*inline bool isKeyDown(int VirtualCode)
{
   return (GetKeyState(VirtualCode) & HIGH_ORDER_BIT);
}

void keyevent()
{
   int ret;
   while(true)
   {
       if(isKeyDown(VK_CONTROL))
       {
           //Control has been pressed
           if(isKeyDown(VK_Z))
           {
                cout << "Control+Z\n";
           }
           else if(isKeyDown(VK_Y))
           {
                cout << "Control+Y\n";
           }
       }
       std::this_thread::sleep_for(std::chrono::milliseconds(100));
   }
}*/

int main()
{
    TestResult tr;
    TestRegistry::runAllTests(tr);
    string cmd;
    //string arg[10];
    //std::thread t1(keyevent);
    map<string, Media*> mapMedia;
    CommandManager cmdm;
    while(cmd != "exit"){
        cout<< ":- ";
        getline(cin, cmd);
        stringstream ss(cmd);
        string sub_str;
        vector<string> cmdBuffer;
        map<string, Media*>::iterator iter;

        while(getline(ss, sub_str, ' '))
            cmdBuffer.push_back(sub_str);

        string temp = cmdBuffer[cmdBuffer.size() - 1];

        if(cmd.find("def") != string::npos){
            DefCommand dc(&mapMedia, cmdBuffer[1], temp);
            cmdm.ExecuteCMD(&dc);
        }
        else if(cmd.find(".area") != string::npos){
            int pointIndex = cmd.find('.',0);
            string shapeName = cmd.substr(0,pointIndex);
            map<string, Media*>::iterator iter;
            iter = mapMedia.find(shapeName);
            AreaVisitor av;
            Media *m;

            m = iter -> second;
            m -> accept(&av);
            cout << ">> " << av.getArea() << endl;
        }

        else if(cmd.find(".perimeter") != string::npos){
            int pointIndex = cmd.find('.',0);
            string shapeName = cmd.substr(0, pointIndex);
            map<string, Media*>::iterator iter;
            iter = mapMedia.find(shapeName);
            PerimeterVisitor pv;
            Media *m;

            m = iter -> second;
            m -> accept(&pv);
            cout << ">> " << pv.getPerimeter() << endl;
        }

        else if (cmdBuffer[0] == "delete"){
            delCommand dc(&mapMedia, cmdBuffer);
            cmdm.ExecuteCMD(&dc);
        }
        else if (cmd.find("show") != string::npos){
            for(map<string, Media*>::iterator iter = mapMedia.begin(); iter != mapMedia.end(); iter++){
                cout << iter -> first << endl;
            }
        }
        else if (cmd.find("add") != string::npos){
            AddCommand ac(&mapMedia, cmdBuffer[1], cmdBuffer[3]);

            cmdm.ExecuteCMD(&ac);

            iter = mapMedia.find(cmdBuffer[3]);
            Media *cm = iter -> second;
            DescriptionVisitor dv;
            cm ->accept(&dv);
            NameVisitor nv;
            cm ->accept(&nv);
            cout << cmdBuffer[3] << " = " << nv.getName() << " = " << dv.getDescription() <<endl;
        }
        else if (cmd.find("save") != string::npos){
            iter = mapMedia.find(cmdBuffer[1]);
            vector<string> output;
            Media *cm =iter -> second;

            DescriptionVisitor dv;
            cm->accept(&dv);
            NameVisitor nv;
            cm->accept(&nv);
            output.push_back(dv.getDescription());
            output.push_back(nv.getName());

            //cout << cmdBuffer[3].substr(1,cmdBuffer[3].size() - 2) << endl;
            MyDocument md;
            md.writeDocument(cmdBuffer[3].substr(1,cmdBuffer[3].size() - 2),output);
            cout << ">> " << cmdBuffer[1] << " save to " << cmdBuffer[3].substr(1,cmdBuffer[3].size() - 2) << endl;
        }
        else if (cmd.find("load") != string::npos){
            MyDocument document;
            MediaDirctror md;
            string temp = document.openDocument(cmdBuffer[1].substr(1,cmdBuffer[1].size() - 2));

            stringstream temps(temp);
            string sub_str;
            vector<string> fileBuffer;
            cout << ">> " << "loading " << cmdBuffer[1].substr(1,cmdBuffer[1].size() - 2) << "..." << endl;

            while(getline(temps, sub_str, '\n'))
                fileBuffer.push_back(sub_str);

            md.concrete(fileBuffer[0]);
            stack<MediaBuilder *> mbs;

            mbs = md.getStack();
            ComboMedia *cm = mbs.top() ->getComboMedia();
            vector<string> nameVector;
            nameVector = md.getNameVector(fileBuffer[1]);

            cm -> setMedia(&mapMedia, &nameVector);

            DescriptionVisitor dv;
            cm->accept(&dv);
            NameVisitor nv;
            cm->accept(&nv);

            cout << nameVector[0] << " = " << nv.getName() << " = " << dv.getDescription() << endl;
        }
        else{
            cout << "error" << endl;
        }
        ReUnCommand(&cmdm);
    }
    return 0;
}

void ReUnCommand(CommandManager *cm){
    char key;
    int asciiValue = 0;
    while(asciiValue != 110){
        cout << "ctrl+z for Undo, ctrl+y for Redo, n for continue" <<endl;
        key = getch();
        asciiValue = key;
        if(asciiValue == 25){
            cm->RedoCMD();
        }
        else if(asciiValue == 26){
            cm->UndoCMD();
        }
    }
}
