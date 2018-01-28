#include <cstdarg>
#include <cstddef>
#include <cstdio>
#include <ctime>
#include <iostream>
#include <memory>
#include <string>
#include <vector>

using std::cout;
using std::endl;
using std::string;
using std::unique_ptr;

void debug_log(const char *fmt, ...)
{
    std::time_t t = std::time(nullptr);
    char time_buf[100];
    std::strftime(time_buf, sizeof time_buf, "%D %T", std::gmtime(&t));
    va_list args1;
    va_start(args1, fmt);
    va_list args2;
    va_copy(args2, args1);
    std::vector<char> buf(1+std::vsnprintf(NULL, 0, fmt, args1));
    va_end(args1);
    std::vsnprintf(buf.data(), buf.size(), fmt, args2);
    va_end(args2);
    std::printf("%s [debug]: %s\n", time_buf, buf.data());
}

string FormatString(const char* format, ...)
{
    va_list args1;
    va_start(args1, format);
    va_list args2;
    va_copy(args2, args1);
	size_t size = std::vsnprintf(nullptr, 0, format, args1) + 1;
    va_end(args1);

    unique_ptr<char[]> buf( new char[size] ); 
    std::vsnprintf(buf.get(), size, format, args2);
    va_end(args2);

    return string( buf.get(), buf.get() + size - 1 ); // We don't want the '\0' inside
}

template<typename ... Args>
string string_format( const string& format, Args ... args )
{
    size_t size = snprintf( nullptr, 0, format.c_str(), args ... ) + 1; // Extra space for '\0'
    unique_ptr<char[]> buf( new char[size]); 
    snprintf( buf.get(), size, format.c_str(), args ... );
    return string( buf.get(), buf.get() + size - 1 ); // We don't want the '\0' inside
}

int main(int argc, char* arv[])
{
    cout << FormatString("Hello world line: %d %s", __LINE__, "page") << endl;
    cout << string_format("Hello world line: %d %s", __LINE__, "page") << endl;

    debug_log("Logging, %d, %d, %d", 1, 2, 3);

    return 0;
}
