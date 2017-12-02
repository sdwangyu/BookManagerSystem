#include<iostream>
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<time.h>
#include<vector>
#include<algorithm>
#include<iomanip>
#include<sstream>
#include <fstream>

#define ALLNUM allNum//存放allcard和allbook
#define BOOKINFORMATION bookInformation//全部图书信息
#define CARDINFORMATION cardInformation//全部用户信息
#define ADMININFORMATION adminInformation//全部管理员信息
#define BOOK_LEND_RECORD bookLendRecord//10.30 Record借书记录对应文件
#define BOOK_RENEW_RECORD bookRenewRecord//10.30 .Record续借记录对应文件
#define BOOK_ORDER_RECORD bookOrderRecord//预约记录
#define BOOK_ORDER_CANCEL_RECORD bookOrderCancelRecord//10.30 取消预约预约到期记录对应文件
#define SIGN_IN_RECORD signInRecord//10.30 Record登陆记录对应文件
#define SIGN_UP_RECORD signUpRecord//注册记录文件
#define SIGN_OUT_RECORD signOutRecord//注销记录文件
#define BUFFERZONE_ORDER bufferOrderZone//预约缓冲区文件
#define BUFFERZONE_LEND bufferLendZone//借书缓冲区文件
#define SEARCHRESULT searchResult//查询结果文件
#define SEARCHRESULTTEMP searchResultTmp//查询结果临时文件
#define LOG log;//图书馆大日志
#define ADMININ_ADD_BOOK admininAddBook //11.7管理员增加书记录
#define ADMININ_CHANGE_STORAGE admininChangeStorage//11.7管理员改变库存记录文件
#define BOOK_RETURN_RECORD bookReturnRecord //11.1还书记录对应文件
//#define BUFFER_NEW bufferNew //缓存区临时文件


using namespace std;

int allcard;//从文件中读取 修改后重新写入文件  用户注册 ++
int allbook;//增加图书 ++
int alladmin;//全部管理员，新的管理员增加时++

fstream iofile;//文件流

struct so_1
{
    int a;//记录长度or相似度
    int b;//记录数组下标
};
struct so_2
{
    double a;
    int b;
};

bool comp_1(const so_1 &x, const so_1 &y)
{
    return x.a < y.a;
}
bool comp_2(const so_2 &x, const so_2 &y)
{
    return x.a > y.a;
}

int isLeapYear(int year) //判断是否是闰年,返回1为闰年，返回0不是闰年
{
    if (year % 4 == 0 && year % 100 != 0)return 1;
    if (year % 400 == 0)return 1;
    return 0;
}

int getDayInYear(int year, int month, int day) //得到该日期是一年的第几天
{
    int months[12] = { 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 };
    if (isLeapYear(year))months[1] = 29;
    for (int i = 0; i<month - 1; i++)
    {
        day = day + months[i];
    }
    return day;
}

int compareDate(int year1, int month1, int day1, int year2, int month2, int day2) //日期比较,日期1year1month1day1-日期2year2month2day2,
{
    int days = 0;
    //不需要判断日期数字是否合理，系统日期不会出现32天的情况
    if (year1 == year2) //年份相等
    {
        if (month1 == month2)days = day1 - day2;//月份相等
        else days = getDayInYear(year1, month1, day1) - getDayInYear(year2, month2, day2);//月份不等
    }
    else  //年份不同，则日期差值为日期1的在year1的第几天+日期2在year2还剩下的天数+year1和year2两年之间的天数差值
    {
        //确保year1年份比year2晚
        if (year1 > year2)
        {
            if (isLeapYear(year2))
                days = 366 - getDayInYear(year2, month2, day2); //取得日期2在该年还剩下多少天
            else
                days = 365 - getDayInYear(year2, month2, day2);
            days += getDayInYear(year1, month1, day1); //取得日期1在当年中的第几天
            for (int year = year2 + 1; year < year1; year++)//取得year1和year2之间的天数差值
            {
                if (isLeapYear(year))
                    days += 366;
                else
                    days += 365;
            }
        }
        else days = -compareDate(year2, month2, day2, year1, month1, day1);    //date1小于date2的情况
    }
    // if(days<0)exit(0);//出错则退出程序
    return days;
}
class Book//构造函数 复制构造函数
{
public:
    friend class Administrator;
    friend class Record;
    Book(char BookID[10], char BookName[100], char Author[20], char Publisher[20], short Storage)//构造函数
    {

        for (int i = 0; i<10; i++)
        {
            bookID[i] = BookID[i];
        }
        for (int i = 0; i<100; i++)
        {
            bookName[i] = BookName[i];
        }
        for (int i = 0; i<20; i++)
        {
            author[i] = Author[i];
        }
        for (int i = 0; i<20; i++)
        {
            publisher[i] = Publisher[i];
        }
        memset(books, '3', sizeof(books));//把books全部初始化为3
        books[20] = '\0';
        if(Storage < 21)
        {
            storage = Storage;//初始库存为10本
            for(int i = 0; i < Storage; i++)
            {
                books[i] = '1';  //把前storage本书置为1 表示可借
            }
        }
        else printf("Error,Store should <21");
        bookMan = 0;//初始预约人数为0
        tStorage = 0;//初始预约该书的人数为0
        flag = '1';   //所有标记 0表示不存在 1表示存在//此处，1表示书可借
        //books[10] = { 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 };
        //books = (int*)malloc(storage*sizeof(int));        //分配给该类书对应库存量的空间大小    在管理员修改库存的时候应该重新分配内存
    }
    Book()
    {
        for (int i = 0; i<10; i++)
        {
            bookID[i] = ' ';
        }
        for (int i = 0; i<100; i++)
        {
            bookName[i] = ' ';
        }
        for (int i = 0; i<20; i++)
        {
            author[i] = ' ';
        }
        for (int i = 0; i<20; i++)
        {
            publisher[i] = ' ';
        }
        memset(books, '3', sizeof(books));//把books全部初始化为3
        books[20] = '\0';
        storage = 0;//初始库存为10本
        bookMan = 0;//初始预约人数为0
        tStorage = 0;//初始预约该书的人数为0
        flag = '1';   //所有标记 0表示不存在 1表示存在//此处，1表示书可借
    }

    Book(Book &book) //复制构造函数
    {
        for (int i = 0; i<10; i++)
        {
            bookID[i] = book.bookID[i];
        }
        for (int i = 0; i<100; i++)
        {
            bookName[i] = book.bookName[i];
        }
        for (int i = 0; i<20; i++)
        {
            author[i] = book.author[i];
        }
        for (int i = 0; i<20; i++)
        {
            publisher[i] = book.publisher[i];
        }
        storage = book.storage;
        bookMan = book.bookMan;
        tStorage = book.tStorage;
        flag = book.flag;
        for (int i = 0; i < 21; i++)
        {
            books[i] = book.books[i];
        }
    }
    char *getbookID()
    {
        return bookID;
    }
    void setbookID(char newbookID[10])
    {
        for (int i = 0; i<10; i++)
        {
            bookID[i] = newbookID[i];
        }
    }
    char *getbookName()
    {
        return bookName;
    }
    void setbookName(char newbookName[100])
    {
        for (int i = 0; i<100; i++)
        {
            bookName[i] = newbookName[i];
        }
    }
    char *getauthor()
    {
        return author;
    }
    void setauthor(char newauthor[20])
    {
        for (int i = 0; i<20; i++)
        {
            author[i] = newauthor[i];
        }
    }
    char *getpublisher()
    {
        return publisher;
    }
    void setpublisher(char newpublisher[20])
    {
        for (int i = 0; i<20; i++)
        {
            publisher[i] = newpublisher[i];
        }
    }
    short getstorage()
    {
        return storage;
    }
    void setstorage(short newstorage)
    {
        storage = newstorage;
    }
    void addstorage(short newstorage)
    {
        int i=0;
        while(books[i] != '3') i++; //i为目前的库存
        if(i + newstorage >20) //如果增加库存后超过20 提示越界 拒绝修改
            cout << "Out of range, should < " << 20 - i << endl;
        else
        {
            storage += newstorage; //新库存
            while(newstorage != 0)
            {
                books[i + newstorage - 1] = '1';
                newstorage--;
            }
        }

    }
    short getbookMan()
    {
        return bookMan;
    }
    void setbookMan(short newbookMan)
    {
        bookMan = newbookMan;
    }
    short gettStorage()
    {
        return tStorage;
    }
    void settStorage(short newStorage)
    {
        tStorage = newStorage;
    }
    char getflag()
    {
        return flag;
    }
    void setflag(char newflag)
    {
        flag = newflag;
    }
    char *getBooks()
    {
        return books;
    }
    //11.10 新增修改Books[i]的函数
    void setBooksI(int i, char newbooksi) //i表示第i本书，newbooksi表示新的Books[i]的值
    {
        books[i] = newbooksi;
    }

private:
    char bookID[10];//图书编号
    char bookName[100];//书名
    char author[20];//作者
    char publisher[20];//出版商
    short storage; //库存
    short bookMan; //预约人数
    short tStorage;  //临时库存
    char flag;  //图书是否存在
    char books[21]; //数组中每一项用来表示具体某一本的状态，0：损坏 1：可借 2：借出 3.表示初始化值，这本书还不可以用
    //动态开辟存储空间?
    //书籍库存上限为20
friend class Administrator;
};


class Card//构造函数  复制构造函数
{
public:
    friend class Record;
    Card(char CardID[10], char CPassword[20], char CardHolder[10], double Balance, char CID[19], char CPhone[12]) //构造函数
    {
        for (int i = 0; i<10; i++)
        {
            cardID[i] = CardID[i];
        }

        for (int i = 0; i<20; i++)
        {
            cPassword[i] = CPassword[i];
        }

        for (int i = 0; i<10; i++)
        {
            cardHolder[i] = CardHolder[i];
        }
        lendedCount = 0;//初始已借本数为0
        lendingCount = 10;//初始可借本数为10
        cardState = '1';//1表示未冻结
        balance = Balance;
        oweMoney = 0;
        bookedCount = 0;//初始预约本数为0
        for (int i = 0; i<19; i++)
        {
            cID[i] = CID[i];
        }
        for (int i = 0; i<12; i++)
        {
            cPhone[i] = CPhone[i];
        }
    }
    Card(Card &card) //复制构造函数
    {
        for (int i = 0; i<10; i++)
        {
            cardID[i] = card.cardID[i];
        }
        for (int i = 0; i<20; i++)
        {
            cPassword[i] = card.cPassword[i];
        }
        lendedCount = card.lendedCount;
        lendingCount = card.lendingCount;
        for (int i = 0; i<10; i++)
        {
            cardHolder[i] = card.cardHolder[i];
        }
        cardState = card.cardState;
        balance = card.balance;
        oweMoney = card.oweMoney;
        bookedCount = card.bookedCount;
        for (int i = 0; i<19; i++)
        {
            cID[i] = card.cID[i];
        }
        for (int i = 0; i<12; i++)
        {
            cPhone[i] = card.cPhone[i];
        }
    }
    Card()
    {
        int i = 0;
        for (i = 0; i<10; i++)
        {
            cardID[i] = ' ';
        }
        for (i = 0; i<20; i++)
        {
            cPassword[i] = ' ';
        }
        for (i = 0; i<10; i++)
        {
            cardHolder[i] = ' ';
        }
        balance = 0;
        for (i = 0; i<19; i++)
        {
            cID[i] = ' ';
        }
        for (i = 0; i<12; i++)
        {
            cPhone[i] = ' ';
        }
        lendedCount = 0;//初始已借本数为0
        lendingCount = 10;//初始可借本数为10
        cardState = '1';//1表示未冻结
        oweMoney = 0;
        bookedCount = 0;//初始预约本数为0
    }

    char *getcardID()
    {
        return cardID;
    }
    void setcardID(char newcardID[10])
    {
        for (int i = 0; i<10; i++)
        {
            cardID[i] = newcardID[i];
        }
    }
    char *getcPassword()
    {
        return cPassword;
    }
    void setcPassword(char newcPassword[20])
    {
        for (int i = 0; i<20; i++)
        {
            cPassword[i] = newcPassword[i];
        }
    }
    short getlendedCount()
    {
        return lendedCount;
    }
    void setlendedCount(short newlendedCount)
    {
        lendedCount = newlendedCount;
    }
    short getlendingCount()
    {
        return lendingCount;
    }
    void setlendingCount(short newlendingCount)
    {
        lendingCount = newlendingCount;
    }
    char *getcardHolder()
    {
        return cardHolder;
    }
    void setcardHolder(char newcardHolder[10])
    {
        for (int i = 0; i<10; i++)
        {
            cardHolder[i] = newcardHolder[i];
        }
    }
    char getcardState()
    {
        return cardState;
    }
    void setcardState(char newcardState)
    {
        cardState = newcardState;
    }
    double getbalance()
    {
        return balance;
    }
    void setbalance(double newbalance)
    {
        balance = newbalance;
    }
    double getoweMoney()
    {
        return oweMoney;
    }
    void setoweMoney(double newoweMoney)
    {
        oweMoney = newoweMoney;
    }
    short getbookedCount()
    {
        return bookedCount;
    }
    void setbookedCount(short newbookedCount)
    {
        bookedCount = newbookedCount;
    }
    char *getcID()
    {
        return cID;
    }
    void setcID(char newcID[19])
    {
        for (int i = 0; i<19; i++)
        {
            cID[i] = newcID[i];
        }
    }
    char *getcPhone()
    {
        return cPhone;
    }
    void setcPhone(char newcPhone[12])
    {
        for (int i = 0; i<12; i++)
        {
            cPhone[i] = newcPhone[i];
        }
    }
private:

    char cardID[10];//卡号
    char cPassword[20];//密码
    short lendedCount;//已借本数（不能超过10）
    short lendingCount;//可借本数   （10-已借本数）
    char cardHolder[10];//持卡人姓名
    char cardState;   //账号冻结状态
    double balance;//余额
    double oweMoney;   //违约金
    short bookedCount;//已经预约的本数
    char cID[19];  //身份证号
    char cPhone[12];//持卡人手机号
};

class Record
{
public:
    //10.30 构造函数更改
    Record(char*bookid1, char*cardid1, int Year, int Month, int Day, char flag11, char flag22)
    {

        for (int i = 0; i < 10; i++)
        {
            bookid[i] = bookid1[i];
        }
        for (int i = 0; i < 10; i++)
        {
            cardid[i] = cardid1[i];
        }
        year = Year;
        month = Month;
        day = Day;
        flag1 = flag11;
        flag2 = flag22;
        //获取当前系统日期 自行查询方法 读入当前year month day
    }

    Record(char*cardid1, int Year, int Month, int Day, int flag11)
    {
        for (int i = 0; i < 10; i++)
        {
            cardid[i] = cardid1[i];
        }
        year = Year;
        month = Month;
        day = Day;
        flag1 = flag11;
    }

    //刘峰同学需要的构造函数啦啦~~
    Record(char*bookid1, char*cardid1, int Year, int Month, int Day, char flag11, char flag22, int Order)
    {
        for (int i = 0; i < 10; i++)
        {
            bookid[i] = bookid1[i];
        }
        for (int i = 0; i < 10; i++)
        {
            cardid[i] = cardid1[i];
        }
        year = Year;
        month = Month;
        day = Day;
        flag1 = flag11;
        flag2 = flag22;
        order = Order;
    }

    //默认构造函数
    Record()
    {
        for (int i = 0; i < 10; i++)
        {
            bookid[i] = ' ';
        }
        for (int i = 0; i < 10; i++)
        {
            cardid[i] = ' ';
        }
        year = 0;
        month = 0;
        day = 0;
        flag2 = 1;//用于缓冲区   1对预约记录表示此预约失效并且已经写入记录文件 1对续借记录表示该书已续借
        order = 1;
        flag1 = '1';   //所有标记 0表示不存在 1表示存在//此处，1表示书可借
    }
    //复制构造函数
    Record(Record &R)
    {
        for (int i = 0; i<10; i++)
        {
            bookid[i] = R.bookid[i];
        }
        for (int i = 0; i<10; i++)
        {
            cardid[i] = R.cardid[i];
        }
        year = R.year;
        month = R.month;
        day = R.day;
        flag2 = R.flag2;//用于缓冲区   1对预约记录表示此预约失效并且已经写入记录文件 1对续借记录表示该书已续借
        order = R.order;
        flag1 = R.flag1;
    }

    void bookLendRecord(int flag);//借书记录
    void bookReturnRecord();//还书记录
    void bookOrderRecord();//预约记录
    void bookRenewRecord();//续借记录
    void bookOrderCancelRecord();//取消预约记录
    void bookOrderNoRecord_new();//失效情况下取消预约记录，专门用于update_order函数里
    void bookOrderNoRecord();//预约失效记录
    void signInRecord();//登陆记录
    void signOutRecord();//注销记录
    void signUpRecord();//注册记录
    void admininchangestorage();//管理员改变库存记录
    void admininaddbook();//管理员增加新书记录
    void alter_Date(int addday);//增加一个日期变化的函数

    char getflag1()
    {
        return flag1;
    }
    void setflag1(char newflag1)
    {
        flag1 = newflag1;
    }
    int getyear()
    {
        return year;
    }
    void setyear(int newyear)
    {
        year = newyear;
    }
    int getmonth()
    {
        return month;
    }
    void setmonth(int newmonth)
    {
        month = newmonth;
    }
    int getday()
    {
        return day;
    }
    void setday(int newday)
    {
        day = newday;
    }
    char getflag2()
    {
        return flag2;
    }
    void setflag2(char newflag2)
    {
        flag2 = newflag2;
    }
    int getorder()
    {
        return order;
    }
    void setorder(int neworder)
    {
        order = neworder;
    }
    char *getBookid()
    {
        return bookid;
    }
    char *getCardid()
    {
        return cardid;
    }
private:
    char flag1;  //a借书 b还书 c预约 d续借 e取消预约 f预约失效 g注册记录 h注销记录 i登陆记录 j管理员增加书 k管理员更改库存 l管理员注册
    char bookid[10];
    char cardid[10];
    int year;
    int month;
    int day;
    char flag2;//用于缓冲区   1对预约记录表示此预约失效并且已经写入记录文件 1对续借记录表示该书已续借
    int order;//标识第几本书
};

class Administrator
{
public:
    //11.1构造函数
    Administrator(char Account[5], char APassword[20], char AccountHolder[10], char AID[19], char APhone[12])//构造函数
    {

        for (int i = 0; i<5; i++)
        {
            account[i] = Account[i];
        }
        for (int i = 0; i<20; i++)
        {
            aPassword[i] = APassword[i];
        }
        for (int i = 0; i<10; i++)
        {
            accountHolder[i] = AccountHolder[i];
        }
        for (int i = 0; i<19; i++)
        {
            aID[i] = AID[i];
        }
        for (int i = 0; i < 12; i++)
        {
            aPhone[i] = APhone[i];
        }
        aID[18]='\0';
        aPhone[11]='\0';
    }
    Administrator()
    {
        for (int i = 0; i<5; i++)
        {
            account[i] = ' ';
        }
        for (int i = 0; i<20; i++)
        {
            aPassword[i] = ' ';
        }
        for (int i = 0; i<10; i++)
        {
            accountHolder[i] = ' ';
        }
        for (int i = 0; i<19; i++)
        {
            aID[i] = ' ';
        }
        for (int i = 0; i<12; i++)
        {
            aPhone[i] = ' ';
        }
        aID[18]='\0';
        aPhone[11]='\0';
    }

    //复制构造函数
    Administrator(Administrator &administrator)
    {
        for (int i = 0; i<5; i++)
        {
            account[i] = administrator.account[i];
        }
        for (int i = 0; i<20; i++)
        {
            aPassword[i] = administrator.aPassword[i];
        }
        for (int i = 0; i<10; i++)
        {
            accountHolder[i] = administrator.accountHolder[i];
        }
        for (int i = 0; i<19; i++)
        {
            aID[i] = administrator.aID[i];
        }
        for (int i = 0; i<12; i++)
        {
            aPhone[i] = administrator.aPhone[i];
        }
    }

    void setaccount(char newaccount[5])
    {
        for (int i = 0; i<5; i++)
        {
            account[i] = newaccount[i];
        }
    }

    void setaPassword(char newaPassword[20])
    {
        for (int i = 0; i<20; i++)
        {
            aPassword[i] = newaPassword[i];
        }
    }

    void setaccountHolder(char newaccountHolder[10])
    {
        for (int i = 0; i<10; i++)
        {
            accountHolder[i] = newaccountHolder[i];
        }
    }

    void setaID(char newaID[19])
    {
        for (int i = 0; i<19; i++)
        {
            aID[i] = newaID[i];
        }
    }
    void setaPhone(char newaaPhone[12])
    {
        for (int i = 0; i<12; i++)
        {
            aPhone[i] = newaaPhone[i];
        }
    }
    char *getaccount()
    {
        return account;
    }
    char *getaPassword()
    {
        return aPassword;

    }
    char *getaccountHolder()
    {
        return accountHolder;
    }
    char *getaID()
    {
        return aID;
    }
    char *getaPhone()
    {
        return aPhone;
    }

    void addBook(char *bookID, char *bookName, char *author, char *publisher, short storage);//增加书

    //void deleteBook(Book book);老师说不要删书
    //void operateCard(Card card);老师说不要删卡 听老师的

    //void findbook(char*bookid);        //为了在修改库存之前给类中的私有成员book赋值
    void newStorage(short storage);//新设库存
    void addadmin(char*aPassword, char*accountHolder, char*aID, char*aPhone);

    //11.9管理员所有查看行为函数
    void searchLog();//管理员查看大日志
    void searchLendBuffer();//管理员查看现在正借出的书
    void serchBookInformation();//管理员查看全部图书信息
    void searchCardInformation();//管理员查看全部用户信息
    void searchOrder();//管理员查看所有预约记录
    void searchCancleOrder();//管理员查看全部预约取消预约到期记录文件
    void searchReturn();//管理员查看所有还书信息


private:
    char account[5];        //记得修改上面对应得数值
    char aPassword[20];
    char accountHolder[10];
    char aID[19];
    char aPhone[12];
};

//管理员注册函数

void Administrator::addadmin(char*aPassword, char*accountHolder, char*aID, char*aPhone)
{
    string account_str = to_string(2000 + alladmin + 1);
    char account[5];
    strcpy(account,account_str.c_str());
    Administrator newadministrator(account, aPassword, accountHolder, aID, aPhone);
    FILE*fp_admin;
    if (NULL == (fp_admin = fopen("ADMININFORMATION", "rb+")))
    {
        fprintf(stderr, "Can not open file");
        exit(1);
    }
    fseek(fp_admin, 0, SEEK_END);
    if (fwrite(&newadministrator, sizeof(Administrator), 1, fp_admin) != 1)
        printf("file write error\n");
    time_t timer;
    time(&timer);
    tm* t_tm = localtime(&timer);    //获取了当前时间，并且转换为int类型的year，month，day
    int year = t_tm->tm_year + 1900;
    int month = month = t_tm->tm_mon + 1;
    int day = t_tm->tm_mday;
    Record record(newadministrator.getaccount(), year, month, day, 'l');
    record.signUpRecord();
    fclose(fp_admin);
    alladmin++;
	FILE *fp_num;
	if (NULL == (fp_num = fopen("ALLNUM", "rb+")))
	{
		fprintf(stderr, "Can not open file");
		exit(1);
	}
	if (fwrite(&allcard, sizeof(int), 1, fp_num) != 1)            //覆盖写入?
		printf("file write error\n");
	if (fwrite(&allbook, sizeof(int), 1, fp_num) != 1)
		printf("file write error\n");
	if (fwrite(&alladmin, sizeof(int), 1, fp_num) != 1)
		printf("file write error\n");
	fclose(fp_num);
    return;
}
//管理员查看大日志
void Administrator::searchLog()
{
    FILE *fp_log;
    if (NULL == (fp_log = fopen("LOG", "rb+")))
    {
        fprintf(stderr, "Can not open file");
        exit(1);
    }

}

//11.2管理员新加书函数
void Administrator::addBook(char bookID[10], char bookName[50], char author[20], char publisher[20], short storage)
{
    //
    FILE *fp_add_book;
    FILE *fp_book;
    if (NULL == (fp_add_book = fopen("ADMINI_ADD_BOOK", "rb+")))
    {
        fprintf(stderr, "Can not open file");
        exit(1);
    }
    if (NULL == (fp_book = fopen("BOOKINFORMATION", "rb+")))
    {
        fprintf(stderr, "Can not open file");
        exit(1);
    }
    fseek(fp_add_book, 0, SEEK_END);
    char bookID1[10];
    char bookName1[100];
    char author1[20];
    char publisher1[20];
    short storage1;
	int bookid_1 = allbook + 100000000 + 1;
    sprintf(bookID1, "%d", bookid_1);
    cout << "请输入书名：";
    cin >> bookName1;
    cout << "请输入作者：";
    cin >> author1;
    cout << "请输入出版社：" ;
    cin >> publisher1;
    cout << "请输入库存：" ;
    cin >> storage1;
    Book book(bookID1, bookName1, author1, publisher1, storage1);
	fseek(fp_book, 0, SEEK_END);
    fwrite(&book, sizeof(Book), 1, fp_book);
    allbook++;
    time_t timer;
    time(&timer);
    tm* t_tm = localtime(&timer);    //获取了当前时间，并且转换为int类型的year，month，day
    int year = t_tm->tm_year + 1900;
    int month = month = t_tm->tm_mon + 1;
    int day = t_tm->tm_mday;
    Record record(book.getbookID(), this->getaccount(), year, month, day, 'j', '0');		//这个this是可以用的，因为调用的时候指向的也是一个具体的Administrator对象
    record.admininaddbook();
    fclose(fp_add_book);
    fclose(fp_book);
	FILE *fp_num;
	if (NULL == (fp_num = fopen("ALLNUM", "rb+")))
	{
		fprintf(stderr, "Can not open file");
		exit(1);
	}
	if (fwrite(&allcard, sizeof(int), 1, fp_num) != 1)            //覆盖写入?
		printf("file write error\n");
	if (fwrite(&allbook, sizeof(int), 1, fp_num) != 1)
		printf("file write error\n");
	if (fwrite(&alladmin, sizeof(int), 1, fp_num) != 1)
		printf("file write error\n");
	fclose(fp_num);
}

//在管理员修改库存之前，先让他输入要修改的书的bookid，然后调用findbook函数找到这本书，赋值给record类的私有成员book
//然后让他输入修改后的书的总本数，点击修改库存按钮，执行newStorage函数即可
/*void Administrator::findbook(char*bookid)
{
    FILE *fp_book;
    if (NULL == (fp_book = fopen("BOOKINFORMATION", "rb+")))
    {
        fprintf(stderr, "Can not open file");
        exit(1);
    }
    Book booktemp;
    while (!feof(fp_book))
    {
        fread(&booktemp, sizeof(Book), 1, fp_book);
        if ((string)booktemp.getbookID() == (string)bookid)
        {
            book=booktemp;
            break;
        }
    }
}*/
//11.2管理员改库存函数
void Administrator::newStorage(short addstor) //addstor是要增加的库存数目
{
    //修改库存时输入的值应该改成增加的量或者减少的量，不能直接输入最终的库存量
    time_t timer;
    time(&timer);
    tm* t_tm = localtime(&timer);    //获取了当前时间，并且转换为int类型的year，month，day
    int year = t_tm->tm_year + 1900;
    int month = month = t_tm->tm_mon + 1;
    int day = t_tm->tm_mday;
    Book book;
    char bookid[10];  //qt中向此处传入bookid表示要修改哪本书的库存
    cout << "请输入要修改书籍的id（9位）：" ;
    cin >> bookid;
    FILE *fp_book;
    if (NULL == (fp_book = fopen("BOOKINFORMATION", "rb+")))
    {
        fprintf(stderr, "Can not open file");
        exit(1);
    }
    int position = atoi(bookid) - 100000000 - 1;//书籍位置
    fseek(fp_book, position * sizeof(Book), SEEK_SET);//定位到这本书
    fread(&book, sizeof(Book), 1, fp_book);///取出这本书
    book.addstorage(addstor);//增加这本书的库存 addstor是要增加的数目
    Record record(book.getbookID(), this->getaccount(), year, month, day, 'k', '0');//写入记录	//这个this也是可以用的，理由同上
    record.admininchangestorage();
    fseek(fp_book, position * sizeof(Book), SEEK_SET);//重新定位
    if (fwrite(&book, sizeof(Book), 1, fp_book) != 1)//把修改完的book写回文件
        printf("file write error\n");
}



//Record类内部函数的实现

//11.04增加一个日期变化的函数
void Record::alter_Date(int addday)
{
    int tempday = 0;
    int monthday[12] = { 31, 0, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 };//定义出每个月的天数以方便增加月
    if (isLeapYear(year))monthday[1] = 29;//闰年的2月
    else monthday[1] = 28;//非润年的2月
    tempday = day + addday;//通过减天数，加月数加年数的方法来获取新的日期
    while (tempday>monthday[month - 1])
    {
        tempday = tempday - monthday[month - 1];
        month++;
        if (month > 12)
        {
            month = month - 12;
            if (isLeapYear(++year))monthday[1] = 29;
            else monthday[1] = 28;
        }
    }
    day = tempday;
}

//11.7增加管理员改变书库存记录
void Record::admininchangestorage()
{
    FILE *fp_change_storage;
    FILE *fp_log;
    if (NULL == (fp_change_storage = fopen("ADMININ_CHANGE_STORAGE", "rb+")))
    {
        fprintf(stderr, "Can not open file");
        exit(1);
    }
    if (NULL == (fp_log = fopen("LOG", "rb+")))
    {
        fprintf(stderr, "Can not open file");
        exit(1);
    }
    fseek(fp_change_storage, 0, SEEK_END);
    fseek(fp_log, 0, SEEK_END);
    if (fwrite(this, sizeof(Record), 1, fp_change_storage) != 1)	//这两处的this也是可以用的，指向的是一个Record对象
        printf("file write error\n");
    if (fwrite(this, sizeof(Record), 1, fp_log) != 1)
        printf("file write error\n");
    fclose(fp_change_storage);
    fclose(fp_log);
}

//11.7增加管理员新增书目的记录
void Record::admininaddbook()
{
    FILE *fp_add_book;
    FILE *fp_log;
    if (NULL == (fp_add_book = fopen("ADMININ_ADD_BOOK", "rb+")))
    {
        fprintf(stderr, "Can not open file");
        exit(1);
    }
    if (NULL == (fp_log = fopen("LOG", "rb+")))
    {
        fprintf(stderr, "Can not open file");
        exit(1);
    }
    fseek(fp_add_book, 0, SEEK_END);
    fseek(fp_log, 0, SEEK_END);
    if (fwrite(this, sizeof(Record), 1, fp_add_book) != 1)	//这两处也是可以用的，理由同上
        printf("file write error\n");
    if (fwrite(this, sizeof(Record), 1, fp_log) != 1)
        printf("file write error\n");
    fclose(fp_add_book);
    fclose(fp_log);
}

//10.31借书记录

void Record::bookLendRecord(int flag)        //借书记录
{
    FILE *fp_book_lend;
    FILE *fp_log;
    FILE *fp_buffer;
    FILE *fp_order;
    FILE *fp_new_order;
    if (flag == 0)
    {
        if (NULL == (fp_book_lend = fopen("BOOK_LEND_RECORD", "rb+")))
        {
            fprintf(stderr, "Can not open file");
            exit(1);
        }
        if (NULL == (fp_log = fopen("LOG", "rb+")))
        {
            fprintf(stderr, "Can not open file");
            exit(1);
        }
        if (NULL == (fp_buffer = fopen("BUFFERZONE_LEND", "rb+")))
        {
            fprintf(stderr, "Can not open file");
            exit(1);
        }
        fseek(fp_book_lend, 0, SEEK_END);
        fseek(fp_log, 0, SEEK_END);
        fseek(fp_buffer, 0, SEEK_END);
        if (fwrite(this, sizeof(Record), 1, fp_book_lend) != 1)	//this可用
            printf("file write error\n");
        if (fwrite(this, sizeof(Record), 1, fp_log) != 1)
            printf("file write error\n");
        if (fwrite(this, sizeof(Record), 1, fp_buffer) != 1)
            printf("file write error\n");
        fclose(fp_book_lend);
        fclose(fp_log);
        fclose(fp_buffer);
    }
    else if (flag == 1)
    {


        if (NULL == (fp_book_lend = fopen("BOOK_LEND_RECORD", "rb+")))
        {
            fprintf(stderr, "Can not open file");
            exit(1);
        }
        if (NULL == (fp_log = fopen("LOG", "rb+")))
        {
            fprintf(stderr, "Can not open file");
            exit(1);
        }
        if (NULL == (fp_buffer = fopen("BUFFERZONE_LEND", "rb+")))
        {
            fprintf(stderr, "Can not open file");
            exit(1);
        }
        fseek(fp_book_lend, 0, SEEK_END);
        fseek(fp_log, 0, SEEK_END);
        fseek(fp_buffer, 0, SEEK_END);
        if (fwrite(this, sizeof(Record), 1, fp_book_lend) != 1)
            printf("file write error\n");
        if (fwrite(this, sizeof(Record), 1, fp_log) != 1)
            printf("file write error\n");
        if (fwrite(this, sizeof(Record), 1, fp_buffer) != 1)
            printf("file write error\n");
        fclose(fp_book_lend);
        fclose(fp_log);
        fclose(fp_buffer);

        if (NULL == (fp_order = fopen("BUFFERZONE_ORDER", "rb+")))
        {
            fprintf(stderr, "Can not open file");
            exit(1);
        }
        if (NULL == (fp_new_order = fopen("bufferzone_ordernew", "wb+")))
        {
            fprintf(stderr, "Can not open file");
            exit(1);
        }
        Record record_temp;
        while (!feof(fp_order))
        {
            fread(&record_temp, sizeof(Record), 1, fp_order);
            if ((string)record_temp.getBookid() == (string)this->getBookid() && (string)record_temp.getCardid() == (string)this->getCardid())continue;
            fwrite(&record_temp, sizeof(Record), 1, fp_new_order);
        }
        fclose(fp_order);
        fclose(fp_new_order);
        if (remove("BUFFERZONE_ORDER") != 0)exit(1);
        if (rename("bufferzone_ordernew", "BUFFERZONE_ORDER") != 0)exit(1);

    }

}

//11.1还书记录
//向还书记录和大记录中写入一条还书记录，并在buffer文件中找出对应借书记录，将借书记录删除。
void Record::bookReturnRecord()
{
    FILE *fp_book_return;
    FILE *fp_log;
    FILE *fp_lend_buffer;
    FILE *fp_lend_buffernew;
    if (NULL == (fp_book_return = fopen("BOOK_RETURN_RECORD", "rb+")))
    {
        fprintf(stderr, "Can not open file");
        exit(1);
    }
    if (NULL == (fp_log = fopen("LOG", "rb+")))
    {
        fprintf(stderr, "Can not open file");
        exit(1);
    }
    if (NULL == (fp_lend_buffer = fopen("BUFFERZONE_LEND", "rb+")))
    {
        fprintf(stderr, "Can not open file");
        exit(1);
    }
    if (NULL == (fp_lend_buffernew = fopen("bufferzone_lendnew", "wb+")))
    {
        fprintf(stderr, "Can not open file");
        exit(1);
    }
    Record record_temp;
    while (!feof(fp_lend_buffer))
    {
        fread(&record_temp, sizeof(Record), 1, fp_lend_buffer);
        if ((string)record_temp.getBookid() == (string)this->getBookid() && (string)record_temp.getCardid() == (string)this->getCardid() && record_temp.getorder() == this->getorder())continue;
        fwrite(&record_temp, sizeof(Record), 1, fp_lend_buffernew);
    }
    fclose(fp_lend_buffer);
    fclose(fp_lend_buffernew);
    if (remove("BUFFERZONE_LEND") != 0)exit(1);
    if (rename("bufferzone_lendnew", "BUFFERZONE_LEND") != 0)exit(1);
    fseek(fp_book_return, 0, SEEK_END);
    fseek(fp_log, 0, SEEK_END);
    if (fwrite(this, sizeof(Record), 1, fp_book_return) != 1)
        printf("file write error\n");
    if (fwrite(this, sizeof(Record), 1, fp_log) != 1)
        printf("file write error\n");
    fclose(fp_book_return);
    fclose(fp_log);

}


//11.1预约记录
void Record::bookOrderRecord()
{
    FILE *fp_book_order;
    FILE *fp_log;
    FILE *fp_buffer;
    if (NULL == (fp_book_order = fopen("BOOK_ORDER_RECORD", "rb+")))
    {
        fprintf(stderr, "Can not open file");
        exit(1);
    }
    if (NULL == (fp_log = fopen("LOG", "rb+")))
    {
        fprintf(stderr, "Can not open file");
        exit(1);
    }
    if (NULL == (fp_buffer = fopen("BUFFERZONE_ORDER", "rb+")))
    {
        fprintf(stderr, "Can not open file");
        exit(1);
    }
    fseek(fp_book_order, 0, SEEK_END);
    fseek(fp_log, 0, SEEK_END);
    fseek(fp_buffer, 0, SEEK_END);
    if (fwrite(this, sizeof(Record), 1, fp_book_order) != 1)
        printf("file write error\n");
    if (fwrite(this, sizeof(Record), 1, fp_log) != 1)
        printf("file write error\n");
    if (fwrite(this, sizeof(Record), 1, fp_buffer) != 1)
        printf("file write error\n");
    fclose(fp_book_order);
    fclose(fp_log);
    fclose(fp_buffer);
}

//11.1取消预约记录
void Record::bookOrderCancelRecord()
{
    FILE *fp_book_order_cancel;
    FILE *fp_log;
    FILE *fp_order_buffer;
    FILE *fp_order_buffernew;
    if (NULL == (fp_book_order_cancel = fopen("BOOK_ORDER_CANCEL_RECORD", "rb+")))
    {
        fprintf(stderr, "Can not open file");
        exit(1);
    }
    if (NULL == (fp_log = fopen("LOG", "rb+")))
    {
        fprintf(stderr, "Can not open file");
        exit(1);
    }
    if (NULL == (fp_order_buffer = fopen("BUFFERZONE_ORDER", "rb+")))
    {
        fprintf(stderr, "Can not open file");
        exit(1);
    }
    if (NULL == (fp_order_buffernew = fopen("bufferzone_ordernew", "wb+")))
    {
        fprintf(stderr, "Can not open file");
        exit(1);
    }
    Record record_temp;
    while (!feof(fp_order_buffer))
    {
        fread(&record_temp, sizeof(Record), 1, fp_order_buffer);
        if ((string)record_temp.getBookid() == (string)this->getBookid() && (string)record_temp.getCardid() == (string)this->getCardid() && record_temp.getorder() == this->getorder())continue;
        fwrite(&record_temp, sizeof(Record), 1, fp_order_buffernew);
    }
    fclose(fp_order_buffer);
    fclose(fp_order_buffernew);
    if (remove("BUFFERZONE_ORDER") != 0)exit(1);
    if (rename("bufferzone_ordernew", "BUFFERZONE_ORDER") != 0)exit(1);

    fseek(fp_book_order_cancel, 0, SEEK_END);
    fseek(fp_log, 0, SEEK_END);
    if (fwrite(this, sizeof(Record), 1, fp_book_order_cancel) != 1)
        printf("file write error\n");
    if (fwrite(this, sizeof(Record), 1, fp_log) != 1)
        printf("file write error\n");
    fclose(fp_book_order_cancel);
    fclose(fp_log);
}

//新增无记录删除功能的预约记录函数
void Record::bookOrderNoRecord_new()
{
    FILE *fp_book_order_no;
    FILE *fp_log;
    if (NULL == (fp_book_order_no = fopen("BOOK_ORDER_CANCEL_RECORD", "rb+")))
    {
        fprintf(stderr, "Can not open file");
        exit(1);
    }
    if (NULL == (fp_log = fopen("LOG", "rb+")))
    {
        fprintf(stderr, "Can not open file");
        exit(1);
    }
    fseek(fp_book_order_no, 0, SEEK_END);
    fseek(fp_log, 0, SEEK_END);
    if (fwrite(this, sizeof(Record), 1, fp_book_order_no) != 1)
        printf("file write error\n");
    if (fwrite(this, sizeof(Record), 1, fp_log) != 1)
        printf("file write error\n");
    fclose(fp_book_order_no);
    fclose(fp_log);
}
//11.1预约失效记录
///预约失效和取消预约的文件相同
void Record::bookOrderNoRecord()
{
    FILE *fp_book_order_cancel;
    FILE *fp_log;
    FILE *fp_noorder_buffer;
    FILE *fp_noorder_buffernew;
    if (NULL == (fp_book_order_cancel = fopen("BOOK_ORDER_CANCEL_RECORD", "rb+")))
    {
        fprintf(stderr, "Can not open file");
        exit(1);
    }
    if (NULL == (fp_log = fopen("LOG", "rb+")))
    {
        fprintf(stderr, "Can not open file");
        exit(1);
    }
    if (NULL == (fp_noorder_buffer = fopen("BUFFERZONE_ORDER", "rb+")))
    {
        fprintf(stderr, "Can not open file");
        exit(1);
    }
    if (NULL == (fp_noorder_buffernew = fopen("bufferzone_ordernew", "wb+")))
    {
        fprintf(stderr, "Can not open file");
        exit(1);
    }
    Record record_temp;
    while (!feof(fp_noorder_buffer))
    {
        fread(&record_temp, sizeof(Record), 1, fp_noorder_buffer);
        if ((string)record_temp.getBookid() == (string)this->getBookid() && (string)record_temp.getCardid() == (string)this->getCardid() && record_temp.getorder() == this->getorder())continue;
        fwrite(&record_temp, sizeof(Record), 1, fp_noorder_buffernew);
    }
    fclose(fp_noorder_buffer);
    fclose(fp_noorder_buffernew);
    if (remove("BUFFERZONE_ORDER") != 0)exit(1);
    if (rename("bufferzone_lendnew", "BUFFERZONE_ORDER") != 0) exit(1);

    fseek(fp_book_order_cancel, 0, SEEK_END);
    fseek(fp_log, 0, SEEK_END);
    if (fwrite(this, sizeof(Record), 1, fp_book_order_cancel) != 1)
        printf("file write error\n");
    if (fwrite(this, sizeof(Record), 1, fp_log) != 1)
        printf("file write error\n");
    fclose(fp_book_order_cancel);
    fclose(fp_log);
}


//11.1续借记录
//由于没有取消续借功能，所以续借只存放在续借文件和大日志文件中，并未存放在缓存文件中

//续借也需要写到bufferlend缓冲文件里，因为还书的时候需要从bufferlend中检索出来
//欧巴说的都对啦啦~~mua~~
void Record::bookRenewRecord()
{
    FILE *fp_book_renew;
    FILE *fp_log;
    FILE *fp_buffer;
    FILE *fp_new_buffer_lend;
    if (NULL == (fp_book_renew = fopen("BOOK_RENEW_RECORD", "rb+")))
    {
        fprintf(stderr, "Can not open file");
        exit(1);
    }
    if (NULL == (fp_log = fopen("LOG", "rb+")))
    {
        fprintf(stderr, "Can not open file");
        exit(1);
    }
    if (NULL == (fp_buffer = fopen("BUFFERZONE_LEND", "rb+")))
    {
        fprintf(stderr, "Can not open file");
    }
    if (NULL == (fp_new_buffer_lend = fopen("bufferzone_lendnew", "wb+")))
    {
        fprintf(stderr, "Can not open file");
        exit(1);
    }
    Record record_temp;
    while (!feof(fp_buffer))
    {
        fread(&record_temp, sizeof(Record), 1, fp_buffer);
        if ((string)record_temp.getBookid() == (string)this->getBookid() && (string)record_temp.getCardid() == (string)this->getCardid() && record_temp.getorder() == this->getorder())continue;
        fwrite(&record_temp, sizeof(Record), 1, fp_new_buffer_lend);
    }
    fclose(fp_buffer);
    fclose(fp_new_buffer_lend);
    if (remove("BUFFERZONE_LEND") != 0)exit(1);
    if (rename("bufferzone_lendnew", "BUFFERZONE_LEND") != 0)exit(1);
    if (NULL == (fp_buffer = fopen("BUFFERZONE_LEND", "rb+")))
    {
        fprintf(stderr, "Can not open file");
    }
    fseek(fp_book_renew, 0, SEEK_END);
    fseek(fp_log, 0, SEEK_END);
    fseek(fp_buffer, 0, SEEK_END);
    this->setflag1('d');
    if (fwrite(this, sizeof(Record), 1, fp_book_renew) != 1)
        printf("file write error\n");
    if (fwrite(this, sizeof(Record), 1, fp_log) != 1)
        printf("file write error\n");
    if (fwrite(this, sizeof(Record), 1, fp_buffer) != 1)
        printf("file write error\n");
    fclose(fp_book_renew);
    fclose(fp_log);
    fclose(fp_buffer);
}

//11.1登录记录
//登录记录未存在缓存文件中
void Record::signInRecord()
{
    FILE *fp_sign_in;
    FILE *fp_log;
    if (NULL == (fp_sign_in = fopen("SIGN_IN_RECORD", "rb+")))
    {
        fprintf(stderr, "Can not open file");
        exit(1);
    }
    if (NULL == (fp_log = fopen("LOG", "rb+")))
    {
        fprintf(stderr, "Can not open file");
        exit(1);
    }
    fseek(fp_sign_in, 0, SEEK_END);
    fseek(fp_log, 0, SEEK_END);
    if (fwrite(this, sizeof(Record), 1, fp_sign_in) != 1)
        printf("file write error\n");
    if (fwrite(this, sizeof(Record), 1, fp_log) != 1)
        printf("file write error\n");
    fclose(fp_sign_in);
    fclose(fp_log);
}

//11.1注销记录
//未记录在缓冲文件中
void Record::signOutRecord()
{
    FILE *fp_sign_out;
    FILE *fp_log;
    if (NULL == (fp_sign_out = fopen("SIGN_OUT_RECORD", "rb+")))
    {
        fprintf(stderr, "Can not open file");
        exit(1);
    }
    if (NULL == (fp_log = fopen("LOG", "rb+")))
    {
        fprintf(stderr, "Can not open file");
        exit(1);
    }
    fseek(fp_sign_out, 0, SEEK_END);
    fseek(fp_log, 0, SEEK_END);
    if (fwrite(this, sizeof(Record), 1, fp_sign_out) != 1)
        printf("file write error\n");
    if (fwrite(this, sizeof(Record), 1, fp_log) != 1)
        printf("file write error\n");
    fclose(fp_sign_out);
    fclose(fp_log);
}

//11.1注册记录
//未放进缓冲文件中
void Record::signUpRecord()
{
    FILE *fp_sign_up;
    FILE *fp_log;
    if (NULL == (fp_sign_up = fopen("SIGN_UP_RECORD", "rb+")))
    {
        fprintf(stderr, "Can not open file");
        exit(1);
    }
    if (NULL == (fp_log = fopen("LOG", "rb+")))
    {
        fprintf(stderr, "Can not open file");
        exit(1);
    }
    fseek(fp_sign_up, 0, SEEK_END);
    fseek(fp_log, 0, SEEK_END);
    if (fwrite(this, sizeof(Record), 1, fp_sign_up) != 1)
        printf("file write error\n");
    if (fwrite(this, sizeof(Record), 1, fp_log) != 1)
        printf("file write error\n");
    fclose(fp_sign_up);
    fclose(fp_log);
}


class Library
{
public:
    /* void Library(Book book1,Card card1) {
     book = book1;
     card = card1;
     }*/
    /*void Library(Card card1) {
     card = card1;
     }*/
    Library()
    {
        //Book book1;
        //Card card1;
        //card=card1;
        // book=book1;

    }
    void signInUser(char*username_PutIn, char*password_PutIn);//用户登陆
    void signInAdmin(char*adminname_PutIn, char*password_PutIn);//管理员登陆
    void signUp(char*password, char*cardHolder, char*CID, char*CPhone);//用户注册
    void signOut();//用户注销
    void signOut_Admin();//管理员注销
    // void matchCid();//身份证ID匹配
    void ResetPassword(char*oldpassword, char*newpassword1, char*newpassword2);//输入新密码后重设密码写入原位置
    void update_Order();//函数用于用户进入系统时 对缓冲区进行更新
    void update_book();//函数用于在登陆后判断用户的已借书籍是否已经超期

    void charge(double money);//充值函数
    void Rcharge();//处理用户违约金
    // void resetCard();//更新修改卡信息 手机

    void Search(int select);//查询书本函数

    void deleteOrderFail();//删除orderbuffer中失效的预约记录
    void setbook(Book book1) {//对需要操作的书进行赋值
        book = book1;
    }
    void bookLend();//直接进行的借书
    void bookLendOrder();//通过预约成功借书
    void bookReturn(Record record1);//还书
    void bookOrder();//预约
    void bookOrderCancel();//未到期的取消预约
    void bookRenew(Record record1);//续借

	Administrator getAdmin();
private:
    Book book;
    Card card;
    Administrator admin;
};

void Library::Search(int select) //select 1表示前方一致（书名） 2表示模糊查询（书名）3表示作者 4表示出版社
{
    FILE *fp1;
    FILE *fp2;
    Book tc;
    char source[100];
    cout << "Please enter the search Information:";
    cin >> source;
    char target[100];
    int i, j;
    int n = 0;
    int num = 0;
    if (NULL == (fp1 = fopen("bookInformation", "rb")))
    {
        fprintf(stderr, "Can not open file bookInformation");
    }
    if (NULL == (fp2 = fopen("searchResult", "wb+")))
    {
        fprintf(stderr, "Can not open file searchResult");
    }
    if (select == 1)
    {
        vector<so_1> sou_1;
        sou_1.reserve(10000);
        while (n != albook)
        {
            fread(&tc, sizeof(Book), 1, fp1);
            size_t position;
            string target = tc.getbookName();
            position = target.find(source);
            if (position == 0)
            {
                if (fwrite(&tc, sizeof(Book), 1, fp2) != 1)
                    printf("file write error\n");
                so_1 temp;
                temp.b = num;
                temp.a = strlen(tc.getbookName());
                sou_1.push_back(temp);
                num++;
            }
            n++;
        }
        //num 为查找出来的书籍总数量
        sort(sou_1.begin(), sou_1.end(), comp_1);
        Book abc;   //打印书的信息，测试的时候只打印前十本，方便测试
        for (i = 0; i < num; i++)
        {
            fseek(fp2, sou_1[i].b*sizeof(Book), 0);
            fread(&abc, sizeof(Book), 1, fp2);
            cout << i + 1 << " " << abc.getbookID() << " " << abc.getbookName() << " " << abc.getauthor() << " " << abc.getpublisher() << abc.getstorage()<< endl;
        }
        //选择第number本书，把该书取出来放入book
        int number;
        cout<<"Please input the num:";
        cin >> number;
        fseek(fp2, sou_1[number-1].b*sizeof(Book), 0);
        fread(&book, sizeof(Book), 1, fp2);
        cout<<book.getbookName()<< " 已选中" <<endl;
        fclose(fp1);
        fclose(fp2);

    }
    else if (select == 2)
    {
        vector<so_2> sou_2;
        sou_2.reserve(10000);
        while (n!= allbook)
        {
            double same=0,different=0;
            double Jaccardindex=0;
            fread(&tc,sizeof(Book),1,fp1);
            strcpy(target,tc.getbookName());
            int d[100][100] = { 0 };
            int arraytarget[50];
            int arraytargetlength=0;//target数组长度
            for(int i = 0; i < strlen(target); ++i)
            {
                int ch = target[i];
                if(ch < 0)
                {
                    //  使用unsigned char 读取可以获取128 - 255 之间的数值
                    unsigned char c = target[i];
                    int itemp = c;
                    // 范围选择170 - 254是为了去掉中文标点及其他字符，范围请参考汉字ASCII对照表
                    if(itemp >= 170 && itemp <= 254)
                    {
                        arraytarget[arraytargetlength]=target[i]*256+target[i+1];
                    }
                    // 此时（ch < 0）两个char表示一个汉字，所以跳过第二个char
                    ++i;
                    ++arraytargetlength;
                }
                else if( (ch >= 97 && ch <= 122) || (ch >= 48 && ch <= 57))
                {
                    arraytarget[arraytargetlength]=ch;
                    arraytargetlength++;
                }
            }
            //source数组转换
            int arraysource[50];
            int arraysourcelength=0;//source数组长度
            for(int i = 0,t=0; i < strlen(source); ++i)
            {
                int ch = source[i];
                if(ch < 0)
                {
                    //  使用unsigned char 读取可以获取128 - 255 之间的数值
                    unsigned char c = source[i];
                    int itemp = c;
                    // 范围选择170 - 254是为了去掉中文标点及其他字符，范围请参考汉字ASCII对照表
                    if(itemp >= 170 && itemp <= 254)
                    {
                        arraysource[arraysourcelength]=source[i]*256+source[i+1];
                    }
                    // 此时（ch < 0）两个char表示一个汉字，所以跳过第二个char
                    ++i;
                    ++arraysourcelength;
                }
                else if( (ch >= 97 && ch <= 122) || (ch >= 48 && ch <= 57))
                {
                    arraysource[arraysourcelength]=ch;
                    arraysourcelength++;
                }
            }
            //计算相似度
            for(i = 0; i <= arraysourcelength; i++)
                d[i][0] = i;
            for(j = 0; j <= arraytargetlength; j++)
                d[0][j] = j;

            for(i = 1; i <= arraysourcelength; i++)
            {
                for(j = 1; j <= arraytargetlength; j++)
                {
                    if((arraysource[i - 1] == arraytarget[j - 1]))
                    {
                        d[i][j] = d[i - 1][j - 1]; //不需要编辑操作
                        same=same+1;
                    }
                    else
                    {
                        int edIns = d[i][j - 1] + 1; //source 插入字符
                        int edDel = d[i - 1][j] + 1; //source 删除字符
                        int edRep = d[i - 1][j - 1] + 1; //source 替换字符
                        d[i][j] = min(min(edIns, edDel), edRep);
                    }
                }
            }
            Jaccardindex=same/(d[arraysourcelength][arraytargetlength]+same);
            // printf("相似度%f\n",Jaccardindex);
            // printf("转换次数%d\n", d[strlen(source)][strlen(target)]);
            if(Jaccardindex>0)
            {
                if(fwrite(&tc,sizeof(Book),1,fp2)!=1)
                    printf("file write error\n");
                so_2 temp;
                temp.b=num;
                temp.a=Jaccardindex;
                sou_2.push_back(temp);
                num++;
            }
            memset(target,0,50);
            n++;
        }
        sort(sou_2.begin(),sou_2.end(),comp_2);
        Book abc;
        for(i=0; i<num; i++)
        {
            fseek(fp2,sou_2[i].b*sizeof(Book),0);
            fread(&abc,sizeof(Book),1,fp2);
            cout << i + 1 << " " << abc.getbookID() << " " << abc.getbookName() << " " << abc.getauthor() << " " << abc.getpublisher()<< abc.getstorage() << endl;
        }
        int number;
        cout<<"Please input the num:";
        cin >> number;
        fseek(fp2, sou_2[number-1].b*sizeof(Book), 0);
        fread(&book, sizeof(Book), 1, fp2);
        cout<<book.getbookName()<< " 已选中" <<endl;
        fclose(fp1);
        fclose(fp2);
    }
    else
    {
        if (select == 3) //与作者进行匹配
        {
            while (n != allbook)
            {
                fread(&tc, sizeof(Book), 1, fp1);
                if (strcmp(source, tc.getauthor()) == 0)
                {
                    if (fwrite(&tc, sizeof(Book), 1, fp2) != 1)
                        printf("file write error\n");
                    num++;
                }
                n++;
            }
        }
        if (select == 4) //与出版社进行匹配
        {
            while (n != allbook)
            {
                fread(&tc, sizeof(Book), 1, fp1);
                if (strcmp(source, tc.getpublisher()) == 0)
                {
                    if (fwrite(&tc, sizeof(Book), 1, fp2) != 1)
                        printf("file write error\n");
                    num++;
                }
                n++;
            }
        }
        fseek(fp2, 0, SEEK_SET);
        for (i = 0; i < num; i++)
        {
            fread(&tc, sizeof(Book), 1, fp2);
            cout << i + 1 << " " << tc.getbookID() << " " << tc.getbookName() << " " << tc.getauthor() << " " << tc.getpublisher() << tc.getstorage() << endl;
        }
        int number;
        cout<<"Please input the num:";
        cin >> number;
        fseek(fp2, (number-1) * sizeof(Book), 0);
        fread(&book, sizeof(Book), 1, fp2);
        cout << book.getbookName() << " 已选中" << endl;
        fclose(fp1);
        fclose(fp2);
    }
}
void Library::bookLend() { //借书 1.直接借书
    if (card.getlendedCount() == 10) {//已借本数超过上限
        cout << "可借本书已达到上限，无法再进行借阅！" << endl;
    }
    else{//可借本数没有超过上限
        if (book.getstorage() >= 2) { //库存允许
            cout << "借阅成功" << endl;
            int order = 1;//标识第几本书
            char *q = book.getBooks();
            while (!(*(q + order) == '1')) {//从第一本书开始检索而不是第0本
                order++;
            }
            book.setBooksI(order, '2');//将这本书改为已借出
            book.setstorage(book.getstorage() - 1);//库存-1
            card.setlendedCount(card.getlendedCount() + 1);//已借本数+1
            card.setlendingCount(card.getlendingCount() - 1);//可借本数-1
            //生成一条借书的记录
            time_t timer;
            time(&timer);
            tm* t_tm = localtime(&timer);    //获取了当前时间，并且转换为int类型的year，month，day
            int year = t_tm->tm_year + 1900;
            int month = month = t_tm->tm_mon + 1;
            int day = t_tm->tm_mday;
            Record record(book.getbookID(), card.getcardID(), year, month, day, 'a', '0', order);//生成一条借书的记录
            record.alter_Date(30);    //加上30天，把应还日期写进记录
            record.bookLendRecord(0);
            //写回book文件
            FILE *fp_book;
            if (NULL == (fp_book = fopen("BOOKINFORMATION", "rb+")))
            {
                fprintf(stderr, "Can not open file");
                exit(1);
            }
            int position = atoi(book.getbookID()) - 100000000 - 1;
            fseek(fp_book, position*sizeof(book), 0);
            if (fwrite(&book, sizeof(Book), 1, fp_book) != 1) {
                printf("file write error\n");
            }
            fclose(fp_book);
        }
        else { //库存不够
            int choice;
            cout << "库存不够，借阅失败！" << endl;//借阅失败
            cout << "是否进行预约？" << endl;
            cout << "1.是   2.否" << endl;//提示框①
            cin >> choice;
            while (1) {
                if (choice == 1) {
                    bookOrder();
                    break;
                }
                else if (choice == 2) {
                    //关闭提示框①返回查询界面
                    break;
                }
                else {
                    cout << "输入有误，请重新输入！" << endl;
                    cout << "是否进行预约？" << endl;
                    cout << "1.是   2.否" << endl;//提示框②
                    cin >> choice;
                    //关闭提示框②
                }
            }
        }
    }
}

void Library::bookLendOrder() {//2.通过预约成功借书
    time_t timer;
    time(&timer);
    tm* t_tm = localtime(&timer);    //获取了当前时间，并且转换为int类型的year，month，day
    int year = t_tm->tm_year + 1900;
    int month = month = t_tm->tm_mon + 1;
    int day = t_tm->tm_mday;
    card.setlendedCount(card.getlendedCount() + 1);//已借本数+1
    card.setlendingCount(card.getlendingCount() - 1);//可借本数-1
    card.setbookedCount(card.getbookedCount() - 1);//人的预约本数-1

    book.setbookMan(book.getbookMan() - 1);//书的预约人数-1
    book.settStorage(book.gettStorage() - 1);//书的临时库存-1
    int order = 1;//标识第几本书
    char *q = book.getBooks();
    while (!(*(q + order) == '1')) {//从第一本书开始检索
        order++;
    }
    book.setBooksI(order, '2');//将这本书改为已借出
    //生成一条借书的记录
    Record record(book.getbookID(), card.getcardID(), year, month, day, 'a', '0', order);
    record.alter_Date(30);
    record.bookLendRecord(1);
    //写回book文件
    FILE *fp_book;
    if (NULL == (fp_book = fopen("BOOKINFORMATION", "rb+")))
    {
        fprintf(stderr, "Can not open file");
        exit(1);
    }
    int position = atoi(book.getbookID()) - 100000000 - 1;
    fseek(fp_book, position*sizeof(book), 0);
    if (fwrite(&book, sizeof(Book), 1, fp_book) != 1) {
        printf("file write error\n");
    }
    fclose(fp_book);
}

void Library::bookReturn(Record record1){ //还书（需要用到qt）
    cout << "还书成功！" << endl;
    time_t timer;
    time(&timer);
    tm* t_tm = localtime(&timer);    //获取了当前时间，并且转换为int类型的year，month，day
    int year = t_tm->tm_year + 1900;
    int month = month = t_tm->tm_mon + 1;
    int day = t_tm->tm_mday;
    card.setlendedCount(card.getlendedCount() - 1);//已借本数-1
    card.setlendingCount(card.getlendingCount() + 1);//可借本数+1
    //1.检测这本书是否有人预约
    if (book.getbookMan()>0) {
        //1.1若有人预约
        if (book.gettStorage() == book.getbookMan()) {//检测临时库存是否等于预约人数，若等于则库存+1
            book.setstorage(book.getstorage() + 1);
        }
        else {//若不等于，临时库存+1
            book.settStorage(book.gettStorage() + 1);
        }
    }
    else {
        //1.2若无人预约，库存+1
        book.setstorage(book.getstorage() + 1);
    }
    //将order改为1可借
    book.setBooksI(record1.getorder(),'1');
    //写回book文件
    FILE *fp_book;
    if (NULL == (fp_book = fopen("BOOKINFORMATION", "rb+")))
    {
        fprintf(stderr, "Can not open file");
        exit(1);
    }
    int position = atoi(book.getbookID()) - 100000000 - 1;
    fseek(fp_book, position*sizeof(book), 0);
    if (fwrite(&book, sizeof(Book), 1, fp_book) != 1) {
        printf("file write error\n");
    }
    fclose(fp_book);
    //生成一条还书记录
    Record record(book.getbookID(), card.getcardID(), year, month, day, 'b', '0',record1.getorder());
    record.bookReturnRecord();
    //用户超期处理
    if (!(compareDate(record1.getyear(), record1.getmonth(), record1.getday(), year, month, day) > 0) )
    {
          double money = 0.5*compareDate(year, month, day, record1.getyear(), record1.getmonth(), record1.getday());
          card.setbalance(card.getbalance()-money);
          card.setoweMoney(card.getoweMoney()-money);
          cout << "扣除" << money <<"元违约金" <<endl;
    }
}

void Library::bookOrder(){//预约
    //预约记录就记录预约时间即可，因为为了方便在update_order里使用
    if (card.getbookedCount() == 5) {//预约本数已达上限
        cout << "您的预约本数已达上限，无法进行预约！" << endl;
    }
    else{
        cout << "预约成功！" << endl;//提示预约成功
        book.setbookMan(book.getbookMan() + 1);//书的预约人数+1
        card.setbookedCount(card.getbookedCount() + 1);//人的预约本数+1
        //写回book文件
        FILE *fp_book;
        if (NULL == (fp_book = fopen("BOOKINFORMATION", "rb+")))
        {
            fprintf(stderr, "Can not open file");
            exit(1);
        }
        int position = atoi(book.getbookID()) - 100000000 - 1;
        fseek(fp_book, position*sizeof(book), 0);
        if (fwrite(&book, sizeof(Book), 1, fp_book) != 1) {
            printf("file write error\n");
        }
        fclose(fp_book);
        //生成一条预约记录
        time_t timer;
        time(&timer);
        tm* t_tm = localtime(&timer);    //获取了当前时间，并且转换为int类型的year，month，day
        int year = t_tm->tm_year + 1900;
        int month = month = t_tm->tm_mon + 1;
        int day = t_tm->tm_mday;
        Record record(book.getbookID(), card.getcardID(), year, month, day, 'c', '0');
        record.bookOrderRecord();
    }
}

void Library::bookOrderCancel(){//取消预约 1.未到期取消预约
    // Record record(book.getBookID(), card.getcardID(), year, month, day, 'e', '0');
    int choice;
    cout << "确定取消预约吗？" << endl;
    cout << "1.是 2.否" << endl;
    cin >> choice;
    while (1) {
        if (choice == 1) {//1.
            cout << "成功取消预约！" << endl;
            if (book.getbookMan() == book.gettStorage()) { //若取消预约时临时库存等于预约人数
                book.settStorage(book.gettStorage() - 1);//临时库存-1
                book.setstorage(book.getstorage() + 1);//库存+1
            }
            book.setbookMan(book.getbookMan() - 1);//此书的预约人数-1
            card.setbookedCount(card.getbookedCount() - 1);//此人的预约数量-1
            //生成一条取消预约的记录
            time_t timer;
            time(&timer);
            tm* t_tm = localtime(&timer);    //获取了当前时间，并且转换为int类型的year，month，day
            int year = t_tm->tm_year + 1900;
            int month = month = t_tm->tm_mon + 1;
            int day = t_tm->tm_mday;
            Record record(book.getbookID(), card.getcardID(), year, month, day, 'e', '0');
            record.bookOrderCancelRecord();
            //写回book文件
            FILE *fp_book;
            if (NULL == (fp_book = fopen("BOOKINFORMATION", "rb+")))
            {
                fprintf(stderr, "Can not open file");
                exit(1);
            }
            int position = atoi(book.getbookID()) - 100000000 - 1;
            fseek(fp_book, position*sizeof(book), 0);
            if (fwrite(&book, sizeof(Book), 1, fp_book) != 1) {
                printf("file write error\n");
            }
            fclose(fp_book);
            break;
        }
        else if (choice == 2) {//2.否
            //返回借阅信息界面
            break;
        }
        else {//输入有误
            cout << "输入有误，请重新输入！" << endl;
            cout << "确定取消预约吗？" << endl;
            cout << "1.是   2.否" << endl;
            cin >> choice;
        }
    }
}

void Library::bookRenew(Record record1){//图书续借（需要用到qt）
   //获取借书的应还书时间
    int year = record1.getyear();
    int month = record1.getmonth();
    int day = record1.getday();
    Record record(book.getbookID(), card.getcardID(), year, month, day, 'd', '1',record1.getorder());
    cout << "续借成功" << endl;
    record.alter_Date(30);        //加上30天，将应还日期写进记录
    record.bookRenewRecord();//生成一条续借记录
}

void Library::deleteOrderFail() {//将预约缓冲区里已标记为1的记录删除
    FILE *fp_buffer;
    FILE *fp_new_buffer_order;
    if (NULL == (fp_buffer = fopen("BUFFERZONE_ORDER", "rb+")))
    {
        fprintf(stderr, "Can not open file");
        exit(1);
    }
    if (NULL == (fp_new_buffer_order = fopen("bufferzone_ordernew", "wb+")))
    {
        fprintf(stderr, "Can not open file");
        exit(1);
    }
    Record record_temp;
    while (!feof(fp_buffer))
    {
        fread(&record_temp, sizeof(Record), 1, fp_buffer);
        if (record_temp.getflag2()=='1' && (string)record_temp.getCardid() == (string)card.getcardID()) {        //只能删除当前用户失效的预约记录，所以应该判断这条记录的cardID和当前用户的cardID是否一致
            continue;
        }
        fwrite(&record_temp, sizeof(Record), 1, fp_new_buffer_order);
    }
    fclose(fp_buffer);
    fclose(fp_new_buffer_order);
    if (remove("BUFFERZONE_ORDER") != 0)exit(1);
    if (rename("bufferzone_ordernew", "BUFFERZONE_ORDER") != 0)exit(1);
}

void Library::signInUser(char*username_PutIn, char*password_PutIn)         //用户登录
{
    //将用户输入的id和密码传到形参以便进行账号和密码的匹配
    /*FILE*fpEnd = fopen("BOOKINFORMATION", "rb+");    //用于标志文件的末尾，以控制查找时的循环变量的控制。
     if (fpEnd == NULL) {
     printf("file error\n");
     exit(1);
     }
     fseek(fpEnd, 0, SEEK_END);        //把fpEnd指针移到文件末尾*/
    FILE *fp = fopen("CARDINFORMATION", "rb+");        //在循环时每一次往后移动的指针
    if (fp == NULL)
    {
        printf("file error\n");
        exit(1);
    }
    Card card_find;
    Card card_temp;
    int i = 0;    //循环变量，用于将fp向后移动
    while (/*fp != fpEnd*/i<allcard)
    {
        fseek(fp, i * sizeof(Card), SEEK_SET);
        fread(&card_temp, sizeof(Card), 1, fp);
        if (strcmp(card_temp.getcardID(), username_PutIn) == 0)     //如果找到对应的card就用复制构造函数把找到的值赋值给一个暂时的变量card_find，以便于后面的密码匹配
        {
            card_find = card_temp;
            break;
        }
        i++;
    }
    if (strcmp(card_find.getcardID(), username_PutIn) == 0 && strcmp(card_find.getcPassword(), password_PutIn) == 0)
    {
        //账号和密码匹配成功后就可以登录成功了，然后就直接把查找到的card_find赋值给私有成员card
        card = card_find;
        time_t timer;
        time(&timer);
        tm* t_tm = localtime(&timer);    //获取了当前时间，并且转换为int类型的year，month，day
        int year = t_tm->tm_year + 1900;
        int month = month = t_tm->tm_mon + 1;
        int day = t_tm->tm_mday;
        Record record(card.getcardID(), year, month, day, 'i');
        record.signInRecord();
        fclose(fp);
        return;
    }
    else
    {
        fclose(fp);
        return;
    }

}

void Library::signInAdmin(char*adminname_PutIn, char*password_PutIn)     //管理员登录
{
    //将管理员输入的id和密码传到形参以便进行账号和密码的匹配
    /*FILE*fpEnd = fopen("ADMININFORMATION", "rb+");    //用于标志文件的末尾，以控制查找时的循环变量的控制。
     if (fpEnd == NULL) {
     printf("file error\n");
     exit(1);
     }
     fseek(fpEnd, 0, SEEK_END);        //把fpEnd指针移到文件末尾*/
    FILE*fp = fopen("ADMININFORMATION", "rb+");        //在循环时每一次往后移动的指针
    if (fp == NULL)
    {
        printf("file error\n");
        exit(1);
    }
    Administrator admin_find;
    Administrator admin_temp;
    int i = 0;    //循环变量，用于将fp向后移动
    while (/*fp != fpEnd*/i<alladmin)
    {
        fseek(fp, i * sizeof(Administrator), SEEK_SET);
        fread(&admin_temp, sizeof(Administrator), 1, fp);
        if (strcmp(admin_temp.getaccount(), adminname_PutIn) == 0)     //如果找到对应的admin就用复制构造函数把找到的值赋值给一个暂时的变量admin_find，以便于后面的密码匹配
        {
            admin_find = admin_temp;
            break;
        }
        i++;
    }
    if (strcmp(admin_find.getaccount(), adminname_PutIn) == 0 && strcmp(admin_find.getaPassword(), password_PutIn) == 0)
    {
        //账号和密码匹配成功后就可以登录成功了，然后就直接把查找到的admin_find赋值给私有成员admin
        admin = admin_find;
        time_t timer;
        time(&timer);
        tm* t_tm = localtime(&timer);    //获取了当前时间，并且转换为int类型的year，month，day
        int year = t_tm->tm_year + 1900;
        int month = month = t_tm->tm_mon + 1;
        int day = t_tm->tm_mday;
        Record record(admin.getaccount(), year, month, day, 'i');
        record.signInRecord();
        fclose(fp);
        return;
    }
    else
    {
        fclose(fp);
        return;
    }
}

void Library::signUp(char*password, char*cardHolder, char*CID, char*CPhone)     //用户注册
{
    string account_str = to_string(10000 + allcard + 1);
    char account[10];
    strcpy(account,account_str.c_str());
    Card newcard(account, password, cardHolder, 0, CID, CPhone);
    FILE*fp_card;
    if (NULL == (fp_card = fopen("CARDINFORMATION", "rb+")))
    {
        fprintf(stderr, "Can not open file");
        exit(1);
    }
    fseek(fp_card, 0, SEEK_END);
    if (fwrite(&newcard, sizeof(Card), 1, fp_card) != 1)
        printf("file write error\n");
    time_t timer;
    time(&timer);
    tm* t_tm = localtime(&timer);    //获取了当前时间，并且转换为int类型的year，month，day
    int year = t_tm->tm_year + 1900;
    int month = month = t_tm->tm_mon + 1;
    int day = t_tm->tm_mday;
    Record record(newcard.getcardID(), year, month, day, 'g');
    record.signUpRecord();
    fclose(fp_card);
    allcard++;
	FILE *fp_num;
	if (NULL == (fp_num = fopen("ALLNUM", "rb+")))
	{
		fprintf(stderr, "Can not open file");
		exit(1);
	}
	if (fwrite(&allcard, sizeof(int), 1, fp_num) != 1)            //覆盖写入?
		printf("file write error\n");
	if (fwrite(&allbook, sizeof(int), 1, fp_num) != 1)
		printf("file write error\n");
	if (fwrite(&alladmin, sizeof(int), 1, fp_num) != 1)
		printf("file write error\n");
	fclose(fp_num);
    return;
}

void Library::signOut()         //用户注销
{
    //把刚刚登录时获取的card写回文件原来的位置
    FILE*fp_card;
    if (NULL == (fp_card = fopen("CARDINFORMATION", "rb+")))
    {
        fprintf(stderr, "Can not open file");
        exit(1);
    }
    int position = atoi(card.getcardID()) - 10000 - 1;
    fseek(fp_card, position*sizeof(Card), 0);
    if (fwrite(&card, sizeof(Card), 1, fp_card) != 1)
        printf("file write error\n");
    time_t timer;
    time(&timer);
    tm* t_tm = localtime(&timer);    //获取了当前时间，并且转换为int类型的year，month，day
    int year = t_tm->tm_year + 1900;
    int month = month = t_tm->tm_mon + 1;
    int day = t_tm->tm_mday;
    Record record(card.getcardID(), year, month, day, 'h');
    record.signOutRecord();
    fclose(fp_card);
    //把card变量重新用无参的构造函数赋值，是否能用？
    Card card_blank;
    card = card_blank;
    //是否将allcard，allbook，alladmin写回文件？
    FILE *fp_num;
    if (NULL == (fp_num = fopen("ALLNUM", "rb+")))
    {
        fprintf(stderr, "Can not open file");
        exit(1);
    }
    if (fwrite(&allcard, sizeof(int), 1, fp_num) != 1)            //覆盖写入?
        printf("file write error\n");
    if (fwrite(&allbook, sizeof(int), 1, fp_num) != 1)
        printf("file write error\n");
    if (fwrite(&alladmin, sizeof(int), 1, fp_num) != 1)
        printf("file write error\n");
    fclose(fp_num);
}

void Library::signOut_Admin()         //管理员注销
{
    //把刚刚登录时获取的admin写回文件原来的位置
    FILE*fp_admin;
    if (NULL == (fp_admin = fopen("ADMININFORMATION", "rb+")))
    {
        fprintf(stderr, "Can not open file");
        exit(1);
    }
    int position = atoi(card.getcardID()) - 2000 - 1;    //管理员账户格式2001
    fseek(fp_admin, position*sizeof(Administrator), 0);
    if (fwrite(&admin, sizeof(Administrator), 1, fp_admin) != 1)
        printf("file write error\n");
    time_t timer;
    time(&timer);
    tm* t_tm = localtime(&timer);    //获取了当前时间，并且转换为int类型的year，month，day
    int year = t_tm->tm_year + 1900;
    int month = month = t_tm->tm_mon + 1;
    int day = t_tm->tm_mday;
    Record record(admin.getaccount(), year, month, day, 'h');
    record.signOutRecord();
    fclose(fp_admin);
    //同用户注销函数
    Administrator admin_blank;
    admin = admin_blank;
    //是否将allcard，allbook，alladmin写回文件？
    FILE *fp_num;
    if (NULL == (fp_num = fopen("ALLNUM", "rb+")))
    {
        fprintf(stderr, "Can not open file");
        exit(1);
    }
    if (fwrite(&allcard, sizeof(int), 1, fp_num) != 1)            //覆盖写入?
        printf("file write error\n");
    if (fwrite(&allbook, sizeof(int), 1, fp_num) != 1)
        printf("file write error\n");
    if (fwrite(&alladmin, sizeof(int), 1, fp_num) != 1)
        printf("file write error\n");
    fclose(fp_num);
}

/*void Library::matchCid(){        //身份证ID匹配

 }*/

void Library::ResetPassword(char*oldpassword, char*newpassword1, char*newpassword2)     //输入新密码后重设密码写入原位置
{
    if ((string)oldpassword == (string)card.getcPassword())
    {
        if ((string)newpassword1 == (string)newpassword2)
            card.setcPassword(newpassword1);
    }
    return;
}

void Library::update_Order()             //函数用于用户进入系统时 对缓冲区进行更新
{
    /*
     匹配查看预约记录是否失效
     如果失效 写入系统记录 将该记录标识置为1
     然后找到该书 令其预约人-1
     如果此时临时库存>预约人数
     把书放入库存 临时库存-1
     #define BUFFERZONE_ORDER bufferOrderZone//预约缓冲区文件
     #define BOOKINFORMATION bookInformation//全部图书信息
     #define CARDINFORMATION cardInformation//全部用户信息
     */
    FILE *fp_buffer_order = NULL;
    FILE *fp_bookInfo = NULL;
    FILE *fp_cardInfo = NULL;
    if ((fp_buffer_order = fopen("BUFFERZONE_ORDER", "rb+")) == NULL)
    {
        fprintf(stderr, "Can not open file");
        exit(1);
    }
    if ((fp_bookInfo = fopen("BOOKINFORMATION", "rb+")) == NULL)
    {
        fprintf(stderr, "Can not open file");
        exit(1);
    }
    if ((fp_cardInfo = fopen("CARDINFORMATION", "rb+")) == NULL)
    {
        fprintf(stderr, "Can not open file");
        exit(1);
    }
    Record record_temp;
    Card card_temp;
    Book book_temp;
    time_t timer;
    time(&timer);
    tm* t_tm = localtime(&timer);    //获取了当前时间，并且转换为int类型的year，month，day
    int year = t_tm->tm_year + 1900;
    int month = month = t_tm->tm_mon + 1;
    int day = t_tm->tm_mday;
    while (!feof(fp_buffer_order)) //feof()函数可以用来判断文件是否到达文件尾，若到达文件尾，函数返回值为1
    {
        fread(&record_temp, sizeof(Record), 1, fp_buffer_order);
        if (compareDate(year, month, day, record_temp.getyear(), record_temp.getmonth(), record_temp.getday()) > 10)
        {
            //修改各个需要修改的变量,书的预约人数-1，比较书的临时库存和预约人数的大小，再对库存和临时库存做相应操作。用户的预约本数-1
            while (!feof(fp_bookInfo))
            {
                fread(&book_temp, sizeof(Book), 1, fp_bookInfo);
                if (strcmp(book_temp.getbookID(), record_temp.getBookid()) == 0)
                {
                    book_temp.setbookMan(book_temp.getbookMan()-1);
                    if (book_temp.getbookMan() < book_temp.gettStorage()) //预约人数小于临时库存
                    {
                        book_temp.settStorage(book_temp.gettStorage()-1);//临时库存-1
                        book_temp.setstorage(book_temp.getstorage() + 1);//库存+1
                    }
                    if (fwrite(&book_temp, sizeof(Book), 1, fp_bookInfo) != 1)printf("file write error\n");//修改文件中的内容
                    break;
                }
            }
            while (!feof(fp_cardInfo))
            {
                fread(&card_temp, sizeof(Card), 1, fp_cardInfo);
                if (strcmp(card_temp.getcardID(), record_temp.getCardid()) == 0)
                {
                    card_temp.setbookedCount(card_temp.getbookedCount()-1);//用户的预约本数减一
                    if (fwrite(&card_temp, sizeof(Card), 1, fp_cardInfo) != 1)printf("file write error\n");//修改文件中的内容
                    break;
                }
            }
			//将fp_bookInfo和fp_cardInfo调回文件开头，便于对下一条记录的card和book搜索
            rewind(fp_bookInfo);
            rewind(fp_cardInfo);//rewind()函数用于将文件指针重新指向文件的开头，同时清除和文件流相关的错误和eof标记，相当于调用fseek(stream, 0, SEEK_SET)
            record_temp.setflag2('1');//1对预约记录表示此预约失效
			int size = sizeof(Record);
            fseek(fp_buffer_order,-size,SEEK_CUR);//由于fread函数使用后会使指针后移，所以在重写当前位置时要将指针向前移动一个单位，SEEK_CUR表示从当前位置
            if (fwrite(&record_temp, sizeof(Record), 1, fp_buffer_order) != 1)printf("file write error\n");//更新预约缓冲文件
            record_temp.setflag1('f');
            record_temp.setyear(year);
            record_temp.setmonth(month);
            record_temp.setday(day);//将record_temp修改成一条预约失效记录
            record_temp.bookOrderNoRecord_new();//调用函数写入失效记录文件
        }
    }
	fclose(fp_bookInfo);
	fclose(fp_cardInfo);
    fclose(fp_buffer_order);
}

void Library::update_book()         //函数用于在登录后判断用户的已借书籍是否已经超期
{
	FILE*fp_lendbuffer;
	if ((fp_lendbuffer = fopen("BUFFERZONE_LEND", "rb+")) == NULL)
	{
		fprintf(stderr, "Can not open file");
		exit(1);
	}
	/*FILE*fp_End = fopen("BUFFERZONE_LEND", "rb+");
	if (fp_End == NULL)
	{
	printf("file error\n");
	exit(1);
	}*/
	//fseek(fp_End, 0, SEEK_END);        //把fpEnd指针移到文件末尾*/
	Record record_temp;        //用于读取借书buffer中的每一条记录
	int i = 0;
	fseek(fp_lendbuffer, i * sizeof(Record), SEEK_SET);
	card.setoweMoney(0);	//每次登陆时重新计算超期违约金
	while (!feof(fp_lendbuffer))
	{
		if (fread(&record_temp, sizeof(Record), 1, fp_lendbuffer)){
			if ((string)record_temp.getCardid() == (string)card.getcardID())
			{
				//确实是当前用户借阅并且未还的书籍
				cout << record_temp.getBookid() << " " << record_temp.getCardid() << " " << record_temp.getyear() << " " << record_temp.getmonth() << " " << record_temp.getday() << " " << record_temp.getflag1() << " " << record_temp.getorder() << endl;
				time_t timer;
				time(&timer);
				tm* t_tm = localtime(&timer);    //获取了当前时间，并且转换为int类型的year，month，day
				int year = t_tm->tm_year + 1900;
				int month = month = t_tm->tm_mon + 1;
				int day = t_tm->tm_mday;
				//判断当前时间与应还日期
				if (!(compareDate(record_temp.getyear(), record_temp.getmonth(), record_temp.getday(), year, month, day) > 0))
				{
					//如果当前日期超过还书日期，那么就进行违约金处理；

					card.setoweMoney(card.getoweMoney() + 0.5*compareDate(year, month, day, record_temp.getyear(), record_temp.getmonth(), record_temp.getday()));//按超期一天0.5元计算
					if (card.getbalance() < card.getoweMoney())card.setcardState('0');        //余额不足冻结账号
					cout << "违约金为：" << card.getoweMoney() << endl;
				}
			}
			//i++;
			//fseek(fp_lendbuffer, i * sizeof(Record), SEEK_SET);
			cout << endl;
		}
	}
	fclose(fp_lendbuffer);
	//fclose(fp_End);
	cout << "更新结束" << endl;
}

void Library::charge(double money)             //充值函数
{
    card.setbalance(card.getbalance() + money);
	if (card.getbalance() > card.getoweMoney() && card.getcardState() == '0'){
		card.setcardState('1');

	}
}

void Library::Rcharge()         //处理用户违约金
{
    double owemoney = card.getoweMoney();
    card.setbalance(card.getbalance() - owemoney);
}

/*void Library::resetCard(){        //更新修改卡信息 手机

 }*/

