#include<iostream>
#include<string.h>
using namespace std;



Class Book
{
Public:
    Book(char char BookID[20],BookName[20],char Author[20],char Publisher[20],short Storage,bool BookState,short BookMan,short TStorage)
    {
        int i;
        for(i=0; i<20; i++)
        {
            bookID[i]=BookID[i];
        }
        for(i=0; i<20; i++)
        {
            bookName[i]=BookName[i];
        }
        for(i=0; i<20; i++)
        {
            author[i]=Author[i];
        }
        for(i=0; i<20; i++)
        {
            publisher[i]=Publisher[i];
        }
        storage=Storage;
        BookState=bookState;
        bookMan=BookMan;
        tStorage=TStorage;
    }
    char *getbookID(char BookID[20])
    {
        return bookID;
    }
    char *getbookName()
    {
        return bookName;
    }
    char *getauthor()
    {
        return author;
    }

//继续编写


Private:
    char bookID[20];
    char bookName[20];
    char author[20];
    char publisher[20];
    short storage;
    bool bookState;
    short bookMan;
    short tStorage;
    char flag[1];  //图书是否可用

};


Class Card
{
Public:
    Card();
    setPassword
    updatePassword
    setbanlance
    setcard
// 需要编写
    Private：
    char cardID[11];
    char aPassword[20];
    short lendedCount;
    short lendingCount;
    char cardHolder[10];
    char cardState[1];   //账号冻结状态
    double balance;
    double oweMoney;   //违约金
    short bookedCount;
    char cID[18];  //身份证号
    char cPhone[11];
    char flag[1];  //账号是否可用
};


Class Library
{
Public:

    void charge(Card card);
    void bookLend(Book book);
    void bookReturn(Book book);
    void bookOrder(Book book);
    void bookRenew(Book book);

private:


};

Class Administrator
{
Public:

    void addBook(Book book);
    void deleteBook(Book book);
    void newStorage(Card card);
    void searchRecord();
    void operateCard(Card card);

Private:
    char cardID[11];
    char aPassword[20];
    char cardHolder[10];
    char cID[18];
    char cPhone[11];

}

Class Record
{
Public:

    void bookLendRecord();
    void bookReturnRecord();
    void bookOrderRecord();
    void bookRenewRecord();


Private:


}

void login();
void register();
void update();
