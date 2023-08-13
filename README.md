# SigHandler

A reliable and extensible macos .NET Core native crash handler.

## Usage

Add a ProjectReference to sig_handler in your project and import its target.
```msbuild
<Import Project="../sig_handler/sig_handler.targets" />
<PropertyGroup>
    <AppName>test_app</AppName>
</PropertyGroup>
<ItemGroup>
    <ProjectReference Include="..\sig_handler\sig_handler.csproj"/>
</ItemGroup>
```

Call setup method 
```c#
using Sig8;

var sig = new SigHandler("MYAPPNAME", "/System/Applications/Calculator.app");
```

This will open the calculator on segfault. You can replace calculator with your own crash handler app. In the crash handler app
you can use `SigHandler.RetrieveCurrentCrash()` to get information about the current crash and `SigHandler.RetrievePreviousCrashes()` to get information about all previous crashes next time the app is launched.

## File system

Crashes are logged in /Users/<user>/Library/Application Support/FML/<app_name>/<crash_id>/...

## TODO
1. Optional. Ensure that the sampler has exited before the crash app reads the sample log file. Maybe by creating a
`done` file in the crash. Or we could just wait 3s.
2. Implement RetrieveCurrentCrash.
3. Implement RetrievePreviousCrashes
4. Signal re-rentrancy. What happens if this signal handler is interrupted by a second signal? 
Should we block all signals?