using System.Runtime.InteropServices;

namespace Sig8;
public class SigHandler {
    [DllImport("handler", CallingConvention = CallingConvention.Cdecl)]
    public static extern void sig_handler_set_app_name(string appName, int length);
    [DllImport("handler", CallingConvention = CallingConvention.Cdecl)]
    public static extern void sig_handler_set_app_callback_app(string crashAppPath, int crashAppPathLen);
    [DllImport("handler", CallingConvention = CallingConvention.Cdecl)]
    public static extern void sig_handler_trigger_segfault();

    private string _appName;

    /// <summary>
    /// Provides app specific data to the signal handler so crashes can be logged in uniq dir.
    /// </summary>
    /// <param name="uniqAppName">A string unique to your application. If it collides with another app on the system you may receive their crash reports. Keep it <= 40 in length and using a-Z,0-9 chars only.</param>
    /// <param name="crashReporterPath">Optional. An application to launch when your app crashes. It will be launched with ENV vars pointing you to the crash data.</param>
    /// <returns></returns>
    public SigHandler(string uniqAppName, string? crashReporterPath = null) {
        if (uniqAppName.Length > 40) {
            throw new Exception("A long app name risks reaching path length limits. Decrease it.");
        }

        if (!uniqAppName.All(char.IsLetterOrDigit)) {
            throw new Exception("Only use a-Z and 0-9 for app names to avoid any issues with filesystem allowed chars");
        }

        sig_handler_set_app_name(uniqAppName, uniqAppName.Length);
        
        if (!string.IsNullOrEmpty(crashReporterPath)) {
            sig_handler_set_app_callback_app(crashReporterPath, crashReporterPath.Length);
        }

        _appName = uniqAppName;
    }

    /// <summary>
    /// Call this method (ONLY) from your crash handler app to retrieve the current crash
    /// </summary>
    /// <returns></returns>
    public static void RetrieveCurrentCrash(string uniqAppName) {
        // TODO: IMPL
        string dir = Environment.GetEnvironmentVariable("SIG8_CRASH_DUMP_PATH");
        throw new NotImplementedException();
    }
    
    /// <summary>
    /// Call this method at app launch to retrieve unhandled crashes. This method should cleanup those crashes as well.
    /// </summary>
    /// <returns></returns>
    public void RetrievePreviousCrashes() {
        // TODO: Use app name to determine crash dir and loop through all crashes found in it
        Console.WriteLine($"~/Library/Application Support/SIG8/{_appName}/ has 1 dir per crash.");
        throw new NotImplementedException();
    }
    
    /// <summary>
    /// Triggers a segfault in non-managed code.
    /// </summary>
    public static void TriggerSegfault() {
        sig_handler_trigger_segfault();
    }
}