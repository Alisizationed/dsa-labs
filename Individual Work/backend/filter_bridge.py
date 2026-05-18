import ctypes
import json
from pathlib import Path

_LIB_DIR = Path(__file__).parent
_LIB_PATH = _LIB_DIR / "prefilter.dll"

# More realistic escalation threshold
_BASE_THRESHOLD = 0.12


def _load_lib() -> ctypes.CDLL:
    if not _LIB_PATH.exists():
        raise FileNotFoundError(
            f"prefilter.dll not found at {_LIB_PATH}. "
            "Compile prefilter.c into prefilter.dll first."
        )

    lib = ctypes.CDLL(str(_LIB_PATH))

    lib.analyse_text.argtypes = [
        ctypes.c_char_p,
        ctypes.c_char_p,
        ctypes.c_int,
    ]
    lib.analyse_text.restype = ctypes.c_int

    return lib


_lib = _load_lib()


def run_prefilter(text: str) -> dict:
    buf = ctypes.create_string_buffer(4096)

    rc = _lib.analyse_text(
        text.encode("utf-8", errors="replace"),
        buf,
        4096
    )

    if rc != 0:
        raise RuntimeError("prefilter buffer overflow — text too large")

    return json.loads(buf.value.decode("utf-8"))

def needs_llm(text: str) -> tuple[bool, dict]:
    scores = run_prefilter(text)
    return scores["should_escalate"], scores