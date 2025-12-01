#pragma config(Sensor, S1,      c1,             sensorEV3_Color)
#pragma config(Sensor, S2,      c2,             sensorEV3_Color)
#pragma config(Sensor, S3,      c3,             sensorEV3_Color)
#pragma config(Motor,  motorB,  lm,             tmotorEV3_Large, PIDControl, driveLeft, encoder)
#pragma config(Motor,  motorC,  rm,             tmotorEV3_Large, PIDControl, driveRight, encoder)

// [?

int limitRow = 4;
int limitColumn = 4;

int nMotorSpeedSetting = 15, vertex = 0, count = 0, row = 0, val, r = 1, c = 1,;

int S[6][6], dt[6][6];

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
      // [?EEcE?

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

   if((getColorName(c3) >= 3 && getColorName(c3) <= 5 && row % 2 == 0) || (getColorName(c3) >= 3 && getColorName(c3) <= 5 && row % 2 == 1)) vertex++;
   else vertex = 0;

   if( vertex == 1 ) {
      count++;
   }
}

// [EQ

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

// [

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

      // [?E?
      if( count == limitColumn - 1 ) {

         // [?
         if( row == limitRow - 1 ) return;

         // A|
         if( row % 2 == 0 ) {

            // [?E
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

         // E|
         else
         {
            // [?E
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

         // E
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

void goUp() {

   r--; row++;
   turnRight();
   count = 0;

   while(true) {
      go();

      if(count == 1) {
         if (r == 0 && c == 0) {
            stopMotor();  // ??x ? H?
            break; // K D??b
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

void goLeft() {

   c--;
   count = 0;

   while(true) {
      go();

      if(count == 1) {
         if (r == 0 && c == 0) {
            stopMotor();  // ??x ? H?
            break; // K D??b
         }

         setMotorSpeed(lm, nMotorSpeedSetting);
         setMotorSpeed(rm, nMotorSpeedSetting);
         sleep(900);
         break;
      }
   }
}

task main() {

   while( getButtonPress(1) == 0 ) sleep(10);

   completeSearch();

   // 180?

   // --- [EQE?
   setMotorSpeed(lm, nMotorSpeedSetting);
   setMotorSpeed(rm, nMotorSpeedSetting);
   sleep(1100); // EEQE?E

   setMotorSpeed(lm, nMotorSpeedSetting);
   setMotorSpeed(rm, -nMotorSpeedSetting);
   sleep(2300); // 180EQE?EcE?

   setMotorSpeed(lm, 0);
   setMotorSpeed(rm, 0);
   sleep(500);

   // 2. (3,0) -> (3,3) ?EEcEE
   for(int i = 0; i < limitColumn - 1; i++)
   {
      while(getColorName(c3) > 4)
      {
         if((getColorName(c3) >= 3 && getColorName(c3) <= 5)) break;
         lineTracing();
      }

      setMotorSpeed(lm, nMotorSpeedSetting);
      setMotorSpeed(rm, nMotorSpeedSetting);
      sleep(400); // EQE?
   }

   setMotorSpeed(lm, 0);
   setMotorSpeed(rm, 0);
   sleep(500);

   setMotorSpeed(lm, nMotorSpeedSetting);
   setMotorSpeed(rm, nMotorSpeedSetting);
   sleep(1000); // ?E?

   // 3. (3,3)EQE?EcE?EEcEcE?E?
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

   // DP ?
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

   eraseDisplay();
   displayBigTextLine(1, "Score = %d", finalScore);

   while( getButtonPress(1) == 0 ) sleep(10);
}