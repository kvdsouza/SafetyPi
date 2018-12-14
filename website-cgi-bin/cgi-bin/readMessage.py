Last login: Thu Dec 13 08:59:32 on ttys001
Kyles-MacBook-Pro-261:~ Kyle1$ ssh kvdsouza@myth.stanford.edu
kvdsouza@myth.stanford.edu's password:
Last login: Thu Dec 13 11:34:50 2018 from 171.64.70.60
=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
 myth60.stanford.edu
 Ubuntu 16.04 (Linux 4.4.0-101-generic amd64)
 8 x Intel(R) Core(TM) i7-7700K CPU @ 4.20GHz, 31.30 GB RAM, 31.88 GB swap
=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
   -> For help with your SUNetID, AFS, or class dirs call 725-HELP, or visit
      http://helpsu.stanford.edu
   -> For problems with hardware or local/OS software, visit
      http://support.cs.stanford.edu
   -> The myths are not for CPU-intensive workloads. For alternative
      computers see http://farmshare.stanford.edu
   -> The new myths are: myth51 - 66

kvdsouza@myth60:~$ ls
cgi-bin  Documents  esriLicense.config	module	News	  private  SOC180B
Desktop  Downloads  Mail		Music	Pictures  public   WWW
kvdsouza@myth60:~$ cd cgi-bin
kvdsouza@myth60:~/cgi-bin$ ls
messages.txt   names.txt    printenv.pl     sendMessage.py  testWrite.cgi
nameSave.html  phpinfo.php  readMessage.py  test.py
kvdsouza@myth60:~/cgi-bin$ cat messages.txt
hello humans
kvdsouza@myth60:~/cgi-bin$ cat messages.txt
hello humans2
kvdsouza@myth60:~/cgi-bin$ cat messages.txt
toohotblah
kvdsouza@myth60:~/cgi-bin$ cat messages.txt
toohotblah
kvdsouza@myth60:~/cgi-bin$ cat messages.txt
toohot
kvdsouza@myth60:~/cgi-bin$ cat messages.txt
hello humans2
kvdsouza@myth60:~/cgi-bin$ cat messages.txt
hello humans2
kvdsouza@myth60:~/cgi-bin$ cat messages.txt
toohot
kvdsouza@myth60:~/cgi-bin$ cat messages.txt
toohot
kvdsouza@myth60:~/cgi-bin$ cat messages.txt
toohot
kvdsouza@myth60:~/cgi-bin$ cat messages.txt
toohot
kvdsouza@myth60:~/cgi-bin$ cat messages.txt
toohot
kvdsouza@myth60:~/cgi-bin$ cat messages.txt
toohot
kvdsouza@myth60:~/cgi-bin$ cat messages.txt
toohot
kvdsouza@myth60:~/cgi-bin$ cat messages.txt
toohot
kvdsouza@myth60:~/cgi-bin$ cat messages.txt
hello humans2
kvdsouza@myth60:~/cgi-bin$ cat messages.txt
toohot
kvdsouza@myth60:~/cgi-bin$ packet_write_wait: Connection to 171.64.15.20 port 22: Broken pipe
Kyles-MacBook-Pro-261:~ Kyle1$ ssh kvdsouza@myth.stanford.edu
kvdsouza@myth.stanford.edu's password:
Last login: Thu Dec 13 15:22:00 2018 from 171.64.70.60
=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
 myth60.stanford.edu
 Ubuntu 16.04 (Linux 4.4.0-101-generic amd64)
 8 x Intel(R) Core(TM) i7-7700K CPU @ 4.20GHz, 31.30 GB RAM, 31.88 GB swap
=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
   -> For help with your SUNetID, AFS, or class dirs call 725-HELP, or visit
      http://helpsu.stanford.edu
   -> For problems with hardware or local/OS software, visit
      http://support.cs.stanford.edu
   -> The myths are not for CPU-intensive workloads. For alternative
      computers see http://farmshare.stanford.edu
   -> The new myths are: myth51 - 66

kvdsouza@myth60:~$ cd cgi-bin
kvdsouza@myth60:~/cgi-bin$ ls
messages.txt   names.txt    printenv.pl     sendMessage.py  testWrite.cgi
nameSave.html  phpinfo.php  readMessage.py  test.py
kvdsouza@myth60:~/cgi-bin$ cd readMessage.py
-bash: cd: readMessage.py: Not a directory
kvdsouza@myth60:~/cgi-bin$ cat messages.txt
housesafe.
kvdsouza@myth60:~/cgi-bin$ vim readMessage.txt
kvdsouza@myth60:~/cgi-bin$ vim readMessage.py
kvdsouza@myth60:~/cgi-bin$ vim readMessage.py
kvdsouza@myth60:~/cgi-bin$ vim readMessage.py
kvdsouza@myth60:~/cgi-bin$ vim readMessage.py
kvdsouza@myth60:~/cgi-bin$ vim readMessage.py

#text stuff for html

print("""<h1 style="color: #5e9ca0;"><span style="background-color: #000000; color: #ff6600;">Fire Security!</span></h1>""")
print("""<h3><span style="background-color: #000000; color: #ff6600;">Have you ever wanted to know whether you turned your oven or stove off?</span></h3>""")
print("""<h1><span style="color: #000000;">This Application Allows You To Check Up on Your&nbsp;Kitchen (And Protect Your Home)!</span></h1>""")
print("""<h2>Please Exercise Caution - Tread Carefully Before Fully Trusting This Code With Your Home :-)</h2>""")
print("""<h3><em>Your kitchen status is:</em></h3>""")
print("""<p>&nbsp;</p>""")

try:
  with open("messages.txt") as f:
    lines = f.readlines()
    lines = [x[:-1] for x in lines] # remove newlines
    print("""<h3>Your kitchen is {}! (at temperature Fahrenheit 30) :). </h3>""".format(lines[-1]))
except FileNotFoundError:
    print("""<h3>Your kitchen is safe""")

print("""<p>&nbsp;</p>
<p>A CS107E Creation - created by Kyle D'Souza, with help and support of 107E friends, teachers, and colleagues.&nbsp;</p>""")
