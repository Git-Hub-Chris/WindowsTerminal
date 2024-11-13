// Copyright (c) Microsoft Corporation.
// Licensed under the MIT license.

#include "pch.h"
#include "Extensions.h"
#include "Extensions.g.cpp"
#include "ExtensionsViewModel.g.cpp"

#include <LibraryResources.h>
#include "..\WinRTUtils\inc\Utils.h"

using namespace winrt::Windows::UI::Xaml;
using namespace winrt::Windows::UI::Xaml::Controls;
using namespace winrt::Windows::UI::Xaml::Navigation;

namespace winrt::Microsoft::Terminal::Settings::Editor::implementation
{
    Extensions::Extensions()
    {
        InitializeComponent();
    }

    void Extensions::OnNavigatedTo(const NavigationEventArgs& e)
    {
        _ViewModel = e.Parameter().as<Editor::ExtensionsViewModel>();
    }

    void Extensions::ExtensionLoaded(const Windows::Foundation::IInspectable& sender, const Windows::UI::Xaml::RoutedEventArgs& /*args*/)
    {
        const auto& toggleSwitch = sender.as<Controls::ToggleSwitch>();
        const auto& extensionSource = toggleSwitch.Tag().as<hstring>();
        toggleSwitch.IsOn(_ViewModel.GetExtensionState(extensionSource));
    }

    void Extensions::ExtensionToggled(const Windows::Foundation::IInspectable& sender, const Windows::UI::Xaml::RoutedEventArgs& /*args*/)
    {
        const auto& toggleSwitch = sender.as<Controls::ToggleSwitch>();
        const auto& extensionSource = toggleSwitch.Tag().as<hstring>();
        _ViewModel.SetExtensionState(extensionSource, toggleSwitch.IsOn());
    }

    ExtensionsViewModel::ExtensionsViewModel(const Model::CascadiaSettings& settings) :
        _settings{ settings }
    {
        std::vector<IInspectable> fragmentExtensions;
        fragmentExtensions.reserve(settings.FragmentExtensions().Size());

        std::vector<IInspectable> profilesModified;
        std::vector<IInspectable> profilesAdded;
        std::vector<IInspectable> colorSchemesAdded;
        for (const auto&& fragExt : settings.FragmentExtensions())
        {
            fragmentExtensions.push_back(fragExt);

            for (const auto&& profile : fragExt.ModifiedProfilesView())
            {
                profilesModified.push_back(profile);
            }

            for (const auto&& profile : fragExt.NewProfilesView())
            {
                profilesAdded.push_back(profile);
            }

            for (const auto&& scheme : fragExt.ColorSchemesView())
            {
                colorSchemesAdded.push_back(scheme);
            }
        }

        _fragmentExtensions = single_threaded_observable_vector<IInspectable>(std::move(fragmentExtensions));
        _profilesModified = single_threaded_observable_vector<IInspectable>(std::move(profilesModified));
        _profilesAdded = single_threaded_observable_vector<IInspectable>(std::move(profilesAdded));
        _colorSchemesAdded = single_threaded_observable_vector<IInspectable>(std::move(colorSchemesAdded));
    }

    // Returns true if the extension is enabled, false otherwise
    bool ExtensionsViewModel::GetExtensionState(hstring extensionSource) const
    {
        if (const auto& disabledExtensions = _DisabledProfileSources())
        {
            uint32_t ignored;
            return !disabledExtensions.IndexOf(extensionSource, ignored);
        }
        // "disabledProfileSources" not defined --> all extensions are enabled
        return true;
    }

    // Enable/Disable an extension
    void ExtensionsViewModel::SetExtensionState(hstring extensionSource, bool enableExt)
    {
        // get the current status of the extension
        uint32_t idx;
        bool currentlyEnabled = true;
        const auto& disabledExtensions = _DisabledProfileSources();
        if (disabledExtensions)
        {
            currentlyEnabled = !disabledExtensions.IndexOf(extensionSource, idx);
        }

        // current status mismatches the desired status,
        // update the list of disabled extensions
        if (currentlyEnabled != enableExt)
        {
            // If we're disabling an extension and we don't have "disabledProfileSources" defined,
            // create it in the model directly
            if (!disabledExtensions && !enableExt)
            {
                std::vector<hstring> disabledProfileSources{ extensionSource };
                _settings.GlobalSettings().DisabledProfileSources(single_threaded_vector<hstring>(std::move(disabledProfileSources)));
                return;
            }

            // Update the list of disabled extensions
            if (enableExt)
            {
                disabledExtensions.RemoveAt(idx);
            }
            else
            {
                disabledExtensions.Append(extensionSource);
            }
        }
    }
}
