//
//  dirent.cpp
//  OS5
//  dirent操作
//  Created by 陈明霏 on 2019/2/26.
//  Copyright © 2019年 陈明霏. All rights reserved.
//

#include <stdio.h>
#include "dirent.h"
#include "datablock.h"
#include <math.h>
using namespace std;
#define RWXRNN 60
#define RWXRWX 63
#define RWXNNN 56
//不考虑硬软链接，即一个parent

dirent::dirent(dirent *parent, string filename,iNode inode, bool dirflag):inode(inode){
    this->user = inode.i_uid;
    this->chom = RWXRNN;
    this->dirflag = dirflag;
    this->filename = filename;
    this->parent = parent;
    if(parent==NULL)
        this->path = "/" + filename;
    else{
        this->path = parent->path + "/" + filename;
        //child list
        parent->addChild(this);
    }
    if(dirflag==true){
        
        //.,..
        if(parent){
            this->inode.i_nlink += 1;
            this->inode.i_count += 1;
            this->child_list.push_back(parent);
        }
        
        this->child_list.push_back(this);
 
    }
    
}

//mkdir or create
void dirent::addChild(dirent *child){
    if(this->dirflag==true){
        this->child_list.push_back(child);
        child->parent = this;
        this->inode.i_count += 1;
        this->inode.i_nlink += 1;
    }
    else
        cout<<"File can't add child!"<<endl;
    
}


//mv
void dirent::setParent(dirent *parent){
    if(this->parent&&this->parent->path!=parent->path){
        //双方childlist删除
        this->parent->child_list.erase(remove(this->parent->child_list.begin(), this->parent->child_list.end(), this), this->parent->child_list.end());
        if(this->dirflag==true)
            this->child_list.erase(remove(this->child_list.begin(), this->child_list.end(), this->parent), this->child_list.end());
    }
    //更换parent
    this->parent = parent;
    //新parent孩子列表变化
    if(parent){
        parent->addChild(this);
        this->child_list.push_back(parent);
        //0 ..; 1 .
    }
    
}


//rm
void dirent::removeChild(dirent *s){
    this->child_list.erase(remove(this->child_list.begin(), this->child_list.end(), s), this->child_list.end());
    this->inode.i_nlink -= 1;
    this->inode.i_count -= 1;
    
    s->~dirent();
}

//ls
void dirent::show(){
    vector<string> type= {"File", "Dir"};
    map<unsigned int,string> index_chom;
    index_chom.insert(make_pair(63,"rwxrwx"));
    index_chom.insert(make_pair(60,"rwxr--"));
    index_chom.insert(make_pair(56,"rwx---"));
    cout<<"-----------dirent info------------"<<endl;
    cout<<"filename:\t"<<this->filename<<endl;
    cout<<"path:\t"<<this->path<<endl;
    cout<<"chom:\t"<<index_chom[this->chom]<<endl;
    cout<<"max file size:\t"<<this->inode.i_size<<endl;
    cout<<"real file size: "<<this->inode.size<<endl;
    cout<<"type:\t"<<type[this->dirflag]<<endl;
    cout<<"user:\t"<<this->inode.i_uid<<endl;
    cout<<"i_count:\t"<<this->inode.i_count<<endl;
    cout<<"--------------------------------"<<endl;
}

void dirent::showdir(){
    if(this->dirflag==true){
        if(this->parent)
            cout<<".. ";
        cout<<". ";

        for(unsigned long i=0;i<this->child_list.size();i++){
            if(this->child_list[i]->path!=this->path && ((this->parent && this->child_list[i]->path!=this->parent->path)||this->parent==NULL))
                cout<<this->child_list[i]->filename<<"  ";
        }
        cout<<endl;
        
    }
    else
        cout<<"Not a directory!"<<endl;
}

void dirent::showdirdetail(){
    map<unsigned int,string> index_chom;
    index_chom.insert(make_pair(63,"rwxrwx"));
    index_chom.insert(make_pair(60,"rwxr--"));
    index_chom.insert(make_pair(56,"rwx---"));
    vector<string> type= {"File", "Dir"};
    if(this->dirflag==true){
        for(unsigned long i=0;i<this->child_list.size();i++){
            if(this->child_list[i]->path!=this->path && ((this->parent && this->child_list[i]->path!=this->parent->path)||this->parent==NULL)){
                cout<<this->child_list[i]->filename<<"\t"<<index_chom[this->child_list[i]->chom]<<"\t";
                cout<<type[this->child_list[i]->dirflag]<<"\t"<<this->child_list[i]->user<<"\t";
                if(this->child_list[i]->dirflag==false)
                    cout<<this->child_list[i]->inode.size<<"\t"<<child_list[i]->inode.i_count<<endl;
                else
                    cout<<"-"<<"\t"<<child_list[i]->inode.i_count<<endl;
            }
        }
        cout<<endl;
        
    }
    else
        cout<<"Not a directory!"<<endl;
}

//read
unsigned long dirent::dread(char *buffer, unsigned long type_size, unsigned long count){
    return this->inode.iread(buffer, type_size, count);
}

//write
unsigned long dirent::dwrite(const char *buffer, unsigned long type_size, unsigned long count){
    return this->inode.iwrite(buffer, type_size, count);;
}

dirent::~dirent(){
    this->filename = "";
    this->path = "";
    unsigned int ino_pos = this->inode.ino * sizeof(iNode) + s_block->inodemap_pos;
    ((iNode*)(&base[ino_pos]))->deleteiNode();
    this->inode.~iNode();
    if(this->parent){
        this->parent->child_list.erase(remove(this->parent->child_list.begin(), this->parent->child_list.end(), this), this->parent->child_list.end());
        this->parent->inode.i_nlink -= 1;
        this->parent->inode.i_count -= 1;
        this->parent = nullptr;
    }
    while(this->child_list.size()>2){
        this->child_list[child_list.size()-1]->~dirent();
        this->child_list.pop_back();
    }
}





