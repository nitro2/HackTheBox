import struct

#Start	   plc_address	Function	Data	CRC	End
#3.5 Char 	8 Bit	 8 Bit	  N * 8Bit	16 Bit
# Define the format string for the struct
coil ={
    "in_valve":12,
    "out_valve":21,
    "start":33,
    "manual_mode_control":9947,
    "cutoff":5,
    "cutoff_in":26,
    "force_start_out":52,
    "force_start_in":1336,
}
ON = 0xFF00
OFF = 0x0000

format_string = ">BBHH"

def command(c, action):
    coil[c]
    plc_address = 82 # 1 byte
    function = 5 # 1 byte - write
    coil_address =  coil[c] # 2 bytes
    data =  action # 2 bytes

    packed_data = struct.pack(format_string, plc_address, function, coil_address, data)
    print(packed_data.hex())

command("start", ON)
command("manual_mode_control", OFF)
command("cutoff", OFF)
command("cutoff_in", ON)
command("force_start_out", ON)
command("force_start_in", OFF)
