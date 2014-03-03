//
//  Localize.h
//  CrossWordClient
//
//  Created by cy on 14-3-2.
//
//

#ifndef __CrossWordClient__Localize__
#define __CrossWordClient__Localize__

#include <iostream>
#include "jansson.h"

class Localize {
public:
    static Localize* sharedLocalize(int type = 0);
    const char* getString(const char* jsonName);
    
private:
    Localize(int type);
    ~ Localize();
    
    json_t *m_json;
};

#endif /* defined(__CrossWordClient__Localize__) */
