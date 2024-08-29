/*
========================================================================================================

Name: 27a
Author: Himanshu Rawat
Description: Write a program to execute ls -Rl by the following system calls
		a. execl
Date: 29th August, 2024

========================================================================================================
*/


#include<stdio.h>
#include<unistd.h>

int main(){
	execl("/bin/ls", "ls", "-Rl", NULL); 
}


/*
========================================================================================================
Output:

./a.out
.:
total 188
-rw-rw-r-- 1 himanshu-rawat himanshu-rawat   523 Aug 13 23:48 10.cpp
-rw-rw-r-- 1 himanshu-rawat himanshu-rawat   936 Aug 14 15:59 11.cpp
-rw-rw-r-- 1 himanshu-rawat himanshu-rawat   703 Aug 14 17:16 12.cpp
-rw-rw-r-- 1 himanshu-rawat himanshu-rawat   402 Aug 21 22:50 13.cpp
-rw-rw-r-- 1 himanshu-rawat himanshu-rawat   724 Aug 22 08:31 14.cpp
-rw-rw-r-- 1 himanshu-rawat himanshu-rawat   139 Aug 22 08:44 15.cpp
-rw-rw-r-- 1 himanshu-rawat himanshu-rawat   779 Aug 24 23:01 16a.cpp
-rw-rw-r-- 1 himanshu-rawat himanshu-rawat   776 Aug 24 23:06 16b.cpp
-rw-rw-r-- 1 himanshu-rawat himanshu-rawat   541 Aug 27 22:41 17a.c
-rw-rw-r-- 1 himanshu-rawat himanshu-rawat  1315 Aug 27 22:53 17b.c
-rw-rw-r-- 1 himanshu-rawat himanshu-rawat  1080 Aug 28 20:21 18a.c
-rw-rw-r-- 1 himanshu-rawat himanshu-rawat   831 Aug 28 20:21 18b.c
-rw-rw-r-- 1 himanshu-rawat himanshu-rawat   604 Aug 28 20:14 18.c
-rw-rw-r-- 1 himanshu-rawat himanshu-rawat   433 Aug 29 09:25 19.c
-rw-rw-r-- 1 himanshu-rawat himanshu-rawat   302 Aug 13 22:39 1a.cpp
-rw-rw-r-- 1 himanshu-rawat himanshu-rawat   298 Aug 13 22:34 1b.cpp
-rw-rw-r-- 1 himanshu-rawat himanshu-rawat   252 Aug 13 22:58 1c.cpp
-rw-rw-r-- 1 himanshu-rawat himanshu-rawat   470 Aug 28 20:43 20.c
-rw-rw-r-- 1 himanshu-rawat himanshu-rawat   332 Aug 28 22:07 21.c
-rw-rw-r-- 1 himanshu-rawat himanshu-rawat   439 Aug 28 22:33 22.c
-rw-rw-r-- 1 himanshu-rawat himanshu-rawat   194 Aug 28 22:45 23.c
-rw-rw-r-- 1 himanshu-rawat himanshu-rawat   240 Aug 28 23:32 24.c
-rw-rw-r-- 1 himanshu-rawat himanshu-rawat   414 Aug 29 10:13 25.c
-rw-rw-r-- 1 himanshu-rawat himanshu-rawat   129 Aug 29 10:35 26a.c
-rw-rw-r-- 1 himanshu-rawat himanshu-rawat   161 Aug 29 10:46 26b.c
-rw-rw-r-- 1 himanshu-rawat himanshu-rawat    81 Aug 29 10:41 26.c
-rw-rw-r-- 1 himanshu-rawat himanshu-rawat    92 Aug 29 11:06 27a.c
-rw-rw-r-- 1 himanshu-rawat himanshu-rawat    88 Aug 29 11:08 27b.c
-rw-rw-r-- 1 himanshu-rawat himanshu-rawat   126 Aug 29 11:13 27c.c
-rw-rw-r-- 1 himanshu-rawat himanshu-rawat   116 Aug 29 11:18 27d.c
-rw-rw-r-- 1 himanshu-rawat himanshu-rawat   112 Aug 29 11:25 27e.c
-rw-rw-r-- 1 himanshu-rawat himanshu-rawat  1203 Aug 29 20:21 28.c
-rw-rw-r-- 1 himanshu-rawat himanshu-rawat  2603 Aug 29 19:23 29.c
-rw-rw-r-- 1 himanshu-rawat himanshu-rawat   202 Aug 10 13:00 2.cpp
-rw-rw-r-- 1 himanshu-rawat himanshu-rawat  1204 Aug 29 19:14 30.c
-rw-rw-r-- 1 himanshu-rawat himanshu-rawat   280 Aug 13 22:59 3.cpp
-rw-rw-r-- 1 himanshu-rawat himanshu-rawat   487 Aug 13 23:04 4.cpp
-rw-rw-r-- 1 himanshu-rawat himanshu-rawat   217 Aug 10 16:17 5.cpp
-rw-rw-r-- 1 himanshu-rawat himanshu-rawat   270 Aug 13 19:41 6.cpp
-rw-rw-r-- 1 himanshu-rawat himanshu-rawat   745 Aug 13 20:44 7.cpp
-rw-rw-r-- 1 himanshu-rawat himanshu-rawat   539 Aug 13 22:00 8.cpp
-rw-rw-r-- 1 himanshu-rawat himanshu-rawat   722 Aug 13 22:18 9.cpp
-rwxrwxr-x 1 himanshu-rawat himanshu-rawat 16008 Aug 29 20:29 a.out
-rwxrwxrwx 1 himanshu-rawat himanshu-rawat   136 Aug 14 15:44 data.txt


========================================================================================================
 */

