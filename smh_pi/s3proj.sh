#!/bin/sh

_project_dir="./shm"

# Update all
sudo apt update -y && sudo apt upgrade -y

# Install nodejs, SQL
sudo apt install network-manager  nodejs npm mariadb-server net git gh -y

# create a new access point connection
sudo nmcli c add type wifi ifname "wlan0" con-name "wifihost2" ssid "GG-SMH" wifi.mode "ap" wifi-sec.key-mgmt "wpa-psk" wifi-sec.psk "GravityGardens!" ipv4.method "shared" ipv4.ignore-auto-dns "yes" ipv4.may-fail "no" ipv6.method "disabled" 802-11-wireless.powersave 2

# # Create directory
# if [ -d "$_project_dir" ]; then
# 	sudo rm -R -f -v "$_project_dir"
# fi
# mkdir "$_project_dir"
# cd "$_project_dir"

# skip this step since we are already inside the project folder
# Download git project. This will ask for username and password because it is a private repository
# printf "Generate user key at github -> user settings > developer settings > personal acces tokens > fine grained tokens\n"
# git clone https://github.com/leobeo-png/GravityGardens-SMH.git

# SQL setup (Unattended). This is copied from mysql_secure_installation
sudo mysql -sfu root < db_setup.sql

# Setup the node environment
# cd projectfolder
cd webserver
npm install


