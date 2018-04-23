#include<iostream>
#include<vector>
#include<list>
#include<Eigen/Dense>
#include <fstream>
#include"DataDesign.h"

using namespace std;
using namespace Eigen;

#define Vmax 450
#define Vmin 150
#define INF 987654321
razer_data* ReadFile(string filename){  //��matlab���ɵ��ı��ļ��ж�������
	ifstream fid(filename);  //�ļ���
	int SizeOfFile,length;   
	double TimeKick;
	double d,f,t;           //r,fai,theta
	char buff[256];		   //������
	razer_data *Universe=(razer_data*)malloc(sizeof(razer_data));    //�����������ݵ����ݽṹ
	if(!fid){
		cout<<"file is not exist or open filed"<<endl;
		return NULL;
	}
	else{
		fid.getline(buff,20);
		sscanf(buff,"total=%d",&SizeOfFile);										//�����ܳ���
		Universe->Tlong=SizeOfFile;
		Universe->time_step=0.01;
		Universe->whole_time=Universe->Tlong*0.01;
		Universe->d=(Data_with_time3*)malloc(sizeof(Data_with_time3)*SizeOfFile); //�����㹻��С���ڴ�
		for(int i=0;i<SizeOfFile;i++){
			memset(buff,0,256);													//���
			fid.getline(buff,20);
			sscanf(buff,"t=%lf n=%d",&TimeKick,&length);
			Universe->d[i].time=TimeKick;											//��������
			Universe->d[i].l=length;
			Universe->d[i].s=(single_measure_by_3_dim*)malloc(sizeof(single_measure_by_3_dim)*length);
			for(int j=0;j<length;j++){
				memset(buff,0,256);													//���
				fid.getline(buff,256);
				sscanf(buff,"%lf %lf %lf",&d,&f,&t);
				Universe->d[i].s[j].r=d;
				Universe->d[i].s[j].fai=f;
				Universe->d[i].s[j].theta=t;
			}
			printf("This is %d times Read,get %f,%d\r\n",i,TimeKick,length);
		}

		return Universe;
	}

}
bool Constrain(single_measure_by_3_dim w,single_measure_by_3_dim v,double t=0.1){
	if(abs(v.r-w.r)<100)
		return true;
	else
		return false;
}
single_measure_by_3_dim FindClose(single_measure_by_3_dim x,Data_with_time3 d){
	double min=INF;
	single_measure_by_3_dim temp;
	for(int i=0;i<d.l;i++){
		if(abs(d.s[i].r-x.r)<min){
			min=abs(d.s[i].r-x.r);
			temp=d.s[i];
		}

	}
	//cout<<"close is"<<temp.r;
	return temp;
}

start_trace::start_trace(razer_data *r){
	rd=r;
	cout<<"class star_trace initialized successfully\r\n";
}
Trace start_trace::get_a_start_trace(bool(*pf)(single_measure_by_3_dim,single_measure_by_3_dim,double t)){
	Data_with_time3 p=rd->d[0];
	Data_with_time3 q=rd->d[1];
	Data_with_time3 r=rd->d[2];
	Data_with_time3 s=rd->d[3];

	vector<list<single_measure_by_3_dim>> v;	//������ܵĺ������ȴ�����
	for(int i=0;i<p.l;i++){
		for(int j=0;j<q.l;j++){
			list<single_measure_by_3_dim> temp; //�ֲ���������ʱ�洢�б�
			temp.push_back(p.s[i]);
			if(Constrain(p.s[i],q.s[j])){
				temp.push_back(q.s[j]);
				v.push_back(temp);
			}

		}
	}
	vector<list<single_measure_by_3_dim>>::iterator i=v.begin();
	while(i!=v.end()){
		//cout<<(*i).back().r;
		single_measure_by_3_dim next_point=FindClose((*i).back(),r);
		if(Constrain((*i).back(),next_point)){
			(*i).push_back(next_point);
			i++;
		}
		else{
			i=v.erase(i);     //vector.erase�����β�����ָ��ĵ���������һ��������
		}	
	}
	vector<list<single_measure_by_3_dim>>::iterator ij=v.begin();
	while(ij!=v.end()){
		//cout<<(*i).back().r;
		single_measure_by_3_dim next_point=FindClose((*ij).back(),s);
		if(Constrain((*ij).back(),next_point)){
			(*ij).push_back(next_point);
			ij++;
		}
		else{
			ij=v.erase(ij);     //vector.erase�����β�����ָ��ĵ���������һ��������
		}	
	}
		cout<<"***********************\r\n";
			cout<<"***********************\r\n";
		vector<list<single_measure_by_3_dim>>::iterator ii=v.begin();
	    for(;ii!=v.end();ii++){
			list<single_measure_by_3_dim>::iterator jj=(*ii).begin();
			for(;jj!=(*ii).end();jj++){
				cout<<(*jj).r<<"\r\n";		
			}
			cout<<"\r\n";
	    }


		vector<list<single_measure_by_3_dim>>::iterator WriteInTrace=v.begin();

		Trace Have_Start_trace=(Trace)malloc(sizeof(trace));  //���ò���
		Have_Start_trace->length=(*WriteInTrace).size();
		Have_Start_trace->final_time=Have_Start_trace->length*0.01; 
		Have_Start_trace->final_trace=(single_measure_by_3_dim*)malloc(sizeof(single_measure_by_3_dim)*Have_Start_trace->length);
		int Increas=0;

		list<single_measure_by_3_dim>::iterator iter=(*WriteInTrace).begin();
		for(;iter!=(*WriteInTrace).end();iter++){
			Have_Start_trace->final_trace[Increas++]=(*iter);
		}
		
	assert(Have_Start_trace);	//��ָ����
	return Have_Start_trace;
}
start_trace::~start_trace(){
	//ʲô������
}
std::vector<double> Regular::FormVector(Trace t){ //9ά�������(x,vx,ax,y,vy,ay,z,vz,az)
	int length=t->length;
	vector<double> cage;   //ʹ��STLģ��ָ����Ҫ��ʽ����
	if(length!=4){
		cout<<"Trace Start Failed Not Equals 4\r\n";
	}
	else{
		//ʹ�ù켣�����������-�������
		//....
		double x=t->final_trace[t->length-1].r;
		double y=t->final_trace[t->length-1].fai;
		double z=t->final_trace[t->length-1].theta;
		cage.push_back(x);
		cage.push_back(0);
		cage.push_back(0);
		cage.push_back(y);
		cage.push_back(0);
		cage.push_back(0);
		cage.push_back(z);
		cage.push_back(0);
		cage.push_back(0);
	}
	return cage;
}
Eigen::VectorXd Regular::Regularize(std::vector<double> d){ //�γ�vectorXd v(9);
	int length=d.capacity();
	VectorXd v(length);  //����v(9)
	for(int i=0;i<length;i++){
		v(i)=d.at(i);	
	}
	return v;
}
Eigen::Matrix<double,9,9> Regular::FormCovMatrix(Trace t){ //���óɵ�λ��
	Matrix<double,9,9> m;
	m.setIdentity();
	return m;
}

Eigen::VectorXd TimeAssociate::Vec_Ass(Eigen::Matrix<double,9,9> MoveMat,Eigen::VectorXd v,double t1,double t2){
	VectorXd nv(9);
	nv.setIdentity();
	if(t2>t1){
		nv=(t2-t1)*MoveMat*v;
	}
	else{
		cout<<"Parmater t2 must lager than t1\r\n";
	}
	return nv;
}

Eigen::Matrix<double,9,9> TimeAssociate::Cov_Mat_Ass(Eigen::Matrix<double,9,9> MoveMat,Eigen::Matrix<double,9,9> m,double t1,double t2){
	Matrix<double,9,9> nm;
	Matrix<double,9,9> T_Move_Mat;
	T_Move_Mat=(t2-t1)*MoveMat;
	nm=T_Move_Mat*m*T_Move_Mat.adjoint(); 
	return nm;
}
double ParmaterEstimer(single_measure_by_3_dim x,single_measure_by_3_dim y,single_measure_by_3_dim z,double StartTime,double TimeKick=0.01){
	//������׵Ĳ������� ax2+bx+c StartTime����ʼʱ��㣬TimeKickʱ����
	double p,q,r;
	Vector3d b;
	Matrix<double,3,3> A;
	Vector3d par;
	p=x.r;
	q=y.r;
	r=z.r;

	b<<p,q,r;
	A<<StartTime*StartTime,StartTime,1,
		(StartTime+TimeKick)*(StartTime+TimeKick),(StartTime+TimeKick),1,
		(StartTime+2*TimeKick)*(StartTime+2*TimeKick),(StartTime+2*TimeKick),1;

	par=A.inverse()*b;
	Vector3d PeNext;
	PeNext<<(StartTime+3*TimeKick)*(StartTime+3*TimeKick),(StartTime+3*TimeKick),1;
	return par.adjoint()*PeNext;

}


int main(){
	string f="razer_data.txt";
	razer_data *Sample_data=ReadFile(f);
	start_trace *st=new start_trace(Sample_data);
	Trace NewT=st->get_a_start_trace(Constrain);
	//for(int fuck=0;fuck<4;fuck++)
	//	cout<<NewT->final_trace[fuck].r<<"\r\n";

	vector<double> q=Regular::FormVector(NewT);
	
	VectorXd v1=Regular::Regularize(q);
	Matrix<double,9,9> m=Regular::FormCovMatrix(NewT);
	cout<<"After Regularize the estimate conv mattix product vector\r\n"<<m*v1;
	
	cout<<"***********************\r\n";
	cout<<"***********************\r\n";
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
}
