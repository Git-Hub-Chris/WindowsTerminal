﻿// Copyright (c) Microsoft Corporation.
// Licensed under the MIT license.

#include "pch.h"
#include "MainPage.h"
#include "MainPage.g.cpp"
#include "Launch.h"
#include "Interaction.h"
#include "Rendering.h"
#include "Profiles.h"
#include "GlobalAppearance.h"
#include "ColorSchemes.h"
#include "Keybindings.h"

namespace winrt
{
    namespace MUX = Microsoft::UI::Xaml;
}

using namespace winrt::Windows::Foundation;
using namespace winrt::Windows::UI::Xaml;
using namespace winrt::Microsoft::Terminal::Settings::Model;
using namespace winrt::Windows::UI::Core;
using namespace winrt::Windows::System;

namespace winrt::Microsoft::Terminal::Settings::Editor::implementation
{
    winrt::Microsoft::Terminal::Settings::Model::CascadiaSettings MainPage::_settingsSource{ nullptr };

    MainPage::MainPage(CascadiaSettings settings)
    {
        InitializeComponent();

        // TODO GH#1564: When we actually connect this to Windows Terminal,
        //       this section will clone the active AppSettings
        MainPage::_settingsSource = settings;
        _settingsClone = nullptr;
    }

    CascadiaSettings MainPage::Settings()
    {
        return _settingsSource;
    }

    void MainPage::SettingsNav_Loaded(IInspectable const&, RoutedEventArgs const&)
    {
        auto initialItem = SettingsNav().MenuItems().GetAt(0);
        SettingsNav().SelectedItem(initialItem);

        // Manually navigate because setting the selected item programmatically doesn't trigger ItemInvoked.
        if (auto tag = initialItem.as<MUX::Controls::NavigationViewItem>().Tag())
        {
            Navigate(contentFrame(), unbox_value<hstring>(tag));
        }
    }

    void MainPage::SettingsNav_ItemInvoked(MUX::Controls::NavigationView const&, MUX::Controls::NavigationViewItemInvokedEventArgs const& args)
    {
        auto clickedItemContainer = args.InvokedItemContainer();

        if (clickedItemContainer != NULL)
        {
            Navigate(contentFrame(), unbox_value<hstring>(clickedItemContainer.Tag()));
        }
    }

    void MainPage::SettingsNav_BackRequested(MUX::Controls::NavigationView const&, MUX::Controls::NavigationViewBackRequestedEventArgs const& /*args*/)
    {
        On_BackRequested();
    }

    bool MainPage::On_BackRequested()
    {
        if (!contentFrame().CanGoBack())
        {
            return false;
        }

        if (SettingsNav().IsPaneOpen() &&
            (SettingsNav().DisplayMode() == MUX::Controls::NavigationViewDisplayMode(1) ||
             SettingsNav().DisplayMode() == MUX::Controls::NavigationViewDisplayMode(0)))
        {
            return false;
        }

        contentFrame().GoBack();
        return true;
    }

    void MainPage::Navigate(Controls::Frame contentFrame, hstring clickedItemTag)
    {
        const hstring launchSubpage = L"Launch_Nav";
        const hstring interactionSubpage = L"Interaction_Nav";
        const hstring renderingSubpage = L"Rendering_Nav";

        const hstring globalProfileSubpage = L"GlobalProfile_Nav";

        const hstring colorSchemesPage = L"ColorSchemes_Nav";
        const hstring globalAppearancePage = L"GlobalAppearance_Nav";

        if (clickedItemTag == launchSubpage)
        {
            contentFrame.Navigate(xaml_typename<Editor::Launch>());
        }
        else if (clickedItemTag == interactionSubpage)
        {
            contentFrame.Navigate(xaml_typename<Editor::Interaction>());
        }
        else if (clickedItemTag == renderingSubpage)
        {
            contentFrame.Navigate(xaml_typename<Editor::Rendering>());
        }
        else if (clickedItemTag == globalProfileSubpage)
        {
            contentFrame.Navigate(xaml_typename<Editor::Profiles>());
        }
        else if (clickedItemTag == colorSchemesPage)
        {
            contentFrame.Navigate(xaml_typename<Editor::ColorSchemes>());
        }
        else if (clickedItemTag == globalAppearancePage)
        {
            contentFrame.Navigate(xaml_typename<Editor::GlobalAppearance>());
        }
    }

    void MainPage::OpenJsonTapped(IInspectable const& /*sender*/, Windows::UI::Xaml::Input::TappedRoutedEventArgs const& /*args*/)
    {
        const CoreWindow window = CoreWindow::GetForCurrentThread();
        const auto rAltState = window.GetKeyState(VirtualKey::RightMenu);
        const auto lAltState = window.GetKeyState(VirtualKey::LeftMenu);
        const bool altPressed = WI_IsFlagSet(lAltState, CoreVirtualKeyStates::Down) ||
                                WI_IsFlagSet(rAltState, CoreVirtualKeyStates::Down);

        const auto target = altPressed ? SettingsTarget::DefaultsFile : SettingsTarget::SettingsFile;
        _OpenJsonHandlers(nullptr, target);
    }

    void MainPage::OpenJsonKeyDown(IInspectable const& /*sender*/, Windows::UI::Xaml::Input::KeyRoutedEventArgs const& args)
    {
        if (args.Key() == VirtualKey::Enter || args.Key() == VirtualKey::Space)
        {
            const auto target = args.KeyStatus().IsMenuKeyDown ? SettingsTarget::DefaultsFile : SettingsTarget::SettingsFile;
            _OpenJsonHandlers(nullptr, target);
        }
    }
}
