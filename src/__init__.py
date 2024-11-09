import math

import numpy as np

from . import _c


def _time_stretch(audio: np.ndarray, sample_rate: int, factor: float) -> np.ndarray:
    num_channels, num_frames = audio.shape
    out = np.empty((num_channels, math.ceil(num_frames * factor)), dtype=audio.dtype)
    _c.time_stretch(audio, num_channels, sample_rate, factor, out)
    return out


def time_stretch(audio: np.ndarray, sample_rate: int, factor: float) -> np.ndarray:
    assert audio.dtype == np.float32
    if audio.ndim == 1:
        return _time_stretch(audio[None], sample_rate, factor).squeeze(0)
    else:
        return _time_stretch(audio, sample_rate, factor)
