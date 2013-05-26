case $MACHTYPE in (x86_64*) r=64;; esac
./Arifexp$r -high -size -time -rep 10 Arif3 >Arif3.wrk
