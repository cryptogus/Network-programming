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
## raw socket
Raw 소켓은 네트워크 프로그래밍에서 일반적인 소켓과는 다르게, 전송 계층(TCP, UDP)에서 직접적으로 작동하는 소켓 유형입니다. 이는 특정 프로토콜의 헤더와 데이터를 직접 다룰 수 있게 해주는데, 이를 통해 사용자가 직접 프로토콜의 세부 사항을 제어하고 네트워크 패킷을 조작할 수 있습니다.

Raw 소켓은 주로 다음과 같은 목적으로 사용됩니다:

1. **패킷 스니핑(Sniffing):** Raw 소켓을 사용하여 네트워크 상에서 전송되는 패킷을 읽고 분석할 수 있습니다. 네트워크 프로토콜의 동작을 디버깅하거나 분석할 때 유용합니다.

2. **패킷 주입(Injection):** Raw 소켓을 사용하여 사용자가 생성한 패킷을 네트워크에 주입할 수 있습니다. 이는 네트워크 트래픽을 모의실험하거나, 특정 상황에서의 네트워크 동작을 시뮬레이션할 때 유용합니다.

3. **네트워크 스택 우회(Bypassing Network Stack):** Raw 소켓은 전송 계층의 기능을 우회하고, 직접 프로토콜의 헤더와 데이터를 다룰 수 있습니다. 이는 특정 네트워크 작업을 수행할 때 유연성을 제공합니다.

Raw 소켓은 일반적으로 특권이 필요한 작업이며, 운영 체제에 따라 그 사용에 대한 제한이 있을 수 있습니다. 또한, 이러한 기능을 사용할 때는 주의가 필요하며, 잘못된 사용은 네트워크에 문제를 일으킬 수 있습니다.
설명 잘 나와 있음: https://mangkyu.tistory.com/16

## udp + tun device를 이용한 vpn (update 예정, wireguard 참고)
패킷 전달 과정 설명 잘 되어있음: https://aws-hyoh.tistory.com/169
### tunneling test
After build, local test  
![image](https://github.com/cryptogus/Network-programming/assets/60291830/6788678a-e339-4052-8ec8-38be0329b4e3)
We only tested clients because this is a WSL2 environment with a single eth0 network device and no gateways or other equipment.  
```bash
$ cd tests
$ ./udp_client_test 192.168.10.3 12340
# another terminal
$ sudo ./tun_client_test
```
![스크린샷 2024-02-02 220244](https://github.com/cryptogus/Network-programming/assets/60291830/6140408b-0796-41f1-9aaa-e7dc1b3548db)

In Wireshark, a packet with the other party's private IP, port 192.168.10.3:12340, as the destination  
The source ip address comes up as the ip assigned to tun0 (it should come up as eth0's ip), which is what happens because we're testing on one machine, and the iptables command in tun_alloc.cpp that changes the source ip is currently commented out.

![스크린샷 2024-02-02 220323](https://github.com/cryptogus/Network-programming/assets/60291830/686faced-4d6c-49e6-b967-08f2810b2b80)

Wireshark capture of a VPN (By tun interface) sending packets through tunneling to the other side's VPN. Note that no encryption is used here for testing purposes.  
For now, for the sake of generality, we've set the public IP of the server to 122.211.166.156 for testing.

https://www.gl-inet.com/products/
If you can, I'd recommend buying two devices, one with LAN and one with WAN ports, and testing them.  
The device probably has a device name of br-lan for the LAN port and eth0 for the WAN port. A tun device is added in between.  
# Raw Socket과 TUN/TAP Device의 공통점과 차이점

### 공통점:

1. **네트워크 스택 우회:**
   - 둘 다 네트워크 스택을 우회하여 네트워크 패킷에 접근할 수 있는 방법을 제공합니다.

2. **패킷 조작:**
   - Raw Socket과 TUN/TAP Device는 패킷을 직접 조작할 수 있는 기능을 제공합니다.

3. **커스텀 프로토콜 지원:**
   - 둘 다 사용자 정의 프로토콜을 구현하고 전송할 수 있는 유연성을 제공합니다.

### 차이점:

1. **레이어:**
   - **Raw Socket:** Raw Socket은 전송 계층 (Transport Layer)에서 동작합니다. 패킷에 대한 전체 프로토콜 스택이 제공되며, 사용자는 패킷의 헤더 및 데이터를 직접 조작할 수 있습니다.
   - **TUN/TAP Device:** TUN/TAP Device는 네트워크 계층 (Network Layer)에서 동작합니다. 이는 주로 IP 패킷에 사용되며, 사용자는 IP 패킷을 읽고 쓸 수 있습니다.

2. **목적:**
   - **Raw Socket:** Raw Socket은 일반적으로 네트워크 패킷을 스니핑, 패킷 주입 또는 커스텀 프로토콜 구현과 같은 특수한 용도로 사용됩니다.
   - **TUN/TAP Device:** TUN/TAP Device는 주로 VPN(Virtual Private Network)과 같은 응용 분야에서 사용되며, 가상 네트워크 인터페이스로 동작하여 커널에 의해 관리됩니다.

3. **데이터 송수신 방식:**
   - **Raw Socket:** 주로 소켓을 통해 데이터를 송수신하며, 소켓 API를 사용하여 통신합니다.
   - **TUN/TAP Device:** 가상 네트워크 인터페이스로 동작하므로, 커널과의 상호 작용을 통해 데이터를 송수신합니다.

4. **헤더 유무:**
   - **Raw Socket:** 패킷을 그대로 송수신하므로, 추가적인 프로토콜 헤더를 직접 처리해야 합니다.
   - **TUN/TAP Device:** 주로 IP 패킷을 다루며, 커널에서 IP 헤더를 처리합니다.

5. **운영체제 의존성:**
   - **Raw Socket:** 운영체제에 따라 Raw Socket의 사용이 다를 수 있습니다.
   - **TUN/TAP Device:** TUN/TAP Device는 리눅스와 같은 특정 운영체제에서 가장 흔하게 사용됩니다.

6. **보안:**
   - **Raw Socket:** Raw Socket은 네트워크 스택을 우회하므로 주의해야 합니다. 무분별한 사용은 보안 문제를 야기할 수 있습니다.
   - **TUN/TAP Device:** TUN/TAP Device는 커널에서 관리되므로, 일반적으로 보다 안전하게 사용할 수 있습니다.

### 요약:

- Raw Socket과 TUN/TAP Device는 둘 다 네트워크 패킷에 접근하여 조작할 수 있는 기능을 제공합니다.
- Raw Socket은 전송 계층에서 동작하며, 네트워크 스택을 완전히 우회할 수 있습니다.
- TUN/TAP Device는 주로 네트워크 계층에서 동작하며, 주로 VPN 등 가상 네트워크 환경에서 사용됩니다.
- Raw Socket은 특수한 용도로 사용되는 반면, TUN/TAP Device는 주로 VPN과 같은 응용 분야에서 활용됩니다.