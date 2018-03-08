#coding=utf-8
import os
import sys
import getopt
import traceback
sys.path.append("./tasklist/")
sys.path.append("./support/")
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

# 获取配置文件
config_filenames = ['config.yaml']
config = {}
for f in config_filenames:
    if os.path.exists(f) :
        d = yaml.load(open(f))
        if d:
            config = dict(config.items() + d.items())


action_keys = dev_tool.actions['keys']
action_values = dev_tool.actions['values']
action_indexes = sys.argv[1:]

# 当需要额外参数时，可以在这里进行设置，传递到config中

try:
    optlist,action_indexes = getopt.getopt(sys.argv[1:],'d',["compress=",'scale=','rename='])      
except getopt.GetoptError:
    print 'tool.py 参数有问题'
    sys.exit(2)

for opt in optlist:
    if opt[0] == '--compress':
    	config['compress'] = opt[1]
    elif opt[0] == '--scale' :
        config['scale'] = opt[1]
    elif opt[0] == '--rename' :
        config['rename'] = opt[1]

while True:
    if len(action_indexes):
        action_index = action_indexes.pop(0)
    else:
        print ""
        print "="*30
        print "actions:"
        for i in range(0,len(action_keys)):
            action =action_values[action_keys[i]]
            print "\t",i+1,":",action['name']#.decode('utf-8')
        print "\tq : quit"
        action_index = raw_input("select: ")
    print "\n action [%s]\n"%action_index
    if action_index.isdigit():
        action_index = int(action_index) - 1
        if action_index>=0 and action_index < len(action_keys):
            callback = action_values[action_keys[action_index]]['callback']
            callback(config)
    elif action_index=='q':
        sys.exit(0)
    else:
        if action_values.has_key(action_index):
            action_values[action_index]['callback'](config)

