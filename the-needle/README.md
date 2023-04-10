# The needle

## CHALLENGE DESCRIPTION
As a part of our SDLC process, we've got our firmware ready for security testing. Can you help us by performing a security assessment?

## Walkthrough

Download the file, we get the `firmware.bin`.

It's a ARM zImage `firmware.bin: Linux kernel ARM boot executable zImage (big-endian)`

Look like we need to dig in the content of the binary to find out the key.

Use `binwalk firmware`

```s
DECIMAL       HEXADECIMAL     DESCRIPTION
--------------------------------------------------------------------------------
0             0x0             Linux kernel ARM boot executable zImage (big-endian)
14419         0x3853          xz compressed data
14640         0x3930          xz compressed data
538952        0x83948         Squashfs filesystem, little endian, version 4.0, compression:xz, size: 2068458 bytes, 995 inodes, blocksize: 262144 bytes, created: 2021-03-11 03:18:10
```

Here we'are! We have squashfs filesystem.
Let extract it from the firmware.bin: `dd if=firmware.bin of=fs.bin skip=538952 bs=1 count=16238264`
Then mount it to our `/mnt` : `mount -o loop fs.bin /mnt/armlinux/`

```s
bin  dev  etc  lib  mnt  overlay  proc  rom  root  sbin  sys  tmp  usr  var  www
```

View the tree:

```s
.
├── bin
│   ├── ash -> busybox
│   ├── board_detect
│   ├── busybox
│   ├── cat -> busybox
│   ├── chgrp -> busybox
│   ├── chmod -> busybox
│   ├── chown -> busybox
│   ├── config_generate
│   ├── cp -> busybox
│   ├── date -> busybox
│   ├── dd -> busybox
│   ├── df -> busybox
│   ├── dmesg -> busybox
│   ├── echo -> busybox
│   ├── egrep -> busybox
│   ├── false -> busybox
│   ├── fgrep -> busybox
│   ├── fsync -> busybox
│   ├── grep -> busybox
│   ├── gunzip -> busybox
│   ├── gzip -> busybox
│   ├── ipcalc.sh
│   ├── kill -> busybox
│   ├── ln -> busybox
│   ├── lock -> busybox
│   ├── login -> busybox
│   ├── ls -> busybox
│   ├── mkdir -> busybox
│   ├── mknod -> busybox
│   ├── mktemp -> busybox
│   ├── mount -> busybox
│   ├── mv -> busybox
│   ├── netmsg -> busybox
│   ├── netstat -> busybox
│   ├── nice -> busybox
│   ├── opkg
│   ├── pidof -> busybox
│   ├── ping -> busybox
│   ├── ping6 -> busybox
│   ├── ps -> busybox
│   ├── pwd -> busybox
│   ├── rm -> busybox
│   ├── rmdir -> busybox
│   ├── sed -> busybox
│   ├── sh -> busybox
│   ├── sleep -> busybox
│   ├── sync -> busybox
│   ├── tar -> busybox
│   ├── touch -> busybox
│   ├── true -> busybox
│   ├── ubus
│   ├── uclient-fetch
│   ├── umount -> busybox
│   ├── uname -> busybox
│   ├── vi -> busybox
│   ├── wget -> uclient-fetch
│   └── zcat -> busybox
├── dev
│   └── console
├── etc
│   ├── banner
│   ├── banner.failsafe
│   ├── board.d
│   ├── config
│   ├── crontabs
│   ├── device_info
│   ├── diag.sh
│   ├── dnsmasq.conf
│   ├── dropbear
│   ├── firewall.user
│   ├── fstab
│   ├── group
│   ├── hosts
│   ├── hotplug.d
│   ├── hotplug.json
│   ├── hotplug-preinit.json
│   ├── init.d
│   ├── inittab
│   ├── iproute2
│   ├── localtime -> /tmp/localtime
│   ├── luci-uploads
│   ├── modules.d
│   ├── mtab -> /proc/mounts
│   ├── openwrt_release
│   ├── openwrt_version
│   ├── opkg
│   ├── opkg.conf
│   ├── os-release -> ../usr/lib/os-release
│   ├── passwd
│   ├── ppp
│   ├── preinit
│   ├── profile
│   ├── protocols
│   ├── rc.button
│   ├── rc.common
│   ├── rc.d
│   ├── rc.local
│   ├── resolv.conf -> /tmp/resolv.conf
│   ├── scripts
│   ├── services
│   ├── shadow
│   ├── shells
│   ├── sysctl.conf
│   ├── sysctl.d
│   ├── sysupgrade.conf
│   ├── TZ -> /tmp/TZ
│   └── uci-defaults
├── lib
│   ├── config
│   ├── firmware
│   ├── functions
│   ├── functions.sh
│   ├── ixp4xx.sh
│   ├── ld-musl-armeb.so.1 -> libc.so
│   ├── libblobmsg_json.so
│   ├── libc.so
│   ├── libfstools.so
│   ├── libgcc_s.so.1
│   ├── libjson_script.so
│   ├── libsetlbf.so
│   ├── libubox.so
│   ├── libubus.so
│   ├── libuci.so
│   ├── libvalidate.so
│   ├── modules
│   ├── netifd
│   ├── network
│   ├── preinit
│   ├── upgrade
│   └── wifi
├── mnt
├── overlay
├── proc
├── rom
│   └── note
├── root
├── sbin
│   ├── askfirst
│   ├── devstatus
│   ├── firstboot
│   ├── fw3
│   ├── halt -> ../bin/busybox
│   ├── hotplug-call
│   ├── hwclock -> ../bin/busybox
│   ├── ifconfig -> ../bin/busybox
│   ├── ifdown -> ifup
│   ├── ifstatus
│   ├── ifup
│   ├── init
│   ├── ip -> ../bin/busybox
│   ├── jffs2mark -> jffs2reset
│   ├── jffs2reset
│   ├── kmodloader
│   ├── led.sh
│   ├── logd
│   ├── logread
│   ├── luci-reload
│   ├── mkswap -> ../bin/busybox
│   ├── mount_root
│   ├── mtd
│   ├── netifd
│   ├── pivot_root -> ../bin/busybox
│   ├── poweroff -> ../bin/busybox
│   ├── procd
│   ├── reboot -> ../bin/busybox
│   ├── reload_config
│   ├── route -> ../bin/busybox
│   ├── rpcd
│   ├── start-stop-daemon -> ../bin/busybox
│   ├── switch_root -> ../bin/busybox
│   ├── sysctl -> ../bin/busybox
│   ├── sysupgrade
│   ├── ubusd
│   ├── uci
│   ├── udevtrigger
│   ├── udhcpc -> ../bin/busybox
│   ├── urandom_seed
│   ├── validate_data
│   └── wifi
├── sys
├── tmp
├── usr
│   ├── bin
│   ├── lib
│   ├── libexec
│   ├── sbin
│   └── share
├── var -> /tmp
└── www
    ├── cgi-bin
    ├── index.html
    └── luci-static
```

This system is like a common OpenWRT router.


Use `nmap` to scan the target machine:
```
nmap -sV -sC -Pn 144.126.192.55  -p 31995

PORT      STATE SERVICE VERSION
31995/tcp open  telnet  BusyBox telnetd
Service Info: Host: hwtheneedle-1444568-69999b9697-xtv5x
```

So the target machine serves BusyBox over telnet.

Search for the startup script in the file system:
```s
grep -r "telnet" .
./etc/scripts/telnetd.sh:TELNETD=`rgdb -g /sys/telnetd`
./etc/scripts/telnetd.sh:	echo "Start telnetd ..." > /dev/console
./etc/scripts/telnetd.sh:		telnetd -l "/usr/sbin/login" -u Device_Admin:$sign	-i $lf &
./etc/scripts/telnetd.sh:		telnetd &
./etc/services:telnet		23/tcp
./lib/upgrade/common.sh:				*procd*|*ash*|*init*|*watchdog*|*ssh*|*dropbear*|*telnet*|*login*|*hostapd*|*wpa_supplicant*|*nas*|*relayd*) : ;;
```

Got the telnetd service script:
```s
#!/bin/sh
sign=`cat /etc/config/sign`
TELNETD=`rgdb
TELNETD=`rgdb -g /sys/telnetd`
if [ "$TELNETD" = "true" ]; then
	echo "Start telnetd ..." > /dev/console
	if [ -f "/usr/sbin/login" ]; then
		lf=`rgbd -i -g /runtime/layout/lanif`
		telnetd -l "/usr/sbin/login" -u Device_Admin:$sign	-i $lf &
	else
		telnetd &
	fi
```

Here we see the security hole: `telnetd -l "/usr/sbin/login" -u Device_Admin:$sign	-i $lf &`
-> login user: `Device_Admin`
-> password: sign=`cat /etc/config/sign`  ==> ....

Then `nc 167.99.82.33 32377` and login using found User/Pass -> `cat flag.txt` => Got the flag
