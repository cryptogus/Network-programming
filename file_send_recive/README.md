# TCP, UDP, HTTPS for testing

## To-do lists
- udp client -> server 로 파일 올리기
    - client가 while 문에서 계속 보내기만 하고, server 는 받기만 하는 구조
    - 이렇게되면 (꼭 VPN 안타는 경우여도) 네트워크 경로가 좀 많아지면 받는쪽에서 보내는 속도를 못따라가서 다 못받을수가 있음
    - 서버가 받으면 다 받았다는 응답을 client 에 보내고 client가 그 응답을 확인한 후에 다시 보내는 구조를 만들어 볼까싶음 ( client -> server -> client 이게 1사이클)
    - recieve 쪽 버퍼가 더 커야함, 한번에 크게크게 보내야함 (4096 또는 8192 바이트 정도). 이 부분은 실제 큰 파일을 다운로드 해보면서 wireshark 로 찍어보니 보통 한 TCP 패킷당 2700 바이트 정도 잡힘. (말이 TCP지 wireshark에서는 TLS 패킷의 data 크기가 10000바이트라면 2000바이트씩 TCP 조각 데이터를 찍어주고(실제론 암호화된 데이터 2000바이트) 마지막 TCP 바이트를 TLS 패킷이라고 gui로 보여줌. packet len이 10000바이트보다 작을 것. 하지만 잘 보면 wireshark에서 데이터를 모아서 보여줌. 물론 암호화되어 있어서 읽을 순 없음, 글로 쓰는 것 보다 실제 테스트해보길 바란다.) https://ubuntu.com/download/desktop 이 사이트에서 우분투 os를 받아보면서 wireshark로 패킷 찍어보길. wireshark 필터는 ip.addr == <ubuntu.com IP>, <ubuntu.com IP>는 `ping ubuntu.com` 같은 명령어로 쉽게 알아낼 수 있다. \
    **그리고 버퍼 크기가 중요한 이유에 대해 테스트를 통해 알아냈다.** `BUFSIZ` 는 각 임베디드 환경, os등에 따라 달라질 수 있다. 그렇기에 `buffer` 배열의 크기를 `8192`로 고정 후 TCP로 `안드로이드 기기->간이 L2스위치->공유기->라즈베리파이` 보내는파일 보내는 테스트를 했다. 결과는 TCP server를 열었던 라즈베리파이에서 `Connection reset by peer 에러`가 뜬다. client였던 안드로이드는 이미 파일을 다 보내고 종료해버렸다. 이로써 생각한 것은 **server 쪽에서 client가 보낸 패킷을 다 받기 전에 client는 통신을 종료하므로 TCP 프로토콜의 특성상 저런 메시지가 뜰 수 밖에 없다** 였다. 이를 재현한 코드는 `src/tcp-buffer-error`디렉터리에 있다. 루프백으로도 확인 가능하다.(추가 예정)\
    루프백 테스트에서 client 쪽에서 1040 바이트만큼 파일 읽어서 보내고 server 쪽에서는 1024 바이트씩 받도록 했더니 Connection reset by peer 에러가 server 쪽에서 뜸(pc 환경에 따라 바이트 크기를 조절해야 할 수도있다.)\
    하지만 server 쪽 버퍼가 더 큰 경우에는 루프백으로 테스트시 이런 일이 발생하지는 않았다.\
    즉, 어떤 환경에서 코드를 빌드하는지에 따라 버퍼를 무조건 크게 잡는다고 그 버퍼 크기만큼 패킷을 보내지도 받지도 않는다는 점이다. (아마 fread로 읽어오는 크기의 한계가 안드로이드 기기와 라즈베리파이에서 한번에 받을 수 있는 패킷 데이터 크기가 달라서 그럴 수 도 있을 것 같다. 코드상 안드로이드 기기는 데이터를 다 보냈으니 프로그램이 종료된 것이고 서버인 라즈베리파이는 데이터를 다 받기 전에 클라이언트가 통신을 종료해버린 것) `UDP` 에서는 이런 일이 발생하지 않았다.


- recvfrom() 전에는 select() 로 확인하고 받도록 해야 ( https://m.blog.naver.com/whtie5500/221692806173 )
- 서버쪽에서 더 확실하게 다 받으려면, 수신만 하고 버퍼에 저장하는 쓰레드, (버퍼 확인하고) 파일에 쓰는 쓰레드 따로 만들어서 처리하거나 현재는 client가 계속 보내고 있는데, 서버 코드는 받고 바로 바로 파일에 쓰는 중임. 파일에 쓰는게 시간이 좀 걸리는 작업인데, 네트워크가 몰리게 되면 그동안 받는거를 못하니까 파일을 다 못 받고 종료함. (loopback 테스트에서는 큰 문제가 없음)
- `Netcat` 명령어랑 비교해보기, 리눅스는 `nc`, 윈도우는 https://nmap.org/ncat/ 여기가 공식 사이트이고 여러 블로그에 netcat 다운로드 방법이라고 올라와 있는 글들에 마치 공식사이트 처럼 다운로드 받으라고 되어있는 사이트가 있는데 바이러스가 있는 사이트 같다.(바이러스 토탈 사이트에 url도 찍어봄) 어쨋든 nmap에 통합되어 있어서 그냥 다운 받으면 자동으로 설치되고, powershell에서 `ncat` 명령어로 실행 가능하다. 사용법은 `nc`와 같은 것으로 보인다. 이 명령어들로 파일도 다운받아보고 서버도 열어보면서 wireshark로 패킷을 찍어보고 비교해보면 도움이 많이 될 것 같다. 추가적으로 `curl` 명령어도 비교해보면 https, http 등 많은 패킷을 볼 수 있을 것 같다.

On Ubuntu 22.04 LTS, use [libcurl](https://curl.se/libcurl/) for https requests
```bash
$ sudo apt install gcc build-essential cmake make libssl-dev libcurl4-openssl-dev gcc-arm-linux-gnueabihf
$ curl-config --cflags
$ curl-config --libs
```
## Testing with loopback

```bash
$ cmake -B build -S . && cd build
$ make
$ cd tests
```
tcp server port: 12340\
udp server port: 12345

## Build

```bash
$ cmake -B build -S . && cd build
$ make
```
### cross compile for raspberry
Make sure to cross-compile and link libcurl as well. Not implemented in cmake yet
```bash
$ cmake -B build -DCMAKE_TOOLCHAIN_FILE=toolchain_rasppi.cmake -S . && cd build
$ make
```
### Test with qemu
```bash
$ sudo apt install qemu-user-static
$ qemu-arm-static -L /usr/arm-linux-gnueabihf <실행 파일>
```

## Source code explain
tests/ 실행 파일들을 사용해서 wireshark(or tcpdump 등)을 이용하여 packet을 캡처해보면 이해가 더 쉽다.
전체적으로 코드들은 비슷하고 세부적으로 조금씩 다르다.

- `tcp/tcp.c`, `udp/udp.c`: `tcp_client`, `udp_client`는 목적지 ip, port와 보내고자 하는 file 경로(실행파일 기준)를 입력하면 FILE 포인터로 파일을 `fread`로 읽어 buffer 배열(현재 크기 BUFSIZ 매크로만큼)에 담은 후 보내준다. 파일을 다 읽으면 프로그램이 종료된다. `tcp_server` 에는 linux용 multi thread를 적용했다(pthread). `udp_server` 와 `tcp_server`(tcp에서는 정확히는 `handle_client` 함수 부분) 마다 파일을 다 받고 종료하도록 구성하였다. tests/의 코드들에는 받은 정보를 recieve라는 디렉터리를 생성해 그 안에 client에서 보낸 내용을 새로운 파일에 쓰도록 만들도록 하였다. 종료 조건은 tcp와 udp의 특징에 의해 다르게 구성했는데, 주석에서 확인 가능하다.

`tcp.c`
```c
while (1) {
    memset(buffer, 0, sizeof(buffer));
    ssize_t reval = recv(args->client_sock, buffer, sizeof(buffer), 0);
    if (reval < 0) {
        perror("Error receiving data");
        exit(1);
    } else if (reval == 0) { // Transmission complete, TCP의 recv 함수가 0을 반환하는 경우는 일반적으로 송신자가 연결을 닫은 경우입니다.
        // 이것은 TCP의 한 특징으로, 소켓 연결이 양방향 통신에서 닫히면 recv 함수가 0을 반환합니다.
        // 이것은 EOF(End of File) 조건을 나타내며, 클라이언트가 더 이상 데이터를 전송하지 않고 연결을 종료했음을 의미합니다.
        break;
    }
    fwrite(buffer, 1, reval, args->fp);
}

```
`udp.c`
```c
while (1) {
    socklen_t client_len = sizeof(client_addr);
    ssize_t reval = recvfrom(udp_sock, buffer, sizeof(buffer), 0, (struct sockaddr *)&client_addr, &client_len);
    printf("%ld\n", reval);
    if (reval < 0) {
        perror("Error receiving data");
    }
    fwrite(buffer, 1, reval, fp);
    if (reval < sizeof(buffer))
        break; // Transmission complete, UDP 소켓에서는 recvfrom 함수가 0을 반환하는 경우는 없다.
        //  UDP는 연결이 없는 프로토콜로, 개별 패킷 간의 상태를 유지하지 않음, 따라서 recvfrom 함수는 항상 적어도 하나의 바이트를 수신하거나, 에러가 발생할 때 -1을 반환
}      //  0을 반환하는 것은 TCP 소켓에서만 해당되는 상황
```
`tcp.c`에만 있는 `http_request` 함수는 file 경로 대신 http message를 넣어주면 그 응답을 모두 받아온 후 프로그램이 종료되도록 하였다. `tests/http-request-test.c` 에서 사용법 확인 가능하다.
