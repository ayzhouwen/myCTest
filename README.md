# myCTest
主要测试一些Linux c语言代码
# 开发环境
win10+vscode+虚拟机centos7.5  ，远程开发模式
# 开发环境搭建步骤
1）win10 下载vscode，下载安装远程c++ cmake 开发相关的插件网上百度 <br>
2）在centos上git下载代码 <br>
3）用vscode远程连接centos的目录，确保可以正常ssh连接<br>
4）vscode中替换launch.json，settings.json tasks.json<br>
5）选择zwgdb调试可以 正常编译，打断点调试了<br>
# 发现的问题
1）调试时有时候单步调试或者逐过程过多发现程序卡死，可以在vscode 的调用堆栈中恢复运行，不必重启vscode<br>
2）有时候断点红点去了，但是还是进入断点，那么在vscode中的菜单中运行-》删除所有断点即可<br>
3）遇到没法打断点情况，看看是不是生成的执行文件没有调试符号，现在cmkaelist.txt  默认是生成调试符号的<br>

