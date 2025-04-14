# KLib2Cpp TCP/IP Data Acquisition Client

This project is a C++-based system that receives sensor data over TCP/IP and visualizes it in real time. The `KLib2Cpp` class handles socket communication and converts the received data into a 2D array (ADC values). The `DrawOFW` class uses openFrameworks to visualize this data.

---

## Project Structure

```
.
├── KLib2Cpp.h / KLib2Cpp.cpp     # TCP client and data processing
├── DrawOFW.h / DrawOFW.cpp       # openFrameworks visualization class
├── ofApp.h / ofApp.cpp           # openFrameworks application entry point
├── main.cpp (console)            # Console-based example
├── main.cpp (openFrameworks)     # openFrameworks-based entry point
```

---

## Features

- TCP/IP socket connection and data reception using `ofxTCPClient`
- Packet validation and parsing based on predefined header and tail
- Conversion to 2D ADC array
- Visualization via console or openFrameworks
- Real-time color visualization with `DrawOFW::draw()`

---

## Requirements

- Visual Studio 2017
- OpenFrameworks 0.1.0 (included in this repository): OpenFrameworks folder should be at `c:\of_v10.0_vs_release\`
  - Download: http://openframeworks.cc/download
- Snowforce 3
  - https://github.com/kitronyx/snowforce3/blob/master/Snowforce3.0_2022.02.17.exe

---

## Quick Start

### Console Mode

![Original](img/KLib2_Cpp_Console_Demo.png)

> **Setup in 3 Easy Steps (Console Mode)**

1. Download or clone source code **klib2-cpp  [Console Project]** in Visual Studio 2017
2. Build the solution and run the console `main.cpp`
3. Observe the printed ADC data in the console

### openFrameworks Mode

![Original](img/KLib2_Cpp_Demo.png)

> **For Visualization (OpenFrameworks Mode)**

1. Download or clone source code **klib2-cpp  [OpenFrameWorks]**
2. Open `KLib2.sln` inside `apps/KLib2/KLib2/` with Visual Studio 2017  
3. Build and run the OpenFrameworks app  

---

## Console Build Instructions

1. Download or clone source code **klib2-cpp  [Console Project]**
2. /Klib2CPP_ConsoleProject\Klib2CPP_Project/KLib2Cpp.h : KLib2 header file
3. /Klib2CPP_ConsoleProject\Klib2CPP_Project/KLib2Cpp.cpp : KLib2 c++ code file

---

## openFrameworks Build Instructions

1. Download or clone source code **klib2-cpp  [OpenFrameWorks]**
2. Open ./of_v0.10.0_vs2017_release/apps/KLib2/KLib2/KLib2.sln
   - /KLib2_Cpp/src/ofApp.cpp : Main event routine OpenFrameworks
   - /KLib2_Cpp/KLib2Cpp.h : KLib2 header file
   - /KLib2_Cpp/KLib2Cpp.cpp : KLib2 c++ code file

---

## Program Flow

- **Console Mode**:
  1. Call `KLib2Cpp::start()` to connect
  2. Call `read()` to receive and parse packet
  3. Call `printadc()` to print data

- **openFrameworks Mode**:
  - `setup()` initializes TCP connection and `DrawOFW`
  - `update()` calls `klib->read()`
  - `draw()` visualizes the data with `DrawOFW::draw(adc)`

---

## DrawOFW Usage

The `DrawOFW` class visualizes each sensor node in green intensity using:

```cpp
void DrawOFW::draw(int ** frame, int scale) {
    ofSetColor(0, frame[i][j] * scale, 0);
    ofDrawRectangle(j*size, i*size, size, size);
}
```

---

## Class API Summary

| Class | Key Methods |
|--------|-------------|
| `KLib2Cpp` | `start()`, `read()`, `stop()`, `getcount()`, `printadc()` |
| `DrawOFW`  | `draw(int** frame, int scale)` |

### KLib2Cpp

| Method | Description |
|--------|-------------|
| `start()` | Start TCP connection |
| `read()` | Receive data and return 2D array |
| `stop()` | Close TCP connection |
| `printadc()` | Print ADC data to console |
| `setserver_ip(std::string)` | Set server IP |
| `setport(int)` | Set port |
| `getport()` | Get current port number |
| `getserver_ip()` | Get current server IP |
| `getcount()` | Get number of received packets |
| `deleteAdc()` | Free internal ADC memory |

### DrawOFW

| Method | Description |
|--------|-------------|
| `draw(int** frame, int scale)` | Render the 2D frame as green rectangles |

---

## How to Display Utility Software with an Icon Like the Real-Time Analyzer
1. In the bin\utility folder, create a subfolder — it can have the same name as the executable or a unique name.
2. Place the executable file and an icon file (.ico) inside the folder.
3. Even if the folder name and executable name are different, the utility will run as long as there is only one executable file in the folder.
4. The Real-Time Analyzer will display the utility using the provided icon.

---

## Notes

- Large initial buffer (`2 << 24`) may require careful memory handling
- On `recv()` failure, the connection will close automatically
- Always call `deleteAdc()` to clean up internal buffer

---

## Contact

For questions or support, please visit:  
**https://www.kitronyx.com/support_request** or contact your project coordinator.


---

# KLib2Cpp TCP/IP 데이터 수집 클라이언트

이 프로젝트는 TCP/IP를 통해 센서 데이터를 수신하고 이를 실시간으로 시각화하는 C++ 기반 시스템입니다. `KLib2Cpp` 클래스는 소켓 통신을 처리하고 수신된 데이터를 2차원 배열(ADC 값)로 변환합니다. `DrawOFW` 클래스는 openFrameworks를 사용하여 데이터를 시각화합니다.

---

## 프로젝트 구조

```
.
├── KLib2Cpp.h / KLib2Cpp.cpp     # TCP 클라이언트 및 데이터 처리
├── DrawOFW.h / DrawOFW.cpp       # openFrameworks 시각화 클래스
├── ofApp.h / ofApp.cpp           # openFrameworks 애플리케이션 진입점
├── main.cpp (콘솔용)             # 콘솔 기반 예제
├── main.cpp (openFrameworks용)   # openFrameworks 실행 진입점
```

---

## 주요 기능

- `ofxTCPClient`를 이용한 TCP/IP 소켓 통신 및 데이터 수신
- 헤더/테일 기반 패킷 유효성 검사 및 파싱
- 수신된 데이터를 2차원 ADC 배열로 변환
- 콘솔 및 openFrameworks 기반 시각화 지원
- `DrawOFW::draw()`을 통한 실시간 색상 시각화

---

## 요구 사항

- Visual Studio 2017
- OpenFrameworks 0.10.0 (이 저장소에 포함됨): OpenFrameworks 폴더는 `c:\of_v10.0_vs_release\`에 있어야 합니다  
  - 다운로드: http://openframeworks.cc/download
- Snowforce 3  
  - https://github.com/kitronyx/snowforce3/blob/master/Snowforce3.0_2022.02.17.exe

---

## 퀵스타트

### 콘솔 모드드

![Original](img/KLib2_Cpp_Console_Demo.png)

> **3단계로 빠르게 시작하기 (콘솔 모드)**

1. **klib2-cpp [Console Project]** 소스 코드 다운로드 또는 클론하고고 Visual Studio 2017로 엽니다
2. 이 저장소를 클론하고 콘솔 프로젝트를 
3. 솔루션을 빌드하고 `main.cpp`를 실행합니다  

### openFrameworks 모드

![Original](img/KLib2_Cpp_Demo.png)

> **시각화 실행 (OpenFrameworks 모드)**

1.  **klib2-cpp [OpenFrameworks]** 소스 코드 다운로드 또는 클론하고고 Visual Studio 2017로 엽니다
2. `apps/KLib2/KLib2/` 경로의 `KLib2.sln` 솔루션 파일을 엽니다  
3. 솔루션을 빌드하고 실행합니다

---

### 콘솔 빌드 방법

1. 소스 코드 다운로드 또는 클론: **klib2-cpp [Console Project]**
2. `/Klib2CPP_ConsoleProject/Klib2CPP_Project/KLib2Cpp.h`: 헤더 파일
3. `/Klib2CPP_ConsoleProject/Klib2CPP_Project/KLib2Cpp.cpp`: 구현 파일

### openFrameworks 빌드 방법

1. 소스 코드 다운로드 또는 클론: **klib2-cpp [OpenFrameworks]**
2. `./of_v0.10.0_vs2017_release/apps/KLib2/KLib2/KLib2.sln` 열기
   - `/KLib2_Cpp/src/ofApp.cpp`: OpenFrameworks 메인 루프
   - `/KLib2_Cpp/KLib2Cpp.h`: 헤더 파일
   - `/KLib2_Cpp/KLib2Cpp.cpp`: 구현 파일

---

## 프로그램 흐름

- **콘솔 모드**:
  1. `KLib2Cpp::start()` 호출하여 연결
  2. `read()` 호출하여 데이터 수신 및 파싱
  3. `printadc()` 호출하여 데이터 출력

- **openFrameworks 모드**:
  - `setup()`에서 TCP 연결 및 `DrawOFW` 초기화
  - `update()`에서 `klib->read()` 호출
  - `draw()`에서 `DrawOFW::draw(adc)` 호출하여 데이터 시각화

---

## DrawOFW 클래스 사용 예

`DrawOFW` 클래스는 각 센서 노드를 녹색 강도로 시각화합니다:

```cpp
void DrawOFW::draw(int ** frame, int scale) {
    ofSetColor(0, frame[i][j] * scale, 0);
    ofDrawRectangle(j*size, i*size, size, size);
}
```

---

## 클래스 API 요약

| 클래스 | 주요 메서드 |
|--------|--------------|
| `KLib2Cpp` | `start()`, `read()`, `stop()`, `getcount()`, `printadc()` |
| `DrawOFW`  | `draw(int** frame, int scale)` |

### KLib2Cpp

| 함수명 | 설명 |
|--------|------|
| `start()` | TCP 연결 시작 |
| `read()` | 데이터를 수신하고 2D 배열로 반환 |
| `stop()` | TCP 연결 종료 |
| `printadc()` | 콘솔에 ADC 데이터 출력 |
| `setserver_ip(std::string)` | 서버 IP 설정 |
| `setport(int)` | 포트 설정 |
| `getport()` | 현재 포트 반환 |
| `getserver_ip()` | 현재 IP 반환 |
| `getcount()` | 수신된 패킷 수 반환 |
| `deleteAdc()` | 내부 ADC 메모리 해제 |

### DrawOFW

| 함수명 | 설명 |
|--------|------|
| `draw(int** frame, int scale)` | 2D 프레임을 녹색 정사각형으로 시각화 |

---

## 유틸리티 소프트웨어를 실시간 분석기처럼 아이콘으로 표시하는 방법

1. `bin\utility` 폴더 내에 하위 폴더를 생성합니다. 폴더 이름은 실행 파일과 같거나 달라도 됩니다.
2. 해당 폴더에 실행 파일과 아이콘(.ico) 파일을 넣습니다.
3. 폴더 안에 실행 파일이 하나만 있으면 실행됩니다.
4. 실시간 분석기는 해당 아이콘을 사용해 유틸리티를 표시합니다.

---

## 참고 사항

- 초기 버퍼 크기가 큼(`2 << 24`): 메모리 관리 유의
- `recv()` 실패 시 자동으로 연결 종료
- `deleteAdc()` 호출로 메모리 정리 필요

---

## 문의

기술 지원 또는 문의는  
**https://www.kitronyx.co.kr/support_request** 를 방문하거나 담당자에게 문의해 주세요.
