# TCP, UDP, HTTPS for testing

Ubuntu 22.04 LTS
```bash
$ sudo apt install build-essential cmake libssl-dev libcurl4-openssl-dev
$ curl-config --cflags
$ curl-config --libs
```
## Testing with loopback

```bash
$ cmake -B build -S . && cd build
$ make
$ cd tests
```
tcp server port: 12340
udp server port: 12345
```
build
└──tests
    ├── 30mb_test.txt
    ├── tcp-client-test*
    ├── tcp-server-test*
    ├── test.txt
    ├── udp-client-test*
    └── udp-server-test*
```