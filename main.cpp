#include <iostream>
#include "bigint.h"

using namespace std;



int main() {

	{

		bigint b1 =  "-22";
		cout << b1 << endl;
		b1.show(); 
		cout<<"length :"<< b1.getLength() << endl;
		cout << "-------" << endl;
		


		bigint b2 =  "2";
		cout << b2 << endl;
		b2.show();
		cout << "length :" << b2.getLength() << endl;
		cout << "-------" << endl;

		
		//++b2;
		//cout << b2 << endl;
		/*bigint b3 = "2222";
		cout << b3 << endl;
		b3.show();
		cout << "length :" << b3.getLength() << endl;
		cout << "-------" << endl;*/


		bigint b4 = b1 / b2 ;
		cout << b4 << endl;

		/*b1.toEquelSize(b1, b2);

		b1.show();
		cout << "length" << b1.getLength() << endl;
		cout << "-------" << endl;
		cout << "length" << b2.getLength() << endl;
		b2.show();

		cout << "cao" << endl;*/
		/*bigint b4 = b1 - b2;
	
		if (b4.isPlus()) {
			cout << "·ûºÅ£º+"<< endl;
		}
		else if (b4.isMinus()){
			cout << "·ûºÅ£º-" << endl;
		}
		else if(b4.isZero()){
			cout << "·ûºÅ£º?" << endl;
		}*/
		
		cout << "\nbye" << endl;
		cin.get();
	}



	return 0;
}