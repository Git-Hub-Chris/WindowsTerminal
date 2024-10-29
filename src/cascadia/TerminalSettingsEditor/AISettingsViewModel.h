// Copyright (c) Microsoft Corporation.
// Licensed under the MIT license.

#pragma once

#include "AISettingsViewModel.g.h"
#include "ViewModelHelpers.h"
#include "Utils.h"

namespace winrt::Microsoft::Terminal::Settings::Editor::implementation
{
    struct AISettingsViewModel : AISettingsViewModelT<AISettingsViewModel>, ViewModelHelper<AISettingsViewModel>
    {
    public:
        AISettingsViewModel(Model::CascadiaSettings settings);

        // DON'T YOU DARE ADD A `WINRT_CALLBACK(PropertyChanged` TO A CLASS DERIVED FROM ViewModelHelper. Do this instead:
        using ViewModelHelper<AISettingsViewModel>::PropertyChanged;

        bool AreAzureOpenAIKeyAndEndpointSet();
        winrt::hstring AzureOpenAIEndpoint();
        void AzureOpenAIEndpoint(winrt::hstring endpoint);
        winrt::hstring AzureOpenAIKey();
        void AzureOpenAIKey(winrt::hstring key);
        bool AzureOpenAIActive();
        void AzureOpenAIActive(bool active);
        bool AzureOpenAIAllowed() const noexcept;
        winrt::hstring AzureOpenAIStatus();

        bool IsOpenAIKeySet();
        winrt::hstring OpenAIKey();
        void OpenAIKey(winrt::hstring key);
        bool OpenAIActive();
        void OpenAIActive(bool active);
        bool OpenAIAllowed() const noexcept;
        winrt::hstring OpenAIStatus();

        bool AreGithubCopilotTokensSet();
        winrt::hstring GithubCopilotAuthMessage();
        void GithubCopilotAuthValues(winrt::hstring authValues);
        bool GithubCopilotActive();
        void GithubCopilotActive(bool active);
        bool GithubCopilotAllowed() const noexcept;
        bool IsTerminalPackaged();
        void InitiateGithubAuth_Click(const Windows::Foundation::IInspectable& sender, const Windows::UI::Xaml::RoutedEventArgs& e);
        til::typed_event<Windows::Foundation::IInspectable, Windows::Foundation::IInspectable> GithubAuthRequested;
        winrt::hstring GithubCopilotStatus();

    private:
        Model::CascadiaSettings _Settings;
        winrt::hstring _githubCopilotAuthMessage;

        winrt::hstring _getStatusHelper(const winrt::Microsoft::Terminal::Settings::Model::LLMProvider provider);

        winrt::Microsoft::Terminal::Settings::Editor::MainPage::GithubAuthCompleted_revoker _githubAuthCompleteRevoker;

        void _OnGithubAuthCompleted(const winrt::hstring& message);
    };
};

namespace winrt::Microsoft::Terminal::Settings::Editor::factory_implementation
{
    BASIC_FACTORY(AISettingsViewModel);
}
