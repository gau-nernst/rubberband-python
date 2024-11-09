import math

import numpy as np

from . import _c


def _rubberband(
    audio: np.ndarray,
    sample_rate: int,
    time_ratio: float,
    pitch_scale: float,
) -> np.ndarray:
    num_channels, num_frames = audio.shape
    out = np.empty((num_channels, math.ceil(num_frames * time_ratio)), dtype=audio.dtype)
    _c.rubberband(audio, num_channels, sample_rate, time_ratio, pitch_scale, out)
    return out


def rubberband(
    audio: np.ndarray,
    sample_rate: int,
    time_ratio: float = 1.0,
    pitch_scale: float = 1.0,
) -> np.ndarray:
    assert audio.dtype == np.float32
    if audio.ndim == 1:
        return _rubberband(audio[None], sample_rate, time_ratio, pitch_scale).squeeze(0)
    else:
        return _rubberband(audio, sample_rate, time_ratio, pitch_scale)


def time_stretch(audio: np.ndarray, sample_rate: int, ratio: float) -> np.ndarray:
    return rubberband(audio, sample_rate, time_ratio=ratio)


def frequency_scale(audio: np.ndarray, sample_rate: int, scale: float) -> np.ndarray:
    return rubberband(audio, sample_rate, pitch_scale=scale)


def pitch_shift(audio: np.ndarray, sample_rate: int, semitones: float) -> np.ndarray:
    scale = 2.0 ** (semitones / 12)
    return rubberband(audio, sample_rate, pitch_scale=scale)
