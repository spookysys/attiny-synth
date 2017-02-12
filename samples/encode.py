import numpy as np
import scipy as sp
from scipy.io import wavfile
from pprint import pprint
import random


BLOCK_SIZE = 8
AVERAGE_BITS = 8
NOISE_BITS = 8

# This repeats too quickly
class Rand8:
	def __init__(self, seed):
		self.state = seed

	def get_int(self):
		carry = (self.state >> 7) & 1
		self.state = (self.state << 1) & 0xFF
		if ((self.state != 0) and (carry != 0)) or ((self.state == 0) and (carry == 0)):
			self.state = self.state ^ 0x1d
		return self.state

	def get_s8(self):
		return np.int8(self.get_int())


class Rand:
	def __init__(self, seed):
		self.state = random.Random(seed)

	def get_s8(self):
		return np.int8(self.state.randint(-128, 127))


def encode(data_in):
	blocks = int(np.ceil(data_in.size / BLOCK_SIZE))
	data_in = np.append(data_in, np.zeros(blocks * BLOCK_SIZE - data_in.size, dtype=np.int16))

	averages = np.empty(blocks, dtype=float)
	noises = np.empty(blocks, dtype=float)

	for i in range(blocks):
		vals = data_in[i*BLOCK_SIZE:(i+1)*BLOCK_SIZE]
		averages[i] = average = np.mean(vals, dtype=np.float)
		noises[i] = noise = np.mean([np.abs(x - average) for x in vals], dtype=np.float)

	# Normalize
	# -> Sum of average and noise may not exceed [-1.0:1.0]
	max_vol = np.max(np.abs(averages) + noises)
	averages = np.multiply(averages, 1. / max_vol)
	noises = np.multiply(noises, 1. / max_vol)

	# Quantize
	averages_i = np.array([int(np.round(x * 2**(AVERAGE_BITS-1))) for x in averages])
	noises_i = np.array([int(np.round(x * 2**NOISE_BITS)) for x in noises])

	return (averages_i, noises_i)

def decode(averages, noises):
	rand = Rand(0)
	data_decoded = np.zeros(averages.size * BLOCK_SIZE, dtype=np.int16)
	for block_i, (average, noise) in enumerate(zip(averages, noises)):
		noise_samples = [np.float(rand.get_s8()) for x in range(BLOCK_SIZE)]
		noise_samples_mean = np.mean(noise_samples)
		noise_scaler = noise / 2.**NOISE_BITS
		average_scaler = 128 / 2.**(AVERAGE_BITS-1)
		samples = [
			(noise_sample - noise_samples_mean) * noise_scaler + average * average_scaler
			for noise_sample in noise_samples
		]
		for sample_i, sample in enumerate(samples):
			#assert sample >= -128 and sample <= 127
			if sample < -128:
				sample = -128
			if sample > 127:
				sample = 127
			sample = np.int16(np.round(sample))
			data_decoded[block_i * BLOCK_SIZE + sample_i] = sample * 256 + sample
	return data_decoded

for stub in ["snare-tape", "hihat-808", "hihat-analog", "hihat-electro", "perc-808", "perc-metal", "snare-808", "snare-electro", "tom-808", "tom-acoustic01"]:
	# stub = "snare-tape"
	file_in = "gen_samples/step1/" + stub + ".wav"
	file_raw = "gen_samples/done_raw/" + stub + ".raw"
	file_inc = "gen_samples/done_inc/" + stub + ".inc"
	file_decoded = "gen_samples/done_check/" + stub + ".wav"

	print("Converting " + file_in)

	# Load
	rate, data_in = wavfile.read(file_in)

	# Encode
	(averages, noises) = encode(data_in)

	# Print compressed data
	print("Averages: " + str(averages))
	print("Noises: " + str(noises))

	# Decode
	data_decoded = decode(averages, noises)
	wavfile.write(file_decoded, rate, data_decoded)

	# todo: compress

	# data_coded[0].tofile(file_raw)


