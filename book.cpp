#include<iostream>
#include<stdio.h>
#include<string>
#include<time.h>
#include<windows.h>

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
#define BUFFERZONE bufferZone//缓冲区文件
#define SEARCHRESULT searchResult//查询结果文件
#define SEARCHRESULTTEMP searchResultTmp//查询结果临时文件
#define LOG log;//图书馆大日志
#define BOOK_RETURN_RECORD bookReturnRecord //11.1还书记录对应文件

using namespace std;

int allcard;//从文件中读取 修改后重新写入文件  用户注册 ++
int allbook;//增加图书 ++
int alladmin;//全部管理员，新的管理员增加时++

int compareDate();//匹配

class Book//构造函数 复制构造函数
{
public:
    Book(char BookID[10],char BookName[50],char Author[20],char Publisher[20],char Storage)//构造函数
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
        storage=Storage;//初始库存为10本
        bookMan=0;//初始预约人数为0
        tStorage=0;//初始预约该书的人数为0
        flag='1';   //所有标记 0表示不存在 1表示存在//此处，1表示书可借
    }
    Book()
    {
        for(int i=0; i<10; i++)
        {
            bookID[i]=' ';
        }
        for(int i=0; i<50; i++)
        {
            bookName[i]=' ';
        }
        for(int i=0; i<20; i++)
        {
            author[i]=' ';
        }
        for(int i=0; i<20; i++)
        {
            publisher[i]=' ';
        }
        storage=10;//初始库存为10本
        bookMan=0;//初始预约人数为0
        tStorage=0;//初始预约该书的人数为0
        flag='1';   //所有标记 0表示不存在 1表示存在//此处，1表示书可借
    }

    Book(Book &book) //复制构造函数
    {
        for(int i=0; i<10; i++)
        {
            bookID[i]=book.bookID[i];
        }
        for(int i=0; i<50; i++)
        {
            bookName[i]=book.bookName[i];
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
    void setbookID(char newbookID[10])
    {
        for(int i=0; i<10; i++)
        {
            bookID[i]=newbookID[i];
        }
    }
    char *getbookName()
    {
        return bookName;
    }
    void setbookName(char newbookName[50])
    {
        for(int i=0; i<50; i++)
        {
            bookName[i]=newbookName[i];
        }
    }
    char *getauthor()
    {
        return author;
    }
    void setauthor(char newauthor[20])
    {
        for(int i=0; i<20; i++)
        {
            author[i]=newauthor[i];
        }
    }
    char *getpublisher()
    {
        return publisher;
    }
    void setpublisher(char newpublisher[20])
    {
        for(int i=0; i<20; i++)
        {
            publisher[i]=newpublisher[i];
        }
    }
    short getstorage()
    {
        return storage;
    }
    void setstorage(short newstorage)
    {
        storage=newstorage;
    }
    short getbookMan()
    {
        return bookMan;
    }
    void setbookMan(short newbookMan)
    {
        bookMan=newbookMan;
    }
    short gettStorage()
    {
        return tStorage;
    }
    void settStorage(short newStorage)
    {
        tStorage=newStorage;
    }
    char getflag()
    {
        return flag;
    }
    void setflag(char newflag)
    {
        flag=newflag;
    }
private:
    char bookID[10];//图书编号
    char bookName[50];//书名
    char author[20];//作者
    char publisher[20];//出版商
    short storage; //库存
    short bookMan; //预约人数
    short tStorage;  //临时库存
    char flag;  //图书是否存在


friend class Administrator;//将BOOK类设为管理员类的友元类，否则管理员类中的改库存函数无法访问tStorage私有变量
};


class Card//构造函数  复制构造函数
{
Public:
    Card(char CardID[10],char CPassword[20],char CardHolder[10],double Balance,char CID[18],char CPhone[11]){//构造函数
    	for(int i=0; i<10; i++)
        {
            cardID[i]=CardID[i];
        }

        for(int i=0; i<20; i++)
        {
            cPassword[i]=CPassword[i];
        }

        for(int i=0; i<10; i++)f
        {
            cardHolder[i]=CardHolder[i];
        }
        lendedCount=0;//初始已借本数为0
        lendingCount=10;//初始可借本数为10
        bookState='1';//1表示未冻结
        balance=Balance;
        oweMoney=0;
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
    		for(int i=0; i<10; i++)
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
        oweMoney=card.oweMoney;
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
    Card()
    {
        int i=0;
        for(i=0;i<10<i++)
        {
            cardID[i]=' ';
        }
        for(i=0;i<20;i++)
        {
            cPassword[i]=' ';
        }
        for(i=0;i<10;i++)
        {
            cardHolder[i]=' ';
        }
        balance=0;
        for(i=0;i<18;i++)
        {
            cID[i]=' ';
        }
        for(i=0;i<11;i++)
        {
            cPhone=' ';
        }
        lendedCount=0;//初始已借本数为0
        lendingCount=10;//初始可借本数为10
        bookState='1';//1表示未冻结
        oweMoney=0;
        bookedCount=0;//初始预约本数为0
    }

   	char *getcardID(){
   			return cardID;
   	}
   	void setcardID(char newcardID[10]){
   			for(int i=0; i<10; i++)
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
   	double getoweMoney(){
   			return oweMoney;
   	}
   	void setoweMoney(double newoweMoney){
   			oweMoney=newoweMoney;
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

    char cardID[10];//卡号
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
Public
	//11.1构造函数
	Administrator(char Account[11], char APassword[20], char AccountHolder[10], char AID[18], char APhone[11])//构造函数
	{

		for (int i = 0; i<11; i++)
		{
			account[i] = Account[i];
		}
		for (int i = 0; i<20; i++)
		{
			APassword[i] = aPassword[i];
		}
		for (int i = 0; i<10; i++)
		{
			AccountHolder[i] = accountHolder[i];
		}
		for (int i = 0; i<18; i++)
		{
			aID[i] = AID[i];
		}
		for (int i = 0; i < 11; i++)
		{
			aPhone[i] = APhone[i];
		}
	}
	Administrator()
	{
		for (int i = 0; i<11; i++)
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
		for (int i = 0; i<18; i++)
		{
			aID[i] = ' ';
		}
		for (int i = 0; i<11; i++)
		{
			aPhone[i] = ' ';
		}
	}
	
	//复制构造函数
	Administrator(Administrator &administrator) 
	{
		for (int i = 0; i<11; i++)
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
		for (int i = 0; i<18; i++)
		{
			aID[i] = administrator.aID[i];
		}
		for (int i = 0; i<11; i++)
		{
			aPhone[i] = administrator.aPhone[i];
		}
	}

	void setaccount(char newaccount[10])
	{
		for (int i = 0; i<11; i++)
		{
			account[i] = newaccount[i];
		}
	}

	void setaPassword(char newaPassword[50])
	{
		for (int i = 0; i<20; i++)
		{
			aPassword[i] = newaPassword[i];
		}
	}

	void setaccountHolder(char newaccountHolder[20])
	{
		for (int i = 0; i<10; i++)
		{
			accountHolder[i] = newaccontHolder[i];
		}
	}

	void setaID(char newaID[20])
	{
		for (int i = 0; i<18; i++)
		{
			aID[i] = newaID[i];
		}
	}
	void setaPhone(char newaaPhone[20])
	{
		for (int i = 0; i<11; i++)
		{
			aPhone[i] = newaPhone[i];
		}
	}
	char *getaccount()
	{
		return account;
	}
	char *getaPassword()
	{
		return aPassword
	
	}
	char *getaccountHolder()
	{
		return accountHolder;
	}
	char *aID()
	{
		return aID;
	}
	char *aPhone()
	{
		return aPhone;
	}

	void addBook(Book book);//增加书
	//void deleteBook(Book book);老师说不要删书
	void newStorage(Book book);//新设库存
	void searchRecord();//查询记录   1.
	//void operateCard(Card card);老师说不要删卡 听老师的
    Private:
    char account[10];
    char aPassword[20];
    char accountHolder[10];
    char aID[18];
    char aPhone[11];
	Book book;
};

//11.2管理员新加书函数
void Administrator::addBook(Book book)
{
	FILE *fp_add_book;
	if (NULL == (fp_book_lend = fopen("BOOKINFORMATION", "rb+")))
	{
		fprintf(stderr, "Can not open file");
		exit(1);
	}
	fseek(fp_add_book, 0, SEEK_END);
	fwrite(&book,sizeof(Book),1,fp_add_book);
	fclose(fp_add_book);
}

//11.2管理员改库存函数
void Administrator::newStorage(Book book)
{
	//让管理员输入想要加的本数。如果删除几本，就写负值
	short nStorage;
	cin >> nStorage;
	book.setstorage(book.getstorage() + nStorage);
}


class Record
{
Public:
	//10.30 构造函数更改
	Record(char*bookid1, char*cardid1, int Year, int Month, int Day, char flag11, char flag22)
	//Record(char* bookid, char* cardid, char flag11, int Year, int Month, int Day, char flag22)
    {

		bookid = bookid1;
		cardid = cardid1;
		Year = year;
		Month = month;
		Day = day;
		flag1 = flag11;
		flag2 =flag22;
        //获取当前系统日期 自行查询方法 读入当前year month day
    }


	Record(char*cardid1, int Year, int Month, int Day, int flag11, int flag22) 
	{
		cardid = card1;
		Year = year;
		Month = month;
		Day = day;
		flag1 = flag11;
		flag2 = flag22;
	}
	//复制构造函数
	Record(Record &R);

    void bookLendRecord();//借书记录
    void bookReturnRecord();//还书记录
	void bookOrderRecord();//预约记录
    void bookRenewRecord();//续借记录
    void bookOrderCancelRecord();//取消预约记录
    void bookOrderNoRecord();//预约失效记录
    void signInRecord();//登陆记录
    void signOutRecord();//注销记录
	void signUpRecord();//注册记录

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
	private：
    char flag1;  //a借书 b还书 c预约 d续借 e取消预约 f预约失效 g注册记录 h注销记录 i登陆记录
    Book book;
    Card card;
	char*bookid;
	char*cardid;
    int year;
    int month;
    int day;
    char flag2;//用于缓冲区   1对预约记录表示此预约失效并且已经写入记录文件 1对续借记录表示该书已续借
};

//Record类内部函数的实现


//10.31借书记录

void Record::bookLendRecord()		//借书记录
{
	FILE *fp_book_lend;
	FILE *fp_log;
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
	fseek(fp_book_lend, 0, SEEK_END);
	fseek(fp_log, 0, SEEK_END);
	this->setflag1('a');
	if (fwrite(this, sizeof(Record), 1, fp_book_lend) != 1)
		printf("file write error\n");
	if (fwrite(this, sizeof(Record), 1, fp_log) != 1)
		printf("file write error\n");
	fclose(fp_book_lend);
	fclose(fp_log);
}

//11.1还书记录
//向还书记录和大记录中写入一条还书记录，并在buffer文件中找出对应借书记录，将借书记录删除。
void Record::bookReturnRecord()
{
	FILE *fp_book_return;
	FILE *fp_log;
	//FILE *fp_buffer;
	if (NULL == (fp_book_lend = fopen("BOOK_RETURN_RECORD", "rb+")))
	{
		fprintf(stderr, "Can not open file");
		exit(1);
	}
	if (NULL == (fp_log = fopen("LOG", "rb+")))
	{
		fprintf(stderr, "Can not open file");
		exit(1);
	}
	//if (NULL == (fp_buffer = fopen("BUFFERZONE", "rb+")))
	//{
		//fprintf(stderr, "Can not open file");
		//exit(1);
//	}
	fseek(fp_book_return, 0, SEEK_END);
	fseek(fp_log, 0, SEEK_END);
	this->setflag1('b');
	//Record record();
	//time_t timer;
	//time(&timer);
	//tm* t_tm = localtime(&timer);	//获取了当前时间，并且转换为int类型的year，month，day
	//int year = t_tm->tm_year + 1900;
	//int month = month = t_tm->tm_mon + 1;
	//int day = t_tm->tm_mday;
	//Record new_record(book, card, 'a', year, month, day, '0');
	if (fwrite(this, sizeof(Record), 1, fp_book_return) != 1)
		printf("file write error\n");
	if (fwrite(this, sizeof(Record), 1, fp_log) != 1)
		printf("file write error\n");
	fclose(fp_book_return);
	fclose(fp_log);
	//fclose(fp_buffer);

}


//11.1预约记录
void Record::bookOrderRecord()
{
	FILE *fp_book_order;
	FILE *fp_log;
	//FILE *fp_buffer;
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
	//if (NULL == (fp_buffer = fopen("BUFFERZONE", "rb+")))
	//{
		//fprintf(stderr, "Can not open file");
		//exit(1);
	//}
	fseek(fp_book_order, 0, SEEK_END);
	fseek(fp_log, 0, SEEK_END);
	this->setflag1('c');
	//fseek(fp_buffer, 0, SEEK_END);
	//Record record();
	//time_t timer;
	//time(&timer);
	//tm* t_tm = localtime(&timer);	//获取了当前时间，并且转换为int类型的year，month，day
	//int year = t_tm->tm_year + 1900;
	//int month = month = t_tm->tm_mon + 1;
	//int day = t_tm->tm_mday;
	//Record new_record(book, card, 'c', year, month, day, '1');
	if (fwrite(this, sizeof(Record), 1, fp_book_order) != 1)
		printf("file write error\n");
	if (fwrite(this, sizeof(Record), 1, fp_log) != 1)
		printf("file write error\n");
	//if (fwrite(&new_record, sizeof(Record), 1, fp_buffer) != 1)
	//	printf("file write error\n");
	fclose(fp_book_order);
	fclose(fp_log);
	//fclose(fp_buffer);
}

//11.1取消预约记录
void Record::bookOrderCancelRecord()
{
	FILE *fp_book_order_cancel;
	FILE *fp_log;
	if (NULL == (fp_book_lend = fopen("BOOK_ORDER_CANCEL_RECORD", "rb+")))
	{
		fprintf(stderr, "Can not open file");
		exit(1);
	}
	if (NULL == (fp_log = fopen("LOG", "rb+")))
	{
		fprintf(stderr, "Can not open file");
		exit(1);
	}
	/*if (NULL == (fp_buffer = fopen("BUFFERZONE", "rb+")))
	{
	fprintf(stderr, "Can not open file");
	exit(1);
	}*/
	fseek(fp_book_order_cancel, 0, SEEK_END);
	fseek(fp_log, 0, SEEK_END);
	this->setflag1('e');
	//fseek(fp_buffer, 0, SEEK_END);
	//Record record();
	//time_t timer;
	//time(&timer);
	//tm* t_tm = localtime(&timer);	//获取了当前时间，并且转换为int类型的year，month，day
	//int year = t_tm->tm_year + 1900;
	//int month = month = t_tm->tm_mon + 1;
	//int day = t_tm->tm_mday;
	//Record new_record(book.getBookID(), card.getCardID(), 'a', year, month, day, '0');
	if (fwrite(this, sizeof(Record), 1, fp_book_order_cancel) != 1)
		printf("file write error\n");
	if (fwrite(this, sizeof(Record), 1, fp_log) != 1)
		printf("file write error\n");
	//if (fwrite(&new_record, sizeof(Record), 1, fp_buffer) != 1)
	//printf("file write error\n");
	fclose(fp_book_order_cancel);
	fclose(fp_log);
	//fclose(fp_buffer);
}

//11.1预约失效记录
///预约失效和取消预约的文件相同
void Record::bookOrderNoRecord()
{
	FILE *fp_book_order_cancel;
	FILE *fp_log;
	if (NULL == (fp_book_lend = fopen("BOOK_ORDER_CANCEL_RECORD", "rb+")))
	{
		fprintf(stderr, "Can not open file");
		exit(1);
	}
	if (NULL == (fp_log = fopen("LOG", "rb+")))
	{
		fprintf(stderr, "Can not open file");
		exit(1);
	}
	/*if (NULL == (fp_buffer = fopen("BUFFERZONE", "rb+")))
	{
	fprintf(stderr, "Can not open file");
	exit(1);
	}*/
	fseek(fp_book_order_cancel, 0, SEEK_END);
	fseek(fp_log, 0, SEEK_END);
	this->setflag1('f');
	//fseek(fp_buffer, 0, SEEK_END);
	//Record record();
	//time_t timer;
	//time(&timer);
	//tm* t_tm = localtime(&timer);	//获取了当前时间，并且转换为int类型的year，month，day
	//int year = t_tm->tm_year + 1900;
	//int month = month = t_tm->tm_mon + 1;
	//int day = t_tm->tm_mday;
	//Record new_record(book.getBookID(), card.getCardID(), 'a', year, month, day, '0');
	if (fwrite(this, sizeof(Record), 1, fp_book_order_cancel) != 1)
		printf("file write error\n");
	if (fwrite(this, sizeof(Record), 1, fp_log) != 1)
		printf("file write error\n");
	//if (fwrite(&new_record, sizeof(Record), 1, fp_buffer) != 1)
	//printf("file write error\n");
	fclose(fp_book_order_cancel);
	fclose(fp_log);
	//fclose(fp_buffer);

}


//11.1续借记录
//由于没有取消续借功能，所以续借只存放在续借文件和大日志文件中，并未存放在缓存文件中
void Record::bookRenewRecord()
{
	FILE *fp_book_renew;
	FILE *fp_log;
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
	fseek(fp_book_renew, 0, SEEK_END);
	fseek(fp_log, 0, SEEK_END);
	this->setflag1('d');
//	Record record();
	//time_t timer;
	//time(&timer);
	//tm* t_tm = localtime(&timer);	//获取了当前时间，并且转换为int类型的year，month，day
	//int year = t_tm->tm_year + 1900;
	//int month = month = t_tm->tm_mon + 1;
	//int day = t_tm->tm_mday;
	//Record new_record(book, card, 'd', year, month, day, '1');
	if (fwrite(this, sizeof(Record), 1, fp_book_renew) != 1)
		printf("file write error\n");
	if (fwrite(this, sizeof(Record), 1, fp_log) != 1)
		printf("file write error\n");
	fclose(fp_book_renew);
	fclose(fp_log);

}

//11.1登陆记录
//登陆记录未存在缓存文件中
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
	this->setflag1('i');
	//Record record();
	//time_t timer;
	//time(&timer);
	//tm* t_tm = localtime(&timer);	//获取了当前时间，并且转换为int类型的year，month，day
	//int year = t_tm->tm_year + 1900;
	//int month = month = t_tm->tm_mon + 1;
	//int day = t_tm->tm_mday;
	//Record new_record(card, year, month, day);
	if (fwrite(&new_record, sizeof(Record), 1, fp_sign_in) != 1)
		printf("file write error\n");
	if (fwrite(&new_record, sizeof(Record), 1, fp_log) != 1)
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
	if (NULL == (fp_sign_in = fopen("SIGN_OUT_RECORD", "rb+")))
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
	this->setflag1('h');
	//Record record();
	//time_t timer;
	//time(&timer);
	//tm* t_tm = localtime(&timer);	//获取了当前时间，并且转换为int类型的year，month，day
	//int year = t_tm->tm_year + 1900;
	//int month = month = t_tm->tm_mon + 1;
	//int day = t_tm->tm_mday;
	//Record new_record(card, year, month, day);
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
	if (NULL == (fp_sign_in = fopen("SIGN_UP_RECORD", "rb+")))
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
	this->setflag1('g');
	//Record record();
	//time_t timer;
	//time(&timer);
	//tm* t_tm = localtime(&timer);	//获取了当前时间，并且转换为int类型的year，month，day
	//int year = t_tm->tm_year + 1900;
	//int month = month = t_tm->tm_mon + 1;
	//int day = t_tm->tm_mday;
	//Record new_record(card, year, month, day);
	if (fwrite(this, sizeof(Record), 1, fp_sign_up) != 1)
		printf("file write error\n");
	if (fwrite(this, sizeof(Record), 1, fp_log) != 1)
		printf("file write error\n");
	fclose(fp_sign_up);
	fclose(fp_log);
}


class Library
{
Public:
    void Library(Book book1,Card card1) {
        book = book1;
        card = card1;
    }
    void Library(Card card1) {
        card = card1;
    }
    void Library()
    {
        Book book1;
        Card card1;
        card=card1;
        book=book1;
    }
    void signInUser(char*username_PutIn, char*password_PutIn);//用户登陆
	void signInAdmin(char*adminname_PutIn, char*password_PutIn);//管理员登陆
	void signUp(char*password, char*cardHolder, char*CID, char*CPhone);//用户注册
    void signOut();//用户注销
   // void matchCid();//身份证ID匹配
	void ResetPassward(char*newpassword);//输入新密码后重设密码写入原位置
    void update();//函数用于用户进入系统时 对缓冲区进行更新
    void charge();//充值函数
    void Rcharge();//处理用户违约金
    void resetCard();//更新修改卡信息 姓名 身份证 手机

    void Search();//查询书本函数


    void bookLend();//直接进行的借书
    void bookLendOrder();//通过预约成功借书
    void bookReturn();//还书
    void bookOrder();//预约
    void bookOrderCancel();//未到期的取消预约
    void bookOrderCancelExpired();//过期了的取消预约
    void bookRenew();//续借
Private:
    Book book;
    Card card;
	Administrator admin;
};

void Library::bookLend() { //借书 1.直接借书
        Record record(book,card);
        if(card.getlendedCount()==10) {//可借本数超过上限
            cout<<"可借本书已达到上限，无法再进行借阅！"<<end;
        }
        else{//可借本数没有超过上限
            if(book.getstorage()>=2) { //库存允许
                cout<<"借阅成功"<<endl;
                book.setstorage(book.getstorage()-1);//库存-1
                card.setlendedCount(card.getlendedCount()+1);//已借本数+1
                card.setlendingCount(card.getlendingCount()-1);//可借本数-1
                record.bookLendRecord(); //生成一条借书的记录
                //写回book文件
                //写回card文件
            }
            else { //库存不够
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

void Library::bookLendOrder() {//2.通过预约成功借书
    Record record(book,card);
    card.setlendedCount(card.getlendedCount()+1);//已借本数+1
    card.setlendingCount(card.getlendingCount()-1);//可借本数-1
    card.setbookedCount(card.getbookedCount()-1);//人的预约本数-1
    //写回card文件

    book.setbookMan(book.getbookMan()-1);//书的预约人数-1
    book.settStorage(book.gettStorage()-1);//书的临时库存-1
    //写回book文件

    record.bookLendRecord(); //生成一条借书的记录

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

void Library::bookOrderCancel(){//取消预约 1.未到期取消预约
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

void Library::bookOrderCancelExpired() {//2.过期取消预约
    Record record(book,card);
    book.settStorage(book.gettStorage()-1);//书的临时库存-1
    book.setstorage(book.getstorage()+1);//书的库存+1
    book.setbookMan(book.getbookMan()-1);//书的预约人数-1
    card.setbookedCount(card.getbookedCount()-1);//人的预约本数-1
    record.bookOrderCancelRecord();//生成一条取消预约的记录
    //写回book文件

    //写回card文件

}

void Library::bookRenew(){//图书续借
    Record record(book,card);
    cout<<"续借成功"<<endl;
    record.bookRenewRecord();//生成一条续借记录
}


void Library::signInUser(char*username_PutIn, char*password_PutIn){		//用户登录
	//将用户输入的id和密码传到形参以便进行账号和密码的匹配
	/*FILE*fpEnd = fopen("BOOKINFORMATION", "rb+");	//用于标志文件的末尾，以控制查找时的循环变量的控制。
	if (fpEnd == NULL) {
		printf("file error\n");
		exit(1);
	}
	fseek(fpEnd, 0, SEEK_END);		//把fpEnd指针移到文件末尾*/
	FILE*fp = fopen("BOOKINFORMATION", "rb+");		//在循环时每一次往后移动的指针
	if (fp == NULL) {
		printf("file error\n");
		exit(1);
	}

	Card card_temp;
	int i = 0;	//循环变量，用于将fp向后移动
	while (/*fp != fpEnd*/i<allcard){
		fseek(fp, i * sizeof(Card), SEEK_SET);
		fread(&card_temp, sizeof(Card), 1, fp);
		if ((string)card_temp.getcardID() == (string)username_PutIn){	//如果找到对应的card就用复制构造函数把找到的值赋值给一个暂时的变量card_find，以便于后面的密码匹配
			Card card_find(card_temp);
			break;
		}
		i++;
	}
	if (((string)card_find.getcardID() == (string)username_PutIn) && ((string)card_find.getcPassword() == (string)password_PutIn)){
		//账号和密码匹配成功后就可以登录成功了，然后就直接把查找到的card_find赋值给私有成员card
		card(card_find);
		fclose(fp);
		return;
	}
	else {
		fclose(fp);
		return;
	}

}

void Library::signInAdmin(char*adminname_PutIn, char*password_PutIn){	//管理员登录
	//将管理员输入的id和密码传到形参以便进行账号和密码的匹配
	/*FILE*fpEnd = fopen("ADMININFORMATION", "rb+");	//用于标志文件的末尾，以控制查找时的循环变量的控制。
	if (fpEnd == NULL) {
		printf("file error\n");
		exit(1);
	}
	fseek(fpEnd, 0, SEEK_END);		//把fpEnd指针移到文件末尾*/
	FILE*fp = fopen("ADMININFORMATION", "rb+");		//在循环时每一次往后移动的指针
	if (fp == NULL) {
		printf("file error\n");
		exit(1);
	}

	Administrator admin_temp;
	int i = 0;	//循环变量，用于将fp向后移动
	while (/*fp != fpEnd*/i<alladmin){
		fseek(fp, i * sizeof(Administrator), SEEK_SET);
		fread(&admin_temp, sizeof(Administrator), 1, fp);
		if ((string)admin_temp.getAccount() == (string)adminname_PutIn){	//如果找到对应的admin就用复制构造函数把找到的值赋值给一个暂时的变量admin_find，以便于后面的密码匹配
			Administrator admin_find(admin_temp);
			break;
		}
		i++;
	}
	if (((string)admin_find.getAccount() == (string)adminname_PutIn) && ((string)admin_find.getcPassword() == (string)password_PutIn)){
		//账号和密码匹配成功后就可以登录成功了，然后就直接把查找到的admin_find赋值给私有成员admin
		admin(admin_find);
		fclose(fp);
		return;
	}
	else {
		fclose(fp);
		return;
	}
}

void Library::signUp(char*password, char*cardHolder, char*CID, char*CPhone){	//用户注册
	Card newcard((string)(1000000000 + allcard + 1), password, cardHolder, 0, CID, CPhone);
	FILE*fp_card;
	if (NULL == (fp_card = fopen("CARDINFORMATION", "rb+"))){
		fprintf(stderr, "Can not open file");
		exit(1);
	}
	fseek(fp_card, 0, SEEK_END);
	if (fwrite(&newcard, sizeof(Card), 1, fp_card) != 1)
		printf("file write error\n");
	fclose(fp_card);
	return;
}

void Library::signOut(){		//用户注销
	//把刚刚登陆时获取的card写回文件原来的位置？

}

/*void Library::matchCid(){		//身份证ID匹配

}*/

void Library::ResetPassword(char*cid, char*newpassword1, char*newpassword2){	//输入新密码后重设密码写入原位置
	if ((string)cid == (string)card.getcID){
		if ((string)newpassword1 == (string)newpassword2)
			setcPassword(newpassword1);
	}
	return;
}

void Library::update(){			//函数用于用户进入系统时 对缓冲区进行更新

}

void Library::charge(){			//充值函数

}

void Library::Rcharge(){		//处理用户违约金

}

void Library::resetCard(){		//更新修改卡信息 姓名 身份证 手机

}

