#!/bin/env python3
import math

print("#pragma once");
print("namespace tables {")

# NOTE: This script is currently broken. Fix it!

# Outputs C++ defining a table of integers
def output(type, name, list):
	print("static const " + type + " " + name + "["+ str(len(list)) + "] = {")
	print(','.join([str(x) for x in list]))
	print("};")


# Call func with values from 0 to 1, using approptiate steps, producing a list
def listify(func, len):
	return [func(x/len) for x in range(0, len)]


# Normalize range 0-1 to fit in an int8_t
def normalize_int8(x):
	return int(127.99 * x)


# Bassdrum waveform
output("int8_t", "bd", listify( 
	lambda x: normalize_int8(
		math.sin(x * 2*math.pi)
	),
	256
))

# Pitch table
# output("uint8_t", 
# 	table_uint8(12, "pitch", lambda x: (x/12)**2 )
# )

print("} // namespace tables")