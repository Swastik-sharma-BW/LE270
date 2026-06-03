
import time
import os
import json
import time
import shutil
import subprocess
import sys

has_ext_prepack = False
ext_prepack_address = 0
ext_prepack_size = 0
has_inr_prepack = False
inr_prepack_address = 0
inr_prepack_size = 0

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
                subprocess.run(['./../preset/lfsutil', '-i', fs_path, '-w', '-f', levels[idx-1], '-F', local_file_path])
            else:
                subprocess.run(['./../preset/lfsutil', '-i', fs_path, '-w', '-d', levels[idx-1]])

def get_ext_pkgfile_info():
    global has_ext_prepack, ext_prepack_address, ext_prepack_size
    ext_json_path = "./../preset/ext_prepack.json"
    if os.path.exists(ext_json_path):
        try:
            with open(ext_json_path, 'r') as f:
                data = json.load(f)
            ext_prepack_address = data.get('address', ["0"])[0]
            ext_prepack_size = data.get('size', ["0"])[0]
            subprocess.run(['./../preset/lfsutil', '-i', './ex_feedback_fs_data.bin', '-c', 'block_size=4096', '--read_size=256', '-S', ext_prepack_size])
            import_files_to_fs(ext_json_path, True)
            has_ext_prepack = True
        except Exception as e:
            print("ext_prepack.json no exist!")
            has_ext_prepack = False

def get_pkgfile_info():
    global has_inr_prepack, inr_prepack_address, inr_prepack_size
    partinfo_path = "./../preset/partinfo_config.json"
    inr_json_path = "./../preset/inr_prepack.json"
    if not os.path.exists(inr_json_path):
        return
    if os.path.exists(partinfo_path):
        try:
            with open(partinfo_path, 'r', encoding='utf-8') as f:
                data = json.load(f)
            macros = data.get('macros', {})
            inr_prepack_address = macros.get('FLASH_FS_REGION_START', "0")
            inr_prepack_size = macros.get('FLASH_FS_REGION_SIZE', "0")
            subprocess.run(['./../preset/lfsutil', '-i', './feedback_fs_data.bin', '-c', 'block_size=4096', '--read_size=256', '-S', inr_prepack_size])
            import_files_to_fs(inr_json_path, False)
            has_inr_prepack = True
        except Exception as e:
            print("inr_prepack.json no exist!")
            has_inr_prepack = False

def packet_ec718_app_binpkg(apbinpkgpath, appbinpkgpath):
    appaddr = None
    chipype = ""
    chipinfo = f"{os.path.dirname(os.path.abspath(__file__))}{os.sep}..{os.sep}chiptype.txt"
    secboot_file = os.path.join(os.path.dirname(os.path.abspath(__file__)), 'pub_key_sys.pem')
    head_size = 64
    if not os.path.exists(chipinfo):
        print(f"{chipinfo} does not exist")
        return
    
    if not os.path.exists(apbinpkgpath):
        print(f"{apbinpkgpath} does not exist")
        return 

    output_base = os.path.splitext(appbinpkgpath)[0]

    with open(chipinfo, "r") as fp:
        chipype = fp.read()
        if len(chipype) == 0:
            print(f"invalid chipype")
            return
                       
    packetinput = []
    if os.path.exists(appbinpkgpath):
        with open(appbinpkgpath, 'rb') as file:
            if os.path.exists(secboot_file):
                file.seek(160)
                head_size += 160

            data = file.read(4)
            appaddr = int.from_bytes(data, byteorder='little') - head_size
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

        if has_ext_prepack:
            memmapfilecontent += f"#define EF_IMG_LFS_LNA {ext_prepack_address}\n"
            memmapfilecontent += f"#define EF_IMG_LFS_SIZE {ext_prepack_size}\n"
            packetinput.append(f"-input ./ex_feedback_fs_data.bin -addrname EF_IMG_LFS_LNA -flashsize EF_IMG_LFS_SIZE")
        if has_inr_prepack:
            memmapfilecontent += f"#define PKGFLXLFS_LNA {inr_prepack_address}\n"
            memmapfilecontent += f"#define PKGFLXLFS_SIZE {inr_prepack_size}\n"
            packetinput.append(f"-input ./feedback_fs_data.bin -addrname PKGFLXLFS_LNA -flashsize PKGFLXLFS_SIZE")

        memmapfilecontent += f"#define PKGFLXAPP_APP0_LNA {'0x%x' % (appaddr)}\n"
        memmapfilecontent += f"#define PKGFLXAPP_APP0_LIMIT {'0x%x' % (appsize)}\n"
        packetinput.append(f"-input {appbinpkgpath} -addrname PKGFLXAPP_APP0_LNA -flashsize PKGFLXAPP_APP0_LIMIT")
        
        
        with open("memmap.h", "w") as fp:
            fp.write(memmapfilecontent)
        if sys.platform.upper().startswith("WIN"):
            wincmd = f"{os.path.dirname(os.path.abspath(__file__))}{os.sep}fcelf.exe -M {' '.join(packetinput)} -pkgmode 1 -banoldtool 1 -productname {chipype}_PRD_APP -def memmap.h -outfile {output_base}.binpkg"
        else:
            wincmd = f"{os.path.dirname(os.path.abspath(__file__))}{os.sep}fcelf -M {' '.join(packetinput)} -pkgmode 1 -banoldtool 1 -productname {chipype}_PRD_APP -def memmap.h -outfile {output_base}.binpkg"
        print("cmd %s" % (wincmd))
        subprocess.run(wincmd, shell=True)
    else:
        print("app not exist")

    os.remove("memmap.h")
    
if __name__ == "__main__":
    if len(sys.argv) > 2:
        if not sys.platform.upper().startswith("WIN"):
            get_ext_pkgfile_info()
            get_pkgfile_info()
        packet_ec718_app_binpkg(sys.argv[1], sys.argv[2])
    else:
        print(f"{sys.argv[0]} apfilepath appfilepath")
