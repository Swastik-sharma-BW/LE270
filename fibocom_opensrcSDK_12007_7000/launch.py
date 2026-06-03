#!/usr/bin/env python

#-*-coding:UTF-8-*-
import os
import re
import sys
import glob
import shutil
import subprocess
import json
import opencpu as opencpu_tools
#python
#python --version
prjtag='project/ec7xx_0h00/ap/apps/fibo_soft/GCC/target.inc'
BuildBash='GccBuild.sh'
project='fibo_soft'
ChipType='EC718S'
VerType='OPENCPU'
BUILD_PRJ=''
delcmd=''
BLDSTDIR=''
PJDSTDIR=''
flash_ld_src_dir='ec718p_flash_ld'
flash_ld_org_dir='ec_original_718_flash_ld'
config_file = ''
TAGDIR='project/ec7xx_0h00/ap/apps'
#optional parameter
optional=''
flash_mem_mapdef_dir='device/target/board/ec7xx_0h00/common/pkginc/'
pycmd='python'

if sys.platform == "linux" :
    pycmd='python3'
    delcmd = 'rm -rf '
else :
    delcmd = 'rd /s/q '
print('now build system is %s'% sys.platform)

def  record_information(pro,chip):
    infofile='./gccout/info.txt'
    text = chip + ' ' + pro
    folder = os.path.exists('gccout')
    if not folder:
        os.makedirs('gccout')
    with open(infofile,'w+') as file:
        file.seek(0)
        file.truncate()
        file.flush()
        file.write(text)
        file.close()    
        
if len(sys.argv) == 2 :
    print('Get script argv = %s'% sys.argv[1])
#   clean build file
    if sys.argv[1] == "clean" :
        try:
            shutil.rmtree('gccout')
        except:
            pass
        exit()
    else :
        print('no need clean.')
else :
   print('') 

def fibo_get_chip_type():
    global ChipType
    global optional
    global inr_presets_fs
    global ext_presets_fs

    with open(prjtag, "r") as file:
        contents=file.read()
    if contents.find("CONFIG_FFW_AUDIO_AMR_ENABLE = y") != -1:
        print("Audio Amr : On")
        optional=optional+' '+' audio_amr'
        print("optional : %s"% optional)
    chipresult=re.search(r'PLATFORM\s*=\s*(\w+)', contents)
    if chipresult is not None:
        ChipType=chipresult.group(1)
        print("Chip Type: %s" % (ChipType))
        OUT_PATH = './gccout/ec7xx_0h00_%s/ap/fibo_soft/' % (ChipType.lower())
    else:
        print("Find Chip Type ERROR, please check %s"% target_file)
        file.close()
        exit()
    if re.search(r'CONFIG_FIBO_PRESETS_ENABLE\s*=\s*y', contents) is not None: #contents.find("CONFIG_FIBO_PRESETS_ENABLE = y") != -1:
        optional=optional+' '+'inr_presets_fs'
    if re.search(r'CONFIG_FIBO_EXT_PRESETS_ENABLE\s*=\s*y', contents) is not None: #contents.find("CONFIG_FIBO_EXT_PRESETS_ENABLE = y") != -1:
        optional=optional+' '+'ext_presets_fs'

    if contents.find("CONFIG_FFW_POWERON_BOOT_ENABLE = y") != -1:
        print("Poweron to boot mode: On")
        optional=optional+' '+'poweron_boot'
        print("optional : %s"% optional)

    if re.search(r'CONFIG_FFW_FEATURE_SECBOOT\s*=\s*y', contents) is not None:
        print("secboot")
        optional=optional+' '+'secboot'
        print("optional : %s"% optional)

def add_partition_app(targetname, target_file):
    targetpathbak = os.path.join(os.path.dirname(target_file), 'target.inc.bak')
    if os.path.exists(targetpathbak):
        os.remove(target_file)
        os.rename(targetpathbak, target_file)
        print(f"{targetpathbak}-->{target_file}")
    
    for line in open(target_file):
        if 'CONFIG_PARTINFO_JSON_PATH' in line:
            if '=' in line:
                partitionfile = line.split('=')[1].strip().strip('"')
                print(partitionfile)
                with open(partitionfile, 'r') as fp:
                    partitionjson = fp.read()
                    filejson = json.loads(partitionjson)
                    pat = filejson['macros']
                    
                    apaddr = int(pat["AP_FLASH_LOAD_ADDR"].strip(), 16)
                    apsize = int(pat["AP_FLASH_LOAD_SIZE"].strip(), 16)
                    
                    appsize = 64 * 1024
                    appramsize = 16 * 1024
                    appaddr = int(pat["APP_IMAGE_FLASH_LOAD_ADDR"].strip(), 16)
                    # appsize = int(pat["APP_IMAGE_FLASH_LOAD_SIZE"].strip(), 16)

                    pat['AP_FLASH_LOAD_SIZE'] = "0X%X" % (apsize - appsize)
                    pat['APP_IMAGE_FLASH_LOAD_ADDR'] = "0X%X" % (apaddr + apsize - appsize)
                    pat['APP_IMAGE_FLASH_LOAD_SIZE'] = "0X%X" % (appsize)
                    pat['APP_RAM_DATA_SIZE'] = "0X%X" % (appramsize)
                    

    targetpath = target_file
    targetpathbak = os.path.join(os.path.dirname(target_file), 'target.inc.bak')
    if not os.path.exists(targetpathbak):
        os.rename(targetpath, targetpathbak)
        
    x = json.dumps(filejson, indent=4)
    filename = "%s_%s_partition.json" % (targetname, filejson['chiptype'])
    if os.path.exists(filename):
        os.remove(filename)
    with open(filename, "w") as fp:
        fp.write(x)
    
    with open(targetpath, 'w') as fp:
        for line in open(targetpathbak):
            if 'CONFIG_PARTINFO_JSON_PATH' in line:
                fp.write(f"CONFIG_PARTINFO_JSON_PATH=\"{filename}\"\n")
            else:
                fp.write(line)

def fibo_select_build_prj(project_name=None, compile_app=False):
    global BLDSTDIR
    global PJDSTDIR
    global BUILD_PRJ
    cur_path = './target'
    file_list = os.listdir(cur_path)
    for idx, d in enumerate(file_list):
        print('%s, %s' % (idx + 1, d))
    if project_name:
        BUILD_PRJ = project_name
        print('SELECT: %s' % BUILD_PRJ)    
    else :
        selectprj = input('\r\nselect project number: ')
        select_idx = int(selectprj)

        if (select_idx > len(file_list)):
            print('ERROR')
        else:
            BUILD_PRJ=file_list[select_idx - 1]
            print('SELECT: %s' % BUILD_PRJ)

    BLDSTDIR = TAGDIR + '/bootloader/GCC/target.inc'
    SRCDIR = 'target/' + BUILD_PRJ+ '/target.inc'
    compile_app and add_partition_app(BUILD_PRJ, SRCDIR)
    #cmd=delcmd + BLDSTDIR
    #os.system(cmd)
    shutil.copy(SRCDIR, BLDSTDIR)

    PJDSTDIR = TAGDIR + '/fibo_soft/GCC/target.inc'
    #cmd=delcmd + PJDSTDIR
    #os.system(cmd)
    shutil.copy(SRCDIR, PJDSTDIR)

def fibo_del_targe():
    #print("unlink %s"% BLDSTDIR)
    #print("unlink %s"% PJDSTDIR)
    os.unlink(BLDSTDIR)
    os.unlink(PJDSTDIR)


def  fibo_build_project():
    BUILD_PATH=os.path.join(os.getcwd(),'gccout','ec7xx_0h00_%s'% ChipType.lower(),'ap')
    print('now build ROOTDIR is %s'% BUILD_PATH)
    cmd=delcmd + BUILD_PATH 
    os.system(cmd)

    output_devcfg(BUILD_PRJ, os.path.join(BUILD_PATH, 'fibo_soft'))

    if sys.platform == "linux" : 
        BuildBash='GccBuild.sh'
        cmd = './' + BuildBash + ' ' + project + ' ' + ChipType + ' ' + VerType + ' ' + optional
    else :
        BuildBash='GccBuild_ec7xx.bat'
        cmd = BuildBash + ' ' + project + ' ' + ChipType + ' ' + VerType + ' ' + optional
    print("@@@build script: %s"% cmd)
    record_information(BUILD_PRJ, ChipType)
    os.system(cmd)
    BUIPRJDIR=os.path.join(os.getcwd(),'gccout','ec7xx_0h00_%s'% ChipType.lower(),'%s'% BUILD_PRJ )
    if(os.path.exists(BUIPRJDIR)) :
        cmd= delcmd + BUIPRJDIR
        os.system(cmd)
    shutil.copytree(BUILD_PATH, BUIPRJDIR)

# 0 Recovery ld file; 1 set current ld file 
'''  
def  fibo_set_flash_ld(sel):
    if ChipType == "EC718S" :
        type_name = '718s'
    if ChipType == "EC718P" :
        type_name = '718p'
    print("chip type_name %s"% type_name)
    if sel == "1" :
        print("Set mem file for %s"% ChipType)
        src_file_path='device/target/board/ec7xx_0h00/flash_ld/ec'+ type_name +'_flash_ld/mem_map_'+type_name + '.h'
        dest_file_path='device/target/board/ec7xx_0h00/common/inc/mem_map_'+ type_name+ '.h'
        #os.unlink(dest_file_path)
        shutil.copy(src_file_path, dest_file_path)
        src_file_path='device/target/board/ec7xx_0h00/flash_ld/ec'+ type_name +'_flash_ld'+'/pkg_'+ type_name+'_mapdef.h'
        dest_file_path='device/target/board/ec7xx_0h00/common/pkginc/pkg_'+ type_name +'_mapdef.h'
        #os.unlink(dest_file_path)
        shutil.copy(src_file_path, dest_file_path)        
    else :
        print("Recovery mem file for %s"% ChipType)
        src_file_path='./device/target/board/ec7xx_0h00/flash_ld/ec_original_718_flash_ld/mem_map_'+ type_name +'.h'
        dest_file_path='./device/target/board/ec7xx_0h00/common/inc/mem_map_'+ type_name +'.h'
        #os.unlink(dest_file_path)
        shutil.copy(src_file_path, dest_file_path)
        src_file_path='./device/target/board/ec7xx_0h00/flash_ld/ec_original_718_flash_ld/pkg_'+ type_name +'_mapdef.h'
        dest_file_path='./device/target/board/ec7xx_0h00/common/pkginc/pkg_'+ type_name +'_mapdef.h'
        #os.unlink(dest_file_path)
        shutil.copy(src_file_path, dest_file_path)
'''
def  fibo_set_flash_ld(sel):
    contents=''
    with open(prjtag, "r") as file:
        contents = file.read()

    find_str = 'CONFIG_PARTINFO_JSON_PATH'
    tmp = ''
    global config_file
    if contents.find(find_str) :
        with open(prjtag, 'r') as f:
            line = f.readline()
            while line:               
                if -1 != line.find(find_str):
                    tmp = line;
                    start_index = tmp.find('"') + 1
                    end_index = tmp.find('"', start_index)
                    config_file = tmp[start_index:end_index]
                    break
                line = f.readline()
            print("find config file %s !"% config_file)
    else :
        print("not find config file") 
    if len(config_file) > 0 :
        cmd = pycmd + ' fibocom/fibo-framework/build/ec718/config/parase.py  ' + config_file
        os.system(cmd)

def  fibo_restore_lib():
    global BUILD_PRJ
    global ChipType
    src_file='target/' + BUILD_PRJ + '/lib/' + ChipType.lower()
    dest_file='prebuild/PLAT/lib/gcc/' + ChipType.lower()
    print("src_file %s !"% src_file)
    if os.path.exists(src_file) :
        file_list = os.listdir(src_file)
        for idx, d in enumerate(file_list):
            libdir = d
            print('now find lib dir %s' % libdir)
            src_lib_file = src_file + '/' + libdir;
            dest_lib_file = dest_file + '/' + libdir
            if os.path.exists(src_lib_file) :
                print('now find lib source %s' % src_lib_file)
                if os.path.exists(dest_lib_file) :
                    shutil.rmtree(dest_lib_file)
                shutil.copytree(src_lib_file, dest_lib_file)


def  fibo_set_build_script():
    src_file=''
    dest_file='tools/scripts/Makefile.rules'
    if sys.platform == "linux" :
        src_file = 'tools/scripts/linux/Makefile.rules'
    else :
        src_file = 'tools/scripts/win/Makefile.rules'
    shutil.copy(src_file, dest_file)

def import_files_to_fs(pre_file_pkg, import_ext=True):
    fs_path = './ex_feedback_fs_data.bin' if import_ext else './feedback_fs_data.bin'
    with open(pre_file_pkg, 'r', encoding='utf-8') as json_file:
        data = json.load(json_file)

    def extract_levels(file_path):
        parts = file_path.split('/')
        levels = []
        for i in range(1, len(parts)):
            levels.append('/' + '/'.join(parts[1:i + 1]))
        return levels

    for file_info in data["files"]:
        file_path = file_info["file"]
        local_file_path = file_info["local_file"]
        levels = extract_levels(file_path)
        num_elements = len(levels)
        for idx, level in enumerate(levels, start=1):
            if idx == num_elements:
                subprocess.run(['./tools/lfsutil', '-i', fs_path, '-w', '-f', levels[idx-1], '-F', local_file_path])
            else:
                subprocess.run(['./tools/lfsutil', '-i', fs_path, '-w', '-d', levels[idx-1]])

def get_pkgfile_info():  
    with open(prjtag, "r") as file:
        contents = file.read()
    match_path = re.search(r'CONFIG_PACKAGE_FILE_PATH\s*=\s*"([^"]+)"', contents)
    if match_path:
        pre_file_pkg = match_path.group(1)
        mapdef_file = 'pkg_' + ChipType[2:].lower() + '_mapdef.h'
        writefile = flash_mem_mapdef_dir + mapdef_file
        with open(writefile, 'r') as file:
            for line in file:
                match = re.search(r'#define\s+PKGFLXLFS_SIZE\s+\((0[Xx][0-9A-Fa-f]+)\)', line)
                if match:
                    lf_size = match.group(1)
        subprocess.run(['./tools/lfsutil', '-i', './feedback_fs_data.bin', '-c', 'block_size=4096', '--read_size=256', '-S', lf_size])
        import_files_to_fs(pre_file_pkg, False)
    else:
        return


def get_ext_pkgfile_info():
    mapdef_file = 'pkg_' + ChipType[2:].lower() + '_mapdef.h'
    writefile = flash_mem_mapdef_dir + mapdef_file

    with open(prjtag, "r") as file:
        contents = file.read()
    match_start = re.search(r'CONFIG_PACKAGE_EXT_FLASH_START\s*=\s*(\S+)', contents)
    match_size = re.search(r'CONFIG_PACKAGE_EXT_FLASH_SIZE\s*=\s*(\S+)', contents)
    match_path = re.search(r'CONFIG_PACKAGE_EXT_FILE_PATH\s*=\s*"([^"]+)"', contents)
    if match_path:
        pre_file_pkg = match_path.group(1)
    else:
        return

    if match_start:
        lf_start = match_start.group(1)
    else:
        return
    if match_size:
        lf_size = match_size.group(1)
        subprocess.run(['./tools/lfsutil', '-i', './ex_feedback_fs_data.bin', '-c', 'block_size=4096', '--read_size=256', '-S', lf_size])
        import_files_to_fs(pre_file_pkg, True)
    else:
        return

    file_mapdef = open(writefile, 'a')
    tmp = '#define EF_IMG_LFS_LNA'
    line = f"{tmp}  ({lf_start})\r\n"
    file_mapdef.write(line)
    tmp = '#define EF_IMG_LFS_SIZE'
    line = f"{tmp}  ({lf_size})\r\n"
    file_mapdef.write(line)
    file_mapdef.close()

#add secboot func by fibocom liuben
def secboot_func():
    with open(prjtag, "r") as file:
        contents = file.read()

    if re.search(r'CONFIG_FFW_FEATURE_SECBOOT\s*=\s*y', contents) is not None:
        print("secboot copy file")

        src_path = 'fibocom/fibo-framework/application/opencpu/secboot_app'
        app_ld_dst_path = 'fibocom/fibo-framework/application/opencpu'
        app_cmake_dst_path = 'fibocom/fibo-framework/application/opencpu/sdk/build'
        headset_dst_path = 'fibocom/fibo-framework/application/opencpu/sdk/tools'
        fibo_env_dst_path = 'fibocom/fibo-framework/application/opencpu/sdk'

        src_file = os.path.join(src_path, 'app.ld')
        app_ld_dst_file = os.path.join(app_ld_dst_path, 'app.ld')
        shutil.copyfile(src_file, app_ld_dst_file)

        src_file = os.path.join(src_path, 'CMakeLists.txt')
        app_cmake_dst_file = os.path.join(app_cmake_dst_path, 'CMakeLists.txt')
        shutil.copyfile(src_file, app_cmake_dst_file)

        src_file = os.path.join(src_path, 'headset')
        headset_tool_file = os.path.join(headset_dst_path, 'headset')
        shutil.copyfile(src_file, headset_tool_file)

        src_file = os.path.join(src_path, 'fibo_env')
        fibo_env_file = os.path.join(fibo_env_dst_path, 'fibo_env')
        shutil.copyfile(src_file, fibo_env_file)

        secboot_pukey_src_path = 'target/' + BUILD_PRJ + '/secboot_pubkey'
        secboot_pukey_dst_path = 'project/ec7xx_0h00/ap/apps/bootloader/code/main'
        if not os.path.exists(secboot_pukey_dst_path + '/VerifyKeyFile.c'):
            if os.path.exists(secboot_pukey_src_path):
                shutil.copy(secboot_pukey_src_path+'/VerifyKeyFile.c', secboot_pukey_dst_path)
                shutil.copy(secboot_pukey_src_path+'/pub_key_bl.pem', secboot_pukey_dst_path)
                shutil.copy(secboot_pukey_src_path+'/pub_key_sys.pem', secboot_pukey_dst_path)
        return

def opencpu_memmap_file_change():
    source_file = 'gccout/app.lds'
    destination_file = 'fibocom/fibo-framework/application/opencpu/sdk/fakeheader/mem_map.h'
    if not os.path.exists(source_file):
        return
    with open(source_file, 'r') as file:
        lines = file.readlines()
        line5 = lines[4]      
        ram_addr = re.search(r'0x[0-9a-fA-F]+', line5).group(0)
        if ram_addr == '0x0' :
            print(f"APP_RAM_ADDR_START is 0x0, noneed to change mem_map.h!!!")
            return;
            
    if not os.path.exists(destination_file):
        return
    with open(destination_file, 'r') as file:
        mem_map_content = file.read()
        mem_map_content = re.sub(r'#define\s+APP_RAM_ADDR_START\s+\(0x[0-9a-fA-F]+\)', f'#define    APP_RAM_ADDR_START           ({ram_addr})', mem_map_content)
        with open(destination_file, 'w') as file:
            file.write(mem_map_content)

    print(f"APP_RAM_ADDR_START has been updated to {ram_addr}")


def is_func_enable(project_name, func_macro):
    pattern = rf'^(?!#)\s*{func_macro}\s*=\s*y\s*'
    target_file = os.path.join('target', project_name, 'target.inc')
    with open(target_file, 'r') as file:
        for line in file:
            line = line.strip()
            if not line:
                continue
            match = re.search(pattern, line)
            if match:
                return True
    return False

def find_chip_type(project_name):
    pattern = rf'^(?!#)\s*PLATFORM\s*=\s*(\w+)\s*'
    target_file = os.path.join('target', project_name, 'target.inc')
    try:
        with open(target_file, 'r') as file:
            for line in file:
                line = line.strip()
                if not line:
                    continue
                match = re.search(pattern, line)
                if match:
                    return match.group(1).lower()
    except FileNotFoundError:
        return None

    return None

def output_devcfg(project_name = None, out_folder=None):
    def_xml_name = 'f_def_devcfg.xml'
    cus_xml_name = 'f_cust_devcfg.xml'

    import importlib
    gendevcfg = importlib.import_module('fibocom.fibo-config.fibo_devcfg.tools.gendevcfg')

    if project_name is None:
        project_name = BUILD_PRJ

    enable = is_func_enable(project_name, 'CONFIG_FIBO_DEVCFG')
    print(f"--- FIBO DEVCFG enable: {enable}")
    if not enable:
        return

    chip_type = find_chip_type(project_name)
    if chip_type is None:
        print(f"--- error: find not chip type")
        return

    src_relative_path = os.path.join('target', project_name)
    if out_folder is None:
        out_relative_path = os.path.join(os.getcwd(), 'gccout', 'ec7xx_0h00_%s'% chip_type, 
                                            '%s'% project_name, 'fibo_soft', 'devcfg')
    else:
        out_relative_path = os.path.join(os.getcwd(), out_folder, 'devcfg')
    def_xml = os.path.abspath(os.path.join(src_relative_path, def_xml_name))
    cus_xml = os.path.abspath(os.path.join(src_relative_path, cus_xml_name))
    out_dir = os.path.abspath(os.path.join(out_relative_path))
    gendevcfg.pkg_devcfg(def_xml, cus_xml, out_dir)

def main():
    compileapp = False
    if len(sys.argv) > 1:
        if (sys.argv[1].isdecimal()):
            compileapp = True
            project_name = None
        else:
            project_name = sys.argv[1]

            # output devcfg bin file only
            if len(sys.argv) == 3 and sys.argv[2] == '-devcfg':
                output_devcfg(project_name)
                exit()
    else:
        project_name = None
         
    fibo_select_build_prj(project_name, compileapp)
    fibo_get_chip_type()
    fibo_set_flash_ld('1')
    get_pkgfile_info()
    get_ext_pkgfile_info()
    secboot_func()
    #fibo_set_build_script()
    fibo_restore_lib()
    fibo_build_project()
    opencpu_memmap_file_change()
    compileapp and opencpu_tools.main(None, 'appcompile')
    #fibo_set_flash_ld('0')
    #fibo_del_targe()

main()
