```markdown
# DirSlam

DirSlam is a simple directory enumeration tool written in C++. It uses libcurl to make HTTP requests to a target URL and checks if certain directories exist.

## Usage

```bash
./dirslam -l https://website.com/ -w wordlist.txt
```

In the command above, `-l` specifies the target URL and `-w` specifies the path to the wordlist file.

## Wordlist

The wordlist file should contain a list of directory names, one per line. For example:

```
index
images
download
2006
news
...
```

When you run DirSlam, it will append each directory name to the target URL and make an HTTP request to that URL. If the server responds with a 200 status code, DirSlam will print a message indicating that the directory exists.

## Building

To build DirSlam, you need a C++ compiler and libcurl. Once you have these installed, you can build DirSlam with the following command:

```bash
g++ -o dirslam DirSlam.cpp -lcurl
```

## License

This project is licensed under the [MIT License](LICENSE).
```
