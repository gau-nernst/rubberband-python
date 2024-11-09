import math

import numpy as np
import pytest

import rubberband_python

audio_mono = np.random.randn(16_000).astype(np.float32)
audio_stereo = np.random.randn(2, 16_000).astype(np.float32)
AUDIO = [audio_mono, audio_stereo]


@pytest.mark.parametrize("audio", AUDIO)
@pytest.mark.parametrize("time_ratio", [1.0, 0.8, 1.2])
@pytest.mark.parametrize("pitch_scale", [1.0, 0.8, 1.2])
def test_rubberband(audio, time_ratio, pitch_scale):
    out = rubberband_python.rubberband(audio, 16_000, time_ratio, pitch_scale)
    expected_len = math.ceil(audio.shape[-1] * time_ratio)
    expected_shape = audio.shape[:-1] + (expected_len,)
    assert out.shape == expected_shape
