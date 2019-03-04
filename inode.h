//
//  inode.h
//  OS5
//  iNode
//  Created by 陈明霏 on 2019/2/26.
//  Copyright © 2019年 陈明霏. All rights reserved.
//

#ifndef inode_h
#define inode_h
#include <iostream>
#include "global.h"
#include "superblock.h"
#define MAX_BLOCK_NUM 13
using namespace std;

/*块指针分配
 0-9:直接指针
 10：一级间接索引指针
 11：二级间接索引指针
 12：三级间接索引指针
 
 simple版本都是直接指针
 */
class iNode{
    public:
        unsigned int ino;//i节点号
        //unsigned short i_mode;//文件模式16位，前4位表示文件类型，后12位表示权限
        unsigned long i_size;//文件最大大小，单位字节
        unsigned long size;//文件实际大小，单位字节
        unsigned long i_blocks;//文件所占块数
        string i_uid;//所属用户
        unsigned int i_zone[MAX_BLOCK_NUM];//块指针
        unsigned short i_count;//引用计数
        unsigned short i_nlink;//硬链接数
        unsigned long iread(char *buffer, unsigned long type_size, unsigned long count);
        unsigned long iwrite(const char *buffer, unsigned long type_size, unsigned long count);
        void iNodeinit(unsigned long size, string uid);
        iNode(unsigned long size, string uid);
        void copy(iNode &inode);
        void deleteiNode();
        ~iNode();
        void printInfo();
};
//inode大小：18*4+8*2 = 88字节



#endif /* inode_h */
