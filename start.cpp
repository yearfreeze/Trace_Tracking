#include<iostream>
#include<vector>
#include<Eigen/Dense>

using namespace std;
using namespace Eigen;

bool vector_equals(VectorXd v,VectorXd w){
	for(int i=0;i<3;i++)
		if(v(i)!=w(i))
			return false;
	return true;
}


class TreeNonde{
	public:
		TreeNonde(){
			val.setZero();
			son.clear();
		}
		TreeNonde(VectorXd v){
			val=v;
			son.clear();
		}
		void Add_son(VectorXd x){
			TreeNonde *tn=new TreeNonde(x);
			son.push_back(tn);
		}
		VectorXd get(){
			return val;
		}

		void Destory_son(VectorXd x){
			vector<TreeNonde*>::const_iterator i=son.begin();
			for(;i!=son.end();i++){
				if(vector_equals((*i)->get(),x)){		
					delete(*i);
					son.erase(i);
					break;
				}				
			}
		}
	/*	void mysize(){
			vector<TreeNonde*>::const_iterator i=son.begin();
			for(;i!=son.end();i++){
				cout<<(*i)->get();
				cout<<"\r\n";
			}

		}*/

	private:
		VectorXd val;
		vector<TreeNonde*> son;
};

class Data{
	public:
		Data(vector<VectorXd> a,vector<VectorXd> b,vector<VectorXd> c,vector<VectorXd> d){
			L1=a;
			L2=b;
			L3=c;
			L4=d;
		}

	private:
		vector<VectorXd> L1;
		vector<VectorXd> L2;
		vector<VectorXd> L3;
		vector<VectorXd> L4;
};


/*int main(){
	Vector3d x,y,z;
	x<<1,1,1;
	y<<2,2,2;
	z<<3,3,3;
	TreeNonde *p=new TreeNonde(x);
	p->Add_son(y);
	p->Add_son(z);

	cout<<"........."<<endl;
	p->Destory_son(z);

	//p->mysize();
	
	VectorXd v(3);
	v<<1,2,3;
	//cout<<v;
	Matrix<double,3,1> m,n;
	m<<1,
		2,
		3;
	cout<<m;
	n<<4,
		5,
		6;
	n.adjoint();
	cout<<n;
	return 0;
}*/
