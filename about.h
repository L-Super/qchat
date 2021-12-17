#pragma once
#include <QString>
//TODO:还不清楚这样是否规范
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
    version = "1.0.0";
    introduce = ("QChat is a local chat software");
}
