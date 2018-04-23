#include<vector>
#include<Eigen/Dense>

typedef struct single_measure_by_3_dim //��ʱ�̵ĵ�������ֵ
{
	double r;   //����
	double fai; //fai�Ƕ�
	double theta;//theta�Ƕ�
};
/*typedef struct single_measure_by_2_dim
{
	double fai;
	double theta;
};*/

typedef struct Data_with_time3  //��ʱ�̵�һ������
{
	double time;
	int l;					//��̬����s�ĳ���
	single_measure_by_3_dim *s; //ʱ��time����ά���⶯̬����
};
/*typedef struct Data_with_time2
{
	double time;
	single_measure_by_2_dim *s; //ʱ��time�¶�ά���⶯̬���飨ֻ�������Ƕȣ�
};*/

typedef struct razer_data  //�洢(A,B,C)��ĳһ������������
{
	double time_step;  //����ʱ����
	double whole_time; //�ܲ���ʱ��
	int Tlong;        //��̬�����ܳ���
	Data_with_time3 *d; //��̬����
};

razer_data* ReadFile(std::string filename); //���ļ��ж�������

/*typedef struct wava_data
{
	double time_step;  //����ʱ����
	double whole_time; //�ܲ���ʱ��
	Data_with_time2 *d; //���ײ��״������һ���״�� ��̬����
};*/

typedef struct trace   //������ʼ���صĽṹ��
{
	double final_time;  //������ʼ����ʱ��
	int length;     //��̬���鳤��
	single_measure_by_3_dim *final_trace; //�켣��̬����
}* Trace;



class Regular{	//��ʽ���뷴��ʽ���� ��3��(r,fai,theta)�ȵõ�vecotr<double>���ٵõ�vectorxd v(9)
	public:
		static std::vector<double> FormVector(Trace t); //��3��(r,fai,theta)�ȼ���vecotr<double>
		static Eigen::VectorXd Regularize(std::vector<double> d); //��vecotr<double>���ٵõ�vectorxd v(9)
		static Eigen::Matrix<double,9,9> FormCovMatrix(Trace t); //�γ�9*9��Э�������
		/*

		InverRegular����ʽ����ʱ����
		*/
};

bool Constrain(single_measure_by_3_dim w,single_measure_by_3_dim v,double t); //Լ������ 

//�Ӵ�������״�����ʼ��������
class start_trace{
	public:
		start_trace(razer_data *r); //���캯��
		Trace get_a_start_trace(bool(*pf)(single_measure_by_3_dim,single_measure_by_3_dim,double t)); //��������*rd����������ʼһ������ �β�Ϊһ��Լ������ָ��
		~start_trace();

	private:
		razer_data *rd;
};

class TimeAssociate{
	public:
		static Eigen::VectorXd Vec_Ass(Eigen::Matrix<double,9,9> MoveMat,Eigen::VectorXd v,double t1,double t2);//�����������t1�����˶�ѧ����ת����t2
		static Eigen::Matrix<double,9,9> Cov_Mat_Ass(Eigen::Matrix<double,9,9> MoveMat,Eigen::Matrix<double,9,9> m,double t1,double t2);
};

class PKF{  //�������˲��Ӻ����Ĺ���
	private:
		Eigen::Matrix<double,9,9> P_prev; //no bias estimate of previous covariance
		Eigen::VectorXd x_prev;			//error???????
		Eigen::Matrix<double,9,9> F;  //sport function
		Eigen::Matrix<double,9,9> Q,R; //noise covariance
		Eigen::Matrix<double,9,9> H;  //projcet
	
	public:
		PKF();
		PKF(Eigen::Matrix<double,9,9> covar,Eigen::VectorXd x);
		Eigen::Matrix<double,9,9> get_mat(char type);
		Eigen::VectorXd get_vector();
		void set_mat(char type,Eigen::Matrix<double,9,9> temp);
		void set_vector(Eigen::VectorXd v);
		Eigen::VectorXd update(Eigen::VectorXd z_k);  //�������˲���ʱ�� k+1״̬����
		Eigen::VectorXd updata(Data_with_time3 *data); //ֱ����ĳһ��tʱ�̵�n����ά״̬�����и���

		//Eigen::VectorXd merge();//�ںϲ���
};