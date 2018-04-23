#include<iostream>   
#include<windows.h>

using namespace std;
HANDLE HTime;

/*DWORD WINAPI thread(PVOID param){
	WaitForSingleObject(HTime,2000);
	cout<<"Thread is Tricking per 1s\r\n";
	return 0;
}*/

int main(){	
	//HANDLE hand=CreateThread(NULL,0,thread,0,0,NULL);

	LARGE_INTEGER li;
	HTime=CreateWaitableTimer(NULL,false,NULL);
	const int nTimerUnitsPerSecond=10000000;
	li.QuadPart=-(0.01)*nTimerUnitsPerSecond;//触发器设置在set之后0.01s
	SetWaitableTimer(HTime,&li,1000,NULL,NULL,FALSE); //自动触发每间隔1s

	while(TRUE){
		WaitForSingleObject(HTime,2000);
		cout<<"Thread is Tricking per 1s\r\n";
	}

	return 0;
}
