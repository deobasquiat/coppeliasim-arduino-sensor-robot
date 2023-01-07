#include <Arduino.h>

// Define Tags
#define SW_UP 11
#define SW_RIGHT 10
#define SW_LEFT 9
#define SW_DOWN 8
#define Forward "u"
#define Right "r"
#define Left "l"
#define Backwards "d"
#define Move "m"


// Structs
struct speed
{

    char sMove; // Upwards Speed
    char sRight; // Right Wheel Speed
    char sLeft; // Left Wheel Speed
    char sDown; // Downwards Speed

};

// Arrays
char sensor_array[7];

// Global Variables
int SW_UP_Status = 0;
int SW_RIGHT_Status = 0;
int SW_LEFT_Status = 0;
int SW_DOWN_Status = 0;
speed  Robot_Speed = { 0, 0 };
double Left_Sensor = 1.0;
double  Right_Sensor = 1.0;
int received_data;
String data;
bool controlLeft = false;
bool controlRight = false;
bool controlStable = false;

// Prototypes
void Robot_Move(int velr, int vell);
void read_Sensors();
void readings();
void forwards();
void backwards();
void right();
void left();
void remote_control();
void stable();
void automatic();
void selfTurn();

void Robot_Move(int velr, int vell)
{

    int sinal = velr * vell;

    if (sinal == 0)
    {
        Serial.print('m');
        Serial.print(velr);
        Serial.print(vell);
        return;
    }

    if (sinal > 0)
    {
        if (vell > 0 || velr > 0)
        {
            Serial.print('m');
            Serial.print(velr);
            Serial.print(vell);
        }
        if (vell < 0 || velr < 0)
        {
            Serial.print('n');
            Serial.print('-');
            Serial.print(abs(velr));
            Serial.print('-');
            Serial.print(abs(vell));
        }
    } else {
        if (vell > 0)
        {
            Serial.print('m');
            Serial.print(vell);
            Serial.print('-');
            Serial.print(abs(velr));
        }
        if (velr > 0)
        {
            Serial.print('m');
            Serial.print(velr);
            Serial.print('-');
            Serial.print(abs(vell));
        }
    }

    // Robot_Speed.sRight = velr;
    // Robot_Speed.sLeft = vell;

    // Serial.print("m");
    // Serial.print(Robot_Speed.sRight);
    // Serial.print(Robot_Speed.sLeft);

}

void read_Sensors()
{
    if (Serial.available() > 0) // Test if serial has something to read or not
    {
        received_data = Serial.read();
        if (char (received_data) == 'L')
        {
            for (int i = 0; i < 7; i++)
            {
                if (Serial.available() > 0) {
                    received_data = Serial.read();
                    // sensor_array[i] = char(received_data);
                    data += char(received_data);
                } else {
                    i--;
                }
            }
            Left_Sensor = data.toDouble();
            // Serial.print(Left_Sensor);
            data = "";

        }

        if (char (received_data) == 'R')
        {
            for (int i = 0; i < 7; i++)
            {
                if (Serial.available() > 0) {
                    received_data = Serial.read();
                    // sensor_array[i] = char(received_data);
                    data += char(received_data);
                } else {
                    i--;
                }
            }
            Right_Sensor = data.toDouble();
            // Serial.print(Right_Sensor);
            data = "";

        }
    }
}

void readings()
{
    // Move Up
    SW_UP_Status = digitalRead(SW_UP);

    // Move Right
    SW_RIGHT_Status = digitalRead(SW_RIGHT);

    // Move Left
    SW_LEFT_Status = digitalRead(SW_LEFT);

    // Move Down
    SW_DOWN_Status = digitalRead(SW_DOWN);
}

void forwards()
{
    if (SW_UP_Status == 1)
    {
        Robot_Move(9, 9);
        while (SW_UP_Status == 1)
        {
            readings();
        }
    }
}

void backwards()
{
    if (SW_DOWN_Status == 1)
    {
        Robot_Move(0, 0);
        while (SW_DOWN_Status == 1)
        {
            readings();
        }
    }
}

void right()
{
    if (SW_RIGHT_Status == 1)
    {
        Robot_Move(3, 5);
        while (SW_RIGHT_Status == 1)
        {
            readings();
        }
        Robot_Move(9, 9);
    }
}

void left()
{
    if (SW_LEFT_Status == 1)
    {
        Robot_Move(5, 3);
        while (SW_LEFT_Status == 1)
        {
            readings();
        }
        Robot_Move(9, 9);
    }
}

void remote_control()
{
    // Move Up
    SW_UP_Status = digitalRead(SW_UP);

    if (SW_UP_Status == 1)
    {
        forwards();
        // Serial.write(Forward, 2);
        // Robot_Move(5, 5);
        // while (SW_UP_Status == 1)
        // {
        //     // readings();
        //     SW_UP_Status = digitalRead(SW_UP);
        // }
    }

    // Move Right
    SW_RIGHT_Status = digitalRead(SW_RIGHT);

    if (SW_RIGHT_Status == 1)
    {
        right();
        // Serial.write(Right, 2);
        // Robot_Move(4, 5);
        // while (SW_RIGHT_Status == 1)
        // {
        //     // readings();
        //     SW_RIGHT_Status = digitalRead(SW_RIGHT);
        // }
        // Robot_Move(5, 5);
    }

    // Move Left
    SW_LEFT_Status = digitalRead(SW_LEFT);

    if (SW_LEFT_Status == 1)
    {
        left();
        // Serial.write(Left, 2);
        // Robot_Move(5, 4);
        // while (SW_LEFT_Status == 1)
        // {
        //     // readings();
        //     SW_LEFT_Status = digitalRead(SW_LEFT);
        // }
        // Robot_Move(5, 5);
    }

    // Move Down
    SW_DOWN_Status = digitalRead(SW_DOWN);

    if (SW_DOWN_Status == 1)
    {
        backwards();
        // Serial.write(Backwards, 2);
        // Robot_Move(-5, -5);
        // while (SW_DOWN_Status == 1)
        // {
        //     // readings();
        //     SW_DOWN_Status = digitalRead(SW_DOWN);
        // }
    }
    // if (SW_DOWN_Status == 1 && digitalRead(SW_UP) == 1)
    // {
    //     Robot_Move(0, 0);
    //     downwards();
    // }

    // if (SW_UP_Status == 1 && digitalRead(SW_DOWN) == 1)
    // {
    //     Robot_Move(0, 0);
    //     upwards();
    // }
}

void stable()
{

    if (Left_Sensor == 1.0 && Right_Sensor == 1.0 && controlStable == false)
    {
        Robot_Move(9, 9);
        // Serial.print("Stable");
        controlStable = true;
    }

}

// Work In Progress
void automatic ()
{

    if (Left_Sensor < 0.30 && controlLeft == false)
    {
        Robot_Move(4, 5);
        controlLeft = true;
    }

    if (Right_Sensor < 0.30 && controlRight == false)
    {
        Robot_Move(5, 4);
        controlRight = true;
    }

}

void selfTurn()
{

}

void setup()
{

    // Arduino Input Pins Config
    pinMode(11, INPUT); //SW_UP
    pinMode(10, INPUT); //SW_RIGHT
    pinMode(9, INPUT); //SW_LEFT
    pinMode(8, INPUT); //SW_DOWN

    // Config of Serial Comunication
    Serial.begin(9600);

}

void loop()
{

    remote_control();
    read_Sensors();
    stable();
    automatic();

}