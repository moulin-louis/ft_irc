#!/bin/bash

# Check if necessary parameters are provided
if [ "$#" -ne 2 ]; then
  echo "Usage: $0 <port> <password>"
  exit 1
fi

port=$1
password=$2

# Launch the IRC server
gnome-terminal -- bash -c "valgrind --track-fds=yes ./ircserv $port $password; exec bash"

# Wait for the server to start
sleep 1

# Launch two instances of irssi, each in a separate terminal window, connecting to the server
for i in {1..3}; do
  gnome-terminal -- bash -c "irssi -c localhost -p $port -w $password; exec bash"
  sleep 1
done