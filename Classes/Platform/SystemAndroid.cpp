//android的系统方法

#include "System.h"

#include "cocos2d.h"
#include "jni/JniHelper.h"


USING_NS_CC;

// 检测WIFI是否可用
bool System::isWIFIEnabled()
{
	JniMethodInfo method;
	//if (JniHelper::getStaticMethodInfo(method, "com/og/common/OGMainActivity", "isWifiAvailable", "()Z"))
	if (JniHelper::getStaticMethodInfo(method, "com/crossword/main/CrossWordClient", "isNetworkAvailable", "()Z"))
	{
		JNIEnv* env = method.env;
		bool enabled = env->CallStaticBooleanMethod(method.classID, method.methodID);
        
		env->DeleteLocalRef(method.classID);
		return enabled;
	}
    return false;
}

// 检测3G网络是否可用
bool System::is3GEnabled()
{
	JniMethodInfo method;
	//if (JniHelper::getStaticMethodInfo(method, "com/og/common/OGMainActivity", "is3GAvailable", "()Z"))
	if (JniHelper::getStaticMethodInfo(method, "com/crossword/main/CrossWordClient", "isNetworkAvailable", "()Z"))
	{
		JNIEnv* env = method.env;
		bool enabled = env->CallStaticBooleanMethod(method.classID, method.methodID);
        
		env->DeleteLocalRef(method.classID);
		return enabled;
	}
    
	return false;
}

//网络是否可用
bool System::isNetAvailable()
{
    return   (isWIFIEnabled() ||  is3GEnabled());
//    return true;
}