//
//  main.cpp
//  OS5
//  main
//  Created by 陈明霏 on 2019/2/26.
//  Copyright © 2019年 陈明霏. All rights reserved.
//


#include <iostream>
#include <string>
#include "inode.h"
#include "superblock.h"
#include "dirent.h"
#include "myFileSystem.h"
#include <map>
using namespace std;
char *base;
string root = "cmf";
bool running = true;
string root_usr_name = "cmf";
string root_usr_psd = "123456";
map<string,string> usrmap;
superBlock *s_block;
int main(int argc, const char * argv[]) {
    cout << "------------------------Welcome !--------------------------"<< endl;
    //分配100M的“硬盘空间" -> base
    base = (char*)malloc(100*1024*1024*sizeof(char));
    memset(base, 0, 100*1024*1024*sizeof(char));
    cout << "-----------100M Space Allocated Successfully !-------------"<< endl;
    //创建文件系统
    ((superBlock*)(&base[0]))->init();
    s_block = ((superBlock*)(&base[0]));
    myFileSystem filesys;
    usrmap.insert(make_pair(root_usr_name, root_usr_psd));
    filesys.filesys_main();
    cout << "------------------------Bye bye !--------------------------"<< endl;

    return 0;
}
