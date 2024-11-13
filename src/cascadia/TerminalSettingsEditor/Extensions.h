// Copyright (c) Microsoft Corporation.
// Licensed under the MIT license.

#pragma once

#include "Extensions.g.h"
#include "ExtensionsViewModel.g.h"
#include "ViewModelHelpers.h"
#include "Utils.h"

namespace winrt::Microsoft::Terminal::Settings::Editor::implementation
{
    struct Extensions : public HasScrollViewer<Extensions>, ExtensionsT<Extensions>
    {
    public:
        Extensions();

        void OnNavigatedTo(const Windows::UI::Xaml::Navigation::NavigationEventArgs& e);
        void ExtensionLoaded(const Windows::Foundation::IInspectable& sender, const Windows::UI::Xaml::RoutedEventArgs& args);
        void ExtensionToggled(const Windows::Foundation::IInspectable& sender, const Windows::UI::Xaml::RoutedEventArgs& args);

        WINRT_PROPERTY(Editor::ExtensionsViewModel, ViewModel, nullptr);
    };

    struct ExtensionsViewModel : ExtensionsViewModelT<ExtensionsViewModel>, ViewModelHelper<ExtensionsViewModel>
    {
    public:
        ExtensionsViewModel(const Model::CascadiaSettings& settings);

        // Views
        Windows::Foundation::Collections::IVectorView<IInspectable> FragmentExtensions() const noexcept { return _fragmentExtensions.GetView(); }
        Windows::Foundation::Collections::IVectorView<IInspectable> ProfilesModified() const noexcept { return _profilesModified.GetView(); }
        Windows::Foundation::Collections::IVectorView<IInspectable> ProfilesAdded() const noexcept { return _profilesAdded.GetView(); }
        Windows::Foundation::Collections::IVectorView<IInspectable> ColorSchemesAdded() const noexcept { return _colorSchemesAdded.GetView(); }

        bool GetExtensionState(hstring extensionSource) const;
        void SetExtensionState(hstring extensionSource, bool enableExt);

    private:
        Model::CascadiaSettings _settings;
        Windows::Foundation::Collections::IVector<IInspectable> _fragmentExtensions;
        Windows::Foundation::Collections::IVector<IInspectable> _profilesModified;
        Windows::Foundation::Collections::IVector<IInspectable> _profilesAdded;
        Windows::Foundation::Collections::IVector<IInspectable> _colorSchemesAdded;

        Windows::Foundation::Collections::IVector<hstring> _DisabledProfileSources() const noexcept { return _settings.GlobalSettings().DisabledProfileSources(); }
    };
};

namespace winrt::Microsoft::Terminal::Settings::Editor::factory_implementation
{
    BASIC_FACTORY(Extensions);
    BASIC_FACTORY(ExtensionsViewModel);
}
