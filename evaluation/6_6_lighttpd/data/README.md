- Build `pfdisset`:
```
cmake -B pfdisset/build pfdisset
make -C pfdisset/build
```
- Make sure PHP-CGI is installed. In `php.ini`, set `upload_max_filesize` to `2G` and `post_max_size` to `3G`.
- Make sure the `server.document-root` path in `lighttpd.conf` points to the htdocs folder in this directory
- Run `prepare_files.sh` to generate test files to upload
- Edit `benchmark.sh`, select the configuration(s) you want to test
- Run `benchmark.sh`, which calls `benchmark.py` and the `run-*.sh` scripts internally.

