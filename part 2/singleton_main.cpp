#include "Singleton.cpp"

using namespace std;

void write_to_file(const string data, Singleton<ofstream> *single)
{
    const char *write_data = data.c_str();
    ofstream *stream = single->get_data();
    stream->write(write_data, data.size());
}

int main(int argc, char const *argv[])
{
    ofstream stream;
    stream.open("test.txt", ios::out | ios::in);

    int num = 10;
    thread threads[num];

    for (size_t i = 0; i < num; ++i)
    {
        string data = "Thread " + to_string(i) + "\n";
        Singleton<ofstream> *single = Singleton<ofstream>::Instance(&stream);
        threads[i] = thread(write_to_file, data, single);
    }

    for (size_t i = 0; i < num; ++i)
    {
        threads[i].join();
    }

    return 0;
}