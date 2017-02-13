import numpy as np
import scipy as sp
from scipy.io import wavfile
from pprint import pprint
from scipy.signal import butter, lfilter, freqz
import random
import resampy
import glob
from os import path

BLOCK_SIZE = 8
SR_ORIG = 22050


# AVERAGE_BITS = 8
# NOISE_BITS = 8


class Rand:
	def __init__(self, seed):
		self.state = random.Random(seed)

	def get_s8(self):
		return np.int8(self.state.randint(-128, 127))


def butter_lowpass(cutoff, fs, order=5):
    nyq = 0.5 * fs
    normal_cutoff = cutoff / nyq
    b, a = butter(order, normal_cutoff, btype='high', analog=False)
    return b, a

def butter_lowpass_filter(data, cutoff, fs, order=5):
    b, a = butter_lowpass(cutoff, fs, order=order)
    y = lfilter(b, a, data)
    return y

def encode(data_in):
	# Pad input array to BLOCK_SIZE
	blocks = int(np.ceil(data_in.size / BLOCK_SIZE))
	data_in = np.append(data_in, np.zeros(blocks * BLOCK_SIZE - data_in.size, dtype=np.int16))

	# Down-sample, then up-sample again
	base_lo = resampy.resample(data_in.astype(np.float), SR_ORIG, SR_ORIG/BLOCK_SIZE)
	base_upsampled = resampy.resample(base_lo, SR_ORIG/BLOCK_SIZE, SR_ORIG)

	# Generate array of noise amount
	noise_level_hi = np.array([np.abs(a - b) for a, b in zip(data_in, base_upsampled)])
	noise_level_lo = resampy.resample(noise_level_hi, SR_ORIG, SR_ORIG/BLOCK_SIZE)

	# Check some things
	assert data_in.size == noise_level_hi.size and base_lo.size == blocks

	# Quantize
	# averages_i = np.array([int(np.round(x * 2**(AVERAGE_BITS-1))) for x in averages])
	# noises_i = np.array([int(np.round(x * 2**NOISE_BITS)) for x in noises])

	return (base_lo, noise_level_lo)


def decode(base_lo, noise_level_lo):
	base = resampy.resample(base_lo, SR_ORIG/BLOCK_SIZE, SR_ORIG)
	noise_level = resampy.resample(noise_level_lo, SR_ORIG/BLOCK_SIZE, SR_ORIG)
	noise = np.array([noise_level * random.random() for noise_level in noise_level])

	# Highpass filter noise
	noise = butter_lowpass_filter(noise, 100, SR_ORIG/BLOCK_SIZE)
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


filelist = [
	path.splitext(path.basename(x))[0]
	for x in glob.glob("samples/wav_22k/*.wav")
]

filelist = ["snare-tape", "hihat-808", "hihat-analog", "hihat-electro", "perc-808", "perc-metal", "snare-808", "snare-electro", "tom-808", "tom-acoustic01"]

for stub in filelist:

	file_in = "samples/wav_22k/" + stub + ".wav"
	file_raw = "samples/done_raw/" + stub + ".raw"
	file_inc = "samples/done_inc/" + stub + ".inc"
	file_decoded = "samples/done_check/" + stub + ".wav"

	print("Converting " + file_in)

	# Load
	rate, data_in = wavfile.read(file_in)

	# Encode
	(averages, noises) = encode(data_in)

	# Print compressed data
	# print("Averages: " + str(averages))
	# print("Noises: " + str(noises))

	# Decode
	data_decoded = decode(averages, noises)
	wavfile.write(file_decoded, rate, data_decoded)

	# todo: compress

	# data_coded[0].tofile(file_raw)


print("Done!")