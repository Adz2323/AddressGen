# AddressGen
Generating Sequential Privatekeys &amp; Publickeys using Libbitcoin

im not a coder of any kind Just for my own general learning & playing around

# Compile
apt update && apt upgrade
apt install root-repo
apt install git -y
apt install build-essential -y
apt install libssl-dev
apt install libgcrypt20-dev
apt-get install libgmp-dev


g++ -std=c++11 -o keygen keygen.cpp -I/root/include -L/root/lib -lbitcoin-system -lboost_system -lboost_filesystem -lboost_thread -lboost_program_options -lboost_regex -lboost_log -lsecp256k1 -lgmp -lpthread

Change Libbitcoin libary folder as needed

Run using
./keygen -n 1000
