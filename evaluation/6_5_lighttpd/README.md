# Lighttpd

In this folder, we provide:
- Our patch to lighttpd 1.4.75 that implements PreFence on plugin level
- A patch to the httpit benchmark that writes the benchmark results to a logfile (not only to a dynamic TUI that is hard to capture automatically)
- Two example web applications
- An example configuration file for the web server

## Obtaining, patching and compiling lighttpd1.4

To reproduce our experiments, we recommend having a unpatched ("stock") copy and a patched copy of the [lighttpd web server](https://github.com/lighttpd/lighttpd1.4) available. To set them up, perform the following steps in this folder:

```
git clone --depth=1 --branch=lighttpd-1.4.75 https://github.com/lighttpd/lighttpd1.4.git lighttpd1.4-stock
cp -r lighttpd1.4-stock lighttpd1.4-prefence
cd lighttpd1.4-prefence
git apply ../lighttpd_1.4.75_prefence.patch
cd ..
```

Next, we need to compile both copies of lighttpd from source. The build process is [documented in detail by the lighttpd authors](https://redmine.lighttpd.net/projects/lighttpd/wiki/InstallFromSource#build-prerequisites). That page also lists the required prerequisites. On our Debian-based system, we had to install the following packages:

```
sudo apt install build-essential autoconf automake m4 libtool pkg-config libpcre2-dev libssl-dev zlib1g-dev
```

The required packages and how to install them may differ depending on the Linux distribution used.

Afterward, we performed the following steps to compile both copies of lighttpd:

```
cd lighttpd1.4-stock
./autogen.sh
./configure --with-openssl
make -j$(nproc)
cd ..
cd lighttpd1.4-prefence
./autogen.sh
./configure --with-openssl
make -j$(nproc)
```

The main binaries can then be found in `lighttpd1.4-{stock,prefence}/src/lighttpd`. The modules are compiled in ` in `lighttpd1.4-{stock,prefence}/src/.libs`.

## Obtaining, patching and compiling httpit

By default, the [httpit benchmark](https://github.com/gonetx/httpit) shows results in a dynamic terminal-based UI (TUI). This UI is hard to capture automatically, so we extended httpit to (optionally) write the final benchmark results to a log file as well. The path to this logfile is specified in a new `--logfile` command line flag.

As httpit is written in Go, make sure to install the Go compiler on your operating system first. Then follow these steps to obtain a copy of httpit and apply our logfile patch:

```
git clone --branch=v0.5.0 https://github.com/gonetx/httpit httpit-with-log-output
cd httpit-with-log-output
git apply ../httpit_v0.5.0-logfile.patch
go build
```

If the build process completes successfully, there will be a binary called `httpit` in the `httpit-with-log-output` folder. When this binary is run without any parameters, it prints a help message to stderr. If our patch was applied successfully, this message should list the `--logfile` flag.

## Web server configuration, web applications, benchmarking

We provide an example configuration file the `data/` directory, as well as a few shell scripts to run the lighttpd server (either patched or stock) with this configuration file.

The web server configuration file, `data/lighttpd.conf`, contains some absolute file paths. These paths need to be adjusted to point to the correct directories. The configuration file sets up the web server to serve the two web applications that we explain in the following sections.

In addition, we provide a benchmark script (`bench_progressive.sh`) that collects the data required to plot Figure 7 in the paper, as well as the plot script `plot-python-progressive.py` to plot that figure from the collected data. It starts the web server, generates a database (for the Python-based web application), and runs the httpit benchmark.

### 1. C++: prefetchable

This web application is written in C++ and searches the minimum value in a random column of a 2D array that is hard-coded into the program (and thus kept in RAM at runtime). We assume that this workload can easily be prefetched. We used this workload only for testing.

This application is exposed through CGI at the following URL: `https://localhost:8443/index.prefetchable`.

### 2. Python: index-min.py

This web application was evaluated in the paper. It essentially does the same as the C++ application, but replaces the hard-coded in-memory data structure with a SQLite database. This database can be generated with the script `data/htdocs/generate.py`. Our benchmark script `data/bench_progressive.sh` collects data for databases of different sizes and calls `generate.py` automatically to generate this file.

This application is exposed through CGI at the following URL: `https://localhost:8443/index-min.py`.