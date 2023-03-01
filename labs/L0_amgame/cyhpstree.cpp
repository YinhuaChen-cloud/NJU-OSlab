// 适合我的目标：把所有进程的名字、pid、以及父进程按顺序打印出来即可

// 您可以使用标准C/C++库中的dirent.h头文件来列出一个目录下的所有文件。
// 以下是一个示例程序，可以列出给定目录（即DIR_PATH）下的所有文件名：

#include <iostream>
#include <dirent.h> // 目录读写相关API
#include <cxxabi.h> // 用来解锁mangled type string name
#include <regex> // 正则表达式
#include <unistd.h> // readlink
#include <fcntl.h> // readlink
#include <libgen.h> // basename() 的头文件

using namespace std;

int main() {
// 1. 得到系统中所有进程的编号 (每个进程都会有唯一的编号) 保存到列表里； -- checked
// 方案：获取 /proc 文件夹下所有以数字为开头的目录
    DIR *dir;
    struct dirent *ent;
    const char *dir_path = "/proc";

    // 正则表达式
    regex pattern("\\d+");
    // 用来存放 pid 的列表
    vector<pid_t> pidList;
    // 用来存放所有进程的名字的列表
    vector<string> nameList;
    // 用来存放所有进程的父进程 pid
    vector<pid_t> parentPidList;
    // 从/proc目录中提取所有进程的pid
    if ((dir = opendir(dir_path)) != NULL)  // 打开目录
    {
        while ((ent = readdir(dir)) != NULL)  // 读取目录，一次读一个
        {
            if (regex_match(ent->d_name, pattern)) {
                pidList.push_back(stoi(ent->d_name));
            }
            // cout << typeid(ent->d_name).name() << endl;
        }
        closedir(dir); // 关闭目录
    } 
    else 
    {
        cout << "Could not open directory: " << dir_path << endl;
        return EXIT_FAILURE;
    }

// 2. 对列表里的每个编号，得到它自己的名字
    for (size_t i = 0; i < pidList.size(); ++i) {
        string exe_path = "/proc/" + to_string(pidList[i]) + "/exe";
        char link_path[1024];
        ssize_t len = readlink(exe_path.c_str(), link_path, sizeof(link_path));
        if (len == -1) {
            // it means the pid does not have exe file on fs, remove this pid
            pidList.erase(pidList.begin() + i);
            // and decrease i once
            i--;
            continue;
        }
        link_path[len] = '\0';

        // 获取文件路径的basename
        char *base = basename(&link_path[0]);

        nameList.push_back(string(base));
    }

// 3. 对列表里的每个编号，得到它的的父亲是谁
    for (size_t i = 0; i < pidList.size(); ++i) {
        string status_path = "/proc/" + to_string(pidList[i]) + "/status";

        FILE* file = fopen(status_path.c_str(), "r");
        if (file == NULL) {
            printf("Failed to open file %s\n", status_path.c_str());
            return -1;
        }

        char line[128];
        int ppid = -1;
        while (fgets(line, sizeof(line), file)) {
            if (strncmp(line, "PPid:", 5) == 0) {
                ppid = atoi(line + 5);
                break;
            }
        }

        fclose(file);

        parentPidList.push_back(ppid);
    }

// 4. 按照 pid process_name ppid 格式打印所有vector
    for (size_t i = 0; i < pidList.size(); ++i) {
        std::cout << pidList[i] << "\t";
        std::cout << nameList[i] << "\t ";
        std::cout << parentPidList[i] << endl;
    }

    return EXIT_SUCCESS;
}


