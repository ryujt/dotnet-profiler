#pragma once

#include <windows.h>
#include <stack>

using namespace std;

class PerformanceCounter {
public:
    static PerformanceCounter& getIncetance() {
        static PerformanceCounter instance;
        return instance;
    }

    /**
    * 함수의 시작 시점을 스택에 저장한다.
    */
    void enter() {
        LARGE_INTEGER counter;
        QueryPerformanceCounter(&counter);
        _stack.push(counter);
    }

    /**
    * 함수의 종료시점과 스택의 최상위 시간과 비교해서 함수의 실행 시간을 리턴한다.
    * @return 함수 실행에 걸린 시간
    */
    double leave() {
        LARGE_INTEGER counter;
        QueryPerformanceCounter(&counter);

        LARGE_INTEGER exCounter = _stack.top();
        _stack.pop();

        // TODO: 예제의 실행시간이 너무 짧아서, 임시로 시간대신 카운터 수를 그대로 사용하였다.
//        return (counter.QuadPart - exCounter.QuadPart) / _frequency.QuadPart;
        return (counter.QuadPart - exCounter.QuadPart);
    }

private:
    PerformanceCounter() {
        QueryPerformanceFrequency(&_frequency);
    }

    PerformanceCounter(const PerformanceCounter& ref) {
        QueryPerformanceFrequency(&_frequency);
    }

    PerformanceCounter& operator=(const PerformanceCounter& ref) {}
    ~PerformanceCounter() {}

    LARGE_INTEGER _frequency;
    stack<LARGE_INTEGER> _stack;
};