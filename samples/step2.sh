# Run from git bash, so you can run encode.exe

# Note: encode.exe from "../dox/ADPCM Decoder/AVR336/Tools" should be here
mkdir -p done_inc done_raw
for f in `ls tmp2`
do
	./encode.exe -l -2 -i tmp2/$f -c done_inc/${f%.*}.inc
	./encode.exe -l -2 -i tmp2/$f -o done_raw/${f%.*}.raw
done

