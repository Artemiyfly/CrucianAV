FTP_ADRESS = "93.189.45.35"
FTP_USER = "Artemiy_fly@mail.ru"
FTP_PASSWORD = "74aa4a3f"

import os.path as p
import os
import sys
from ftplib import FTP
import shutil

class core:
 def update(this, last_update):
  connection = FTP(FTP_ADRESS, timeout = 4)
  resp = connection.login(FTP_USER, FTP_PASSWORD)
  if (resp[0:3] != "230"):
   print("FTP CONNECTION ERROR")
   return "ErrC"
  connection.cwd("shrd")
  curDir = p.dirname(sys.argv[0]) + "\\"
  if connection.sendcmd("MDTM core.exe")[4:18] > last_update:
   cFile = open(curDir + "core.exe", "wb") #Updating core
   connection.retrbinary("RETR core.exe", cFile.write)
   cFile.close()
  if connection.sendcmd("MDTM server.exe")[4:18] > last_update:
   cFile = open(curDir + "server.exe", "wb") #Updating server
   connection.retrbinary("RETR server.exe", cFile.write)
   cFile.close()
  latest_version = connection.sendcmd("MDTM layout")[4:18]
  if latest_version > last_update:
   if p.exists(curDir + "viruses"):
    shutil.rmtree(curDir + "viruses")
   os.makedirs(curDir + "viruses")
   connection.cwd("viruses")
   for vir in connection.nlst():
    if vir[0] != '.':
     cFile = open(curDir + "viruses\\" + vir, "wb")
     connection.retrbinary("RETR " + vir, cFile.write)
     cFile.close()
   os.system('"' + curDir + "server.exe" + '"')
   #shutil.rmtree(curDir + "viruses")
  connection.quit()
  return latest_version
 
 def send(this, path):
  connection = FTP(FTP_ADRESS, timeout = 4)
  resp = connection.login(FTP_USER, FTP_PASSWORD)
  if (resp[0:3] != "230"):
   print("FTP CONNECTION ERROR")
   return "ErrC"
  connection.cwd("shrd//viruses")
  connection.storbinary("STOR " + str(len(connection.nlst()) - 1), open(path, "r"))
  connection.quit()
  return 0
a = core()
a.update("1")