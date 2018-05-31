# -*- coding: utf-8 -*-

import wx,wx.grid
import xlrd
import numpy as np
import matplotlib
matplotlib.use('Agg')#绘画标记，否
    
import matplotlib.pyplot as plt
from matplotlib.pyplot import savefig 

iteration=100
threhold=88

def diff(m,cut):
#diff the raw data using the next sub now one
    L=[]
    for i in range(cut):
        temp=m[i+1]-m[i]
        L.append(temp)
    return L


def noise(mu,sigma,num):
	return list(np.random.normal(mu,sigma,num))


def expection(arr):   #Examine variable,such as A
    arr=np.array(arr)
    return [np.mean(arr),np.std(arr)]


def slice(low,high,point_nunm):
    L=[]
    i=low
    step=(high-low)/point_nunm
    while(i<high):
        L.append(i)
        i+=step
    return L


def TUfunction(arr,cut,recover,k=0):#arr是横坐标List，cut是断开点，recover是恢复点，k是错误斜率
    
    y0=10            #角度起始值
    g=0.2

    if(k!=0):
        head=[arr[p] for p in range(len(arr)) if p<cut]
        middle=[arr[p] for p in range(len(arr)) if cut<=p and p<=recover]
        tail=[arr[p] for p in range(len(arr)) if recover<p]
        link_point=head[-1]
        link_point_y=y0-0.5*g*link_point**2
        temp=[y0-0.5*g*t*t for t in head]+[k*t+(link_point_y-k*link_point) for t in middle]+[y0-0.5*g*t*t for t in tail]
    else:
        temp=[y0-0.5*g*t*t for t in arr]
    return temp
        

def AOfunction(arr,cut,recover,k=0):#arr是横坐标List，cut是断开点，recover是恢复点，k是错误斜率
    
    if(k!=0):
        head=[arr[p] for p in range(len(arr)) if p<cut]
        middle=[arr[p] for p in range(len(arr)) if cut<=p and p<=recover]
        tail=[arr[p] for p in range(len(arr)) if recover<p]
        link_point=head[-1]
        link_point_y=(link_point-10)**2
        temp=[0.1*(t-10)**2 for t in head]+[0.1*k*t+0.1*(link_point_y-k*link_point) for t in middle]+[0.1*(t-10)**2 for t in tail]
    else:
        temp=[0.1*(t-10)**2 for t in arr]
    return temp


def merge(a,b):#a and b is lists
    if(len(a)!=len(b)):
        print('parmeter len is not match')
    c=[]
    for i in range(len(a)):
        c.append(a[i]+b[i])
    return c


def AddPercentNoise(MaxNumber,Per,RawData): #RawData is a list,Per is 2%
    NoiseData=noise(0,MaxNumber*Per,len(RawData))
    return merge(NoiseData,RawData)


def CheckOut(Base,data):    #Base is undiff data,parmater data is next point

    dup_Base=[p for p in Base]  #duplicate the Base set
    last_point=dup_Base[len(dup_Base)-1] #last point in dup_base
        
    diff_Base=diff(dup_Base,len(dup_Base)-1)
        
    gate=expection(diff_Base)
    
    print ('gate is******')
    print (gate)
    print ('******')
    
    times=0
    NewP=data-last_point
    diff_Base.append(NewP)
    if(NewP>0):
        updown=1
    else:
        updown=-1
    for i in range(iteration):
        choice_index=np.random.choice(len(diff_Base),len(diff_Base)-1,replace=False)
        
        temp_data=list(np.array(diff_Base)[choice_index])
        #print(temp_data)
        TD_exp=expection(temp_data)
        print(TD_exp)
        if(updown>0):
            if(TD_exp[1]>gate[1] and TD_exp[0]>gate[0]):
                times=times+1
        else:
            if(TD_exp[1]>gate[1] and TD_exp[0]<1.4*gate[0]):
                times=times+1
       
    
    print('check out over')
    print(times)
    if(times>=threhold):
        print ('this point is excatlly a fake point')
        return True
    else:
        print ('cantt sure')
        return False



class Example(wx.Frame):
	def __init__(self, parent, title):
		super(Example, self).__init__(parent, title = title, size = (800,500))
		#self.InitTensorflow()
		self.InitUI()
		self.Centre()
		self.Show()

	def InitUI(self): 
		p=wx.Panel(self) 
		whole_box=wx.BoxSizer(wx.HORIZONTAL)
		#set train static box
		train_box=wx.StaticBox(p,-1,'参数设置:')
		train_box_Sizer=wx.StaticBoxSizer(train_box,wx.VERTICAL)
		pbox=wx.BoxSizer(wx.VERTICAL)
		hbox1 = wx.BoxSizer(wx.HORIZONTAL) 
		hbox2 = wx.BoxSizer(wx.HORIZONTAL) 
		hbox3 = wx.BoxSizer(wx.HORIZONTAL) 
		hbox4 = wx.BoxSizer(wx.HORIZONTAL)
		hbox5 = wx.BoxSizer(wx.HORIZONTAL)
		
		self.noiseST=wx.StaticText(p,-1,style = wx.ALIGN_CENTER)
		self.noiseST.SetLabel('噪声 强度  :')
		noiseLevel=['0.1%','1%','2%','5%','10%']
		self.noiseCombo=wx.ComboBox(p,choices=noiseLevel)
		#self.noiseCombo.Bind(wx.EVT_COMBOBOX,self.Oncombo)#setting dealling function 
		#set hbox1
		hbox1.Add(self.noiseST,0,wx.EXPAND|wx.ALIGN_LEFT|wx.ALL,5)
		hbox1.Add(self.noiseCombo,0,wx.EXPAND|wx.ALIGN_LEFT|wx.ALL,5) 
		
		self.funcST=wx.StaticText(p,-1,style = wx.ALIGN_CENTER)
		self.funcST.SetLabel('函数 形式  :')
		funcLevel=['直线函数','凹函数','凸函数']
		self.funcCombo=wx.ComboBox(p,choices=funcLevel)
		#self.funcCombo.Bind(wx.EVT_COMBOBOX,self.Oncombo) #setting dealling function   ?????set another
		hbox2.Add(self.funcST,0,wx.EXPAND|wx.ALIGN_LEFT|wx.ALL,5)
		hbox2.Add(self.funcCombo,0,wx.EXPAND|wx.ALIGN_LEFT|wx.ALL,5)
		#next
		self.psST=wx.StaticText(p,-1,style = wx.ALIGN_CENTER)  #point setting ST
		self.psST.SetLabel('离群点位置 :')
		self.psTC=wx.TextCtrl(p,style=wx.TE_CENTRE)
		#self.psTC.Bind(wx.EVT_TEXT,self.OnEnterPressed)
		hbox3.Add(self.psST,0,wx.EXPAND|wx.ALIGN_LEFT|wx.ALL,5)
		hbox3.Add(self.psTC,0,wx.EXPAND|wx.ALIGN_LEFT|wx.ALL,5)
		
		self.fireup=wx.Button(p,label="检测") 
		self.fireup.Bind(wx.EVT_BUTTON, self.OnEnterPressed)
		hbox4.Add(self.fireup,0,wx.EXPAND|wx.ALIGN_LEFT|wx.ALL,5)
		
		self.ppST=wx.StaticText(p,-1,style = wx.ALIGN_CENTER) #point power ST
		self.ppST.SetLabel('强         度 :')
		self.ppTC=wx.TextCtrl(p,style=wx.TE_CENTRE)
		#self.ppTC.Bind(wx.EVT_TEXT_ENTER,self.OnEnterPressed)  
		hbox5.Add(self.ppST,0,wx.EXPAND|wx.ALIGN_LEFT|wx.ALL,5)
		hbox5.Add(self.ppTC,0,wx.EXPAND|wx.ALIGN_LEFT|wx.ALL,5)
		
		pbox.Add(hbox1, 0, wx.EXPAND|wx.ALIGN_LEFT|wx.ALL, 5)
		pbox.Add(hbox2, 0, wx.EXPAND|wx.ALIGN_LEFT|wx.ALL, 5) 
		pbox.Add(hbox3, 0, wx.EXPAND|wx.ALIGN_LEFT|wx.ALL, 5) 
		pbox.Add(hbox5, 0, wx.EXPAND|wx.ALIGN_LEFT|wx.ALL, 5) 
		pbox.Add(hbox4, 0, wx.EXPAND|wx.ALIGN_LEFT|wx.ALL, 5) 
		
		train_box_Sizer.Add(pbox, 0, wx.EXPAND|wx.ALIGN_LEFT|wx.ALL, 10) 
		
		#set show static box
		show_box=wx.StaticBox(p,-1,'曲线展示:')
		show_box_Sizer=wx.StaticBoxSizer(show_box,wx.VERTICAL)
		qbox=wx.BoxSizer(wx.VERTICAL)
		
		temp=wx.Image("blank.jpg",wx.BITMAP_TYPE_JPEG).Scale(200,300).ConvertToBitmap()
		self.bmp=wx.StaticBitmap(p,bitmap=temp)
		
		qbox.Add(self.bmp,0,wx.EXPAND|wx.ALIGN_LEFT|wx.ALL, 5)
		show_box_Sizer.Add(qbox, 0, wx.EXPAND|wx.ALIGN_LEFT|wx.ALL, 10)
		#set test static box
		test_box=wx.StaticBox(p,-1,'结果统计:')
		test_box_Sizer=wx.StaticBoxSizer(test_box,wx.VERTICAL)
		rbox=wx.BoxSizer(wx.VERTICAL)
		#compoent setting
		
		self.grid=wx.grid.Grid(p,-1)
		self.grid.CreateGrid(100,3)
		self.grid.SetColLabelValue(0, "serial num")
		self.grid.SetColLabelValue(1, "excatl target")
		self.grid.SetColLabelValue(2, "fake target")
		
		rbox.Add(self.grid, 0, wx.EXPAND|wx.ALIGN_LEFT|wx.ALL, 5)
		
		test_box_Sizer.Add(rbox, 0, wx.EXPAND|wx.ALIGN_LEFT|wx.ALL, 10)
		whole_box.Add(train_box_Sizer,0, wx.EXPAND|wx.ALIGN_LEFT|wx.ALL, 2)
		whole_box.Add(show_box_Sizer,0, wx.EXPAND|wx.ALIGN_LEFT|wx.ALL, 2)
		whole_box.Add(test_box_Sizer,0, wx.EXPAND|wx.ALIGN_LEFT|wx.ALL, 2)
		p.SetSizer(whole_box)
		

	def OnClick(self,event):
		print('OnClick')
	def Oncombo(self,event):
		print('Oncombo')
	def OnEnterPressed(self,event):

		index_noise=self.noiseCombo.GetValue()
		print('choice:%s'%index_noise)
		index_func=self.funcCombo.GetValue()
		print('choice:%s'%index_func)
		index_ps=self.psTC.GetValue()
		print('choice:%s'%index_ps)
		index_pp=self.ppTC.GetValue()
		print('choice:%s'%index_pp)
		self.cut_slope=int(index_pp) 
		self.cut_point=int(index_ps)
		#setting some parmater of class using self.
		#self.check_pos=int(self.jcTC.GetValue())
		
		
		if(index_noise=='0.1%'):
			self.percent=0.001
		if(index_noise=='1%'):
			self.percent=0.01
		if(index_noise=='2%'):
			self.percent=0.02
		if(index_noise=='5%'):
			self.percent=0.05
		if(index_noise=='10%'):
			self.percent=0.10
		if(index_func=='直线函数'):# read data from xlsx
			data=[]
			fake_F=[]
			#Data IO
			init_data=xlrd.open_workbook('razer.xlsx')
			table=init_data.sheets()[0]
			nrow=table.nrows
			for i in range(int(nrow/2)-50,int(nrow/2)+50):
				data.append(table.row_values(i)[0])
				fake_F.append(table.row_values(i)[1])
			plt.clf()
			plt.plot(range(int(nrow/2)-50,int(nrow/2)+50),data,color='red')
			plt.plot(range(int(nrow/2)-50,int(nrow/2)+50),fake_F,color='blue',linestyle='--')
			plt.xlim((450.0,550.0))
			plt.xticks(np.arange(450.0,550.0,10.0))
			
			savefig('rard.jpg')         #save pic file

		if(index_func=='凹函数'):
			data=[]    #data是加过噪声的虚假目标
			fake_F=[]  #fake_F真实
			L=slice(0.1,10.0,100)
			fake_F=AOfunction(L,self.cut_point,self.cut_point+20,0) #just for paint
			
			F=AOfunction(L,self.cut_point,self.cut_point+20,self.cut_slope)
			data=AddPercentNoise(max(F),self.percent,F)
			plt.clf()
			plt.plot(L,data,color='red')
			plt.plot(L,fake_F,color='blue', linestyle='--')
			plt.xlim((0.1,10.0))
			plt.xticks(np.arange(0.1,10.0,0.6))
			savefig('rard.jpg')
			
		if(index_func=='凸函数'):
			data=[]
			fake_F=[]
			L=slice(0.1,10.0,100)
			fake_F=TUfunction(L,self.cut_point,self.cut_point+20,0) #just for paint
			
			F=TUfunction(L,self.cut_point,self.cut_point+20,self.cut_slope)
			data=AddPercentNoise(max(F),self.percent,F)
			plt.clf()
			plt.plot(L,data,color='red')
			plt.plot(L,fake_F,color='blue', linestyle='--')
			plt.xlim((0.1,10.0))
			plt.xticks(np.arange(0.1,10.0,0.6))
			savefig('rard.jpg')
			
		
		rard_Image=wx.Image("rard.jpg",wx.BITMAP_TYPE_JPEG).Scale(500,500).ConvertToBitmap()
		self.bmp.SetBitmap(rard_Image) #write pic
		"""
		if(CheckOut(data[:self.check_pos],data[self.check_pos+1])):
			self.ansTC.SetLabel('离群点')
		else:
			self.ansTC.SetLabel('正常点')
		"""
		previous_set_length=10  #基础判断需要长度
		evidence_excat_data=fake_F[:previous_set_length]
		evidence_fake_data=data[:previous_set_length]
		
		print(data)
		print('***************')
		print(evidence_fake_data)
		print('***************')
		
		for check_index in range(100-previous_set_length):
			self.grid.SetCellValue(check_index, 0, str(previous_set_length+check_index))
			if(CheckOut(evidence_excat_data,fake_F[previous_set_length+check_index])): #为真的时候是虚假点
				self.grid.SetCellTextColour(check_index, 1, wx.RED)
				self.grid.SetCellValue(check_index, 1, 'F')
			else:
				evidence_excat_data.append(fake_F[previous_set_length+check_index])
				self.grid.SetCellValue(check_index, 1, 'T')
				
			
			if(CheckOut(evidence_fake_data,data[previous_set_length+check_index])): #为真的时候是虚假点
				self.grid.SetCellTextColour(check_index, 2, wx.RED)
				self.grid.SetCellValue(check_index,2, 'F')
			else:
				evidence_fake_data.append(data[previous_set_length+check_index])
				self.grid.SetCellValue(check_index,2, 'T')
			
		print(evidence_fake_data)
			
	

app = wx.App() 
Example(None, title = 'None') 
app.MainLoop()