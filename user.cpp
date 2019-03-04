//
//  user.cpp
//  OS5
//  用户及权限操作
//  Created by 陈明霏 on 2019/2/26.
//  Copyright © 2019年 陈明霏. All rights reserved.
//

#include <stdio.h>
#include <string>
#include "user.h"
#include "global.h"
using namespace std;

User::User(){
    this->username = root_usr_name;
    this->password = "";
}


bool User::usr_register(){
    if(this->username == root_usr_name){
        string name, psd;
        cout<<"Username: ";
        getline(cin, name);
        cout<<"Password: ";
        getline(cin, psd);
        usrmap.insert(make_pair(name,psd));
        cout<<"Register Successfully!"<<endl;
        this->username = name;
        this->password = psd;
        return true;
    }
    else{
        cout<<"Permission denied!"<<endl;
        return false;
    }
}

User::~User(){
    this->username = "";
    this->password = "";
}
bool User::login(){
    cout<<"Username: ";
    string name;
    getline(cin, name);
    if(usrmap.find(name)!=usrmap.end()){
        string psd;
        cout<<"Password: ";
        getline(cin, psd);
        if(psd!=usrmap[name]){
            cout<<"Password Wrong!"<<endl;
            return false;
        }
        this->username = name;
        this->password = psd;
        return true;
    }
    cout<<"Username not exist!"<<endl;
    return false;
    
}
bool User::logout(){
    this->~User();
    cout<<"Logout Successfully!"<<endl;
    return true;
}
string User::get_usrname(){
    return username;
}
