# Run from ubuntu, so you can get shntool


sudo apt-get install shntool


# mkdir -p tmp1
# for f in `ls 99drums`
# do
# 	sox 99drums/$f -b 16 -r 22050 tmp1/$f
# done

mkdir -p tmp2
sox 99drums/perc-808.wav -b 16 -r 22050 tmp2/perc-808.wav
sox 99drums/hihat-electro.wav -b 16 -r 22050 tmp2/hihat-electro.wav
sox 99drums/hihat-analog.wav -b 16 -r 22050 tmp2/hihat-analog.wav
sox 99drums/hihat-808.wav -b 16 -r 22050 tmp2/hihat-808.wav
sox 99drums/snare-electro.wav -b 16 -r 22050 tmp2/snare-electro.wav
sox 99drums/perc-metal.wav -b 16 -r 22050 tmp2/perc-metal.wav
sox 99drums/snare-808.wav -b 16 -r 22050 tmp2/snare-808.wav
sox 99drums/snare-tape.wav -b 16 -r 22050 tmp2/snare-tape.wav

