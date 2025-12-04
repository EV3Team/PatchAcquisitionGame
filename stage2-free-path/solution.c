#pragma config(Sensor, S1,      c1,             sensorEV3_Color)
#pragma config(Sensor, S2,      c2,             sensorEV3_Color)
#pragma config(Sensor, S3,      c3,             sensorEV3_Color)
#pragma config(Motor,  motorB,  lm,             tmotorEV3_Large, PIDControl, driveLeft, encoder)
#pragma config(Motor,  motorC,  rm,             tmotorEV3_Large, PIDControl, driveRight, encoder)

// [?

int limitRow = 4;
int limitColumn = 5;

int nMotorSpeedSetting = 15, vertex = 0, count = 0, row = 0, val, r = 1, c = 1,;

int S[6][6], dt[6][6];

//DFS
// [DFS ¾E°i?
int visitedSim[6][6];       // ½A¹A
int bestPath[30];           // AOA¾AuA¸·I A￡Aº A
int tempPath[30];           // CoAc A?
int maxScore = -10000;      // ¹ß?
int bestStepCount = 0;      // AO°i A¡
const int MAX_DEPTH = 10;   // AO´e A½≫o ?


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


// --- [DFS ¹eÆ®·?
void dfs(int x, int y, int currentScore, int steps)
{
    // 1. ±aAu A¶?
    if (x == 0 && y == 0)
    {
        // CoAc A¡¼o°¡ Ao?
        if (currentScore > maxScore)
        {
            maxScore = currentScore;
            bestStepCount = steps;
            for(int i=0; i<steps; i++) bestPath[i] = tempPath[i]; // °
        }
        return;
    }

    // 2. °¡AoA¡±a: ³
    if (steps >= MAX_DEPTH) return;

    // 3. 4¹æCa A½≫o (¼ø¼­: ?
    int dx[] = {-1, 0, 1, 0}; // ≫o, AA
    int dy[] = {0, -1, 0, 1}; // ≫o, AA

    for (int i = 0; i < 4; i++)
    {
        int nx = x + dx[i];
        int ny = y + dy[i];

        // ¸E ¹U
        if (nx < 0 || nx >= limitRow || ny < 0 || ny >= limitColumn) continue;

        // A¡¼o
        int moveCost = -1; // AI
        int patchScore = 0;

        // ÆÐA¡
        if (S[nx][ny] == 1) // ≫
        {
             if (visitedSim[nx][ny] == 0) patchScore = 5; // A¹
             else patchScore = -2; // Ac?
        }
        else if (S[nx][ny] == -1) // ÆA¶
        {
             patchScore = -5; // ¹?
        }

        // [Aß¿a] AI¹I ¹æ¹®
        // CIAo¸¸ ¹≪CN ≫±≫?
        if (visitedSim[nx][ny] >= 2) continue;

        // ≫oA
        visitedSim[nx][ny]++;
        tempPath[steps] = i; // ¹
        displayBigTextLine(4, "curr_step = %d", steps);

        // Ac±I E￡
        dfs(nx, ny, currentScore + moveCost + patchScore, steps + 1);

        // ¿ø≫o
        visitedSim[nx][ny]--;
    }
}

// --- [A￡Aº °æ?
void executePath()
{
    // ·Iº¿Aº CoAc (limitRow-1, limitColumn-1)¿?
    // AAC￥ ±aAØ: ≫o(-1, 0), AA(0, -1), C

    for(int i=0; i<bestStepCount; i++)
    {
        int dir = bestPath[i]; // 0:≫o, 1:AA, 2

        // CoAc ·Iº¿Aº C×≫o
        // ±aAØ: ¿ÞAE(W

        if (dir == 1) // AA (Left) -
        {
            // ±
            c--; // AA
        }
        else if (dir == 0) // ≫o (Up) - ¿A¸￥
        {
            turnRight();
            sleep(200);
            r--; // AA
        }
        else if (dir == 2) // CI (Down) - ¿ÞAEA
        {
            turnLeft();
            sleep(200);
            r++;
        }
        else if (dir == 3) // ¿i (Right) - μU·I
        {
            // 180?
            setMotorSpeed(lm, nMotorSpeedSetting);
            setMotorSpeed(rm, -nMotorSpeedSetting);
            sleep(1400); // 180μμ AI ?
            stopMotor();
            sleep(200);
            c++;
        }

        // ½CA| AI
        // ±³A÷·I±iA
        count = 0;
        while(true)
        {
            go(); // ¶oAI?
            if(count == 1) // ±?
            {
                 if(r==0 && c==0 && i == bestStepCount-1) {
                     // AOA¾ ?
                     stopMotor();
                     break;
                 }
                 // ±³A÷·I Ae°u ¹×
                 setMotorSpeed(lm, nMotorSpeedSetting);
                 setMotorSpeed(rm, nMotorSpeedSetting);
                 sleep(1100); // **Aß¿a: ·Iº¿
                 break;
            }
        }

        // AIμ¿ EA ´U½A '?
        if (dir == 0) // ¾Æ±i A§·I °￢A¸?
        {
            turnLeft();
            sleep(100);
        }
        else if (dir == 2) // ¾Æ±i ¾Æ·¡·I
        {
            turnRight();
            sleep(100);
        }
        else if (dir == 3) // ¾Æ±i ¿A¸￥AE °
        {
            setMotorSpeed(lm, nMotorSpeedSetting);
            setMotorSpeed(rm, -nMotorSpeedSetting);
            sleep(1400);
            stopMotor();
        }
        // dir == 1 (AA)AI ¶§´A A

        // E­¸e¿
        eraseDisplay();
        displayBigTextLine(1, "Step: %d/%d", i+1, bestStepCount);
        displayBigTextLine(4, "Pos: %d, %d", r, c);
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

   playTone(440, 10);

    // 1. DFS º??
    maxScore = -10000;
    bestStepCount = 0;
    for(int i=0; i<limitColumn; i++)
        for(int j=0; j<limitRow; j++) visitedSim[i][j] = 0;

    // ½AAUA¡ (3,3) ¹æ¹® A³¸® (li
    visitedSim[limitRow-1][limitColumn-1] = 1;

    // 2. DFS ½CCa (A
    // AIAU: CoAcr, CoAcc,
    dfs(limitRow-1, limitColumn-1, 0, 0);

    // °e?
    playTone(880, 20);
    sleep(500);
    eraseDisplay();
    displayBigTextLine(1, "Go!");
    displayBigTextLine(4, "MaxScore: %d", maxScore);

    // AAC￥ AE±
    r = limitRow - 1;
    c = limitColumn - 1;

    // 3. °e≫e
    executePath();


    // 4. AOA?
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
