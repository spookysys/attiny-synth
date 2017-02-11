# Run from git bash, so you can run encode.exe

# Note: encode.exe from "../dox/ADPCM Decoder/AVR336/Tools" should be here
mkdir -p step2
for f in `ls step1`
do
	./encode.exe -l -2 -i step1/$f -c step2/${f%.*}.inc
done

