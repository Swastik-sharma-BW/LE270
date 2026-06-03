import re
import os
import subprocess
import shutil
import sys

def convert_bin(xml_file, out_file):
    cmd = ['./fibconfig', '-i', xml_file, '-o', out_file]
    try:
        if sys.version_info >= (3, 7):  
            output = subprocess.check_output(cmd, cwd=os.path.dirname(os.path.abspath(__file__)), 
                                            text=True)  
        else:  
            output = subprocess.check_output(cmd, cwd=os.path.dirname(os.path.abspath(__file__)), 
                                            universal_newlines=True)  
    # print(f'output: {output}')  # show echo
    except subprocess.CalledProcessError as e:  
        print(f"--- error: convert_bin() failed: \nerrcode: {e.returncode} \nerrinfo: {e.output}")  
        return False

    pattern = r"Make binary file successfully!!!"
    match = re.search(pattern, output)
    return bool(match)   

def parse_bin(bin_file):
    cmd = ['./fibconfig', '-p', bin_file]  
    try:
        if sys.version_info >= (3, 7):  
            output = subprocess.check_output(cmd, cwd=os.path.dirname(os.path.abspath(__file__)),  
                                            text=True)
        else:  
            output = subprocess.check_output(cmd, cwd=os.path.dirname(os.path.abspath(__file__)),  
                                            universal_newlines=True)
    # print(f'output: {output}')  # show echo
    except subprocess.CalledProcessError as e:  
        print(f"--- error: parse_bin() failed: \nerrcode: {e.returncode} \nerrinfo: {e.output}")  
        return None

    pattern = r"data version: (\S+)"
    match = re.search(pattern, output)
    if match:
        data_version = match.group(1)
        return data_version
    else:
        return None

def pkg_devcfg(def_xml, cus_xml, out_dir):
    if not os.path.exists(out_dir):
        os.makedirs(out_dir)
    
    print(f'--- def_xml: {os.path.relpath(def_xml)}')
    print(f'--- cus_xml: {os.path.relpath(cus_xml)}')
    # print(f'--- out_dir: {os.path.relpath(out_dir)}')

    # default configuration
    if not os.path.isfile(def_xml):
        print(f'--- def_xml not existed')
        return

    print(f'--- convert default configuration ...')
    basename = os.path.splitext(os.path.basename(def_xml))[0]
    out_file = os.path.abspath(os.path.join(out_dir, basename)) + '.bin'

    result = convert_bin(def_xml, out_file)
    # print(f'--- convert_bin(): {bool(result)}')
    if not result:
        return

    def_data_ver = parse_bin(out_file)
    print(f'    output file : {os.path.relpath(out_file)}')
    print(f"    data version: {def_data_ver}")
    if def_data_ver is None:
        return

    shutil.copy(def_xml, out_dir)

    # customer configuration
    if not os.path.isfile(cus_xml):
        print(f'--- cus_xml not existed')
        return

    print(f'--- convert customer configuration ...')
    cus_outfile = os.path.splitext(os.path.basename(cus_xml))[0]
    out_file = os.path.abspath(os.path.join(out_dir, cus_outfile)) + '.bin'

    result = convert_bin(cus_xml, out_file)
    # print(f'--- convert_bin(): {result}')
    if not result:
        return

    cus_data_ver = parse_bin(out_file)
    print(f'    output file : {os.path.relpath(out_file)}')
    print(f"    data version: {cus_data_ver}")
    if cus_data_ver is None:
        return

    shutil.copy(cus_xml, out_dir)
    
    data_ver_file = os.path.join(out_dir, 'devcfg_ver_info.txt')

    print(f"--- data version recorded in: {os.path.relpath(data_ver_file)}")
    with open(data_ver_file, 'w', encoding='utf-8') as file:  
        file.write('default configuration data version : ' + def_data_ver)
        file.write('\n')
        file.write('customer configuration data version: ' + cus_data_ver)
