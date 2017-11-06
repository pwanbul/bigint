/*************************************************
Copyright:Yzr
Author:Yzr
Date:2017-11-1
Description:大数运算，操作数不限定位数，支持加减乘除
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
//构造函数
bigint::bigint()
{
	_array = NULL;
	_size = 0;
	_flag = '*';
}
//构造函数，保存用户的输入，存储在int数组中，符号单独保存，长度为数据的绝对值长度
bigint::bigint(string op)
{
	if (op.at(0) == 45) {
		_flag = '-';
		_size = op.length() - 1;
	}
	else if(op.at(0) == 43){
		cout << "操作数如果为正数，请不要输入\"+\"号" << endl;		
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
//拷贝构造函数
bigint::bigint(const bigint & op)
{
	_size = op._size;
	_flag = op._flag;
	_array = new int[_size];
	for (int x = 0; x < _size; x++) {
		_array[x] = op._array[x];
	}
}
//通过bigint::bigint(int size)构造出来的对象，其符号默认为*,值为零
bigint::bigint(int size)
{
	_size = size;
	_flag = '*';                 
	_array = new int[_size];
	for (int x = 0; x < _size; x++){
		_array[x] = 0;
	}
}
//析构函数
bigint::~bigint()
{
	delete[] _array;
}
//赋值构造函数
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
//重载的加法运算符
bigint bigint::operator+(bigint & opr) 
{	
	bigint tempopl = *this;							//为了防止之后的操作会对原左右操作数产生影响，这里需要为左右操作数产生临时对象
	bigint tempopr = opr;
	int tempLength = toEquelSize(tempopl,tempopr);	//获得操作数中位数较长的操作数的位数
	bigint result = tempLength + 1;					//加法可能产生进位，因此保存最终结果的对象的大小应该 tempLength + 1


	cout << "重载 + "<< endl;
	if (isPlus() && opr.isPlus()) {					//符号的判断，由于操作数的值和符号分开保存，因此可以预先判断出结果的符号
		cout << "+ +" << endl;
		cout << "call add" << endl;
		result._flag = '+';						//左右操作数均为正数，结果为正
		add(tempopl, tempopr, result);			//调用加法，均为引用传递
	}
	else if (isPlus() && opr.isMinus()) {
		cout << "+ -" << endl;
		cout << "call sub" << endl;
		if (isAbsoluteMax(opr) == 0) {			//操作数为一正一负且绝对值相等，结果为零
			result._flag = '+';  
			//这种情况直接输出result  ,因为result虽然被初始化为多个含有多个0的对象，但format之后可以恢复正常（bigint result = tempLength + 1;）
		}
		else if (isAbsoluteMax(opr) == 1) {		//操作数为一正一负且左边大于右边的，调用减法，结果为正数
			result._flag = '+';
			sub(tempopl, tempopr, result);
		}
		else if (isAbsoluteMax(opr) == -1) {	//操作数为一正一负且右边大于左边的，调用减法时要交换参数顺序，结果为负数
			result._flag = '-';
			cout << "这里要交换操作数，把addsub定义成参数传递" << endl;
			sub(tempopr, tempopl, result);
		}
		
	}
	else if (isMinus() && opr.isPlus()) {
		cout << "- +" << endl;
		cout << "call sub" << endl;
		if (isAbsoluteMax(opr) == 0) {					//操作数为一负一正且绝对值大小相等，结果为零
			result._flag = '+';  
		}
		else if (isAbsoluteMax(opr) == 1) {				//操作数为一负一正且左边大于右边，调用减法结果为负数
			result._flag = '-';
			sub(tempopl, tempopr, result);
		}
		else if (isAbsoluteMax(opr) == -1) {			//操作数为一负一正且右边大于左边，调用减法时交换参数顺序，结果为正数
	
			result._flag = '+';
			cout << "这里要交换操作数，把addsub定义成参数传递" << endl;
			sub(tempopr, tempopl, result);
		}
	
	}
	else if (isMinus() && opr.isMinus()) {				//左右操作数均为负数，结果为负
		cout << "- -" << endl;
		cout << "call add" << endl;
		result._flag = '-';
		add(tempopl, tempopr, result);
	}
	//result.show();
	//cout<<"format之后"<< endl;
	format(result);
	//result.show();
	cout << "---------------" << endl;
	return result ;
}
//重载的减法运算符
bigint bigint::operator-(bigint & opr)
{
	bigint tempopl = *this;
	bigint tempopr = opr;
	int tempLength = toEquelSize(tempopl, tempopr);
	bigint result = tempLength + 1;    //防止进位之后位置不够
	cout << "重载 - " << endl;

	if (isPlus() && opr.isPlus()) {
		cout << "+ +" << endl;
		cout << "call sub" << endl;
		if (isAbsoluteMax(opr) == 0) {							//左右操作数均为正数且绝对值大小相等，结果为零
			result._flag = '+';  
			//这种情况直接输出result  ,因为result虽然被初始化为多个含有多个0的对象，但format之后可以恢复正常（bigint result = tempLength + 1;）
		}
		else if (isAbsoluteMax(opr) == 1) {						//左右操作数均为正数且左边大于右边，调用减法，结果为正数
			result._flag = '+';									
			sub(tempopl, tempopr, result);
		}
		else if (isAbsoluteMax(opr) == -1) {					//左右操作数均为正数且右边大于左边，调用减法时交换参数顺序，结果为负数
			result._flag = '-';
			cout << "交换操作数" << endl;
			sub(tempopr, tempopl, result);
		}
	}
	else if (isPlus() && opr.isMinus()) {						
		cout << "+ -" << endl;
		cout << "call add" << endl;
																//操作数一正一负，调用加法，结果为正数
		result._flag = '+';
		add(tempopl, tempopr, result);
		
	}
	else if (isMinus() && opr.isPlus()) {						
		cout << "- +" << endl;
		cout << "call add" << endl;
		result._flag = '-';										//操作数一负一正，调用加法，结果为负数
		add(tempopl, tempopr, result);
	}
	else if (isMinus() && opr.isMinus()) {						
		cout << "- -" << endl;
		cout << "call sub" << endl;
		if (isAbsoluteMax(opr) == 0) {							//操作数均为负数且绝对值大小相等，结果为零
			result._flag = '+';
		}
		else if (isAbsoluteMax(opr) == 1) {						//操作数均为负数且左边大于右边，调动减法，结果为负数
			result._flag = '-';
			sub(tempopl, tempopr, result);
		}
		else if (isAbsoluteMax(opr) == -1) {					//操作数均为负数且右边大于左边，调动减法时交换参数顺序，结果为正数
			result._flag = '+';
			cout << "交换操作数" << endl;
			sub(tempopr, tempopl, result);
		}
	}
	//result.show();
	cout << "format之后" << endl;
	format(result);
	//result.show();
	cout << "---------------" << endl;
	return result;
}
//重载的乘法运算符
bigint bigint::operator*(bigint & opr)
{
	
	bigint tempopl = *this;
	bigint tempopr = opr;
	int tempLength = toEquelSize(tempopl, tempopr);
	bigint result = tempLength * 2;							//防止进位之后位置不够，最终结果的位数不会超过较长乘数的位数的2倍


	cout << "重载 * " << endl;								//这里符号好判断，相同为正，相异为负
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
	//cout<<"format之后"<< endl;
	format(result);
	//result.show();
	cout << "---------------" << endl;
	return result;
	
}
//重载的除法运算符
bigint bigint::operator/(bigint & opr)
{
	bigint tempopl = *this;
	bigint tempopr = opr;							 //这里并没有把操作数对齐成相同的长度
	tempopl._flag = tempopr._flag = '+';			//左右操作数必须设置为正数

	bigint result = tempopl._size;					 //结果的位数不会超过被除数的位数

	cout << "重载 / " << endl;
	if (tempopl.isAbsoluteMax(opr) == 0) {			//左右操作数的绝对值大小一样，结果为1，符号单独判断
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
	else if (tempopr.isAbsoluteMax(tempopl) == 1) {		//除数不能大于被除数
		cout << "除数不能大于被除数" << endl;
		abort();
	}
	else if (tempopl.isAbsoluteMax(opr) > 0){			//被除数大于除数
		if (isPlus() && opr.isPlus()) {
			cout << "+ +" << endl;
			cout << "call div" << endl;
			
			div(tempopl, tempopr, result);
			result._flag = '+';							//这里符号的确定，要放在计算出结果之后
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
	//cout<<"format之后"<< endl;
	format(result);
	//result.show();
	cout << "---------------" << endl;
	return result;


}
//重载的自增运算符
bigint bigint::operator++()
{
	bigint temp = "1";
	*this = *this + temp;
	return *this;
}
/*在进行除法运算时，需要试除，
例如547除以2，先要商100，不行再商200，
这些数可以理解成n*100(n取1，2，3...),
之后还要n*10，这个函数就是来确定基数是100，还是10的*/
bigint bigint::tempQuotient(int wei)
{
	string quotient = "1";
	for (int x = 1 ; x < wei; x++) {
		quotient.append("0");
	}
	return bigint(quotient);
}
/*在计算除法时，需要计算试除的位置，
如120/10 ，cut函数会返回被除数的前除数._size位上的数，
以便除数和它来比较，如12和10比较。
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
//判断操作数的符号
bool bigint::isPlus() const
{
	if (_flag == '+')
		return true;
	else
		return false;
}
//判断操作数的符号
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

//加法函数
void bigint::add(bigint & opl, bigint & opr, bigint & result)
{
	//cout << "opl size :" << opl._size << endl;
	//cout << "opr size:" << opr._size << endl;
	//cout << "result size:" << result._size << endl;

	int *carry = new int[opl._size+1];							//carry数组保存进位数,位数比最大操作数多一位，此时操作数已经对齐
	int *tempresult = new int[opl._size+1];						//tempresult保存中间结果，位数比最大操作数多一位，此时操作数已经对齐
	
	setArr(carry, opl._size+1);									//置0
	setArr(tempresult, opl._size+1);							//置0


	for (int x = opl._size ; x > 0; x--) {						//核心算法，模拟手工计算的方式
		int temp = opl._array[x-1] + opr._array[x-1];
		tempresult[x] = temp % 10;

		if (tempresult[x] != temp) {
			carry[x]++;
		}
	}
	getArr(carry, opl._size+1);									//输出carry数组，进位为1
	getArr(tempresult, opl._size+1);							//输出tempresult数组

		
	left_n(carry, opl._size+1, 1);								//把长度为opl._size+1的carry数组左移1位，低位补0
	cout << "carry 移动后：" << endl;
	getArr(carry, opl._size + 1);
	getArr(tempresult, opl._size + 1);


	bool *flag = new bool[opl._size + 1];						//设置一个bool数组，来保存carry数组和tempresult数组相加后进位的位
	setBoolArr(flag, opl._size + 1);							//置flag数组为false
	//getBoolArr(flag, opl._size + 1);
	for (int x = opl._size ; x >= 0; x--) {						
		int temp = carry[x] + tempresult[x];
		result._array[x] = temp % 10;
		if (result._array[x] != temp) {
			flag[x] = true;
		}
	}
	cout << "未计算bool数组时" << endl;							
	getBoolArr(flag, opl._size + 1);
	getArr(result._array, opl._size + 1);
	cout << "计算bool数组后" << endl;							
	for (int x = opl._size; x >= 0; x--) {						//flag数组中为true的位，表示最后结果中对应位的左一位有进位
		if (flag[x] == true) {
			result._array[x - 1]++;
		}
	}
	getArr(result._array, opl._size + 1);

	delete[] flag;
	delete[] tempresult;
	delete[] carry;
	
}
//减法函数
void bigint::sub(bigint & opl, bigint & opr, bigint & result)
{
	int *carry = new int[opl._size + 1];									
	int *tempresult = new int[opl._size + 1];

	setArr(carry, opl._size + 1);
	setArr(tempresult, opl._size + 1);
	

	for (int x = opl._size; x > 0; x--) {								//借位为-1，保存在carry中
		if (opl._array[x - 1] < opr._array[x - 1]) {					//中间结果保存在tempresult中
			opl._array[x - 1] += 10;
			carry[x]--;
		}
		tempresult[x] = opl._array[x - 1] - opr._array[x - 1];
	}

	getArr(carry, opl._size + 1);
	getArr(tempresult, opl._size + 1);

	
	left_n(carry, opl._size + 1, 1);									//把长度为opl._size+1的carry数组左移1位，低位补0
	cout << "carry 移动后：" << endl;
	getArr(carry, opl._size + 1);
	getArr(tempresult, opl._size + 1);



	for (int x = result._size - 1; x >= 0; x--) {						//把carry和tempresult合并，记得处理100-1这种情况
		int temp = carry[x] + tempresult[x];
		if (temp == -1) {
			result._array[x] = 9;
			tempresult[x - 1]--;
		}
		else {
			result._array[x] = temp;
		}
	}

	cout << "合并carry和tempresult之后" << endl;
	getArr(result._array, opl._size + 1);


	delete[] tempresult;
	delete[] carry;
}
//乘法函数
void bigint::mul(bigint & opl, bigint & opr, bigint & result)
{

	//假定数组第一维长度为m， 第二维长度为n
	

	int **tempresult = NULL, **carry = NULL;
	int i, j, k, q;
	//创建二位数组，最好设计成函数，行数为较大操作数的位数，列数为较大操作数的位数*2
	tempresult = new int *[opl._size];
	carry = new int *[opl._size];
	for (int i = 0; i<opl._size; i++)
	{
		tempresult[i] = new int[opl._size * 2];
		carry[i] = new int[opl._size * 2];
	}
	for (int i = 0; i < opl._size; i++) {				//两个二位数组均置0
		for (int j = 0; j < opl._size * 2; j++) {
			tempresult[i][j] = 0;
			carry[i][j] = 0;
		}
	}

	int xx = 0;											//关键算法，进位保存在carry二位数组中，中间结果保存在tempresult二位数组中
	for (j = opl._size - 1; j >= 0; j--) {
		for (i = opl._size - 1, q = opl._size * 2 - 1, k = opl._size * 2 - 1; i >= 0 && q >= 0 && k >= 0; i--, q--, k--) {

			int temp = opl._array[i] * opr._array[j];
			tempresult[j][k] = temp % 10;
			if (tempresult[j][k] != temp) {
				carry[j][q] = carry[j][q] + temp / 10;
			}
		}
	}
		
	//输出carry数组
	cout << "输出carry数组" << endl;
	for (int i = 0; i < opl._size; i++) {
		cout << i << ":";
		for (int j = 0; j < opl._size * 2; j++) {
			cout << carry[i][j];
		}
		cout << endl;
	}

	//输出tempresult数组 
	cout << "输出tempresult数组 " << endl;
	for (int i = 0; i < opl._size; i++) {
		cout << i << ":";
		for (int j = 0; j < opl._size * 2; j++) {
			cout << tempresult[i][j];
		}
		cout << endl;
	}
	
	
	for (int x = 0; x <opl._size; x++) {					//左移carry和tempresult数组
		left_n(carry[x], opl._size * 2, opl._size - x );
		left_n(tempresult[x], opl._size * 2, opl._size - (x + 1));
	}

	cout << "转换后" << endl;
	//输出carry数组
	cout << "输出carry数组" << endl;
	for (int i = 0; i < opl._size; i++) {
		cout << i << ":";
		for (int j = 0; j < opl._size * 2; j++) {
			cout << carry[i][j];
		}
		cout << endl;
	}

	//输出tempresult数组 
	cout << "输出tempresult数组 " << endl;
	for (int i = 0; i < opl._size; i++) {
		cout << i << ":";
		for (int j = 0; j < opl._size * 2; j++) {
			cout << tempresult[i][j];
		}
		cout << endl;
	}

	int *flag = new int[opl._size * 2];							//flag数组保存，carry和tempresult数组的进位，可能大于1
	setArr(flag, opl._size * 2);								//flag置0
	int temp = 0;
	for (int j = opl._size * 2 - 1; j >= 0; j--) {
		for (int i = opl._size - 1; i >= 0; i--) {
			temp = temp + tempresult[i][j] + carry[i][j];
			result._array[j] = temp % 10;						//中间结果放在result._array数组中
			if (result._array[j] != temp) {
				flag[j] = temp / 10;
			}
		}
		temp = 0;
	}
	cout << "移动flag前 :" << endl;
	getArr(flag, opl._size * 2);								//flag左移，（其实在乘法中flag也可以设计成int类型的）
	cout << "result :" << endl;
	result.show();

	
	cout << "移动flag后 :" << endl;
	

	if (!isEmptyArr(flag, opl._size * 2)) {						//当flag不为空时，将flag和result._array相加，并放入result._array中
		left_n(flag, opl._size * 2, 1);
		getArr(flag, opl._size * 2);
		for (int x = opl._size * 2 - 1; x >= 0; x--) {						
			int temp = result._array[x] + flag[x];
			flag[x] = 0;
			result._array[x] = temp % 10;
			if (result._array[x] != temp) {
				flag[x-1]++;									//2个数相加最多进1位
			}
		}
	}

	
	result.show();
	delete[]  carry;
	delete[]  tempresult;
	delete[]  flag;

}
//除法函数
void bigint::div(bigint & opl, bigint & opr, bigint & result)
{
	/*这种做法效率很低*/
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
	
	if (opr.isAbsoluteMax(cut(opr._size)) == 1) {						//这里需要确定试除的起始位置，通过cut函数，具体参看cut函数
		flag = opr._size;
	}
	else if (opr.isAbsoluteMax(cut(opr._size)) == -1) {
		flag = opr._size - 1;
	}
	else if (opr.isAbsoluteMax(cut(opr._size)) == 0) {
		flag = opr._size - 1;
	}

	cout << "flag : " << flag << endl;

	bigint* temp =  new bigint[opl._size-flag];						//保存中间结果，其个数和试除的位数有关，需要置为bigint("0")
	for (int x = 0; x < opl._size - flag; x++) {
		temp[x] = bigint("0");
	}

	bigint xx = opl;																					
	bigint yy;
	bigint zz ;
	bigint qq;
	bigint ww;
	
	for (int x = opl._size - flag; x > 0; x--) {	//核心算法，模拟手工计算
		
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
		if ((xx.isAbsoluteMax(opr) == -1)&& x==1 ||(xx.isAbsoluteMax(bigint("0")) == 0)) {       //当x==1时，表示到了最后一位，余数小于9即可
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

//isAbsoluteMax函数在比较操作数绝对值大小时，不会受到操作数是否对齐的影响
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
//重载的<<
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
//将操作数转换为相等位数，以较长操作数为准
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
//这个函数是将结果中前面多余的0去掉，如果不去掉，在计算10+2+3时可能会出问题
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
	cout << "op large ：" << op._size << endl;
	
	cout << "realLarge ：" << realLarge << endl;
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
//得到对象的大小
int bigint::getLength()
{
	return _size;
}
//原样输出对象
void bigint::show()

{
	for (int x = 0; x < _size; x++) {
		cout <<"["<<x<<"]"<< _array[x] << endl;
	}
}
//一维数组设置为全0
void setArr(int *arr, int size) {
	for (int x = 0; x < size; x++) {
		arr[x] = 0;
	}
}
//显示一维数组
void getArr(int *arr, int size) {
	for (int x = 0; x < size ; x++) {
		cout << arr[x] << ",";
	}
	cout << endl;
}
//判断一维数组是否为空
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
//翻转一维数组
void reverse(int *src, int n) {
	int i, tmp;
	for (i = 0; i<n / 2; i++) {
		tmp = src[i];
		src[i] = src[n - i - 1];
		src[n - i - 1] = tmp;
	}
}
//左移一维数组，高位溢出，地位补0
void left_n(int *src, int len, int n) {
	n %= len;
	n = (n + len) % len;
	reverse(src, n);
	reverse(src + n, len - n);
	reverse(src, len);
}
//一维bool数组设置为全false
void setBoolArr(bool *arr,int size) {
	for (int x = 0; x < size; x++) {
		arr[x] = false;
	}
}
//显示一维bool数组
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


