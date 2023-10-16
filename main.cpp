#include <iostream>
#include <chrono>
#include <ctime>
#include "sha.h"
#include <unistd.h>
#include <termios.h>
using namespace std;

char getch(void)
{
    char buf = 0;
    struct termios old = {0};
    fflush(stdout);
    if(tcgetattr(0, &old) < 0)
        perror("tcsetattr()");
    old.c_lflag &= ~ICANON;
    old.c_lflag &= ~ECHO;
    old.c_cc[VMIN] = 1;
    old.c_cc[VTIME] = 0;
    if(tcsetattr(0, TCSANOW, &old) < 0)
        perror("tcsetattr ICANON");
    if(read(0, &buf, 1) < 0)
        perror("read()");
    old.c_lflag |= ICANON;
    old.c_lflag |= ECHO;
    if(tcsetattr(0, TCSADRAIN, &old) < 0)
        perror("tcsetattr ~ICANON");
    //printf("%c\n", buf);
    return buf;
 }

int main(){
    cout << "Welcome!!!" << endl;
    char ch;
    //const char ENTER = 13;
    string input1, input2, output1, output2;
    SHA256 in, check;
    bool ex = false;
    while (ex == false){
        cout << "Input to hash (type 'q' to quit): ";
        ch = getch();
        while(ch != 10){
            if (ch != 0) {
                input1 += ch;
                cout << '*';
            }
            ch = getch();
        }
        cout << endl;
        if (input1 != "q"){
            in.update(input1);
            uint8_t * digest1 = in.digest();
            output1 = SHA256::toString(digest1);
            cout << "Hashed: " << output1 << endl;
            cout << "To confirm: ";
            ch = getch();
            while(ch != 10){
                if (ch != 0) {
                    input2 += ch;
                    cout << '*';
                }
                ch = getch();
            }
            cout << endl;
            check.update(input2);
            uint8_t * digest2 = check.digest();
            output2 = SHA256::toString(digest2);
            cout << "Hashed: " << output2 << endl;
            if (output1 == output2){
                cout << "Great! You are good at remembering your password." << endl;

            }
            else {
                cout << "Inputs didn't match!!" << endl;
            }
            delete[] digest1;
            delete[] digest2;
        }
        else{
            ex = true;
        }
    }
    cout << "Enjoy!!" << endl;
    return 0;
}