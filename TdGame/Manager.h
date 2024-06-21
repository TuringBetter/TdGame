#pragma once
#include <memory>
#include <thread>
#include <mutex>

template <typename T>
class Manager
{
protected:
	Manager() = default;
    ~Manager() = default;
	Manager(const Manager<T>&) = delete;
	Manager& operator= (const Manager<T>& st) = delete;
	static std::shared_ptr<T> _instance;
public:
    static std::shared_ptr<T> GetInstance() {
        static std::once_flag s_flag;
        std::call_once(s_flag, [&]() {
            //why not make_shared?
            _instance = std::shared_ptr<T>(new T);
            });
        return _instance;
    }
};

template <typename T>
std::shared_ptr<T> Manager<T>::_instance = nullptr;