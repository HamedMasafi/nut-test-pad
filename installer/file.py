import fileinput
import re
import ntpath
import os
import shutil
import traceback
import sys

def read(name):
    if not os.path.exists(name):
        return None
    f = open(name, "r")
    return f.read()

def write(name, content):
    f = open(name, "w+")
    f.write(content)
    f.close()

def write_exception(name, ex):
    f = open(f"build/logs/{name}.txt", "w+")
    f.write(str(ex))
    f.close()

def replace(filename, search, replace):
    with fileinput.FileInput(filename, inplace=True) as file:
        for line in file:
            line = re.sub(search, replace, line)
            print(line, end='')


def recursive_overwrite(src, dest, ignore=None):
    if os.path.isdir(src):
        if not os.path.isdir(dest):
            os.makedirs(dest)
        files = os.listdir(src)
        if ignore is not None:
            ignored = ignore(src, files)
        else:
            ignored = set()
        for f in files:
            if f not in ignored:
                recursive_overwrite(os.path.join(src, f), 
                                    os.path.join(dest, f), 
                                    ignore)
    else:
        shutil.copyfile(src, dest)

def file_copy_flow_link(src, dst):
    dstfile = dst + "/" +  ntpath.basename(src)
    # print('cp %s to %s' % (src,dstfile))
    if os.path.islink(src):
        if os.path.lexists(dstfile):
            os.unlink(dstfile)
        linkto = os.readlink(src)
        file_copy_flow_link(os.path.dirname(src) + "/" + linkto, dst)
    else:
        shutil.copy(src, dstfile)

def copytree(src, dst, symlinks=False, ignore=None):
    for item in os.listdir(src):
        s = os.path.join(src, item)
        d = os.path.join(dst, item)
        try:
            if os.path.isdir(s):
                shutil.copytree(s, d, symlinks, ignore)
            else:
                shutil.copy2(s, d)
        except shutil.Error:
            pass
        except FileExistsError:
            pass

def mkdir(directory):
    if not os.path.exists(directory):
        os.makedirs(directory)


def platform_file_name(filename):
    if sys.platform == 'linux' and filename.endswith(".dll"):
        bn = os.path.splitext(os.path.basename(filename))[0]
        newfilename = os.path.dirname(filename) + "/lib" + bn + ".so"
        return newfilename
    return filename

def copy_files_from_list(f, qt_path, out_path):
    count = 0
    total = 0
    content = read(f).split('\n')
    for l in content:
        if l != '' and l != None:
            
            parts = l.format(qt_path = qt_path, out_path = out_path).split(',')
            if len(parts) != 2:
                continue

            total += 1
            src = platform_file_name(parts[0])
            dst = platform_file_name(parts[1])
            #+ "/" + os.path.basename(src)

            # print(f"copy from {src} to {dst}")
            if os.path.exists(dst):
                continue
            # try:
            if os.path.isdir(src):
                shutil.copytree(src, dst)
            else:
                dir = os.path.dirname(dst)
                if not os.path.exists(dir):
                    os.makedirs(dir)
                shutil.copyfile(src, dst)
            count += 1
            # except FileExistsError:
            #     pass
            # except:
            #     print("Error: " + l)
    return count, total