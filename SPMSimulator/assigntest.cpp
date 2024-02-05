#include "assigntest.h"

AssignTest::AssignTest(QObject *parent) : QObject(parent)
{

}

AssignTest &AssignTest::operator=(const AssignTest &other)
{
    this->str1 = other.str1;
    this->str2 = other.str2;
    this->var1 = other.var1;

    return *this;
}
