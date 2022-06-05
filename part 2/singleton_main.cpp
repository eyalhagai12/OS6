#include "Singleton.cpp"
using namespace std;

void write_to_file(const string data, Singleton<ofstream> *single)
{
 // guard can be added here   
    const char *write_data = data.c_str();
    ofstream *stream = single->get_data();
    *stream << write_data;
}

int main(int argc, char const *argv[])
{
    Singleton <ofstream> * single = Singleton<ofstream>::Instance();

    int num = 10;
    thread threads[num];
    single->get_data()->open("test.txt", ios::out | ios::in);

    for (size_t i = 0; i < num; ++i)
    {
        string data = "Thread " + to_string(i) + "\n";
        threads[i] = thread(write_to_file, data, single);
    }

    for (size_t i = 0; i < num; ++i)
    {
        threads[i].join();
    }
    single->get_data()->close();

    return 0;
}