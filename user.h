//
//  user.h
//  OS5
//  用户信息管理
//  Created by 陈明霏 on 2019/2/26.
//  Copyright © 2019年 陈明霏. All rights reserved.
//

#ifndef user_h
#define user_h
#include <iostream>
using namespace std;

class User{
    string username;
    string password;
public:
    User();
    ~User();
    bool login();
    bool logout();
    bool usr_register();
    string get_usrname();
};

#endif /* user_h */
