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
    short gettStorage()
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
    short lendedCount;//已借本数  已借+可借=10
    short lendingCount;//可借本数
    char cardHolder[10];
    char cardState[1];   //账号冻结状态
    double balance;
    double oweMoney;   //违约金
    short bookedCount;//预约本数5    提示不可借不可预约
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
    void (Book book1,Card card1) {
        book = book1;
        card = card1;
    }
    void (Card card1) {
        card = card1;
    }
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
    void bookOrderCancel();//取消预约
    void bookRenew();//续借
Private:
    Book book;
    Card card;
};

void Library::bookLend() { //借书 1.直接借书  2.通过预约借书？？？
        Record record(book,card);
        if(card.getlendedCount()==10) {//可借本数超过上限
            cout<<"可借本书已达到上限，无法再进行借阅！"<<end;
        }
        else{//可借本数没有超过上限
            if(book.getstorage()>=2) {
                cout<<"借阅成功"<<endl;
                book.setstorage(book.getstorage()-1);//库存-1
                card.setlendedCount(card.getlendedCount()+1);//已借本数+1
                card.setlendingCount(card.getlendingCount()-1);//可借本数-1
                record.bookLendRecord(); //生成一条借书的记录
                //写回book文件
                //写回card文件
            }
            else {
                int choice;
                cout<<"库存不够，借阅失败！"<<endl;//借阅失败
                cout<<"是否进行预约？"<<endl;
                cout<<"1.是   2.否"<<endl;//提示框①
                cin>>choice;
                while(1) {
                    if(choice==1) {
                        bookOrder();
                        break;
                    }
                    else if(choice==2) {
                    //关闭提示框①返回查询界面
                        break;
                    }
                    else {
                        cout<<"输入有误，请重新输入！"<<endl;
                        cout<<"是否进行预约？"<<endl;
                        cout<<"1.是   2.否"<<endl;//提示框②
                        cin>>choice;
                        //关闭提示框②
                    }
                }
            }
        }
}

void Library::bookReturn(){ //还书
    Record record(book,card);
    card.setlendedCount(book.getlendedCount()-1);//已借本数-1
    card.setlendingCount(card.getlendingCount()+1);//可借本数+1
    //写回card文件

    //1.检测这本书是否有人预约
    if(book.getbookMan()>0) {
        //1.1若有人预约
        if(book.gettStorage()==book.getbookMan()) {//检测临时库存是否等于预约人数，若等于则库存+1
            book.setstorage(book.getstorage()+1);
            //写回book文件
        }
        else {//若不等于，临时库存+1
            book.settStorage(book.gettStorage()+1);
            //写回book文件
        }
    }
    else {
        //1.2若无人预约，库存+1
        book.setstorage(book.getstorage()+1);
        //写回book文件
    }
    record.bookReturnRecord();//生成一条还书记录
}

void Library::bookOrder(){//预约
    Record record(book,card);
    if(card.getbookMan()==5) {//预约本数已达上限
        cout<<"您的预约本数已达上限，无法进行预约！"<<endl;
    }
    else{
        cout<<"预约成功！"<<endl;//提示预约成功
        book.setbookMan(book.getbookMan()+1);//书的预约人数+1
        card.setbookedCount(card.getbookedCount()+1);//人的预约本数+1
        //写回book文件
        //写回card文件
        record.bookOrderRecord();//生成一条预约记录
    }

}

void Library::bookOrderCancel(){//取消预约 1.未到期取消预约 2.过期取消预约？？？
    Record record(book,card);
    int choice;
    cout<<"确定取消预约吗？"<<endl;
    cout<<"1.是 2.否"<<endl;
    cin>>choice;
    while(1) {
        if(choice==1) {//1.
            cout<<"成功取消预约！"<<endl;
            book.setbookMan(book.getbookMan()-1);//此书的预约人数-1
            card.setbookedCount(card.getbookedCount()-1);//此人的预约数量-1
            record.bookOrderCancelRecord();//生成一条取消预约的记录
            //写回book文件
            //写回card文件
            break;
        }
        else if(choice==2) {//2.否
            //返回借阅信息界面
            break;
        }
        else {//输入有误
            cout<<"输入有误，请重新输入！"<<endl;
            cout<<"确定取消预约吗？"<<endl;
            cout<<"1.是   2.否"<<endl;
            cin>>choice;
        }
    }
}

void Library::bookRenew(){//图书续借
    Record record(book,card);
    cout<<"续借成功"<<endl;
    record.bookRenewRecord();//生成一条续借记录
}


