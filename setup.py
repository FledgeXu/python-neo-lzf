import sys
from pathlib import Path

from setuptools import Extension, setup

VERSION = (0, 3, 4)

compile_args = ["/W3"] if sys.platform == "win32" else ["-Wall"]
long_description = Path("README.txt").read_text(encoding="utf-8")

setup(
    name="python-neo-lzf",
    description="A fork of python-lzf with pre-built wheel files.",
    long_description=long_description,
    long_description_content_type="text/plain",
    version=".".join(map(str, VERSION)),
    author="Fledge Shiu",
    author_email="xzk0701@gmail.com",
    url="https://github.com/FledgeXu/python-neo-lzf",
    ext_modules=[
        Extension(
            "lzf",
            ["lzf_module.c", "lzf_c.c", "lzf_d.c"],
            include_dirs=["."],
            extra_compile_args=compile_args,
        )
    ],
    license="BSD-3-Clause",
    classifiers=[
        "Development Status :: 5 - Production/Stable",
        "Intended Audience :: Developers",
        "Natural Language :: English",
        "Operating System :: OS Independent",
        "Programming Language :: Python :: 3",
        "Programming Language :: Python :: 3.7",
        "Programming Language :: Python :: 3.8",
        "Programming Language :: Python :: 3.9",
        "Programming Language :: Python :: 3.10",
        "Programming Language :: Python :: 3.11",
        "Programming Language :: Python :: 3.12",
        "Programming Language :: Python :: 3.13",
        "Programming Language :: Python :: 3.14",
        "Programming Language :: C",
        "Topic :: System :: Archiving :: Compression",
    ],
    python_requires=">=3.7",
)
