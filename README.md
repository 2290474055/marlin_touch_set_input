该项目主要作用是模拟驱动来实现对触摸屏的操控。如：滑动，点击操作。使用的真机是piexl 1<br>
项目分为3个部分<br>
触摸屏驱动部分：<br>
  文件synaptics_dsx_core.c，该驱动为内核原本的驱动，其中添加了函数synaptics_rmi4_f12_abs_report_me为关键代码用来向输入子系统报告触摸屏操作。通过封装synaptics_rmi4_f12_abs_report_me实现2个函数set_input_touch_slide，set_input_touch_click分别实现滑动和点击事件<br>
自己的驱动部分：<br>
  文件hello.c，该驱动的主要作用是为了打通用户层，使得用户层可以通过ioctl来通过该驱动来调用synaptics_dsx_core.c内的函数。所以该驱动只是在/dev下创建一个文件<br>
用户层部分：<br>
  文件my_dev_user.cpp，打开自己的驱动文件然后通过自定义zeyu_io_cmd_data结构体传送数据，来实现点击

编译的话，可以直接替代synaptics_dsx_core.c文件使用，然后将hello.c自己的驱动添加进去，my_dev_user.cpp在android studio直接编译就可以
