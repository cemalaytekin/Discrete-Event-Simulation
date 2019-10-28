main
#include <iostream>
#include <fstream>
#include <iomanip>
#include <vector>
#include <sstream>
#include <algorithm>
#include <iterator>
#include <queue>
#include "Order.h"

using namespace std;
class Cashier{
public:
    bool isFree=true;
    float sum=0;
};

class Barista{
public:
    float sum=0;
    bool isFree=true;
};

class OrderTurnaround{
public:
     float turnaroundTime=0;
};

Cashier *cashiers; // keeps cashiers
Barista *baristas; //keeps baristas
OrderTurnaround *orders; //keeps turnaround Times of each order


template<class Container>
void split1(const string &str, Container &cont) {
    istringstream iss(str);
    copy(istream_iterator<string>(iss),
         istream_iterator<string>(),
         back_inserter(cont));
}

struct compareArrival {
    bool operator()(const Order &l, const Order &r) {
        return l.arrival >= r.arrival;
    }
};

struct comparePrice {
    bool operator()(const Order &l, const Order &r) {
        return l.price < r.price;
    }
};

priority_queue<Order, vector<Order>, compareArrival> pq;
priority_queue<Order, vector<Order>, comparePrice> baristaqPrice;
priority_queue<Order, vector<Order>, compareArrival> cashq;
int cashierNumber;
int totalCashierNumber;
int baristaNumber;
int orderNumber;
float currentTime;
bool isFreeCash;
bool isFreeBar;
int maxCashier=0;
int maxBarista=0;

void firstModel() {
    currentTime = 0;

    while (!pq.empty()) {
        Order o = pq.top();
        pq.pop();
        currentTime = o.arrival;

        if (!o.isCashed) {
            cashq.push(o);

            if(cashq.size()>maxCashier){
                maxCashier = cashq.size();
            }

            isFreeCash=false;
            Cashier c;
            int i;
            for(i = 0; i<cashierNumber; i++){
                c = cashiers[i];
                if(c.isFree) {
                    isFreeCash = true;
                    break;
                }
            }
            if (isFreeCash) {
                Order co = cashq.top();
                cashq.pop();
                cashiers[i].isFree=false;
                co.outCashier = currentTime+ co.order;
                co.arrival = co.outCashier;
                co.isCashed = true;
                co.cashierID = i;
                co.originalCashierArrival=currentTime;
                pq.push(co);
            } else {

            }
        } else if (!o.isBaristed) {
            cashiers[o.cashierID].sum+=currentTime-o.originalCashierArrival;
            if (!cashq.empty()){
                Order newCas = cashq.top();
                cashq.pop();
                newCas.outCashier = currentTime+newCas.order;
                newCas.arrival = newCas.outCashier;
                newCas.isCashed=true;
                newCas.cashierID = o.cashierID;
                newCas.originalCashierArrival=currentTime;
                pq.push(newCas);
            }

            baristaqPrice.push(o);
            if(baristaqPrice.size()>maxBarista) {
                maxBarista = baristaqPrice.size();
            }
            int j;
            Barista b;
            isFreeBar=false;
            for(j = 0; j<baristaNumber; j++){
                b = baristas[j];
                if(b.isFree){
                    isFreeBar=true;
                    break;
                }
            }
            if (isFreeBar) {
                Order bo = baristaqPrice.top();
                baristaqPrice.pop();
                baristas[j].isFree=false;
                bo.outBarista = currentTime + bo.brew;
                bo.arrival = bo.outBarista;
                bo.isBaristed = true;
                bo.originalBaristaArrival=currentTime;
                bo.baristaID = j;
                pq.push(bo);
            } else {
            }
        } else {
            cout << "Kahveniz Hazýr. Price: " << o.price << endl;
            cout << "Islem bitis zamaný: " << currentTime<<endl;
            baristas[o.baristaID].sum+=currentTime-o.originalBaristaArrival;
            cout<<"islemi biten order: "<<o.orderId<<endl;
            cout << endl;
            orders[o.orderId].turnaroundTime=currentTime-o.originalArrival;
            if (!baristaqPrice.empty()) {
                Order newBar = baristaqPrice.top();
                baristaqPrice.pop();
                newBar.outBarista = currentTime + newBar.brew;
                newBar.arrival = newBar.outBarista;
                newBar.isBaristed = true;
                newBar.originalBaristaArrival=currentTime;
                newBar.baristaID = o.baristaID;
                pq.push(newBar);
            }
        }

    }
}

int main() {
    ifstream infile("input.txt");

    string line;
    vector<string> input;
    // process first line
    getline(infile, line);
    totalCashierNumber = stoi(line);
    cashierNumber = totalCashierNumber;

    cashiers = new Cashier[cashierNumber];
    baristas = new Barista[cashierNumber/3];
    baristaNumber = totalCashierNumber / 3;
    cout << "number of cashiers: " << cashierNumber << endl;

    getline(infile, line);
    orderNumber = stoi(line);
    cout << "number of orders: " << orderNumber << endl;
    orders = new OrderTurnaround[orderNumber];
    for (int i = 0; i < orderNumber; i++) {
        getline(infile, line);
        cout << "line: " << line << endl;

        vector<string> words;
        split1(line, words);
        Order order(stof(words[0]), stof(words[1]), stof(words[2]), stof(words[3]));
        pq.push(order);
    }

    firstModel();
    cout<<"maxCashierNumber: "<<maxCashier<<endl;
    cout<<"maxBaristaNumber: "<<maxBarista<<endl;
    for(int i=0; i<cashierNumber; i++){
        cout<<cashiers[i].sum/currentTime<<endl;
    }
    cout<<"######################"<<endl;
    for(int i=0; i<baristaNumber; i++){
        cout<<baristas[i].sum/currentTime<<endl;
    }
    cout<<"######################"<<endl;
    for(int i=0; i<orderNumber; i++){
        cout<<orders[i].turnaroundTime<<endl;
    }
    return 0;
}


Order.h
//
// Created by cemal on 11.11.2017.
//

#ifndef PROJECT2_ORDER_H
#define PROJECT2_ORDER_H


class Order {
public:
    float arrival;
    float order;
    float brew;
    float price;
    float enterCashier;
    float outCashier;
    float enterBarista;
    float outBarista;
    float originalArrival;
    float originalCashierArrival;
    float originalBaristaArrival;
    int orderId;
    int cashierID;
    int baristaID;
    bool isCashed;
    bool isBaristed;
    Order(float arrival, float order, float brew, float price);

};


#endif //PROJECT2_ORDER_H

order.cpp

//
// Created by cemal on 11.11.2017.
//

#include "Order.h"
static int id=0;
Order::Order(float arrival, float order, float brew, float price){
    this->arrival=arrival;
    this->order=order;
    this->brew=brew;
    this->price=price;
    this->originalArrival=arrival;
    this->orderId=id;
    id++;
}

