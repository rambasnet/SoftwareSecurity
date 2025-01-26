set args good.txt
break vulnerable
run
next
print $rbp
print &buffer
print "offset: "
print /d (void *)$rbp - (void *)&buffer
continue
quit
