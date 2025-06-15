#ifndef MENU_HPP
#define MENU_HPP

struct Pemain {
    char nama[50];
    Pemain* next;
};

struct Queue {
    Pemain* front;
    Pemain* rear;

    Queue();
    void enqueue(const char* nama);
    void tampilkan();
};

#endif