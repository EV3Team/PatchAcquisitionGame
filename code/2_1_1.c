#pragma config(Sensor, S1,      c1,             sensorEV3_Color)
#pragma config(Sensor, S2,      c2,             sensorEV3_Color)
#pragma config(Sensor, S3,      c3,             sensorEV3_Color)
#pragma config(Motor,  motorB,  lm,             tmotorEV3_Large, PIDControl, driveLeft, encoder)
#pragma config(Motor,  motorC,  rm,             tmotorEV3_Large, PIDControl, driveRight, encoder)




// [°À¢

int limitRow = 4;

int limitColumn = 4;



int nMotorSpeedSetting = 15, vertex = 0, count = 0, row = 0, val, r = 0, c = 0;

int S[6][6], dt[6][6];

void lineTracing() {

	val = 2;

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

void go()

{

	if( getColorName(c2) == 5 ) // 5 : red

	{

		// [°À¢ÁE¢ÆEcE¢ÆE°ÀcE

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

	if((getColorName(c3) >= 3 && getColorName(c3) <= 5)) vertex++;

	else vertex = 0;

	if( vertex == 1 ) {

		count++;
	}

}



// [¢ÆE°ÀcE°À

void turnLeft()

{



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

	sleep(500);



	setMotorSpeed(lm, 0);

	setMotorSpeed(rm, 0);

	sleep(200);

}



// [¢ÆE°

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



		// [°À¢ÁE¢ÆEcE

		if( count == limitColumn - 1 )

		{

			// [°À¢ÁE¢

			if( row == limitRow - 1 ) return;



			// A|¢ÆE°

			if( row % 2 == 0 )

			{

				// [°À¢ÁE¢Æ

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

			// E|¢ÆE°

			else

			{

				// [°À¢ÁE¢Æ

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

				sleep(1100);

				turnLeft();

			}



			// ¢ÆE°À

			if( row % 2 == 0 )

			{

				while( getColorName(c3) != 4 ) go();



				setMotorSpeed(lm, nMotorSpeedSetting);

				setMotorSpeed(rm, nMotorSpeedSetting);

				sleep(1000);

				turnRight();

			}

			else

			{

				while( getColorName(c1) != 4 ) go();



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
						// [A®œ°˛¢Æ®°°À¢Á°ÕiE °ß°˛I°ß°˛°ß°À] °À°˛°
			if (r == 0 && c == 0)
			{
				stopMotor();
				break; // ¢Æ¢¥cCA A¢ÆiAa
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



void goLeft()

{

	c--;

	count = 0;

	while(true)

	{

		go();

		if( count == 1 )

		{

					// [A®œ°˛¢Æ®°°À¢Á°ÕiE °ß°˛I°ß°˛°ß°À] °À°˛°
			if (r == 0 && c == 0)
			{
				stopMotor();
				break; // ¢Æ¢¥cCA A¢ÆiAa
			}

			setMotorSpeed(lm, nMotorSpeedSetting);

			setMotorSpeed(rm, nMotorSpeedSetting);

			sleep(1100);

			break;

		}

	}

}



task main()

{

	while( getButtonPress(1) == 0 ) sleep(10);

	completeSearch();



	// 180?

	// --- [¢ÆE°ÀcE°À¢ÁEcE°À

	// 1. A¢ÆE°ÀcE°À¢ÁEcE°À¢ÁE¢ÆEcEcE°À¢ÁE¢ÆEcE¢ÆE°ÀcEc
	setMotorSpeed(lm, nMotorSpeedSetting);
	setMotorSpeed(rm, nMotorSpeedSetting);
	sleep(1100); // E¢ÆE°ÀcE°À¢ÁEcE°À¢

	setMotorSpeed(lm, nMotorSpeedSetting);
	setMotorSpeed(rm, -nMotorSpeedSetting);
	sleep(2300); // 180¢ÆE°ÀcE°À¢ÁEcE°À¢ÁE¢ÆEcE

	setMotorSpeed(lm, 0);
	setMotorSpeed(rm, 0);
	sleep(500);

	// 2. (3,0) -> (3,3) °À¢ÁE¢ÆEcE¢ÆE°ÀcEcE
	// AI°À¢ÁE¢ÆEcE¢ÆE°ÀcEcE¢ÆE°ÀcE°À¢ÁEcE
	for(int i = 0; i < limitColumn; i++)
	{
		// ¢ÆE°ÀcE°À¢ÁEcE°À¢ÁE¢Æ
		// (go CO¢ÆE°ÀcE°À¢ÁEcE°À¢ÁE¢
		while(getColorName(c3) > 4)
		{
			// °À¢ÁE¢ÆEcE¢ÆE°ÀcEcE¢
			if((getColorName(c3) >= 3 && getColorName(c3) <= 5)) break; // E¢ÆE°ÀcE°À¢

			// A°À¢ÁE¢ÆEcE¢ÆE°ÀcEcE¢ÆE
			lineTracing();
		}

		// °À¢ÁE¢ÆEcE¢ÆE°ÀcEcE¢ÆE°ÀcE°À¢ÁEcEcE¢Æ
		setMotorSpeed(lm, nMotorSpeedSetting);
		setMotorSpeed(rm, nMotorSpeedSetting);
		sleep(400); // ¢ÆE°ÀcE°À¢Á
	}

	setMotorSpeed(lm, 0);
	setMotorSpeed(rm, 0);
	sleep(500);

	setMotorSpeed(lm, nMotorSpeedSetting);
	setMotorSpeed(rm, nMotorSpeedSetting);
	sleep(1200); // °À¢ÁE¢ÆEc

	// 3. (3,3)¢ÆE°ÀcE°À¢ÁEcE°À¢ÁE¢ÆEcEcE°À¢ÁE¢ÆEcE¢ÆE°ÀcEcE
	// (1¢ÆE°ÀcE¢ÆE°ÀcIU°À¢ÁE¢ÆEc°À¢Á¢Æ°ø¢ÆE°Àce) ¢ÆE°Àc°À¢Á¢Æ
	// 2300ms°À¢ÁE¢ÆEc°À¢Á¢Æ°ø¢ÆE°Àc¢ÆE°ÀcE°À¢ÁEc 270¢ÆE°ÀcIi¢ÆE°ÀcI
	setMotorSpeed(lm, -nMotorSpeedSetting);
	setMotorSpeed(rm, nMotorSpeedSetting);
	sleep(1300);

	// (2¢ÆE°ÀcE¢ÆE°ÀcIU°À¢ÁE¢ÆEc°À¢Á¢Æ°ø¢ÆE°Àce) ¢
	// °À¢ÁE¢ÆEc°À¢Á¢Æ°ø¢ÆE°Àc¢ÆE°ÀcE°À¢ÁEc¢ÆE°ÀcE?i¢ÆE°ÀcIi°À¢Á
	// 4 = Yellow, 5 = Red, 1 = Black. ¢ÆE°Àc°À¢Á¢Æ¢Ø¢Æ
	// Ai, 4¢ÆE°Àc°À¢Á¢Æ¢Ø¢ÆE°Àc°ß
	while( getColorName(c2) > 4 && getColorName(c3) > 4 )
	{
		sleep(2);
	}

	sleep(750);

	// (3¢ÆE°ÀcE¢ÆE°ÀcIU°À¢ÁE¢ÆEc
	stopMotor();
	sleep(1000);


	count = row = 0;



	// DP ¢Æ

	for(int i = 0; i < limitRow; i++)

	for(int j = 0; j < limitColumn; j++)

	{

		if(i == 0 && j == 0) dt[i][j] = S[i][j];

		else if(i == 0) dt[i][j] = dt[i][j-1] + S[i][j];

		else if(j == 0) dt[i][j] = dt[i-1][j] + S[i][j];

		else dt[i][j] = max(dt[i-1][j], dt[i][j-1]) + S[i][j];

	}



	int x = 0;
	int y = 110; // E¢ÆE°ÀcE°À

	// E¢ÆE°ÀcE°À
	eraseDisplay();

	// [°À¢ÁE¢ÆEcE¢ÆE°À
	for(int i = 0; i < limitRow; i++)
	{
		y = y - 15; // AU ¢ÆE°Àc
		x = 10;     // °À¢ÁE¢ÆEc

		for(int j = 0; j < limitColumn; j++)
		{
			char mark; // Aa¢ÆE°

			if (S[i][j] == 1)       mark = 'O'; // ¢ÆE°ÀcE°À
			else if (S[i][j] == -1)  mark = 'X'; // °À¢ÁE¢ÆEcE¢ÆE°ÀcEcE¢
			else                    mark = '+'; // °À¢ÁE¢ÆEc

			// ¢ÆE°ÀcE°À¢
			displayStringAt(x, y, "%c", mark);

			x = x + 25; // A¢ÆE°ÀcE
		}
	}



	row = 0;

	// °À¢ÁE¢ÆEcE¢Æ

	r = limitRow - 1;

	c = limitColumn - 1;



	// AOAu ?

	while( r != 0 || c != 0 )

	{

		if( r == 0 ) {
			goLeft();
		}

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
