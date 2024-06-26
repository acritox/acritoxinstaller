#!/bin/bash
# This script automatically configures your Live-USB-Stick to be persistent
# 
# Written by Andreas Loibl <andreas@andreas-loibl.de>
KDIALOG="$(which kdialog)"                      || KDIALOG="/usr/bin/kdialog"
ZENITY="$(which zenity)"                        || ZENITY="/usr/bin/zenity"
YAD="$(which yad)"                        || YAD="/usr/bin/yad"

partition=3
filesystem=ext4

. /etc/default/distro
isodev="$(awk '{if($2=="/live/image"||$2=="/lib/live/mount/medium"||$2=="/run/live/medium"){print $1;}}' /proc/mounts)"
case $isodev in
/dev/sd*) ;;
*) isodev= ;;
esac
case "$( ls /grub.cmdline 2>/dev/null)" in
/grub.cmdline)
if [ -x "$KDIALOG" ]; then
    $KDIALOG --error "Persistent mode seems to be enabled already!" --title "Persistent Live-USB-Stick"
elif [ -x "$ZENITY" ]; then
    $ZENITY --error --text "Persistent mode seems to be enabled already!" --title "Persistent Live-USB-Stick"
elif [ -x "$YAD" ]; then
    $YAD --center --error --text "Persistent mode seems to be enabled already!" --title "Persistent Live-USB-Stick"
fi
exit 1
;;
esac

if [ -d "/run/live/persistence" ]; then
if [ -x "$KDIALOG" ]; then
    $KDIALOG --error "Persistent mode seems to be enabled already!" --title "Persistent Live-USB-Stick"
elif [ -x "$ZENITY" ]; then
    $ZENITY --error --text "Persistent mode seems to be enabled already!" --title "Persistent Live-USB-Stick"
elif [ -x "$YAD" ]; then
    $YAD --center --error --text "Persistent mode seems to be enabled already!" --title "Persistent Live-USB-Stick"
fi
exit 1
fi

if [ "$FLL_DISTRO_MODE" != "live" -o -z "$isodev" ]; then
if [ -x "$KDIALOG" ]; then
    $KDIALOG --error "This script can only be used from a Live USB-Stick!" --title "Persistent Live-USB-Stick"
elif [ -x "$ZENITY" ]; then
        $ZENITY --error --text "This script can only be used from a Live USB-Stick!" --title "Persistent Live-USB-Stick"
elif [ -x "$YAD" ]; then
        $YAD --center --error --text "This script can only be used from a Live USB-Stick!" --title "Persistent Live-USB-Stick"
fi
exit 1
fi

if ((UID)); then
if [ -x "$KDIALOG" ]; then
$KDIALOG --warningcontinuecancel "This script automatically configures your Live-USB-Stick to be persistent:

* it adds a new partition into the unused space of your stick
* formats the partition with $filesystem filesystem
* writes config files to the new partition

===============================================================
CURRENT CHANGES ARE NOT STORED, YOU HAVE TO REBOOT TO ENABLE PERSISTENCE!
===============================================================

Press \"Continue\" when you are ready to start.

" --title "Persistent Live-USB-Stick" || exit 0
elif [ -x "$ZENITY" ]; then

$ZENITY --question --text "This script automatically configures your Live-USB-Stick to be persistent:

* it adds a new partition into the unused space of your stick
* formats the partition with $filesystem filesystem
* writes config files to the new partition

===============================================================
CURRENT CHANGES ARE NOT STORED, YOU HAVE TO REBOOT TO ENABLE PERSISTENCE!
===============================================================

Press \"Continue\" when you are ready to start.

" --title "Persistent Live-USB-Stick" || exit 0
elif [ -x "$YAD" ]; then

$YAD --center --question --text "This script automatically configures your Live-USB-Stick to be persistent:

* it adds a new partition into the unused space of your stick
* formats the partition with $filesystem filesystem
* writes config files to the new partition

===============================================================
CURRENT CHANGES ARE NOT STORED, YOU HAVE TO REBOOT TO ENABLE PERSISTENCE!
===============================================================

Press \"Continue\" when you are ready to start.

" --title "Persistent Live-USB-Stick" || exit 0

fi
cp "$0" "/tmp/$(basename "$0")"
chmod +x "/tmp/$(basename "$0")"
if [ -x "$KDIALOG" ]; then
$KDIALOG --progressbar "Please wait..." --title "Persistent Live-USB-Stick" 0
sudo "/tmp/$(basename "$0")" "$@"
elif [ -x "$ZENITY" ]; then
sudo "/tmp/$(basename "$0")" "$@" | $ZENITY --progress --pulsate --text "Please wait..." --title "Persistent Live-USB-Stick" 0
elif [ -x "$YAD" ]; then
sudo "/tmp/$(basename "$0")" "$@" | $YAD --center --progress --pulsate --text "Please wait..." --title "Persistent Live-USB-Stick" 0
fi

exit $?
fi

exec >/tmp/persistent.log 2>&1
set -x

. /usr/share/acritoxinstaller/modules/partitions
isodisk="$(get_disk $isodev)"
is_removeable "$isodisk" || exit 1

nop() { return 0; }
trap nop TERM KILL HUP

# Debug output
set -x

if [ ! -e "$isodisk"$partition ]; then
	# Add partition
	echo "n|p|$partition|||w" | tr '|' '\n' | fdisk "$isodisk" 
	partprobe "$isodisk"
	while [ ! -e "$isodisk"$partition ]; do sleep 1; done
	mkfs.$filesystem -L "persistence" "$isodisk"$partition
fi

if [ -d /run/live/medium ]; then
    mkdir -p /run/live/persistence
    mount "$isodisk"$partition /run/live/persistence
    set -- $(cat /proc/cmdline)
    shift
    echo "set persistence_cmdline='$@ persistence'" > /run/live/persistence/grub.cmdline
    echo "/ union" > /run/live/persistence/persistence.conf
    umount /run/live/persistence
    rmdir /run/live/persistence

else

if [ -d /lib/live/mount ]; then
    mkdir -p /lib/live/mount/persistence
    mount "$isodisk"$partition /lib/live/mount/persistence
    set -- $(cat /proc/cmdline)
    shift
    echo "set persistence_cmdline='$@ persistence'" > /lib/live/mount/persistence/grub.cmdline
    echo "/ union" > /lib/live/mount/persistence/persistence.conf
    umount /lib/live/mount/persistence
    rmdir /lib/live/mount/persistence
elif [ -d /live ]; then
    mkdir -p /live/persistence
    mount "$isodisk"$partition /live/persistence
    set -- $(cat /proc/cmdline)
    shift
    echo "set persistence_cmdline='$@ persistence'" > /live/persistence/grub.cmdline
    echo "/ union" > /live/persistence/persistence.conf
    umount /live/persistence
    rmdir /live/persistence
fi
fi
#killall -9 kdialog
kill $(ps ax | grep kdialog | grep Live-USB | cut -c2-5)
killall -9 kdialog_progress_helper
exit 0
