#include <QCoreApplication>
#include<iostream>
#include "Unit.cpp"
int main(int argc, char *argv[])
{
    CsharpFabric* s = new CsharpFabric;
    QCoreApplication a(argc, argv);
    std::cout << generateProgram2(s) << std::endl;

    return a.exec();
}
