#pragma once
#include "version.h"
#include <QString>

#if __cplusplus >= 201103L

class About
{

public:
    static About &GetInstance();
#else
#include <mutex>
std::mutex mymutex;
class About
{
public:
    static About *GetInstance();
#endif

    QString GetAuthor() { return author; }
    QString GetVersion() { return version; }
    QString GetIntroduce() { return introduce; }

    class DeleteClass // 类中套类，用来释放对象
    {
      public:
        ~DeleteClass()
        {
            if(About::m_instance)
            {
                delete About::m_instance;
                About::m_instance = nullptr;
            }
        }
    };

private:
    About();
    static About *m_instance; //静态成员变量
    static DeleteClass deleter;// 定义一个静态成员变量，程序结束时，系统会自动调用它的析构函数从而释放单例对象

private:
    QString author;
    QString version;
    QString introduce;
};

About *About::m_instance = nullptr;
About::DeleteClass deleter;

About::About()
{
    author = "Listening";
    version = FILE_VERSION_STR;
    introduce = FILE_DESCRIPTION;
}

#if __cplusplus >= 201103L
//static Singleton* get_instance()
//这样做并不好，无法避免用户使用delete instance导致对象被提前销毁。建议使用返回引用的方式。
About &About::GetInstance()
{
    // 《Effective C++》中简洁的singleton写法 Meyers' Singleton
    // C++11及以后的版本（如C++14）的多线程下，正确。
    // 因为C++11以后,规定了local static在多线程条件下的初始化行为，要求编译器保证了内部静态变量的线程安全性
    static About m_instance;
    return m_instance;
}
#else
// 传统经典方式
About *About::GetInstance()
{
    if(m_instance == nullptr)
    {
        std::lock_guard<std::mutex> mylock(mymutex);
        if(m_instance == nullptr)
        {
            m_instance = new About();
        }
    }
    return m_instance;
}
#endif
