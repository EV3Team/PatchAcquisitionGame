#pragma config(Sensor, S1, c1, sensorEV3_Color)
#pragma config(Sensor, S2, c2, sensorEV3_Color)
#pragma config(Sensor, S3, c3, sensorEV3_Color)
#pragma config(Motor, motorB, lm, tmotorEV3_Large, PIDControl, driveLeft, encoder)
#pragma config(Motor, motorC, rm, tmotorEV3_Large, PIDControl, driveRight, encoder)

// 전역 변수 선언
int limitRow = 4;        // 격자판의 행 개수
int limitColumn = 4;     // 격자판의 열 개수
int nMotorSpeedSetting = 15;  // 기본 모터 속도
int vertex = 0;          // 교차점 감지 카운터
int count = 0;           // 현재 행에서 이동한 열 개수
int row = 0;             // 현재 탐색 중인 행 번호
int val, r = 1, c = 1;
int S[6][6];             // 격자판 패치 정보 저장 배열 (O:+1, X:-1, +:0)
int dt[6][6];            // DP 테이블 (최대 점수 저장)

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
    stopMotor();
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
    
    stopMotor();
    sleep(200);
}

// 모터 정지 함수
void stopMotor() {
    setMotorSpeed(lm, 0);
    setMotorSpeed(rm, 0);
}

// 4x4 격자판 완전 탐색 함수 (행 우선 탐색)
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
            
            stopMotor();
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

// 위로 한 칸 이동 (최단 경로 복귀 시 사용)
void goUp() {
    r--;
    row++;
    turnRight();
    count = 0;
    
    while(true) {
        go();
        
        if(count == 1) {
            // 출발점 도착 시 정지
            if(r == 0 && c == 0) {
                stopMotor();
                break;
            }
            
            setMotorSpeed(lm, nMotorSpeedSetting);
            setMotorSpeed(rm, nMotorSpeedSetting);
            sleep(1100);
            turnLeft();
            sleep(100);
            break;
        }
    }
}

// 왼쪽으로 한 칸 이동 (최단 경로 복귀 시 사용)
void goLeft() {
    c--;
    count = 0;
    
    while(true) {
        go();
        
        if(count == 1) {
            // 출발점 도착 시 정지
            if(r == 0 && c == 0) {
                stopMotor();
                break;
            }
            
            setMotorSpeed(lm, nMotorSpeedSetting);
            setMotorSpeed(rm, nMotorSpeedSetting);
            sleep(900);
            break;
        }
    }
}

task main() {
    // 버튼 누를 때까지 대기
    while(getButtonPress(1) == 0) sleep(10);
    
    // 1단계: 4x4 격자판 완전 탐색
    completeSearch();
    
    // 2단계: 이동점으로 이동 준비 (180도 회전)
    setMotorSpeed(lm, nMotorSpeedSetting);
    setMotorSpeed(rm, nMotorSpeedSetting);
    sleep(1100);
    
    setMotorSpeed(lm, nMotorSpeedSetting);
    setMotorSpeed(rm, -nMotorSpeedSetting);
    sleep(2300);
    
    stopMotor();
    sleep(500);
    
    // 3단계: (3,0) → (3,3) 이동점까지 이동
    for(int i = 0; i < limitColumn - 1; i++) {
        while(getColorName(c3) > 4) {
            if((getColorName(c3) >= 3 && getColorName(c3) <= 5)) break;
            lineTracing();
        }
        
        setMotorSpeed(lm, nMotorSpeedSetting);
        setMotorSpeed(rm, nMotorSpeedSetting);
        sleep(400);
    }
    
    stopMotor();
    sleep(500);
    
    setMotorSpeed(lm, nMotorSpeedSetting);
    setMotorSpeed(rm, nMotorSpeedSetting);
    sleep(1000);
    
    // 4단계: (3,3) 이동점에서 좌회전하여 출발점 방향으로 정렬
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
    
    // 5단계: DP 알고리즘으로 최대 점수 경로 계산
    for(int i = 0; i < limitRow; i++) {
        for(int j = 0; j < limitColumn; j++) {
            if(i == 0 && j == 0) {
                dt[i][j] = S[i][j];  // 시작점 초기화
            }
            else if(i == 0) {
                dt[i][j] = dt[i][j-1] + S[i][j];  // 첫 행: 왼쪽에서만 이동 가능
            }
            else if(j == 0) {
                dt[i][j] = dt[i-1][j] + S[i][j];  // 첫 열: 위에서만 이동 가능
            }
            else {
                dt[i][j] = max(dt[i-1][j], dt[i][j-1]) + S[i][j];  // 최댓값 선택
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
    
    // 최종 획득 점수 계산
    int finalScore = dt[limitRow-1][limitColumn-1];
    row = 0;
    r = limitRow - 1;
    c = limitColumn - 1;
    
    // 7단계: DP 경로 역추적하여 출발점까지 최단 경로 복귀
    while(r != 0 || c != 0) {
        if(r == 0) {
            goLeft();  // 첫 행: 왼쪽으로만 이동 가능
        }
        else if(c == 0) {
            goUp();    // 첫 열: 위로만 이동 가능
        }
        else if(dt[r-1][c] > dt[r][c-1]) {
            goUp();    // 위쪽 경로가 더 높은 점수
        }
        else {
            goLeft();  // 왼쪽 경로가 더 높은 점수
        }
        
        // 현재 위치 디스플레이 출력
        eraseDisplay();
        displayBigTextLine(1, "%d %d(%d)", r, c, row);
    }
    
    // 출발점 도착 알림
    playTone(240, 20);
    sleep(200);
    stopMotor();
    
    // 최종 점수 디스플레이 출력
    eraseDisplay();
    displayBigTextLine(1, "Score = %d", finalScore);
    
    // 종료 대기
    while(getButtonPress(1) == 0) sleep(10);
}