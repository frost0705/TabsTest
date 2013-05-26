case $MACHTYPE in (x86_64*) r=64;; esac
./Arifexp$r -high -size -time -rep 100 Arif1 >Arif1.wrk
