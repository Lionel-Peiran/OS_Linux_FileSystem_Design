//
//  global.h
//  OS5
//
//  Created by 陈明霏 on 2019/2/27.
//  Copyright © 2019年 陈明霏. All rights reserved.
//

#ifndef global_h
#define global_h
//分配空间时 按字节分配 char*指针
#include <string.h>
#include <map>
#include "superblock.h"

using namespace std;
extern char* base;
extern string root;
extern bool running;
extern string root_usr_name;
extern string root_usr_psd;
extern map<string,string> usrmap;

extern superBlock *s_block;
#endif /* global_h */
