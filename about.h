#pragma once
#include "version.h"
#include <QString>
//TODO:设置为单例
class About
{
public:
    About();
    ~About() = default;
    QString GetAuthor() { return author; }
    QString GetVersion() { return version; }
    QString GetIntroduce() { return introduce; }
private:
    QString author;
    QString version;
    QString introduce;
};

About::About()
{
    author = "Listening";
    version = FILE_VERSION_STR;
    introduce = FILE_DESCRIPTION;
}
