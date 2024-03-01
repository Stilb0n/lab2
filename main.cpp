#include <QCoreApplication>
#include<iostream>
#include "Unit.cpp"

int main(int argc, char *argv[])
{
    setlocale(LC_ALL,"Russian");

 // CsharpFabric* s = new CsharpFabric;
 //  CFabric* ss = new CFabric;
 JavaFabric* sss = new JavaFabric;
    QCoreApplication a(argc, argv);
  std::cout << generateProgram2(sss) << std::endl;

    return a.exec();
}
