#include<vector>
#include<Eigen/Dense>

typedef struct single_measure_by_3_dim //无时刻的单个量测值
{
	double r;   //距离
	double fai; //fai角度
	double theta;//theta角度
};
/*typedef struct single_measure_by_2_dim
{
	double fai;
	double theta;
};*/

typedef struct Data_with_time3  //带时刻的一行量测
{
	double time;
	int l;					//动态数组s的长度
	single_measure_by_3_dim *s; //时刻time下三维量测动态数组
};
/*typedef struct Data_with_time2
{
	double time;
	single_measure_by_2_dim *s; //时刻time下二维量测动态数组（只有两个角度）
};*/

typedef struct razer_data  //存储(A,B,C)中某一个的整个数据
{
	double time_step;  //采样时间间隔
	double whole_time; //总采样时间
	int Tlong;        //动态数组总长度
	Data_with_time3 *d; //动态数组
};

razer_data* ReadFile(std::string filename); //从文件中读入数据

/*typedef struct wava_data
{
	double time_step;  //采样时间间隔
	double whole_time; //总采样时间
	Data_with_time2 *d; //毫米波雷达和另外一个雷达的 动态数组
};*/

typedef struct trace   //航迹起始返回的结构体
{
	double final_time;  //航迹起始结束时刻
	int length;     //动态数组长度
	single_measure_by_3_dim *final_trace; //轨迹动态数组
}* Trace;



class Regular{	//格式化与反格式化类 由3个(r,fai,theta)先得到vecotr<double>型再得到vectorxd v(9)
	public:
		static std::vector<double> FormVector(Trace t); //由3个(r,fai,theta)先计算vecotr<double>
		static Eigen::VectorXd Regularize(std::vector<double> d); //由vecotr<double>型再得到vectorxd v(9)
		static Eigen::Matrix<double,9,9> FormCovMatrix(Trace t); //形成9*9的协方差矩阵
		/*

		InverRegular反格式化暂时空着
		*/
};

bool Constrain(single_measure_by_3_dim w,single_measure_by_3_dim v,double t); //约束函数 

//从带距离的雷达来起始航迹的类
class start_trace{
	public:
		start_trace(razer_data *r); //构造函数
		Trace get_a_start_trace(bool(*pf)(single_measure_by_3_dim,single_measure_by_3_dim,double t)); //函数根据*rd的内容来起始一条航迹 形参为一个约束函数指针
		~start_trace();

	private:
		razer_data *rd;
};

class TimeAssociate{
	public:
		static Eigen::VectorXd Vec_Ass(Eigen::Matrix<double,9,9> MoveMat,Eigen::VectorXd v,double t1,double t2);//将设计向量从t1根据运动学方程转换到t2
		static Eigen::Matrix<double,9,9> Cov_Mat_Ass(Eigen::Matrix<double,9,9> MoveMat,Eigen::Matrix<double,9,9> m,double t1,double t2);
};

class PKF{  //卡尔曼滤波加航迹的关联
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
		Eigen::VectorXd update(Eigen::VectorXd z_k);  //卡尔曼滤波的时间 k+1状态更新
		Eigen::VectorXd updata(Data_with_time3 *data); //直接由某一个t时刻的n个三维状态来进行更新

		//Eigen::VectorXd merge();//融合操作
};