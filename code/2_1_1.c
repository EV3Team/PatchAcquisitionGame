#pragma config(Sensor, S1,      c1,             sensorEV3_Color)
#pragma config(Sensor, S2,      c2,             sensorEV3_Color)
#pragma config(Sensor, S3,      c3,             sensorEV3_Color)
#pragma config(Motor,  motorB,  lm,             tmotorEV3_Large, PIDControl, driveLeft, encoder)
#pragma config(Motor,  motorC,  rm,             tmotorEV3_Large, PIDControl, driveRight, encoder)

#define DIFF 10

// [¡§u¨Ï
int limitRow = 4;
int limitColumn = 4;

int nMotorSpeedSetting = 15, vertex = 0, count = 0, row = 0, val, r = 0, c = 0;
int S[6][6], dt[6][6];

void go()
{
	val = 3;
	if( getColorName(c2) == 5 ) // 5 : red
	{
		// [¡Íi¡Ë?Au ¡§¡þ?¢®¨¡¨Ï¢®] ¡Ë
		if( row % 2 == 0 ) S[row][count] = 1;
		else S[row][(limitColumn - 1) - count] = 1;
	}
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
	if((getColorName(c3) >= 2 && getColorName(c3) <= 5)) vertex++;
	else vertex = 0;
	if( vertex == 1 ) {
		count++;
		playTone(440, 20); sleep(100);
	}
}

// [¢®E¡Ëc¡Ë¢ç¢®¢
void turnLeft()
{

	setMotorSpeed(lm, -nMotorSpeedSetting * 4/10);
	setMotorSpeed(rm, nMotorSpeedSetting * 4/10);

	while( getColorName(c1) <= 4 || getColorName(c2) <= 4 )
	{
		sleep(2);
	}

	while( getColorName(c1) > 4 )
	{
		sleep(2);
	}
	while( getColorName(c2) > 4 )
	{
		sleep(2);
	}
	sleep(250);

	setMotorSpeed(lm, 0);
	setMotorSpeed(rm, 0);
	sleep(200);
}

// [¢®E¡Ëc¡
void turnRight()
{
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

void stopMotor()
{
	setMotorSpeed(lm, 0);
	setMotorSpeed(rm, 0);
}

void completeSearch()
{
	while(true)
	{
		go();

		// [¡Íi¡Ë?Au ¡§¡þ?
		if( count == limitColumn - 1 )
		{
			// [¡Íi¡Ë?Au 
			if( row == limitRow - 1 ) return;

			// A|¢®E¡Ëc
			if( row % 2 == 0 )
			{
				// [¡Ë?¨Ï¨£¡§¡
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
				sleep(800);
				turnRight();
			}
			// E|¢®E¡Ëc
			else
			{
				// [¡Ë?¨Ï¨£¡§¡
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
				sleep(800);
				turnLeft();
			}

			// ¢®E¡ËcE¢
			if( row % 2 == 0 )
			{
				while( getColorName(c3) > 4 ) go();

				setMotorSpeed(lm, nMotorSpeedSetting);
				setMotorSpeed(rm, nMotorSpeedSetting);
				sleep(1200);
				turnRight();
			}
			else
			{
				while( getColorName(c1) > 4 ) go();

				setMotorSpeed(lm, nMotorSpeedSetting);
				setMotorSpeed(rm, nMotorSpeedSetting);
				sleep(1200);
				turnLeft();
			}
			row++;
			count = 0;

			setMotorSpeed(lm, -15);
			setMotorSpeed(rm, -15);
			sleep(410);
		}
	}
}

int max(int a, int b)
{
	return a > b ? a : b;
}

void goUp()
{
	r--; row++;
	turnRight();
	count = 0;
	while(true)
	{
		go();
		if( count == 1 )
		{
			sleep(300);
			turnLeft();
			break;
		}
	}
}

void goLeft()
{
	c--;
	count = 0;
	while(true)
	{
		go();
		if( count == 1 )
		{
			setMotorSpeed(lm, 30);
			setMotorSpeed(rm, 30);
			sleep(400);
			break;
		}
	}
}

task main()
{
	while( getButtonPress(1) == 0 ) sleep(10);
	completeSearch();

	// 180¡
	setMotorSpeed(lm, 20);
	setMotorSpeed(rm, 20);
	sleep(500);

	// 180¡
	setMotorSpeed(lm, -20);
	setMotorSpeed(rm, 20);
	sleep(1650);

	count = row = 0;

	// DP ¢®¨¡
	for(int i = 0; i < limitRow; i++)
		for(int j = 0; j < limitColumn; j++)
	{
		if(i == 0 && j == 0) dt[i][j] = S[i][j];
		else if(i == 0) dt[i][j] = dt[i][j-1] + S[i][j];
		else if(j == 0) dt[i][j] = dt[i-1][j] + S[i][j];
		else dt[i][j] = max(dt[i-1][j], dt[i][j-1]) + S[i][j];
	}

	int x = 0;
	int y = 100;
	// E¢®¨Ï
	for(int i = 0; i < limitRow; i++)
	{
		y = y - 10;
		x = 0;
		for(int j = 0; j < limitColumn; j++)
		{
			displayStringAt(x, y, "%d[%d]", S[i][j], dt[i][j]);
			x = x + 25;
		}
	}

	row = 0;
	// ¡§oAAU AAC¢®I ¡
	r = limitRow - 1;
	c = limitColumn - 1;

	// AOAu ¢
	while( r != 0 || c != 0 )
	{
		if( r == 0 ) goLeft();
		else if( c == 0 ) goUp();
		else if( dt[r-1][c] > dt[r][c-1] ) goUp();
		else goLeft();
		eraseDisplay();
		displayBigTextLine(1, "%d %d(%d)", r, c, row);
	}

	playTone(240, 20); sleep(200);
	stopMotor();
	while( getButtonPress(1) == 0 ) sleep(10);
}
