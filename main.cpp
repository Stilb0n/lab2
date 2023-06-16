#include <QCoreApplication>
#include<iostream>
#include "Unit.cpp"
#include "lol.cpp"
int main(int argc, char *argv[])
{

    QCoreApplication a(argc, argv);
    std::cout << generateProgram2() << std::endl;

    return a.exec();
}
