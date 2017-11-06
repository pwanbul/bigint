#pragma once
#include <string>
using namespace std;
class bigint
{
private:

	int *  _array;
	int _size;
	char _flag;
public:
	
	//构造函数
	bigint();
	bigint(string op);
	//拷贝构造函数
	bigint(const bigint &op);
	bigint(int size);
	
	//析构函数
	~bigint();
	//重载的赋值符
	bigint & operator=(const bigint &opr);
	//重载的cout<<
	friend ostream& operator<<(ostream & os, const bigint & opr);
	//重载的加法,返回值为对象，而不是引用或者指针，考虑到和可能会比加数大
	bigint  operator+( bigint &opr);
	//重载的减法
	bigint operator-( bigint &opr);
	//重载的乘法
	bigint operator*( bigint &opr);
	//重载的除法
	bigint operator/( bigint &opr);
	bigint operator++();
	//仅用在除法中
	bigint tempQuotient(int wei);
	//仅用在除法中
	bigint cut(int wei);
	//判断是否为正数
	bool isPlus()const;
	//判断是否为负数
	bool isMinus()const;
	//bool isZero()const;
	//单纯的加减乘除，不考虑符号
	void add(bigint &opl, bigint &opr,	bigint & result);
	void sub(bigint &opl, bigint &opr,	bigint & result);
	void mul(bigint &opl, bigint &opr,	bigint & result);
	void div(bigint &opl, bigint &opr,	bigint & result);
	//判断2个操作数的绝对值大小，作操作数大于右操作数为1，否则为-1，相等为0；
	int isAbsoluteMax(const bigint &op);
	//把操作数转换成相等的长度，以较大操作数的长度为准，不考虑符号,在传递参数时，不要直接传递原操作数
	int toEquelSize(bigint &opl,bigint &opr);
	//去除最后结果中前面多余的零,并缩小到实际大小
	void format(bigint & op);
	//获得长度
	int getLength();
	//按数组的为序输出，测试用
	void show(); 
};
