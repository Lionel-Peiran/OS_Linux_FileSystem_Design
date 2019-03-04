//
//  myFileSystem.cpp
//  OS5
//  文件层面操作及命令解释
//  Created by 陈明霏 on 2019/2/26.
//  Copyright © 2019年 陈明霏. All rights reserved.
//

#include <stdio.h>
#include "myFileSystem.h"

//默认初始化目录或文件都是写死大小了，一旦建立大小也写死
#define DIRENT_SIZE 1
#define FILE_SIZE 10
#define RWXRNN 60
#define RWXRWX 63
#define RWXNNN 56
using namespace std;

//创建文件系统
myFileSystem::myFileSystem():root_inode(iNode(DIRENT_SIZE*s_block->blockSize, root_usr_name)),root_dentry(dirent(NULL, "root", root_inode, true)),user(User()){
    unsigned int ino_pos = root_inode.ino * sizeof(iNode) + s_block->inode_table;
    ((iNode*)(&base[ino_pos]))->copy(root_inode);
    this->curr_dentry = &root_dentry;
    cout << "FileSystem Created Successfully!"<< endl;
}

//注销文件系统
myFileSystem::~myFileSystem(){
     cout << "FileSystem Destroyed Successfully!"<< endl;
}


//dirent
void myFileSystem::ls(){
    if(this->curr_dentry->dirflag==true)
        this->curr_dentry->showdir();
    else
        cout<<this->curr_dentry->filename<<" is not a directory!"<<endl;
}
dirent* myFileSystem::cd(string filename){
    if(this->curr_dentry->parent!=nullptr && filename==".."){
        this->curr_dentry = this->curr_dentry->parent;
        cout << this->curr_dentry->path << endl;
        return this->curr_dentry;
    }
    if(filename=="."){
        return this->curr_dentry;
    }
    for(auto it=this->curr_dentry->child_list.begin();it!=this->curr_dentry->child_list.end();it++){
        if((*it)->filename == filename){
            this->curr_dentry = (*it);
            cout << this->curr_dentry->path << endl;
            return this->curr_dentry;
        }
    }
    cout<<filename<<" is not under "<<this->curr_dentry->path<<" !"<<endl;
    return this->curr_dentry;
}
void myFileSystem::mkdir(string dirname){
    if(dirname.find(" ")!=-1){
        cout << "Invalid Dirname! "<< endl;
        return;
    }
    iNode inode(DIRENT_SIZE*s_block->blockSize, this->user.get_usrname());
    unsigned int ino_pos = inode.ino * sizeof(iNode) + s_block->inodemap_pos;
    ((iNode*)(&base[ino_pos]))->copy(inode);
    dirent *newdir = new dirent(this->curr_dentry, dirname, inode, true);
   
}
void myFileSystem::rmdir(string dirname){
    for(auto it=this->curr_dentry->child_list.begin();it!=this->curr_dentry->child_list.end();it++){
        if((*it)->filename == dirname && (*it)->dirflag==true){
            this->curr_dentry->removeChild(*it);
            return;
        }
    }
    cout<<"Directory "<<dirname<<" is not under "<<this->curr_dentry->path<<" !"<<endl;
}

//file
void myFileSystem::create(string filename){
    if(filename.find(" ")!=-1){
        cout << "Invalid Filename! "<< endl;
        return;
    }
    iNode inode(FILE_SIZE*s_block->blockSize, this->user.get_usrname());
    unsigned int ino_pos = inode.ino * sizeof(iNode) + s_block->inodemap_pos;
    ((iNode*)(&base[ino_pos]))->copy(inode);
    dirent *newfile = new dirent(this->curr_dentry, filename, inode, false);

}
void myFileSystem::delfile(string filename){
    for(auto it=this->curr_dentry->child_list.begin();it!=this->curr_dentry->child_list.end();it++){
        if((*it)->filename == filename && (*it)->dirflag==false){
            this->curr_dentry->removeChild(*it);
            return;
        }
    }
    cout<<"File "<<filename<<" is not under "<<this->curr_dentry->path<<" !"<<endl;
}
void myFileSystem::openf(string filename){
    for(auto it=this->curr_dentry->child_list.begin();it!=this->curr_dentry->child_list.end();it++){
        if((*it)->filename == filename && (*it)->dirflag==false){
            this->curr_dentry = *it;
            cout<<"File "<<filename<<" is open now! "<<endl;
            return;
        }
    }
    cout<<"File "<<filename<<" is not under "<<this->curr_dentry->path<<" !"<<endl;
}

void myFileSystem::chmod(dirent *dir, unsigned int chom){
    dir->chom = chom;
    if(dir->dirflag==true){
        for(auto it=dir->child_list.begin();it!=dir->child_list.end();it++){
            if((*it)->path!=dir->path && ((dir->parent && (*it)->path!= dir->path)||dir->parent==NULL))
                chmod(*it, chom);
        }
    }
}

void myFileSystem::closef(string filename){
    if(this->curr_dentry->filename!=filename)
        cout<<"File "<<filename<<" is not open! "<<endl;

    else{
        this->curr_dentry = this->curr_dentry->parent;
        cout<<"File "<<filename<<" is close now! "<<endl;
        if(this->curr_dentry==nullptr){
            cout<<"error!"<<endl;
            exit(-1);
        }
    }
}
void myFileSystem::showf(string filename){
    if(this->curr_dentry->filename!=filename)
        cout<<"File "<<filename<<" is not open! "<<endl;
    else{
        this->curr_dentry->show();
    }
}
unsigned long myFileSystem::readf(){
    if(this->curr_dentry->dirflag==true){
        cout<<this->curr_dentry->filename<<" is a dir! "<<endl;
        return 0;
    }
    if(this->curr_dentry->user!= this->user.get_usrname() && !(this->curr_dentry->chom&4)){
        cout<<"Permission Denied!"<<endl;
        return 0;
    }
    else{
        char buffer[102400];
        cout << "Input Bytes num to read:";
        unsigned long count;
        scanf("%lu", &count);
        getchar();
        unsigned long len = this->curr_dentry->dread(buffer, sizeof(char), count);
        cout << "Read " << len << " Bytes: "<<endl;
        cout << buffer <<endl;
        return len;
    }
}
unsigned long myFileSystem::writef(){
    if(this->curr_dentry->dirflag==true){
        cout<<this->curr_dentry->filename<<" is a dir! "<<endl;
        return 0;
    }
    
    
    if(this->curr_dentry->user!= this->user.get_usrname() && !(this->curr_dentry->chom&2)){
        cout<<"Permission Denied!"<<endl;
        return 0;
    }
    else{
        cout << "Input content to write:";
        string buffer_s;
        getline(cin, buffer_s);
        const char *buffer = buffer_s.c_str();
        unsigned long len = this->curr_dentry->dwrite(buffer, sizeof(char), (unsigned long)buffer_s.length());
        cout << "Write " << len << " Bytes! "<<endl;
        return len;
    }
}

void myFileSystem::showdirdetail(){
    this->curr_dentry->showdirdetail();
}
//user

void myFileSystem::filesys_main(){
    //提示用户登录，否则就running为false
   if(this->user.login()!=true)
       running = false;
    cout << this->curr_dentry->path << endl;
    while(running){
        //获取输入string
        cout << "$ ";
        string s;
        getline(cin, s);
        //解析
        unsigned long len = s.find(" ");
        string com, target;
        if(len==-1){
            com = s;
            target = "";
        }
        else{
            unsigned long size = s.size();
            com = s.substr(0, len);
            target = s.substr(len+1, size);
        }
        
        //目录
        if(com == "cd"){
            cd(target);
            continue;
        }
        if(com == "ls"){
            ls();
            continue;
        }
        if(com == "showdir"){
            this->curr_dentry->show();
            continue;
        }
        if(com == "mkdir"){
            mkdir(target);
            
            continue;
        }
        if(com == "rmdir"){
            rmdir(target);
            continue;
        }
        if(com == "ll"){
            showdirdetail();
            continue;
        }


        //文件
        if(com == "fcreate"){
            create(target);
            continue;
        }
        if(com == "fdelete"){
            delfile(target);
            continue;
        }
        if(com == "fopen"){
            openf(target);
            continue;
        }
        if(com == "fclose"){
            closef(target);
            continue;
        }
        if(com == "fshow"){
            showf(target);
            continue;
        }
        //fwrite 32
        if(com == "fwrite"){

            writef();
            continue;
        }
        if(com == "fread"){

            readf();
            continue;
        }
        
        //用户权限
        if(com == "register"){
            this->user.usr_register();
            continue;
        }
        if(com == "login"){
            if(this->user.login()!=true)
                running = false;
            this->curr_dentry->user = this->user.get_usrname();
            continue;
        }
        if(com == "logout"){
            this->user.~User();
            running = false;
            continue;
        }
        if(com == "chmod"){
            cout<<"New Chomd Type: ";
            unsigned int chom;
            scanf("%u", &chom);
            getchar();
            bool flag = false;
            for(auto it=this->curr_dentry->child_list.begin();it!=this->curr_dentry->child_list.end();it++){
                if((*it)->filename == target && (*it)->path!=this->curr_dentry->path && ((this->curr_dentry->parent && (*it)->path!=this->curr_dentry->path)||this->curr_dentry->parent==NULL)){
                    chmod((*it),chom);
                    flag = true;
                    break;
                }
            }
            if(flag==false)
               cout<<"File "<<target<<" is not under "<<this->curr_dentry->path<<" !"<<endl;
            continue;
        }
        cout << "Invalid Command! "<< endl;
    }
}


