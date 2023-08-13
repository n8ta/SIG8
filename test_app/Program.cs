// See https://aka.ms/new-console-template for more information

using System;
using System.Runtime.InteropServices;
using System.Threading;
using HealthKit;
using Sig8;

public class DummyClass {
    public void SomeMethod() {}
}

// Test application for the SigHandler library. Set APP_NAME and FAIL_TYPE env vars when you launch it to control
// what kinds of test it performs and where things are logged.

public static class Program {
    public static int Main() {
        var appName = Environment.GetEnvironmentVariable("APP_NAME") ?? "TESTAPPNAME";
        var crashAppPath = Environment.GetEnvironmentVariable("CRASH_APP_PATH") ?? "/Applications/Calculator.app";
        var failType = Environment.GetEnvironmentVariable("FAIL_TYPE") ?? "nativeSegfault";

        Console.WriteLine($"Main app PID: {Environment.ProcessId}\n" +
                          $"APP_NAME: {appName}\n" +
                          $"FAIL_TYPE: {failType}\n" +
                          $"CRASH_APP_PATH: {crashAppPath}");

        // Make sure we didn't override the .NET runtime sig handlers
        DummyClass? test = null;
        try {
            test.SomeMethod();
        }
        catch (Exception) {
            Console.WriteLine($"Confirmed null reference exceptions are working.");
        }

        var sig = new SigHandler("MYAPPNAME", "/Users/n8ta/run.bash");

        if (failType == "segfault") {
            Marshal.ReadInt32(IntPtr.Zero);
        }

        if (failType == "nativeSegfault") {
            SigHandler.TriggerSegfault();
        }

        Thread.Sleep(100000);

        return 0;
    }
}