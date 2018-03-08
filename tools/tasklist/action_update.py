#coding=utf-8
import os,sys,getopt,md5,json,time,base64,fileinput,shutil
def travel_file(root,cb):
    for lists in os.listdir(root):
        if not lists.startswith('.'):
            path = os.path.join(root,lists)
            if os.path.isdir(path):
                travel_file(path,cb)
            else:
                # print path
                cb(path)
def update_version(manifest,mspath,mode):
    v_list = str(manifest['version']).split('.')
    sec = int(v_list[1])
    fin = int(v_list[2])
    fin += 1
    # if fin > 9:
    #     sec += 1
    #     fin = 0
    v_list[1] = str(sec)
    if fin < 10:
        v_list[2] = '0'+str(fin)
    else:
        v_list[2] = str(fin)   
    manifest['version'] = '.'.join(v_list)
    deployfest = json.load(open(os.path.join(mspath,mode+'_deploy.manifest'),'r'))
    deployfest[manifest['version']] = time.strftime('%Y-%m-%d  %H:%M:%S',time.localtime(time.time()))  
    json.dump(deployfest,open(os.path.join(mspath,mode+'_deploy.manifest'),'w'),indent=0,sort_keys=True,separators=(',', ':'))
import action_tool
def move_files(mspath,rtpath,mode):
    res_path = rtpath+'/res/'
    src_path = rtpath+'/src/'
    site_path =  os.path.expanduser('~')+'/Sites/desgame/res/'
    des_path = os.path.expanduser('~')+'/Sites/desgame/'
    # remove
    os.system('cd '+site_path+'&& rm -rf ./*')
    # action_tool.stripFile(mspath+'/'+mode+'_project.manifest',res_path+'project.manifest')
    os.system('cp '+mspath+'/'+mode+'_project.manifest '+res_path+'project.manifest')
    os.system('cp '+mspath+'/'+mode+'_version.manifest '+res_path+'version.manifest')
    # rsync
    os.system('cd '+src_path+'&& rsync --exclude=child/ --exclude=.git ./ ~/Sites/desgame/res -rv')
    os.system('cd '+res_path+'&& rsync --exclude=.svn --exclude=child/ ./ ~/Sites/desgame/res -rv')
    # move
    os.system('cp '+mspath+'/'+mode+'_project.manifest '+des_path+'project.manifest')
    os.system('cp '+mspath+'/'+mode+'_version.manifest '+des_path+'version.manifest')

def generate_mainfest(mode,mspath,rtpath,version):
	manifest = json.load(open(os.path.join(mspath,mode+'_project.manifest'),'r'))
   	manifest['assets'] = {}
   	#src
   	tmp_path = rtpath+'/src/'
   	def add_asset_src(path):
		rela_path = os.path.relpath(path,tmp_path).replace('\\','/')
		if not rela_path.startswith('child'):
			# print rela_path
			manifest['assets'][rela_path]={'md5':md5.new(open(path,'rb').read()).hexdigest()}
   	travel_file(tmp_path,add_asset_src)
   	#res
   	tmp_path = rtpath+'/res/'
	def add_asset_res(path):
		rela_path = os.path.relpath(path,tmp_path).replace('\\','/')
		if not rela_path.startswith('project.manifest') and not rela_path.startswith('version.manifest') and not rela_path.startswith('child'):
			# print rela_path
			manifest['assets'][rela_path]={'md5':md5.new(open(path,'rb').read()).hexdigest()}
	travel_file(tmp_path,add_asset_res)
	#update version
	update_version(manifest,mspath,mode)
	#save
	json.dump(manifest,open(os.path.join(mspath,mode+'_project.manifest'),'w'),indent=0,sort_keys=True,separators=(',', ':'))
	#version manifest
	versionfest = json.load(open(os.path.join(mspath,mode+'_version.manifest'),'r'))
	versionfest['version'] = manifest['version']
	json.dump(versionfest,open(os.path.join(mspath,mode+'_version.manifest'),'w'),indent=0,sort_keys=True,separators=(',', ':'))
    #move
	move_files(mspath,rtpath,mode)
	version[0] = versionfest['version']


def runDatingDev(config):
    version = ['0']
    generate_mainfest('dev',config['dating_res'],config['dating_root'],version)
    curVersion = version[0]
    print '大厅 dev version= '+curVersion
    # move_files(config['dating_res'],config['dating_root'],'dev')
    # print(config['dating_res'])
    print '--------------------------- 传输到打包机200 -------------------------------------'
    os.system('cd ~/Sites/desgame && rsync -av project.manifest '+config['dev_tran']+'/des')
    os.system('cd ~/Sites/desgame && rsync -av version.manifest '+config['dev_tran']+'/des')
    os.system('cd ~/Sites/desgame/res && rsync --exclude=child/ ./ '+config['dev_tran']+'/des/res -av')

def runDatingDebug(config):
    version = ['0']
    generate_mainfest('debug',config['dating_res'],config['dating_root'],version)
    curVersion = version[0]
    print '--------------------------- 传输到 测试机器 -------------------------------------'
    os.system('cd ~/Sites/desgame && rsync -av project.manifest '+config['debug_tran']+'/des')
    os.system('cd ~/Sites/desgame && rsync -av version.manifest '+config['debug_tran']+'/des')
    os.system('cd ~/Sites/desgame/res && rsync --exclude=child/ ./ '+config['debug_tran']+'/des/res -av')
    print '------ 大厅 debug version= '+curVersion
    action_tool.lookVersion('0',config['lookver_url'])
    action_tool.setVersion('0',curVersion,config['updtver_url'])

def runDatingOnline(config):
    pass
    # action_tool.lookVersion('0',config['lookver_url'])
    # action_tool.setVersion('0','0.0.05',config['updtver_url'])
def submitsvn(config):
    print '提交大厅资源到 svn 服务器'
    os.system("cd "+config['dating_root']+"/res && svn st && svn ci -m 'dating up'")
