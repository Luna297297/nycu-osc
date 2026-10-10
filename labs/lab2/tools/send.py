import sys

if len(sys.argv) != 2:
    print("Lack of argument, process terminate!")
    sys.exit(1)


serial_port = sys.argv[1]
print("Current Serial Port is: " + serial_port)

pattern = bytes([0x00, 0x0A, 0x0D, 0xFF])
print(pattern.hex())

with open(serial_port, "wb", buffering=0) as tty:
    bytes_written = tty.write(pattern) # 回傳寫入多少 bytes
    print("Bytes written:", bytes_written)

    if len(pattern) != bytes_written:
        print("Incomplete write: expected " + len(pattern) + " bytes, wrote " + bytes_written + " bytes")
        sys.exit(1)

