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
    * �Լ��� ���� ������ ���ÿ� �����Ѵ�.
    */
    void enter() {
        LARGE_INTEGER counter;
        QueryPerformanceCounter(&counter);
        _stack.push(counter);
    }

    /**
    * �Լ��� ��������� ������ �ֻ��� �ð��� ���ؼ� �Լ��� ���� �ð��� �����Ѵ�.
    * @return �Լ� ���࿡ �ɸ� �ð�
    */
    double leave() {
        LARGE_INTEGER counter;
        QueryPerformanceCounter(&counter);

        LARGE_INTEGER exCounter = _stack.top();
        _stack.pop();

        // TODO: ������ ����ð��� �ʹ� ª�Ƽ�, �ӽ÷� �ð���� ī���� ���� �״�� ����Ͽ���.
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