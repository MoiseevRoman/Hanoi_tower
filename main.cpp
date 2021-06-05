#include <iostream>
#include <iostream>
#include <windows.h>
#include <locale>
#include <conio.h>
#include <limits>
#include <stack>

#include "conutils.h"

void draw_rod(int x, int y) {
    int x_ = x;
    int y_ = y;
    for (int i = 0; i < 10; i++) {
        console_gotoxy(x_, y_);
        std::cout << '|';
        y_ ++;
    }
}

class Disk {
public:
    int x_;
    int y_;
    int length_;
    Disk( int x, int y,int length) {
        draw_d( x, y, length);
    };
    Disk* prev_;

    void draw_d(int x, int y, int length) {     //Disk's draw
        char ch = 220;
        int x_d = x+1, y_d = y;
        length_ = length;

        for (int i = 0; i < length/2; i++) {
            console_gotoxy(x_d, y_d);
            std::cout << ch;
            x_d++;
        }
        x_d = x;
        for (int i = 0; i < length/2; i++) {
            x_d--;
            console_gotoxy(x_d, y_d);
            std::cout << ch;
        }
        x_ = x; y_ = y;         //Disk's coord

    };
};


class Stack {                 // Tower_Stack
    Disk* tos_ = nullptr;

public:
    int size_s;

    void push( int x, int y, int length) {
        if (tos_ == nullptr) {
            tos_ = new Disk( x, y, length);
        }
        else {
            Disk* ptr = new Disk( x, y,length);
            ptr->prev_ = tos_;
            tos_ = ptr;
        }
        size_s++;
    };

    Disk* pop() {
        Disk* ptr = tos_;
        tos_ = ptr->prev_;
        size_s--;
        return ptr;
    };

    Disk* peek() {
        return tos_;
    };

    int size_stack(){
        return size_s;}
};

class Tower {
    int x_s = 15;
    int y_s = 15;


    Stack* s_ptr = nullptr;
    Disk* d_ptr = nullptr;

public:
    Tower(int num_, Stack *steck) {
        while (num_ > 0) {
            console_gotoxy(x_s, y_s);
            steck->push(x_s, y_s, num_ * 2);
            num_--;
            y_s--;
        }
    };
};

class Game {

public:
    int x_ = 15;
    int y_;
    int size_;
    Stack* new_tower;

    Disk* d_ptr = nullptr;
    Stack* s_ptr = nullptr;
    Game(Stack *steck1) {            //Ustanovlenie na4ala igri na 1 piramidu
        s_ptr = steck1;
        DrawPointer(x_);
        size_ = s_ptr->size_s;
    };
    void DrawPointer(int x, int y = 17) {
        char ptr = 30;
        console_gotoxy(x, y);
        std::cout << ptr;
    }

    void RemovePointer(int x, int y = 17) {
        console_gotoxy(x, y);
        std::cout << ' ';
    }

    void Move(Disk *disk_ch, int x, int y) {
        int len = disk_ch->length_;
        disk_ch->draw_d(x, y,len);
    }
    void ReMove(Disk *disk_ch, int x, int y) {
        int len = disk_ch->length_;
        char ch = ' ';

        int x_d = (disk_ch -> x_) + 1, y_d = disk_ch -> y_;

        for (int i = 0; i < len / 2; i++) {
            console_gotoxy(x_d, y_d);
            std::cout << ch;
            x_d++;
        }
        x_d = x;
        for (int i = 0; i < len / 2; i++) {
            x_d--;
            console_gotoxy(x_d, y_d);
            std::cout << ch;
        }
    }


    void Start(Stack *steck1, Stack *steck2, Stack *steck3) {
        new_tower = steck3;

        console_hide_cursor();
        WORD vk;
        bool isFinish = false;
        bool isUp = false;
        bool isDown = true;

        while (!isFinish) {
            if (_kbhit()) {
                vk = console_get_vk();

                switch (vk) {
                case VK_UP:
                    if (isUp) {   // Podnimaem vverx
                        break;
                    }

                    if (s_ptr != nullptr) {
                        Disk* d_ch = s_ptr->peek();
                        if (d_ch == nullptr)
                            break;
                        d_ptr = s_ptr->pop();
                        ReMove(d_ptr, x_, y_);
                        y_ = 4;
                        Move(d_ptr, x_, y_);
                        isUp = true;
                        isDown = false;
                    }

                    break;
                case VK_DOWN: // Opuskaem disk vniz
                    if (isDown)
                        break;
                    if (s_ptr != nullptr && d_ptr != nullptr) {
                        Disk *prev_disk = s_ptr->peek();
                        if (prev_disk == nullptr) {
                            int len = d_ptr->length_;
                            int x_d = d_ptr->x_;int y_d = d_ptr->y_;
                            ReMove(d_ptr, x_, y_);
                            s_ptr->push( x_d, 15, len);
                            d_ptr = nullptr;
                            isDown = true;
                            isUp = false;
                            break;
                        }
                        else {
                            if (prev_disk == nullptr)
                                break;
                            int prev_len = prev_disk->length_;
                            int len = d_ptr->length_;
                            if (prev_len > len) {
                                int x_d = prev_disk->x_; int y_d = prev_disk->y_;
                                ReMove(d_ptr, x_, y_);
                                x_ = x_d; y_ = y_d - 1;
                                Move(d_ptr, x_, y_);
                                s_ptr->push(x_, y_, len);
                                d_ptr = nullptr;
                                isUp = false;
                            }
                        }
                    }


                    break;
                case VK_LEFT:           // Kursor vlevo
                    if (d_ptr == nullptr) {
                        if (x_ > 15) {
                            RemovePointer(x_);
                            x_ = x_ - 20;
                            DrawPointer(x_);
                        }
                        switch (x_) {
                        case 15:
                            s_ptr = steck1;
                        break;

                        case 35:
                            s_ptr = steck2;
                        break;
                        }
                    }
                    else
                    {
                        if (x_ > 15) {
                            RemovePointer(x_);
                            ReMove(d_ptr, x_, y_);
                            x_ = x_ - 20;
                            DrawPointer(x_);
                        }
                        Move(d_ptr, x_, y_);
                        switch (x_) {
                        case 15:
                            s_ptr = steck1;
                        break;

                        case 35:
                            s_ptr = steck2;
                        break;
                        }
                    }

                    break;
                case VK_RIGHT:            //Kursor vpravo
                    if (d_ptr == nullptr) {
                        if (x_ < 55) {
                            RemovePointer(x_);
                            x_ = x_ + 20;
                            DrawPointer(x_);
                        }
                        switch (x_) {
                        case 35:
                            s_ptr = steck2;
                        break;

                        case 55:
                            s_ptr = steck3;
                        break;

                        }
                    }
                    else {
                        if (x_ < 55) {
                            RemovePointer(x_);
                            ReMove(d_ptr, x_, y_);
                            x_ = x_ + 20;
                            DrawPointer(x_);
                        }
                        Move(d_ptr, x_, y_);
                        switch (x_) {
                        case 35:
                            s_ptr = steck2;
                        break;

                        case 55:
                            s_ptr = steck3;
                        break;
                        }
                    }

                    break;
                case VK_ESCAPE:              //Vixod iz igri
                    console_gotoxy(15, 8);
                    std::cout << " \t\t\t\tGAME OVER ";
                    isFinish = true;
                break;
                }
            }
            int new_size = new_tower->size_s;
            if (size_ == new_size) {
                isFinish = true;
                console_gotoxy(15, 8);
                std::cout << " \t\t\t\tYOU WIN ";
            }
        }
    }
};

int main()
{
    int disks_quan = 0;
    int st= 0;
    while (st == 0){
        system("cls");
        std::cout << "Enter number of disks 3-8: ";
        std::cin >> disks_quan;
        if (disks_quan < 3) {
            std::cout << "Enter more disks";
        }
        else if (disks_quan > 8) {
            std::cout << "Too much";
        }
        else if(disks_quan<9&&disks_quan>2){
            st++;
        }
    }

    draw_rod(15, 6);
    draw_rod(35, 6);
    draw_rod(55, 6);

    Stack *s1 = new Stack;
    Tower t (disks_quan, s1);

    Stack* s2 = new Stack;
    Stack* s3 = new Stack;

    std::cout << std::endl;
    Game game(s1);
    game.Start(s1, s2, s3);
}

