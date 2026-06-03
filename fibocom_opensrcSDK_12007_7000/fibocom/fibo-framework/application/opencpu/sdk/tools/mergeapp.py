
import time
import os
import json
import time
import shutil
import pkg_resources
import subprocess
import sys


def packet_ec718_ap_app_pkg(apbinpkgpath, appbinpkgpath):
    fsimg = None
    appaddr = None

    if not os.path.exists(apbinpkgpath):
        print("Notice", f"文件({apbinpkgpath})不存在")
        return   
    
    if os.path.exists(appbinpkgpath):
        with open(appbinpkgpath, 'rb') as file:
            data = file.read(4)
            appaddr = int.from_bytes(data, byteorder='little') - 64
            appsize = os.path.getsize(appbinpkgpath)
            print("appaddr=%x appsize=%d" % (appaddr, appsize))
            
    wincmdunzip = f".{os.sep}assets{os.sep}fcelf{'' if os.sep == '/' else '.exe'} -E -input {apbinpkgpath} -info imagedata_test.info -dir pkg_extract_tmp_test"
    subprocess.run(wincmdunzip, shell=True)
    
    if os.path.exists("pkg_extract_tmp_test") and os.path.exists("imagedata_test.info"):
        pass
    else:
        print("Notice", "解压失败")
        return
    
    with open("imagedata_test.info", 'r') as fp:
        imginfo = json.loads(fp.read())
        memmapfilecontent = f"\
            #define EF_IMG_MERGE_LNA 0x80000000\n\
            #define EF_IMG_MERGE_LIMIT 0x400000\n\
            #define PKGFLXCALI_PKGIMG_MERGE_LNA 0x9f3000\n\
            #define PKGFLXCALI_PKGIMG_MERGE_LIMIT 0xc000\n"
        packetinput = []
        for item in imginfo['imageinfo']:
            if item['addr'] > 0 and item['flashsize'] > 0:
                memmapfilecontent += f"#define {item['type']}_IMG_MERGE_LNA {'0x%x' % (item['addr'])}\n"
                memmapfilecontent += f"#define {item['type']}_IMG_MERGE_LIMIT {'0x%x' % (item['flashsize'])}\n"
                packetinput.append(f"-input ./pkg_extract_tmp_test/{item['file']} -addrname {item['type']}_IMG_MERGE_LNA -flashsize {item['type']}_IMG_MERGE_LIMIT")

        if os.path.exists(appbinpkgpath):        
            memmapfilecontent += f"#define PKGFLXAPP_APP0_LNA {'0x%x' % (appaddr)}\n"
            memmapfilecontent += f"#define PKGFLXAPP_APP0_LIMIT {'0x%x' % (appsize)}\n"
            packetinput.append(f"-input {appbinpkgpath} -addrname PKGFLXAPP_APP0_LNA -flashsize PKGFLXAPP_APP0_LIMIT")
        
        
        with open("memmap.h", "w") as fp:
            fp.write(memmapfilecontent)

        wincmd = f".{os.sep}assets{os.sep}fcelf{'' if os.sep == '/' else '.exe'} -M {' '.join(packetinput)} -pkgmode {imginfo['pkgarg']['pkgmode']} -banoldtool 1 -productname {imginfo['pkgarg']['productname']} -def memmap.h -outfile fibo_soft_all.binpkg"
        print("wincmd %s" % (wincmd))
        subprocess.run(wincmd, shell=True)
    # shutil.rmtree("pkg_extract_tmp_test")
    # os.remove("imagedata_test.info")
    # os.remove("memmap.h")
    if fsimg is not None:
        os.remove(fsimg)
        
        
def packet_ec718_app_binpkg(appbinpkgpath):
    fsimg = None
    appaddr = None
    apbinpkgpath = 'win.py'
    if not os.path.exists(apbinpkgpath):
        print("Notice", f"文件({apbinpkgpath})不存在")
        return   
    packetinput = []
    if os.path.exists(appbinpkgpath):
        with open(appbinpkgpath, 'rb') as file:
            data = file.read(4)
            appaddr = int.from_bytes(data, byteorder='little') - 64
            appsize = os.path.getsize(appbinpkgpath)
            print("appaddr=%x appsize=%d" % (appaddr, appsize))
    
        memmapfilecontent = f"\
            #define EF_IMG_MERGE_LNA 0x80000000\n\
            #define EF_IMG_MERGE_LIMIT 0x400000\n\
            #define PKGFLXCALI_PKGIMG_MERGE_LNA 0x9f3000\n\
            #define PKGFLXCALI_PKGIMG_MERGE_LIMIT 0xc000\n \
            #define AP_IMG_MERGE_LNA 0x83e000\n\
            #define CP_IMG_MERGE_LNA 0x81a000\n\
            #define BL_IMG_MERGE_LNA 0x803000 \n\
            #define AP_IMG_MERGE_LIMIT {os.path.getsize(apbinpkgpath)}\n \
            #define CP_IMG_MERGE_LIMIT {os.path.getsize(apbinpkgpath)} \n\
            #define BL_IMG_MERGE_LIMIT {os.path.getsize(apbinpkgpath)}\n"
            
            
        packetinput.append(f"-input {apbinpkgpath} -addrname AP_IMG_MERGE_LNA -flashsize AP_IMG_MERGE_LIMIT")
        packetinput.append(f"-input {apbinpkgpath} -addrname CP_IMG_MERGE_LNA -flashsize CP_IMG_MERGE_LIMIT")
        packetinput.append(f"-input {apbinpkgpath} -addrname BL_IMG_MERGE_LNA -flashsize BL_IMG_MERGE_LIMIT")

        memmapfilecontent += f"#define PKGFLXAPP_APP0_LNA {'0x%x' % (appaddr)}\n"
        memmapfilecontent += f"#define PKGFLXAPP_APP0_LIMIT {'0x%x' % (appsize)}\n"
        packetinput.append(f"-input {appbinpkgpath} -addrname PKGFLXAPP_APP0_LNA -flashsize PKGFLXAPP_APP0_LIMIT")
        
        
        with open("memmap.h", "w") as fp:
            fp.write(memmapfilecontent)

        wincmd = f".{os.sep}assets{os.sep}fcelf{'' if os.sep == '/' else '.exe'} -M {' '.join(packetinput)} -pkgmode 1 -banoldtool 1 -productname FIBO_APP_PRODUCT -def memmap.h -outfile testapp.binpkg"
        print("wincmd %s" % (wincmd))
        subprocess.run(wincmd, shell=True)
    else:
        print("app not exist")

    # os.remove("memmap.h")
    if fsimg is not None:
        os.remove(fsimg)


def extract_resources():
    current_dir = os.getcwd()
    target_dir = os.path.join(current_dir, 'assets')
    if not os.path.exists(target_dir):
        os.makedirs(target_dir)
    
    for resource in pkg_resources.resource_listdir(__name__, 'assets'):
        source_path = pkg_resources.resource_filename(__name__, f'assets/{resource}')
        target_path = os.path.join(target_dir, resource)
        if source_path != target_path:
            shutil.copy2(source_path, target_path)
             
if __name__ == "__main__":
    # mk_fs_img("E:\\at-test-framework\\fibocom", 1024*1024)
    # exit()
    #extract_resources()
    if len(sys.argv) > 2:
        packet_ec718_ap_app_pkg(sys.argv[1], sys.argv[2])
    else:
        print(f"{sys.argv[0]} apfilepath appfilepath")
