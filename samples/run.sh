
mkdir -p wav_22k wav_44k

for f in `ls 99drums`
do
	sox 99drums/$f -b 16 -r 22050 wav_22k/$f
	sox 99drums/$f -b 16 -r 44100 wav_44k/$f
done

