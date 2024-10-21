#!/bin/bash
# Split the string into 4-character chunks
# Linux
echo "{BTH_yhw_d1d4s_1t_3vf_3h_g4lt_n05_3hkc4t}!?" | fold -w4 | rev | paste -s -d ''
# MacOSX:
#echo "{BTH_yhw_d1d4s_1t_3vf_3h_g4lt_n05_3hkc4t}!?" | fold -w4 | rev | awk '{ORS=""; print}'
