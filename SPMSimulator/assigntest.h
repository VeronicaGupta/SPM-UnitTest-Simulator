#ifndef ASSIGNTEST_H
#define ASSIGNTEST_H

#include <QObject>
#include <QDebug>

class AssignTest : public QObject
{
    Q_OBJECT
public:
    explicit AssignTest(QObject *parent = nullptr);

    AssignTest& operator=(const AssignTest&other);

    QString str1;
    QString str2;
    int var1;
signals:

//private:

};

class MyString {
public:
    MyString() : data(nullptr) {}

    // Constructor that takes a C-string
    MyString(const char* str) {
        data = new char[strlen(str) + 1];
        strcpy(data, str);
    }

    // Copy constructor
    MyString(const MyString& other) {
        data = new char[strlen(other.data) + 1];
        strcpy(data, other.data);
    }

    // Destructor to release memory
    ~MyString() {
        delete[] data;
    }

    // Overload the assignment operator (=)
    MyString& operator=(const MyString& other) {
        if (this == &other) {
            // Self-assignment; no action needed
            return *this;
        }

        // Release the old data
        delete[] data;

        // Copy the new data
        data = new char[strlen(other.data) + 1];
        strcpy(data, other.data);

        return *this;
    }

    // Display the string
    void display() const {
        qDebug() << data ;
    }

private:
    char* data;
};

#endif // ASSIGNTEST_H
