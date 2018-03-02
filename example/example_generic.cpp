#include <iostream>
#include <map>
#include <string>
#include <boost/lexical_cast.hpp>

using namespace std;

class Example {
public:

    template <typename T>
    Example& put(const string& key, T value) {
        m[key] = boost::lexical_cast<std::string>(value);
        return *this;
    }

    template <typename T>
    T get(const string &key) {
        map<string, string>::iterator it = m.find(key);
        if (it == m.end()) {
            return T();
        }
        return boost::lexical_cast<T>(m[key]);
    }

private:
    map<string, string> m;
};

int main(int argc, char **argv) {
    Example example;
    example.put("a", 1).put("b", 2L).put("c", 3.0F).put("d", 4.0);
    cout << "a: " << example.get<int>("a") << ", e: " << example.get<int>("e") << endl;
}