all: logger acmonitor test_aclog ransom_logs

logger: logger.c
	gcc -Wall -fPIC -shared -o logger.so logger.c -lcrypto -ldl 

acmonitor: acmonitor.c 
	gcc acmonitor.c -o acmonitor

test_aclog: test_aclog.c 
	gcc test_aclog.c -o test_aclog

ransom_logs: ransom_logs.c
	gcc ransom_logs.c -o ransom_logs

run: logger.so test_aclog
	LD_PRELOAD=./logger.so ./test_aclog

clean:
	rm -rf logger.so
	rm -rf test_aclog
	rm -rf acmonitor
	
	rm -rf file_logging.log
	rm -rf file_*
	rm -rf efile*
	rm -rf cfile*
	rm -rf ransom_logs

