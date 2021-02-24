#!/bin/sh

curl -s https://packagecloud.io/install/repositories/varnishcache/varnish65/script.deb.sh | sudo bash
sudo apt-get install varnish=6.5.1~focal-1 varnish-dev=6.5.1~focal-1

