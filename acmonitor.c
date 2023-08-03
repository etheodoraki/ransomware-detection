#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

struct entry {
	int uid; /* user id (positive integer) */
	int access_type; /* access type values [0-2] */
	int action_denied; /* is action denied values [0-1] */
	time_t date; /* file access date */
	time_t time; /* file access time */
	char *file; /* filename (string) */
	char *fingerprint; /* file fingerprint */
};

struct user {
	int uid;				// user ID
	int ua_access_count;			// number of file accesses
	char files[10][FILENAME_MAX];
	int malicius;			//malicius user=1 if unauth file_access_cnt>7
	struct user * next;
};

struct user * findUser(struct user *, int );

void usage(void){
	printf(
	       "\n"
	       "usage:\n"
	       "\t./monitor \n"
		   "Options:\n"
		   "-m, Prints malicious users\n"
		   "-i <filename>, Prints table of users that modified "
		   "the file <filename> and the number of modifications\n"
		   "-h, Help message\n\n"
		   );

	exit(1);
}

void list_unauthorized_accesses(FILE *log){
	/* add your code here */
	struct entry entry;
	struct user *uaUsers = NULL;	//list of users with anauthorizes file accesses
	struct user *headUser = NULL;
	struct user *tmp = NULL;
	time_t t = time(NULL);
	struct  tm tm = *localtime(&t);
	int first = 0;

	char c;
	int lines = 0;
	for (c=getc(log); c!= EOF; c = getc(log)){
		if (c == '\n') 
            lines = lines + 1; 
	}
	rewind(log);
	printf("%d\n",lines);
	int uid;
	
	for (size_t i = 0; i < lines; i++){
			int file_tries = 1;
		fscanf(log,"%d\t%s\t%02d-%02d-%d\t%02d:%02d:%02d\t%d\t%d\t%*s\n",
			&entry.uid,entry.file, 
			&tm.tm_mday,&tm.tm_mon,&tm.tm_year,
			&tm.tm_hour,&tm.tm_min,&tm.tm_sec,
			&entry.access_type,&entry.action_denied);
		
		//printf("uid: %d %s\n",entry.uid,entry.file);
		// find malicious users
		if (entry.action_denied){
			++first;
			//if first user with unauthorized access, initialize him
			if (first == 1){
				uaUsers = (struct user *)malloc(sizeof(struct user));
				uaUsers->uid = entry.uid;
				uaUsers->ua_access_count = 1;
				memcpy(uaUsers->files[0],entry.file,FILENAME_MAX);
				uaUsers->malicius = 0;
				uaUsers->next = NULL;
				headUser = uaUsers;
				//printf("%d %d %d\n",uaUsers->uid, uaUsers->ua_access_count, uaUsers->malicius);
			}else{
			/* if not the first malicious user 
			   there is posibility to make multiple unauthorized accesses
			   in this case find him and check if he has >7 unauth. accesses for distinct files*/
				tmp = headUser;
				tmp = findUser(tmp,entry.uid);
				if (tmp != NULL){

					//printf("%d %d %d\n",tmp->uid, tmp->ua_access_count, tmp->malicius);

					// if not already malicious
					if (tmp->malicius != 1){
						//compare files
						for (size_t i = 0; i < 7; i++){
							if(strcmp(entry.file,tmp->files[i]) == 0)
								file_tries = 2;
						}
						//add unauthorized accesses for different files
						if(file_tries != 2){
							tmp->ua_access_count++;
							// add file to list
							memcpy(tmp->files[tmp->ua_access_count],entry.file,FILENAME_MAX);
						}
					}
					// make him malicious if he is
					if (tmp->ua_access_count > 7)
						tmp->malicius = 1;					
				}else{
				//add new
					struct user *newuaUser;
					struct user *temp = uaUsers;
					newuaUser = (struct user *)malloc(sizeof(struct user));
					newuaUser->uid = entry.uid;
					newuaUser->ua_access_count = 1;
					newuaUser->malicius = 0;
					memcpy(newuaUser->files[0],entry.file,FILENAME_MAX);
					//find position to append
					while (temp->next != NULL){
						temp = temp->next;
					}
					temp->next = newuaUser;
					newuaUser->next = NULL;
				}				
			}		
		}
	}
	// print malicious users
	tmp = headUser;
	while (tmp != NULL){
		if(tmp->malicius)
			printf("Malicious User ID: %d\n", tmp->uid);
		tmp = tmp->next;
	}
	
	while (headUser != NULL){
		tmp = headUser->next;
		free(headUser);
		headUser = tmp;
	}

	/* ... */
	return;
}


void
list_file_modifications(FILE *log, char *file_to_scan)
{

	/* add your code here */
	/* ... */
	/* ... */
	/* ... */
	/* ... */

	return;

}


int main(int argc, char *argv[]){
	int ch;
	FILE *log;

	if (argc < 2)
		usage();

	log = fopen("./file_logging.log", "r");
	if (log == NULL) {
		printf("Error opening log file \"%s\"\n", "./log");
		return 1;
	}

	while ((ch = getopt(argc, argv, "hi:m")) != -1) {
		switch (ch) {		
		case 'i':
			list_file_modifications(log, optarg);
			break;
		case 'm':
			list_unauthorized_accesses(log);
			break;
		default:
			printf("Please select a valid option from the above.\n");
			usage();
		}
	}

	fclose(log);
	argc -= optind;
	argv += optind;	
	
	return 0;
}


struct user * findUser(struct user *usersList, int uid){
	struct user *temp = usersList;
	while (temp != NULL){
		if (temp->uid == uid){
			return temp; 	// found
		}
		temp = temp->next;		
	}
	return NULL; 			// not found
}