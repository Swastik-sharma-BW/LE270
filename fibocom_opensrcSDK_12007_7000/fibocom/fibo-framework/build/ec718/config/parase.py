#!/usr/bin/env python

#-*-coding:UTF-8-*-
import os
import re
import sys
import glob
import shutil
import subprocess
import json


#python
#python --version

print('now build system is %s'% sys.platform)

arr = []
checkflag = -1
chiptype=''
flash_orignal_fd_dir='device/target/board/ec7xx_0h00/flash_ld/ec_original_718_flash_ld'
flash_mem_map_dir='device/target/board/ec7xx_0h00/common/inc/'
flash_mem_mapdef_dir='device/target/board/ec7xx_0h00/common/pkginc/'
middle_str=''
map_file=''
mapdef_file=''
multiapp=''
config_file=sys.argv[1]
print('system param is %s '% config_file)

def get_middle_str(temp):
    global middle_str
    if len(temp) > 0 :
        tmp_str = temp.lower()
        length = len(tmp_str)
        middle_str = tmp_str[2:length]
    

def init_arr():
    with open(config_file, 'r') as f:
        data = json.load(f)
        global chiptype
        global multiapp
        chiptype = data['chiptype']
        multiapp = data.get('multiapp', {})
        appram = data.get('appram',{})
        #print("Get app info %s -- %s"% (multiapp,appram))
        get_middle_str(chiptype)
        print("Get chip type from config file %s -- %s"% (chiptype,middle_str))
        #AP IMAGE
        arr.append([])
        arr[0].append('AP_FLASH_LOAD_ADDR')
        arr[0].append(data['macros']['AP_FLASH_LOAD_ADDR'])
        arr.append([])
        arr[1].append('AP_FLASH_LOAD_SIZE')
        arr[1].append(data['macros']['AP_FLASH_LOAD_SIZE'])
        arr.append([])
        arr[2].append('AP_FLASH_LOAD_UNZIP_SIZE')
        arr[2].append(data['macros']['AP_FLASH_LOAD_SIZE'])
        #FFS 
        arr.append([])
        arr[3].append('FLASH_FS_REGION_START')
        arr[3].append(data['macros']['FLASH_FS_REGION_START'])
        arr.append([])
        arr[4].append('FLASH_FS_REGION_END')
        arr[4].append(data['macros']['FLASH_FS_REGION_SIZE'])
        #HIB 
        arr.append([])
        arr[5].append('FLASH_MEM_BACKUP_NONXIP_ADDR')
        arr[5].append(data['macros']['FLASH_MEM_BACKUP_NONXIP_ADDR'])
        arr.append([])
        arr[6].append('FLASH_MEM_BACKUP_SIZE')
        arr[6].append(data['macros']['FLASH_MEM_BACKUP_SIZE'])
        #FOTA 
        arr.append([])
        arr[7].append('FLASH_FOTA_REGION_START')
        arr[7].append(data['macros']['FLASH_FOTA_REGION_START'])
        arr.append([])
        arr[8].append('FLASH_FOTA_REGION_LEN')
        arr[8].append(data['macros']['FLASH_FOTA_REGION_LEN'])
        #EXT BOOTLOADER
        arr.append([])
        arr[9].append('BOOTLOADEREXT_FLASH_LOAD_ADDR')
        arr[9].append(data['macros']['BOOTLOADEREXT_FLASH_LOAD_ADDR'])
        arr.append([])
        arr[10].append('BOOTLOADEREXT_FLASH_LOAD_SIZE')
        arr[10].append(data['macros']['BOOTLOADEREXT_FLASH_LOAD_SIZE'])
        #APP IMAGE 
        arr.append([])
        arr[11].append('APP_IMAGE_FLASH_LOAD_ADDR')
        arr[11].append(data['macros']['APP_IMAGE_FLASH_LOAD_ADDR'])
        arr.append([])
        arr[12].append('APP_IMAGE_FLASH_LOAD_SIZE')
        arr[12].append(data['macros']['APP_IMAGE_FLASH_LOAD_SIZE'])
        #RES TTS 
        arr.append([])
        arr[13].append('RES0_FILE_FLASH_LOAD_ADDR')
        arr[13].append(data['macros']['RES0_FILE_FLASH_LOAD_ADDR'])
        arr.append([])
        arr[14].append('RES0_FILE_FLASH_LOAD_SIZE')
        arr[14].append(data['macros']['RES0_FILE_FLASH_LOAD_SIZE'])
        #RES2 for customer 
        arr.append([])
        arr[15].append('RES2_FILE_FLASH_LOAD_ADDR')
        arr[15].append(data['macros']['RES2_FILE_FLASH_LOAD_ADDR'])
        arr.append([])
        arr[16].append('RES2_FILE_FLASH_LOAD_SIZE')
        arr[16].append(data['macros']['RES2_FILE_FLASH_LOAD_SIZE'])
        #FFS 
        arr.append([])
        arr[17].append('RESERVED_FLASH_FS_REGION_ADDR')
        arr[17].append(data['macros']['RESERVED_FLASH_FS_REGION_ADDR'])
        arr.append([])
        arr[18].append('RESERVED_FLASH_FS_REGION_SIZE')
        arr[18].append(data['macros']['RESERVED_FLASH_FS_REGION_SIZE'])

        arr.append([])
        arr[19].append('APP_RAM_DATA_SIZE')
        arr[19].append(data['macros'].get('APP_RAM_DATA_SIZE', '0x0'))

        # multiapp add 
        if multiapp != {} and appram != {}:
            arr.append([])
            arr[20].append('APP_IMAGE_FLASH_LOAD_ADDR')
            arr[20].append(data['multiapp'].get('APP1_IMAGE_FLASH_LOAD_ADDR', '0x0'))

            arr.append([])
            arr[21].append('APP_IMAGE_FLASH_LOAD_SIZE')
            arr[21].append(data['multiapp'].get('APP1_IMAGE_FLASH_LOAD_SIZE', '0x0'))

            arr.append([])
            arr[22].append('APP_RAM_DATA_SIZE')
            arr[22].append(data['appram'].get('APP1_RAM_DATA_SIZE', '0x0'))

            arr.append([])
            arr[23].append('APP2_IMAGE_FLASH_LOAD_ADDR')
            arr[23].append(data['multiapp'].get('APP2_IMAGE_FLASH_LOAD_ADDR', '0x0'))

            arr.append([])
            arr[24].append('APP2_IMAGE_FLASH_LOAD_SIZE')
            arr[24].append(data['multiapp'].get('APP2_IMAGE_FLASH_LOAD_SIZE', '0x0'))

            arr.append([])
            arr[25].append('APP2_RAM_DATA_SIZE')
            arr[25].append(data['appram'].get('APP2_RAM_DATA_SIZE', '0x0'))
            print("Get app info %s"% arr)

def total_sum():
    global checkflag
    sum = 0
    for i in range(2,19,2):
        #print("%s--%d"% (arr[i][0],sum))
        #print("arr[i][1] size %d"% (int(arr[i][1],base=16)/1024))
        sum +=int(arr[i][1],base=16)
    #EC718P ROM avaliable 3532KB
    #print("Bootloader2 size %d"% int(arr[10][1],base=16))
    if arr[10][1] == '0x0' :
        #add ap begin addr size
        sum +=((int(arr[0][1],base=16))-(int('0x800000',base=16)))
    else :
        sum +=(int(arr[9][1],base=16)-int('0x800000',base=16))

    #print("without rel and palt total size %d KB"% (sum/1024))
    if sum > (4096*1024):
        sum += (1024*(104+8))
    else :
        if sum > (2048*1024):
            sum += (1024*(52+8))
        else :
            sum += (1024*(52+4))
    
    print("ROM(FLASH) total size %d KB"% (sum/1024))
    if ((sum/1024)==2048) or ((sum/1024)==4096) or ((sum/1024)==8192):
        checkflag = 0
        print("total size check %d"% checkflag)
    else :
        checkflag = -1
        print("total size check %d"% checkflag)
        
def write_line_into_file(file,line):
    flag = 0
    strlen = 0
    for i in range(0,11):
        tmp = '#define ' + arr[i][0]
        if line.find(tmp) != -1 :
            #print("in %s -- find %s"% (line, tmp))
            strlen = 40-len(tmp)
            ''' i = 0 app addr , i =9 bt2 addr  need add base addr'''
            if int(arr[i][1],base=16) > int('0x800000',base=16) and i != 0 and i != 9:
                tmp = tmp + strlen*' '+'('+ hex(int(arr[i][1],base=16)- int('0x800000',base=16)) +')\r\n'
            else :
                tmp = tmp + strlen*' '+'('+ arr[i][1] +')\r\n'
            if i == 2 :
                tmp = '#define ' + arr[i][0]
                tmp = tmp + strlen*' '+'(' + hex(int(arr[i][1],base=16)+52*1024) +') //add less 56KB\r\n'
            if i == 4 :
                tmp = '#define ' + arr[i][0]
                tmp = tmp + strlen*' '+'(' + hex(int(arr[i-1][1],base=16) + int(arr[i][1],base=16) - int('0x800000',base=16)) +') //modify end addr\r\n'
            print(tmp)
            file.write(tmp)
            flag = 1
            break
    if flag == 0 :
        tmp = '#define FLASH_FOTA_REGION_END'
        if line.find(tmp) != -1 :
            strlen = 40-len(tmp)
            line = tmp + strlen*' '+'('+ hex(int(arr[7][1],base=16) + int(arr[8][1],base=16) - int('0x800000',base=16)) +')\r\n'
        
        tmp = '#define FLASH_HIB_BACKUP_EXIST'
        if line.find(tmp) != -1 :
            strlen = 40-len(tmp)
            if arr[6][1] != '0x0' :
                line = tmp + strlen*' '+'(1)\r\n'
            else :
                line = tmp + strlen*' '+'(0)\r\n'
        
        file.write(line)
    
        
    tmp = '#define AP_FLASH_XIP_ADDR'
    if line.find(tmp) != -1 :
        
        if arr[10][1] != '0x0' :
            tmp='\r\n\r\n'
            file.write(tmp)
            tmp = '#define CONFIG_FFW_TWO_BOOTLOADER 1 \r\n'
            file.write(tmp)
        '''
            tmp = '#define BOOTLOADEREXT_FLASH_LOAD_ADDR'
            strlen = 40-len(tmp)
            tmp = tmp + strlen*' '+'(' + hex(int(arr[9][1],base=16)) +')\r\n'
            file.write(tmp)
            tmp = '#define BOOTLOADEREXT_FLASH_LOAD_SIZE'
            strlen = 40-len(tmp)
            tmp = tmp + strlen*' '+'(' + hex(int(arr[10][1],base=16)) +')\r\n'
            file.write(tmp)
            tmp = '#define BOOTLOADEREXT_FLASH_LOAD_UNZIP_SIZE'
            strlen = 48-len(tmp)
            tmp = tmp + strlen*' '+'(' + hex(int(arr[10][1],base=16)) +')\r\n'
            file.write(tmp)
        '''
            
        if arr[14][1] != '0x0' :
            tmp='\r\n\r\n'
            file.write(tmp)        
            tmp = '#define RES0_FILE_FLASH_LOAD_ADDR'
            strlen = 40-len(tmp)
            tmp = tmp + strlen*' '+'(' + hex(int(arr[13][1],base=16)) +')\r\n'
            file.write(tmp)
            tmp = '#define RES0_FILE_FLASH_LOAD_SIZE'
            strlen = 40-len(tmp)
            tmp = tmp + strlen*' '+'(' + hex(int(arr[14][1],base=16)) +')\r\n'
            file.write(tmp)

        if arr[16][1] != '0x0' :
            tmp='\r\n\r\n'
            file.write(tmp)        
            tmp = '#define RES2_FILE_FLASH_LOAD_ADDR'
            strlen = 40-len(tmp)
            tmp = tmp + strlen*' '+'(' + hex(int(arr[15][1],base=16)) +')\r\n'
            file.write(tmp)
            tmp = '#define RES2_FILE_FLASH_LOAD_SIZE'
            strlen = 40-len(tmp)
            tmp = tmp + strlen*' '+'(' + hex(int(arr[16][1],base=16)) +')\r\n'
            file.write(tmp)
            
        if arr[18][1] != '0x0' :
            tmp='\r\n\r\n'
            file.write(tmp)        
            tmp = '#define RESERVED_FLASH_FS_REGION_ADDR'
            strlen = 40-len(tmp)
            tmp = tmp + strlen*' '+'(' + hex(int(arr[17][1],base=16)) +')\r\n'
            file.write(tmp)
            tmp = '#define RESERVED_FLASH_FS_REGION_SIZE'
            strlen = 40-len(tmp)
            tmp = tmp + strlen*' '+'(' + hex(int(arr[18][1],base=16)) +')\r\n'
            file.write(tmp)
            
        if arr[19][1] != '0x0' :
            tmp='\r\n\r\n'
            file.write(tmp)        
            tmp = '#define APP_RAM_DATA_SIZE'
            strlen = 40-len(tmp)
            tmp = tmp + strlen*' '+'(' + hex(int(arr[19][1],base=16)) +')\r\n'
            file.write(tmp)

        if arr[12][1] != '0x0' :
            tmp='\r\n\r\n'
            file.write(tmp)        
            tmp = '#define APP_IMAGE_FLASH_LOAD_ADDR'
            strlen = 40-len(tmp)
            tmp = tmp + strlen*' '+'(' + hex(int(arr[11][1],base=16)) +')\r\n'
            file.write(tmp)

            tmp='\r\n\r\n'
            file.write(tmp)        
            tmp = '#define APP_IMAGE_FLASH_LOAD_SIZE'
            strlen = 40-len(tmp)
            tmp = tmp + strlen*' '+'(' + hex(int(arr[12][1],base=16)) +')\r\n'
            file.write(tmp)

def write_mem_map_file():
    global map_file
    map_file = 'mem_map_' + middle_str + '.h'
    openfile = flash_orignal_fd_dir +'/'+map_file
    with open(openfile, 'r') as f:
        line = f.readline()
        writefile = flash_mem_map_dir + map_file
        file = open(writefile, 'w')
        while line:
            #file.write(line)
            write_line_into_file(file, line)
            line = f.readline()
        file.close()

    print("add opencpu mem_map.h file into fakeheader dir. APP size %s" %arr[12][1])
    if arr[12][1] != '0x0' :
        openfile = 'fibocom/fibo-framework/application/opencpu/sdk/fakeheader/mem_map.h'
        content = []
        file = open(openfile, 'w')
        content.append(f'#ifndef __APP_MEM_MAP_H__')
        content.append(f'#define __APP_MEM_MAP_H__\r\n\r\n#define    APP_IMAGE_HEAD               (64)\r\n#define    APP_RAM_ADDR_START           (0x0c400000)')
        if multiapp != {}:
            print("This is a Multi App OPENCPU Project.")  
            if ((int(arr[22][1],base=16) + int(arr[25][1],base=16)) != int(arr[19][1],base=16)) or ((int(arr[20][1],base=16)+(int(arr[21][1],base=16))+int(arr[24][1],base=16)) != int(arr[13][1],base=16)) :
                print("Multi App OPENCPU Project partinfo error.please check file %s" % config_file)
                exit()
            for i in range(20, 26):
                tmp = '#define    ' + arr[i][0]
                strlen = 40-len(tmp)
                tmp = tmp + strlen*' '+'(' + arr[i][1] +')'
                content.append(tmp)
        else :
            print("This is a Single App OPENCPU Project.")  
            for i in range(11, 13):
                tmp = '#define    ' + arr[i][0]
                strlen = 40-len(tmp)
                tmp = tmp + strlen*' '+'(' + arr[i][1] +')'
                content.append(tmp)
            tmp = '#define    ' + arr[19][0]
            strlen = 40-len(tmp)
            tmp = tmp + strlen*' '+'(' + arr[19][1] +')'
            content.append(tmp)           
        content.append(f'\r\n#endif\r\n')
        
        for line in content:
            tmp = line + '\r\n'
            file.write(tmp)
            #print("mem_map.h line: %s"% tmp)
        file.close()

            
def write_line_into_mapdef_file(file,line):
    tmp = '#define AP_PKGIMG_LNA'
    if line.find(tmp) != -1 :
        line = tmp + '  (' + arr[0][1] +')\r\n'

    tmp = '#define AP_PKGIMG_LIMIT_SIZE'
    if line.find(tmp) != -1 :
        line = tmp + '  (' + arr[1][1] +')\r\n'
        
    tmp = '#define PKGFLX_BL2_PKGIMG_LNA'
    if line.find(tmp) != -1 :
        line = tmp + '  (' + arr[9][1] + ')\r\n'
        
    tmp = '#define PKGFLX_BL2_PKGIMG_LIMIT_SIZE'
    if line.find(tmp) != -1 :
        line = tmp + '  (' + arr[10][1] + ')\r\n'
        
    file.write(line)

    tmp = '#define AP_FLASH_BASE_LNA'
    if line.find(tmp) != -1 :
        line = '\r\n\r\n'
        # BOOTLOADER2
        if arr[10][1] != '0x0' :
            tmp = '#define PKGFLX_BL2_PKGIMG_LNA'
            line = tmp + '  (' + arr[9][1] + ')\r\n'
            file.write(line)
            tmp = '#define PKGFLX_BL2_PKGIMG_LIMIT_SIZE'
            line = tmp + '  (' + arr[10][1] + ')\r\n'
            file.write(line) 

        # RES for TTS or OTHER
        if arr[14][1] != '0x0' :
            tmp = '#define PKGFLX_FIBO_TTS_RES_LNA'
            line = tmp + '  (' + arr[13][1] + ')\r\n'
            file.write(line)
            tmp = '#define PKGFLX_FIBO_TTS_RES_SIZE'
            line = tmp + '  (' + arr[14][1] + ')\r\n'
            file.write(line)
            
        # RES2 for CUSTOMER(not for file syetem zone)
        if arr[16][1] != '0x0' :
            tmp = '#define PKGFLX_FIBO_CUST_RES_LNA'
            line = tmp + '  (' + arr[15][1] + ')\r\n'
            file.write(line)
            tmp = '#define PKGFLX_FIBO_CUST_RES_SIZE'
            line = tmp + '  (' + arr[16][1] + ')\r\n'
            file.write(line) 
            
        # DEVCFG RESERVED_FLASH_FS_REGION
        if arr[18][1] != '0x0' :
            tmp = '#define PKGFLX_FIBO_RES_DEVCFG_DEF_LNA'
            line = tmp + '  (' + arr[17][1] + ')\r\n'
            file.write(line)
            tmp = '#define PKGFLX_FIBO_RES_DEVCFG_DEF_SIZE'
            line = tmp + '  (0x2000)\r\n'
            file.write(line) 

            tmp = '#define PKGFLX_FIBO_RES_DEVCFG_CUST_LNA'
            line = tmp + '  (' + hex(int(arr[17][1],base=16) + 0x2000) + ')\r\n'
            file.write(line)
            tmp = '#define PKGFLX_FIBO_RES_DEVCFG_CUST_SIZE'
            line = tmp + '  (0x2000)\r\n'
            file.write(line) 
            
        # APP IMAGE 
        if arr[12][1] != '0x0' :
            tmp = '#define PKGFLXAPP_APP0_LNA'
            line = tmp + '  (' + arr[11][1] + ')\r\n'
            file.write(line)
            tmp = '#define PKGFLXAPP_APP0_SIZE'
            line = tmp + '  (' + arr[12][1] + ')\r\n'
            file.write(line)
        
        
def write_mapdef_file():
    global mapdef_file 
    mapdef_file = 'pkg_' + middle_str + '_mapdef.h'
    openfile = flash_orignal_fd_dir +'/'+ mapdef_file
    with open(openfile, 'r') as f:
        line = f.readline()
        writefile = flash_mem_mapdef_dir + mapdef_file
        file = open(writefile, 'w')
        while line:
            write_line_into_mapdef_file(file, line)
            line = f.readline()
        file.close()
        
def write_innr_fs_mapdef():
    global mapdef_file
    mapdef_file = 'pkg_' + middle_str + '_mapdef.h'
    writefile = flash_mem_mapdef_dir + mapdef_file
    file = open(writefile, 'a')
    tmp = '#define PKGFLXLFS_LNA'
    line = tmp + '  (' + arr[3][1] +')\r\n'
    file.write(line)
    tmp = '#define PKGFLXLFS_SIZE'
    line = tmp + '  (' + arr[4][1] +')\r\n'
    file.write(line)
    file.close()

def main():
    init_arr()
    #print(arr)
    total_sum()
    #modify mem_map_ec71xx.h
    write_mem_map_file()
    #modify pkg_71xx_mapdef.h
    write_mapdef_file()
    write_innr_fs_mapdef()
    
#count=17
#print(count)
#print(hex(count))
#print(int(str(count),base=16))

main()
