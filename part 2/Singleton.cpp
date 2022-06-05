#include <iostream>
#include <fstream>
#include <thread>

template <typename T>
class Singleton
{
protected:
    Singleton(T *value)   {
        value_ = new T();
    }

    static Singleton *singleton_;

    T *value_;

public:
    static Singleton *Instance(T *value = nullptr)
    {
        if (singleton_ == nullptr)
        {
            singleton_ = new Singleton(value);
        }

        return singleton_;
    }

    static void Destroy(Singleton *single)
    {
        delete single;
    }

    T *get_data() { return this->value_; }
};

template <typename T>
Singleton<T> *Singleton<T>::singleton_ = nullptr;

