#!/bin/sh

echo "" > /tmp/rcagent_token_mutex

/opt/rcagent/nas_service &
while true;
do
    /opt/rcagent/cgi_processor
done
