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

def move_files(name,mspath,rtpath,mode):
    res_path = rtpath+'/res/child/'+name+'/'
    src_path = rtpath+'/src/child/'+name+'/'
    site_path =  os.path.expanduser('~')+'/Sites/desgame/res/child/'+name+'/'
    des_path = os.path.expanduser('~')+'/Sites/desgame/res/child'
    if os.path.isdir(des_path) == False:
        os.system('mkdir '+des_path)
    if os.path.isdir(site_path) == False:
        os.system('mkdir '+site_path)
    # remove
    os.system('cd '+site_path+'&& rm -rf ./*')
    os.system('cp '+mspath+'/'+mode+'_project.manifest '+res_path+'project.manifest')
    os.system('cp '+mspath+'/'+mode+'_version.manifest '+res_path+'version.manifest')
    # # rsync
    os.system('cd '+src_path+'&& rsync --exclude=child/ --exclude=.git ./ ~/Sites/desgame/res/child/'+name+' -rv')
    os.system('cd '+res_path+'&& rsync --exclude=.svn --exclude=child/ ./ ~/Sites/desgame/res/child/'+name+' -rv')

def generate_mainfest(name,mode,mspath,rtpath,version):
    manifest = json.load(open(os.path.join(mspath,mode+'_project.manifest'),'r'))
    manifest['assets'] = {}
    #src
    tmp_path = rtpath+'/src/child/'+name+'/'
    def add_asset_src(path):
        rela_path = os.path.relpath(path,tmp_path).replace('\\','/')
        # print rela_path
        manifest['assets'][rela_path]={'md5':md5.new(open(path,'rb').read()).hexdigest()}
    travel_file(tmp_path,add_asset_src)
    #res
    tmp_path = rtpath+'/res/child/'+name+'/'
    def add_asset_res(path):
        rela_path = os.path.relpath(path,tmp_path).replace('\\','/')
        if not rela_path.startswith('project.manifest') and not rela_path.startswith('version.manifest'):
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
    move_files(name,mspath,rtpath,mode)
    version[0] = versionfest['version']
import action_tool
################################ 长沙麻将
def runcsmjDev(config):
    version = ['0']
    generate_mainfest('csmj','dev',config['csmj_res'],config['dating_root'],version)
    curVersion = version[0]
    print '长沙麻将 dev version= '+curVersion
    os.system('cd ~/Sites/desgame/res/child && rsync -av csmj '+config['dev_tran']+'/des/res/child')

def runcsmjDebug(config):
    version = ['0']
    generate_mainfest('csmj','debug',config['csmj_res'],config['dating_root'],version)
    curVersion = version[0]
    print '--------------------------- 传输到 测试机器 -------------------------------------'
    os.system('cd ~/Sites/desgame/res/child && rsync -av csmj '+config['debug_tran']+'/des/res/child')
    print '------ 长沙麻将 debug version= '+curVersion
    action_tool.lookVersion('10',config['lookver_url'])
    action_tool.setVersion('10',curVersion,config['updtver_url'])

def runcsmjOnline(config):
    pass

################################ 红中麻将
def runhzmjDev(config):
    version = ['0']
    generate_mainfest('hzmj','dev',config['hzmj_res'],config['dating_root'],version)
    curVersion = version[0]
    print '红中麻将 dev version= '+curVersion
    os.system('cd ~/Sites/desgame/res/child && rsync -av hzmj '+config['dev_tran']+'/des/res/child')

def runhzmjDebug(config):
    version = ['0']
    generate_mainfest('hzmj','debug',config['hzmj_res'],config['dating_root'],version)
    curVersion = version[0]
    curVersion = version[0]
    print '--------------------------- 传输到 测试机器 -------------------------------------'
    os.system('cd ~/Sites/desgame/res/child && rsync -av hzmj '+config['debug_tran']+'/des/res/child')
    print '------ 红中麻将 debug version= '+curVersion
    action_tool.lookVersion('12',config['lookver_url'])
    action_tool.setVersion('12',curVersion,config['updtver_url'])

def runhzmjOnline(config):
    pass

################################ 跑得快
def runfstrDev(config):
    version = ['0']
    generate_mainfest('fastrun','dev',config['fastrun_res'],config['dating_root'],version)
    curVersion = version[0]
    print '--------------------------- 传输到 测试机器 -------------------------------------'
    os.system('cd ~/Sites/desgame/res/child && rsync -av fastrun '+config['debug_tran']+'/des/res/child')
    print '跑得快 dev version= '+curVersion
    os.system('cd ~/Sites/desgame/res/child && rsync -av fastrun '+config['dev_tran']+'/des/res/child')

def runfstrDebug(config):
    version = ['0']
    generate_mainfest('fastrun','debug',config['fastrun_res'],config['dating_root'],version)
    curVersion = version[0]
    print '--------------------------- 传输到 测试机器 -------------------------------------'
    os.system('cd ~/Sites/desgame/res/child && rsync -av fastrun '+config['debug_tran']+'/des/res/child')
    print '------ 跑得快 debug version= '+curVersion
    action_tool.lookVersion('13',config['lookver_url'])
    action_tool.setVersion('13',curVersion,config['updtver_url'])

def runfstrOnline(config):
    pass
# 汉寿跑胡子
def runhsphzDev(config):
    version = ['0']
    generate_mainfest('hsphz','dev',config['hsphz_res'],config['dating_root'],version)
    curVersion = version[0]
    print '汉寿跑胡子 dev version= '+curVersion

def runhsphzDebug(config):
    version = ['0']
    generate_mainfest('hsphz','debug',config['hsphz_res'],config['dating_root'],version)
    curVersion = version[0]
    print '--------------------------- 传输到 测试机器 -------------------------------------'
    os.system('cd ~/Sites/desgame/res/child && rsync -av hsphz '+config['debug_tran']+'/des/res/child')
    print '------ 汉寿跑胡子 debug version= '+curVersion
    action_tool.lookVersion('4',config['lookver_url'])
    action_tool.setVersion('4',curVersion,config['updtver_url'])

def runhsphzOnline(config):
    pass

# 梨树麻将
def runlsmjDebug(config):
    version = ['0']
    generate_mainfest('lsmj','debug',config['lsmj_res'],config['dating_root'],version)
    curVersion = version[0]
    print '--------------------------- 传输到 测试机器 -------------------------------------'
    os.system('cd ~/Sites/desgame/res/child && rsync -av lsmj '+config['debug_tran']+'/des/res/child')
    print '------ 梨树麻将 debug version= '+curVersion
    action_tool.lookVersion('14',config['lookver_url'])
    action_tool.setVersion('14',curVersion,config['updtver_url'])
# 常德跑胡子
def runcdphzDebug(config):
    version = ['0']
    generate_mainfest('cdphz','debug',config['cdphz_res'],config['dating_root'],version)
    curVersion = version[0]
    print '--------------------------- 传输到 测试机器 -------------------------------------'
    os.system('cd ~/Sites/desgame/res/child && rsync -av cdphz '+config['debug_tran']+'/des/res/child')
    print '------ 常德跑胡子 debug version= '+curVersion
    action_tool.lookVersion('2',config['lookver_url'])
    action_tool.setVersion('2',curVersion,config['updtver_url'])