#!/bin/sh
#gst-launch -vt autovideosrc ! jpegenc ! multifilesink location=work/output-%05d.jpeg
gst-launch -vt autovideosrc ! jpegenc ! 'image/jpeg, framerate=(fraction)5/1' ! multifilesink location=output-%05d.jpeg
