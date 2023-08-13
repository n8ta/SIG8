# What is this

This is a C dynamic library designed to be loaded before the .NET runtime by the linker via the DYLD_INSERT_LIBRARIES env var.

When this lib loads it registers its own segfault handler that will then be overwritten by .NET. Our own signal handler will be called
by .NET when .NET encounters a segfault in non-managed code.

When our signal handler is it, it does some basic logging, launches a new process to sample the crashing process, and launches a crash handler
app supplied by the user. This crash handler app can then use the SIG8_CRASH_DUMP_PATH env var to locate the crash data and persist it.

# Structure

- fsutils.c: Filesystem utilities
- handler.c: Manages signal handling during library and init and communication with primary c# app
- launcher.c: Launches the crash handler app with appropriate env vars
- logger.c: Called by handler.c during a crash to log the crash.
- metalogger.c: The crash handler's own per-crash logging. Used for debugging the crash handler itself.
- sampler.c: Launches a new process to sample the crashing process
- segfault.c: Provides a trigger_segfault() function that can be called from c# to trigger a non-managed segfault.
- main.c: Useful for testing the  crash handler independently of a c# app.

# Building

`./build.bash` builds x86_64 + arm64 libraries and then combines them into one fat library `handler.dylib` to be consumed from c#.

It also builds that same handler + the main.c file to aid in debugging.

# Use from c#

The c# app should call `sig_handler_set_app_name` and `sig_handler_set_app_callback_app` as early as possible. Until the app name is set crashes are not logged.