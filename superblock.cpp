//
//  superblock.cpp
//  OS5
//
//  Created by 陈明霏 on 2019/2/26.
//  Copyright © 2019年 陈明霏. All rights reserved.
//

#include "superblock.h"
#include <math.h>
#define inode_size 88
using namespace std;



/*
 磁盘空间分配
 superblock                                    块0（1块）
 inode 88字节                                   块1-1000(1000块）
 Inode位图                                      块1001-1002(2块）
 free_block位图                                 块1003-1015(13块）
 data_block                                    块1016-102399(101384块）  99M
 */
int superBlock::init()
{
    this->blockSize = 1024;                            //块大小,1KB
    this->inode_num = 11630;                           //i节点数目
    this->inode_remain = 11630;                        //i节点剩余数目
    this->block_num = 1024 * 100;                      //磁盘块数
    this->block_remain = 1024 * 100;                   //磁盘块剩余数
    this->inode_table = this->blockSize*1;             //iNode位图的存放偏移位置
    this->inodemap_pos = ceil((this->inode_table + inode_num*inode_size) *1./ blockSize) * blockSize;//空闲iNode表偏移位置
    this->freeblock_pos = (inodemap_pos*1./blockSize + ceil(inode_num/8./blockSize))*blockSize;         //空闲块位图偏移位置
    this->first_data_block = ceil(ceil(freeblock_pos + block_num / 8.) *1./ blockSize) * blockSize;     //第一个数据块的位置
    this->first_data_block_no = ceil(first_data_block*1./blockSize);//第一个数据块的号码
    this->blockSize_bit = 10;//块大小占用位数
    this->maxBytes = 1024*1024*10;//文件最大大小
    this->block_remain -= first_data_block_no;//去掉保留块
    return 1;
}

superBlock::superBlock(){
    superBlock::init();
    //写入base
}

superBlock::~superBlock(){
    
}

