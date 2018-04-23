#include<iostream>
#include<list>
#include<vector>
#include<stdlib.h>
using namespace std;

int a[10]={7,2,3,4,5,6,7,8,9,10};
int b[10]={10,11,12,13,14,15,161,17,7,10};

bool Constrain(int a,int b){
	if(b-a<10)
		return true;
	else
		return false;
}

void trace(bool(*fp)(int,int)){
		
		vector<list<int>> v;
		//for(int i=0;i<10;i++){
		for(int j=0;j<10;j++){
			
			list<int> pos;
			pos.push_back(a[0]);
			if(Constrain(a[0],b[j])){
				cout<<b[j]<<"\r\n";
				pos.push_back(b[j]);
				v.push_back(pos);
			}
		}
		//}
		cout<<"***********************\r\n";
		vector<list<int>>::iterator i=v.begin();
	    for(;i!=v.end();i++){
			list<int>::iterator j=(*i).begin();
			for(;j!=(*i).end();j++){
				cout<<*j<<endl;			
			}
	    }
	
}

/*int main(){
	list<int> v;
	v.push_back(1);
	v.push_back(2);
	v.push_back(3);
	v.push_back(4);
	list<int>::iterator i=v.begin();
	for(;i!=v.end();i++)
		cout<<*i;
	trace(Constrain);
	return 0;
}*/