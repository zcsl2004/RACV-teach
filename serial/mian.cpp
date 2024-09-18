#include <iostream>  
#include <unistd.h> // UNIX 标准函数定义  
#include <fcntl.h>  // 文件控制定义  
#include <termios.h> // POSIX 终端控制定义  
#include <cstring>  // 字符串操作函数  
  
using namespace std;  
  
// 设置串口参数  
int setSerialPort(int fd) {  
    struct termios tty;  
    memset(&tty, 0, sizeof tty);  
  
    if (tcgetattr(fd, &tty) != 0) {  
        cerr << "Error from tcgetattr" << endl;  
        return -1;  
    }  
  
    cfsetospeed(&tty, B9600);  
    cfsetispeed(&tty, B9600);  
  
    tty.c_cflag = (tty.c_cflag & ~CSIZE) | CS8; // 8-bit chars  
    tty.c_iflag &= ~IGNBRK; // 禁用break processing  
    tty.c_lflag = 0;        // 禁用canonical input  
    tty.c_oflag = 0;        // 禁用output processing  
    tty.c_cc[VMIN] = 0;       
    tty.c_cc[VTIME] = 5;    
  
    tty.c_iflag &= ~(IXON | IXOFF | IXANY); // 关闭软件流控  
    tty.c_cflag |= (CLOCAL | CREAD); // 忽略modem controls, 启用接收器  
    tty.c_cflag &= ~(PARENB | PARODD); // 清除校验位  
    tty.c_cflag |= 0;  
    tty.c_cflag &= ~CSTOPB;  
    tty.c_cflag &= ~CRTSCTS;  
  
    if (tcsetattr(fd, TCSANOW, &tty) != 0) {  
        cerr << "Error from tcsetattr" << endl;  
        return -1;  
    }  
    return 0;  
}  
  
int main() {  
    const char *portName = "/dev/ttyS1"; // 串口名称，根据实际情况修改  
    int serialPort = open(portName, O_RDWR);  
  
    if (serialPort < 0) {  
        cerr << "Error opening " << portName << ": " << strerror(errno) << endl;  
        return 1;  
    }  
  
    if (setSerialPort(serialPort) < 0) {  
        close(serialPort);  
        return 1;  
    }  
  
    const char *msg = "hello, pc\n";  
    while (true) {  
        // 发送数据  
        write(serialPort, msg, strlen(msg));  
  
        // 接收数据  
        char buf[256];  
        int num_bytes = read(serialPort, buf, sizeof(buf));  
        if (num_bytes < 0) {  
            cerr << "Error reading: " << strerror(errno) << endl;  
            break;  
        }  
  
        // 显示接收到的数据  
        cout << "Received: " << string(buf, num_bytes) << endl;  
  
        // 延时，以避免过快发送数据  
        sleep(1);  
    }  
  
    close(serialPort);  
    return 0;  
}