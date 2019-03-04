//
//  datablock.h
//  OS5
//
//  Created by 陈明霏 on 2019/2/26.
//  Copyright © 2019年 陈明霏. All rights reserved.
//

#ifndef datablock_h
#define datablock_h

#include <stdio.h>
#include "superblock.h"
#include "global.h"
using namespace std;

class datablock{
    public:
    //data 
        unsigned int datablock_pos;     //数据块偏移位置
        unsigned int datablock_no;      //数据块号
        datablock();
        datablock(unsigned int datablock_pos, unsigned int datablock_no);
        ~datablock();
        void deleteblock();
    
        unsigned long bread(char *buffer, unsigned long type_size, unsigned long count);
        unsigned long bwrite(const char *buffer, unsigned long type_size, unsigned long count);
        void printInfo();
};

#endif /* datablock_h */
