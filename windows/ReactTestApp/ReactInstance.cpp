#include "pch.h"

#include "ReactInstance.h"

#include <filesystem>

#include <winrt/Windows.Storage.h>
#include <winrt/Windows.Web.Http.Headers.h>

using winrt::Microsoft::ReactNative::ReactInstanceSettings;
using winrt::Windows::Foundation::IAsyncOperation;
using winrt::Windows::Foundation::PropertyValue;
using winrt::Windows::Foundation::Uri;
using winrt::Windows::Storage::ApplicationData;
using winrt::Windows::Web::Http::HttpClient;

namespace
{
    winrt::hstring const kBreakOnFirstLine = L"breakOnFirstLine";
    winrt::hstring const kUseDirectDebugger = L"useDirectDebugger";
    winrt::hstring const kUseFastRefresh = L"useFastRefresh";
    winrt::hstring const kUseWebDebugger = L"useWebDebugger";

    bool RetrieveLocalSetting(winrt::hstring const &key, bool defaultValue)
    {
        auto localSettings = ApplicationData::Current().LocalSettings();
        auto values = localSettings.Values();
        return winrt::unbox_value_or<bool>(values.Lookup(key), defaultValue);
    }

    void StoreLocalSetting(winrt::hstring const &key, bool value)
    {
        auto localSettings = ApplicationData::Current().LocalSettings();
        auto values = localSettings.Values();
        values.Insert(key, PropertyValue::CreateBoolean(value));
    }
}  // namespace

namespace ReactTestApp
{
    void ReactInstance::LoadJSBundleFrom(JSBundleSource source)
    {
        auto instanceSettings = reactNativeHost_.InstanceSettings();
        switch (source) {
            case JSBundleSource::DevServer:
                instanceSettings.JavaScriptMainModuleName(L"index");
                instanceSettings.JavaScriptBundleFile(L"");
                break;
            case JSBundleSource::Embedded:
                instanceSettings.JavaScriptBundleFile(winrt::to_hstring(GetBundleName()));
                break;
        }

        Reload();
    }

    void ReactInstance::Reload()
    {
        auto instanceSettings = reactNativeHost_.InstanceSettings();

        instanceSettings.UseWebDebugger(UseWebDebugger());
        instanceSettings.UseDirectDebugger(UseDirectDebugger());

        auto useFastRefresh = UseFastRefresh();
        instanceSettings.UseFastRefresh(useFastRefresh);
        instanceSettings.UseLiveReload(useFastRefresh);

        // instanceSettings.EnableDeveloperMenu(false);

        reactNativeHost_.ReloadInstance();
    }

    bool ReactInstance::BreakOnFirstLine() const
    {
        return RetrieveLocalSetting(kBreakOnFirstLine, false);
    }

    void ReactInstance::BreakOnFirstLine(bool breakOnFirstLine)
    {
        StoreLocalSetting(kBreakOnFirstLine, breakOnFirstLine);
        Reload();
    }

    bool ReactInstance::UseDirectDebugger() const
    {
        return RetrieveLocalSetting(kUseDirectDebugger, false);
    }

    void ReactInstance::UseDirectDebugger(bool useDirectDebugger)
    {
        if (useDirectDebugger) {
            // Remote debugging is incompatible with direct debugging
            StoreLocalSetting(kUseWebDebugger, false);
        }
        StoreLocalSetting(kUseDirectDebugger, useDirectDebugger);
        Reload();
    }

    bool ReactInstance::UseFastRefresh() const
    {
        return RetrieveLocalSetting(kUseFastRefresh, true);
    }

    void ReactInstance::UseFastRefresh(bool useFastRefresh)
    {
        StoreLocalSetting(kUseFastRefresh, useFastRefresh);
        Reload();
    }

    bool ReactInstance::UseWebDebugger() const
    {
        return RetrieveLocalSetting(kUseWebDebugger, false);
    }

    void ReactInstance::UseWebDebugger(bool useWebDebugger)
    {
        if (useWebDebugger) {
            // Remote debugging is incompatible with direct debugging
            StoreLocalSetting(kUseDirectDebugger, false);
        }
        StoreLocalSetting(kUseWebDebugger, useWebDebugger);
        Reload();
    }

    std::string GetBundleName()
    {
        std::vector entryFileNames = {"index.windows",
                                      "main.windows",
                                      "index.native",
                                      "main.native",
                                      "index"
                                      "main"};

        for (std::string &&n : entryFileNames) {
            std::string path = "Bundle\\" + n + ".bundle";
            if (std::filesystem::exists(path)) {
                return n;
            }
        }

        return "";
    }

    IAsyncOperation<bool> IsDevServerRunning()
    {
        Uri uri(L"http://localhost:8081/status");
        HttpClient httpClient;
        try {
            auto r = co_await httpClient.GetAsync(uri);
            co_return r.IsSuccessStatusCode();
        } catch (winrt::hresult_error &) {
            co_return false;
        }
    }

}  // namespace ReactTestApp
