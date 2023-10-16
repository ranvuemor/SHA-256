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

string gen_random(const int len) {
    static const char alphanum[] =
        "0123456789"
        "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
        "abcdefghijklmnopqrstuvwxyz";
    string tmp_s;
    tmp_s.reserve(len);

    for (int i = 0; i < len; ++i) {
        tmp_s += alphanum[rand() % (sizeof(alphanum) - 1)];
    }
    
    return tmp_s;
}

int main(){
    cout << endl  << "Welcome!!!" << endl << "==========" << endl;
    char ch;
    //const char ENTER = 13;
    bool ex = false;
    while (ex == false){
        string input1, input2, output1, output2;
        SHA256 in, check;
        srand((unsigned)time(NULL) * getpid());  
        string salt = gen_random(12);
        cout << "Input to hash (type 'q' to quit): ";
        ch = 0;
        while(ch != 10){
            if (ch != 0) {
                input1 += ch;
                cout << '*';
            }
            ch = getch();
        }
        cout << endl;
        if (input1 == "q"){
            ex = true;
            break;
        }
        else{
            input1 = salt + input1;
            in.update(input1);
            uint8_t * digest1 = in.digest();
            output1 = SHA256::toString(digest1);
            //output1 = salt + output1;
            cout << "Hashed: " << output1 << endl << endl;
            cout << "To confirm: ";
            ch = 0;
            while(ch != 10){
                if (ch != 0) {
                    input2 += ch;
                    cout << '*';
                }
                ch = getch();
            }
            input2 = salt + input2;
            cout << endl;
            check.update(input2);
            uint8_t * digest2 = check.digest();
            output2 = SHA256::toString(digest2);
            //output2 = salt + output2;
            cout << "Hashed: " << output2 << endl << endl;
            if (output1 == output2){
                cout << "Inputs matched!! You are good at remembering your passwords." << endl << endl;
                //cout << input1 << "  " << input2 << endl;
            }
            else {
                cout << "Inputs didn't match!!" << endl << endl;
                //cout << input1 << "  " << input2 << endl;
            }
            delete[] digest1;
            delete[] digest2;
        }
    }
    cout << endl << "Enjoy!!" << endl;
    return 0;
}