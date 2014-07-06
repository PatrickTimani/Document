#! /bin/bash
IPBASE="141.47.45"
UPFILE=vslabd
MODE=755
UPDIR="/ramuser"
USERNAME=labor
PASSWORD=labor
TMPFILE=ftpcmds

if [ $# -eq 0 ]; then
	echo
	echo "Parameter missing."
	echo
	echo "Usage: up.cmd nnn"
	echo
	echo "	nnn is the rightmost octet of the board's IP address"
	echo "	vslabd will be uploaded to $IPBASE.nnn"
	echo
	exit
fi
echo "$UPFILE will be uploaded to $IPBASE.$1"
echo "open $IPBASE.$1" > $TMPFILE
echo "user $USERNAME $PASSWORD" >> $TMPFILE
echo "cd $UPDIR" >> $TMPFILE
echo "put $UPFILE" >> $TMPFILE
echo "chmod $MODE $UPFILE" >> $TMPFILE
echo "bye" >> $TMPFILE
cat $TMPFILE | ftp -npiv
rm $TMPFILE
