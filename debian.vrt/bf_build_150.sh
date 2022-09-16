#!/bin/sh

curl -s https://packagecloud.io/install/repositories/varnishcache/varnish71/script.deb.sh | sudo bash
sudo apt-get install varnish varnish-dev

