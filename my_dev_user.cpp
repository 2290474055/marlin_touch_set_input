//
// Created by 22904 on 2023/12/12.
//
#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/ioctl.h>

#define INPUT_TOUCH_CLICK   1
#define INPUT_TOUCH_SLIDE   2
struct zeyu_io_cmd_data{
    int cmd;
    union
    {
        int x_start;
        int x;
    } ;
    union
    {
        int y_start;
        int y;
    } ;
    int x_end;
    int y_end;
};

int main(int argc, char *argv[]){
    int fd = open("/dev/zeyudev",O_RDWR);
    if(fd < 0){
        printf("open err");
    }
    struct zeyu_io_cmd_data data = {0};
    for(int i = 0;i <= 100;i++){
        data.cmd = INPUT_TOUCH_CLICK;
        data.x = 1049;
        data.y = 725;
        ioctl(fd,0,&data);

        usleep(3000000);

        data.cmd = INPUT_TOUCH_SLIDE;
        data.x_start = 614;
        data.y_start = 1401;
        data.x_end = 645;
        data.y_end = 254;
        ioctl(fd,0,&data);
        usleep(3000000);
    }


    close(fd);
    printf("main end");
    return 0;
}
