#pragma config(Sensor, S1, c1, sensorEV3_Color)
#pragma config(Sensor, S2, c2, sensorEV3_Color)
#pragma config(Sensor, S3, c3, sensorEV3_Color)
#pragma config(Motor, motorB, lm, tmotorEV3_Large, PIDControl, driveLeft, encoder)
#pragma config(Motor, motorC, rm, tmotorEV3_Large, PIDControl, driveRight, encoder)

// 전역 변수 선언
int limitRow = 4;        // 격자판의 행 개수
int limitColumn = 5;     // 격자판의 열 개수 (5x4 격자판)
int nMotorSpeedSetting = 15;  // 기본 모터 속도
int vertex = 0;          // 교차점 감지 카운터
int count = 0;           // 현재 행에서 이동한 열 개수
int row = 0;             // 현재 탐색 중인 행 번호
int val, r = 1, c = 1;
int S[6][6];             // 격자판 패치 정보 저장 배열 (1:빨강(+5), -1:파랑(-5), 0:일반)
int dt[6][6];            // DP 테이블 (최대 점수 저장)

// DFS 관련 전역 변수
int visitedSim[6][6];    // 시뮬레이션용 방문 횟수 배열
int bestPath[30];        // 최적 경로를 저장하는 배열 (방향 저장)
int tempPath[30];        // 현재 탐색 중인 경로 (임시 저장)
int maxScore = -10000;   // 최대 점수 (초기값: 매우 작은 값)
int bestStepCount = 0;   // 최적 경로의 이동 횟수
const int MAX_DEPTH = 10; // DFS 최대 탐색 깊이 제한 (무한 루프 방지)

// 라인 트레이싱 함수: 노란색 선을 따라 주행
void lineTracing() {
    val = 3.2;
    
    // c2 센서가 노란색을 감지하면 좌회전 (4: yellow)
    if(getColorName(c2) == 4) {
        setMotorSpeed(lm, nMotorSpeedSetting - val);
        setMotorSpeed(rm, nMotorSpeedSetting + val);
    }
    // 노란색이 아니면 우회전
    else {
        setMotorSpeed(lm, nMotorSpeedSetting + val);
        setMotorSpeed(rm, nMotorSpeedSetting - val);
    }
}

// 전진하며 패치 탐색 및 기록 함수
void go() {
    // 빨간색 패치 감지 시 배열에 +1 기록 (5: red)
    if(getColorName(c2) == 5) {
        if(row % 2 == 0) S[row][count] = 1;  // 짝수 행: 왼쪽→오른쪽
        else S[row][(limitColumn - 1) - count] = 1;  // 홀수 행: 오른쪽→왼쪽
        playTone(440, 20);
        sleep(100);
    }
    
    // 초록색 패치 감지 시 배열에 -1 기록 (3: blue/green)
    if(getColorName(c2) == 3) {
        if(row % 2 == 0) S[row][count] = -1;
        else S[row][(limitColumn - 1) - count] = -1;
        playTone(660, 20);
        sleep(50);
    }
    
    lineTracing();
    
    // c3 센서로 교차점 감지
    if((getColorName(c3) >= 3 && getColorName(c3) <= 5 && row % 2 == 0) || 
       (getColorName(c3) >= 3 && getColorName(c3) <= 5 && row % 2 == 1)) {
        vertex++;
    }
    else {
        vertex = 0;
    }
    
    // 교차점 통과 시 count 증가
    if(vertex == 1) {
        count++;
    }
}

// 좌회전 함수: 노란색 선을 찾을 때까지 회전
void turnLeft() {
    setMotorSpeed(lm, -nMotorSpeedSetting * 4/10);
    setMotorSpeed(rm, nMotorSpeedSetting * 4/10);
    
    // c1, c2 센서가 모두 라인을 벗어날 때까지 대기
    while(getColorName(c1) <= 4 || getColorName(c2) <= 4) {
        sleep(2);
    }
    
    // c1 센서가 노란색 선을 감지할 때까지 회전
    while(getColorName(c1) != 4) {
        sleep(2);
    }
    
    // c2 센서가 노란색 선을 감지할 때까지 회전
    while(getColorName(c2) != 4) {
        sleep(2);
    }
    
    sleep(550);
    setMotorSpeed(lm, 0);
    setMotorSpeed(rm, 0);
    sleep(200);
}

// 우회전 함수: 노란색 선을 찾을 때까지 회전
void turnRight() {
    setMotorSpeed(lm, nMotorSpeedSetting * 4/10);
    setMotorSpeed(rm, -nMotorSpeedSetting * 4/10);
    
    // c3, c2 센서가 모두 라인을 벗어날 때까지 대기
    while(getColorName(c3) <= 4 || getColorName(c2) <= 4) {
        sleep(2);
    }
    
    // c3 센서가 라인 밖에 있을 때까지 회전
    while(getColorName(c3) > 4) {
        sleep(2);
    }
    
    // c2 센서가 라인 밖에 있을 때까지 회전
    while(getColorName(c2) > 4) {
        sleep(2);
    }
    
    setMotorSpeed(lm, 0);
    setMotorSpeed(rm, 0);
    sleep(200);
}

// 모터 정지 함수
void stopMotor() {
    setMotorSpeed(lm, 0);
    setMotorSpeed(rm, 0);
}

// 4x5 격자판 완전 탐색 함수 (행 우선 탐색)
void completeSearch() {
    while(true) {
        go();
        
        // 현재 행의 마지막 열에 도착한 경우
        if(count == limitColumn - 1) {
            // 마지막 행까지 탐색 완료 시 종료
            if(row == limitRow - 1) return;
            
            // 짝수 행일 때: 오른쪽 끝에서 다음 행으로 이동
            if(row % 2 == 0) {
                // 마지막 교차점에서 빨간색 패치 추가 감지
                for(int i = 0; i < 4; i++) {
                    if(getColorName(c2) == 5) {
                        if(row % 2 == 0) S[row][count] = 1;
                        else S[row][(limitColumn - 1) - count] = 1;
                        playTone(440, 20);
                        sleep(80);
                    }
                    else sleep(90);
                }
                
                setMotorSpeed(lm, nMotorSpeedSetting);
                setMotorSpeed(rm, nMotorSpeedSetting);
                sleep(600);
                turnRight();
            }
            // 홀수 행일 때: 왼쪽 끝에서 다음 행으로 이동
            else {
                for(int i = 0; i < 3; i++) {
                    if(getColorName(c2) == 5) {
                        if(row % 2 == 0) S[row][count] = 1;
                        else S[row][(limitColumn - 1) - count] = 1;
                        playTone(440, 20);
                        sleep(80);
                    }
                    else sleep(90);
                }
                
                setMotorSpeed(lm, nMotorSpeedSetting);
                setMotorSpeed(rm, nMotorSpeedSetting);
                sleep(900);
                turnLeft();
            }
            
            // 다음 행의 첫 교차점까지 이동
            if(row % 2 == 0) {
                while(getColorName(c3) != 4) go();
                setMotorSpeed(lm, nMotorSpeedSetting);
                setMotorSpeed(rm, nMotorSpeedSetting);
                sleep(900);
                turnRight();
            }
            else {
                while(getColorName(c1) != 4) go();
                setMotorSpeed(lm, nMotorSpeedSetting);
                setMotorSpeed(rm, nMotorSpeedSetting);
                sleep(1200);
                turnLeft();
            }
            
            row++;
            count = 0;
            
            // 다음 행 시작 위치로 후진
            setMotorSpeed(lm, -nMotorSpeedSetting);
            setMotorSpeed(rm, -nMotorSpeedSetting);
            if(row % 2 == 0) {
                sleep(800);
            }
            else {
                sleep(1250);
            }
            
            setMotorSpeed(lm, 0);
            setMotorSpeed(rm, 0);
            sleep(600);
            
            // 다음 행 탐색 시작
            setMotorSpeed(lm, nMotorSpeedSetting);
            setMotorSpeed(rm, nMotorSpeedSetting);
            sleep(450);
        }
    }
}

// 최댓값 반환 함수 (DP 계산에 사용)
int max(int a, int b) {
    return a > b ? a : b;
}

// DFS 함수: 자유 경로 탐색으로 최대 점수 경로 찾기
// x, y: 현재 위치, currentScore: 현재까지 누적 점수, steps: 이동 횟수
void dfs(int x, int y, int currentScore, int steps) {
    // 1. 기저 조건: 출발점(0,0)에 도착한 경우
    if(x == 0 && y == 0) {
        // 현재 경로의 점수가 최대 점수보다 높으면 갱신
        if(currentScore > maxScore) {
            maxScore = currentScore;
            bestStepCount = steps;
            // 최적 경로 저장
            for(int i = 0; i < steps; i++) {
                bestPath[i] = tempPath[i];
            }
        }
        return;
    }
    
    // 2. 가지치기: 탐색 깊이 제한 (너무 긴 경로 방지)
    if(steps >= MAX_DEPTH) return;
    
    // 3. 4방향 탐색 (상, 하, 좌, 우)
    int dx[] = {-1, 0, 1, 0};  // 상(-1), 하(1), 좌(0), 우(0)
    int dy[] = {0, -1, 0, 1};  // 상(0), 하(0), 좌(-1), 우(1)
    
    for(int i = 0; i < 4; i++) {
        int nx = x + dx[i];
        int ny = y + dy[i];
        
        // 격자판 범위 체크
        if(nx < 0 || nx >= limitRow || ny < 0 || ny >= limitColumn) continue;
        
        // 이동 비용 계산
        int moveCost = -1;  // 각 칸 이동 시 -1점
        int patchScore = 0;
        
        // 패치 점수 계산
        if(S[nx][ny] == 1) {  // 빨간색 패치
            if(visitedSim[nx][ny] == 0) {
                patchScore = 5;   // 첫 방문: +5점
            }
            else {
                patchScore = -2;  // 재방문: -2점
            }
        }
        else if(S[nx][ny] == -1) {  // 파란색 패치
            patchScore = -5;  // 방문 시마다 -5점
        }
        
        // 중복 방문 제한: 같은 칸을 2번 초과하여 방문하지 않음
        if(visitedSim[nx][ny] >= 2) continue;
        
        // 다음 위치로 이동 (백트래킹)
        visitedSim[nx][ny]++;
        tempPath[steps] = i;  // 현재 방향 저장 (0:상, 1:좌, 2:하, 3:우)
        displayBigTextLine(4, "curr_step = %d", steps);
        
        // 재귀 호출로 다음 위치 탐색
        dfs(nx, ny, currentScore + moveCost + patchScore, steps + 1);
        
        // 백트래킹: 방문 취소
        visitedSim[nx][ny]--;
    }
}

// 최적 경로 실행 함수: DFS로 찾은 경로를 실제로 로봇이 이동
void executePath() {
    // 로봇은 현재 (limitRow-1, limitColumn-1) 위치에서 시작
    // 목표: 출발점 (0,0)까지 최적 경로로 이동
    
    for(int i = 0; i < bestStepCount; i++) {
        int dir = bestPath[i];  // 0:상, 1:좌, 2:하, 3:우
        
        // 현재 로봇의 방향을 고려하여 회전 및 이동
        
        if(dir == 1) {  // 왼쪽으로 이동
            c--;  // 열 감소
        }
        else if(dir == 0) {  // 위로 이동 - 우회전 필요
            turnRight();
            sleep(200);
            r--;  // 행 감소
        }
        else if(dir == 2) {  // 아래로 이동 - 좌회전 필요
            turnLeft();
            sleep(200);
            r++;  // 행 증가
        }
        else if(dir == 3) {  // 오른쪽으로 이동 - 180도 회전
            setMotorSpeed(lm, nMotorSpeedSetting);
            setMotorSpeed(rm, -nMotorSpeedSetting);
            sleep(1400);  // 180도 회전 시간
            stopMotor();
            sleep(200);
            c++;  // 열 증가
        }
        
        // 다음 교차점까지 이동
        count = 0;
        while(true) {
            go();  // 라인 트레이싱하며 전진
            
            if(count == 1) {  // 교차점 도착
                // 최종 목적지인 출발점 도착 시
                if(r == 0 && c == 0 && i == bestStepCount - 1) {
                    stopMotor();
                    break;
                }
                
                // 교차점을 조금 더 지나가서 정확한 위치 조정
                setMotorSpeed(lm, nMotorSpeedSetting);
                setMotorSpeed(rm, nMotorSpeedSetting);
                sleep(1100);
                break;
            }
        }
        
        // 이동 후 원래 방향으로 복귀 (다음 이동을 위한 정렬)
        if(dir == 0) {  // 위로 이동했으면 좌회전으로 원래 방향
            turnLeft();
            sleep(100);
        }
        else if(dir == 2) {  // 아래로 이동했으면 우회전으로 원래 방향
            turnRight();
            sleep(100);
        }
        else if(dir == 3) {  // 오른쪽으로 이동했으면 180도 회전
            setMotorSpeed(lm, nMotorSpeedSetting);
            setMotorSpeed(rm, -nMotorSpeedSetting);
            sleep(1400);
            stopMotor();
        }
        // dir == 1 (왼쪽)인 경우는 방향 조정 불필요
        
        // 현재 상태 디스플레이 출력
        eraseDisplay();
        displayBigTextLine(1, "Step: %d/%d", i + 1, bestStepCount);
        displayBigTextLine(4, "Pos: %d, %d", r, c);
    }
}

task main() {
    // 버튼 누를 때까지 대기
    while(getButtonPress(1) == 0) sleep(10);
    
    // 1단계: 4x5 격자판 완전 탐색
    completeSearch();
    
    // 2단계: 이동점으로 이동 준비 (180도 회전)
    setMotorSpeed(lm, nMotorSpeedSetting);
    setMotorSpeed(rm, nMotorSpeedSetting);
    sleep(1100);
    
    setMotorSpeed(lm, nMotorSpeedSetting);
    setMotorSpeed(rm, -nMotorSpeedSetting);
    sleep(2300);
    
    setMotorSpeed(lm, 0);
    setMotorSpeed(rm, 0);
    sleep(500);
    
    // 3단계: (3,0) → (3,4) 이동점까지 이동
    for(int i = 0; i < limitColumn - 1; i++) {
        while(getColorName(c3) > 4) {
            if((getColorName(c3) >= 3 && getColorName(c3) <= 5)) break;
            lineTracing();
        }
        
        setMotorSpeed(lm, nMotorSpeedSetting);
        setMotorSpeed(rm, nMotorSpeedSetting);
        sleep(400);
    }
    
    setMotorSpeed(lm, 0);
    setMotorSpeed(rm, 0);
    sleep(500);
    
    setMotorSpeed(lm, nMotorSpeedSetting);
    setMotorSpeed(rm, nMotorSpeedSetting);
    sleep(1000);
    
    // 4단계: (3,4) 이동점에서 좌회전하여 출발점 방향으로 정렬
    setMotorSpeed(lm, -nMotorSpeedSetting);
    setMotorSpeed(rm, nMotorSpeedSetting);
    sleep(1300);
    
    while(getColorName(c2) > 4 && getColorName(c3) > 4) {
        sleep(2);
    }
    
    sleep(750);
    stopMotor();
    sleep(1000);
    
    count = row = 0;
    
    // 5단계: DP 알고리즘으로 최대 점수 계산 (참고용)
    for(int i = 0; i < limitRow; i++) {
        for(int j = 0; j < limitColumn; j++) {
            if(i == 0 && j == 0) {
                dt[i][j] = S[i][j];
            }
            else if(i == 0) {
                dt[i][j] = dt[i][j-1] + S[i][j];
            }
            else if(j == 0) {
                dt[i][j] = dt[i-1][j] + S[i][j];
            }
            else {
                dt[i][j] = max(dt[i-1][j], dt[i][j-1]) + S[i][j];
            }
        }
    }
    
    // 6단계: 격자판 패치 정보 디스플레이 출력
    int x = 0;
    int y = 110;
    eraseDisplay();
    
    for(int i = 0; i < limitRow; i++) {
        y = y - 15;
        x = 10;
        
        for(int j = 0; j < limitColumn; j++) {
            char mark;
            
            if(S[i][j] == 1) mark = 'O';       // 빨간색 패치
            else if(S[i][j] == -1) mark = 'X';  // 파란색 패치
            else mark = '+';                    // 일반 격자점
            
            displayStringAt(x, y, "%c", mark);
            x = x + 25;
        }
    }
    
    int finalScore = dt[limitRow-1][limitColumn-1];
    row = 0;
    r = limitRow - 1;
    c = limitColumn - 1;
    
    playTone(440, 10);
    
    // 7단계: DFS 초기화 및 실행
    maxScore = -10000;
    bestStepCount = 0;
    
    // 방문 배열 초기화
    for(int i = 0; i < limitColumn; i++) {
        for(int j = 0; j < limitRow; j++) {
            visitedSim[i][j] = 0;
        }
    }
    
    // 시작점 (3,4) 방문 처리 (이미 있는 위치이므로)
    visitedSim[limitRow-1][limitColumn-1] = 1;
    
    // 8단계: DFS 실행 - 최적 경로 탐색
    // 시작: 현재 위치(3,4), 목표: 출발점(0,0), 초기 점수: 0, 이동 횟수: 0
    dfs(limitRow-1, limitColumn-1, 0, 0);
    
    // DFS 탐색 완료 알림
    playTone(880, 20);
    sleep(500);
    eraseDisplay();
    displayBigTextLine(1, "Go!");
    displayBigTextLine(4, "MaxScore: %d", maxScore);
    
    // 로봇 위치 초기화
    r = limitRow - 1;
    c = limitColumn - 1;
    
    // 9단계: 최적 경로 실행 - 로봇이 실제로 이동
    executePath();
    
    // 10단계: 최종 결과 출력
    playTone(240, 20);
    stopMotor();
    sleep(200);
    
    eraseDisplay();
    displayBigTextLine(1, "FINISH!");
    displayBigTextLine(4, "Pos: %d, %d", r, c);
    displayBigTextLine(7, "Final Score: %d", maxScore);
    
    while(getButtonPress(1) == 0) sleep(10);
    
    playTone(240, 20);
    sleep(200);
    stopMotor();
    
    eraseDisplay();
    displayBigTextLine(1, "Score = %d", finalScore);
    
    while(getButtonPress(1) == 0) sleep(10);
}