//
//  superblock.h
//  OS5
//  superblock
//  Created by 陈明霏 on 2019/2/26.
//  Copyright © 2019年 陈明霏. All rights reserved.
//

#ifndef superblock_h
#define superblock_h

#include <iostream>

using namespace std;

class superBlock{
    public:
        unsigned short inode_num;          //i节点数目
        unsigned short inode_remain;       //i节点剩余数目
        unsigned int block_num;            //磁盘块数
        unsigned int block_remain;         //磁盘块剩余数目
        unsigned int inode_table;          //iNode空闲表的存放偏移位置
        unsigned int inodemap_pos;         //iNode节点偏移地址
        unsigned int freeblock_pos;        //空闲块管理区偏移地址
        unsigned short blockSize;          //块大小
        unsigned short blockSize_bit;      //块大小占用位数
        unsigned long long maxBytes;       //文件最大大小
        unsigned int first_data_block;     //第一个数据块偏移位置
        unsigned int first_data_block_no;  //第一个数据块号

        superBlock();
        ~superBlock();
        void printInfo();
        int init();//初始化新的superBlock
};



#endif /* superblock_h */
