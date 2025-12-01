#pragma config(Sensor, S1, c1, sensorEV3_Color)
#pragma config(Sensor, S2, c2, sensorEV3_Color)
#pragma config(Sensor, S3, c3, sensorEV3_Color)
#pragma config(Motor, motorB, lm, tmotorEV3_Large, PIDControl, driveLeft, encoder)
#pragma config(Motor, motorC, rm, tmotorEV3_Large, PIDControl, driveRight, encoder)

int limitRow = 4;
int limitColumn = 4;
int nMotorSpeedSetting = 15, vertex = 0, count = 0, row = 0, val, r = 0, c = 0;
int S[6][6], dt[6][6];

// 라인 트레이싱
void lineTracing() {
	val = 2; // 속도 보정 값
	if(getColorName(c2) == 4) { // 중앙 센서(c2)가 노란색 라인 위에 있으면 → 왼쪽으로 살짝 틀어서 라인 경계 유지
		setMotorSpeed(lm, nMotorSpeedSetting - val);
		setMotorSpeed(rm, nMotorSpeedSetting + val);
	}
	else {                     // 라인을 벗어나면 → 오른쪽으로 틀어서 라인 복귀
		setMotorSpeed(lm, nMotorSpeedSetting + val);
		setMotorSpeed(rm, nMotorSpeedSetting - val);
	}
}

// 이동 ( 전진하면서 색상 감지 )
void go() {
	if(getColorName(c2) == 5) { 
		if(row % 2 == 0) S[row][count] = 1;
		else S[row][(limitColumn - 1) - count] = 1;
		playTone(440, 20);
		sleep(100);
	}
	if(getColorName(c2) == 3) {
		if(row % 2 == 0) S[row][count] = -1;
		else S[row][(limitColumn - 1) - count] = -1;
		playTone(660, 20);
		sleep(50);
	}
	lineTracing(); // 라인 트레이싱
	if((getColorName(c3) >= 3 && getColorName(c3) <= 5)) vertex++;
	else vertex = 0;
	if(vertex == 1) {
		count++;
	}
}

// 왼쪽으로 회전 ( 센서로 노란색 라인 찾을 때 까지 회전 )
void turnLeft() {
	setMotorSpeed(lm, -nMotorSpeedSetting * 4/10);
	setMotorSpeed(rm, nMotorSpeedSetting * 4/10);
	while(getColorName(c1) <= 4 || getColorName(c2) <= 4) {
		sleep(2);
	}
	while(getColorName(c1) != 4) {
		sleep(2);
	}
	while(getColorName(c2) != 4) {
		sleep(2);
	}
	sleep(500);
	setMotorSpeed(lm, 0);
	setMotorSpeed(rm, 0);
	sleep(200);
}

// 오른쪽으로 회전 ( 센서로 노란색 라인 찾을 때 까지 회전 )
void turnRight() {
	setMotorSpeed(lm, nMotorSpeedSetting * 4/10);
	setMotorSpeed(rm, -nMotorSpeedSetting * 4/10);
	while(getColorName(c3) <= 4 || getColorName(c2) <= 4) {
		sleep(2);
	}
	while(getColorName(c3) > 4) {
		sleep(2);
	}
	while(getColorName(c2) > 4) {
		sleep(2);
	}
	setMotorSpeed(lm, 0);
	setMotorSpeed(rm, 0);
	sleep(200);
}

// 모터 정지
void stopMotor() {
	setMotorSpeed(lm, 0);
	setMotorSpeed(rm, 0);
}

// 4×4 그리드 전체 탐색 ( 지그재그로 이동하며 색상 감지 )
void completeSearch() {
	while(true) {
		go();
		if(count == limitColumn - 1) {
			if(row == limitRow - 1) return;
			if(row % 2 == 0) {
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
				sleep(800);
				turnRight();
			}
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
				sleep(1100);
				turnLeft();
			}
			if(row % 2 == 0) {
				while(getColorName(c3) != 4) go();
				setMotorSpeed(lm, nMotorSpeedSetting);
				setMotorSpeed(rm, nMotorSpeedSetting);
				sleep(1000);
				turnRight();
			}
			else {
				while(getColorName(c1) != 4) go();
				setMotorSpeed(lm, nMotorSpeedSetting);
				setMotorSpeed(rm, nMotorSpeedSetting);
				sleep(1400);
				turnLeft();
			}
			row++;
			count = 0;
			setMotorSpeed(lm, -nMotorSpeedSetting);
			setMotorSpeed(rm, -nMotorSpeedSetting);
			if(row % 2 == 0) {
				sleep(900);
			}
			else {
				sleep(1350);
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

// 두 수 중 큰 수 반환
int max(int a, int b) {
	return a > b ? a : b;
}

// 위로 이동 ( DP 역추적용 이동 함수 )
void goUp() {
	r--; row++;
	turnRight();
	count = 0;
	while(true) {
		go();
		if(count == 1) {
			if(r == 0 && c == 0) {
				stopMotor();
				break;
			}
			setMotorSpeed(lm, nMotorSpeedSetting);
			setMotorSpeed(rm, nMotorSpeedSetting);
			sleep(1300);
			turnLeft();
			sleep(100);
			break;
		}
	}
}

// 왼쪽으로 이동 ( DP 역추적용 이동 함수 )
void goLeft() {
	c--;
	count = 0;
	while(true) {
		go();
		if(count == 1) {
			if(r == 0 && c == 0) {
				stopMotor();
				break;
			}
			setMotorSpeed(lm, nMotorSpeedSetting);
			setMotorSpeed(rm, nMotorSpeedSetting);
			sleep(1100);
			break;
		}
	}
}

// 메인 함수
task main() {
	while(getButtonPress(1) == 0) sleep(10);
	completeSearch();
	setMotorSpeed(lm, nMotorSpeedSetting);
	setMotorSpeed(rm, nMotorSpeedSetting);
	sleep(1100);
	setMotorSpeed(lm, nMotorSpeedSetting);
	setMotorSpeed(rm, -nMotorSpeedSetting);
	sleep(2300);
	setMotorSpeed(lm, 0);
	setMotorSpeed(rm, 0);
	sleep(500);
	for(int i = 0; i < limitColumn; i++) {
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
	sleep(1200);
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
	for(int i = 0; i < limitRow; i++)
	for(int j = 0; j < limitColumn; j++) {
		if(i == 0 && j == 0) dt[i][j] = S[i][j];
		else if(i == 0) dt[i][j] = dt[i][j-1] + S[i][j];
		else if(j == 0) dt[i][j] = dt[i-1][j] + S[i][j];
		else dt[i][j] = max(dt[i-1][j], dt[i][j-1]) + S[i][j];
	}
	int x = 0;
	int y = 110;
	eraseDisplay();
	for(int i = 0; i < limitRow; i++) {
		y = y - 15;
		x = 10;
		for(int j = 0; j < limitColumn; j++) {
			char mark;
			if(S[i][j] == 1) mark = 'O';
			else if(S[i][j] == -1) mark = 'X';
			else mark = '+';
			displayStringAt(x, y, "%c", mark);
			x = x + 25;
		}
	}
	row = 0;
	r = limitRow - 1;
	c = limitColumn - 1;
	while(r != 0 || c != 0) {
		if(r == 0) {
			goLeft();
		}
		else if(c == 0) goUp();
		else if(dt[r-1][c] > dt[r][c-1]) goUp();
		else goLeft();
		eraseDisplay();
		displayBigTextLine(1, "%d %d(%d)", r, c, row);
	}
	playTone(240, 20); sleep(200);
	stopMotor();
	while(getButtonPress(1) == 0) sleep(10);
}