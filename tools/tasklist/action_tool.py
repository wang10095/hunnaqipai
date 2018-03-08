#coding=utf-8
import os,requests,json
from PIL import Image

def run1(config):
	print('oceana run1')
	print(config)

def compressPng(config): # 压缩图片
	print('action_tool compressPng path->',config.has_key('compress'))
	if config.has_key('compress'):
		os.environ['compress'] = config['compress']
	else:
 		os.environ['compress'] = '/data/work/tools/support/outPath'
	cur_path = os.getcwd()+'/support/compress.sh.command'
	os.system(cur_path)

def scalePng(config): # 缩放图片
	print('action_tool scalePng path->',config.has_key('scale'))
	rate = 50
	out_path = "/data/work/tools/support/outPath"
	img_path = ""
	if config.has_key('scale'):
		img_path = config['scale']
	else:
		img_path = "/data/work/tools/support/PNGScale"
	print img_path
	for lists in os.listdir(img_path):
		path = os.path.join(img_path,lists)
		if not os.path.basename(path).startswith('.'):
			img = Image.open(path)
			w = img.size[0] * rate / 100
			h = img.size[1] * rate / 100
			print(path,img,rate,img.size,w,h)
			img.thumbnail((w,h))
			img.save(out_path+"/"+os.path.basename(path))


def renamePng(config): # 重命名图片
	print('action_tool renamePng path->',config.has_key('rename'))
	new_name = ""
	if config.has_key('rename'):
		new_name = config['rename']
	else:
		new_name = "test"
	name_path = "/data/work/tools/support/outPath"
	num = 0
	nums = '%02d' % num 
	for lists in os.listdir(name_path):
		if not lists.startswith('.'):
			path = os.path.join(name_path,lists)
			print path
			num = num + 1
			nums = '%02d' % num 
			print nums
			os.rename(path,os.path.join(name_path,new_name+nums+'.png'))
# 查看版本号
def lookVersion(gid,url):
    headers = {'Content-Type':'application/json; charset=utf-8'}
    payload = {'gameId':'0'}
    payload['gameId'] = gid
    req = requests.post(url,data=json.dumps(payload),headers = headers)
    print req.text
    if req.status_code == requests.codes.ok:
        for k, v in req.json().items():
            # print k, v
            if k == 'data':
                print '------ 当前版本号 gameId: '+gid,'version: '+v
# 更新版本号
def setVersion(gid,version,url):
	headers = {'Content-Type':'application/json; charset=utf-8'}
	payload = {'gameId':'0','version':'0.0.0'}
   	payload['gameId'] = gid
   	payload['version'] = version
	req = requests.post(url,data=json.dumps(payload),headers = headers)
	print req.text
	if req.status_code == requests.codes.ok:
		print '------ 设置版本号 gameId: '+gid,'version: '+version
		
# 压缩json文件
def stripFile(oldFName,newFName):
    fp = open(oldFName,"r+")  
    newFp = open(newFName,"w")  
    for eachline in fp.readlines():  
        newStr = eachline.replace(" ","").replace("\t","").strip()  
        #print "Write:",newStr  
        newFp.write(newStr)  
    fp.close()  
    newFp.close()  
def run7(config):
	print('oceana run7')