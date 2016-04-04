# Dynamic-Linking
This project explores dynamic linking.


Part 1 of the Project:

~~~~~~~~
lab.txt
~~~~~~~~

My first step was to create a program in C that should
compute cos(0.5) and print it to output using printf using
the specific format, "%.17g". This format means print out 
the answer with 17 digits to the right of the decimal point.

	My output: 0.87758256189037276

Next, I used the ldd command on the program to see which 
dynamic libraries were being used. My output was:

	linux-vdso.so.1 =>  (0x00007ffdf3667000)
	libc.so.6 => /lib64/libc.so.6 (0x00007f9acfdaa000)
	/lib64/ld-linux-x86-64.so.2 (0x00007f9ad0192000)

I then ran strace on the program and noticed a variety of 
system calls that were part of stdio.h which were called
using dynamic linking. The system calls open(), close(),
mmap(), read() and write() made up the majority of the 
strace output. Other calls used were execve(), access(),
fstat(), mprotect() and arch_prctl(). 

Now I ran the command: 
	
	ls /usr/bin | awk 'NR%101==nnnnnnnnn%101'

but with my UID inserted instead of the 9 ns. This outputted
about two dozen random commands in /usr/bin. I then used the 
ldd command on all of them to see what each was calling:

abs2rel: 					not a dynamic executable
bashbug: 					not a dynamic executable
chardetect: 				not a dynamic executable
cube_canonize:				No errors
dgawk:						No errors
eu-elflint:					No errors
gdb: 						No errors
gobject-query: 				No errors
gv: 						No errors
ibus-table-createdb:    	not a dynamic executable
jcmd: 						libjli.so => not found
								- After researching what this
							  	  file did, I learned this shared
							  	  object file is only readable by 
							  	  root, so not /usr/bin.
klipper: 					No errors
ldns-test-edns: 			No errors
Mail: 						No errors
msgunfmt: 					No errors
okteta: 					No errors
pamsharpmap: 				No errors
pbmtopgm: 					No errors
pilot: 						No errors
pnmrotate: 					No errors
ppmmix: 					No errors
python: 					No errors
rmiregistry: 				libjli.so => not found
								- Same error as for jcmd.
servicemenuinstallation: 	not a dynamic executable
sqlite3: 					No errors
tchtest: 					No errors
unix2dos: 					No errors
xbiff: 						No errors
xzcat: 						No errors

My final task is to get a sorted list of every dynamic library 
that is used by any of the commands on your list, with no 
duplicates. To do this I first copied all the ldd outputs into a 
single file. I then used the command:

	sed "s/[=>].*//" calls1.txt > test1.txt 

to delete everything in the file after the arrows, =>. This removed
everything but the system calls that are called by my /usr/bin
commands. This included actual numerical addresses and path names to
each system call, like below:

	libcube4.so.7 => /lib64/libcube4.so.7 (0x00007f60252ee000)

Once all the calls were in a single file, I used sort in the command:

	cat test1.txt | sort -u > sorted.txt

to sort the calls and remove the non-unique calls. (Which is done by
the -u flag) The file of sorted calls is shown below:

	/lib64/ld-linux-x86-64.so.2 
 	linux-vdso.so.1 
	libacl.so.1 
	libattica.so.0.4 
	libattr.so.1 
	libaudit.so.1 
	libbz2.so.1 
	libcap.so.2 
	libcom_err.so.2 
	libcroco-0.6.so.3 
	libcrypto.so.10 
	libcrypt.so.1 
	libc.so.6 
	libcube4.so.7 
	libdbus-1.so.3 
	libdbusmenu-qt.so.2 
	libdl.so.2 
	libdw.so.1 
	libelf.so.1 
	libexpat.so.1 
	libfam.so.0 
	libffi.so.6 
	libfontconfig.so.1 
	libfreebl3.so 
	libfreetype.so.6 
	libgcc_s.so.1 
	libgettextlib-0.18.2.so 
	libgettextsrc-0.18.2.so 
	libglib-2.0.so.0 
	libgobject-2.0.so.0 
	libgomp.so.1 
	libgssapi_krb5.so.2 
	libgthread-2.0.so.0 
	libICE.so.6 
	libjli.so 
	libk5crypto.so.3 
	libkasten2controllers.so.2 
	libkasten2core.so.2 
	libkasten2gui.so.2 
	libkasten2okteta1controllers.so.1 
	libkasten2okteta1core.so.1 
	libkasten2okteta1gui.so.1 
	libkcmutils.so.4 
	libkdecore.so.5 
	libkdeinit4_klipper.so 
	libkdeui.so.5 
	libkeyutils.so.1 
	libkfile.so.4 
	libkio.so.5 
	libknewstuff3.so.4 
	libkparts.so.4 
	libkrb5.so.3 
	libkrb5support.so.0 
	liblber-2.4.so.2 
	libldap-2.4.so.2 
	libldns.so.1 
	liblzma.so.5 
	libm.so.6 
	libncurses.so.5 
	libnepomukquery.so.4 
	libnepomuk.so.4 
	libnepomukutils.so.4 
	libnetpbm.so.11 
	libnspr4.so 
	libnss3.so 
	libnssutil3.so 
	libokteta1core.so.1 
	libokteta1gui.so.1 
	libpam.so.0 
	libpcap.so.1 
	libpcre.so.1 
	libplc4.so 
	libplds4.so 
	libpng15.so.15 
	libpthread.so.0 
	libpython2.7.so.1.0 
	libqca.so.2 
	libQtCore.so.4 
	libQtDBus.so.4 
	libQtGui.so.4 
	libQtNetwork.so.4 
	libQtScript.so.4 
	libQtScriptTools.so.4 
	libQtSvg.so.4 
	libQtXml.so.4 
	libreadline.so.6 
	libresolv.so.2 
	librt.so.1 
	libsasl2.so.3 
	libselinux.so.1 
	libsmime3.so 
	libSM.so.6 
	libsolid.so.4 
	libsopranoclient.so.1 
	libsoprano.so.4 
	libsqlite3.so.0 
	libssl3.so 
	libssl.so.10 
	libstdc++.so.6 
	libstreamanalyzer.so.0 
	libstreams.so.0 
	libtinfo.so.5 
	libtokyocabinet.so.9 
	libudev.so.1 
	libunistring.so.0 
	libutil.so.1 
	libuuid.so.1 
	libX11.so.6 
	libXau.so.6 
	libXaw3d.so.8 
	libXaw.so.7 
	libxcb.so.1 
	libXcursor.so.1 
	libXdmcp.so.6 
	libXext.so.6 
	libXfixes.so.3 
	libXft.so.2 
	libXinerama.so.1 
	libXi.so.6 
	libxml2.so.2 
	libXmu.so.6 
	libXpm.so.4 
	libXrandr.so.2 
	libXrender.so.1 
	libXt.so.6 
	libXtst.so.6 
	libz.so.1 
	linux-vdso.so.1
