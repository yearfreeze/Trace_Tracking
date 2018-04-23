#include<iostream>
#include<Eigen/Dense>
#include"DataDesign.h"
using namespace std;
using namespace Eigen;  

PKF::PKF(){
	Q.setIdentity();
	Q*=0.1;
	R.setIdentity();
	R*=0.01;

	F.setIdentity();
	H.setIdentity();
}
PKF::PKF(Matrix<double,9,9> covar,VectorXd x){
	Q.setIdentity();
	Q*=0.1;
	R.setIdentity();
	R*=0.01;

	F.setIdentity();
	H.setIdentity();

	P_prev=covar;
	x_prev=x;
}
Eigen::Matrix<double,9,9> PKF::get_mat(char type){
	switch (type){
			case 'F':
				return F;
			case 'Q':
				return Q;
			case 'R':
				return R;
			case 'H':
				return H;
			case 'P':
				return P_prev;
			default:
				break;
			}
		}
Eigen::VectorXd PKF::get_vector(){
	return x_prev;
}
void PKF::set_mat(char type,Matrix<double,9,9> temp){
	switch (type)
	{
			case 'F':
				F=temp;
			case 'Q':
				Q=temp;
			case 'R':
				R=temp;
			case 'H':
				H=temp;
			case 'P':
				P_prev=temp;
	}
}
void PKF::set_vector(VectorXd v){
	x_prev=v;
}
VectorXd PKF::update(VectorXd z_k){
			//define serval vars
			Matrix<double,9,9> p_posterior_bias,Kk,p_k,IDent;
			VectorXd x_posterior_bias,x_k;
			IDent.Identity(9,9);
			//KF algorithm
			x_posterior_bias=F*x_prev;
			p_posterior_bias=F*P_prev*F.adjoint()+Q;
			Kk=p_posterior_bias*H.adjoint()*(H*p_posterior_bias*H.adjoint()+R).inverse(); //klamon 

			x_k=x_posterior_bias+Kk*(z_k-H*x_posterior_bias);
			p_k=(IDent-Kk*H)*p_posterior_bias;

			//update
			x_prev=x_k;
			P_prev=p_k;
			return x_k;
}



/*int main(){
	
	VectorXd v(9);
	v<<1,2,3,4,5,4,3,2,1;
	VectorXd z(9);
	z<<1,2,3,3,3,3,3,2,1;
	Matrix<double,9,9> rm;
	rm.setRandom(9,9);

	cout<<"p_prev is"<<endl;
	cout<<rm;
	cout<<"\r\n";
	PKF *kf=new PKF(rm,v);
	v=kf->update(z);
	cout<<v;
	//cout<<kf->get_mat('R');
	return 0;
}*/
