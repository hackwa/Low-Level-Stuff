#!/usr/bin/python3

import hashing

ptolemy = hashing.hashStore()
a= ["QCOjOIWkOP","UGJFNPXORI","VdEIAYyrns","TFAYSanwpB","YxpidxRLPO","UbInFTCTtp","ZCcxVbbGUf","TviKMJhdJw","ebWgeupjjx","SALaKukDRT"] * 100000
length = len(a)
print("Benchmarking hashfunc for", length, "strings")
for i in range(length):
    hashValue=ptolemy.Murmur(a[i])
print(ptolemy.cacheObject)

# No caching
# Benchmarking hashfunc for 1000000 strings
#./hashBenchmark.py  9.15s user 0.00s system 99% cpu 9.199 total
# Caching :/
#./hashBenchmark.py  13.39s user 0.00s system 99% cpu 13.437 total
