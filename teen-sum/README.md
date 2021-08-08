# teen-sum
Hosted in BSides Noida CTF 2021: https://ctftime.org/event/1397
```
Points - 490	Number of solves - 11

Presenting the more hardened and more feature-rich?? adder.

nc 34.71.103.59 14141

Author:whoamiT

Attachments: teen-sum-release.zip
```

This repo contains the original challenge as was developed by me initially, but did a few modifications later to make it a bit easier to pwn. The CTF version of challenge can be extracted from `teen-sum-release.zip`. The patch file is below:
```diff
$ git diff teen-sum-orignal.c teen-sum-ctf.c
diff --git a/teen-sum-orignal.c b/teen-sum-ctf.c
index f42781d..8421334 100644
--- a/teen-sum-orignal.c
+++ b/teen-sum-ctf.c
@@ -72,6 +72,7 @@ void calc()
                printf("> ");
                scanf("%ld", &arr[i]);
                sum += arr[i];
+               printf("You entered %ld\n", arr[i++]);
-				i++;
        }
 
        puts("Want to update your name?\n0. No\n1. Yes");
```

`exploit.py` is attached for your reference and works in both cases.