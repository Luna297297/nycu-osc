import struct
import sys


if len(sys.argv) != 3:
    print("Lack of argument, process terminate!")
    sys.exit(1)


serial_port = sys.argv[1]
print("Current Serial Port is: " + serial_port)
kernel_img = sys.argv[2]

# get magic number
magic = 0x544F4F42
pattern = struct.pack("<I", magic)

# get kernel size
with open(kernel_img, "rb") as kernel_file:
    kernel_data = kernel_file.read()
kernel_size = len(kernel_data)
kernel_size_byte = struct.pack("<I", len(kernel_data))

"""header = struct.pack("<II",
    0x544F4F42,
    kernel_size,
)"""

with open(serial_port, "wb", buffering=0) as tty:
    bytes_written = tty.write(pattern) # 回傳寫入多少 bytes
    print("Bytes written: ", bytes_written)
    print(pattern.hex())

    if len(pattern) != bytes_written:
        print(f"Incomplete write: expected {len(pattern)} bytes, wrote {bytes_written} bytes")
        sys.exit(1)
    
    kernel_byte_written = tty.write(kernel_size_byte)
    print("Kernel Size Bytes written: ", kernel_byte_written)
    print(kernel_size_byte.hex())

    if len(kernel_size_byte) != kernel_byte_written:
        print(f"Incomplete kernel size write: expected {len(kernel_size_byte)} bytes, wrote {kernel_byte_written} bytes")
        sys.exit(1)

    total_written = 0

    while total_written < kernel_size:
        written = tty.write(kernel_data[total_written:])

        if written == 0:
            print("Failed to write kernel data.")
            sys.exit(1)

        total_written += written
    print("Kernel data bytes written: ", total_written)

    print("Kernel first 4 bytes:", kernel_data[:4].hex())

