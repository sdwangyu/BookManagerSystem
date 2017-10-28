#include<iostream>
#include<string.h>
using namespace std;

int allcard=5000;
int allbook=100000;

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
    short bookMan; //预约人数
    short tStorage;  //临时库存 
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
    char cPassword[20];
    short lendedCount;//已借本数
    short lendingCount;//可借本数
    char cardHolder[10];
    char cardState[1];   //账号冻结状态
    double balance;
    double oweMoney;   //违约金
    short bookedCount;//预约本数    已借+可借+预约=10 提示不可借不可预约
    char cID[18];  //身份证号
    char cPhone[11];
    char flag[1];  //账号是否可用
};


Class Administrator
{
Public:

    void addBook(Book book);//增加书
    //void deleteBook(Book book);老师说不要删书
    void newStorage(Book book);//新设库存
    void searchRecord();//查询记录
    //void operateCard(Card card);老师说不要删卡 听老师的
Private：
    char account[11];
    char aPassword[20];
    char accountHolder[10];
    char aID[18];
    char aPhone[11];

}


Class Library
{
Public:
    void signIn();//用户和管理员登陆
    void signUp();//用户注册
    void signOut();//用户注销
    void update();//函数用于用户进入系统时 对缓冲区进行更新
    void charge();//充值函数
    void Rcharge();//处理用户违约金
    void resetCard();//更新修改卡信息 姓名 身份证 手机
    void Search();//查询书本函数
    void PersonalIformation();//用户信息 显示当前借书和预约状态
    void bookLend(Book book);//借书
    void bookReturn(Book book);//还书
    void bookOrder(Book book);//预约
    void bookorderCancel(Card card);//取消预约
    void bookRenew(Book book);//续借
    void bookLendRecord(Book book，Card card);//借书记录
    void bookReturnRecord(Book book，Card card);//还书记录
    void bookOrderRecord(Book book，Card card);//预定记录
    void bookRenewRecord();//续借记录
    void bookOrderCancelRecord(Book book，Card card);//取消预约记录
    void bookOrderNoRecord(Book book，Card card);//预约失效记录
    void bookOrderDelete(Book book，Card card);//预约删除记录

Private:
    Book book;
    Card card;


};






