#!/bin/bash

# stop the service
systemctl stop mapipcd.service

echo "";
# copy systemctl file
sudo cp -v mapipcd.service /etc/systemd/system/
echo "";
ls -l /etc/systemd/system/mapipcd.service

echo "";
# copy config
# sudo cp -v mapipcd.cfg /etc/

echo "";
# copy binary
sudo cp -v ../build-mapipcd-Desktop-Debug/mapipcd /usr/bin/
# sudo cp -v ../build-mapipcd-Desktop-Release/mapipcd /usr/bin/

echo "";
# reload systemctl
sudo systemctl daemon-reload

echo "";
# start the servivce
systemctl start mapipcd.service

echo "";
# check install
systemctl status mapipcd.service

