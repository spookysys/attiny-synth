mkdir -p wav_22k

find . -name "*.wav" | while read f
do
	echo $f
	sox "$f" -b 16 -r 22050 "wav_22k/$(basename "$f")"
done

