//
//  datablock.cpp
//  OS5
//  块管理层
//  Created by 陈明霏 on 2019/2/26.
//  Copyright © 2019年 陈明霏. All rights reserved.
//

#include "datablock.h"
#include <stdlib.h>
#include <string.h>
#include "global.h"

using namespace std;

//分配一个datablock
//位示图 012345678...顺序
datablock::datablock(){
    this->datablock_no = -1;
    if(s_block->block_remain<=0){
        //无剩余空闲block可分配
        cout<<"No remain block!"<<endl;
        this->datablock_pos = -1;
        this->datablock_no = -1;
    }
    else{
        unsigned int block_no = -1;
        //获取free块号
        for(unsigned int i=s_block->freeblock_pos;i<s_block->first_data_block;i++){
            char Byte = (short)base[i];
            for(int j=0;j<8;j++){
                if(((Byte>>(7-j))&1) == 0){
                    block_no = i*8+j;
                    //更改位示图
                    base[i] = char((1<<(7-j))|(short)base[i]);
                    s_block->block_remain -= 1;
                    this->datablock_no = i - s_block->freeblock_pos + j;
                    this->datablock_pos = s_block->first_data_block + this->datablock_no * s_block->blockSize;
                    break;
                }
            }
            if(this->datablock_no!=-1)
                break;
        }
        if(block_no==-1){
            cout<<"error!"<<endl;
            exit(-1);
        }
    }
}
datablock::datablock(unsigned int datablock_pos, unsigned int datablock_no){
    this->datablock_pos = datablock_pos;
    this->datablock_no = datablock_no;
}
//回收一个datablock
void datablock::deleteblock(){
    unsigned int bit = this->datablock_no % 8;
    unsigned int posi = this->datablock_no / 8 + s_block->freeblock_pos;
    //更改位示图
    base[posi] = char((~(1<<(7-bit)))&((short)base[posi]));
    s_block->block_remain += 1;
    //清空block区
    memset((char*)(&base[this->datablock_pos]), 0, s_block->blockSize);

}

datablock::~datablock(){

}


//block读
unsigned long datablock::bread(char *buffer, unsigned long type_size, unsigned long count){
    unsigned long size = type_size * count;
    unsigned long len;
    if(size>s_block->blockSize){
        cout<<"too large to read"<<endl;
        len  = size < s_block->blockSize ? size : s_block->blockSize;
    }
    else
        len = size;
    memcpy(buffer, &base[this->datablock_pos], len);
    return len;
}

//block写
unsigned long datablock::bwrite(const char *buffer, unsigned long type_size, unsigned long count){
    unsigned long size = type_size * count;
    unsigned long len;
    if(size>s_block->blockSize){
        cout<<"too large to write"<<endl;
        len  = size < s_block->blockSize ? size : s_block->blockSize;
    }
    else
        len = size;
    memcpy(&base[this->datablock_pos], buffer, len);
    return len;
}

//打印信息
void datablock::printInfo(){
    cout<<"-----------block info------------"<<endl;
    cout<<"block no:\t"<<this->datablock_no<<endl;
    cout<<"block pos:\t"<<this->datablock_pos<<endl;
    cout<<"--------------------------------"<<endl;
}
