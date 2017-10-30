#include<iostream>
using namespace std;

int allcard;//从文件中读取 修改后重新写入文件  用户注册 ++
int allbook;//增加图书 ++

int compareDate();//匹配

class Book//构造函数 复制构造函数
{
Public:
    Book(char BookID[10],char BookName[50],char Author[20],char Publisher[20])//构造函数
    {
        
        for(int i=0; i<10; i++)
        {
            bookID[i]=BookID[i];
        }
        for(int i=0; i<50; i++)
        {
            bookName[i]=BookName[i];
        }
        for(int i=0; i<20; i++)
        {
            author[i]=Author[i];
        }
        for(int i=0; i<20; i++)
        {
            publisher[i]=Publisher[i];
        }
        storage=10;//初始库存为10本
        bookMan=0;//初始预约人数为0
        tStorage=0;//初始预约该书的人数为0
        flag='1';   //所有标记 0表示不存在 1表示存在//此处，1表示书可借
    }
    Book(Book &book){//复制构造函数
    		for(int i=0; i<10; i++)
        {
            bookID[i]=book.bookID[i];
        }
        for(int i=0; i<50; i++)
        {
            bookName[i]=bbok.bookName[i];
        }
        for(int i=0; i<20; i++)
        {
            author[i]=book.author[i];
        }
        for(int i=0; i<20; i++)
        {
            publisher[i]=book.publisher[i];
        }
        storage=book.storage;
        bookMan=book.bookMan;
        tStorage=book.tStorage;
        flag=book.flag;
    }
    char *getbookID()
    {
        return bookID;
    }
    void setbookID(char newbookID[10]){
    		for(int i=0; i<10; i++)
        {
            bookID[i]=newbookID[i];
        }
    }
    char *getbookName()
    {
        return bookName;
    }
    void setbookName(char newbookName[50]){
    		for(int i=0; i<50; i++)
        {
            bookName[i]=newbookName[i];
        }
    }
    char *getauthor()
    {
        return author;
    }
    void setauthor(char newauthor[20]){
    		for(int i=0; i<20; i++)
        {
            author[i]=newauthor[i];
        }
    }
    char *getpublisher()
    {
        return publisher;
    }
    void setpublisher(char newpublisher[20]){
    		for(int i=0; i<20; i++)
        {
            publisher[i]=newpublisher[i];
        }
    }
    short getstorage()
    {
        return storage;
    }
    void setstorage(short newstorage){
    		storage=newstorage;
    }
    short getbookMan()
    {
        return bookMan;
    }
    void setbookMan(short newbookMan){
    		bookMan=newbookMan;
    }
    short gettStorage()
    {
        return tStorage;
    }
    void settStorage(short newtStorage){
    		tStorage=newStorage;
    }
    char getflag()
    {
        return flag;
    }
    void setflag(char newflag){
    		flag=newflag;
    }
Private:
    char bookID[10];//图书编号
    char bookName[50];//书名
    char author[20];//作者
    char publisher[20];//出版商
    short storage; //库存
    short bookMan; //预约人数
    short tStorage;  //临时库存
    char flag;  //图书是否存在
};


class Card//构造函数  复制构造函数
{
Public:
    Card(char CardID[11],char CPassword[20],char CardHolder[10],double Balance,char CID[18],char CPhone[11]){//构造函数
    		for(int i=0; i<11; i++)
        {
            cardID[i]=CardID[i];
        }
        for(int i=0; i<20; i++)
        {
            cPassword[i]=CPassword[i];
        }
        lendedCount=0;//初始已借本数为0
        lendingCount=10;//初始可借本数为10
        for(int i=0; i<10; i++)
        {
            cardHolder[i]=CardHolder[i];
        }
        bookState='1';//1表示未冻结
        balance=Balance;
        ownMoney=0;
        bookedCount=0;//初始预约本数为0
        for(int i=0; i<18; i++)
        {
            cID[i]=CID[i];
        }
        for(int i=0; i<11; i++)
        {
            cPhone[i]=CPhone[i];
        }
    }
    Card(Card &card){//复制构造函数
    		for(int i=0; i<11; i++)
        {
            cardID[i]=card.cardID[i];
        }
        for(int i=0; i<20; i++)
        {
            cPassword[i]=card.cPassword[i];
        }
        lendedCount=card.lendedCount;
        lendingCount=card.lendingCount;
        for(int i=0; i<10; i++)
        {
            cardHolder[i]=card.cardHolder[i];
        }
        bookState=card.bookState;
        balance=card.balance;
        ownMoney=card.ownMoney;
        bookedCount=card.bookedCount;
        for(int i=0; i<18; i++)
        {
            cID[i]=card.cID[i];
        }
        for(int i=0; i<11; i++)
        {
            cPhone[i]=card.cPhone[i];
        }
    }
   	char *getcardID(){
   			return cardID;
   	}
   	void setcardID(char newcardID[11]){
   			for(int i=0; i<11; i++)
        {
            cardID[i]=newcardID[i];
        }
   	}
   	char *getcPassword(){
   			return cPassword;
   	}
   	void setcPassword(char newcPassword[20]){
   			for(int i=0; i<20; i++)
        {
            cPassword[i]=newcPassword[i];
        }
   	}
   	short getlendedCount(){
   			return lendedCount;
   	}
   	void setlendedCount(short newlendedCount){
   			lendedCount=newlendedCount;
   	}
   	short getlendingCount(){
   			return lendingCount;
   	}
   	void setlendingCount(short newlendingCount){
   			lendingCount=newlendingCount;
   	}
   	char *getcardHolder(){
   			return cardHolder;
   	}
   	void setcardHolder(char newcardHolder[10]){
   			for(int i=0; i<10; i++)
        {
            cardHolder[i]=newcardHolder[i];
        }
   	}
   	char getcardState(){
   			return cardState;
   	}
   	void setcardState(char newcardState){
   			cardState=newcardState;
   	}
   	double getbalance(){
   			return balance;
   	}
   	void setbalance(double newbalance){
   			balance=newbalance;
   	}
   	double getownMoney(){
   			return ownMoney;
   	}
   	void setownMoney(double newownMoney){
   			ownMoney=newownMoney;
   	}
   	short getbookedCount(){
   			return bookedCount;
   	}
   	void setbookedCount(short newbookedCount){
   			bookedCount=newbookedCount;
   	}
   	char *getcID(){
   			return cID;
   	}
   	void setcID(char newcID[18]){
   			for(int i=0; i<18; i++)
        {
            cID[i]=newcID[i];
        }
   	}
   	char *getcPhone(){
   			return cPhone;
   	}
   	void setcPhone(char newcPhone[11]){
   			for(int i=0; i<11; i++)
        {
            cPhone[i]=newcPhone[i];
        }
   	}
    Private：
    char cardID[11];//卡号
    char cPassword[20];//密码
    short lendedCount;//已借本数
    short lendingCount;//可借本数
    char cardHolder[10];//持卡人姓名
    char cardState;   //账号冻结状态
    double balance;//余额
    double oweMoney;   //违约金
    short bookedCount;//预约本数（最多可预约本数）  已借+可借+预约=10 提示不可借不可预约
    char cID[18];  //身份证号
    char cPhone[11];//持卡人手机号
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

void Library::signInUser(){

}

void Library::signInAdmin(){

}

void Library::signUp(){

}

void Library::signOut(){

}

void Library::matchCid(){

}

void Library::ResetPassword(){

}

void Library::update(){

}

void Library::charge(){

}

void Library::Rcharge(){

}

void Library::resetCard(){

}

