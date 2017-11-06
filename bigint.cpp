/*************************************************
Copyright:Yzr
Author:Yzr
Date:2017-11-1
Description:�������㣬���������޶�λ����֧�ּӼ��˳�
**************************************************/

#include "bigint.h"
#include <iostream>

using namespace std;
void setArr(int *arr, int size);
void getArr(int *arr, int size);
bool isEmptyArr(int *arr, int size);
void reverse(int *src, int n);
void left_n(int *src, int len, int n);
void setBoolArr(bool *arr, int size);
void getBoolArr(bool *arr, int size);
//���캯��
bigint::bigint()
{
	_array = NULL;
	_size = 0;
	_flag = '*';
}
//���캯���������û������룬�洢��int�����У����ŵ������棬����Ϊ���ݵľ���ֵ����
bigint::bigint(string op)
{
	if (op.at(0) == 45) {
		_flag = '-';
		_size = op.length() - 1;
	}
	else if(op.at(0) == 43){
		cout << "���������Ϊ�������벻Ҫ����\"+\"��" << endl;		
		abort();		
	}
	else {
		_flag = '+';
		_size = op.length();
	}
	_array = new int[_size];
	if (isPlus()) {
		for (int x = 0; x < _size; x++) {
			_array[x] = op.at(x) - 48;
		}
	}
	else {
		for (int x = 0; x < _size; x++) {
			_array[x] = op.at(x+1) - 48;
		}
	}
}
//�������캯��
bigint::bigint(const bigint & op)
{
	_size = op._size;
	_flag = op._flag;
	_array = new int[_size];
	for (int x = 0; x < _size; x++) {
		_array[x] = op._array[x];
	}
}
//ͨ��bigint::bigint(int size)��������Ķ��������Ĭ��Ϊ*,ֵΪ��
bigint::bigint(int size)
{
	_size = size;
	_flag = '*';                 
	_array = new int[_size];
	for (int x = 0; x < _size; x++){
		_array[x] = 0;
	}
}
//��������
bigint::~bigint()
{
	delete[] _array;
}
//��ֵ���캯��
bigint & bigint::operator=(const bigint & opr)
{
	if (this == &opr)
		return *this;
	delete[] _array;
	_size = opr._size;
	_flag = opr._flag;
	_array = new int[_size];
	for (int x = 0; x < _size; x++) {
		_array[x] = opr._array[x];
	}
	return *this;
}
//���صļӷ������
bigint bigint::operator+(bigint & opr) 
{	
	bigint tempopl = *this;							//Ϊ�˷�ֹ֮��Ĳ������ԭ���Ҳ���������Ӱ�죬������ҪΪ���Ҳ�����������ʱ����
	bigint tempopr = opr;
	int tempLength = toEquelSize(tempopl,tempopr);	//��ò�������λ���ϳ��Ĳ�������λ��
	bigint result = tempLength + 1;					//�ӷ����ܲ�����λ����˱������ս���Ķ���Ĵ�СӦ�� tempLength + 1


	cout << "���� + "<< endl;
	if (isPlus() && opr.isPlus()) {					//���ŵ��жϣ����ڲ�������ֵ�ͷ��ŷֿ����棬��˿���Ԥ���жϳ�����ķ���
		cout << "+ +" << endl;
		cout << "call add" << endl;
		result._flag = '+';						//���Ҳ�������Ϊ���������Ϊ��
		add(tempopl, tempopr, result);			//���üӷ�����Ϊ���ô���
	}
	else if (isPlus() && opr.isMinus()) {
		cout << "+ -" << endl;
		cout << "call sub" << endl;
		if (isAbsoluteMax(opr) == 0) {			//������Ϊһ��һ���Ҿ���ֵ��ȣ����Ϊ��
			result._flag = '+';  
			//�������ֱ�����result  ,��Ϊresult��Ȼ����ʼ��Ϊ������ж��0�Ķ��󣬵�format֮����Իָ�������bigint result = tempLength + 1;��
		}
		else if (isAbsoluteMax(opr) == 1) {		//������Ϊһ��һ������ߴ����ұߵģ����ü��������Ϊ����
			result._flag = '+';
			sub(tempopl, tempopr, result);
		}
		else if (isAbsoluteMax(opr) == -1) {	//������Ϊһ��һ�����ұߴ�����ߵģ����ü���ʱҪ��������˳�򣬽��Ϊ����
			result._flag = '-';
			cout << "����Ҫ��������������addsub����ɲ�������" << endl;
			sub(tempopr, tempopl, result);
		}
		
	}
	else if (isMinus() && opr.isPlus()) {
		cout << "- +" << endl;
		cout << "call sub" << endl;
		if (isAbsoluteMax(opr) == 0) {					//������Ϊһ��һ���Ҿ���ֵ��С��ȣ����Ϊ��
			result._flag = '+';  
		}
		else if (isAbsoluteMax(opr) == 1) {				//������Ϊһ��һ������ߴ����ұߣ����ü������Ϊ����
			result._flag = '-';
			sub(tempopl, tempopr, result);
		}
		else if (isAbsoluteMax(opr) == -1) {			//������Ϊһ��һ�����ұߴ�����ߣ����ü���ʱ��������˳�򣬽��Ϊ����
	
			result._flag = '+';
			cout << "����Ҫ��������������addsub����ɲ�������" << endl;
			sub(tempopr, tempopl, result);
		}
	
	}
	else if (isMinus() && opr.isMinus()) {				//���Ҳ�������Ϊ���������Ϊ��
		cout << "- -" << endl;
		cout << "call add" << endl;
		result._flag = '-';
		add(tempopl, tempopr, result);
	}
	//result.show();
	//cout<<"format֮��"<< endl;
	format(result);
	//result.show();
	cout << "---------------" << endl;
	return result ;
}
//���صļ��������
bigint bigint::operator-(bigint & opr)
{
	bigint tempopl = *this;
	bigint tempopr = opr;
	int tempLength = toEquelSize(tempopl, tempopr);
	bigint result = tempLength + 1;    //��ֹ��λ֮��λ�ò���
	cout << "���� - " << endl;

	if (isPlus() && opr.isPlus()) {
		cout << "+ +" << endl;
		cout << "call sub" << endl;
		if (isAbsoluteMax(opr) == 0) {							//���Ҳ�������Ϊ�����Ҿ���ֵ��С��ȣ����Ϊ��
			result._flag = '+';  
			//�������ֱ�����result  ,��Ϊresult��Ȼ����ʼ��Ϊ������ж��0�Ķ��󣬵�format֮����Իָ�������bigint result = tempLength + 1;��
		}
		else if (isAbsoluteMax(opr) == 1) {						//���Ҳ�������Ϊ��������ߴ����ұߣ����ü��������Ϊ����
			result._flag = '+';									
			sub(tempopl, tempopr, result);
		}
		else if (isAbsoluteMax(opr) == -1) {					//���Ҳ�������Ϊ�������ұߴ�����ߣ����ü���ʱ��������˳�򣬽��Ϊ����
			result._flag = '-';
			cout << "����������" << endl;
			sub(tempopr, tempopl, result);
		}
	}
	else if (isPlus() && opr.isMinus()) {						
		cout << "+ -" << endl;
		cout << "call add" << endl;
																//������һ��һ�������üӷ������Ϊ����
		result._flag = '+';
		add(tempopl, tempopr, result);
		
	}
	else if (isMinus() && opr.isPlus()) {						
		cout << "- +" << endl;
		cout << "call add" << endl;
		result._flag = '-';										//������һ��һ�������üӷ������Ϊ����
		add(tempopl, tempopr, result);
	}
	else if (isMinus() && opr.isMinus()) {						
		cout << "- -" << endl;
		cout << "call sub" << endl;
		if (isAbsoluteMax(opr) == 0) {							//��������Ϊ�����Ҿ���ֵ��С��ȣ����Ϊ��
			result._flag = '+';
		}
		else if (isAbsoluteMax(opr) == 1) {						//��������Ϊ��������ߴ����ұߣ��������������Ϊ����
			result._flag = '-';
			sub(tempopl, tempopr, result);
		}
		else if (isAbsoluteMax(opr) == -1) {					//��������Ϊ�������ұߴ�����ߣ���������ʱ��������˳�򣬽��Ϊ����
			result._flag = '+';
			cout << "����������" << endl;
			sub(tempopr, tempopl, result);
		}
	}
	//result.show();
	cout << "format֮��" << endl;
	format(result);
	//result.show();
	cout << "---------------" << endl;
	return result;
}
//���صĳ˷������
bigint bigint::operator*(bigint & opr)
{
	
	bigint tempopl = *this;
	bigint tempopr = opr;
	int tempLength = toEquelSize(tempopl, tempopr);
	bigint result = tempLength * 2;							//��ֹ��λ֮��λ�ò��������ս����λ�����ᳬ���ϳ�������λ����2��


	cout << "���� * " << endl;								//������ź��жϣ���ͬΪ��������Ϊ��
	if (isPlus() && opr.isPlus()) {							
		cout << "+ +" << endl;
		cout << "call mul" << endl;
		result._flag = '+';									
		mul(tempopl, tempopr, result);
	}
	else if (isPlus() && opr.isMinus()) {
		cout << "+ -" << endl;
		cout << "call mul" << endl;
		result._flag = '-';  
		mul(tempopl, tempopr, result);
	}
	else if (isMinus() && opr.isPlus()) {
		cout << "- +" << endl;
		cout << "call mul" << endl;
		result._flag = '-';
		mul(tempopl, tempopr, result);

	}
	else if (isMinus() && opr.isMinus()) {
		cout << "- -" << endl;
		cout << "call mul" << endl;
		result._flag = '+';
		mul(tempopl, tempopr, result);
	}
	//result.show();
	//cout<<"format֮��"<< endl;
	format(result);
	//result.show();
	cout << "---------------" << endl;
	return result;
	
}
//���صĳ��������
bigint bigint::operator/(bigint & opr)
{
	bigint tempopl = *this;
	bigint tempopr = opr;							 //���ﲢû�аѲ������������ͬ�ĳ���
	tempopl._flag = tempopr._flag = '+';			//���Ҳ�������������Ϊ����

	bigint result = tempopl._size;					 //�����λ�����ᳬ����������λ��

	cout << "���� / " << endl;
	if (tempopl.isAbsoluteMax(opr) == 0) {			//���Ҳ������ľ���ֵ��Сһ�������Ϊ1�����ŵ����ж�
		result =bigint("1");			
		if (isPlus() && opr.isPlus()) {
			result._flag = '+';
		}
		else if (isPlus() && opr.isMinus()) {
			result._flag = '-';
		}
		else if (isMinus() && opr.isPlus()) {
			result._flag = '-';
		}
		else if (isMinus() && opr.isMinus()) {
			result._flag = '+';
		}
	}
	else if (tempopr.isAbsoluteMax(tempopl) == 1) {		//�������ܴ��ڱ�����
		cout << "�������ܴ��ڱ�����" << endl;
		abort();
	}
	else if (tempopl.isAbsoluteMax(opr) > 0){			//���������ڳ���
		if (isPlus() && opr.isPlus()) {
			cout << "+ +" << endl;
			cout << "call div" << endl;
			
			div(tempopl, tempopr, result);
			result._flag = '+';							//������ŵ�ȷ����Ҫ���ڼ�������֮��
		}
		else if (isPlus() && opr.isMinus()) {
			cout << "+ -" << endl;
			cout << "call div" << endl;
			div(tempopl, tempopr, result);
			result._flag = '-';

		}
		else if (isMinus() && opr.isPlus()) {
			cout << "- +" << endl;
			cout << "call div" << endl;
			
			div(tempopl, tempopr, result);
			result._flag = '-';

		}
		else if (isMinus() && opr.isMinus()) {
			cout << "- -" << endl;
			cout << "call div" << endl;
			div(tempopl, tempopr, result);
			result._flag = '+';

		}
	}
	
	//result.show();
	//cout<<"format֮��"<< endl;
	format(result);
	//result.show();
	cout << "---------------" << endl;
	return result;


}
//���ص����������
bigint bigint::operator++()
{
	bigint temp = "1";
	*this = *this + temp;
	return *this;
}
/*�ڽ��г�������ʱ����Ҫ�Գ���
����547����2����Ҫ��100����������200��
��Щ����������n*100(nȡ1��2��3...),
֮��Ҫn*10���������������ȷ��������100������10��*/
bigint bigint::tempQuotient(int wei)
{
	string quotient = "1";
	for (int x = 1 ; x < wei; x++) {
		quotient.append("0");
	}
	return bigint(quotient);
}
/*�ڼ������ʱ����Ҫ�����Գ���λ�ã�
��120/10 ��cut�����᷵�ر�������ǰ����._sizeλ�ϵ�����
�Ա�����������Ƚϣ���12��10�Ƚϡ�
*/
bigint bigint::cut(int wei)
{
	string s ;
	for (int x = 0; x < wei; x++) {
		int temp = _array[x];
		if (temp == 0) {
			s.append("0");
		}else if (temp == 1) {
			s.append("1");
		} else if (temp == 2) {
			s.append("2");
		}
		else if(temp == 3) {
			s.append("3");
		}
		else if (temp == 4) {
			s.append("4");
		}
		else if (temp == 5) {
			s.append("5");
		}
		else if (temp == 6) {
			s.append("6");
		}
		else if (temp == 7) {
			s.append("7");
		}
		else if (temp == 8) {
			s.append("8");
		}
		else if (temp == 9) {
			s.append("9");
		}
		
	}
	return bigint(s);
	
}
//�жϲ������ķ���
bool bigint::isPlus() const
{
	if (_flag == '+')
		return true;
	else
		return false;
}
//�жϲ������ķ���
bool bigint::isMinus() const
{
	if (_flag == '-') {
		return true;
	}
	else {
		return false;
	}
}

/*bool bigint::isZero() const
//{
//	if (_flag == '?') {
//		return true;
//	}
//	else {
//		return false;
//	}
//}
*/

//�ӷ�����
void bigint::add(bigint & opl, bigint & opr, bigint & result)
{
	//cout << "opl size :" << opl._size << endl;
	//cout << "opr size:" << opr._size << endl;
	//cout << "result size:" << result._size << endl;

	int *carry = new int[opl._size+1];							//carry���鱣���λ��,λ��������������һλ����ʱ�������Ѿ�����
	int *tempresult = new int[opl._size+1];						//tempresult�����м�����λ��������������һλ����ʱ�������Ѿ�����
	
	setArr(carry, opl._size+1);									//��0
	setArr(tempresult, opl._size+1);							//��0


	for (int x = opl._size ; x > 0; x--) {						//�����㷨��ģ���ֹ�����ķ�ʽ
		int temp = opl._array[x-1] + opr._array[x-1];
		tempresult[x] = temp % 10;

		if (tempresult[x] != temp) {
			carry[x]++;
		}
	}
	getArr(carry, opl._size+1);									//���carry���飬��λΪ1
	getArr(tempresult, opl._size+1);							//���tempresult����

		
	left_n(carry, opl._size+1, 1);								//�ѳ���Ϊopl._size+1��carry��������1λ����λ��0
	cout << "carry �ƶ���" << endl;
	getArr(carry, opl._size + 1);
	getArr(tempresult, opl._size + 1);


	bool *flag = new bool[opl._size + 1];						//����һ��bool���飬������carry�����tempresult������Ӻ��λ��λ
	setBoolArr(flag, opl._size + 1);							//��flag����Ϊfalse
	//getBoolArr(flag, opl._size + 1);
	for (int x = opl._size ; x >= 0; x--) {						
		int temp = carry[x] + tempresult[x];
		result._array[x] = temp % 10;
		if (result._array[x] != temp) {
			flag[x] = true;
		}
	}
	cout << "δ����bool����ʱ" << endl;							
	getBoolArr(flag, opl._size + 1);
	getArr(result._array, opl._size + 1);
	cout << "����bool�����" << endl;							
	for (int x = opl._size; x >= 0; x--) {						//flag������Ϊtrue��λ����ʾ������ж�Ӧλ����һλ�н�λ
		if (flag[x] == true) {
			result._array[x - 1]++;
		}
	}
	getArr(result._array, opl._size + 1);

	delete[] flag;
	delete[] tempresult;
	delete[] carry;
	
}
//��������
void bigint::sub(bigint & opl, bigint & opr, bigint & result)
{
	int *carry = new int[opl._size + 1];									
	int *tempresult = new int[opl._size + 1];

	setArr(carry, opl._size + 1);
	setArr(tempresult, opl._size + 1);
	

	for (int x = opl._size; x > 0; x--) {								//��λΪ-1��������carry��
		if (opl._array[x - 1] < opr._array[x - 1]) {					//�м���������tempresult��
			opl._array[x - 1] += 10;
			carry[x]--;
		}
		tempresult[x] = opl._array[x - 1] - opr._array[x - 1];
	}

	getArr(carry, opl._size + 1);
	getArr(tempresult, opl._size + 1);

	
	left_n(carry, opl._size + 1, 1);									//�ѳ���Ϊopl._size+1��carry��������1λ����λ��0
	cout << "carry �ƶ���" << endl;
	getArr(carry, opl._size + 1);
	getArr(tempresult, opl._size + 1);



	for (int x = result._size - 1; x >= 0; x--) {						//��carry��tempresult�ϲ����ǵô���100-1�������
		int temp = carry[x] + tempresult[x];
		if (temp == -1) {
			result._array[x] = 9;
			tempresult[x - 1]--;
		}
		else {
			result._array[x] = temp;
		}
	}

	cout << "�ϲ�carry��tempresult֮��" << endl;
	getArr(result._array, opl._size + 1);


	delete[] tempresult;
	delete[] carry;
}
//�˷�����
void bigint::mul(bigint & opl, bigint & opr, bigint & result)
{

	//�ٶ������һά����Ϊm�� �ڶ�ά����Ϊn
	

	int **tempresult = NULL, **carry = NULL;
	int i, j, k, q;
	//������λ���飬�����Ƴɺ���������Ϊ�ϴ��������λ��������Ϊ�ϴ��������λ��*2
	tempresult = new int *[opl._size];
	carry = new int *[opl._size];
	for (int i = 0; i<opl._size; i++)
	{
		tempresult[i] = new int[opl._size * 2];
		carry[i] = new int[opl._size * 2];
	}
	for (int i = 0; i < opl._size; i++) {				//������λ�������0
		for (int j = 0; j < opl._size * 2; j++) {
			tempresult[i][j] = 0;
			carry[i][j] = 0;
		}
	}

	int xx = 0;											//�ؼ��㷨����λ������carry��λ�����У��м���������tempresult��λ������
	for (j = opl._size - 1; j >= 0; j--) {
		for (i = opl._size - 1, q = opl._size * 2 - 1, k = opl._size * 2 - 1; i >= 0 && q >= 0 && k >= 0; i--, q--, k--) {

			int temp = opl._array[i] * opr._array[j];
			tempresult[j][k] = temp % 10;
			if (tempresult[j][k] != temp) {
				carry[j][q] = carry[j][q] + temp / 10;
			}
		}
	}
		
	//���carry����
	cout << "���carry����" << endl;
	for (int i = 0; i < opl._size; i++) {
		cout << i << ":";
		for (int j = 0; j < opl._size * 2; j++) {
			cout << carry[i][j];
		}
		cout << endl;
	}

	//���tempresult���� 
	cout << "���tempresult���� " << endl;
	for (int i = 0; i < opl._size; i++) {
		cout << i << ":";
		for (int j = 0; j < opl._size * 2; j++) {
			cout << tempresult[i][j];
		}
		cout << endl;
	}
	
	
	for (int x = 0; x <opl._size; x++) {					//����carry��tempresult����
		left_n(carry[x], opl._size * 2, opl._size - x );
		left_n(tempresult[x], opl._size * 2, opl._size - (x + 1));
	}

	cout << "ת����" << endl;
	//���carry����
	cout << "���carry����" << endl;
	for (int i = 0; i < opl._size; i++) {
		cout << i << ":";
		for (int j = 0; j < opl._size * 2; j++) {
			cout << carry[i][j];
		}
		cout << endl;
	}

	//���tempresult���� 
	cout << "���tempresult���� " << endl;
	for (int i = 0; i < opl._size; i++) {
		cout << i << ":";
		for (int j = 0; j < opl._size * 2; j++) {
			cout << tempresult[i][j];
		}
		cout << endl;
	}

	int *flag = new int[opl._size * 2];							//flag���鱣�棬carry��tempresult����Ľ�λ�����ܴ���1
	setArr(flag, opl._size * 2);								//flag��0
	int temp = 0;
	for (int j = opl._size * 2 - 1; j >= 0; j--) {
		for (int i = opl._size - 1; i >= 0; i--) {
			temp = temp + tempresult[i][j] + carry[i][j];
			result._array[j] = temp % 10;						//�м�������result._array������
			if (result._array[j] != temp) {
				flag[j] = temp / 10;
			}
		}
		temp = 0;
	}
	cout << "�ƶ�flagǰ :" << endl;
	getArr(flag, opl._size * 2);								//flag���ƣ�����ʵ�ڳ˷���flagҲ������Ƴ�int���͵ģ�
	cout << "result :" << endl;
	result.show();

	
	cout << "�ƶ�flag�� :" << endl;
	

	if (!isEmptyArr(flag, opl._size * 2)) {						//��flag��Ϊ��ʱ����flag��result._array��ӣ�������result._array��
		left_n(flag, opl._size * 2, 1);
		getArr(flag, opl._size * 2);
		for (int x = opl._size * 2 - 1; x >= 0; x--) {						
			int temp = result._array[x] + flag[x];
			flag[x] = 0;
			result._array[x] = temp % 10;
			if (result._array[x] != temp) {
				flag[x-1]++;									//2�����������1λ
			}
		}
	}

	
	result.show();
	delete[]  carry;
	delete[]  tempresult;
	delete[]  flag;

}
//��������
void bigint::div(bigint & opl, bigint & opr, bigint & result)
{
	/*��������Ч�ʺܵ�*/
	/*bigint temp = "0";
	bigint one = "1";

	for (temp = bigint("1"); opl.isAbsoluteMax(temp) == 1; ++temp) {
		if ((((opl.isAbsoluteMax(temp*opr)) == 1)|| ((opl.isAbsoluteMax(temp*opr)) == 0)) && ((opr * (temp+one)).isAbsoluteMax(opl)) == 1)
			break;
	}
	result = temp;*/



	int x = 0;
	int y = 0;
	int flag = 0;												
	
	if (opr.isAbsoluteMax(cut(opr._size)) == 1) {						//������Ҫȷ���Գ�����ʼλ�ã�ͨ��cut����������ο�cut����
		flag = opr._size;
	}
	else if (opr.isAbsoluteMax(cut(opr._size)) == -1) {
		flag = opr._size - 1;
	}
	else if (opr.isAbsoluteMax(cut(opr._size)) == 0) {
		flag = opr._size - 1;
	}

	cout << "flag : " << flag << endl;

	bigint* temp =  new bigint[opl._size-flag];						//�����м�������������Գ���λ���йأ���Ҫ��Ϊbigint("0")
	for (int x = 0; x < opl._size - flag; x++) {
		temp[x] = bigint("0");
	}

	bigint xx = opl;																					
	bigint yy;
	bigint zz ;
	bigint qq;
	bigint ww;
	
	for (int x = opl._size - flag; x > 0; x--) {	//�����㷨��ģ���ֹ�����
		
		for (zz = bigint("1"); zz.isAbsoluteMax(bigint("10")) == -1; ++zz) {
			
			temp[x - 1] = zz * tempQuotient(x);
			qq = (zz+bigint("1")) * tempQuotient(x);		
			yy = (temp[x - 1] * opr);
			ww = (qq*opr);
			if (((yy.isAbsoluteMax(xx) == -1)|| yy.isAbsoluteMax(xx) == 0) && ww.isAbsoluteMax(xx) == 1) {
				xx = xx - yy;
				break;
			}
		}	
		if ((xx.isAbsoluteMax(opr) == -1)&& x==1 ||(xx.isAbsoluteMax(bigint("0")) == 0)) {       //��x==1ʱ����ʾ�������һλ������С��9����
		break;
		}
	}

	result = bigint("0");
	/*cout << "temp[1]" << endl;
	cout << temp[1] << endl;
	cout << "temp[0]" << endl;
	cout << temp[0] << endl;*/
	for (int x = opl._size - flag - 1; x >= 0; x--) {
		result = result + temp[x];
	}

	delete[] temp;	
}

//isAbsoluteMax�����ڱȽϲ���������ֵ��Сʱ�������ܵ��������Ƿ�����Ӱ��
int bigint::isAbsoluteMax(const bigint & op)
{
	if (_size > op._size) {
		return 1;
	}
	else if (_size < op._size) {
		return -1;
	}
	else if (_size == op._size) {
		int i = 0;
		for(int x = 0; x < _size; x++) {
			if (_array[x] == op._array[x]) {
				i++;
				continue;
			}
			else if (_array[x] > op._array[x]) {
				return 1;
			}
			else if (_array[x] < op._array[x]) {
				return -1;
			}
		}
		if (i == _size) {
			return 0;
		}
	}
	
}
//���ص�<<
ostream & operator<<(ostream & os, const bigint & opr)
{
	if (opr.isMinus()) {
		os << '-';
	}
	for (int x = 0; x < opr._size; x++) {
		os << opr._array[x];
	}
	return os;
}
//��������ת��Ϊ���λ�����Խϳ�������Ϊ׼
int bigint::toEquelSize(bigint & opl, bigint & opr)
{
	if (opl._size == opr._size) {
		return opl._size;
	}
	else if (opl._size > opr._size) {
		
		int *temp = opr._array;
		opr._array = new int[opl._size];
		for (int x = 0; x < opl._size; x++) {
			opr._array[x] = 0;
		}
		int x;
		int y;
		for ( x = opl._size - 1, y = opr._size - 1; x >=0 && y >= 0; x--,y--) {
			
				opr._array[x] = temp[y];
		}
		opr._size = opl._size;
		delete[] temp;
		return opl._size;
	}
	else if (opl._size < opr._size) {
	
		int *temp = opl._array;
		opl._array = new int[opr._size];
		for (int x = 0; x < opr._size; x++) {
			opl._array[x] = 0;
		}
		int x;
		int y;
		for (x = opr._size - 1, y = opl._size - 1; x >= 0 && y >= 0; x--, y--) {

			opl._array[x] = temp[y];
		}
		opl._size = opr._size;
		delete[] temp;
		return opr._size;
	}
	
	
	
}
//��������ǽ������ǰ������0ȥ���������ȥ�����ڼ���10+2+3ʱ���ܻ������
void bigint::format(bigint & op)
{
	int realLarge = 0;

	for (int x = 0; x < op._size; x++) {
		if ( op._array[x] != 0) {
			realLarge= op._size - x;
			break;
		}
	}

	if (realLarge == 0) {
		realLarge = 1;
	}
	cout << "op large ��" << op._size << endl;
	
	cout << "realLarge ��" << realLarge << endl;
	int  *temp = op._array;
	op._array = new int[realLarge];


	//cout << "op->_array[0] : " << op._array[1] << endl;
	int x;
	int y;
	for (x = realLarge - 1,y = op._size - 1; x >= 0&&y >= 0; x--,y--) {
		op._array[x] = temp[y];
	}
	op._size = realLarge;
	delete[] temp;
}
//�õ�����Ĵ�С
int bigint::getLength()
{
	return _size;
}
//ԭ���������
void bigint::show()

{
	for (int x = 0; x < _size; x++) {
		cout <<"["<<x<<"]"<< _array[x] << endl;
	}
}
//һά��������Ϊȫ0
void setArr(int *arr, int size) {
	for (int x = 0; x < size; x++) {
		arr[x] = 0;
	}
}
//��ʾһά����
void getArr(int *arr, int size) {
	for (int x = 0; x < size ; x++) {
		cout << arr[x] << ",";
	}
	cout << endl;
}
//�ж�һά�����Ƿ�Ϊ��
bool isEmptyArr(int *arr,int size) {
	
	for (int x = 0; x < size; x++) {
		if (arr[x] != 0) {
			return false;
		}
		else {
			continue;
		}
	}

	return true;
}
//��תһά����
void reverse(int *src, int n) {
	int i, tmp;
	for (i = 0; i<n / 2; i++) {
		tmp = src[i];
		src[i] = src[n - i - 1];
		src[n - i - 1] = tmp;
	}
}
//����һά���飬��λ�������λ��0
void left_n(int *src, int len, int n) {
	n %= len;
	n = (n + len) % len;
	reverse(src, n);
	reverse(src + n, len - n);
	reverse(src, len);
}
//һάbool��������Ϊȫfalse
void setBoolArr(bool *arr,int size) {
	for (int x = 0; x < size; x++) {
		arr[x] = false;
	}
}
//��ʾһάbool����
void getBoolArr(bool *arr,int size) {
	for (int x = 0; x < size; x++) {
		if (arr[x] == true) {
			cout<<"true"<<",";
		}
		else if (arr[x] == false) {
			cout << "fasle" << ",";
		}
	}
	cout << endl;
}


