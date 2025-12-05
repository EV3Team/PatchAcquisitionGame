#pragma config(Sensor, S1,      c1,             sensorEV3_Color)
#pragma config(Sensor, S2,      c2,             sensorEV3_Color)
#pragma config(Sensor, S3,      c3,             sensorEV3_Color)
#pragma config(Motor,  motorB,  lm,             tmotorEV3_Large, PIDControl, driveLeft, encoder)
#pragma config(Motor,  motorC,  rm,             tmotorEV3_Large, PIDControl, driveRight, encoder)

// 전역 변수 선언

int limitRow = 4;
int limitColumn = 5;

int nMotorSpeedSetting = 15, vertex = 0, count = 0, row = 0, val, r = 1, c = 1, dfs_run = 0;

int S[6][6], dt[6][6];

//DFS 알고리즘 관련 변수
int visitedSim[6][6];       // 방문 횟수 기록
int bestPath[30];           // 최적 경로 저장
int tempPath[30];           // 임시 경로 저장
int maxScore = -10000;      // 최대 점수
int bestStepCount = 0;      // 최적 경로의 단계 수
const int MAX_DEPTH = 10;   // 최대 탐색 깊이


void lineTracing() {

	val = 3.2;

	if( getColorName(c2) == 4 ) // 4 : yellow
	{
		setMotorSpeed(lm, nMotorSpeedSetting - val);
		setMotorSpeed(rm, nMotorSpeedSetting + val);
	}
	else
	{
		setMotorSpeed(lm, nMotorSpeedSetting + val);
		setMotorSpeed(rm, nMotorSpeedSetting - val);
	}
}

void go() {

	if( getColorName(c2) == 5 ) // 5 : red
	{
		// 빨간색 패치 발견 시 배열에 1로 저장

		if( row % 2 == 0 ) S[row][count] = 1;
		else S[row][(limitColumn - 1) - count] = 1;

		playTone(440, 20);
		sleep(100);
	}

	if( getColorName(c2) == 3 ) {
		if( row % 2 == 0 ) S[row][count] = -1;
		else S[row][(limitColumn - 1) - count] = -1;
		playTone(660, 20);
		sleep(50);
	}

	lineTracing();

	if((getColorName(c3) >= 3 && getColorName(c3) <= 5) ||
    (getColorName(c1) >= 3 && getColorName(c1) <= 5 && dfs_run == 1)) vertex++;
	else vertex = 0;

	if( vertex == 1 ) {
		count++;
	}
}

// 좌회전 함수

void turnLeft() {

	setMotorSpeed(lm, -nMotorSpeedSetting * 4/10);
	setMotorSpeed(rm, nMotorSpeedSetting * 4/10);

	while( getColorName(c1) <= 4 || getColorName(c2) <= 4 )
	{
		sleep(2);
	}

	while( getColorName(c1) != 4 )
	{
		sleep(2);
	}

	while( getColorName(c2) != 4 )
	{
		sleep(2);
	}

	sleep(550);

	setMotorSpeed(lm, 0);
	setMotorSpeed(rm, 0);
	sleep(200);
}

// 우회전 함수

void turnRight() {

	setMotorSpeed(lm, nMotorSpeedSetting * 4/10);
	setMotorSpeed(rm, -nMotorSpeedSetting * 4/10);

	while( getColorName(c3) <= 4 || getColorName(c2) <= 4 )
	{
		sleep(2);
	}

	while( getColorName(c3) > 4 )
	{
		sleep(2);
	}

	while( getColorName(c2) > 4 )
	{
		sleep(2);
	}

	setMotorSpeed(lm, 0);
	setMotorSpeed(rm, 0);
	sleep(200);
}

void stopMotor() {

	setMotorSpeed(lm, 0);
	setMotorSpeed(rm, 0);
}

void completeSearch() {

	while(true) {

		go();

		// 한 행의 끝에 도달했을 때
		if( count == limitColumn - 1 ) {

			// 마지막 행이면 탐색 종료
			if( row == limitRow - 1 ) return;

			// 짝수 행
			if( row % 2 == 0 ) {

				// 빨간색 패치 추가 확인
				for(int i = 0; i < 4; i++)
				{

					if( getColorName(c2) == 5 )
					{
						if( row % 2 == 0 ) S[row][count] = 1;
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

			// 홀수 행
			else
			{
				// 빨간색 패치 추가 확인
				for(int i = 0; i < 3; i++)
				{

					if( getColorName(c2) == 5 )
					{
						if( row % 2 == 0 ) S[row][count] = 1;
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

			// 다음 행으로 이동
			if( row % 2 == 0 ) {

				while( getColorName(c3) != 4 ) go();

				setMotorSpeed(lm, nMotorSpeedSetting);
				setMotorSpeed(rm, nMotorSpeedSetting);
				sleep(900);
				turnRight();
			}

			else {
				while( getColorName(c1) != 4 ) go();

				setMotorSpeed(lm, nMotorSpeedSetting);
				setMotorSpeed(rm, nMotorSpeedSetting);
				sleep(1200);
				turnLeft();
			}

			row++;
			count = 0;

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

			setMotorSpeed(lm, nMotorSpeedSetting);
			setMotorSpeed(rm, nMotorSpeedSetting);
			sleep(450);
		}
	}
}

int max(int a, int b) {

	return a > b ? a : b;
}


// --- DFS 함수 (깊이 우선 탐색)
void dfs(int x, int y, int currentScore, int steps)
{
	// 1. 시작점 도착 시
	if (x == 0 && y == 0)
	{
		// 현재 점수가 더 높으면 갱신
		if (currentScore > maxScore)
		{
			maxScore = currentScore;
			bestStepCount = steps;
			for(int i=0; i<steps; i++) bestPath[i] = tempPath[i]; // 경로 복사
		}
		return;
	}

	// 2. 최대 깊이 도달 시 중단
	if (steps >= MAX_DEPTH) return;

	// 3. 4방향 탐색 (상, 하, 좌, 우)
	int dx[] = {-1, 0, 1, 0}; // 상, 하
	int dy[] = {0, -1, 0, 1}; // 좌, 우

	for (int i = 0; i < 4; i++)
	{
		int nx = x + dx[i];
		int ny = y + dy[i];

		// 범위 체크
		if (nx < 0 || nx >= limitRow || ny < 0 || ny >= limitColumn) continue;

		// 점수 계산
		int moveCost = -1; // 이동 비용
		int patchScore = 0;

		// 패치 점수 계산
		if (S[nx][ny] == 1) // 빨간색 패치
		{
			if (visitedSim[nx][ny] == 0) patchScore = 5; // 첫 방문
			else patchScore = -2; // 재방문 페널티
		}
		else if (S[nx][ny] == -1) // 검은색 패치
		{
			patchScore = -5; // 검은색 패널티
		}

		// 같은 칸을 2번 이상 방문하면 건너뛰기
		// 순환 경로 방지를 위해 필요
		if (visitedSim[nx][ny] >= 2) continue;

		// 방문 표시
		visitedSim[nx][ny]++;
		tempPath[steps] = i; // 방향 저장

		// 재귀 호출
		dfs(nx, ny, currentScore + moveCost + patchScore, steps + 1);

		// 백트래킹
		visitedSim[nx][ny]--;
	}
}

// --- 최적 경로 실행 함수
void executePath()
{
	dfs_run = 1;
	// 현재 위치는 우하단 (limitRow-1, limitColumn-1)
	// 로봇의 초기 방향은 상(Up, -1, 0), 좌(Left, 0, -1)

	for(int i=0; i<bestStepCount; i++)
	{
		int dir = bestPath[i]; // 0:상, 1:좌, 2:하, 3:우
		int prevDir = 0;
		if(i == 0) {
			prevDir == bestPath[i];
		} else {
			prevDir = bestPath[i-1];
		}


		// 현재 위치에서 다음 위치로 이동하기 전에
		// 초기 방향이 좌측이므로 회전 필요

		if (dir == 1) // 좌 (Left) - 회전 불필요
		{
			// 그대로 직진
			c--; // 좌측 이동
		}
		else if (dir == 0) // 상 (Up) - 우회전 필요
		{
			turnRight();
			sleep(500);
			r--; // 상단 이동
		}
		else if (dir == 2) // 하 (Down) - 좌회전 필요
		{
			turnLeft();
			sleep(200);
			r++;
		}
		else if (dir == 3) // 우(Right) - 180도 회전
		{
			// 180도 회전
			setMotorSpeed(lm, nMotorSpeedSetting);
			setMotorSpeed(rm, -nMotorSpeedSetting);
			sleep(1300); // 180도 회전 시간
			stopMotor();
			sleep(200);
			c++;
		}

		// 다음 교차점까지 이동
		// 라인트레이싱 사용
		count = 0;
		while(true)
		{
			go(); // 라인 추종 및 패치 감지
			if(count == 1) // 다음 교차점 도달
			{
				if(r==0 && c==0 && i == bestStepCount-1) {
					// 최종 목적지 도착
					stopMotor();
					break;
				}
				// 라인트레이싱 종료 후 직진
				setMotorSpeed(lm, nMotorSpeedSetting);
				setMotorSpeed(rm, nMotorSpeedSetting);
				if((prevDir + 3)%4 == dir){
					sleep(1000);
					}
				else {
					sleep(1400);
					}
				break;
			}
		}

		// 회전한 경우 원래 방향으로 복귀
		if (dir == 0) // 상으로 갔다면 좌회전으로 복귀
		{
			turnLeft();
			sleep(150);
		}
		else if (dir == 2) // 하로 갔다면 우회전으로 복귀
		{
			turnRight();
			sleep(150);
		}
		else if (dir == 3) // 우로 갔다면 180도 회전
		{
			setMotorSpeed(lm, nMotorSpeedSetting);
			setMotorSpeed(rm, -nMotorSpeedSetting);
			sleep(1300);
			stopMotor();
		}
		// dir == 1 (좌측)인 경우 회전 불필요

		// 진행 상황 표시
		eraseDisplay();
		displayBigTextLine(1, "Step: %d/%d", i+1, bestStepCount);
		displayBigTextLine(4, "Pos: %d, %d", r, c);
	}
}

task main() {

	while( getButtonPress(1) == 0 ) sleep(10);

	completeSearch();

	// 180도 회전 및 우하단으로 이동

	// --- 우하단으로 이동 준비
	setMotorSpeed(lm, nMotorSpeedSetting);
	setMotorSpeed(rm, nMotorSpeedSetting);
	sleep(1100); // 전진하여 교차점 진입

	setMotorSpeed(lm, nMotorSpeedSetting);
	setMotorSpeed(rm, -nMotorSpeedSetting);
	sleep(2300); // 180도 회전하여 방향 전환

	setMotorSpeed(lm, 0);
	setMotorSpeed(rm, 0);
	sleep(500);

	// 2. (3,0) -> (3,3) 우측으로 이동
	for(int i = 0; i < limitColumn - 1; i++)
	{
		while(getColorName(c3) > 4)
		{
			if((getColorName(c3) >= 3 && getColorName(c3) <= 5)) break;
			lineTracing();
		}

		setMotorSpeed(lm, nMotorSpeedSetting);
		setMotorSpeed(rm, nMotorSpeedSetting);
		sleep(400); // 교차점 통과
	}

	setMotorSpeed(lm, 0);
	setMotorSpeed(rm, 0);
	sleep(500);

	setMotorSpeed(lm, nMotorSpeedSetting);
	setMotorSpeed(rm, nMotorSpeedSetting);
	sleep(1000); // 추가 직진

	// 3. (3,3)에서 방향 전환하여 라인트레이싱 준비
	setMotorSpeed(lm, -nMotorSpeedSetting);
	setMotorSpeed(rm, nMotorSpeedSetting);
	sleep(1300);

	while( getColorName(c2) > 4 && getColorName(c3) > 4 )
	{
		sleep(2);
	}

	sleep(750);

	stopMotor();
	sleep(1000);

	count = row = 0;

	// DP 테이블 계산
	for(int i = 0; i < limitRow; i++)
		for(int j = 0; j < limitColumn; j++)
	{
		if(i == 0 && j == 0) dt[i][j] = S[i][j];
		else if(i == 0) dt[i][j] = dt[i][j-1] + S[i][j];
		else if(j == 0) dt[i][j] = dt[i-1][j] + S[i][j];
		else dt[i][j] = max(dt[i-1][j], dt[i][j-1]) + S[i][j];
	}

	int x = 0;
	int y = 110;

	eraseDisplay();

	for(int i = 0; i < limitRow; i++)
	{
		y = y - 15;
		x = 10;

		for(int j = 0; j < limitColumn; j++)
		{
			char mark;

			if (S[i][j] == 1)       mark = 'O';
			else if (S[i][j] == -1)  mark = 'X';
			else                    mark = '+';

			displayStringAt(x, y, "%c", mark);
			x = x + 25;
		}
	}

	int finalScore = dt[limitRow-1][limitColumn-1];

	row = 0;

	r = limitRow - 1;
	c = limitColumn - 1;

	playTone(440, 10);

	// 1. DFS 초기화
	maxScore = -10000;
	bestStepCount = 0;
	for(int i=0; i<limitColumn; i++)
		for(int j=0; j<limitRow; j++) visitedSim[i][j] = 0;

	// 시작점(3,3) 방문 표시
	visitedSim[limitRow-1][limitColumn-1] = 1;

	// 2. DFS 실행 (시작점에서 (0,0)으로)
	// 시작 위치: 우하단(r, c),
	dfs(limitRow-1, limitColumn-1, 0, 0);

	// 경로 계산 완료
	playTone(880, 20);
	sleep(500);
	eraseDisplay();
	displayBigTextLine(1, "Go!");
	displayBigTextLine(4, "MaxScore: %d", maxScore);

	// 로봇 위치 초기화
	r = limitRow - 1;
	c = limitColumn - 1;

	// 3. 경로 실행
	executePath();


	// 4. 최종 도착
	playTone(240, 20);
	stopMotor();
	sleep(200);

	eraseDisplay();
	displayBigTextLine(1, "FINISH!");
	displayBigTextLine(4, "Pos: %d, %d", r, c);
	displayBigTextLine(7, "Final Score: %d", maxScore);

	while( getButtonPress(1) == 0 ) sleep(10);

	playTone(240, 20); sleep(200);
	stopMotor();

	eraseDisplay();
	displayBigTextLine(1, "Score = %d", finalScore);

	while( getButtonPress(1) == 0 ) sleep(10);
}