# RendererCmdController
Use a tcp server control the renderer's plugin and system settings.

How to use

open bash:
    1,telnet 127.0.0.1 5000
    
    2,user Gearslogy (this cmd is register user to control the renderer)
    
    3,kc::help (kc::help ,kc:: is a command prefix,it have kc::help,kc::lsr,kc::lstask,kc::killtask)
    
    4,sc::help (sc::help ,sc:: is a command prefix,it send command to the renderer)
    


sc:: command exp:
    kc::lsr (list all of the client that had connected the server)
    
    {
        HOSTNAME,TCPPORT,IP,AUTH
        
        render_001,6,127.0.0.1,RENDERER
        
        render_002,12,192.168.10.198,RENDERER
        
        render_003,14,192.168.10.200,RENDERER
        
        Gearslogy,15,127.0.0.1,CONTORLLER
        
        
    }
    sc::6::ls -al /  (meaning of you can send a command to the tcp port 6) 
    
    total 162
    dr-xr-xr-x.   26 root root  4096 Feb 14 10:20 .
    dr-xr-xr-x.   26 root root  4096 Feb 14 10:20 ..
    -rw-r--r--.    1 root root     0 Feb 14 10:20 .autofsck
    dr-xr-xr-x.    2 root root  4096 Apr 30  2016 bin
    dr-xr-xr-x.    5 root root  1024 Mar 14  2015 boot
    drwxr-xr-x.    2 root root  4096 Jul 24  2015 cgroup
    drwx------.    3 root root  4096 Jan  2  2000 .dbus
    drwxr-xr-x.   19 root root  4060 Feb 14 12:16 dev
    drwxr-xr-x.  136 root root 12288 Feb 14 12:16 etc
    drwxr-xr-x.    4 root root  4096 Jan  2  2000 home
    dr-xr-xr-x.   13 root root  4096 Mar 17  2016 lib
    dr-xr-xr-x.   10 root root 12288 Apr 26  2016 lib64
    drwx------.    2 root root 16384 Jan  2  2000 lost+found
    drwxr-xr-x.    4 root root  4096 Feb 14 12:16 media
    drwxr-xr-x.    2 root root     0 Feb 14 10:20 misc
    drwxrwxrwx.    7 root root  4096 May 26  2016 mnt
    drwxr-xr-x.    2 root root     0 Feb 14 10:20 net
    drwxrwxrwx.   13 root root  4096 Dec 27 11:01 opt
    dr-xr-xr-x. 1147 root root     0 Feb 14 10:19 proc
    dr-xr-x---.   18 root root  4096 Feb 14 11:21 root
    dr-xr-xr-x.    2 root root 12288 May 26  2016 sbin
    drwxr-xr-x.    7 root root     0 Feb 14 10:19 selinux
    drwxr-xr-x.    2 root root  4096 Sep 23  2011 srv
    drwxr-xr-x.   13 root root     0 Feb 14 10:19 sys
    drwxrwxrwx.   25 root root 49152 Feb 14 13:08 tmp
    drwxr-xr-x.   16 root root  4096 Jun  2  2016 usr
    drwxr-xr-x.   23 root root  4096 Mar 28  2016 var
            
