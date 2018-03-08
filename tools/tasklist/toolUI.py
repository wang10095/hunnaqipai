#coding=utf-8
from Tkinter import * 
import tkMessageBox
import getopt
import traceback
import os
import sys

def menu(dev_tool,config):
	action_keys = dev_tool.actions['keys']
	action_values = dev_tool.actions['values']

	root = Tk()
	root.title("tools!")
	root.geometry("%dx%d+%d+%d" % (450,600,100,100))
	Message(root,text = 'welcome you to game',width = 300,bg = "green",pady = 10,justify = LEFT).pack()

	def callback(num):
		fuc_callback = action_values[action_keys[num]]["callback"]
		fuc_callback(config)
	for i in xrange(len(action_keys)):
		key = action_keys[i]
		fuc_name = action_values[key]["name"]
		win_t = Frame(root,width=300, height=30)
		win_t.grid_propagate(False)
		win_t.pack(anchor = "w")
		Button(win_t,text = str(i+1),pady = 4,width = 5,command = lambda v = i :callback(v)).grid(row=i, column=0)
		Message(win_t,text = fuc_name,width = 300,pady = 4,justify = LEFT).grid(row=i, column=1)

	win_bot = Frame(root,width=440, height=500, bg='red')
	win_bot.propagate(False)
	win_bot.pack()
	exit = Button(win_bot,text = "quit",command = lambda :root.destroy())#width = 50,bg = "red",fg = "green",height = 20
	exit.bind("<Return>",lambda e:root.destroy())
	exit.focus_set()
	exit.pack(expand=YES,fill=BOTH)
	root.mainloop()

def big_alert(msg):
    print '='*80
    print msg
    print '='*80
try:
    import __init__ as dev_tool
    from dependencies import yaml
except ImportError:
    traceback.print_exc()
    big_alert('can not find a directory')
    sys.exit(1)

config_filenames = ['../config.yaml']
config = {}
for f in config_filenames:
    if os.path.exists(f) :
        d = yaml.load(open(f))
        if d:
            config = dict(config.items() + d.items())
# print(config)
menu(dev_tool,config)