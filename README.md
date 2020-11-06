此程序为hex转化为yxf或者fwf格式

输入参数为 hex文件名 配置文件名

例如：yxf_cvt 8361M.hex cvt_config.ini

cvt_config.ini里描述了电路板型号，版本等信息

fwf                  ;format

9091                 ;Model

YX-A05-3A            ;PCB

panel                ;Node

1.01                 ;version

highlead panel       ;description

;后的为注释，实际不应有

作为keil插件使用时，加入环境变量的参数为 !H $Pcvt_config.ini

!H表示hex路径，$P表示工程路径，cvt_config.ini应放在工程根目录下面
