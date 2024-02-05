#!/bin/bash

# Define a function to create virtual ttyUSB nodes
create_virtual_ttyusb() {
	if [ $# -ne 2 ]; then
		echo "Usage: create_virtual_ttyusb <port> <usb0_index>"
		return 1
	fi

	local port="$1"
	local usb0_index="$2"

# Create a virtual ttyUSB node linked to a TCP socket
        socat -d -d pty,link="/dev/ttyUSB$usb0_index",raw,echo=0,ignoreeof tcp-connect:"127.0.0.1":"$port" &

# Wait briefly for socat to create the nodes
	sleep 1

# Get the PTS device linked by socat
	local linked_pts=$(readlink "/dev/ttyUSB$usb0_index")

# Change the permissions of the linked PTS node with sudo
	chmod a+rw "$linked_pts"

# Provide feedback
        echo -e "Virtual ttyUSB$usb0_index node created and linked to TCP port $port.\n"
        echo -e "Permissions of $linked_pts adjusted for read and write access.\n"
}

# Define an array of port numbers and corresponding usb0 indices
ports=(6000 6001 6002 6003 6004 6005 6006 6007 6008)
usb0_indices=(100 101 102 103 104 105 106 107 108)
#ports=(6000)
#usb0_indices=(100)

# Loop through the arrays and create virtual ttyUSB nodes
for i in "${!ports[@]}"; do
	create_virtual_ttyusb "${ports[i]}" "${usb0_indices[i]}"
done

