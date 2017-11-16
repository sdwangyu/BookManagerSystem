// UMLtest.cpp : �������̨Ӧ�ó������ڵ㡣
//

#include "stdafx.h"


int _tmain(int argc, _TCHAR* argv[])
{
	return 0;
}

#include<iostream>
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<time.h>
#include<windows.h>
#include<sstream>
#include <fstream>

#define ALLNUM allNum//���allcard��allbook
#define BOOKINFORMATION bookInformation//ȫ��ͼ����Ϣ
#define CARDINFORMATION cardInformation//ȫ���û���Ϣ
#define ADMININFORMATION adminInformation//ȫ������Ա��Ϣ
#define BOOK_LEND_RECORD bookLendRecord//10.30 Record�����¼��Ӧ�ļ�
#define BOOK_RENEW_RECORD bookRenewRecord//10.30 .Record�����¼��Ӧ�ļ�
#define BOOK_ORDER_RECORD bookOrderRecord//ԤԼ��¼
#define BOOK_ORDER_CANCEL_RECORD bookOrderCancelRecord//10.30 ȡ��ԤԼԤԼ���ڼ�¼��Ӧ�ļ�
#define SIGN_IN_RECORD signInRecord//10.30 Record��½��¼��Ӧ�ļ�
#define SIGN_UP_RECORD signUpRecord//ע���¼�ļ�
#define SIGN_OUT_RECORD signOutRecord//ע����¼�ļ�
#define BUFFERZONE_ORDER bufferOrderZone//ԤԼ�������ļ�
#define BUFFERZONE_LEND bufferLendZone//���黺�����ļ�
#define SEARCHRESULT searchResult//��ѯ����ļ�
#define SEARCHRESULTTEMP searchResultTmp//��ѯ�����ʱ�ļ�
#define LOG log;//ͼ��ݴ���־
#define ADMININ_ADD_BOOK admininAddBook //11.7����Ա�������¼
#define ADMININ_CHANGE_STORAGE admininChangeStorage//11.7����Ա�ı����¼�ļ�
#define BOOK_RETURN_RECORD bookReturnRecord //11.1�����¼��Ӧ�ļ�
//#define BUFFER_NEW bufferNew //��������ʱ�ļ�

using namespace std;

int allcard;//���ļ��ж�ȡ �޸ĺ�����д���ļ�  �û�ע�� ++
int allbook;//����ͼ�� ++
int alladmin;//ȫ������Ա���µĹ���Ա����ʱ++

int isLeapYear(int year){//�ж��Ƿ�������,����1Ϊ���꣬����0��������
	if (year % 4 == 0 && year % 100 != 0)return 1;
	if (year % 400 == 0)return 1;
	return 0;
}

int getDayInYear(int year, int month, int day){//�õ���������һ��ĵڼ���
	int months[12] = { 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 };
	if (isLeapYear(year))months[1] = 29;
	for (int i = 0; i<month - 1; i++){
		day = day + months[i];
	}
	return day;
}

int compareDate(int year1, int month1, int day1, int year2, int month2, int day2){//���ڱȽ�,����1year1month1day1-����2year2month2day2,
	int days = 0;
	//����Ҫ�ж����������Ƿ����ϵͳ���ڲ������32������
	if (year1 == year2){//������
		if (month1 == month2)days = day1 - day2;//�·����
		else days = getDayInYear(year1, month1, day1) - getDayInYear(year2, month2, day2);//�·ݲ���
	}
	else {//��ݲ�ͬ�������ڲ�ֵΪ����1����year1�ĵڼ���+����2��year2��ʣ�µ�����+year1��year2����֮���������ֵ
		//ȷ��year1��ݱ�year2��
		if (year1 > year2){
			if (isLeapYear(year2))
				days = 366 - getDayInYear(year2, month2, day2); //ȡ������2�ڸ��껹ʣ�¶�����
			else
				days = 365 - getDayInYear(year2, month2, day2);
			days += getDayInYear(year1, month1, day1); //ȡ������1�ڵ����еĵڼ���
			for (int year = year2 + 1; year < year1; year++)//ȡ��year1��year2֮���������ֵ
			{
				if (isLeapYear(year))
					days += 366;
				else
					days += 365;
			}
		}
		else days = -compareDate(year2, month2, day2, year1, month1, day1);	//date1С��date2�����
	}
	// if(days<0)exit(0);//�������˳�����
	return days;
}

class Book//���캯�� ���ƹ��캯��
{
public:
	Book(char BookID[10], char BookName[100], char Author[20], char Publisher[20], char Storage)//���캯��
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
		storage = Storage;//��ʼ���Ϊ10��
		bookMan = 0;//��ʼԤԼ����Ϊ0
		tStorage = 0;//��ʼԤԼ���������Ϊ0
		flag = '1';   //���б�� 0��ʾ������ 1��ʾ����//�˴���1��ʾ��ɽ�
		//books[10] = { 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 };
		books = (int*)malloc(storage*sizeof(int));		//������������Ӧ������Ŀռ��С	�ڹ���Ա�޸Ŀ���ʱ��Ӧ�����·����ڴ�
		for (int i = 0; i<storage; i++)
		{
			books[i] = 1;
		}
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
		storage = 10;//��ʼ���Ϊ10��
		bookMan = 0;//��ʼԤԼ����Ϊ0
		tStorage = 0;//��ʼԤԼ���������Ϊ0
		flag = '1';   //���б�� 0��ʾ������ 1��ʾ����//�˴���1��ʾ��ɽ�
	}

	Book(Book &book) //���ƹ��캯��
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
		for (int i = 0; i < storage; i++)
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
	int *getBooks()
	{
		return books;
	}
	void setBooks(int *bbooks)
	{
		for (int i = 0; i < storage; i++)
			books[i] = bbooks[i];
	}
	//11.10 �����޸�Books[i]�ĺ���
	void setBooksI(int i, int newbooksi){//i��ʾ��i���飬newbooksi��ʾ�µ�Books[i]��ֵ
		books[i] = newbooksi;
	}
private:
	char bookID[10];//ͼ����
	char bookName[100];//����
	char author[20];//����
	char publisher[20];//������
	short storage; //���
	short bookMan; //ԤԼ����
	short tStorage;  //��ʱ���
	char flag;  //ͼ���Ƿ����
	int *books;//���10���飬������ÿһ��������ʾ10���о���ĳһ����״̬��0���� 1���ɽ� 2�����		��ʼֵȫ����Ϊ1
	//��̬���ٴ洢�ռ�?



	friend class Administrator;//��BOOK����Ϊ����Ա�����Ԫ�࣬�������Ա���еĸĿ�溯���޷�����tStorage˽�б���
};

class Card//���캯��  ���ƹ��캯��
{
public:
	Card(char CardID[10], char CPassword[20], char CardHolder[10], double Balance, char CID[18], char CPhone[11]){//���캯��
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
		lendedCount = 0;//��ʼ�ѽ豾��Ϊ0
		lendingCount = 10;//��ʼ�ɽ豾��Ϊ10
		cardState = '1';//1��ʾδ����
		balance = Balance;
		oweMoney = 0;
		bookedCount = lendingCount - lendedCount;//��ʼԤԼ����Ϊ0
		for (int i = 0; i<18; i++)
		{
			cID[i] = CID[i];
		}
		for (int i = 0; i<11; i++)
		{
			cPhone[i] = CPhone[i];
		}
	}
	Card(Card &card){//���ƹ��캯��
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
		for (int i = 0; i<18; i++)
		{
			cID[i] = card.cID[i];
		}
		for (int i = 0; i<11; i++)
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
		for (i = 0; i<18; i++)
		{
			cID[i] = ' ';
		}
		for (i = 0; i<11; i++)
		{
			cPhone[11] = ' ';
		}
		lendedCount = 0;//��ʼ�ѽ豾��Ϊ0
		lendingCount = 10;//��ʼ�ɽ豾��Ϊ10
		cardState = '1';//1��ʾδ����
		oweMoney = 0;
		bookedCount = 0;//��ʼԤԼ����Ϊ0
	}

	char *getcardID(){
		return cardID;
	}
	void setcardID(char newcardID[10]){
		for (int i = 0; i<10; i++)
		{
			cardID[i] = newcardID[i];
		}
	}
	char *getcPassword(){
		return cPassword;
	}
	void setcPassword(char newcPassword[20]){
		for (int i = 0; i<20; i++)
		{
			cPassword[i] = newcPassword[i];
		}
	}
	short getlendedCount(){
		return lendedCount;
	}
	void setlendedCount(short newlendedCount){
		lendedCount = newlendedCount;
	}
	short getlendingCount(){
		return lendingCount;
	}
	void setlendingCount(short newlendingCount){
		lendingCount = newlendingCount;
	}
	char *getcardHolder(){
		return cardHolder;
	}
	void setcardHolder(char newcardHolder[10]){
		for (int i = 0; i<10; i++)
		{
			cardHolder[i] = newcardHolder[i];
		}
	}
	char getcardState(){
		return cardState;
	}
	void setcardState(char newcardState){
		cardState = newcardState;
	}
	double getbalance(){
		return balance;
	}
	void setbalance(double newbalance){
		balance = newbalance;
	}
	double getoweMoney(){
		return oweMoney;
	}
	void setoweMoney(double newoweMoney){
		oweMoney = newoweMoney;
	}
	short getbookedCount(){
		return bookedCount;
	}
	void setbookedCount(short newbookedCount){
		bookedCount = newbookedCount;
	}
	char *getcID(){
		return cID;
	}
	void setcID(char newcID[18]){
		for (int i = 0; i<18; i++)
		{
			cID[i] = newcID[i];
		}
	}
	char *getcPhone(){
		return cPhone;
	}
	void setcPhone(char newcPhone[11]){
		for (int i = 0; i<11; i++)
		{
			cPhone[i] = newcPhone[i];
		}
	}
private:

	char cardID[10];//����
	char cPassword[20];//����
	short lendedCount;//�ѽ豾��
	short lendingCount;//�ɽ豾��
	char cardHolder[10];//�ֿ�������
	char cardState;   //�˺Ŷ���״̬
	double balance;//���
	double oweMoney;   //ΥԼ��
	short bookedCount;//ԤԼ����������ԤԼ������  �ѽ�+�ɽ�+ԤԼ=10 ��ʾ���ɽ費��ԤԼ
	char cID[18];  //���֤��
	char cPhone[11];//�ֿ����ֻ���
};

class Administrator
{
public:
	//11.1���캯��
	Administrator(char Account[5], char APassword[20], char AccountHolder[10], char AID[18], char APhone[11])//���캯��
	{

		for (int i = 0; i<5; i++)
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
		for (int i = 0; i<18; i++)
		{
			aID[i] = ' ';
		}
		for (int i = 0; i<11; i++)
		{
			aPhone[i] = ' ';
		}
	}

	//���ƹ��캯��
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
		for (int i = 0; i<18; i++)
		{
			aID[i] = administrator.aID[i];
		}
		for (int i = 0; i<11; i++)
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

	void setaID(char newaID[18])
	{
		for (int i = 0; i<18; i++)
		{
			aID[i] = newaID[i];
		}
	}
	void setaPhone(char newaaPhone[11])
	{
		for (int i = 0; i<11; i++)
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

	void addBook(char *bookID, char *bookName, char *author, char *publisher, short storage);//������

	//void deleteBook(Book book);��ʦ˵��Ҫɾ��
	//void operateCard(Card card);��ʦ˵��Ҫɾ�� ����ʦ��

	void findbook(char*bookid);		//Ϊ�����޸Ŀ��֮ǰ�����е�˽�г�Աbook��ֵ
	void newStorage(short storage);//������
	void addadmin(char*aPassword, char*accountHolder, char*aID, char*aPhone);

	//11.9����Ա���в鿴��Ϊ����
	void searchLog();//����Ա�鿴����־
	void searchLendBuffer();//����Ա�鿴�������������
	void serchBookInformation();//����Ա�鿴ȫ��ͼ����Ϣ
	void searchCardInformation();//����Ա�鿴ȫ���û���Ϣ
	void searchOrder();//����Ա�鿴����ԤԼ��¼
	void searchCancleOrder();//����Ա�鿴ȫ��ԤԼȡ��ԤԼ���ڼ�¼�ļ�
	void searchReturn();//����Ա�鿴���л�����Ϣ


private:
	char account[5];		//�ǵ��޸������Ӧ����ֵ
	char aPassword[20];
	char accountHolder[10];
	char aID[18];
	char aPhone[11];
	Book book;
};

//����Աע�ắ��

void Administrator::addadmin(char*aPassword, char*accountHolder, char*aID, char*aPhone){
	string account_str = to_string(20000 + alladmin + 1);
	char account[5];
	strcpy(account,account_str.c_str());
	Administrator newadministrator(account, aPassword, accountHolder, aID, aPhone);
	FILE*fp_admin;
	if (NULL == (fp_admin = fopen("ADMININFORMATION", "rb+"))){
		fprintf(stderr, "Can not open file");
		exit(1);
	}
	fseek(fp_admin, 0, SEEK_END);
	if (fwrite(&newadministrator, sizeof(Administrator), 1, fp_admin) != 1)
		printf("file write error\n");
	time_t timer;
	time(&timer);
	tm* t_tm = localtime(&timer);	//��ȡ�˵�ǰʱ�䣬����ת��Ϊint���͵�year��month��day
	int year = t_tm->tm_year + 1900;
	int month = month = t_tm->tm_mon + 1;
	int day = t_tm->tm_mday;
	Record record(newadministrator.getaID(), year, month, day, 'l');
	record.signUpRecord();
	fclose(fp_admin);
	alladmin++;
	return;
}
//����Ա�鿴����־
void Administrator::searchLog()
{
	FILE *fp_log;
	if (NULL == (fp_log = fopen("LOG", "rb+")))
	{
		fprintf(stderr, "Can not open file");
		exit(1);
	}

}

//11.2����Ա�¼��麯��
void Administrator::addBook(char *bookID, char *bookName, char *author, char *publisher, short storage)
{
	FILE *fp_add_book;
	FILE *fp_book;
	char bookID[10];
	char bookName[50];
	char author[20];
	char publisher[20];
	short storage;
	short bookMan;
	short tStorage;
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
	Book book(bookID, bookName, author, publisher, storage);
	fwrite(&book, sizeof(Book), 1, fp_book);
	allbook++;
	time_t timer;
	time(&timer);
	tm* t_tm = localtime(&timer);	//��ȡ�˵�ǰʱ�䣬����ת��Ϊint���͵�year��month��day
	int year = t_tm->tm_year + 1900;
	int month = month = t_tm->tm_mon + 1;
	int day = t_tm->tm_mday;
	Record record(book.getbookID(), this->getaccount(), year, month, day, 'j', '0');
	record.admininaddbook();
	fclose(fp_add_book);
	fclose(fp_book);

}

//�ڹ���Ա�޸Ŀ��֮ǰ������������Ҫ�޸ĵ����bookid��Ȼ�����findbook�����ҵ��Ȿ�飬��ֵ��record���˽�г�Աbook
//Ȼ�����������޸ĺ������ܱ���������޸Ŀ�水ť��ִ��newStorage��������
void Administrator::findbook(char*bookid)
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
		if ((string)booktemp.getbookID == (string)bookid)
		{
			book(booktemp);
			break;
		}
	}
}
//11.2����Ա�Ŀ�溯��
void Administrator::newStorage(short storage)
{

	time_t timer;
	time(&timer);
	tm* t_tm = localtime(&timer);	//��ȡ�˵�ǰʱ�䣬����ת��Ϊint���͵�year��month��day
	int year = t_tm->tm_year + 1900;
	int month = month = t_tm->tm_mon + 1;
	int day = t_tm->tm_mday;
	int oldstorage = book.getstorage();
	book.setstorage(storage);
	book.books = (int *)realloc(books, storage*sizeof(int));		//�޸Ŀ��֮����Ҫ���¸�books��̬�����ڴ�
	if (storage > oldstorage){			//����������ˣ�����books���鲻�ý����κβ��������������������ˣ��ͱ�����·����books���ڴ渳ֵΪ1
		for (int i = oldstorage; i < storage; i++)
			books[i] = 1;
	}
	Record record(book.getbookID(), this->getaccount(), year, month, day, 'k', '0');
	record.admininchangestorage();
	FILE *fp_book;
	if (NULL == (fp_book = fopen("BOOKINFORMATION", "rb+")))
	{
		fprintf(stderr, "Can not open file");
		exit(1);
	}
	int position = atoi(book.getbookID()) - 100000000 - 1;
	fseek(fp_book, position * sizeof(Book), SEEK_SET);
	if (fwrite(&book, sizeof(Book), 1, fp_book) != 1)
		printf("file write error\n");
}

class Record
{
public:
	//10.30 ���캯������
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
		//��ȡ��ǰϵͳ���� ���в�ѯ���� ���뵱ǰyear month day
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

	//����ͬѧ��Ҫ�Ĺ��캯������~~
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

	//Ĭ�Ϲ��캯��
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
		flag2 = 1;//���ڻ�����   1��ԤԼ��¼��ʾ��ԤԼʧЧ�����Ѿ�д���¼�ļ� 1�������¼��ʾ����������
		order = 1;
		flag1 = '1';   //���б�� 0��ʾ������ 1��ʾ����//�˴���1��ʾ��ɽ�
	}
	//���ƹ��캯��
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
		flag2 = R.flag2;//���ڻ�����   1��ԤԼ��¼��ʾ��ԤԼʧЧ�����Ѿ�д���¼�ļ� 1�������¼��ʾ����������
		order = R.order;
		flag1 = R.flag1;
	}

	void bookLendRecord(int flag);//�����¼
	void bookReturnRecord();//�����¼
	void bookOrderRecord();//ԤԼ��¼
	void bookRenewRecord();//�����¼
	void bookOrderCancelRecord();//ȡ��ԤԼ��¼
	void bookOrderNoRecord();//ԤԼʧЧ��¼
	void signInRecord();//��½��¼
	void signOutRecord();//ע����¼
	void signUpRecord();//ע���¼
	void admininchangestorage();//����Ա�ı����¼
	void admininaddbook();//����Ա���������¼
	void alter_Date(int day);//����һ�����ڱ仯�ĺ���

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
	char flag1;  //a���� b���� cԤԼ d���� eȡ��ԤԼ fԤԼʧЧ gע���¼ hע����¼ i��½��¼ j����Ա������ k����Ա���Ŀ�� l����Աע��
	char bookid[10];
	char cardid[10];
	int year;
	int month;
	int day;
	char flag2;//���ڻ�����   1��ԤԼ��¼��ʾ��ԤԼʧЧ�����Ѿ�д���¼�ļ� 1�������¼��ʾ����������
	int order;//��ʶ�ڼ�����
};

//Record���ڲ�������ʵ��

//11.04����һ�����ڱ仯�ĺ���
void Record::alter_Date(int addday){
	int tempday = 0;
	int monthday[12] = { 31, 0, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 };//�����ÿ���µ������Է���������
	if (isLeapYear(year))monthday[1] = 29;//�����2��
	else monthday[1] = 28;//�������2��
	tempday = day + addday;//ͨ�����������������������ķ�������ȡ�µ�����
	while (tempday>monthday[month - 1]){
		tempday = tempday - monthday[month - 1];
		month++;
		if (month > 12){
			month = month - 12;
			if (isLeapYear(++year))monthday[1] = 29;
			else monthday[1] = 28;
		}
	}
	day = tempday;
}

//11.7���ӹ���Ա�ı������¼
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
	if (fwrite(this, sizeof(Record), 1, fp_change_storage) != 1)
		printf("file write error\n");
	if (fwrite(this, sizeof(Record), 1, fp_log) != 1)
		printf("file write error\n");
	fclose(fp_change_storage);
	fclose(fp_log);
}

//11.7���ӹ���Ա������Ŀ�ļ�¼
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
	if (fwrite(this, sizeof(Record), 1, fp_add_book) != 1)
		printf("file write error\n");
	if (fwrite(this, sizeof(Record), 1, fp_log) != 1)
		printf("file write error\n");
	fclose(fp_add_book);
	fclose(fp_log);
}

//10.31�����¼

void Record::bookLendRecord(int flag)		//�����¼
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
		if (fwrite(this, sizeof(Record), 1, fp_book_lend) != 1)
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
		if (remove("bufferOrderZone") != 0)exit(1);
		if (rename("bufferzone_ordernew", "bufferOrderZone") != 0)exit(1);

	}

}

//11.1�����¼
//�����¼�ʹ��¼��д��һ�������¼������buffer�ļ����ҳ���Ӧ�����¼���������¼ɾ����
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
		if ((string)record_temp.getBookid() == (string)this->getBookid() && (string)record_temp.getCardid() == (string)this->getCardid() && record_temp.getorder() == this->getorder)continue;
		fwrite(&record_temp, sizeof(Record), 1, fp_lend_buffernew);
	}
	fclose(fp_lend_buffer);
	fclose(fp_lend_buffernew);
	if (remove("bufferLendZone") != 0)exit(1);
	if (rename("bufferzone_lendnew", "bufferLendZone") != 0)exit(1);
	fseek(fp_book_return, 0, SEEK_END);
	fseek(fp_log, 0, SEEK_END);
	if (fwrite(this, sizeof(Record), 1, fp_book_return) != 1)
		printf("file write error\n");
	if (fwrite(this, sizeof(Record), 1, fp_log) != 1)
		printf("file write error\n");
	fclose(fp_book_return);
	fclose(fp_log);

}


//11.1ԤԼ��¼
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

//11.1ȡ��ԤԼ��¼
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
		if ((string)record_temp.getBookid() == (string)this->getBookid() && (string)record_temp.getCardid() == (string)this->getCardid() && record_temp.getorder() == this->getorder)continue;
		fwrite(&record_temp, sizeof(Record), 1, fp_order_buffernew);
	}
	fclose(fp_order_buffer);
	fclose(fp_order_buffernew);
	if (remove("bufferOrderZone") != 0)exit(1);
	if (rename("bufferzone_ordernew", "bufferOrderZone") != 0)exit(1);

	fseek(fp_book_order_cancel, 0, SEEK_END);
	fseek(fp_log, 0, SEEK_END);
	if (fwrite(this, sizeof(Record), 1, fp_book_order_cancel) != 1)
		printf("file write error\n");
	if (fwrite(this, sizeof(Record), 1, fp_log) != 1)
		printf("file write error\n");
	fclose(fp_book_order_cancel);
	fclose(fp_log);
}

//11.1ԤԼʧЧ��¼
///ԤԼʧЧ��ȡ��ԤԼ���ļ���ͬ
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
		if ((string)record_temp.getBookid() == (string)this->getBookid() && (string)record_temp.getCardid() == (string)this->getCardid() && record_temp.getorder() == this->getorder)continue;
		fwrite(&record_temp, sizeof(Record), 1, fp_noorder_buffernew);
	}
	fclose(fp_noorder_buffer);
	fclose(fp_noorder_buffernew);
	if (remove("bufferLendZone") != 0)exit(1);
	if (rename("bufferzone_lendnew", "bufferLendZone") != 0) exit(1);

	fseek(fp_book_order_cancel, 0, SEEK_END);
	fseek(fp_log, 0, SEEK_END);
	if (fwrite(this, sizeof(Record), 1, fp_book_order_cancel) != 1)
		printf("file write error\n");
	if (fwrite(this, sizeof(Record), 1, fp_log) != 1)
		printf("file write error\n");
	fclose(fp_book_order_cancel);
	fclose(fp_log);
}


//11.1�����¼
//����û��ȡ�����蹦�ܣ���������ֻ����������ļ��ʹ���־�ļ��У���δ����ڻ����ļ���

//����Ҳ��Ҫд��bufferlend�����ļ����Ϊ�����ʱ����Ҫ��bufferlend�м�������
//ŷ��˵�Ķ�������~~mua~~
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
		if ((string)record_temp.getBookid() == (string)this->getBookid() && (string)record_temp.getCardid() == (string)this->getCardid() && record_temp.getorder() == this->getorder)continue;
		fwrite(&record_temp, sizeof(Record), 1, fp_new_buffer_lend);
	}
	fclose(fp_buffer);
	fclose(fp_new_buffer_lend);
	if (remove("bufferLendZone") != 0)exit(1);
	if (rename("bufferzone_lendnew", "bufferLendZone") != 0)exit(1);
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

//11.1��½��¼
//��½��¼δ���ڻ����ļ���
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

//11.1ע����¼
//δ��¼�ڻ����ļ���
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

//11.1ע���¼
//δ�Ž������ļ���
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
	this->setflag1('g');
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
	void signInUser(char*username_PutIn, char*password_PutIn);//�û���½
	void signInAdmin(char*adminname_PutIn, char*password_PutIn);//����Ա��½
	void signUp(char*password, char*cardHolder, char*CID, char*CPhone);//�û�ע��
	void signOut();//�û�ע��
	void signOut_Admin();//����Աע��
	// void matchCid();//���֤IDƥ��
	void ResetPassword(char*oldpassword, char*newpassword1, char*newpassword2);//�������������������д��ԭλ��
	void update_Order();//���������û�����ϵͳʱ �Ի��������и���
	void update_book();//���������ڵ�½���ж��û����ѽ��鼮�Ƿ��Ѿ�����

	void charge(double money);//��ֵ����
	void Rcharge();//�����û�ΥԼ��
	// void resetCard();//�����޸Ŀ���Ϣ �ֻ�

	void Search();//��ѯ�鱾����


	void bookLend();//ֱ�ӽ��еĽ���
	void bookLendOrder();//ͨ��ԤԼ�ɹ�����
	void bookReturn();//����
	void bookOrder();//ԤԼ
	void bookOrderCancel();//δ���ڵ�ȡ��ԤԼ
	void bookRenew();//����
private:
	Book book;
	Card card;
	Administrator admin;
};
fstream iofile;
void Library::bookLend() { //���� 1.ֱ�ӽ���
	if (card.getlendedCount() == 10) {//�ɽ豾����������
		cout << "�ɽ豾���Ѵﵽ���ޣ��޷��ٽ��н��ģ�" << endl;
	}
	else{//�ɽ豾��û�г�������
		if (book.getstorage() >= 2) { //�������
			cout << "���ĳɹ�" << endl;
			book.setstorage(book.getstorage() - 1);//���-1
			card.setlendedCount(card.getlendedCount() + 1);//�ѽ豾��+1
			card.setlendingCount(card.getlendingCount() - 1);//�ɽ豾��-1
			int order = 1;//��ʶ�ڼ�����
			int *p = book.getBooks();
			while (!(*(p + order) == 1)) {//�ӵ�һ���鿪ʼ���������ǵ�0��
				order++;
			}
			book.setBooksI(order, 2);//���Ȿ���Ϊ�ѽ��
			//����һ������ļ�¼
			time_t timer;
			time(&timer);
			tm* t_tm = localtime(&timer);	//��ȡ�˵�ǰʱ�䣬����ת��Ϊint���͵�year��month��day
			int year = t_tm->tm_year + 1900;
			int month = month = t_tm->tm_mon + 1;
			int day = t_tm->tm_mday;
			Record record(book.getbookID(), card.getcardID(), year, month, day, 'a', '0', order);//����һ������ļ�¼
			record.alter_Date(30);	//����30�죬��Ӧ������д����¼
			record.bookLendRecord(0);
			//д��book�ļ�
			ofstream outfile("BOOKINFORMATION", ios::binary);
			if (!outfile)
			{
				cerr << "open error!" << endl;
				abort();//�˳�����
			}
			int number = 0;//�ڼ�����
			char *p = book.getbookID();
			for (int i = 0; i<10; i++) {
				int a = 1;
				for (int j = i; j>0; j--) {
					a *= 10;
				}
				number += *(p + i)*a;
			}
			iofile.seekp(number*sizeof(book), ios::beg);  //��λ�ڵڼ�����Ŀ�ͷ
			iofile.write((char *)&book, sizeof(book));  //���µڼ����������
			outfile.close();
		}
		else { //��治��
			int choice;
			cout << "��治��������ʧ�ܣ�" << endl;//����ʧ��
			cout << "�Ƿ����ԤԼ��" << endl;
			cout << "1.��   2.��" << endl;//��ʾ���
			cin >> choice;
			while (1) {
				if (choice == 1) {
					bookOrder();
					break;
				}
				else if (choice == 2) {
					//�ر���ʾ��ٷ��ز�ѯ����
					break;
				}
				else {
					cout << "�����������������룡" << endl;
					cout << "�Ƿ����ԤԼ��" << endl;
					cout << "1.��   2.��" << endl;//��ʾ���
					cin >> choice;
					//�ر���ʾ���
				}
			}
		}
	}
}

void Library::bookLendOrder() {//2.ͨ��ԤԼ�ɹ�����
	time_t timer;
	time(&timer);
	tm* t_tm = localtime(&timer);	//��ȡ�˵�ǰʱ�䣬����ת��Ϊint���͵�year��month��day
	int year = t_tm->tm_year + 1900;
	int month = month = t_tm->tm_mon + 1;
	int day = t_tm->tm_mday;
	card.setlendedCount(card.getlendedCount() + 1);//�ѽ豾��+1
	card.setlendingCount(card.getlendingCount() - 1);//�ɽ豾��-1
	card.setbookedCount(card.getbookedCount() - 1);//�˵�ԤԼ����-1

	book.setbookMan(book.getbookMan() - 1);//���ԤԼ����-1
	book.settStorage(book.gettStorage() - 1);//�����ʱ���-1
	int order = 1;//��ʶ�ڼ�����
	int *p = book.getBooks();
	while (!(*(p + order) == 1)) {//�ӵ�һ���鿪ʼ����
		order++;
	}
	book.setBooksI(order, 2);//���Ȿ���Ϊ�ѽ��
	Record record(book.getbookID(), card.getcardID(), year, month, day, 'a', '0', order);//����һ������ļ�¼
	record.alter_Date(30);
	record.bookLendRecord(1);
	//д��book�ļ�
	ofstream outfile("BOOKINFORMATION", ios::binary);
	if (!outfile)
	{
		cerr << "open error!" << endl;
		abort();//�˳�����
	}
	int number = 0;//�ڼ�����
	char *p = book.getbookID();
	for (int i = 0; i<10; i++) {
		int a = 1;
		for (int j = i; j>0; j--) {
			a *= 10;
		}
		number += *(p + i)*a;
	}
	iofile.seekp(number*sizeof(book), ios::beg);  //��λ�ڵڼ�����Ŀ�ͷ
	iofile.write((char *)&book, sizeof(book));  //���µڼ����������
	outfile.close();
}

void Library::bookReturn(){ //���飨��Ҫ�õ�qt��
	cout << "����ɹ���" << endl;
	time_t timer;
	time(&timer);
	tm* t_tm = localtime(&timer);	//��ȡ�˵�ǰʱ�䣬����ת��Ϊint���͵�year��month��day
	int year = t_tm->tm_year + 1900;
	int month = month = t_tm->tm_mon + 1;
	int day = t_tm->tm_mday;
	card.setlendedCount(card.getlendedCount() - 1);//�ѽ豾��-1
	card.setlendingCount(card.getlendingCount() + 1);//�ɽ豾��+1
	//1.����Ȿ���Ƿ�����ԤԼ
	if (book.getbookMan()>0) {
		//1.1������ԤԼ
		if (book.gettStorage() == book.getbookMan()) {//�����ʱ����Ƿ����ԤԼ����������������+1
			book.setstorage(book.getstorage() + 1);
		}
		else {//�������ڣ���ʱ���+1
			book.settStorage(book.gettStorage() + 1);
		}
	}
	else {
		//1.2������ԤԼ�����+1
		book.setstorage(book.getstorage() + 1);
	}
	//��order��Ϊ1�ɽ�
	/*
	ifstream infile("BUFFERZONE_LEND",ios::binary);
	if(!infile)
	{
	cerr<<"open error!"<<endl;
	abort( );
	}
	char bookID[10];//���ڴ�����ļ��ж�������ı��
	Record record;//record�Ĵ�С���⣬Ĭ�Ϲ��캯��
	int number;//�ڼ�����¼
	for(number=0;;number++) {
	iofile.seekg(i*sizeof(record,ios::beg);
	iofile.read((char *)&bookID[10],sizeof(bookID[10]));
	if(strcmp(book.getbookID(),bookID)==0) {
	break;
	}
	}
	iofile.seekg(number*sizeof(record,ios::beg);
	iofile.read((char *)&record,sizeof(record);
	book.setBooksI(record.getOrder(),1);
	*/
	//д��book�ļ�
	ofstream outfile("BOOKINFORMATION", ios::binary);
	if (!outfile)
	{
		cerr << "open error!" << endl;
		abort();//�˳�����
	}
	int number = 0;//�ڼ�����
	char *p = book.getbookID();
	for (int i = 0; i<10; i++) {
		int a = 1;
		for (int j = i; j>0; j--) {
			a *= 10;
		}
		number += *(p + i)*a;
	}
	iofile.seekp(number*sizeof(book), ios::beg);  //��λ�ڵڼ�����Ŀ�ͷ
	iofile.write((char *)&book, sizeof(book));  //���µڼ����������
	outfile.close();
	Record record(book.getbookID(), card.getcardID(), year, month, day, 'b', '0');
	record.bookReturnRecord();//����һ�������¼
}

void Library::bookOrder(){//ԤԼ
	time_t timer;
	time(&timer);
	tm* t_tm = localtime(&timer);	//��ȡ�˵�ǰʱ�䣬����ת��Ϊint���͵�year��month��day
	int year = t_tm->tm_year + 1900;
	int month = month = t_tm->tm_mon + 1;
	int day = t_tm->tm_mday;
	Record record(book.getbookID(), card.getcardID(), year, month, day, 'c', '0');

	//ԤԼ��¼�ͼ�¼ԤԼʱ�伴�ɣ���ΪΪ�˷�����update_order��ʹ��
	if (card.getbookedCount() == 5) {//ԤԼ�����Ѵ�����
		cout << "����ԤԼ�����Ѵ����ޣ��޷�����ԤԼ��" << endl;
	}
	else{
		cout << "ԤԼ�ɹ���" << endl;//��ʾԤԼ�ɹ�
		book.setbookMan(book.getbookMan() + 1);//���ԤԼ����+1
		card.setbookedCount(card.getbookedCount() + 1);//�˵�ԤԼ����+1
		//д��book�ļ�
		ofstream outfile("BOOKINFORMATION", ios::binary);
		if (!outfile)
		{
			cerr << "open error!" << endl;
			abort();//�˳�����
		}
		int number = 0;//�ڼ�����
		char *p = book.getbookID();
		for (int i = 0; i<10; i++) {
			int a = 1;
			for (int j = i; j>0; j--) {
				a *= 10;
			}
			number += *(p + i)*a;
		}
		iofile.seekp(number*sizeof(book), ios::beg);  //��λ�ڵڼ�����Ŀ�ͷ
		iofile.write((char *)&book, sizeof(book));  //���µڼ����������
		outfile.close();
		//����һ��ԤԼ��¼
		time_t timer;
		time(&timer);
		tm* t_tm = localtime(&timer);	//��ȡ�˵�ǰʱ�䣬����ת��Ϊint���͵�year��month��day
		int year = t_tm->tm_year + 1900;
		int month = month = t_tm->tm_mon + 1;
		int day = t_tm->tm_mday;
		Record record(book.getbookID(), card.getcardID(), year, month, day, 'c', '0');
		record.bookOrderRecord();
	}
}

void Library::bookOrderCancel(){//ȡ��ԤԼ 1.δ����ȡ��ԤԼ
	// Record record(book.getBookID(), card.getcardID(), year, month, day, 'e', '0');
	int choice;
	cout << "ȷ��ȡ��ԤԼ��" << endl;
	cout << "1.�� 2.��" << endl;
	cin >> choice;
	while (1) {
		if (choice == 1) {//1.
			cout << "�ɹ�ȡ��ԤԼ��" << endl;
			if (book.getbookMan() == book.gettStorage()) { //��ȡ��ԤԼʱ��ʱ
				book.settStorage(book.gettStorage() + 1);//��ʱ���-1
				book.setstorage(book.getstorage() + 1);//���+1
			}
			book.setbookMan(book.getbookMan() - 1);//�����ԤԼ����-1
			card.setbookedCount(card.getbookedCount() - 1);//���˵�ԤԼ����-1
			//����һ��ȡ��ԤԼ�ļ�¼
			time_t timer;
			time(&timer);
			tm* t_tm = localtime(&timer);	//��ȡ�˵�ǰʱ�䣬����ת��Ϊint���͵�year��month��day
			int year = t_tm->tm_year + 1900;
			int month = month = t_tm->tm_mon + 1;
			int day = t_tm->tm_mday;
			Record record(book.getbookID(), card.getcardID(), year, month, day, 'e', '0');
			record.bookOrderCancelRecord();
			//д��book�ļ�
			ofstream outfile("BOOKINFORMATION", ios::binary);
			if (!outfile)
			{
				cerr << "open error!" << endl;
				abort();//�˳�����
			}
			int number = 0;//�ڼ�����
			char *p = book.getbookID();
			for (int i = 0; i<10; i++) {
				int a = 1;
				for (int j = i; j>0; j--) {
					a *= 10;
				}
				number += *(p + i)*a;
			}
			iofile.seekp(number*sizeof(book), ios::beg);  //��λ�ڵڼ�����Ŀ�ͷ
			iofile.write((char *)&book, sizeof(book));  //���µڼ����������
			outfile.close();
			break;
		}
		else if (choice == 2) {//2.��
			//���ؽ�����Ϣ����
			break;
		}
		else {//��������
			cout << "�����������������룡" << endl;
			cout << "ȷ��ȡ��ԤԼ��" << endl;
			cout << "1.��   2.��" << endl;
			cin >> choice;
		}
	}
}

void Library::bookRenew(){//ͼ�����裨��Ҫ�õ�qt��
	time_t timer;
	time(&timer);
	tm* t_tm = localtime(&timer);	//��ȡ�˵�ǰʱ�䣬����ת��Ϊint���͵�year��month��day
	int year = t_tm->tm_year + 1900;
	int month = month = t_tm->tm_mon + 1;
	int day = t_tm->tm_mday;
	Record record(book.getbookID(), card.getcardID(), year, month, day, 'd', '1');
	cout << "����ɹ�" << endl;
	record.alter_Date(30);		//����30�죬��Ӧ������д����¼
	record.bookRenewRecord();//����һ�������¼
}


void Library::signInUser(char*username_PutIn, char*password_PutIn){		//�û���¼
	//���û������id�����봫���β��Ա�����˺ź������ƥ��
	/*FILE*fpEnd = fopen("BOOKINFORMATION", "rb+");	//���ڱ�־�ļ���ĩβ���Կ��Ʋ���ʱ��ѭ�������Ŀ��ơ�
	if (fpEnd == NULL) {
	printf("file error\n");
	exit(1);
	}
	fseek(fpEnd, 0, SEEK_END);		//��fpEndָ���Ƶ��ļ�ĩβ*/
	FILE *fp = fopen("BOOKINFORMATION", "rb+");		//��ѭ��ʱÿһ�������ƶ���ָ��
	if (fp == NULL) {
		printf("file error\n");
		exit(1);
	}
	Card card_find;
	Card card_temp;
	int i = 0;	//ѭ�����������ڽ�fp����ƶ�
	while (/*fp != fpEnd*/i<allcard){
		fseek(fp, i * sizeof(Card), SEEK_SET);
		fread(&card_temp, sizeof(Card), 1, fp);
		if ((string)card_temp.getcardID() == (string)username_PutIn){	//����ҵ���Ӧ��card���ø��ƹ��캯�����ҵ���ֵ��ֵ��һ����ʱ�ı���card_find���Ա��ں��������ƥ��
			card_find(card_temp);
			break;
		}
		i++;
	}
	if (((string)card_find.getcardID() == (string)username_PutIn) && ((string)card_find.getcPassword() == (string)password_PutIn)){
		//�˺ź�����ƥ��ɹ���Ϳ��Ե�¼�ɹ��ˣ�Ȼ���ֱ�ӰѲ��ҵ���card_find��ֵ��˽�г�Աcard
		card(card_find);
		time_t timer;
		time(&timer);
		tm* t_tm = localtime(&timer);	//��ȡ�˵�ǰʱ�䣬����ת��Ϊint���͵�year��month��day
		int year = t_tm->tm_year + 1900;
		int month = month = t_tm->tm_mon + 1;
		int day = t_tm->tm_mday;
		Record record(card.getcardID(), year, month, day, 'i');
		record.signInRecord();
		fclose(fp);
		return;
	}
	else {
		fclose(fp);
		return;
	}

}

void Library::signInAdmin(char*adminname_PutIn, char*password_PutIn){	//����Ա��¼
	//������Ա�����id�����봫���β��Ա�����˺ź������ƥ��
	/*FILE*fpEnd = fopen("ADMININFORMATION", "rb+");	//���ڱ�־�ļ���ĩβ���Կ��Ʋ���ʱ��ѭ�������Ŀ��ơ�
	if (fpEnd == NULL) {
	printf("file error\n");
	exit(1);
	}
	fseek(fpEnd, 0, SEEK_END);		//��fpEndָ���Ƶ��ļ�ĩβ*/
	FILE*fp = fopen("ADMININFORMATION", "rb+");		//��ѭ��ʱÿһ�������ƶ���ָ��
	if (fp == NULL) {
		printf("file error\n");
		exit(1);
	}
	Administrator admin_find;
	Administrator admin_temp;
	int i = 0;	//ѭ�����������ڽ�fp����ƶ�
	while (/*fp != fpEnd*/i<alladmin){
		fseek(fp, i * sizeof(Administrator), SEEK_SET);
		fread(&admin_temp, sizeof(Administrator), 1, fp);
		if ((string)admin_temp.getaccount() == (string)adminname_PutIn){	//����ҵ���Ӧ��admin���ø��ƹ��캯�����ҵ���ֵ��ֵ��һ����ʱ�ı���admin_find���Ա��ں��������ƥ��
			admin_find(admin_temp);
			break;
		}
		i++;
	}
	if (((string)admin_find.getaccount() == (string)adminname_PutIn) && ((string)admin_find.getaPassword() == (string)password_PutIn)){
		//�˺ź�����ƥ��ɹ���Ϳ��Ե�¼�ɹ��ˣ�Ȼ���ֱ�ӰѲ��ҵ���admin_find��ֵ��˽�г�Աadmin
		admin(admin_find);
		time_t timer;
		time(&timer);
		tm* t_tm = localtime(&timer);	//��ȡ�˵�ǰʱ�䣬����ת��Ϊint���͵�year��month��day
		int year = t_tm->tm_year + 1900;
		int month = month = t_tm->tm_mon + 1;
		int day = t_tm->tm_mday;
		Record record(admin.getaccount(), year, month, day, 'i');
		record.signInRecord();
		fclose(fp);
		return;
	}
	else {
		fclose(fp);
		return;
	}
}

void Library::signUp(char*password, char*cardHolder, char*CID, char*CPhone){	//�û�ע��
	string account_str = to_string(1000000000 + allcard + 1);
	char account[10];
	strcpy(account,account_str.c_str());
	Card newcard(account, password, cardHolder, 0, CID, CPhone);
	FILE*fp_card;
	if (NULL == (fp_card = fopen("CARDINFORMATION", "rb+"))){
		fprintf(stderr, "Can not open file");
		exit(1);
	}
	fseek(fp_card, 0, SEEK_END);
	if (fwrite(&newcard, sizeof(Card), 1, fp_card) != 1)
		printf("file write error\n");
	time_t timer;
	time(&timer);
	tm* t_tm = localtime(&timer);	//��ȡ�˵�ǰʱ�䣬����ת��Ϊint���͵�year��month��day
	int year = t_tm->tm_year + 1900;
	int month = month = t_tm->tm_mon + 1;
	int day = t_tm->tm_mday;
	Record record(newcard.getcardID(), year, month, day, 'g');
	record.signUpRecord();
	fclose(fp_card);
	allcard++;
	return;
}

void Library::signOut(){		//�û�ע��
	//�Ѹոյ�½ʱ��ȡ��cardд���ļ�ԭ����λ��
	FILE*fp_card;
	if (NULL == (fp_card = fopen("CARDINFORMATION", "rb+"))){
		fprintf(stderr, "Can not open file");
		exit(1);
	}
	int position = atoi(card.getcardID()) - 1000000000 - 1;
	fseek(fp_card, position*sizeof(Card), 0);
	if (fwrite(&card, sizeof(Card), 1, fp_card) != 1)
		printf("file write error\n");
	time_t timer;
	time(&timer);
	tm* t_tm = localtime(&timer);	//��ȡ�˵�ǰʱ�䣬����ת��Ϊint���͵�year��month��day
	int year = t_tm->tm_year + 1900;
	int month = month = t_tm->tm_mon + 1;
	int day = t_tm->tm_mday;
	Record record(card.getcardID(), year, month, day, 'h');
	record.signOutRecord();
	fclose(fp_card);
	card();
	//�Ƿ�allcard��allbook��alladminд���ļ���
	FILE *fp_num;
	if (NULL == (fp_num = fopen("ALLNUM", "rb+"))){
		fprintf(stderr, "Can not open file");
		exit(1);
	}
	if (fwrite(&allcard, sizeof(int), 1, fp_num) != 1)			//����д��?
		printf("file write error\n");
	if (fwrite(&allbook, sizeof(int), 1, fp_num) != 1)
		printf("file write error\n");
	if (fwrite(&alladmin, sizeof(int), 1, fp_num) != 1)
		printf("file write error\n");
	fclose(fp_num);
}

void Library::signOut_Admin(){		//����Աע��
	//�Ѹոյ�½ʱ��ȡ��adminд���ļ�ԭ����λ��
	FILE*fp_admin;
	if (NULL == (fp_admin = fopen("ADMININFORMATION", "rb+"))){
		fprintf(stderr, "Can not open file");
		exit(1);
	}
	int position = atoi(card.getcardID.c_str()) - 2000 - 1;	//����Ա�˻���ʽ2001
	fseek(fp_admin, position*sizeof(Administrator), 0);
	if (fwrite(&admin, sizeof(Administrator), 1, fp_admin) != 1)
		printf("file write error\n");
	time_t timer;
	time(&timer);
	tm* t_tm = localtime(&timer);	//��ȡ�˵�ǰʱ�䣬����ת��Ϊint���͵�year��month��day
	int year = t_tm->tm_year + 1900;
	int month = month = t_tm->tm_mon + 1;
	int day = t_tm->tm_mday;
	Record record(admin.getaccount(), year, month, day, 'h');
	record.signOutRecord();
	fclose(fp_admin);
	admin();
	//�Ƿ�allcard��allbook��alladminд���ļ���
	FILE *fp_num;
	if (NULL == (fp_num = fopen("ALLNUM", "rb+"))){
		fprintf(stderr, "Can not open file");
		exit(1);
	}
	if (fwrite(&allcard, sizeof(int), 1, fp_num) != 1)			//����д��?
		printf("file write error\n");
	if (fwrite(&allbook, sizeof(int), 1, fp_num) != 1)
		printf("file write error\n");
	if (fwrite(&alladmin, sizeof(int), 1, fp_num) != 1)
		printf("file write error\n");
	fclose(fp_num);
}

/*void Library::matchCid(){		//���֤IDƥ��

}*/

void Library::ResetPassword(char*oldpassword, char*newpassword1, char*newpassword2){	//�������������������д��ԭλ��
	if ((string)oldpassword == (string)card.getcPassword){
		if ((string)newpassword1 == (string)newpassword2)
			card.setcPassword(newpassword1);
	}
	return;
}

void Library::update_Order(){			//���������û�����ϵͳʱ �Ի��������и���
	/*
	ƥ��鿴ԤԼ��¼�Ƿ�ʧЧ
	���ʧЧ д��ϵͳ��¼ ���ü�¼��ʶ��Ϊ1
	Ȼ���ҵ����� ����ԤԼ��-1
	�����ʱ��ʱ���>ԤԼ����
	��������� ��ʱ���-1
	#define BUFFERZONE_ORDER bufferOrderZone//ԤԼ�������ļ�
	#define BOOKINFORMATION bookInformation//ȫ��ͼ����Ϣ
	#define CARDINFORMATION cardInformation//ȫ���û���Ϣ
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
	tm* t_tm = localtime(&timer);	//��ȡ�˵�ǰʱ�䣬����ת��Ϊint���͵�year��month��day
	int year = t_tm->tm_year + 1900;
	int month = month = t_tm->tm_mon + 1;
	int day = t_tm->tm_mday;
	while (!feof(fp_buffer_order)){//feof()�������������ж��ļ��Ƿ񵽴��ļ�β���������ļ�β����������ֵΪ1
		fread(&record_temp, sizeof(Record), 1, fp_buffer_order);
		if (compareDate(year, month, day, record_temp.getyear(), record_temp.getmonth(), record_temp.getday()) > 10){
			//�޸ĸ�����Ҫ�޸ĵı���,���ԤԼ����-1���Ƚ������ʱ����ԤԼ�����Ĵ�С���ٶԿ�����ʱ�������Ӧ�������û���ԤԼ����-1
			while (!feof(fp_bookInfo)){
				fread(&book_temp, sizeof(Book), 1, fp_bookInfo);
				if (strcmp(book_temp.getbookID(), record_temp.getBookid()) == 0){
					book_temp.setbookMan(book_temp.getbookMan()-1);
					if (book_temp.getbookMan() < book_temp.gettStorage()){//ԤԼ����С����ʱ���
						book_temp.settStorage(book_temp.gettStorage()-1);//��ʱ���-1
						book_temp.setstorage(book_temp.getstorage() + 1);//���+1
					}
					if (fwrite(&book_temp, sizeof(Book), 1, fp_bookInfo) != 1)printf("file write error\n");//�޸��ļ��е�����
					break;
				}
			}
			while (!feof(fp_cardInfo)){
				fread(&card_temp, sizeof(Card), 1, fp_cardInfo);
				if (strcmp(card_temp.getcardID(), record_temp.getCardid()) == 0){
					card_temp.setbookedCount(card_temp.getbookedCount()-1);//�û���ԤԼ������һ
					if (fwrite(&card_temp, sizeof(Card), 1, fp_cardInfo) != 1)printf("file write error\n");//�޸��ļ��е�����
					break;
				}
			}
			rewind(fp_bookInfo);
			rewind(fp_cardInfo);//rewind()�������ڽ��ļ�ָ������ָ���ļ��Ŀ�ͷ��ͬʱ������ļ�����صĴ����eof��ǣ��൱�ڵ���fseek(stream, 0, SEEK_SET)
			record_temp.setflag2('1');//1��ԤԼ��¼��ʾ��ԤԼʧЧ
			if (fwrite(&record_temp, sizeof(Record), 1, fp_buffer_order) != 1)printf("file write error\n");//����ԤԼ�����ļ�
			record_temp.setflag1('f');
			record_temp.setyear(year);
			record_temp.setmonth(month);
			record_temp.setday(day);//��record_temp�޸ĳ�һ��ԤԼʧЧ��¼
			record_temp.bookOrderNoRecord();//���ú���д��ʧЧ��¼�ļ�
		}
	}
	fclose(fp_buffer_order);
}

void Library::update_book(){		//���������ڵ�½���ж��û����ѽ��鼮�Ƿ��Ѿ�����
	FILE*fp_lendbuffer;
	if ((fp_lendbuffer = fopen("BUFFERZONE_LEND", "rb+")) == NULL)
	{
		fprintf(stderr, "Can not open file");
		exit(1);
	}
	FILE*fp_End = fopen("BUFFERZONE_LEND", "rb+");
	if (fp_End == NULL) {
		printf("file error\n");
		exit(1);
	}
	fseek(fp_End, 0, SEEK_END);		//��fpEndָ���Ƶ��ļ�ĩβ*/
	Record record_temp;		//���ڶ�ȡ����buffer�е�ÿһ����¼
	int i = 0;
	fseek(fp_lendbuffer, i * sizeof(Card), SEEK_SET);
	while (fp_lendbuffer != fp_End){
		fread(&record_temp, sizeof(Record), 1, fp_lendbuffer);
		if ((string)record_temp.getCardid() == (string)card.getcardID()){
			//ȷʵ�ǵ�ǰ�û����Ĳ���δ�����鼮
			time_t timer;
			time(&timer);
			tm* t_tm = localtime(&timer);	//��ȡ�˵�ǰʱ�䣬����ת��Ϊint���͵�year��month��day
			int year = t_tm->tm_year + 1900;
			int month = month = t_tm->tm_mon + 1;
			int day = t_tm->tm_mday;
			//�жϵ�ǰʱ����Ӧ������
			if (!(compareDate(record_temp.getyear(), record_temp.getmonth(), record_temp.getday(), year, month, day) > 0)){
				//�����ǰ���ڳ����������ڣ���ô�ͽ���ΥԼ����
				card.setoweMoney(0.5*compareDate(year, month, day, record_temp.getyear(), record_temp.getmonth(), record_temp.getday()));//������һ��0.5Ԫ����
				if (card.getbalance() >= card.getoweMoney())Rcharge();		//����㹻
				else card.setcardState('0');	//���㣬�����˺�
			}
		}
		i++;
		fseek(fp_lendbuffer, i * sizeof(Card), SEEK_SET);
	}
	fclose(fp_lendbuffer);
	fclose(fp_End);
}

void Library::charge(double money){			//��ֵ����
	card.setbalance(card.getbalance() + money);
}

void Library::Rcharge(){		//�����û�ΥԼ��
	double owemoney = card.getoweMoney();
	card.setbalance(card.getbalance() - owemoney);
}

/*void Library::resetCard(){		//�����޸Ŀ���Ϣ �ֻ�

}*/

