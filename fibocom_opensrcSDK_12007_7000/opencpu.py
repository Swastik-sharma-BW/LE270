# -*- coding: UTF-8 -*-

import shutil
import sys
import os
from pathlib import Path
import re
import time
import tarfile
import xml.etree.ElementTree as ET

project_name=''
chipType=''
app_src=''
prjinfo=os.path.join('gccout', 'info.txt')

def get_project_name():
    global project_name
    global chipType
    global app_src
    if os.path.exists(prjinfo):
        with open(prjinfo, "r") as file:
            lines = file.readlines()
            print("project %s"% lines)
            info = lines[0]
            worlds = info.split()
            chipType = worlds[0].lower()
            project_name= worlds[1]
            print("project %s"% project_name)
            if len(lines) > 1:
                app_src = lines[1].strip()
                print("APP SRC: %s" % app_src)
            else:
                print("there is no other app src code.")
    else :
        print("you need build firmware First.")

def create_oc_export_file(sourcefile, apilist, api_memberlist, inclist):
    with open(sourcefile, "w") as f:
        # f.write('#include "fibo_opencpu.h"\n')
        for inc in inclist:
            f.write("%s\n" % (inc))

        f.write("\n")

        f.write("typedef struct\n{\n")
        for i in api_memberlist:
            f.write("    %s;\n" % (i))
        f.write("}FIBO_OPENCPU_T;\n\n")

        f.write("FIBO_OPENCPU_T g_app_service = \n{\n")
        for i in apilist:
            f.write("    %s,\n" % (i))
        f.write("};\n")

        f.write("void *fibo_get_app_service(void)\n{\n")
        f.write("    return &g_app_service;\n}\n")

def create_core_stub_file(corestubfile, apilist, api_memberlist, inclist, paramlist, typenamelist):
    fibo_printf = """
int fibo_printf(const char *fmt, ...)
{
    char buf[1024] = {0};
    va_list va;
    va_start(va, fmt);
    vsnprintf((char *)buf, sizeof(buf), fmt, va);
    va_end(va);
    g_fibo_api_service->m_fibo_printf_text(buf);
    return 0;
}
    """

    fibo_textTrace = """
int fibo_textTrace(char *fmt, ...)
{
    char buf[1024] = {0};
    va_list va;
    va_start(va, fmt);
    vsnprintf((char *)buf, sizeof(buf), fmt, va);
    va_end(va);
    g_fibo_api_service->m_fibo_printf_text(buf);
    return 0;
}
    """


    with open(corestubfile, "w") as f:
        # f.write('#include "fibo_opencpu.h"\n')
        for inc in inclist:
            f.write("%s\n" % (inc))

        f.write("#include \"fibo_opencpu_api.h\"\n")
        f.write(fibo_printf)
        f.write(fibo_textTrace)

        print("apilen=%d, paramlistlen=%d" % (len(apilist), len(paramlist)))
        for i, api in enumerate(apilist):
            if 'fibo_printf' in api or 'fibo_textTrace' in api:
                print("this is %s" % (api))
                continue 

            xx=paramlist[i].strip()

            cparam = []
            while True:
                f.write("\n//xx=%s\n" % (xx))
                normalarg=re.search(r'[,\(]\s*[\w\*]+?\s+([^\(\)]+?)\s*[,\)]', xx)
                fn=re.search(r'[,\(]\s*[\w\s\*]+\s*\(\s*\*\s*(\w+?)\)\s*\(.+?\)\s*?[,\)]', xx)
                if fn is not None and normalarg is not None:
                    if fn.start() <= normalarg.start():
                        cparam.append(fn[1])
                        xx=xx[fn.end() - 1:]
                        f.write("\n//1cc=%s\n" % (fn[1]))
                    else:
                        cparam.append(normalarg[1].strip('*'))
                        xx=xx[normalarg.end() - 1:]
                        f.write("\n//2cc=%s\n" % (normalarg[1]))
                elif fn is not None:
                    cparam.append(fn[1])
                    f.write("\n//3cc=%s\n" % (fn[1]))
                    xx=xx[fn.end() - 1:]
                elif normalarg is not None:
                    cparam.append(normalarg[1])
                    f.write("\n//4cc=%s\n" % (normalarg[1]))
                    xx=xx[normalarg.end() - 1:]
                else:
                    break

            pvar = []
            for p in cparam:
                is_callback = re.search(r'\w+\s*\(\s*\*\s*(.+?)\)\s*\(.+?\)', p)
                if is_callback is not None:
                    pvar.append(is_callback[1])
                    continue

                arrayp = re.search(r'\s+(\w+?)\[\s*.+\s*\]', p.strip())
                if arrayp is not None:
                    pvar.append(arrayp[1])
                    continue

                p = p.strip().split(' ')
                if len(p) > 0:
                    pp = p[len(p) - 1].strip(' ').strip('*')
                    if pp != 'void': 
                        pvar.append(pp)
                else:
                    pass
            f.write("\n//%s\n" % (paramlist[i]))
            f.write("\n//%s\n" % (cparam))
            f.write("\n//%s\n" % (pvar))
            f.write("\n//xx%s\n" % (xx))
            f.write("%s %s%s\n{ \n    return g_fibo_api_service->m_%s(%s);\n}\n" % (typenamelist[i], api, paramlist[i], api, ", ".join(pvar)))

def create_api_source_file(headerfile, apilist, api_memberlist, inclist):
    with open(headerfile, "w") as f:
        f.write('#ifndef FIBO_EXPORT_OPENCPU_H\n#define FIBO_EXPORT_OPENCPU_H\n')
        f.write("\n")

        for inc in inclist:
            f.write("%s\n" % (inc))

        f.write("typedef struct\n{\n")
        for i in api_memberlist:
            f.write("    %s;\n" % (i))
        f.write("}FIBO_OPENCPU_T;\n\n")

        f.write("extern FIBO_OPENCPU_T *g_fibo_api_service;\n\n")
        # for i in apilist:
        #     f.write("#define %s (g_fibo_api_service->m_%s)\n" % (i, i))

        f.write("\n#endif\n")

def copy_header_file(src, dst):
    print("copy_header_file %s %s" % (src, dst))

    if not os.path.exists(src):
        print("%s not exist" % (src))
        return "OK"

    if os.path.isfile(src):
        headbasefile = os.path.basename(src)
        dstfile = os.path.join(dst, headbasefile)
        if os.path.exists(dstfile):
            os.remove(dstfile)
        print('copy %s %s' % (src, dstfile))
        shutil.copyfile(src, dstfile)

    else: # dir
        headbasefile = os.path.basename(src)
        dstfile = os.path.join(dst, headbasefile)
        print('copy %s %s' % (src, dstfile))

        if os.path.exists(dstfile):
            print("delete %s" % (dstfile))
            shutil.rmtree(dstfile)
        shutil.copytree(src, dstfile)
        for root, _, files in os.walk(dstfile):
            for file in files:
                if os.path.splitext(file)[1] != '.h':
                    sourcefile = os.path.join(root, file)
                    os.remove(sourcefile)

    return "OK"

def compile_fibo_stub():
    dstname = os.path.join(os.getcwd(), 'gccout', 'core_sdk')
    outdir = os.path.join(dstname, 'out_coresdk')
    if sys.platform.upper().startswith('WIN'):
        print('this OS IS Windows')
        compilecmd = "rd /s /q %s & md %s & cd gccout/core_sdk & cd %s & cmake ../build_stub -G Ninja & ninja" % (outdir, outdir, outdir)
    else:
        print('this OS IS Linux')
        os.environ["PATH"] = "%s/gcc-arm-none-eabi/bin:%s/gcc-arm-none-eabi/arm-none-eabi/bin:%s/cmake/bin::%s" % (dstname, dstname, dstname, os.environ["PATH"])
        os.system("mkdir -p %s" % (outdir))
        os.system("chmod +x %s/gcc-arm-none-eabi/bin/*" % (dstname))
        os.system("chmod +x %s/tools/headset" % (dstname))
        os.system("chmod +x %s/tools/ptable" % (dstname))
        os.system("chmod +x %s/tools/fcelf" % (dstname))
        compilecmd = "cd %s;cmake ../build_stub;make" % (outdir)
    print("cmpilecmd %s" % (compilecmd))
    os.system(compilecmd)
    shutil.rmtree(os.path.join(dstname, "build_stub"))
    corestubfile = os.path.join(os.getcwd(), 'gccout', 'corestubfile.c');
    os.remove(corestubfile)

    for root, _, files in os.walk(outdir):
        for file in files:
            if file == 'corestubfile.c.obj':
                sourcefile = os.path.join(root, file)
                dstfile = os.path.join(dstname, 'lib', 'corestubfile.o')
                print('copy %s %s' % (sourcefile, dstfile))
                shutil.copyfile(sourcefile, dstfile)
                shutil.rmtree(outdir)
                return True
    return False

def compile_fibo_app():
    dstname = os.path.join(os.getcwd(), 'gccout', 'core_sdk')
    appdir = os.path.join(dstname, 'out_app')
    if sys.platform.upper().startswith('WIN'):
        print('this OS IS Windows')
        compilecmd = "rd /s /q %s & md %s & cd gccout/core_sdk & cd %s & cmake ../build -DAPP_DEMO_ENABLE=ON -G Ninja & ninja" % (appdir, appdir, appdir)
    else:
        print('this OS IS Linux')
        os.environ["PATH"] = "%s/gcc-arm-none-eabi/bin:%s/gcc-arm-none-eabi/arm-none-eabi/bin:%s/cmake/bin::%s" % (dstname, dstname, dstname, os.environ["PATH"])
        os.system("mkdir -p %s" % (appdir))
        os.system("chmod +x %s/gcc-arm-none-eabi/bin/*" % (dstname))
        os.system("chmod +x %s/tools/headset" % (dstname))
        os.system("chmod +x %s/tools/ptable" % (dstname))
        os.system("chmod +x %s/tools/fcelf" % (dstname))
        os.system("chmod +x %s/tools/ecsecure" % (dstname))
        compilecmd = "cd %s;cmake ../build -DAPP_DEMO_ENABLE=ON;make" % (appdir)
    print("cmpilecmd %s" % (compilecmd))
    os.system(compilecmd)
    
    for root, _, files in os.walk(appdir):
        for file in files:
            if file == 'app.bin':
                appbinpath = os.path.join(root, file)
                with open(appbinpath, 'rb') as fp:
                    data = fp.read(4)
                    appaddr = int.from_bytes(data, byteorder='little') - 64
                    appsize = os.path.getsize(appbinpath)
                    print("appaddr=%x appsize=%d" % (appaddr, appsize))
                dstappbinpath = os.path.join(root, "%x_%s" % (appaddr, file))
                os.rename(appbinpath, dstappbinpath)
                
def compile_fibo_testcase():
    dstname = os.path.join(os.getcwd(), 'gccout', 'core_sdk')
    appdir = os.path.join(dstname, 'out_testcase')
    if sys.platform.upper().startswith('WIN'):
        print('this OS IS Windows')
        compilecmd = "rd /s /q %s & md %s & cd gccout/core_sdk & cd %s & cmake ../build -DTESTCASE_ENABLE=ON -G Ninja & ninja" % (appdir, appdir, appdir)
    else:
        print('this OS IS Linux')
        os.environ["PATH"] = "%s/gcc-arm-none-eabi/bin:%s/gcc-arm-none-eabi/arm-none-eabi/bin:%s/cmake/bin::%s" % (dstname, dstname, dstname, os.environ["PATH"])
        os.system("mkdir -p %s" % (appdir))
        os.system("chmod +x %s/gcc-arm-none-eabi/bin/*" % (dstname))
        os.system("chmod +x %s/tools/headset" % (dstname))
        os.system("chmod +x %s/tools/ptable" % (dstname))
        os.system("chmod +x %s/tools/fcelf" % (dstname))
        compilecmd = "cd %s;cmake ../build -DTESTCASE_ENABLE=ON;make" % (appdir)
    print("cmpilecmd %s" % (compilecmd))
    os.system(compilecmd)
    
    for root, _, files in os.walk(appdir):
        for file in files:
            if file == 'app.bin':
                appbinpath = os.path.join(root, file)
                with open(appbinpath, 'rb') as fp:
                    data = fp.read(4)
                    appaddr = int.from_bytes(data, byteorder='little') - 64
                    appsize = os.path.getsize(appbinpath)
                    print("appaddr=%x appsize=%d" % (appaddr, appsize))
                dstappbinpath = os.path.join(root, "%x_%s" % (appaddr, file))
                os.rename(appbinpath, dstappbinpath)

def add_cmakelist_macros(cmake_file_path, macros):
    """
    将宏定义写入到指定的 CMakeLists.txt 文件中。

    :param cmake_file_path: CMakeLists.txt 文件的路径
    :param macros: 要写入的宏定义列表
    """
    print("add  macros into file  %s" % cmake_file_path)
    with open(cmake_file_path, 'r') as cmake_file:
        cmake_lines = cmake_file.readlines()

    # Find the line with add_definitions
    add_definitions_line = -1
    for i, line in enumerate(cmake_lines):
        if 'add_definitions' in line:
            add_definitions_line = i
            break

    if add_definitions_line != -1:
        # Insert the macros after the add_definitions line
        for macro in macros:
            cmake_lines.insert(add_definitions_line + 1, f'add_definitions(-D{macro})\n')

    with open(cmake_file_path, 'w') as cmake_file:
        cmake_file.writelines(cmake_lines)

def add_cmakelist_macros_from_target_inc():
    """
    从 target.inc 文件中读取宏定义，并将它们写入到 CMakeLists.txt 文件中。
    """
    file_path = os.path.join(os.getcwd(), "target", project_name, "target.inc")
    print("find  macros in file  %s" % file_path)
    with open(file_path, 'r') as file:
        lines = file.readlines()
        macros = []
        for line in lines:
            line = line.strip()
            if re.match(r'^CONFIG_CUST.*\s*=\s*y$', line):
                parts = line.split('=')[0].strip()
                macros.append(parts)
                print("find  macros: -%s-" % parts)

    # Find the CMakeLists.txt file and write the macros
    cmake_file_path = os.path.join(os.getcwd(), "gccout", "core_sdk", "build_stub", "CMakeLists.txt")
    add_cmakelist_macros(cmake_file_path, macros)
    cmake_file_path = os.path.join(os.getcwd(), "gccout", "core_sdk", "build", "CMakeLists.txt")
    add_cmakelist_macros(cmake_file_path, macros)

def parse_and_copy_files():
    xml_file = os.path.join(os.getcwd(), 'target', project_name, 'core_filelist.xml')
    if not os.path.exists(xml_file):
        print(f"Source file {xml_file} does not exist")
        return

    tree = ET.parse(xml_file)
    root = tree.getroot()
    for file_elem in root.findall('file'):
        src = file_elem.get('src')
        dstdir = file_elem.get('dstdir')
        dst = file_elem.get('dst')
        if src and dstdir:
            dst_path = os.path.join(dstdir, os.path.basename(src))
        elif src and dst:
            dst_path = dst
        else:
            print("Invalid file element: missing src or dstdir/dst")
            continue

        src_file =  os.path.join(os.getcwd(), src)
        if not os.path.exists(src):
            print(f"Source file {src} does not exist")
            continue

        dst_dir = os.path.join(os.getcwd(), dstdir)
        if not os.path.exists(dst_dir):
            os.makedirs(dst_dir)

        shutil.copy(src_file, dst_dir)
        print(f"Copied {src} to {dst_dir}")

def create_api_list(fname, create_api_header = False, core_stub = False):
    apilist = []
    api_memberlist = []
    api_paramlist = []
    inclist = []
    typenamelist = []

    print('filename: '  + fname);
    with open(fname, encoding='utf-8') as f:
        for ln in f.readlines():
            ln = ln.strip();
            if len(ln) > 0:
                if re.search(r'^#include', ln) is not None:
                    inclist.append(ln)
                    continue

                if ln[0] == '#':
                    continue

                result0 = re.search(r'(struct\s+\w+\s*\*)\s*(\w+)\s*(\(.*\))\s*;$', ln)
                result = re.search(r'(\w+)\s+(\w+)\s*(\(.*\))\s*;$', ln)
                result1 = re.search(r'(\w+\s*\*)\s*(\w+)\s*(\(.*\))\s*;$', ln)
                result2 = re.search(r'(const\s+\w+\s*\*)\s*(\w+)\s*(\(.*\))\s*;$', ln)

                if result0 is not None:
                    typename = result0[1]
                    funname = result0[2]
                    paramlist= result0[3]
                    function_pointer = "%s (*m_%s)%s" % (typename, funname, paramlist)
                    apilist.append(funname)
                    api_memberlist.append(function_pointer)
                    api_paramlist.append(paramlist)
                    typenamelist.append(typename)
                elif result2 is not None:
                    typename = result2[1]
                    funname = result2[2]
                    paramlist= result2[3]
                    function_pointer = "%s (*m_%s)%s" % (typename, funname, paramlist)
                    apilist.append(funname)
                    api_memberlist.append(function_pointer)
                    api_paramlist.append(paramlist)
                    typenamelist.append(typename)
                    print("const type %s" % (typename))
                elif result is not None:
                    typename = result[1]
                    funname = result[2]
                    paramlist= result[3]
                    function_pointer = "%s (*m_%s)%s" % (typename, funname, paramlist)
                    apilist.append(funname)
                    api_memberlist.append(function_pointer)
                    api_paramlist.append(paramlist)
                    typenamelist.append(typename)                    
                elif result1 is not None:
                    typename = result1[1]
                    funname = result1[2]
                    paramlist= result1[3]
                    print("pointer %s - %s - %s" % (typename, funname, paramlist))
                    function_pointer = "%s (*m_%s)%s" % (typename, funname, paramlist)
                    apilist.append(funname)
                    api_memberlist.append(function_pointer)
                    api_paramlist.append(paramlist)
                    typenamelist.append(typename)                    
                else:
                    print("invalid ln=%s" % (ln))

    headerfile =  os.path.join(os.getcwd(), 'gccout', 'fibo_opencpu_api.h')
    if os.path.exists(headerfile):
        os.remove(headerfile)

    if create_api_header:
        # generate opencpu api header
        create_api_source_file(headerfile, apilist, api_memberlist, inclist)
        if core_stub:
            corestubfile = os.path.join(os.getcwd(), 'gccout', 'corestubfile.c');
            create_core_stub_file(corestubfile, apilist, api_memberlist, inclist, api_paramlist, typenamelist)
        return

    sourcefile = os.path.join(os.getcwd(), 'gccout', 'oc_export_api.c'); #"out%soc_export_api.c" % (os.path.sep)
    create_oc_export_file(sourcefile, apilist, api_memberlist, inclist)
def modify_header_file(project_name, dstname):
    # 检查工程名是否包含"EZVIZ"
    if "OPEN-EZVIZ" not in project_name:
        print(f"{project_name} no need modify_header_file")
        return False

    print(f"{project_name} need modify_header_file")
    fibo_opencomm_src = os.path.join(project_name, 'newlib', 'fibo_opencpu_comm.h')
    fibo_opencomm_dst = os.path.join(dstname, 'sdk', 'include', 'fibo_opencpu_comm.h')
    if os.path.exists(fibo_opencomm_dst):
        os.remove(fibo_opencomm_dst)
    shutil.copyfile(fibo_opencomm_src, fibo_opencomm_dst)

    vs_src = os.path.join(project_name, 'newlib', 'vsprintf.c')
    vs_dst = os.path.join(dstname, 'newlib', 'vsprintf.c')
    if os.path.exists(vs_dst):
        os.remove(vs_dst)
    shutil.copyfile(vs_src, vs_dst)

def compile_fibo_libc():
    dstname = os.path.join(os.getcwd(), 'gccout', 'core_sdk')
    os.environ["PATH"] = "%s/gcc-arm-none-eabi/bin:%s/gcc-arm-none-eabi/arm-none-eabi/bin:%s/cmake/bin::%s" % (dstname, dstname, dstname, os.environ["PATH"])

    os.system("mkdir -p %s/out" % (dstname))
    os.system("chmod +x %s/gcc-arm-none-eabi/bin/*" % (dstname))
    os.system("chmod +x %s/tools/headset" % (dstname))
    os.system("chmod +x %s/tools/ptable" % (dstname))

    compilecmd = "cd %s/out;cmake ../build;make" % (dstname)
    print("cmpilecmd %s" % (compilecmd))
    os.system(compilecmd)

    for root, _, files in os.walk(dstname + "/out"):
        for file in files:
            if file == 'vsprintf.c.obj':
                sourcefile = os.path.join(root, file)
                dstfile = "%s/lib/vsprintf.o" % (dstname)
                print('copy %s %s' % (sourcefile, dstfile))
                shutil.copyfile(sourcefile, dstfile)
                print('rm  %s' % (dstname + "/newlib/vsprintf.c"))
                os.remove(dstname + "/newlib/vsprintf.c")
                print('rm  %s' % (dstname + "/out"))
                shutil.rmtree(dstname + "/out")
                return True
    return False

def get_partinfo_json_path():
    target_inc_path = f"./target/{project_name}/target.inc"
    if not os.path.exists(target_inc_path):
        return None

    with open(target_inc_path, 'r', encoding='utf-8') as f:
        content = f.read()

    match = re.search(r'CONFIG_PARTINFO_JSON_PATH="([^"]+)"', content)
    return match.group(1) if match else None


def create_opencpu_sdk(fname):
    sdksrcpath = os.path.join(os.getcwd(), 'fibocom', 'fibo-framework', 'application', 'opencpu', 'sdk')
    dstname = os.path.join(os.getcwd(), 'gccout', 'core_sdk')
    # if os.path.exists(dstname):
    #     shutil.rmtree(dstname)
    Path(dstname).mkdir(parents=True, exist_ok=True)
    print('filename: '  + fname)

    #copy all header file
    headerpath = os.path.join(dstname,'sdk', 'include')
    Path(headerpath).mkdir(parents=True, exist_ok=True)
    with open(fname, encoding='utf-8') as f:
        for ln in f.readlines():
            ln = ln.strip();
            if len(ln) > 0:
                headermatch = re.search(r'^#INCPATH\s+(.+)', ln)
                if headermatch is not None:
                    dirxx = headermatch[1]
                    reldirxx = os.path.join(os.getcwd(), dirxx)
                    copy_header_file(reldirxx, headerpath)

    for item in os.listdir(os.path.join(sdksrcpath,'fakeheader')):
        srcfile = os.path.join(sdksrcpath, 'fakeheader', item)
        dstfile = os.path.join(headerpath, item)
        print('copy %s %s' % (srcfile, dstfile))
        shutil.copyfile(srcfile, dstfile)

    #copy compile tool chain
    toolchainsrc = os.path.join(os.getcwd(), 'tools', 'gcc-arm-none-eabi')
    toolchainpath = os.path.join(dstname, 'gcc-arm-none-eabi')
    if os.path.exists(toolchainpath):
        shutil.rmtree(toolchainpath)
    shutil.copytree(toolchainsrc, toolchainpath)

    #copy compile tool chain
    ldssrc = os.path.join(os.getcwd(), 'gccout', 'app.lds')
    ldsdst = os.path.join(dstname, 'app.lds')
    if os.path.exists(ldsdst):
        os.remove(ldsdst)
    shutil.copyfile(ldssrc, ldsdst)

    ldssrc = os.path.join(os.getcwd(), 'fibocom', 'fibo-framework', 'application', 'opencpu', 'app.ld')
    ldsdst = os.path.join(dstname, 'app.ld')
    if os.path.exists(ldsdst):
        os.remove(ldsdst)
    shutil.copyfile(ldssrc, ldsdst)

    ldssrc = os.path.join(os.getcwd(), 'fibocom', 'fibo-framework', 'application', 'opencpu', 'app2.ld')
    ldsdst = os.path.join(dstname, 'app2.ld')
    if os.path.exists(ldsdst):
        os.remove(ldsdst)
    shutil.copyfile(ldssrc, ldsdst)

    openolddemosrc = os.path.join(os.getcwd(), 'cust_app', 'fibo_opencpu_demo')
    openolddemodst = os.path.join(dstname, 'fibo_opencpu_demo')
    if os.path.exists(openolddemodst):
        shutil.rmtree(openolddemodst)
    shutil.copytree(openolddemosrc, openolddemodst)

    openolddemosrc = os.path.join(os.getcwd(), 'cust_app', 'app_demo')
    openolddemodst = os.path.join(dstname, 'app_demo')
    if os.path.exists(openolddemodst):
        shutil.rmtree(openolddemodst)
    shutil.copytree(openolddemosrc, openolddemodst)

    for item in os.listdir(sdksrcpath):
        cmakesrcpath = os.path.join(sdksrcpath, item)
        cmakedstpath = os.path.join(dstname, item)
        if os.path.isfile(cmakesrcpath):
            if os.path.exists(cmakedstpath):
                os.remove(cmakedstpath)
            shutil.copyfile(cmakesrcpath, cmakedstpath)
        else:
            if os.path.exists(cmakedstpath):
                shutil.rmtree(cmakedstpath)
            shutil.copytree(cmakesrcpath, cmakedstpath)

    try:
        entrypath = os.path.join(dstname, 'src', 'entry.c')
        os.remove(entrypath)
        print("rm %s" % (entrypath))

        toolssrc = os.path.join(dstname, 'tools', 'crc32.c')
        os.remove(toolssrc)
        print("rm %s" % (toolssrc))

        toolssrc = os.path.join(dstname, 'tools', 'headset.c')
        os.remove(toolssrc)
        print("rm %s" % (toolssrc))

        toolssrc = os.path.join(dstname, 'tools', 'ptable.c')
        os.remove(toolssrc)
        print("rm %s" % (toolssrc))
    except:
        pass

    srcfile = os.path.join(os.getcwd(), 'tools', 'fcelf')
    dstfile = os.path.join(dstname, 'tools', 'fcelf')
    try:
        os.remove(dstfile)
    except:
        pass
    print('copy %s %s' % (srcfile, dstfile))
    shutil.copyfile(srcfile, dstfile)
    
    #copy lfsutil
    src_file = os.path.join(os.getcwd(), 'tools', 'lfsutil')
    dst_file = os.path.join(dstname, 'preset', 'lfsutil')
    tools_dir = os.path.join(dstname, 'preset')
    if not os.path.exists(tools_dir):
        os.makedirs(tools_dir)
    shutil.copy2(src_file, dst_file)

    json_path = get_partinfo_json_path()
    if json_path is not None:
        print(f"partinfo_json_path: {json_path}")
        json_dst_file = os.path.join(dstname, 'preset', 'partinfo_config.json')
        shutil.copyfile(json_path, json_dst_file)

    srcfile = os.path.join(os.getcwd(), 'tools', 'fcelf.exe')
    dstfile = os.path.join(dstname, 'tools', 'fcelf.exe')
    print('copy %s %s' % (srcfile, dstfile))
    try:
        os.remove(dstfile)
    except:
        pass
    shutil.copyfile(srcfile, dstfile)

    #copy secboot system public key by fibocom liuben
    ldssrc = os.path.join(os.getcwd(), 'project', 'ec7xx_0h00', 'ap', 'apps', 'bootloader', 'code', 'main', 'pub_key_sys.pem')
    if os.path.exists(ldssrc):
        ldsdst = os.path.join(dstname, 'tools', 'pub_key_sys.pem')
        if os.path.exists(ldsdst):
            os.remove(ldsdst)
        shutil.copyfile(ldssrc, ldsdst)

        #copy secboot tool chain by fibocom liuben
        ldssrc = os.path.join(os.getcwd(), 'tools', 'ecsecure')
        ldsdst = os.path.join(dstname, 'tools', 'ecsecure')
        if os.path.exists(ldsdst):
            os.remove(ldsdst)
        shutil.copyfile(ldssrc, ldsdst)

    srcchipinfo = os.path.join(os.getcwd(), 'gccout', 'chiptype.txt')
    dstchipinfo = os.path.join(os.getcwd(), 'gccout', 'core_sdk', 'chiptype.txt')
    if os.path.exists(dstchipinfo):
        os.remove(dstchipinfo)
    shutil.copyfile(srcchipinfo, dstchipinfo)
    
    #read new_typedef
    project_path = os.path.join(os.getcwd(), 'target', project_name)
    modify_header_file(project_path, dstname)

    add_cmakelist_macros_from_target_inc()
    parse_and_copy_files()
    # if not compile_fibo_libc():
    #     print("compile libc fail")
    #     return

    # if tarsdk:
    #     nowdate = time.strftime("%Y%m%d%H%M%S", time.localtime(time.time()))
    #     tarcmd = "cd %s; tar -zcf coresdk%s.tar.gz core_sdk" % (os.path.join(os.getcwd(), 'gccout'), nowdate)
    #     print("exec %s" % (tarcmd))
    #     os.system(tarcmd)
def wrap_libc_api():
    apinames = []
    libcfile = "fibocom/fibo-framework/application/opencpu/sdk/newlib/vsprintf.c"
    toolchainfile = "gccout/core_sdk/build/toolchain-gcc.cmake"

    if "OPEN-EZVIZ" in project_name:
        print(f"{project_name} unneed LIBC_WRAP")
        return False
    else:
        with open(libcfile, "r") as fp:
            for ln in fp.readlines():
                wraplibc_api = re.search(r'__wrap_(.+)\s*\(', ln)
                if wraplibc_api is not None:
                    apinames.append(f"--wrap={wraplibc_api[1]}")

        ldflagslien = f"set(FIBOCOM_LIBC_WRAP {' '.join(apinames)})"
        with open(toolchainfile, "a") as file:
            file.write(f"{ldflagslien}\n")

def copy_app_file():
    global app_src
    src_path = os.path.join(os.getcwd(), 'fibocom', 'fibo-framework', 'application', 'opencpu', 'custom', app_src, 'src')   
    dst_path = os.path.join(os.getcwd(), 'gccout', 'core_sdk', 'src')
    print(f"Copied {src_path} to {dst_path}")
    if os.path.exists(src_path):
        if os.path.exists(dst_path):
            shutil.rmtree(dst_path)
        shutil.copytree(src_path, dst_path)
        print(f"Copied {src_path} to {dst_path}")
    else:
        print(f"Source path {src_path} does not exist.")

def copy_testcase_lib():
    targetlib = os.path.join(os.getcwd(), 'target', project_name)
    for root, _, files in os.walk(targetlib):
        for file in files:
            if file == 'libtestcase.a':
                sourcefile = os.path.join(root, file)
                dstfile = os.path.join(os.getcwd(), 'gccout', 'core_sdk', 'lib', file)
                shutil.copy(sourcefile, dstfile)
                print(f'copy {sourcefile}-->{dstfile}')
                return
            
    targetlib = os.path.join(os.getcwd(), 'gccout', f'ec7xx_0h00_{chipType}', project_name)
    for root, _, files in os.walk(targetlib):
        for file in files:
            if file == 'libtestcase.a':
                sourcefile = os.path.join(root, file)
                dstfile = os.path.join(os.getcwd(), 'gccout', 'core_sdk', 'lib', file)
                shutil.copy(sourcefile, dstfile)
                print(f'copy {sourcefile}-->{dstfile}')
                return


def pack(spath, packet):
    total_size = sum(os.path.getsize(os.path.join(dirpath, filename)) for dirpath, dirnames, filenames in os.walk(spath) for filename in filenames)
    bpath = os.path.basename(spath)
    processed_size = 0
    with tarfile.open(packet, "w:gz") as tar:
        for dirpath, dirnames, filenames in os.walk(spath):
            for filename in filenames:
                file_path = os.path.join(dirpath, filename)
                rpath = os.path.relpath(file_path, spath)
                tar.add(file_path, arcname=os.path.join(bpath, rpath))
                processed_size += os.path.getsize(file_path)
                progress = int((processed_size / total_size) * 100)
                sys.stdout.write("\r\033[33mPackaging: [%-50s] %d%%\033[0m" % ('>' * int(progress / 2), progress))
                sys.stdout.flush()
    print("\nPackaging completed!")

# sample_code_dir = os.path.join(os.getcwd(), "fibocom", "opencpu", "sample")
Path(os.path.join(os.getcwd(), 'out')).mkdir(parents=True, exist_ok=True)

def main(chipinfo, action):
    get_project_name()
    core_export_list = os.path.join(os.getcwd(), 'target', project_name, 'core_export.list')
    print(f"core_export dir is : {core_export_list}")
    if os.path.exists(core_export_list):
        print("core sdk core_export.list use Customization core_export.list!")
    else :
        #PLAT/fibocom/fibo-framework/application/opencpu/core_export.list
        core_export_list = os.path.join(os.getcwd(), "fibocom", "fibo-framework", "application", "opencpu", "core_export.list")
    print(f"core_export dir is : %s {core_export_list}"% core_export_list)
    
    if action == 'api':
        create_api_list(core_export_list)
        chiptypeinfo = os.path.join(os.getcwd(), "gccout","chiptype.txt")
        if os.path.exists(chiptypeinfo):
            os.remove(chiptypeinfo)
        with open(chiptypeinfo, "w") as fp:
            fp.write(chipinfo.upper())
    elif action == 'notar':
        create_api_list(core_export_list, True, True)
        create_opencpu_sdk(core_export_list)
        copy_app_file() 
        compile_fibo_stub()
        wrap_libc_api()
    elif action == 'appcompile':
        create_api_list(core_export_list, True, True)
        create_opencpu_sdk(core_export_list)
        copy_app_file()    
        compile_fibo_stub()
        wrap_libc_api()
        copy_testcase_lib()
        compile_fibo_app()
        compile_fibo_testcase()
    else:
        create_api_list(core_export_list, True, True)
        create_opencpu_sdk(core_export_list)
        copy_app_file()
        compile_fibo_stub() 
        wrap_libc_api()
        nowdate = time.strftime("%Y%m%d%H%M%S", time.localtime(time.time()))
        spath = os.path.join(os.getcwd(), "gccout","core_sdk")
        packet = os.path.join(os.getcwd(), "gccout", "coresdk%s.tar.gz") % nowdate
        pack(spath, packet)

if __name__ == '__main__':    
    if len(sys.argv) == 3 and sys.argv[1] == 'api': 
        main(sys.argv[2], sys.argv[1])
    elif len(sys.argv) == 2 and sys.argv[1] == 'notar':
        main(None, sys.argv[1])
    elif len(sys.argv) == 2 and sys.argv[1] == 'appcompile':
        main(None, sys.argv[1])
    else:
        main(None, '')
