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
	
	//���캯��
	bigint();
	bigint(string op);
	//�������캯��
	bigint(const bigint &op);
	bigint(int size);
	
	//��������
	~bigint();
	//���صĸ�ֵ��
	bigint & operator=(const bigint &opr);
	//���ص�cout<<
	friend ostream& operator<<(ostream & os, const bigint & opr);
	//���صļӷ�,����ֵΪ���󣬶��������û���ָ�룬���ǵ��Ϳ��ܻ�ȼ�����
	bigint  operator+( bigint &opr);
	//���صļ���
	bigint operator-( bigint &opr);
	//���صĳ˷�
	bigint operator*( bigint &opr);
	//���صĳ���
	bigint operator/( bigint &opr);
	bigint operator++();
	//�����ڳ�����
	bigint tempQuotient(int wei);
	//�����ڳ�����
	bigint cut(int wei);
	//�ж��Ƿ�Ϊ����
	bool isPlus()const;
	//�ж��Ƿ�Ϊ����
	bool isMinus()const;
	//bool isZero()const;
	//�����ļӼ��˳��������Ƿ���
	void add(bigint &opl, bigint &opr,	bigint & result);
	void sub(bigint &opl, bigint &opr,	bigint & result);
	void mul(bigint &opl, bigint &opr,	bigint & result);
	void div(bigint &opl, bigint &opr,	bigint & result);
	//�ж�2���������ľ���ֵ��С���������������Ҳ�����Ϊ1������Ϊ-1�����Ϊ0��
	int isAbsoluteMax(const bigint &op);
	//�Ѳ�����ת������ȵĳ��ȣ��Խϴ�������ĳ���Ϊ׼�������Ƿ���,�ڴ��ݲ���ʱ����Ҫֱ�Ӵ���ԭ������
	int toEquelSize(bigint &opl,bigint &opr);
	//ȥ���������ǰ��������,����С��ʵ�ʴ�С
	void format(bigint & op);
	//��ó���
	int getLength();
	//�������Ϊ�������������
	void show(); 
};
