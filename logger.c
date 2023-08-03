#define _GNU_SOURCE

#include <time.h>
#include <stdio.h>
#include <dlfcn.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <openssl/md5.h>
#include <limits.h>
#include <stdarg.h>
#include <assert.h>

void logEntries( int , char *,struct tm , int , int , char *);
void getHash(char *, char *);

FILE * fopen(const char *path, const char *mode){
	FILE *original_fopen_ret;
	FILE *(*original_fopen)(const char*, const char*);

	/* call the original fopen function */
	original_fopen = dlsym(RTLD_NEXT, "fopen");
	original_fopen_ret = (*original_fopen)(path, mode);

	/* add your code here */

	printf("In our own fopen, opening %s\n",path);
	int must_create;
	if( access( path, F_OK ) == -1 ) {
    	// file does not exist
		must_create = 1;
	} else {
		// file exists
		must_create = 0;
	}
 // 				__ prepare Log entries __
 // UID
	int UID = getuid();
 // File path
	char *filepath;
	//char actualpath[PATH_MAX+1];
	filepath = realpath(path, NULL);
 // Date + Timestamp
	time_t t = time(NULL);
	struct  tm tm = *localtime(&t);
 //Access type
	int access_type;
	if (original_fopen_ret != NULL && must_create == 0){
		/* access type: open */
		access_type = 1;
	}else if (original_fopen_ret != NULL && must_create == 1){
		/* access type: create */
		access_type = 0;
	}else if(strcmp(mode, "w") == 0){
		access_type = 0;
	}else{
		access_type = 0;
	}
	
 // Is-action-denied flag
 	int action_denied;
	if (original_fopen_ret != NULL){
		action_denied = 0;
	}else{
		action_denied = 1;
	}
 // File fingerprint
	// unsigned char *buffer;
	// unsigned char fingerprint[MD5_DIGEST_LENGTH];
	// long fsize; 
	// fseek(original_fopen_ret, 0,SEEK_END);
	// fsize = ftell(original_fopen_ret);
	// rewind(original_fopen_ret);
	
	// buffer = (unsigned char *)malloc(fsize+1);
	// //copy file content into buffer
	// fread(buffer,fsize+1,1,original_fopen_ret);
	// MD5_CTX ctx;
    // MD5_Init(&ctx);
    // MD5_Update(&ctx, buffer, fsize);
    // MD5_Final(fingerprint, &ctx);
	//MD5((const unsigned char*)buffer, sizeof(buffer) - 1, fingerprint);
	char fingerprint[MD5_DIGEST_LENGTH];
	getHash(filepath,fingerprint);
 // 				Append LOG Entries
	logEntries(UID,filepath,tm,access_type,action_denied,fingerprint);
	free((void*)filepath);
	/* ... */
	return original_fopen_ret;
}

size_t fwrite(const void *ptr, size_t size, size_t nmemb, FILE *stream) {
	size_t original_fwrite_ret;
	size_t (*original_fwrite)(const void*, size_t, size_t, FILE*);

	/* call the original fwrite function */
	original_fwrite = dlsym(RTLD_NEXT, "fwrite");
	original_fwrite_ret = (*original_fwrite)(ptr, size, nmemb, stream);
	
	/* add your code here */
	/* ... */
	printf("In our own fwrite\n");
	// 				__ prepare Log entries __
 // UID
	int UID = getuid();
 // File name : path and name of file
 	// get file name
	char filename[FILENAME_MAX];
	char proclink[FILENAME_MAX];
	// get file descriptor to call readlink
	int fd = fileno(stream);
	sprintf(proclink,"/proc/self/fd/%d", fd);
	//ssize_t r;
	readlink(proclink, filename, FILENAME_MAX-1);
	// if (r!=1){
	// 	filename[r] = '\0';
	// }
	// get file path
	char *filepath;
	//char actualpath[PATH_MAX];
	filepath = realpath(filename, NULL);
 // Date + Timestamp
	time_t t = time(NULL);
	struct  tm tm = *localtime(&t);
 //Access type
	int access_type = 2;
 // Is-action-denied flag
 	int action_denied;
	if (!original_fwrite_ret){
		action_denied = 1;
	}else{
		action_denied = 0;
	}
 // File fingerprint
	char fingerprint[MD5_DIGEST_LENGTH];
	getHash(filepath,fingerprint);
 // 				Append LOG Entries
	logEntries(UID,filepath,tm,access_type,action_denied,fingerprint);
	free((void*)filepath);
	// fflush(stream);
	/* ... */
	return original_fwrite_ret;
}

void getHash(char *path, char *fingerprint){
 	FILE *(*original_fopen)(const char*, const char*);
	original_fopen = dlsym(RTLD_NEXT, "fopen");
	FILE *fptr = (*original_fopen)(path, "r");
	long fsize; 
	unsigned char *buffer;
	unsigned char hash[MD5_DIGEST_LENGTH];
	fseek(fptr, 0,SEEK_END);
	fsize = ftell(fptr);
	rewind(fptr);
	buffer = (unsigned char *)malloc(fsize+1);
	//copy file content into buffer
	fread(buffer,fsize+1,1,fptr);
	MD5_CTX ctx;
    MD5_Init(&ctx);
    MD5_Update(&ctx, buffer, fsize);
    MD5_Final(hash, &ctx);
	//MD5((const unsigned char*)buffer, sizeof(buffer) - 1, hash);
	for (size_t i = 0; i < MD5_DIGEST_LENGTH; i++) {
		sprintf(&fingerprint[i],"%02x", hash[i]);
	}
	fclose(fptr);
}
void logEntries(int UID, char *filepath, struct tm tm, int acc_type, int action_denied, char *hash){
	// opening log file and write at eof
	FILE *(*original_fopen)(const char*, const char*);
	original_fopen = dlsym(RTLD_NEXT, "fopen");
	FILE *logfile = (*original_fopen)("file_logging.log", "a");
				//"UID:%d\tFile:%s\tDate:%02d/%02d/%02d\tTime:%02d:%02d:%02d\tAccess type:%d\tIs-action-denied:%d\tFingerprint:%s\n"
	fprintf(logfile,"%d\t%s\t%02d-%02d-%d\t%02d:%02d:%02d\t%d\t%d\t%s\n", 
	UID,filepath,tm.tm_mday,tm.tm_mon+1,1900+tm.tm_year,tm.tm_hour, tm.tm_min, tm.tm_sec, acc_type,action_denied,hash);
	fclose(logfile);
}

FILE * fopen64(const char * path, const char * mode){
    return fopen(path, mode);
}


