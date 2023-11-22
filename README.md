mcor
====

Mobile Church of Robotron


Bootstrap an install (maybe?), do the following in the home directory of the launch user.
```
# Linux (maybe rpi??)
sudo apt-get install build-essential libgl1-mesa-dev x11proto-core-dev libx11-dev python3-pip python3
# rpi only?
sudo apt-get install omxplayer
mkdir cor
cd cor
git clone https://github.com/churchofrobotron/mcor.git
git clone --recurse-submodules https://github.com/churchofrobotron/cor_scoreboard.git
git clone https://github.com/churchofrobotron/leaderboard_data.git
cd mcor
pip3 install -r requirments.txt
cd ..
cd cor_scoreboard
make -C build/projects/gmake-linux config=release
# make -C build/projects/gmake-rpi config=release
cd ..
cp mcor/installs/trailer/scripts/* .
echo Update location_info with install location id and name.
echo Then launch start_* scripts on boot
```