#include <iostream>

using namespace std;

void print(string content)
{
    std::cout << content << endl;
    return;
}

void print(int content)
{
    std::cout << std::to_string(content) << endl;
    return;
}

string input()
{
    string input;
    std::cin >> input;
    return input;
}

void fib(int depth)
{
    int nr1 = 0;
    int nr2 = 1;
    for (int i = 0; i < depth; i++)
    {
        int nr3 = nr1 + nr2;
        print(nr3);
        nr1 = nr2;
        nr2 = nr3;
    }
}

int main()
{
    print("This program prints fibonacci numbers");
    string end = input();
    //fib(200);

    int a = 2147481903;
    for (int i = 0; i < 100000000; i++)
    {
        a += 1;
        if (a < 0)
        {
            std::cout << a << endl;
            std::cout << a - 1 << endl;
            break;
        }
    }

    return 0;
}