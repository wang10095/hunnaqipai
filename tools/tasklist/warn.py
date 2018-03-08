#coding=utf-8
from Tkinter import * 
import tkMessageBox
def warn(str):
	win = Tk()
	win.title("warning!")
	win.attributes("-topmost",1)
	win.geometry("%dx%d+%d+%d" % (300,120,100,100))
	v = StringVar()
	v.set(str)
	Message(win,text = '',textvariable=v,width = 300,bg="red",pady = 10).pack()
	exit = Button(win,text = "quit",width = 20,height = 1,command = lambda :win.destroy())
	exit.bind("<Return>",lambda e:win.destroy())
	exit.focus_set()
	exit.pack(side = "bottom",ipady = 20,ipadx = 20)
	win.mainloop()

# assert(False,warn("不能干什么，你是不是一个傻逼。傻逼就是你。你咋还不还不知道呢"))
# warn("不能干什么，你是不是一个傻逼。傻逼就是你。你咋还不还不知道呢")