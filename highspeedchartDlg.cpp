
// highspeedchartDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "highspeedchart.h"
#include "highspeedchartDlg.h"
#include "afxdialogex.h"



#include"GenData.h"
#include"core.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


/*
���������ݼ��ϣ���Dlg���캯����init
*/
double *common_p=NULL;
double *common_q=NULL;
double *common_r=NULL;
double *common_ans=new double[100];


double *common_tp=NULL;
double *common_tq=NULL;
double *common_tr=NULL;
double *common_archer=new double[100];

// ����Ӧ�ó��򡰹��ڡ��˵���� CAboutDlg �Ի���

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// �Ի�������
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

// ʵ��
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// ChighspeedchartDlg �Ի���



ChighspeedchartDlg::ChighspeedchartDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(ChighspeedchartDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);

	timer=0;
	mult_edit_string=_T("");
	time_performance=0;
	MSE_control=0;

	common_p=NoiseData(100,0,30,50,-5,0.01);
	common_q=PureData(100,0);
	//common_r=FakeData(100,0,30,50,-5);
	common_r=NoiseData(100,0,40,50,-5,0.03);
	sys=new systems();

	common_tp=NoiseData(100,1,30,50,-2,0.03);  
	common_tq=PureData(100,1);
	common_tr=NoiseData(100,1,40,55,-2,0.03);
	archer_sys=new systems();
}

void ChighspeedchartDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX,IDC_ChartCtrl,my_ChartCtrl);
	DDX_Control(pDX,IDC_Another_ChartCtrl,my_ChartCtrl2);
	DDX_Text(pDX, IDC_MULT_EDIT, mult_edit_string);
	DDX_Text(pDX, IDC_EDIT_TIME, time_performance);
	DDX_Text(pDX, IDC_EDIT_MSE, MSE_control);
}

BEGIN_MESSAGE_MAP(ChighspeedchartDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//ON_BN_CLICKED(IDOK, &ChighspeedchartDlg::OnBnClickedOk)
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_BUTTON1, &ChighspeedchartDlg::OnBnClickedButton1)
	ON_EN_CHANGE(IDC_EDIT1, &ChighspeedchartDlg::OnEnChangeEdit1)
END_MESSAGE_MAP()


// ChighspeedchartDlg ��Ϣ�������

BOOL ChighspeedchartDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// ��������...���˵�����ӵ�ϵͳ�˵��С�

	// IDM_ABOUTBOX ������ϵͳ���Χ�ڡ�
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// ���ô˶Ի����ͼ�ꡣ��Ӧ�ó��������ڲ��ǶԻ���ʱ����ܽ��Զ�
	//  ִ�д˲���
	SetIcon(m_hIcon, TRUE);			// ���ô�ͼ��
	SetIcon(m_hIcon, FALSE);		// ����Сͼ��

	// TODO: �ڴ���Ӷ���ĳ�ʼ������
	CChartAxis *pAxis=NULL;
	CChartAxisLabel *plabel=NULL;
	pAxis=my_ChartCtrl.CreateStandardAxis(CChartCtrl::BottomAxis);
	pAxis->SetAutomatic(true);
  
	plabel=pAxis->GetLabel();
	plabel->SetText(TEXT("time step"));

	pAxis=my_ChartCtrl.CreateStandardAxis(CChartCtrl::LeftAxis);
	pAxis->SetAutomatic(true);
	
	TChartString str1;  
	str1 = _T("title");  
	my_ChartCtrl.GetTitle()->AddString(str1); 

	/*
	this init Another chartctrl
	*/
	CChartAxis *pAs=NULL;
	CChartAxisLabel *pll=NULL;
	pAs=my_ChartCtrl2.CreateStandardAxis(CChartCtrl::BottomAxis);
	pAs->SetAutomatic(true);
  
	pll=pAs->GetLabel();
	pll->SetText(TEXT("yearfreeze"));

	pAs=my_ChartCtrl2.CreateStandardAxis(CChartCtrl::LeftAxis);
	pAs->SetAutomatic(true);
	
	TChartString str2;  
	str2 = _T("titles");  
	my_ChartCtrl2.GetTitle()->AddString(str2); 

	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
}

void ChighspeedchartDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// �����Ի��������С����ť������Ҫ����Ĵ���
//  �����Ƹ�ͼ�ꡣ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ�

void ChighspeedchartDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // ���ڻ��Ƶ��豸������

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// ʹͼ���ڹ����������о���
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// ����ͼ��
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//���û��϶���С������ʱϵͳ���ô˺���ȡ�ù��
//��ʾ��
HCURSOR ChighspeedchartDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


void ChighspeedchartDlg::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ

	//����ʱ�����
	DWORD start_time,end_time;
	start_time=GetTickCount();

	/*
	��ͼ����������timer��
	*/
	my_ChartCtrl.EnableRefresh(true);

	double x[100],y[100],z[100],ans[100]; //x=p,y=q,z=r,ans=returnvalue
	double axis[100];
	int temp=0;
	double ReturnValue;
	char StringBuffer[50]={'\0'};
	
	for(int i=0;i<100;i++){
		x[i]=y[i]=z[i]=ans[i]=0;
		axis[i]=i;
	}

	sprintf(StringBuffer,"%lf#%lf#%lf",common_p[timer],common_q[timer],common_r[timer]);
	ReturnValue=sys->returnData(StringBuffer);
	common_ans[timer]=ReturnValue;
	for(int j=100-timer;j<100;j++){
		x[j]=common_p[temp];
		y[j]=common_q[temp];
		z[j]=common_r[temp];
		ans[j]=common_ans[temp];
		temp++;
	}
	COLORREF SerieColor1 = RGB(255,0,0);
	COLORREF SerieColor2= RGB(255,255,0);
	COLORREF SerieColor3 = RGB(0,0,255);
	COLORREF SerieColor4 = RGB(0,0,0);
	
	my_ChartCtrl.GetLegend()->SetVisible(true);
	my_ChartCtrl.GetLegend()->SetHorizontalMode(true);
	my_ChartCtrl.GetLegend()->UndockLegend(50,50);

	CChartLineSerie *cls1,*cls2,*cls3,*cls4;
	my_ChartCtrl.RemoveAllSeries();  //clear

	cls1=my_ChartCtrl.CreateLineSerie();
	cls1->SetSeriesOrdering(poNoOrdering);
	cls1->AddPoints(axis,x,100);
	cls1->SetColor(SerieColor1);
	cls1->SetName(_T("laser"));

	cls2=my_ChartCtrl.CreateLineSerie();
	cls2->SetSeriesOrdering(poNoOrdering);
	cls2->AddPoints(axis,y,100);
	cls2->SetColor(SerieColor2);
	cls2->SetName(_T("infrare"));

	cls3=my_ChartCtrl.CreateLineSerie();
	cls3->SetSeriesOrdering(poNoOrdering);
	cls3->AddPoints(axis,z,100);
	cls3->SetColor(SerieColor3);
	cls3->SetName(_T("millmeter"));

	cls4=my_ChartCtrl.CreateLineSerie();
	cls4->SetSeriesOrdering(poNoOrdering);
	cls4->AddPoints(axis,ans,100);
	cls4->SetColor(SerieColor4);
	cls4->SetName(_T("return"));
	
	/*
	��������һ���Ƕ�
	*/

	my_ChartCtrl2.EnableRefresh(true);

	double xx[100],yy[100],zz[100],archer[100]; //xx=p,yy=q,zz=r,archer=returnvalue
	int tp=0;
	double RV;
	char SBuffer[50]={'\0'};
	
	for(int i=0;i<100;i++){
		xx[i]=yy[i]=zz[i]=archer[i]=0;

	}

	sprintf(SBuffer,"%lf#%lf#%lf",common_tp[timer],common_tq[timer],common_tr[timer]);
	RV=archer_sys->returnData(SBuffer);
	common_archer[timer]=RV;
	for(int j=100-timer;j<100;j++){
		xx[j]=common_tp[tp];
		yy[j]=common_tq[tp];
		zz[j]=common_tr[tp];
		archer[j]=common_archer[tp];
		tp++;
	}

	my_ChartCtrl2.GetLegend()->SetVisible(true);
	my_ChartCtrl2.GetLegend()->SetHorizontalMode(true);
	my_ChartCtrl2.GetLegend()->UndockLegend(50,50);

	CChartLineSerie *cls5,*cls6,*cls7,*cls8;
	my_ChartCtrl2.RemoveAllSeries();  //clear

	cls5=my_ChartCtrl2.CreateLineSerie();
	cls5->SetSeriesOrdering(poNoOrdering);
	cls5->AddPoints(axis,xx,100);
	cls5->SetColor(SerieColor1);
	cls5->SetName(_T("laser"));

	cls6=my_ChartCtrl2.CreateLineSerie();
	cls6->SetSeriesOrdering(poNoOrdering);
	cls6->AddPoints(axis,yy,100);
	cls6->SetColor(SerieColor2);
	cls6->SetName(_T("infrare"));

	cls7=my_ChartCtrl2.CreateLineSerie();
	cls7->SetSeriesOrdering(poNoOrdering);
	cls7->AddPoints(axis,zz,100);
	cls7->SetColor(SerieColor3);
	cls7->SetName(_T("millmeter"));

	cls8=my_ChartCtrl2.CreateLineSerie();
	cls8->SetSeriesOrdering(poNoOrdering);
	cls8->AddPoints(axis,archer,100);
	cls8->SetColor(SerieColor4);
	cls8->SetName(_T("return"));
	
    /*
	�������߻��ƣ���ʼ���
	*/
	end_time=GetTickCount();
	SetDlgItemInt(IDC_EDIT_TIME,end_time-start_time);

	timer++;

	CEdit *ce=(CEdit *)GetDlgItem(IDC_MULT_EDIT);

	CString strText,addText;
	char EditBuffer[50]={'\0'};
	sprintf(EditBuffer,"ReturnValue= %lf, %lf\r\n",ReturnValue,RV);
	addText=EditBuffer;

	ce->GetWindowText(strText);
	strText+=addText;	
	ce->SetWindowText(strText);
	ce->LineScroll(ce->GetLineCount()-1);



	if(timer==100){
		KillTimer(1);
		//SetDlgItemInt(IDC_EDIT_MSE,calcMSE(common_q,ans,100)+calcMSE(common_tq,archer,100));
		UpdateData(true);
		MSE_control=calcMSE(common_q,ans,100)+calcMSE(common_tq,archer,100);
		UpdateData(false);
	}
	CDialogEx::OnTimer(nIDEvent);
}


void ChighspeedchartDlg::OnBnClickedButton1()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	/*
	��ʼ����ʱ��
	*/
	SetTimer(1,200,NULL);
}


void ChighspeedchartDlg::OnEnChangeEdit1()
{
	// TODO:  ����ÿؼ��� RICHEDIT �ؼ���������
	// ���ʹ�֪ͨ��������д CDialogEx::OnInitDialog()
	// ���������� CRichEditCtrl().SetEventMask()��
	// ͬʱ�� ENM_CHANGE ��־�������㵽�����С�
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
}
