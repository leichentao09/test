#include <jni.h>
#include <string>
#include <zconf.h>
#include "native-lib.h"
#include <syslog.h>

const char *PATH = "/data/data/com.spring.socketprocess/my.sock";
int m_child;
const char *userId;

void child_do_work() {
    //开启socket   服务端
    if (child_create_channel()) {
        child_linlisten_msg();
    }
}

//创建服务端的Socket  读取信息
void child_linlisten_msg() {
    fd_set rfds;
    struct timeval timeout = {3, 0};
    while (1) {
        //清空内容
        FD_ZERO(&rfds);
        FD_SET(m_child, &rfds);
        //4个客户端
        int r = select(m_child + 1, &rfds, NULL, NULL, &timeout);
        if (r > 0) {
            char pkg[256] = {0};
            //保证读到信息 指定
            if (FD_ISSET(m_child, &rfds)) {
                //阻塞函数   读什么
                int result = read(m_child, pkg, sizeof(pkg));

                //开启服务
                execlp("am", "am", "startservice", "--user", userId,
                       "com.spring.socketprocess/com.spring.socketprocess.ProcessService",
                       (char *) NULL);
                break;
            }
        }
    }
}

//服务端获取信息
int child_create_channel() {
    // ip 文件
    int listenfd = socket(AF_LOCAL, SOCK_STREAM, 0);
    unlink(PATH);
    struct sockaddr_un addr; //内存区域

    memset(&addr, 0, sizeof(sockaddr_un));
    addr.sun_family = AF_LOCAL;
    strcpy(addr.sun_path, PATH);
    int connfd = 0;
    if (bind(listenfd, (const sockaddr *) &addr, sizeof(sockaddr_un)) < 0) {
        return 0;
    }

    listen(listenfd, 5); //监听5个进程
    //保证宿主进程连接成功
    while (1) {
        //返回值   客户端地址   阻塞函数
        if ((connfd == accept(listenfd, NULL, NULL)) < 0) {
            if (errno == EINTR) {
                continue;
            } else {
                return 0;
            }
        }
        m_child = connfd;
        break;
    }
    return 1;
}

extern "C"
JNIEXPORT void JNICALL
Java_com_spring_socketprocess_Watcher_createWatcher(JNIEnv *env, jobject instance,
                                                    jstring userId_) {
    userId = env->GetStringUTFChars(userId_, 0);

    //开双进程
    pid_t pid = fork();
    if (pid < 0) {

    } else if (pid == 0) {
        //子进程  守护
        child_do_work();

    } else if (pid > 0) {
        //父进程

    }

    // TODO

    env->ReleaseStringUTFChars(userId_, userId);
}

extern "C"
JNIEXPORT void JNICALL
Java_com_spring_socketprocess_Watcher_connectMonitor(JNIEnv *env, jobject instance) {
    int socked;
    struct sockaddr_un addr; //内存区域
    //子进程    客户端
    while (1) {
        socked = socket(AF_LOCAL, SOCK_STREAM, 0);
        if (socked < 0) {
            return;
        }
        memset(&addr, 0, sizeof(sockaddr));
        addr.sun_family = AF_LOCAL;
        strcpy(addr.sun_path, PATH);

        if (connect(socked, (const sockaddr *) &addr, sizeof(sockaddr_un)) < 0) {
            close(socked);
            sleep(1);//休眠1秒
            continue;//再来尝试
        }
        break;
    }
    // TODO
}