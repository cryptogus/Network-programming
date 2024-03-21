# TCP, UDP, HTTPS for testing

On Ubuntu 22.04 LTS, use [libcurl](https://curl.se/libcurl/) for https requests
```bash
# libssl-dev, libcurl4-openssl-dev는 설치가 가능한 환경일 때만 설치해서 PC에서 처럼 빌드해주면 된다.
$ sudo apt install gcc build-essential cmake make pkg-config
```
## Build and Use on PC
```bash
$ cmake -B build -S .
```
## pkg-config
`build-curl.sh`에서 쓰는 **pkg-config**와 관련하여.
(pkg-config가 없으면 openssl lib/, include/ 다 직접 지정해줘야 해서 어지간히 귀찮은게 아니다.)

`pkg-config`는 라이브러리 및 헤더 파일의 경로 및 컴파일러 옵션과 같은 빌드 설정 정보를 제공하는 유틸리티입니다. 주로 소프트웨어 패키지를 빌드하거나 의존성을 관리할 때 사용됩니다. `pkg-config`를 사용하면 시스템에서 설치된 라이브러리의 정보를 쉽게 얻을 수 있으며, 이를 활용하여 프로젝트를 빌드하는 것이 편리해집니다.

일반적으로 다음과 같은 명령어로 `pkg-config`를 사용할 수 있습니다:

```bash
pkg-config [옵션] [패키지 이름]
```

여기서 옵션은 `--cflags`와 `--libs` 등이 있으며, 패키지 이름은 해당 패키지의 이름을 나타냅니다.

예를 들어, OpenSSL 라이브러리를 사용하는 경우:

```bash
gcc my_program.c -o my_program $(pkg-config --cflags --libs openssl) # -lss -lcrypto
```

위 명령어는 OpenSSL 라이브러리에 필요한 컴파일러 옵션 및 라이브러리 링크 정보를 `pkg-config`를 통해 가져와서 `gcc` 컴파일 명령어에 추가합니다.

`pkg-config`는 주로 Linux 환경에서 사용되며, 소스 코드에서 컴파일 및 빌드 시에 필요한 라이브러리 정보를 간편하게 얻을 수 있도록 도와줍니다.

`.pc` 파일은 `pkg-config`가 자동으로 찾는 표준 위치에 있어야 합니다. 기본적으로 `pkg-config`는 여러 디렉토리를 검색하여 해당 패키지의 `.pc` 파일을 찾습니다.

주로 `.pc` 파일은 `/usr/lib/pkgconfig/` 또는 `/usr/share/pkgconfig/` 디렉토리에 위치합니다. 그러나 시스템에 따라 이 위치는 다를 수 있습니다.

만약 패키지의 `.pc` 파일이 표준 위치에 없다면, `PKG_CONFIG_PATH` 환경 변수를 사용하여 추가 디렉토리를 지정할 수 있습니다. 예를 들어:

```bash
export PKG_CONFIG_PATH=/custom/path/to/pkgconfig:$PKG_CONFIG_PATH
```

위와 같이 설정하면 `pkg-config`는 추가된 디렉토리에서도 `.pc` 파일을 검색하게 됩니다.

따라서, 보통은 `.pc` 파일이 표준 위치에 있고 패키지 이름을 정확하게 지정하면 `pkg-config`가 알아서 필요한 정보를 찾아주게 됩니다.

보통 configure에서 --prefix로 지정해준 위치의 `lib/`에 `pkgconfig/`디렉터리가 있고 이 안에 해당`.pc` 파일이 있다.
해당 디렉터리는 `make install` 명령어를 실행할 때 나타난다. `make install` 을 하지 않더라도 `.pc` 파일은 어딘가에 존재.
예를 들어, `openssl-3.2.1`에도 `openssl.pc` 파일을 찾아 볼 수있다.
## Cross-compile

arm-linux-gnueabihf를 기본 설정으로 설명을 진행한다. (ARM 아키텍처에서 Linux를 타겟으로 설명 진행)

build-openssl.sh, build-zlib.sh, build-curl.sh, CMakeLists.txt 파일들 또한 arm-linux-gnueabihf를 기본 설정으로 진행하며 [target에 따라 수정이 필요하다.](#build-script-참고-자료)

```bash
$ sudo apt install gcc-arm-linux-gnueabihf
$ # Cross compiler 설치 확인
$ arm-linux-gnueabihf-gcc --version
```

- openssl 설치
    ```bash
    $ bash build-openssl.sh
    ```
- zlib 설치
    ```bash
    $ bash build-zlib.sh
    ```
- libcurl 설치
    ```bash
    $ bash build-curl.sh
    ```
- cmake build
    ```bash
    $ cmake -DCROSS_COMPILE=ON -B build -S . && cd build
    $ make -j$(nproc)
    ```

or you can edit `CROSS_COMPILE` section in `CMakeLists.txt` for cross compiling
```bash
$ cmake -DCROSS_COMPILE=ON -B build -S . && cd build
$ make -j$(nproc)
```
### cross compile for raspberry
Make sure to cross-compile and link libcurl as well. Not implemented in cmake yet

```bash
$ cmake -B build -DCMAKE_TOOLCHAIN_FILE=toolchain_rasppi.cmake -S . && cd build
$ make
```
### cross compile for android
These are the scripts that I used to cross-compile for android, so please take note and modify them to use when cross-compiling.
```bash
$ ./build-android.sh
$ ./build-zlib-android.sh
$ ./build-openssl-android.sh
$ ./build-curl-android.sh
$ cmake -B build -DCROSS_COMPILE_ANDROID=ON -S . && cd build
$ make
```
### Build script 참고 자료
- `build-openssl.sh`

    [configure option](https://github.com/openssl/openssl/blob/master/INSTALL.md#configuration-options)
    [android build](https://github.com/openssl/openssl/blob/master/NOTES-ANDROID.md)

- `build-curl.sh`

    [libcurl](https://curl.se/docs/install.html)

### Test with qemu-[아키텍쳐명]-static [실행파일]
**working 디렉터리가 main 디렉터리 위치라고 가정한다.**
- install qemu
    ```bash
    $ sudo apt install qemu-user-static
    ```

- set shared library path
    ```bash
    $ export LD_LIBRARY_PATH=libopenssl/lib:zlib/lib:libcurl/lib:$LD_LIBRARY_PATH
    ```
- openssl test
    ```bash
    $ qemu-arm-static -L /usr/arm-linux-gnueabihf libopenssl/bin/openssl
    ```
- curl test
    ```bash
    $ qemu-arm-static -L /usr/arm-linux-gnueabihf libcurl/bin/curl --help
    ```
- https request test
    ```bash
    $ qemu-arm-static -L /usr/arm-linux-gnueabihf build/https-request https://i.namu.wiki/i/ZklMtOdY5zUwpy-eDcv5ilPABYkPrcPUxO7pK5zS5OZ6JhurWRRFuNufXs-R7-bFxMW2JuaqtJBgRRworro7clCE0_bd7iP7MJAL3kIpsQeXklPqFUWR6U3v6IfFDPVJHOuMikmBzNXwpXoUC6JhkA.webp
    ```
### 사용법
- tcp server
    ```bash
    $ tcp-server <Port>
    ```
- tcp client
    ```bash
    $ tcp-client <Ip> <Port> <실행 파일 기준 보낼 파일 상대 경로>
    ```
- udp server
    ```bash
    $ udp-server <Port>
    ```
- udp client
    ```bash
    $ udp-client <Ip> <Port> <실행 파일 기준 보낼 파일 상대 경로>
    ```
- https request
    ```bash
    $ https-request <https url>
    ```
# Troubleshooting
내가 가진 라즈베리파이 4에는 64비트 운영체제를 설치해두었다. 그런데 default로 설정된 openssl이 32비트에 맞춰 빌드해서 그런지 실행파일들이 하나같이 다음과 같은 오류를 내 뱉었다.
```
cannot execute: required file not found
```
openssl-3.2.1 에서 `./Configure LIST` 명령어로 종류를 맞춰서 해보는 것 도 좋을 것 같다.
근데 이해가 안되는게 hello world 찍는 c코드를 `arm-linux-gnueabihf-gcc` 로 빌드해서 올렸는데 이것도 같은 오류가 난다.
아 내꺼 라즈베리 파이에 들어가서 `gcc -v` 해보니까 Target: aarch64-linux-gnu 이란다.

### Chat GPT 답변
`aarch64-linux-gnu`와 `arm-linux-gnueabihf`는 각각 ARM 아키텍처의 64비트 (ARM64)와 32비트 (ARM)용 크로스 컴파일 툴체인입니다. 이 두 툴체인은 서로 다른 아키텍처용이기 때문에 호환되지 않습니다. 따라서 `aarch64-linux-gnu-gcc`로 빌드된 실행 파일은 `arm-linux-gnueabihf`를 사용하는 시스템에서 실행되지 않습니다.

만약 라즈베리 파이 4의 경우, 보통 `arm-linux-gnueabihf` 툴체인을 사용하여 빌드합니다. 라즈베리 파이 4는 ARMv8-A 아키텍처를 사용하며, 64비트 운영체제를 지원합니다. 따라서 `aarch64-linux-gnu` 툴체인을 사용할 수도 있습니다. 하지만, 보통은 32비트 운영체제를 사용하는 경우가 많습니다.

빌드할 때 사용한 툴체인과 실행할 시스템의 아키텍처가 일치해야 하므로, 라즈베리 파이 4에 대해서는 `arm-linux-gnueabihf-gcc`로 빌드한 실행 파일을 사용하는 것이 적절합니다. 만약 여전히 실행이 안된다면 라즈베리 파이에서 필요한 라이브러리 및 의존성이 충분히 설치되어 있는지 확인해보세요.