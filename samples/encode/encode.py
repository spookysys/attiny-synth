import numpy as np
import scipy as sp
from scipy.io import wavfile
from pprint import pprint
from scipy.signal import butter, lfilter, freqz
import random
import resampy # or scipy.signal.resample_poly ?
import glob
from os import path
from matplotlib import pylab
from invfreqs1 import invfreqs

BLOCK_SIZE = 8
SR_ORIG = 22050


# AVERAGE_BITS = 8
# NOISE_BITS = 8


class Rand:
	def __init__(self, seed):
		self.state = random.Random(seed)

	def get_s8(self):
		return np.int8(self.state.randint(-128, 127))


def butter_make(cutoff, fs, btype, order=5):
	nyq = 0.5 * fs
	normal_cutoff = cutoff / nyq
	b, a = butter(order, normal_cutoff, btype=btype, analog=False)
	return b, a


def butter_filter(data, cutoff, fs, btype, order=5):
	b, a = butter_make(cutoff, fs, btype, order=order)
	y = lfilter(b, a, data)
	return y


def noise_highpass(data_in):
	# Generate filter coefficients
	cutoff_freq = (SR_ORIG / BLOCK_SIZE) / 2
	nyquist = SR_ORIG / 2
	cutoff = cutoff_freq / nyquist
	b, a = butter(5, cutoff, btype='high', analog=False)
	print(b, a)

	samples = data_in.size

	# filter some white noise and do an fft
#	white = np.array([random.random() for x in range(samples)])
#	white_filtrd = lfilter(b, a, white)
#	t = np.fft.rfftfreq(white_filtrd.size, 1.0 / SR_ORIG)
#	h = np.fft.rfft(white_filtrd)
#	pylab.plot(t, abs(h) / 30)


	# Plot filter response
	w, h = sp.signal.freqz(b, a, worN=samples)
	t = sp.linspace(0, SR_ORIG/2, samples)
	pylab.plot(t, abs(h))

	# juggle a bit
	b, a = sp.signal.bilinear(b, a)
#	w, h = sp.signal.freqs(b, a, worN=samples)
#	b, a = invfreqs(h, w, 5, 5)
#	b, a = sp.signal.bilinear(b, a)


	# Plot filter response again
	w, h = sp.signal.freqz(b, a, worN=samples)
	t = sp.linspace(0, SR_ORIG/2, samples)
	pylab.plot(t, abs(h))

	

	print(b, a)


	# Run filter
	data_out = lfilter(b, a, data_in)


	# show my plots
	pylab.show()


	return data_out


def encode(data_in):
	# Pad input array to BLOCK_SIZE
	blocks = int(np.ceil(data_in.size / BLOCK_SIZE))
	data_in = np.append(data_in, np.zeros(blocks * BLOCK_SIZE - data_in.size, dtype=np.int16))

	# Filter
	noise = noise_highpass(data_in)

	# Down-sample
	base_lo = resampy.resample(data_in.astype(np.float), SR_ORIG, SR_ORIG/BLOCK_SIZE)
	base2 = resampy.resample(base_lo, SR_ORIG/BLOCK_SIZE, SR_ORIG)

	# noise varant 2
	noise2 = np.array([a - b for a, b in zip(data_in, base2)])

	# White noise
	white = [random.random() for x in range(data_in.size)]


	# Analyze
	freqs = sp.fftpack.rfftfreq(data_in.size, 1.0 / SR_ORIG)
	FFT_data = sp.fftpack.rfft(data_in)
	FFT_base = sp.fftpack.rfft(base)
	FFT_noise = sp.fftpack.rfft(noise)
	FFT_base2 = sp.fftpack.rfft(base2)
	FFT_noise2 = sp.fftpack.rfft(noise2)
	FFT_white = sp.fftpack.rfft(white)

	order = 4
	b, a = invfreqs.invfreqs(FFT_noise, freqs, order, order)
	b, a = sp.signal.bilinear(b, a)



	t = sp.linspace(0, data_in.size / SR_ORIG, data_in.size)
	pylab.subplot(711)
	pylab.plot(t, data_in)
	pylab.subplot(712)
	pylab.plot(freqs, 20*sp.log10(FFT_data), '-')
	pylab.subplot(713)
	pylab.plot(freqs, 20*sp.log10(FFT_base), '-')
	pylab.subplot(714)
	pylab.plot(freqs, 20*sp.log10(FFT_noise), '-')
	pylab.subplot(715)
	pylab.plot(freqs, 20*sp.log10(FFT_base2), '-')
	pylab.subplot(716)
	pylab.plot(freqs, 20*sp.log10(FFT_noise2), '-')
	pylab.subplot(717)
	pylab.plot(freqs, 20*sp.log10(FFT_white), '-')
	pylab.show()

	exit(1)

	# Calculate noise level
	noise_level = np.array([np.abs(x)*2 for x in noise])




	return (base, noise_level)


def decode(base_lo, noise_level_lo):
	base = resampy.resample(base_lo, SR_ORIG/BLOCK_SIZE, SR_ORIG)
	noise_level = resampy.resample(noise_level_lo, SR_ORIG/BLOCK_SIZE, SR_ORIG)
	noise = np.array([noise_level * random.random() for noise_level in noise_level])

	# Highpass filter noise
	noise = butter_filter(noise, 100, SR_ORIG/BLOCK_SIZE, 'high')
	# noise_lo = resampy.resample(noise, SR_ORIG, SR_ORIG/BLOCK_SIZE)
	# noise_hi = resampy.resample(noise_lo, SR_ORIG/BLOCK_SIZE, SR_ORIG)
	# noise = [noise - (noise_hi - noise_level/2) for noise, noise_hi, noise_level in zip(noise, noise_hi, noise_level)]

	ret = np.empty(base.size, dtype=np.int16)
	for i, (base, noise) in enumerate(zip(base, noise)):
		tmp = noise + base
		if tmp < -0x8000:
			tmp = -0x8000
		if tmp > 0x7fff:
			tmp = 0x7fff
		ret[i] = np.round(tmp)

	return ret

def extract_high_freqs(data_in, base_lo):
	base = resampy.resample(base_lo, SR_ORIG/BLOCK_SIZE, SR_ORIG)
	return np.array([np.int16(a - b) for a, b in zip(data_in, base)])


filelist = [
	path.splitext(path.basename(x))[0]
	for x in glob.glob("samples/wav_22k/*.wav")
]

# filelist = ["snare-tape", "hihat-808", "hihat-analog", "hihat-electro", "perc-808", "perc-metal", "snare-808", "snare-electro", "tom-808", "tom-acoustic01"]

for stub in ["snare-808"]: # filelist:

	file_in = "samples/wav_22k/" + stub + ".wav"
	file_raw = "samples/done_raw/" + stub + ".raw"
	file_inc = "samples/done_inc/" + stub + ".inc"
	file_decoded = "samples/done_check/" + stub + ".wav"

	print("Converting " + file_in)

	# Load
	rate, data_in = wavfile.read(file_in)

	# Encode
	(base, noise_level) = encode(data_in)

	# Print compressed data
	# print("base: " + str(base))
	# print("noise_level: " + str(noise_level))

	# Decode
	#data_decoded = decode(base, noise_level)
	data_decoded = extract_high_freqs(data_in, base)
	wavfile.write(file_decoded, rate, data_decoded)

	# todo: compress

	# data_coded[0].tofile(file_raw)


print("Done!")
