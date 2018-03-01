import sys
import re
from collections import OrderedDict

if(len(sys.argv) < 3):
    print("args: project_dir binpath (custom_c_sources)\n");
    exit();
    
project_dir = sys.argv[1];
binpath = sys.argv[2];
custom_c_sources = "";
if(len(sys.argv) > 3):
    custom_c_sources = sys.argv[3];
custom_c_sources = custom_c_sources.replace(",", "\n");

#Read the Makefile
makefile = open(project_dir+"/Makefile", "r");
content = makefile.read();
makefile.close();

#Modify Makefile
binpath_str = "#######################################\n# binaries\n#######################################\nBINPATH = ";
binpath_index = content.find(binpath_str)+len(binpath_str);
content = content[0:binpath_index] + binpath + content[content.find('\n', binpath_index):];

#Extract the C_SOURCES content
c_sources = re.search(r'C_SOURCES = (.*?)#', content, re.DOTALL).group(1);

#Remove ' ' and '\' to process only file names
c_sources = c_sources.replace("\\", "").replace(" ", "");

#Split C_SOURCES content into lines, then remove doubles
c_sources = [""] + custom_c_sources.splitlines() + c_sources.splitlines()[1:];
c_sources = list(OrderedDict.fromkeys(c_sources));

#Resumes line breaks, and add the last Line Feed character
c_sources = " \\\n".join(c_sources)+"\n";

#Replace the C_SOURCES content
start = content.find("C_SOURCES = ")+12;
end = content.find("\n# ASM sources");
content = content[0:start] + c_sources + content[end:];
    
#Erase and rewrite Makefile
makefile = open(project_dir+"/Makefile", "w");
makefile.write(content);
