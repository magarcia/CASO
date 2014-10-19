# Test running 10 times

## filewriter.c
209715200 bytes (209 MB) writed, 5.473221 s, 34.83 MB/s
209715200 bytes (209 MB) writed, 5.529856 s, 37.79 MB/s
209715200 bytes (209 MB) writed, 5.495514 s, 34.83 MB/s
209715200 bytes (209 MB) writed, 6.783533 s, 29.86 MB/s
209715200 bytes (209 MB) writed, 6.665137 s, 31.36 MB/s
209715200 bytes (209 MB) writed, 5.955460 s, 34.83 MB/s
209715200 bytes (209 MB) writed, 6.017469 s, 33.85 MB/s
209715200 bytes (209 MB) writed, 5.144269 s, 40.63 MB/s
209715200 bytes (209 MB) writed, 5.406204 s, 34.83 MB/s
209715200 bytes (209 MB) writed, 5.269142 s, 39.66 MB/s
Average seconds 5.77398 s, average bandwidth 35.24700 MB/s

## dd if=/dev/zero of=output count=3276800 bs=64
209715200 bytes (210 MB) copied, 23.5378 s, 8.9 MB/s
209715200 bytes (210 MB) copied, 18.5749 s, 11.3 MB/s
209715200 bytes (210 MB) copied, 21.6331 s, 9.7 MB/s
209715200 bytes (210 MB) copied, 20.9481 s, 10.0 MB/s
209715200 bytes (210 MB) copied, 20.8841 s, 10.0 MB/s
209715200 bytes (210 MB) copied, 20.2536 s, 10.4 MB/s
209715200 bytes (210 MB) copied, 22.9367 s, 9.1 MB/s
209715200 bytes (210 MB) copied, 20.7375 s, 10.1 MB/s
209715200 bytes (210 MB) copied, 22.1146 s, 9.5 MB/s
209715200 bytes (210 MB) copied, 24.0496 s, 8.7 MB/s
Average seconds 21.56700 s, average bandwidth 9.77000 MB/s

## dd if=/dev/zero of=output count=409600 bs=512
209715200 bytes (210 MB) copied, 3.40283 s, 61.6 MB/s
209715200 bytes (210 MB) copied, 4.21702 s, 49.7 MB/s
209715200 bytes (210 MB) copied, 3.44619 s, 60.9 MB/s
209715200 bytes (210 MB) copied, 3.80277 s, 55.1 MB/s
209715200 bytes (210 MB) copied, 3.82831 s, 54.8 MB/s
209715200 bytes (210 MB) copied, 3.85575 s, 54.4 MB/s
209715200 bytes (210 MB) copied, 4.46081 s, 47.0 MB/s
209715200 bytes (210 MB) copied, 3.25168 s, 64.5 MB/s
209715200 bytes (210 MB) copied, 3.1239 s, 67.1 MB/s
209715200 bytes (210 MB) copied, 3.69804 s, 56.7 MB/s
Average seconds 3.70873 s, average bandwidth 57.18000 MB/s

## dd if=/dev/zero of=output count=25600 bs=8192
209715200 bytes (210 MB) copied, 0.831517 s, 252 MB/s
209715200 bytes (210 MB) copied, 0.885623 s, 237 MB/s
209715200 bytes (210 MB) copied, 2.49205 s, 84.2 MB/s
209715200 bytes (210 MB) copied, 1.92917 s, 109 MB/s
209715200 bytes (210 MB) copied, 1.92935 s, 109 MB/s
209715200 bytes (210 MB) copied, 2.13844 s, 98.1 MB/s
209715200 bytes (210 MB) copied, 1.07686 s, 195 MB/s
209715200 bytes (210 MB) copied, 1.26276 s, 166 MB/s
209715200 bytes (210 MB) copied, 1.20504 s, 174 MB/s
209715200 bytes (210 MB) copied, 2.84338 s, 73.8 MB/s
Average seconds 1.65942 s, average bandwidth 149.81000 MB/s

