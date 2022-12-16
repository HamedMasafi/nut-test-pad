import json
import ntpath
import subprocess
import time
import shutil
import os

from datetime import timedelta

import file
import msg

qmake = ""
qtif_path = ""
output_file_name = ""
msgctl = msg.message_controller()

def run_proc(p, ccwd="build/cache", showElapsed = False):
    name = ntpath.basename(p[0])
    msgctl.print(f"Running {name}")

    if showElapsed:
            start = time.time()

    with open(f"build/logs/{name}.txt","w") as f:
        if ccwd == "":
            proc = subprocess.Popen(p, stdin=None, stdout=f, stderr=f)
        else:
            proc = subprocess.Popen(p, cwd=ccwd, stdin=None, stdout=f, stderr=f)

    proc.communicate()
    if proc.returncode == 0:
        if (showElapsed):
            end = time.time()
            e = end - start
            t = str(timedelta(seconds=e))
            msgctl.result(True, t)
        else:
            msgctl.result(True)
    else:
        msgctl.result(False)

def create_paths():
    file.mkdir("build")
    file.mkdir("build/cache")
    # file.mkdir(dir_build_packages)
    file.mkdir("build/logs")
    # file.mkdir(dir_build_repo)
    file.mkdir("build/package")
    file.mkdir("build/output")
    return True

def readParams():
    f = open('data.json')
    j = json.load(f)

    global qmake
    global qtif_path
    global output_file_name
    qmake = j["qmake"]
    qtif_path = j["qtif_path"]
    output_file_name = j["output_file_name"]

def build():
    run_proc([qmake, '../../../nut-pad.pro'])
    run_proc(["make"])

def run_cqtdeployer():
    # self.temp_path = tempfile.gettempdir() + "/cqt"
    # ttp = self.temp_path + "/cache"
    # self.output_path = ttp + "/output/"
    
    
    p =[
        "cqtdeployer",
        "-bin", f"build/package/{output_file_name}",

        # "-qmlDir", self.qml_dir,
        "-qmake", qmake,
        "clear",
        # "force-clear",
        # "-recursiveDepth", '4',
        # "-libDir", "./output",
        # "-targetDir", self.temp_path,
        "-qmlOut", 'output',
        "-libOut", 'output',
        "-pluginOut", 'output',
        "-binOut", 'output',
        "-recOut", 'output',
        "-trOut", "output/translations",

        "-verbose", '3',
        "-targetDir", "build/output",
    ]
    with open(f"build/logs/CQtDeployer.txt","w") as f:
        subprocess.run(p, stdin=None, input=None, stdout=f, stderr=f)
    # pp = self.deploy_tool_bin
    # for param in p:
    #     pp += " " + param
    # os.popen(pp)    
    # os.unlink(ttp + "/output/t")
    # os.unlink(ttp + "/output/qt.conf")
    # return os.path.exists(self.output_path)
    
def run_copy_file(src, dst):
    file_name = os.path.basename(src)
    msgctl.print(f"Copying {file_name}")
    if not os.path.exists(src):
        msgctl.result(False)

    if os.path.exists(dst):
        os.remove(dst)

    shutil.copyfile(src, dst)
    msgctl.result(os.path.exists(dst))

def init_packages():
    run_copy_file(f"build/cache/{output_file_name}", f"build/package/{output_file_name}")

if __name__ == "__main__":
    readParams()
    print(f"qmake is : {qmake}")
    create_paths()
    build()
    init_packages()
    run_cqtdeployer()