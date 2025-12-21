# 🎮 Patch Acquisition Game

[![License: MIT](https://img.shields.io/badge/License-MIT-yellow.svg)](https://opensource.org/licenses/MIT)
[![Language: C](https://img.shields.io/badge/Language-C-blue.svg)](https://en.wikipedia.org/wiki/C_(programming_language))
[![Platform: EV3](https://img.shields.io/badge/Platform-LEGO%20EV3-orange.svg)](https://www.lego.com/en-us/mindstorms)

> 한양대학교 ERICA 스마트센서와액츄에이터 기말 프로젝트
> 최단경로 및 자유경로 패치획득게임 풀이

## 📋 목차

- [프로젝트 소개](#-프로젝트-소개)
- [과제 설명](#-과제-설명)
  - [과제 1: 최단경로 패치획득게임](#과제-1-최단경로-패치획득게임)
  - [과제 2: 자유경로 패치획득게임](#과제-2-자유경로-패치획득게임)
- [프로젝트 구조](#-프로젝트-구조)
- [설치 및 실행](#-설치-및-실행)
- [팀 구성](#-팀-구성)
- [시연 영상](#-시연-영상)
- [제출 파일](#-제출-파일)
- [라이선스](#-라이선스)

## 🎯 프로젝트 소개

본 프로젝트는 LEGO EV3 로봇을 활용하여 격자 구조의 맵에서 패치를 획득하며 최고 점수를 달성하는 것을 목표로 합니다. 두 가지 과제로 구성되어 있으며, 각각 최단경로 알고리즘과 자유경로 최적화 전략을 구현합니다.

### 주요 기능

- ✅ 4x4 및 5x4 격자 구조 맵 탐색
- ✅ 빨간색/파란색 패치 인식 및 위치 파악
- ✅ 최단경로 알고리즘 (Dijkstra/A*)
- ✅ 동적 경로 최적화 (Dynamic Programming)
- ✅ 실시간 점수 계산 및 디스플레이 출력
- ✅ EV3 LCD 화면 인터페이스

## 📖 과제 설명

### 과제 1: 최단경로 패치획득게임

#### 🗺️ 지도 출력
4x4 격자구조에서 **빨간색 패치**와 **파란색 패치**가 임의로 제공된 위치에 있을 때, [출발점]에서 [도착점]까지 모든 격자점을 탐색한 후, [이동점]으로 최단경로로 이동하여 도착 시에 각 패치의 위치를 디스플레이에 출력합니다.

#### 🏁 최단경로 복귀 / 점수 출력
[이동점]에서 출발하여 [출발점]으로 최단경로로 이동하면서 패치를 획득하여 최대한 높은 점수를 얻은 후, [출발점] 도착 시에 획득점수를 디스플레이에 출력합니다.

#### 📊 채점 기준
- 🔴 **빨간색 패치**: +1점
- 🔵 **파란색 패치**: -1점
- 격자판은 실습 시간에 제공
- 조별 획득점수에 따라 고득점순으로 차등점수 부여

---

### 과제 2: 자유경로 패치획득게임

#### 🎮 게임 개요
5x4 격자구조에서 **빨간색 패치**와 **파란색 패치**가 임의로 제공된 위치에 있을 때, [출발점]에서 [도착점]까지 모든 격자점을 탐색한 후, [이동점]에서 출발하여 [출발점]으로 자유경로로 이동하면서 패치를 획득하여 최대한 높은 점수를 얻습니다.

#### 📊 채점 기준
- 🔴 **빨간색 패치**
  - 처음 방문 시: **+5점**
  - 재방문 시: 매번 **-2점**
- 🔵 **파란색 패치**
  - 방문할 때마다 매번 **-5점**
- 🚶 **이동 패널티**
  - 각 격자점을 1칸 이동할 때마다 색상과 무관하게 **-1점** 감점
- 자유경로 이동 시, 위/아래/왼쪽/오른쪽으로 자유롭게 이동 가능
- 조별 획득점수에 따라 고득점순으로 차등점수 부여

## 📁 프로젝트 구조

```
PatchAcquisitionGame/
├── 📄 README.md                    # 프로젝트 설명 문서
├── 📄 .gitignore                   # Git 제외 파일 목록
├── 🌐 index.html                   # 프로젝트 웹페이지
├── 🎨 styles.css                   # 웹페이지 스타일시트
├── ⚡ script.js                    # 웹페이지 인터랙티브 기능
├── 📂 images/                      # 이미지 리소스
│   └── logo.png                    # 한양대학교 로고
├── 📂 ppt/                         # 발표 자료
│   └── presentation.pptx           # 기말 과제 발표 PPT
├── 📂 reference/                   # 참고 자료
│   └── graduate_code.c             # 선배 코드 참고 자료
├── 📂 stage1-shortest-path/        # 과제 1: 최단경로
│   ├── 2_1_1.c                     # 개발 과정 코드
│   ├── solution.c                  # 최종 솔루션 코드
│   └── test.c                      # 테스트 코드
└── 📂 stage2-free-path/            # 과제 2: 자유경로
    ├── 2_1_2.c                     # 개발 과정 코드
    ├── solution.c                  # 최종 솔루션 코드
    └── test.c                      # 테스트 코드
```

## 🚀 설치 및 실행

### 필수 요구사항

- LEGO Mindstorms EV3
- EV3 개발 환경 (RobotC 또는 EV3-G)
- C 컴파일러 (GCC 권장)

### 설치 방법

```bash
# 레포지토리 클론
git clone https://github.com/EV3Team/PatchAcquisitionGame.git

# 프로젝트 디렉토리로 이동
cd PatchAcquisitionGame
```

### 실행 방법

#### 과제 1 실행
```bash
# stage1-shortest-path 디렉토리로 이동
cd stage1-shortest-path

# solution.c 파일을 EV3에 업로드 및 실행
# (EV3 개발 환경에 따라 방법이 다를 수 있습니다)
```

#### 과제 2 실행
```bash
# stage2-free-path 디렉토리로 이동
cd stage2-free-path

# solution.c 파일을 EV3에 업로드 및 실행
```

## 👥 팀 구성

### 2_1조

| 이름 | 역할 | 이메일 |
|------|------|--------|
| **윤태웅** | PM (프로젝트 매니저) | taewoong25@hanyang.ac.kr |
| **박재형** | 메인 개발 | - |
| **이현빈** | 개발 마무리 | - |
| **조민** | 발표 | - |
| **전민석** | 발표자료 제작 | - |

### 소속
**한양대학교 ERICA 스마트융합공학부 스마트ICT융합전공**

## 🎬 시연 영상

- [과제 1 시연 영상](https://youtube.com/shorts/sBryQY9EAaY) - 최단경로 패치획득게임
- [과제 2 시연 영상](https://youtu.be/fHmVtW4JwN4) - 자유경로 패치획득게임
- 기말 과제 발표 영상 (업로드 예정)

## 📦 제출 파일

- ✅ 1단계 과제 영상
- ✅ 2단계 과제 영상
- ✅ 1단계 과제 코드 (`stage1-shortest-path/solution.c`)
- ✅ 2단계 과제 코드 (`stage2-free-path/solution.c`)
- ✅ 기말 과제 발표 영상
- ✅ 기말 과제 PPT(PDF 변환) 자료 (`ppt/presentation.pptx`)
- ✅ 조별 과제 셀프채점표
- ✅ 개인채점표

## 🔧 기술 스택

- **언어**: C
- **플랫폼**: LEGO Mindstorms EV3
- **알고리즘**: Dijkstra, A*, Dynamic Programming
- **웹**: HTML5, CSS3, JavaScript (프로젝트 소개 페이지)

## 📊 알고리즘 설명

### 최단경로 알고리즘 (과제 1)
- BFS (Breadth-First Search)를 활용한 최단경로 탐색
- 격자 기반 경로 탐색 최적화
- 패치 위치 저장 및 점수 계산

### 자유경로 최적화 (과제 2)
- Dynamic Programming을 활용한 최적 경로 계산
- 방문 여부 추적 및 점수 최적화
- 이동 비용을 고려한 경로 선택 알고리즘

## 🌐 프로젝트 웹페이지

프로젝트 소개 웹페이지가 GitHub Pages를 통해 배포되었습니다:

**🔗 [https://ev3team.github.io/PatchAcquisitionGame/](https://ev3team.github.io/PatchAcquisitionGame/)**

로컬에서 확인하려면:

```bash
# 웹페이지 열기 (Windows)
start index.html

# 웹페이지 열기 (Mac)
open index.html

# 웹페이지 열기 (Linux)
xdg-open index.html
```

## 📝 라이선스

이 프로젝트는 MIT 라이선스 하에 배포됩니다. 자세한 내용은 [LICENSE](LICENSE) 파일을 참조하세요.

## 📧 문의

프로젝트에 대한 문의사항은 아래 이메일로 연락주세요:

**윤태웅** - taewoong25@hanyang.ac.kr

---

<div align="center">

**© 2025 Patch Acquisition Game Project. All rights reserved.**

Made with ❤️ by 2_1조

[GitHub Repository](https://github.com/EV3Team/PatchAcquisitionGame) | [Project Page](https://github.com/EV3Team/PatchAcquisitionGame)

</div>
