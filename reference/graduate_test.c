#pragma config(Sensor, S1, sensorEV3_Color)
#pragma config(Sensor, S2, sensorEV3_Color)
#pragma config(Sensor, S3, sensorEV3_Color)
#pragma config(Motor, motorB, lm, tmotorEV3_Large, PIDControl, encoder)
#pragma config(Motor, motorC, rm, tmotorEV3_Large, PIDControl, encoder)

#define PI 3.14             //  pi
#define WHEEL_DIAMETER 5.6  // wheel diameter in cm
#define MOTOR_SPEED 20      // motor speed

#define LINE_LENGTH 3  // line length in cm
#define WIDTH 5        // width of the map
#define HEIGHT 4       // height of the map

#define INF -1000000000  // infinity

enum COLOR { BLACK = 1, BLUE, GREEN, YELLOW, RED, WHITE, BROWN };  //  Color
enum DIRECTION { UP, LEFT, DOWN, RIGHT };                          //  Direction

int map[HEIGHT][WIDTH];	// Map

int dx[] = {-1, 0, 1, 0};
int dy[] = {0, -1, 0, 1};

int node[HEIGHT][WIDTH];          // Node
int visited[HEIGHT][WIDTH];       // Visited
int robotVisited[HEIGHT][WIDTH];  // Robot visited

// --- Utility ---
float distance2Encoder(float distance);  //  Convert distance to encoder value
int getMax(int a, int b);                //  Get max
int getWeight(int x, int y);             //  Get weight
void searchBestWeight(int x, int y);     //  Search best weight

// --- Movement ---
void go(int speed);                           //  Go forward
void moveByDistance(float distance);          //  Move forward by distance
void pointTurn(int angle, bool isClockwise);  //  Point turn
void goUntilIntersection();                   //  Go until intersection

int goHigh();                             //  Go high
void move2Node(int direction, int head);  //  Move to node

// --- Sensor ---
void setIntersectionColor(int x, int y);  //  Set map color
void showMap();                           //  Show map

// --- Search ---
void resetNode();  //  Reset visit

task main() {
    setIntersectionColor(0, 0);
    moveByDistance(LINE_LENGTH);
    for (int i = 0; i < HEIGHT; i++) {
        for (int j = 1; j < WIDTH; j++) {
            goUntilIntersection();
            setIntersectionColor(i, i % 2 ? WIDTH - 1 - j : j);
            moveByDistance(LINE_LENGTH);
        }
        if (i != HEIGHT - 1) {
            moveByDistance(LINE_LENGTH / 2);
            pointTurn(180, !(i % 2));

            goUntilIntersection();
            setIntersectionColor(i + 1, i % 2 ? 0 : WIDTH - 1);
            moveByDistance(LINE_LENGTH);

            moveByDistance(LINE_LENGTH / 2);
            pointTurn(180, !(i % 2));
        }
    }

    pointTurn(360, true);
    for (int i = 0; i < WIDTH - 1; i++) {
        goUntilIntersection();
        moveByDistance(LINE_LENGTH);
    }

    moveByDistance(LINE_LENGTH / 2);
    pointTurn(180, false);

    showMap();
    while (!getButtonPress(1))
        ;
    int score = goHigh();
    displayTextLine(1, "Score: %d", score);
    while (!getButtonPress(1))
        ;
}

float distance2Encoder(float distance) {
    return (360 * distance) / (WHEEL_DIAMETER * PI);
}

int getMax(int a, int b) { return a > b ? a : b; }

void resetNode() {
    for (int i = 0; i < HEIGHT; i++)
        for (int j = 0; j < WIDTH; j++) {
            node[i][j] = INF;
            visited[i][j] = 0;
        }
    node[0][0] = 0;
}

int getWeight(int x, int y) {
    int color = map[x][y];

    if (color == RED) {
        if (robotVisited[x][y] || visited[x][y])
            return -2;
        else
            return 5;
    } else if (color == GREEN)
        return -5;
    else
        return 0;
}

void go(int speed) {
    setMotorSpeed(lm, speed);
    setMotorSpeed(rm, speed);
}

void moveByDistance(float distance) {
    float encoderDegree = distance2Encoder(distance);

    resetMotorEncoder(lm);
    resetMotorEncoder(rm);

    setMotorTarget(lm, encoderDegree, MOTOR_SPEED);
    setMotorTarget(rm, encoderDegree, MOTOR_SPEED);

    waitUntilMotorStop(lm);
    waitUntilMotorStop(rm);
}

void pointTurn(int angle, bool isClockwise) {
    resetMotorEncoder(lm);
    resetMotorEncoder(rm);

    setMotorTarget(lm, isClockwise ? angle : -angle, MOTOR_SPEED);
    setMotorTarget(rm, isClockwise ? -angle : angle, MOTOR_SPEED);

    waitUntilMotorStop(lm);
    waitUntilMotorStop(rm);
}

void goUntilIntersection() {
    while (1) {
        if (getColorName(S2) == YELLOW) {
            setMotorSpeed(lm, MOTOR_SPEED / 2);
            setMotorSpeed(rm, MOTOR_SPEED);
        } else {
            setMotorSpeed(lm, MOTOR_SPEED);
            setMotorSpeed(rm, MOTOR_SPEED / 2);
        }

        if (getColorName(S1) == YELLOW || getColorName(S3) == YELLOW) {
            go(0);
            break;
        }
    }
}

int goHigh() {
    int result = 1;
    int x = HEIGHT - 1, y = WIDTH - 1;
    int head = UP;

    while (1) {
        result += getWeight(x, y) - 1;

        if (x == 0 && y == 0) break;

        resetNode();
        robotVisited[x][y]++;
        visited[x][y]++;

        searchBestWeight(0, 0);

        int max = INF;
        int dir = -1;
        for (int i = 0; i < 4; i++) {
            int nx = x + dx[i];
            int ny = y + dy[i];
            if (nx < 0 || ny < 0 || nx >= HEIGHT || ny >= WIDTH) continue;

            if (max < node[nx][ny]) {
                max = node[nx][ny];
                dir = i;
            }
        }

        move2Node(dir, head);

        x += dx[dir];
        y += dy[dir];
        head = dir;
    }

    return result;
}

void move2Node(int direction, int head) {
    int turn = (direction - head + 4) % 4;
    moveByDistance(LINE_LENGTH);
    pointTurn(turn ? (turn % 2 ? 180 : 360) : 0, turn == 3);

    goUntilIntersection();
    moveByDistance(LINE_LENGTH);
}

void setIntersectionColor(int x, int y) {
    resetMotorEncoder(lm);
    resetMotorEncoder(rm);

    setMotorTarget(lm, 40, MOTOR_SPEED);
    waitUntilMotorStop(lm);

    map[x][y] = getColorName(S2);

    setMotorTarget(rm, 40, MOTOR_SPEED);
    waitUntilMotorStop(rm);
}

void showMap() {
    for (int i = 0; i < HEIGHT; i++)
        for (int j = 0; j < WIDTH; j++) {
            char ch;
            if (map[i][j] == YELLOW)
                ch = '+';
            else if (map[i][j] == RED)
                ch = 'O';
            else if (map[i][j] == GREEN)
                ch = 'X';
            displayStringAt((j + 1) * 10, 100 - (i + 1) * 10, "%c", ch);
        }
}

void searchBestWeight(int x, int y) {
    for (int i = 0; i < 4; i++) {
        int nx = x + dx[i];
        int ny = y + dy[i];
        if (nx < 0 || ny < 0 || nx >= HEIGHT || ny >= WIDTH) continue;
        if (nx == 0 && ny == 0) continue;

        int weight = getWeight(nx, ny) - 1;

        visited[x][y]++;

        if (node[nx][ny] == INF || node[nx][ny] < weight + node[x][y]) {
            node[nx][ny] = weight + node[x][y];
            searchBestWeight(nx, ny);
        }

        visited[x][y]--;
    }
}
