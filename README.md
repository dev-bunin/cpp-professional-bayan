## Parameters:

```bash
  -h [ --help ]             Show help and close
  -d [ --dir ] arg          Dir for search
  -e [ --exeption ] arg     Exception dir
  -r [ --recursive ]        Recursive search
  -s [ --minsize ] arg (=1) Min mile size
  -m [ --mask ] arg         Filter mask
  --block-size arg (=5)     BlockSize
  --algoritm arg (=crc32)   Hash algoritm (src32, md5)
```

**Examples**: 

`bayan -r -d /home/user/test1 /home/user/test2 -e ignoreDir -s 5 -m ^.*\\.txt$ --block-size 1024 --algoritm md5`
