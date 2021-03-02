# mapipcd

Project start: 2021 Erick Veil

Map IPC Daemon

A global variable manager with open accessibility between processes.

In Linux, I want to share data between programs easily. I basically want it 
to work like a variable that can be read by multiple, independent processes.

- I do need that data to exist in memory to avoid file lock and races
  between read and write. (Can't use files)
- I do need that data to be accessible from any process I'm running
  (environment variables are out)
- I do need that data to persist when read, so multiple processes can read
  what's been written. (Can't use named pipes)
- I do need that data to be easily written/read using a single command or
  function call. (direct sockets are out).
- I don't need that data to persist between reboot and session.
- I would like a C/C++ interface to natively access the values.

Shared memory is the closest IPC, but uses the unfortunately poorly named function
names and constants common in low level programming, and requires too many
setup commands in a program that uses it. I want to just call out key/value
pairs to easily set and get values and be done with it - I don't care to write
to or read from or have to remember numeric memory addresses. I don't want to
set flags. I just want it to work the same every time.
Do one thing, and not worry about doing the dishes afterwords. The values should 
persist in memory, even after the creating process is destroyed, and so management 
should be indepenant from any of the processes that read or write values.
I don't want to take out the garbage every time I use this.

My solution is to write a daemon that holds the values in memory.
One command will write to a map with a key/value pair.
Another command will read from the map at the provided key.

The ultimate goal is to have the following commands:

sudo systemctl start globstashd
Starts the daemon

globstash set [key] [value]
Writes the value that can be retreived by key.

globstsh get [key]
Reads the value from key and delivers it to stdout.

- All values are saved and output as C text strings.
- Requesting a read from undefined keys will output an empty string.
- Assigning a value to an already defined key will clobber the old value and
  overwrite it.
- All values will be lost if the daemon recieves a SIGTERM.
- No configuration files are necessary.

Some resources:

Start here:
man 7 daemon

New Style Daemon Template:
https://stackoverflow.com/a/59546558

Parent article:
https://stackoverflow.com/questions/17954432/creating-a-daemon-in-linux

More New Style Daemon documentation:
http://0pointer.de/public/systemd-man/daemon.html#New-Style%20Daemons

https://stackoverflow.com/questions/5384168/how-to-make-a-process-daemon

http://www.enderunix.org/docs/eng/daemon.php


TODO:

- What if bad key is requested for get at daemon?
- Make sure daemon clears out if null value sent.
- Make sure client sends null value if empty string passed.

- Create getter and setter program to communicate with mapipcd
- Test gettting and setting values
- Make sure staticlogger outputs WARN to stderr 

- Create a basic daemon that greets the log with hello world on startup
- Give the daemon a command that causes it to write a constant entry to the log
- Give that command an argument that gets saved as a variable, which gets
  written to the log
- Give the daemon a command that will read that saved value and spit it out to
  stdout

- daemonize mapipcd

COMMUNICATION:

The get and set each have their own socket, so there's no parsing of "which
command is this? and we just make use of what's sent. Be lazy about
validation and just log errors about bad formatting when things fail.

To set a key, send a JSON object:

{ 
    "key": "foo",
    "value": "bar"
}

The listner here will just ack with an echo.

To get a value, just request a key:

foo

This listener will ack with the value associated with key.
We will likely ack with a non-printing character if the key is not set.
Try null to start.
If we end up saving bytes, we may want JSON communication here too:

foo

Which will ack with 

{ "value": "bar" }


