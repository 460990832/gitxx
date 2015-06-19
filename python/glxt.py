# -*- coding: utf-8 -*-
"""
Created on Mon Jun 15 22:47:23 2015

@author: 瑾
"""
#修复编码Bug
import sys
reload(sys)
sys.setdefaultencoding("utf-8")
from Tkinter import *
import tkMessageBox
import mysql.connector
#t=mysql.connector.connect(host='www.yjblog.net',user='test2', password='ssgl', database='test2')
t=mysql.connector.connect(host='127.0.0.1',user='root', password='', database='test2')
#修改学生信息界面        
def xiugaixuesheng():
    tl3=Toplevel()
    tl3.title('修改信息')
    tl3.geometry('350x250')
    bt3=Label(tl3,text ="请选择要进行的操作",font=1)
    bt3.place(relx = 0.5,rely = 0.1,anchor = CENTER)
    xzcz1=Radiobutton(tl3,variable =zjsc ,text = '增加学生信息',value = 0)
    xzcz1.place(relx = 0.2,rely = 0.2,anchor = CENTER)
    xzcz2=Radiobutton(tl3,variable =zjsc ,text = '删除学生信息',value = 1)
    xzcz2.place(relx = 0.2,rely = 0.3,anchor = CENTER)
    xganniu=Button(tl3,text = '确定' ,width = 7,height = 1,relief=RIDGE,command=xiugaixueshenghanshu)
    xganniu.place(relx = 0.5,rely = 0.85,anchor = CENTER)
    xhlb=Label(tl3,text='学号')
    xhlb.place(relx = 0.1,rely = 0.4,anchor = CENTER)
    xmlb=Label(tl3,text='姓名')
    xmlb.place(relx = 0.3,rely = 0.4,anchor = CENTER)
    sslb=Label(tl3,text='宿舍号')
    sslb.place(relx = 0.45,rely = 0.4,anchor = CENTER)
    xblb=Label(tl3,text='性别')
    xblb.place(relx = 0.6,rely = 0.4,anchor = CENTER)
    sjlb=Label(tl3,text='手机号')
    sjlb.place(relx = 0.75,rely = 0.4,anchor = W)
    xxkk=Entry(tl3,textvariable = xhxg,width=10)
    xxkk.place(relx = 0.01,rely = 0.5,anchor = W)
    xmkk=Entry(tl3,textvariable = xmxg,width=5)
    xmkk.place(relx = 0.25,rely = 0.5,anchor = W)
    sskk=Entry(tl3,textvariable=ssxg,width=4)
    sskk.place(relx = 0.4,rely = 0.5,anchor = W)
    xbkk=Entry(tl3,textvariable=xbxg,width=3)
    xbkk.place(relx = 0.55,rely = 0.5,anchor = W)
    sjkk=Entry(tl3,textvariable=sjxg,width=11)
    sjkk.place(relx = 0.7,rely = 0.5,anchor = W)
#修改学生信息函数
def xiugaixueshenghanshu():
    if xhxg.get()=="" or xmxg.get()=="" or ssxg.get()=="":
        tkMessageBox.showwarning("错误", "学号、姓名、宿舍号不能为空哦")
    else:        
        if zjsc.get()==0:
            try:
                xg=t.cursor()
                xg.execute("INSERT INTO xuesheng VALUES('%s','%s','%s','%s','%s')" % (xhxg.get(),xmxg.get(),ssxg.get(),xbxg.get(),sjxg.get()))
                t.commit()
                xg.close()
                tkMessageBox.showinfo('提示','添加成功')
            except:
                tkMessageBox.showwarning("错误", "添加学生信息失败")
        if zjsc.get()==1:
            try:
                sc=t.cursor()
                sc.execute("DELETE FROM xuesheng WHERE 学号='%s' AND 姓名 ='%s' AND 宿舍号 = '%s'" % (xhxg.get(),xmxg.get(),ssxg.get()))
                t.commit()
                sc.close()
                tkMessageBox.showinfo('提示','删除成功')
            except:
                tkMessageBox.showwarning("错误", "删除学生信息失败")
#查看宿舍函数
def chakansushe():
    tl = Toplevel()
    tl.title('宿舍信息')
    tl.geometry('400x250')
    cur=t.cursor()
    cur.execute("SELECT * FROM sushe")
    s = cur.fetchall()
    bt1=Label(tl,text ="宿舍号   楼号   人数   性别",font=3)
    bt1.place(relx = 0.4,rely = 0.15,anchor = CENTER)
    yzb=0.3
    for row in s:
        Label(tl,font=2,text ="   %s      %s      %s      %s" % (row[0],row[1],row[2],row[3])).place(relx = 0.39,rely = yzb,anchor = CENTER)
        yzb+=0.1
    cur.close()
#显示结果查询学生结果函数
def chaxunjieguo():
    if  xuehao.get()=="请在此输入学号" or xuehao.get()=="" :
        xuehao.set('0000000')
#        print 'XH'
    if xingming.get()=="请在此输入姓名" or xingming.get()=="":
        xingming.set('name')
 #       print "XM"
    if sushehao.get()=="请在此输入宿舍号"  or sushehao.get()=="":
        sushehao.set('000')
#        print "SSH"
    cxjg=Toplevel()
    cxjg.title('查询结果')
    cxjg.geometry('500x350')
    cxjgts=Label(cxjg,text =" 学号   姓名   宿舍号   性别  手机号",font=2)
    cxjgts.place(relx = 0.5,rely = 0.15,anchor = CENTER)
    cxxs=t.cursor()
    cxxs.execute("SELECT * FROM xuesheng WHERE 学号=%s OR 姓名 ='%s' OR 宿舍号 = %s " % (xuehao.get(),xingming.get(),sushehao.get()))
    cxxsjg = cxxs.fetchall()
    wzz=0.3
    for xx in cxxsjg:
        Label(cxjg,font=1,text ="%-10s  %-3s  %-3s     %-2s  %-11s" % (xx[0],xx[1],xx[2],xx[3],xx[4])).place(relx = 0.5,rely = wzz,anchor = CENTER)
        Checkbutton(cxjg,text = '发送短信').place(relx = 0.92,rely = wzz,anchor = CENTER)       
        wzz+=0.1
    cxxs.close()
def chaxunxuesheng():
    tl2 = Toplevel()
    tl2.title('查询学生')
    tl2.geometry('350x250')
    bt1=Label(tl2,text ="请输入查询信息",font=3)
    bt1.place(relx = 0.5,rely = 0.15,anchor = CENTER)
    shuoming=Label(root,fg = 'red',text = '请选择要进行的操作^_^',font=4)
    shuoming.place(relx = 0.23,rely = 0.11,anchor = CENTER)
    Label(tl2,text='学号').place(relx = 0.25,rely = 0.3,anchor = CENTER)
    Label(tl2,text='姓名').place(relx = 0.25,rely = 0.4,anchor = CENTER)
    Label(tl2,text='宿舍').place(relx = 0.25,rely = 0.5,anchor = CENTER)
    xuehaokuang=Entry(tl2,textvariable = xuehao)
    xuehaokuang.place(relx = 0.5,rely = 0.3,anchor = CENTER)
    
    
    xingmingkuang=Entry(tl2,textvariable = xingming)
    xingmingkuang.place(relx = 0.5,rely = 0.4,anchor = CENTER)
    
    
    sushehaokuang=Entry(tl2,textvariable = sushehao)
    sushehaokuang.place(relx = 0.5,rely = 0.5,anchor = CENTER)
    chaxunanniu=Button(tl2,text = '查询',width = 13,height = 2,relief=RIDGE,command=chaxunjieguo)
    chaxunanniu.place(relx = 0.5,rely = 0.7,anchor = CENTER)
    
def xz():
    if v.get() == 0:
        chakansushe()
    if v.get() ==1 :
        chaxunxuesheng()
    if v.get() == 2:
        xiugaixuesheng()                            

#整体框架
root=Tk()
root.geometry('600x400')
root.title('宿舍管理系统')
#两个按钮
queding=Button(root,text = '确定',command = xz,width = 13,height = 2,relief=RIDGE)
queding.place(relx = 0.8,rely = 0.85,anchor = CENTER)
tuichu=Button(root,text = '退出',command = root.destroy,width = 13,height = 2,relief=RIDGE)
tuichu.place(relx = 0.2,rely = 0.85,anchor = CENTER)
#修改函数所需变量
xhxg=StringVar()
xmxg=StringVar()
ssxg=StringVar()
xbxg=StringVar()
sjxg=StringVar()
xingming=StringVar()
xingming.set("请在此输入姓名")
xuehao=StringVar()
xuehao.set("请在此输入学号")
sushehao=StringVar()
sushehao.set("请在此输入宿舍号")
zjsc=IntVar()
zjsc.set(0)
#主界面判断
v = IntVar()
v.set(0)
#上面的说明
xy1=Radiobutton(root,variable = v,text = '查看宿舍信息',value = 0,font=3)
xy1.place(relx = 0.2,rely = 0.25,anchor = CENTER)
xy2=Radiobutton(root,variable = v,text = '查询学生信息',value = 1,font=3)
xy2.place(relx = 0.2,rely = 0.45,anchor = CENTER)
xy3=Radiobutton(root,variable = v,text = '修改学生信息',value = 2,font=3)
xy3.place(relx = 0.2,rely = 0.65,anchor = CENTER)
root.mainloop()
t.close