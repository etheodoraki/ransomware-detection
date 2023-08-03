# ransomware-detection
Basic ransomware detection
Theodoraki Emmanouela

```
            [ACE414]
  Security of Systems and Services
          Assignment 5
```

# Step 1: Implement​ the ransomware
Development of the file "ransomware.sh" which is able to execute 2 functions.

### Function 1: 
The user gives a number of files that wants to be encrypted and a directorty to 
create them in.
./ransomware -f 1 -e <numofFilestoEncrypt> -d <directorty>
<-e> files are created in a loop using the shared library "logger.so" and the 
file "ransom_logs.c". 
Then each file is being encrypted with openssl -aes-256-cbc and lastly the 
original files are being deleted.

### Function 2: 
The user gives a number of files that wants to be created in a given directorty.
./ransomware -f 2 -c <numofFilestoEncrypt> -d <directorty>
<-c> files are created in a loop and using the shared library "logger.so" and the file "ransom_logs.c"


# Step 2: Use​ the Access Control Logging tool from Assignment 4
In order to use the shared library "logger.so" the following commands 
were called every time a file access happend.

        LD_PRELOAD=./logger.so ./ransom_logs "${DIR}" "e" "$i"          -> for the new files created for the following encryption
        LD_PRELOAD=./logger.so ./ransom_logs "${DIR}" "ee" "$j"         -> for the encrypted files created and the original files that opened
        LD_PRELOAD=./logger.so ./ransom_logs "${DIR}" "c" "$i"          -> for the new files created

In the file_ogging.log file we can see the corresponding logs.

