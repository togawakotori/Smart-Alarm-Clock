hostName=parallels@192.168.1.101
filePath=/media/psf/pi/memoPad.txt
storePath=/home/pi/Documents
rsync -vazu $hostName:$filePath $storePath
