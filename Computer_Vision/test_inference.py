from inference_pipeline import *
import numpy as np
import pytest


def test_crop_frame():
	test_array = np.random.rand(500,500,3)
	comparison = crop_frame(test_array, 10, 10, 10, 10) == test_array[10:-10,10:-10,:]
	assert comparison.all(), 'The crop function is not working properly'

	return