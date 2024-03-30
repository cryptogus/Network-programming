# Error 해결 방법
패킷을 잡아보면 알겠지만, 현재 TCP에서 파일 정보를 받으면 ok라는 메시지를 보내고 받는다.\
그런데 애초에 TCP는 연결지향 통신이라 ACK 메시지를 받기 전까지는 무차별적으로 데이터를 보내지 않는다.\
즉, 이런 확인 메시지를 보내고 받을 필요가 없다는 것이다.\
지금 코드에서 RST 플래그로 TCP 통신이 끊기지만 `tcp.c` 코드의 server 함수에서 send 부분을, client 함수 부분에서 recv 를 주석 처리 해버리면 정상적인 FIN 플래그가 발생하면서 연결이 종료됨을 알 수 있다.\

물론 이에 대한 해결법은 다양할 것이지만 이는 그 중 하나이고, 이 기능을 살리려면 server쪽 buffer 크기를 늘리는 것도 방법이다.
# 원인 분석
Reset flag는 connection이 reset되야 함을 나타내며, segment가 보기에 현재 connection을 위한게 아닌걸 받았을 때 보내져야 한다. RST 비트가 설정된 segment를 수신하면 즉시 연결을 중단한다. 연결이 중단된 경우, 전송 중인 모든 데이터는 손실된 것으로 간주되며 해당 연결에 할당된 모든 버퍼가 해제된다.

TCP (Transmission Control Protocol)에서 RST (Reset) 패킷이 발생하는 경우는 다양합니다.

여기서는 close 때문인 것 같습니다.

RST가 FIN 이후에 나오는 경우는 SO_LINGER 때문이지만,
RST만 나온 것을 봤을 때 socket rcv buffer에 데이터가 남아 있는데 close()를 호출한 것이 아닐까 생각됩니다.

확인을 위해 close()하기 전에 ioctl(fd, FIONREAD, &ret_nread)를 호출해서
socket rcv buffer에 남아있는 데이터가 있는지 확인해보시기 바랍니다.

해보시고 맞는지 틀렸는지 결과 부탁 드립니다.

참고로 close()대신에 shutdown()을 사용하면 rcv buffer에 데이터가 있어도 RST를 보내지 않습니다.

프로세스를 종료 시키면 시스템이 close를 해주므로 위와 똑같은 결과를 가져옵니다.
이미 닫힌 소켓에 패킷이 수신되므로 시스템이 RST를 발생시키는 것이죠.

명시적으로 소켓을 닫지 않고 프로세스를 종료 시키는 방식은 권장하지 않습니다.

만약 close 때문이 아니라면 오류가 나는 상태가 대부분 recv 버퍼 크기 < send 버퍼 크기인데, 수신측의 소켓 버퍼(windwo size)에서 recv 버퍼로 데이터를 읽어들이고 없애는 크기보다 소켓 버퍼에 send 버퍼 크기 데이터를 받아서 채워지는 크기가 더 많아서 발생하는 듯하다. 즉, 소켓 버퍼가 다 비워지지 않은 상태에서 데이터가 들어와서 그런 것이 아닐까 싶다.

https://yangbongsoo.gitbook.io/study/connection_reset\
https://kldp.org/node/137832\
https://velog.io/@youngerjesus/Connection-Reset-by-Peer-문제-해결
