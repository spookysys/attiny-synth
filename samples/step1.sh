# Run from ubuntu, so you can get shntool


sudo apt-get install shntool

mkdir -p step1
for f in `ls 99drums`
do
	sox 99drums/$f -b 16 -r 22050 step1/$f
done

