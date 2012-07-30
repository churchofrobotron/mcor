#!/bin/sh

#USE_SSH="1"

INPUT='../leaderboard/data/leaderboard.txt'
USER='user'

LEADERBOARD_HOST='localhost'
LEADERBOARD_DIR='/tmp'

HISTORY_HOST='localhost'
HISTORY_DIR='/tmp/history'


echo Copying leaderboard.txt to destination
if [ "$USE_SSH" = "1" ] ; then
	rsync -avv $INPUT $USER@$LEADERBOARD_HOST:/$LEADERBOARD_DIR/
else
	rsync -avv $INPUT $LEADERBOARD_DIR/
fi

echo Saving leaderboard.txt for posterity
D=`date +"%Y%m%d"`
if [ "$USE_SSH" = "1" ] ; then
	rsync -avv $INPUT $USER@$LEADERBOARD_HOST:/$LEADERBOARD_DIR/leaderboard_${D}.txt
else
	rsync -avv $INPUT $HISTORY_DIR/leaderboard_${D}.txt
fi
