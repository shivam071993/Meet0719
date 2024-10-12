#!/bin/bash

# Check if the user provided an interface and filename
if [ "$#" -ne 2 ]; then
    echo "Usage: $0 <interface> <output_file>"
    exit 1
fi

INTERFACE=$1
OUTPUT_FILE=$2

# Capture UDP packets
echo "Capturing UDP packets on interface $INTERFACE..."
sudo tcpdump -i "$INTERFACE" udp -w "$OUTPUT_FILE"

# Inform the user that the capture has started
echo "UDP packet capture started. Press Ctrl+C to stop."

# After stopping the capture, inform the user
trap "echo 'UDP packet capture stopped.'; exit" SIGINT
wait