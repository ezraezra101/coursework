#!/bin/bash
# Start server
HOSTNAME=ccc1.wpi.edu
PORT=12345

REMOTE_SERVER=www.google.com
REMOTE_PORT=80

echo "Starting server"
./httpserver $PORT &
usleep 0.5
echo "Local round trip time:"
./httpclient -p $HOSTNAME $PORT | head -1
./httpclient -p $HOSTNAME $PORT | head -1
./httpclient -p $HOSTNAME $PORT | head -1
./httpclient -p $HOSTNAME $PORT | head -1
./httpclient -p $HOSTNAME $PORT | head -1
./httpclient -p $HOSTNAME $PORT | head -1
./httpclient -p $HOSTNAME $PORT | head -1
./httpclient -p $HOSTNAME $PORT | head -1
./httpclient -p $HOSTNAME $PORT | head -1
./httpclient -p $HOSTNAME $PORT | head -1
PID=$(jobs -p) # Background job's process id
kill -INT $PID
echo "Remote $REMOTE_SERVER round trip time:"
./httpclient -p $REMOTE_SERVER $REMOTE_PORT | head -1
./httpclient -p $REMOTE_SERVER $REMOTE_PORT | head -1
./httpclient -p $REMOTE_SERVER $REMOTE_PORT | head -1
./httpclient -p $REMOTE_SERVER $REMOTE_PORT | head -1
./httpclient -p $REMOTE_SERVER $REMOTE_PORT | head -1
./httpclient -p $REMOTE_SERVER $REMOTE_PORT | head -1
./httpclient -p $REMOTE_SERVER $REMOTE_PORT | head -1
./httpclient -p $REMOTE_SERVER $REMOTE_PORT | head -1
./httpclient -p $REMOTE_SERVER $REMOTE_PORT | head -1
./httpclient -p $REMOTE_SERVER $REMOTE_PORT | head -1
