#include <iostream>
#include <iomanip>
#include <chrono>
#include <ctime>
#include <memory>
#include <string>
#include <thread>

#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>


void child_proc(int read_fd);
void parent_proc(int write_fd);
void worker();

using std::cout;
using std::endl;
using std::string;
using std::unique_ptr;

template<typename ... Args>
string string_format( const string& format, Args ... args )
{
    size_t size = snprintf( nullptr, 0, format.c_str(), args ... ) + 1; // Extra space for '\0'
    unique_ptr<char[]> buf( new char[size]); 
    snprintf( buf.get(), size, format.c_str(), args ... );
    return string( buf.get(), buf.get() + size - 1 ); // We don't want the '\0' inside
}

int main(int argc, char* argv[])
{
	int pip[2] = {-1, -1};
    int error = socketpair(AF_UNIX, SOCK_STREAM, 0, pip);
	if (error != 0) {
    	std::cout << "error occured." << std::endl;
	}

    pid_t id = fork();
    if (id == 0) {
        close(pip[1]);
        child_proc(pip[0]);
    } else {
        close(pip[0]);
        parent_proc(pip[1]);
    }

    return 0;
}

void child_worker(int fd)
{
    std::chrono::seconds sec(1);
	int val = 0;
    while (true) {
		read(fd, &val, sizeof(val));
		++val;
		write(fd, &val, sizeof(val));

    	std::time_t t = std::time(nullptr);
	    char time_buf[100];
   		std::strftime(time_buf, sizeof time_buf, "%D %T", std::gmtime(&t));
        std::cout << string_format(("send data %d at time [%s] pid [%d] ppid [%d]\n"), 
						val,
						time_buf,
						getpid(),
						getppid());
    };
}

void child_proc(int fd)
{
    child_worker(fd);
}

void parent_worker(int fd)
{
    std::chrono::seconds sec(1);
	int val = 0;
    while (true) {
		++val;
		write(fd, &val, sizeof(val));

		read(fd, &val, sizeof(val));


    	std::time_t t = std::time(nullptr);
	    char time_buf[100];
   		std::strftime(time_buf, sizeof time_buf, "%D %T", std::gmtime(&t));
        std::cout << string_format(("receive data %d at time [%s] pid [%d] ppid [%d]\n"), 
						val,
						time_buf,
						getpid(),
						getppid());
        std::this_thread::sleep_for(sec);
    };
}

void parent_proc(int fd)
{
    parent_worker(fd);
}

void worker()
{
    std::chrono::seconds sec(1);
    while (true) {
    	std::time_t t = std::time(nullptr);
	    char time_buf[100];
   		std::strftime(time_buf, sizeof time_buf, "%D %T", std::gmtime(&t));

        std::this_thread::sleep_for(sec);

        std::cout << string_format(("time [%s] pid [%d] ppid [%d]\n"), 
						time_buf,
						getpid(),
						getppid());
    };
}
