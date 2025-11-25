#pragma config(Sensor, S1,      c1,             sensorEV3_Color)
#pragma config(Sensor, S2,      c2,             sensorEV3_Color)
#pragma config(Sensor, S3,      c3,             sensorEV3_Color)
#pragma config(Motor,  motorB,          lm,             tmotorEV3_Large, PIDControl, driveLeft, encoder)
#pragma config(Motor,  motorC,          rm,             tmotorEV3_Large, PIDControl, driveRight, encoder)

#define DIFF 10

int nMotorSpeedSetting = 30, vertex = 0, count = 0, row = 0, val, r = 0, c = 0;
int S[6][6], dt[6][6];

void go()
{
    val = 5;
    if( getColorName(c2) == 5 ) // 5 : red
    {
        if( row % 2 == 0 ) S[row][count] = 1;
        else S[row][4-count] = 1;
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
    if(getColorName(c1) == 4 || getColorName(c3) == 4) vertex++;
    else vertex = 0;
    if( vertex == 1 ) {
        count++;
    }
    displayBigTextLine(1, "count = %d", count);
}

// [¢®¡×uoA¢®E¡Ë¡Í¢®IiE] E¢®E¢®©­Au ¢®¡×uO¢®Ii¢
void turnLeft()
{
		setMotorSpeed(lm, nMotorSpeedSetting * 4/10);
    setMotorSpeed(rm, nMotorSpeedSetting * 4/10);
    sleep(1000);
    // 1. E¢®E¢®©­Au ¢®¡×oAAU (¡Ë¢ç¡§uaA
    setMotorSpeed(lm, -nMotorSpeedSetting * 4/10);
    setMotorSpeed(rm, nMotorSpeedSetting * 4/10);

    // 2. [A¡§I¢®©­¡Ë¢ç¡§¢®¢®E¡Ëc] CoAc ¡§Ioa¡Ë¢ç¡§¢®i A
    // AI¡Ë¢ç¡Ë¢¥¢®E¢®©­¡Ë¢ç¡§¢®O C¢®¡×¡§¡
    while( getColorName(c1) <= 4 || getColorName(c2) <= 4 )
    {
        sleep(2);
    }

    // 3. ¡Ë¢çio¡Ë¢ç¡Ë¢¥I¢®E?i ¢®EOoAIA¡
    // sleep ¢®¡×oA¡Ë¢ç¡§¢®¡Ë¢çIA¡Ë¢çi 10 -> 2¡Ë
    while( getColorName(c1) > 4 )
    {
        sleep(2);
    }
    while( getColorName(c2) > 4 )
    {
        sleep(2);
    }
    sleep(350);

    // 4. A¢®E¡Ë¡
    setMotorSpeed(lm, 0);
    setMotorSpeed(rm, 0);
    sleep(200); // ¡Ë¢ç¡§¢®u¢®¡×u¢®¡×¢®©­A
}

// [¢®¡×uoA¢®E¡Ë¡Í¢®IiE] turn
void turnRight()
{
    // 1. E¢®E¢®©­Au ¢®¡×oAAU 
    setMotorSpeed(lm, nMotorSpeedSetting * 4/10);
    setMotorSpeed(rm, -nMotorSpeedSetting * 4/10);

    // 2. [A¡§I¢®©­¡Ë¢ç¡§¢®¢®E¡Ëc]
    while( getColorName(c3) <= 4 || getColorName(c2) <= 4 )
    {
        sleep(2);
    }

    // 3. ¢®EOoAI ¡Ë¢ç¡§¢®¡Ë¢
    while( getColorName(c3) > 4 )
    {
        sleep(2);
    }
    while( getColorName(c2) > 4 )
    {
        sleep(2);
    }

    // 4. A¢
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
        if( count == 4 )
        {
            if( row == 4 ) return;

            // A|¢®¡×uo Ca 
            if( row % 2 == 0 )
            {

                // ¢®¡×u¢®¡×¡Ë¢çAIAU 
                for(int i = 0; i < 4; i++)
                {
                    if( getColorName(c2) == 5 )
                    {
                        if( row % 2 == 0 ) S[row][count] = 1;
                        else S[row][4-count] = 1;
                        playTone(440, 20);
                        sleep(80);
                    }
                    else sleep(90);
                }
                turnRight(); // ¢®¡×uoA¢®
            }
            // E|¢®¡×uo Ca
            else //leftTUrn
            {

                  for(int i = 0; i < 3; i++)
                  {
                      if( getColorName(c2) == 5 )
                      {
                          if( row % 2 == 0 ) S[row][count] = 1;
                          else S[row][4-count] = 1;
                          playTone(440, 20);
                          sleep(80);
                      }
                      else sleep(90);
                  }
                  turnLeft(); // ¢®¡×uoA¢®
            }

            // ¢®E¢®IUA¢®¡×o Ca A¡§I¡§¡ÌAO
            if( row % 2 == 0 )
            {
                  // [¢®¡×uoA¢®E¡Ë¡Í] Eo¡Ë¢çio(6)AI ¢®Ii¢®E
                  // go()CO¢®¡×uo¢®E¢®IA A¡Ë¢çAA¡
                  while( getColorName(c3) > 4 ) go();

                  setMotorSpeed(lm, 35);
                  setMotorSpeed(rm, 30);
                  sleep(400); // ¡Ë¢ç¡§
                  turnRight();
            }
            else
            {
                  while( getColorName(c1) > 4 ) go();

                  setMotorSpeed(lm, 30);
                  setMotorSpeed(rm, 35);
                  sleep(400);
                  turnLeft();
            }
            row++;
            count = 0;
            // [¢®¡×uoA¢®E¡Ë¡Í] EAA¡§I¡§
            setMotorSpeed(lm, -15); // ¢®¡×uO¢®Ii¢®Ii -10A¢®¡×¢®©­ 
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

    // ¢®E¢®©­ECI ¢®E?I¡
    setMotorSpeed(lm, 20);
    setMotorSpeed(rm, 20);
    sleep(500); // 1AE¢®E¢®IA ¡§I¨Ï¨£

    // 180¢®Ii¢®Ii AI (¢®E?¢®¡×I¡Ë¢ç¡§ua¢®E¢
    setMotorSpeed(lm, -20);
    setMotorSpeed(rm, 20);
    sleep(1650);

    count = row = 0;

    // DP ¡Ë
    for(int i = 0; i < 5; i++)
        for(int j = 0; j < 5; j++)
        {
            if(i == 0 && j == 0) dt[i][j] = S[i][j];
            else if(i == 0) dt[i][j] = dt[i][j-1] + S[i][j];
            else if(j == 0) dt[i][j] = dt[i-1][j] + S[i][j];
            else dt[i][j] = max(dt[i-1][j], dt[i][j-1]) + S[i][j];
        }

      int x = 0;
      int y = 100;
      for(int i = 0; i < 5; i++)
      {
          y = y - 10;
          x = 0;
          for(int j = 0; j < 5; j++)
          {
              displayStringAt(x, y, "%d[%d]", S[i][j], dt[i][j]);
              x = x + 25;
          }
      }

      row = 0; r = c = 4;

      // AOAu ¡Ë¢
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
