#include <iostream>
#include <thread>
#include <mutex>
#include <vector>

class Guard
{
private:
    std::mutex &mut;

public:
    Guard(std::mutex &mut) : mut(mut)
    {
        mut.lock();
    }
    ~Guard()
    {
        mut.unlock();
    }
};


using namespace std;

mutex mut;

int global_num = 0;
vector<int> vec;

void add_safe(int idx, int data)
{
    const Guard guard(mut);
    int last = global_num;

    for (size_t i = 0; i < data; i++)
    {
        global_num++;
    }

    cout << "thread - " << idx << ", ";
    cout << last << " + " << data << " = " << global_num << endl;
    vec.push_back(global_num);
}

void add(int idx, int data)
{
    int last = global_num;
    for (size_t i = 0; i < data; i++)
    {
        global_num++;
    }

    cout << "thread - " << idx << ", ";
    cout << last << " + " << data << " = " << global_num << endl;
    vec.push_back(global_num);
}

void print_vec(vector<int> vec)
{
    for (int num : vec)
    {
        cout << num << endl;
    }
}

int main(int argc, char const *argv[])
{
    int num = 10;
    cout << "add with guard: " << endl;
    thread threads2[num];

    for (size_t i = 0; i < num; ++i)
    {
        threads2[i] = thread(add_safe, i, (int)(i * 1000));
    }

    for (size_t i = 0; i < num; ++i)
    {
        threads2[i].join();
    }

    cout << endl;
    cout << "add without guard: " << endl;

    thread threads[num];
    vec.clear();
    global_num = 0;

    for (size_t i = 0; i < num; ++i)
    {
        threads[i] = thread(add, i, (int)(i * 1000));
    }

    for (size_t i = 0; i < num; ++i)
    {
        threads[i].join();
    }
    
    return 0;
}
