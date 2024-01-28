# security-networking
```
CPU:
  Info: quad core model: 11th Gen Intel Core i7-1165G7 bits: 64 type: MT MCP
    cache: L2: 5 MiB
  Speed (MHz): avg: 2803 min/max: N/A cores: 1: 2803 2: 2803 3: 2803
    4: 2803 5: 2803 6: 2803 7: 2803 8: 2803
Kernel: 5.15.133.1-microsoft-standard-WSL2 x86_64 Up: 0m
```
<!--
tls, ipsec, vpn for windows and linux  
https://blog.naver.com/love_tolty/222650880413


서버가 하나의 외부 아이피와 포트를 가지고 있고, 내부에서는 여러 개의 내부 아이피 중 하나를 사용하고 있다는 상황에서, 클라이언트는 외부 아이피와 포트를 이용하여 서버에 접속 가능. 이를 위해서는 몇 가지 고려해야 할 사항

1. **포트 포워딩:** 서버의 라우터에서 특정 포트로 들어오는 외부 연결을 내부 서버로 포워딩하는 설정이 필요합니다. 이렇게 하면 외부에서는 서버의 외부 아이피와 포트를 통해 내부 서버에 연결할 수 있습니다.

2. **내부 아이피 선택:** 여러 개의 내부 아이피 중 하나를 선택해야 합니다. 일반적으로 서버 측에서는 내부 네트워크에서 사용 중인 IP 주소 중 하나를 선택하고, 클라이언트에게 해당 내부 아이피를 사용하도록 알려줍니다.

nat 같은걸로 물리거나  
application gateway 통해서 포워딩하는거지  

reference  
https://namu.wiki/w/NAT  
https://kibbomi.tistory.com/219  
-->
## Build
``` bash
$ sudo apt update && sudo apt upgrade
$ sudo apt install g++ build-essential cmake ninja-build libssl-dev
```
```bash
$ cmake -B build -S . -GNinja
$ cd build && ninja
```
### Check ESTABLISHED(connected port) or LISTENING(connectivity standby port) Port Commands
```bash
$ netstat -a -n --tcp
```

## udp를 이용한 vpn (update 예정, wireguard 참고)
패킷 전달 과정 설명 잘 되어있음: https://aws-hyoh.tistory.com/169
