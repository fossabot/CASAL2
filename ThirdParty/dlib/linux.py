import os
import os.path
import subprocess
import sys
import shutil
from distutils import dir_util

import Globals

class Builder:
  version_ = '18.6'
  
  def start(self):    
    # Variables
    fileName        = 'dlib-18.6'
  
    # Clean our any existing files if they already exist
    print '-- Cleaning DLib files'
    if os.path.exists(fileName):
      shutil.rmtree(fileName)
    if os.path.exists(Globals.target_include_path_ + 'dlib'):
      shutil.rmtree(Globals.target_include_path_ + 'dlib')
    # Decompress our archive
    print '-- Decompressing DLib - check isam_unzip.log'
    if os.path.exists(fileName + '.zip'):
      os.system('unzip ' + fileName + '.zip 1> isam_unzip.log 2>&1')
    
    # Move our headers and libraries
    print '-- Moving headers'
    dir_util.copy_tree(fileName + '/dlib', Globals.target_include_path_)
        
    return True