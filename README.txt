python-lzf: liblzf Python bindings

This package is a direct translation of the C API of liblzf into Python.
It provides two core functions: compress() and decompress().

- compress(data: bytes, max_length: Optional[int] = None) -> Optional[bytes]  
  Compresses the given input bytes. Optionally, a maximum length for the output
  may be specified. If the data cannot be compressed to fit within the specified
  size, the function returns None. If no size is given, the default is one less
  than the length of the input, so the caller must always be prepared to handle
  a return value of None.

- decompress(data: bytes, expected_size: int) -> Optional[bytes]  
  Decompresses the given input bytes and attempts to fit the result into the
  specified uncompressed size. If the result does not fit, the function returns None.

This module is intended as a low-level binding for applications that need fast
compression and decompression for small blocks of data.

Special thanks to teepark for years of selfless maintenance and support of this project.
