//
//  dirent.h
//  OS5
//  目录项
//  Created by 陈明霏 on 2019/2/26.
//  Copyright © 2019年 陈明霏. All rights reserved.
//

/*
 获取路径
 显示子目录
 判断是否为目录
 设置或者更换路径（父节点）
 删除子节点（删除一个目录项只能在其父节点处删）
 设置子节点
 */
#ifndef dirent_h
#define dirent_h

#include <iostream>
#include <string.h>
#include <vector>
#include "inode.h"
#include "superblock.h"
#include "global.h"
using namespace std;

/*
 具体设计ops
 a)目录：ls mkdir rm cd(开） mv
 b)权限/用户：chmod、login、logout、register（root下）注：root是最早init写死了的
 c)文件：create(创建文件)、delete（删除文件）、show（显示文件）、 write、 read、 open、 close
 d)解析命令
 */
class dirent{
    public:
        bool dirflag;
        string filename;
        string path;
        string user;
        unsigned int chom;
        iNode inode;
        vector<dirent*> child_list;     //子目录（含有., ..)
        dirent * parent;                //父目录
        /////////////基本操作//////////////
        dirent(dirent *parent, string filename,iNode inode, bool dirflag); //创建文件目录
        ~dirent(); //删除文件目录
    
        /////////////接口//////////////
        void setParent(dirent *);       //设置父亲节点(mv 自己)
        void addChild(dirent *);        //添加子节点(mkdir)
        void removeChild(dirent *s);    //删除子节点(rm)
        void show();//展示自己的dirent信息(ls)
        void showdir();
        void showdirdetail();
        ///////////目录、文件///////////
        //读
        unsigned long dread(char *buffer, unsigned long type_size, unsigned long count);
        //写
        unsigned long dwrite(const char *buffer, unsigned long type_size, unsigned long count);
        ///////////权限///////////
        //改权限
};

#endif /* dirent_h */
