//
//  inode.cpp
//  OS5
//
//  Created by 陈明霏 on 2019/2/26.
//  Copyright © 2019年 陈明霏. All rights reserved.
//

#include <stdio.h>
#include <vector>
#include <math.h>
#include "inode.h"
#include "datablock.h"
using namespace std;

//分配inode
/*
 size:文件大小
 uid：用户
 */
void iNode::iNodeinit(unsigned long size, string uid){
    this->ino = -1;
    this->size = 0;
    if(s_block->inode_remain<=0){
        //无剩余空闲inode可分配
        cout<<"No remain inode!"<<endl;
        this->ino = -1;
    }
    else{
        unsigned int inode_no = -1;
        //获取free块号
        for(unsigned int i=s_block->inode_table;i<s_block->inodemap_pos;i++){
            short Byte = (short)base[i];
            for(int j=0;j<8;j++){
                if(((Byte>>(7-j))&1) == 0){
                    inode_no = i*8+j;
                    //更改位示图
                    base[i] = char((1<<(7-j))|(short)base[i]);
                    s_block->inode_remain -= 1;
                    this->ino = i - s_block->inode_table + j;
                    this->i_size = size;
                    this->i_blocks = ceil(size*1./s_block->blockSize);
                    this->i_uid = uid;
                    for(int num=0;num<this->i_blocks;num++){
                        datablock dblock;
                        //simple版本
                        if(num>MAX_BLOCK_NUM-1)
                            break;
                        this->i_zone[num] = dblock.datablock_pos;
                    }
                    this->i_count = 1;
                    this->i_nlink = 1;
                    cout<<"first block pos: "<<this->i_zone[0]<<"  ino: "<<this->ino<<endl;
                    break;
                }
                
            }
            if(this->ino!=-1)
                break;
        }
        if(inode_no==-1){
            cout<<"error!"<<endl;
            exit(-1);
        }
    }
    
}

void iNode::copy(iNode &inode){
    this->ino = inode.ino;
    this->i_size = inode.i_size;
    this->size = inode.size;
    this->i_blocks = inode.i_blocks;
    this->i_uid = inode.i_uid;
    

    for(int i=0;i<MAX_BLOCK_NUM;i++)
        this->i_zone[i] = inode.i_zone[i];
    this->i_count = inode.i_count;
    
    this->i_nlink = inode.i_nlink;
}

iNode::iNode(unsigned long size, string uid){
    iNodeinit(size, uid);
}
unsigned long iNode::iread(char *buffer, unsigned long type_size, unsigned long count){
    unsigned long size = type_size * count;
    unsigned long blocksize = s_block->blockSize;
    unsigned long truesize = size < blocksize * this->i_blocks ? size : blocksize * this->i_blocks;
    unsigned long full_block = truesize / blocksize;
    unsigned long moresize = truesize % blocksize;
    char* p = buffer;
    unsigned long i, len = 0;
    for(i=0;i<full_block;i++){
        datablock *block = (datablock*)(&base[this->i_zone[i]]);
        len += block->bread(p, sizeof(char), blocksize);
        p = p + blocksize;
    }
    datablock *block = (datablock*)(&base[this->i_zone[i]]);
    len = max(block->bread(p, sizeof(char), moresize), len);
    return len;
}


unsigned long iNode::iwrite(const char *buffer, unsigned long type_size, unsigned long count){
    unsigned long size = type_size * count;
    unsigned long blocksize = s_block->blockSize;
    unsigned long truesize = size < blocksize * this->i_blocks ? size : blocksize * this->i_blocks;
    unsigned long full_block = truesize / blocksize;
    unsigned long moresize = truesize % blocksize;
    char* p = (char*)(&(*buffer));
    unsigned long i, len = 0;
    for(i=0;i<full_block;i++){
        datablock *block = (datablock*)(&base[this->i_zone[i]]);
        len += block->bwrite(p, sizeof(char), blocksize);
        p = p + blocksize;
    }
    datablock *block = (datablock*)(&base[this->i_zone[i]]);
    len += block->bwrite(p, sizeof(char), moresize);
    this->size = max(this->size, len);
    return len;
}


//释放inode
void iNode::deleteiNode(){
    unsigned int bit = this->ino % 8;
    unsigned int posi = this->ino / 8 + s_block->inode_table;
    //更改位示图
    base[posi] = char((~(1<<(7-bit)))&((short)base[posi]));
    s_block->inode_remain += 1;
    //清空inode区
    this->ino = -1;
    this->size = 0;
    for(int num=0;num<this->i_blocks;num++){
        //释放block
        datablock dblock(this->i_zone[num],(this->i_zone[num]-s_block->first_data_block)/s_block->blockSize);
        dblock.deleteblock();
        this->i_zone[num] = -1;
    }
}

iNode::~iNode(){
    
}

//打印信息
void iNode::printInfo(){
    cout<<"-----------iNode info------------"<<endl;
    cout<<"inode no:\t"<<this->ino<<endl;
    cout<<"max file size:\t"<<this->i_size<<endl;
    cout<<"real file size: "<<this->size<<endl;
    cout<<"block num: "<<this->i_blocks<<endl;
    cout<<"uid:\t"<<this->i_uid<<endl;
    cout<<"count:\t"<<this->i_count<<endl;
    cout<<"nlink:\t"<<this->i_nlink<<endl;
    cout<<"--------------------------------"<<endl;
}


