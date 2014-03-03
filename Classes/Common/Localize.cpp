//
//  Localize.cpp
//  CrossWordClient
//
//  Created by cy on 14-3-2.
//
//

#include "Localize.h"
#include "Utilities.h"

static Localize* s_localize = NULL;

Localize::Localize(int type)
{
    switch (type)
    {
        case 0://cn
            m_json = Utilities::getJsonFromFile("Text/strings.json");
            
            break;
            
        default:
            break;
    }
}

Localize::~Localize()
{
    
}

Localize *Localize::sharedLocalize(int type)
{
    if (!s_localize)
    {
        s_localize = new Localize(type);
    }
    return s_localize;
}

const char* Localize::getString(const char* jsonName)
{
    if (m_json != NULL)
    {
        return json_string_value(json_object_get(m_json, jsonName));
    }
    return "";
}
