
# Kyber768 이란?
NIST에서 2022년에 선정한 양자컴퓨터의 공격에도 안전한(안전하다고 여겨지는) 알고리즘이다. 이런 알고리즘을 양자내성알고리즘 줄여서 PQC라고 한다.



그중 Key 교환에 사용되는 알고리즘이고, 풀 네임은 CRYSTALS-KYBER이다. 768은 보안 강도라고 보면 된다. 같은 알고리즘에서 일반적으로 뒤에 오는 숫자가 클수록 보안 강도가 높다. 예를 들면, AES-128보다 AES-256이 Bruce force attack으로 더 뚫기 어렵다.



단점은 키, 암호문의 크기가 좀 크다.  현재 이로 인해 유해사이트 차단에 문제가 생길 수 있다. 



NIST 선정 양자내성암호들은 아래 NIST 사이트에서 확인 가능하다.

https://csrc.nist.gov/projects/post-quantum-cryptography/selected-algorithms-2022

# X25519 이란?
TLS 1.3 에서 Client Hello 패킷을 잡아보면 접할 수 있는 알고리즘이다. Diffie-Hellman이라는 Key 교환 알고리즘이며, 거기에 보안성과 속도적인 측면을 고려하여 Curve25519라는 타원곡선이 적용된 것이다. 이런 종류의 알고리즘을 줄여서 ECDH이라고 하는데 X448 같은 녀석도 있다. 뒤에 숫자들은 타원곡선의 종류일 뿐, 보안강도와는 다른 의미이다.



X25519Kyber768
단순하게 기존에 사용되는 X25519 키 교환 방식과 Kyber768을 섞은 방식의 키교환 방식이다. 왜 이렇게 사용하냐면, 우선 Kyber라는 알고리즘은 아직 오랫동안 사용되지 않았다.



이게 무슨 말이냐면 AES, RSA 등은 그동안 보안 이슈가 있기는 했지만 30년 넘게 현재까지 살아남은 암호화 알고리즘이다. 암호에서는 오랫동안 큰 문제없이 사용되었다는 것은 어느 정도 본인의 가치를 증명했다고 본다. 그 이유는 해당 암호 알고리즘을 공격했을 때 어느정도 공격까지 통과해야 안전하다고 할 수 있는지에 대한 정량적인 평가가 없기 때문이다.



공격에는 무차별 대입공격, 선형/차분 공격, 부채널공격 등 꽤나 많은 게 있는데, 어떤 암호든 시간만 무제한이라면 무차별 대입공격으로도 언젠가는 뚫린다. 물론 공격에 걸리는 시간이 10000년 정도라면 우린 이미 죽어 의미 없겠지만... (그래서 다항 시간 이내에 뚫리는 암호는 안전하지 않다고 함)



결론적으로 기존 방식은 어느 정도 안전하게 사용해 왔고, Kyber는 새로운 알고리즘이라 아직 어떤 위험이 있을지 모르니 혼합해서(하이브리드 해서) 사용하자는 거다.



# TLS Packet

client hello
실제 chrom 브라우저로 구글에 접속했을 때, 패킷을 잡아보니 해당 알고리즘이 떴다. 참고로 패킷 길이를 보면 1816 바이트이다. 이게 이후 설명할 건데, MTU에 의해 문제가 생긴다.



최대 전송 단위(maximum transmission unit, MTU)란 해당 레이어가 전송할 수 있는 최대 프로토콜 데이터 단위의 크기(바이트)이다. 일반적으로 이더넷에서는 MTU가 1500바이트이다.




server hello숭


이건 server hello에서도 사용한다는 키 교환에 사용 중이란 사진이다.



유해사이트 차단 뚫림은 무슨 소리?
우리나라는 방통위에서 유해사이트를 차단하고 있다. warning.or.kr로 리다이렉트 시키고 있다. 보통 이를 뚫기 위해 VPN이나 Proxy , 패킷 split 등의 프로그램을 이용하는 경우가 많은데, Chrom browser에서는 client hello의 패킷 크기가 MTU보다 더 큰 것을 보았을 것이다.



Wireguard에서도 Kyber768을 키 교환에 적용하면, 간신히 MTU 보다 조금 작은 크기로 패킷 교환이 가능해서 기존의 성능을 크게 건드리지 않고 키 교환이 가능하다. 이런 점에서 보았을 때 내 의견은..



고의적으로 ClientHello 패킷을 split 하지 않아도 MTU 때문에 자동으로 split이 된다는 것이다. 이로 인해 차단이 우회가 될 수 있다는 점이다.



내가 알기로 우리나라는 ISP가 어느 정도의 DPI를 할 수 있는 나라로 알고있다. 간단하게 DPI란 패킷을 열어본다는 것이다. 당연히 이게 가능해야 어디로 가는지 알고 유해사이트를 차단하던지 할 수있는 것이다. 개인의 자유등에 대한 논란이 있지만 그건 이 글의 주제는 아니고...



2024.05.12 현재 Firefox, Whale은 ClientHello가 1개의 패킷으로 전송되고 Chrome, Edge는 2개의 패킷으로 전송된다.



이를 막기 위해서는 ClientHello를 ressemble 하는 장비를 도입하던지 해야 한다.



또한 현재 키 교환 관련 호환성 문제도 있는 듯하다. 그 내용은 아래 링크로 걸어두겠다. 패킷도 올려둔다.



참고 링크

https://chromestatus.com/feature/5257822742249472


Chrome Platform Status



chromestatus.com
https://www-bleepingcomputer-com.cdn.ampproject.org/c/s/www.bleepingcomputer.com/news/security/google-chromes-new-post-quantum-cryptography-may-break-tls-connections/amp/


Google Chrome's new post-quantum cryptography may break TLS connections

Some ​Google Chrome users report having issues connecting to websites, servers, and firewalls after Chrome 124 was released last week with the new quantum-resistant X25519Kyber768 encapsulation mechanism enabled by default.

www.bleepingcomputer.com
https://certera.com/blog/google-chrome-116-adds-hybrid-post-quantum-cryptographic-algorithm/


Google Chrome 116 Introduced Hybrid Post-Quantum Cryptographic Algorithm for HTTPS

Google released first quantum-resilient FIDO2 key implementation to ensure safety and security as the advent of quantum computing approaches in Chrome 116.

certera.com


