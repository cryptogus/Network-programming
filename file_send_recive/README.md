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
tcp server port: 12340\
udp server port: 12345

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