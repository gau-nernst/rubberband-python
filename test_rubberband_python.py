import math

import numpy as np
import pytest
from rubberband_python import time_stretch


@pytest.mark.parametrize("factor", [1.0, 0.8, 1.2])
def test_time_stretch(factor):
    audio = np.random.rand(16_000).astype(np.float32)
    out = time_stretch(audio, 16_000, factor)
    assert out.shape == (math.ceil(audio.shape[0] * factor),)
