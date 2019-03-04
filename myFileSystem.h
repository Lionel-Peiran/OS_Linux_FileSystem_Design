//
//  myFileSystem.h
//  OS5
//  control in filesystem 包含dirent、操作、权限等
//  Created by 陈明霏 on 2019/2/26.
//  Copyright © 2019年 陈明霏. All rights reserved.
//

#ifndef myFileSystem_h
#define myFileSystem_h


#include <iostream>
#include "superblock.h"
#include "inode.h"
#include "dirent.h"
#include "user.h"
#include "global.h"
#include <map>
using namespace std;
//define 权限

/*
 具体设计ops
 a)目录：ls mkdir rm cd
 b)权限/用户：chmod、login、logout、register（root下）注：root是最早init写死了的
 c)文件：create(创建文件)、delete（删除文件）、show（显示文件）、 write、 read、 open、 close
/Users/chenmingfei/Documents/作业U201617123/操作系统课设/OS课设/OS5/OS5/OS5/user.cpp d)解析命令
 */

//dirent相当于文件，本类型相当于文件系统，功能为解释各类命令，在文件层面进行操作

class myFileSystem{
    public:
        User user;
        iNode root_inode;   //根目录inode
        dirent root_dentry;//根目录（iNode root)
        dirent *curr_dentry;//工作目录，即当前目录
        void filesys_main();//命令行工作
        myFileSystem();   //初始化root
        ~myFileSystem();  //注销
    
        //dirent
        void ls();                                     //ls
        dirent* cd(string filename);                   //cd
        void mkdir(string dirname);                    //mkdir
        void rmdir(string dirname);                    //rm
        //file
        void create(string filename);                  //create
        void delfile(string filename);                 //delete
        void openf(string filename);                   //open
        void closef(string filename);                  //close
        void showdirdetail();                          //show detail
        void showf(string filename);                   //show
        unsigned long readf();  //read
        unsigned long writef(); //write
        //user
        void chmod(dirent *dir, unsigned int chom);
    
};



#endif /* myFileSystem_h */
