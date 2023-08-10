# Bump mmCEsim Version Number
#
# Author: Teddy van Jerry (Wuqiong Zhao)
# Date: 2023-08-10

import os
import argparse
import re
import datetime

# Command Line Arguments
# ----------------------
parser = argparse.ArgumentParser(description='Bump mmCEsim Version Number')
parser.add_argument('--from', dest='from_version', type=str, default=None, help='From Version Number')
parser.add_argument('--to', dest='to_version', type=str, default=None, help='To Version Number')

args = parser.parse_args()
assert args.from_version is not None and args.to_version is not None, 'Please specify --from and --to version number'
assert args.from_version != args.to_version, 'From and To version number should be different'

# Print Configuration Information
# -------------------------------
print('Bump mmCEsim Version Number from {} to {}'.format(args.from_version, args.to_version))

# Modify C++ Source and Include Files (Doxygen Comments)
# ------------------------------------------------------
# C++ source files are in directory src/,
# and include files are in directory include/, respectively.

# recursively find all files in directory
def find_files(directory, extension):
    files = []
    for root, _, filenames in os.walk(directory):
        for filename in filenames:
            if filename.endswith(extension):
                files.append(os.path.join(root, filename))
    return files

# find all C++ source and include files
cpp_files = find_files('src', '.cpp') + find_files('tools', '.cpp')
h_files = find_files('include', '.h') + find_files('tools', '.h')
cpp_h_files = cpp_files + h_files

# Replace Version Number Pattern: * @version from_version to * @version to_version
for file in cpp_h_files:
    with open(file, 'r') as f:
        content = f.read()
        content = content.replace('* @version {}'.format(args.from_version), '* @version {}'.format(args.to_version))
    with open(file, 'w') as f:
        f.write(content)

# Modify CMakeLists.txt
# ---------------------
# CMakeLists.txt is in directory ./

# Replace Version Number Pattern: project(mmCEsim VERSION from_version) to project(mmCEsim VERSION to_version)
with open('CMakeLists.txt', 'r') as f:
    content = f.read()
    content = content.replace('project(mmCEsim VERSION {})'.format(args.from_version), 'project(mmCEsim VERSION {})'.format(args.to_version))
with open('CMakeLists.txt', 'w') as f:
    f.write(content)

# Modify include/meta.h
# ---------------------

# There are three macros, _MMCESIM_VER_MAJOR, _MMCESIM_VER_MINOR, _MMCESIM_VER_REVISION
# in include/meta.h, which are used to define the version number of mmCEsim.
# The version number is defined as _MMCESIM_VER_MAJOR._MMCESIM_VER_MINOR._MMCESIM_VER_REVISION.
with open('include/meta.h', 'r') as f:
    content = f.read()
    content = content.replace('#define _MMCESIM_VER_MAJOR {}'.format(args.from_version), '#define _MMCESIM_VER_MAJOR {}'.format(args.to_version))
    content = content.replace('#define _MMCESIM_VER_MINOR {}'.format(args.from_version), '#define _MMCESIM_VER_MINOR {}'.format(args.to_version))
    content = content.replace('#define _MMCESIM_VER_REVISION {}'.format(args.from_version), '#define _MMCESIM_VER_REVISION {}'.format(args.to_version))
with open('include/meta.h', 'w') as f:
    f.write(content)

# Modify Doxygen Configuration File
# ---------------------------------
# Doxygen configuration file is in directory misc/

# Replace Version Number Pattern: PROJECT_NUMBER \s+ = from_version to PROJECT_NUMBER \s+ = to_version
with open('misc/Doxyfile', 'r') as f:
    content = f.read()
    content = content.replace('PROJECT_NUMBER         = {}'.format(args.from_version), 'PROJECT_NUMBER         = {}'.format(args.to_version))
with open('misc/Doxyfile', 'w') as f:
    f.write(content)

# Modify Citation File
# --------------------
# Citation file is ./CITATION.cff

# Replace Version Number Pattern: version: from_version to version: to_version
# Also change the date-released field text to the today (formatted as yyyy-mm-dd), and use regular expression to match.
with open('CITATION.cff', 'r') as f:
    content = f.read()
    content = content.replace('version: {}'.format(args.from_version), 'version: {}'.format(args.to_version))
    content = re.sub(r'date-released: \d{4}-\d{2}-\d{2}', f'date-released: { datetime.date.today() }', content)
with open('CITATION.cff', 'w') as f:
    f.write(content)

# Modify README.md
# ----------------

# Replace Console Output: mmCEsim from_version to mmCEsim to_version
with open('README.md', 'r') as f:
    content = f.read()
    content = content.replace('mmCEsim {}'.format(args.from_version), 'mmCEsim {}'.format(args.to_version))
with open('README.md', 'w') as f:
    f.write(content)

# Modify LaTeX Class File
# -----------------------
# File located at include/mmcesim/tex/simreport.cls

# Change \ProvidesExplClass{simreport}{old_date}{from_version}{Simulation Report} to \ProvidesExplClass{simreport}{yyyy/mm/dd}{to_version}{Simulation Report}
# Also Change \str_const:Nn \c__simreport_mmcesim_ver_str { from_version } to \str_const:Nn \c__simreport_mmcesim_ver_str { to_version }
with open('include/mmcesim/tex/simreport.cls', 'r') as f:
    content = f.read()
    content = re.sub(r'ProvidesExplClass{simreport}{\d{4}/\d{2}/\d{2}}{[0-9.]+}{Simulation Report}', f'ProvidesExplClass{{simreport}}{{{ datetime.date.today():%Y/%m/%d }}}{{{args.to_version}}}{{Simulation Report}}', content)
    content = re.sub(r'c__simreport_mmcesim_ver_str { [0-9.]+ }', f'c__simreport_mmcesim_ver_str {{ {args.to_version} }}', content)
with open('include/mmcesim/tex/simreport.cls', 'w') as f:
    f.write(content)

# Modify VS Code Configuration File
# ---------------------------------
# File located at .vscode/settings.json

# Change "doxdocgen.file.versionTag": "@version from_version" to "doxdocgen.file.versionTag": "@version to_version"
with open('.vscode/settings.json', 'r') as f:
    content = f.read()
    content = content.replace('"doxdocgen.file.versionTag": "@version {}"'.format(args.from_version), '"doxdocgen.file.versionTag": "@version {}"'.format(args.to_version))
with open('.vscode/settings.json', 'w') as f:
    f.write(content)

# Final Messages
# --------------
# Print messages to indicate the end of the script.
print('Done.')
