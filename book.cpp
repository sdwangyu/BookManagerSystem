#include<iostream>
using namespace std;

int allcard;//从文件中读取 修改后重新写入文件  用户注册 ++
int allbook;//增加图书 ++

int compareDate();//匹配

class Book//构造函数 复制构造函数
{
Public:
    Book(char char BookID[20],BookName[20],char Author[20],char Publisher[20],short Storage,short BookMan,short TStorage)
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
        BookState='1';   //所有标记 0表示不存在 1表示存在
        storage=Storage;
        bookState[0]=BookState[0];
        bookMan=BookMan;
        tStorage=TStorage;
        flag='1';
    }
    char *getbookID()
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
    char *getpublisher()
    {
        return publisher;
    }
    short getstorage()
    {
        return storage;
    }
    char *getbookState()
    {
        return bookState;
    }
    short getbookMan()
    {
        return bookMan;
    }
    short tStorage()
    {
        return tStorage;
    }
    char *getflag()
    {
        return flag;
    }
//继续编写


Private:
    char bookID[20];
    char bookName[20];
    char author[20];
    char publisher[20];
    short storage; 
    short bookMan; //预约人数
    short tStorage;  //临时库存
    char flag;  //图书是否存在
};


class Card//构造函数  复制构造函数
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


class Administrator
{
Public:

    void addBook(Book book);//增加书
    //void deleteBook(Book book);老师说不要删书
    void newStorage(Book book);//新设库存
    void searchRecord();//查询记录   1.
    //void operateCard(Card card);老师说不要删卡 听老师的
    Private：
    char account[11];
    char aPassword[20];
    char accountHolder[10];
    char aID[18];
    char aPhone[11];

};



class Record
{
Public:
    Record(Book book1,Card card1)
    {
        book=book1;
        card=card2;
        //获取当前系统日期 自行查询方法 读入当前year month day
    }

    void bookLendRecord();//借书记录
    void bookReturnRecord();//还书记录
    void bookOrderRecord();//预约记录
    void bookRenewRecord();//续借记录
    void bookOrderCancelRecord();//取消预约记录
    void bookOrderNoRecord();//预约失效记录
    void signInRecord();//登陆记录
    void signOutRecord();//注销记录
    void signUpRecord();//注册记录

Private：
    char flag1;  //a借书 b还书 c预约 d续借 e取消预约 f预约失效 g预约记录  
    Book book;
    Card card;
    int year;
    int month;
    int day;
    char flag2;//用于缓冲区   1对预约记录表示它已经写入记录文件 1对续借记录表示该书已续借


};

class Library
{
Public:

    void signInUser();//用户登陆
    void signInAdmin();//管理员登陆
    void signUp();//用户注册
    void signOut();//用户注销
    void matchCid();//身份证ID匹配
    void ResetPassward();//输入新密码后重设密码写入原位置
    void update();//函数用于用户进入系统时 对缓冲区进行更新
    void charge();//充值函数
    void Rcharge();//处理用户违约金
    void resetCard();//更新修改卡信息 姓名 身份证 手机


    void Search();//查询书本函数

    
    void bookLend();//借书
    void bookReturn();//还书
    void bookOrder();//预约
    void bookorderCancel();//取消预约
    void bookRenew();//续借


Private:
    Book book;
    Card card;



};



