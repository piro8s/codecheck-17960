#include "include/timeout.h"

int scanfWithTimeout(char *format, char *input, int t) {
	// int imput;
	struct sigaction sa;
	struct itimerval itimer = {{0, t*1000}, {0, t*1000}};
	void myhandler(int no) { input[0] = '0'; }//タイムアウトしたときのメッセージ

	// シグナルハンドラの設定
	memset(&sa,0,sizeof(struct sigaction));
	sa.sa_handler = myhandler;
	//sa.sa_flags = SA_RESTART;
	if(sigaction(SIGALRM,&sa,NULL) != 0){
		perror("sigaction");
		return -1;
	}
	// タイマーの設定
	// itimer.it_value.tv_sec = itimer.it_interval.tv_sec = t; // sec
	// itimer.it_value.tv_usec = itimer.it_interval.tv_usec = 0; // micro sec
	if(setitimer(ITIMER_REAL, &itimer, NULL) < 0) {
		perror("setitimer");
		return -1;
	}
	// １文字入力
	scanf(format, input);//ここの処理が時間内に終わればタイムアウトせずに処理を続行

	return 0;
}
