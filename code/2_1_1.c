#pragma config(Sensor, S1,      c1,             sensorEV3_Color)
#pragma config(Sensor, S2,      c2,             sensorEV3_Color)
#pragma config(Sensor, S3,      c3,             sensorEV3_Color)
#pragma config(Motor,  motorB,  lm,             tmotorEV3_Large, PIDControl, driveLeft, encoder)
#pragma config(Motor,  motorC,  rm,             tmotorEV3_Large, PIDControl, driveRight, encoder)

#define DIFF 10

// [¼³Á
int limitRow = 4;
int limitColumn = 4;

// [Ãß°¡] noiseFilter º¯¼ö Ãß°¡ (¶óÀÎ
int nMotorSpeedSetting = 30, vertex = 0, count = 0, row = 0, val, r = 0, c = 0;
int noiseFilter = 0;
int S[6][6], dt[6][6];

void go()
{
    val = 5;
    displayBigTextLine(1, "count = %d", count);
    if( getColorName(c2) == 5 ) // 5 : red
    {
        if( row % 2 == 0 ) S[row][count] = 1;
        else S[row][(limitColumn - 1) - count] = 1;
        playTone(440, 20); sleep(100);
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

    // [¼öÁ¤µÊ] µð¹Ù¿î½
    // ¼±(³ë¶û)ÀÌ º¸ÀÌ¸é Áï½Ã vertex Áõ
    if(getColorName(c1) == 4 || getColorName(c3) == 4)
    {
        vertex++;
        noiseFilter = 0; // ¼±ÀÌ º¸ÀÌ¸é 
    }
    else
    {
        vertex = 0;
    }

    if( vertex == 1 ) {
        count++;
        sleep(250);
        // [»èÁ¦µÊ] °­Á¦ Á÷Áø(Blind Forward
        // ·Îº¿ À§Ä¡°¡ Æ²¾îÁö´Â ¿øÀÎÀÌ¾úÀ¸
    }

    displayBigTextLine(1, "count = %d", count);
}

// [ÁÂÈ¸
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

// [¿ìÈ¸
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

    sleep(50);

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

        if( count == limitColumn - 1 )
        {
            if( row == limitRow - 1 ) return;

            if( row % 2 == 0 )
            {
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
                turnRight();
            }
            else
            {
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
                  setMotorSpeed(lm, nMotorSpeedSetting * 4/10);
                            setMotorSpeed(rm, nMotorSpeedSetting * 4/10);
                            sleep(800);
                  turnLeft();
            }

            if( row % 2 == 0 )
            {
                  while( getColorName(c3) > 4 ) go();

                  setMotorSpeed(lm, 35);
                  setMotorSpeed(rm, 30);
                  sleep(400);
                  turnRight();
            }
            else
            {
                  while( getColorName(c1) > 4 ) go();

                  setMotorSpeed(lm, 30);
                  setMotorSpeed(rm, 35);
                  sleep(400);
                  setMotorSpeed(lm, nMotorSpeedSetting * 4/10);
                            setMotorSpeed(rm, nMotorSpeedSetting * 4/10);
                            sleep(400);
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

    // 180µ
    setMotorSpeed(lm, 20);
    setMotorSpeed(rm, 20);
    sleep(500);

    // 180µ
    setMotorSpeed(lm, -20);
    setMotorSpeed(rm, 20);
    sleep(1650);

    count = row = 0;

    // DP °è
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
      // È­¸é 
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
      // ½ÃÀÛ ÁÂÇ
      r = limitRow - 1;
      c = limitColumn - 1;

      // ÁÖ
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
