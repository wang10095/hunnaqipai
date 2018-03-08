#coding=utf-8
import requests
import sqlite3
import os
from PIL import Image

# 字义缩放百分比
rate = 50

# 缩略图命名，在后面加上
resizedAddone = '_resized'

# 支持格式
suportFormat = ['bmp', 'gif', 'jpg', 'png']

def process(files):
    img_path = os.getcwd()+"/"+files
    out_path = os.getcwd()+"/"+"outPath"
    
    for lists in os.listdir(img_path):
        path = os.path.join(img_path,lists)
        if not os.path.basename(path).startswith('.'):
            img = Image.open(path)
            img.thumbnail((img.size[0] * rate / 100, img.size[1] * rate / 100)) #thumbnail 只能缩小不能放大
            img.save(out_path+"/"+os.path.basename(path)) #+'.png'


def lookFiles(files,flg):
    res_path = os.getcwd()+"/../res/"+files
    res_str = ""
    res_str2 = ""
    for lists in os.listdir(res_path):
        if not lists.startswith('.') and lists.startswith(flg):
            tmp = os.path.splitext(lists)[0]
            tmp = tmp.split(flg)[1]
            print(tmp)
            res_str = res_str + lists + ","
            res_str2 = res_str2 + "\'"+tmp+"\'" + ","
    print(res_str2)
    # print(res_path)
def fileRename(files,new):
    img_path = os.getcwd()+"/"+files
    # out_path = os.getcwd()+"/"+"outPath"
    print(img_path)
    num = 0
    nums = '%02d' % num 
    for lists in os.listdir(img_path):
        if not lists.startswith('.'):
            path = os.path.join(img_path,lists)
            # print(path)
            num = num + 1
            nums = '%02d' % num 
            print(lists)
            # print(lists,new+nums+'.png')
            os.rename(path,os.path.join(img_path,new+nums+'.png'))
def ocean():
    pass
# if __name__ == '__main__':
    # req1 = requests.get('http://httpbin.org/get')
    # print req1.url
    # print req1.text

    # payload = {'gameCode':'5','edtion':'0.2.3'}
    # req2 = requests.post("http://101.200.206.194:8090/platform/checkEdtion/",data=payload) # addEdtion game version
    # print req2.text
    # create table version(id integer primary key, edition text);
    # insert into version(id, edition) values(1, '0.0.0');
    # update version set edition  = '0.0.1'  where id=1
    # conn = sqlite3.connect("version.db")
    # cursor = conn.execute("SELECT edition from version where id =1 ")
    # for row in cursor:
    #     print row[0]

    # conn.execute("UPDATE version set edition = '1.7.490' where id=1")
    # conn.commit()

    # cursor = conn.execute("SELECT edition from version where id =1 ")
    # for row in cursor:
    #     print row[0]
    # conn.close()
    # select edition from VERSION
    #  测试图片缩放脚本
    # process('TPIC') # 压缩图片
    # lookFiles('match','match') # 查找文件路径下的 小文件名
    # fileRename('TPIC','tbomb')