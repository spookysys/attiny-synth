
mkdir -p step1

for f in `ls 99drums`
do
	sox 99drums/$f -b 16 -r 22050 step1/$f
done

