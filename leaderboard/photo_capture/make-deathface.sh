cd work
(ls -t|head -n 15;ls)|sort|uniq -u|xargs rm
i=0; for file in *.jpeg; do mv "$file" $(printf "image_%0.3d.jpeg" $i); i=$((i+1)); done
ffmpeg -r 10 -i image_%03d.jpeg deathface.mp4
mv deathface.mp4 ..
cd ..
rm work/*