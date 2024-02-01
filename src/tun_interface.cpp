#include "tun_interface.h"
/*
 * For a real-world VPN, traffic inside UDP tunnel is encrypted
 * A comprehensive encryption is not easy and not the point for this demo
 * I'll just leave the stubs here
 */
// 패딩을 추가하는 함수
void addPadding(char *data, int len) {
    int paddingSize = 16 - (len % 16);
    memset(data + len, paddingSize, paddingSize);
}

// 패딩을 제거하는 함수
int removePadding(char *data, int len) {
    if (len == 0) {
        return 0;
    }

    int paddingSize = data[len - 1];
    if (paddingSize >= len) {
        // Invalid padding size, do not remove anything
        return len;
    }

    return len - paddingSize;
}


// 암호화 함수
void encrypt2(char *plaintext, char *ciphertext, int len) {
    unsigned int Key[8] = {0x3c2d1e0f, 0x78695a4b, 0xb4a59687, 0xf0e1d2c3, 0xc3d2e1f0, 0x8796a5b4, 0x4b5a6978, 0x0f1e2d3c};
    int paddedLen = len + 16 - (len % 16); // 패딩을 추가한 후의 길이
    addPadding(plaintext, len);

    for (int i = 0; i < paddedLen / 16; i++) {
        LEA256_ENC((unsigned int *)ciphertext + (i * 4), (unsigned int *)plaintext + (i * 4), Key);
    }
}

// 복호화 함수
void decrypt2(char *ciphertext, char *plaintext, int len) {
  
    unsigned int Key[8] = {0x3c2d1e0f, 0x78695a4b, 0xb4a59687, 0xf0e1d2c3, 0xc3d2e1f0, 0x8796a5b4, 0x4b5a6978, 0x0f1e2d3c};
    //int paddedLen = ((len + 15) / 16) * 16; // 패딩을 추가한 후의 길이

    for (int i = 0; i < len/16; i++) {
        LEA256_DEC((unsigned int *)plaintext + i * 4, (unsigned int *)ciphertext + i * 4, Key);
    }

    len = removePadding(plaintext, len);
    plaintext[len] = '\0'; // 복호화 후 패딩 제거
}

void Tun_Interface::flags(void) {
    int flags;
    flags = fcntl(sock_, F_GETFL, 0);
        if (flags != -1) {
            if (-1 != fcntl(sock_, F_SETFL, flags | O_NONBLOCK)) {
                this->udp_fd = sock_;
                return;
            }
    }
    perror("fcntl error");

    close(sock_);
}

/*
* tun_buf - memory buffer read from/write to tun dev - is always plain
* udp_buf - memory buffer read from/write to udp fd - is always encrypted
*/
int Tun_Interface::run(void) {

  if ((tun_fd = tun_alloc()) < 0) {
    return 1;
  }

  ifconfig();
  setup_route_table();
  cleanup_when_sig_exit();

  this->flags();
  if (udp_fd == -1) {
    perror("udp_fd socket: ");
    // 에러 처리 로직 추가
  }
  
  char tun_buf[MTU], udp_buf[MTU];
  bzero(tun_buf, MTU);
  bzero(udp_buf, MTU);

  while (1) {
    fd_set readset;
    FD_ZERO(&readset);
    FD_SET(tun_fd, &readset);
    FD_SET(udp_fd, &readset);
    int max_fd = std::max(tun_fd, udp_fd) + 1;

    if (-1 == select(max_fd, &readset, NULL, NULL, NULL)) {
      perror("select error");
      break;
    }

    int r;
    if (FD_ISSET(tun_fd, &readset)) {
      r = read(tun_fd, tun_buf, MTU);
      if (r < 0) {
        // TODO: ignore some errno
        perror("read from tun_fd error");
        break;
      }

      encrypt2(tun_buf, udp_buf, r);
      printf("Writing to UDP %d bytes ...\n", r);
#ifdef AS_CLIENT
    r = sendto(udp_fd, udp_buf, r, 0, (const struct sockaddr *)&server_addr, server_addrlen);
#else
    r = sendto(udp_fd, udp_buf, r, 0, (const struct sockaddr *)&client_addr, client_addrlen);
#endif
      
      if (r < 0) {
        // TODO: ignore some errno
        perror("sendto udp_fd error");
        break;
      }
    }

    if (FD_ISSET(udp_fd, &readset)) {
#ifdef AS_CLIENT
      r = recvfrom(udp_fd, udp_buf, MTU, 0, (struct sockaddr *)&server_addr, &server_addrlen);
#else
      r = recvfrom(udp_fd, udp_buf, MTU, 0, (struct sockaddr *)&client_addr, &client_addrlen);
#endif
      if (r < 0) {
        // TODO: ignore some errno
        perror("recvfrom udp_fd error");
        break;
      }

      decrypt2(udp_buf, tun_buf, r);
      printf("Writing to tun %d bytes ...\n", r);

      r = write(tun_fd, tun_buf, r);
      if (r < 0) {
        // TODO: ignore some errno
        perror("write tun_fd error");
        break;
      }
    }
  }

  close(tun_fd);
  close(udp_fd);

  cleanup_route_table();

  return 0;
}